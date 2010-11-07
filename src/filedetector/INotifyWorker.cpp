#include <QCoreApplication>
#include <QThread>
#include <QDebug>
#include <QFile>
#include <QTimer>
#include <QSocketNotifier>
#include "INotifyWorker.h"

extern "C" {
#include <stdlib.h>
#include <errno.h>
#include <sys/ioctl.h>
}



namespace FileDetector {

INotifyWorker::INotifyWorker(QObject *parent) :
    QObject(parent),
    _fd(-1),
    _reading(false)
{
    _fd = inotify_init();
    if (_fd < 0) {
        qDebug() << "INotify error!" << _fd;;
        return;
    }

    QSocketNotifier *n = new QSocketNotifier(_fd, QSocketNotifier::Read, this);
    connect(n, SIGNAL(activated(int)), SLOT(readINotify(int)));
}



void INotifyWorker::addDirectory(const QDir &dir) {
    QByteArray file = QFile::encodeName(dir.absolutePath());
    if (!file.endsWith("/")) { file += "/"; }

    int wd = inotify_add_watch(_fd, file, IN_CREATE | IN_DELETE_SELF | IN_MOVE_SELF | IN_ONLYDIR );
    if (wd < 0) {
        qDebug() << "INotify add directory error!" << wd;
        return;
    }

    qDebug() << "New wd:" << wd << "directory:" << file;
    _watchDescriptors[wd] = dir.absolutePath();
}

void INotifyWorker::addFile(const QFileInfo &file) {
    const QString fileName = file.absoluteFilePath();
    int wd = inotify_add_watch(_fd, QFile::encodeName(fileName), IN_DELETE_SELF | IN_MOVE_SELF | IN_ATTRIB | IN_CLOSE_WRITE);
    if (wd < 0) {
        qDebug() << "INotify add file error!" << wd << "file:" << fileName;
        return;
    }

    qDebug() << "New wd:" << wd << "file:" << fileName;
    _watchDescriptors[wd] = fileName;
}

void INotifyWorker::readINotify(int fd) {
    Q_ASSERT(QThread::currentThread() != qApp->thread());

    // code adapted from qfileSystemWatcher_inotify:
    if (_reading) {
        qDebug() << "readINotify: recursive call!";
        return;
    }


    int buffSize = 0;
    ioctl(fd, FIONREAD, (char *) &buffSize); // how much is there to read?

    if (buffSize == 0) {
        qDebug() << "Nothing to read";
        return;
    }

    _reading = true;
    qDebug() << "\n\n\n===============================================\n"
                "readINotify: buffer is:" << buffSize;

    QByteArray buffer;
    buffer.reserve(buffSize);
    buffSize = read(fd, buffer.data(), buffSize); 
    const char *at = buffer.data();
    const char * const end = at + buffSize;
    qDebug() << "readINotify: final bufferSize is:" << buffSize 
             << "\n\t at is:" << qPrintable(QString().sprintf("0x%012x", (qint64)at)) 
             << "\n\tend is:" << qPrintable(QString().sprintf("0x%012x", (qint64)end));

    while (at < end) {
        const inotify_event *event = reinterpret_cast<const inotify_event *>(at);

        QStringList list;
        if ((event->mask & IN_ACCESS) != 0) { list << "IN_ACCESS"; }
        if ((event->mask & IN_MODIFY) != 0) { list << "IN_MODIFY"; }
        if ((event->mask & IN_ATTRIB) != 0) { list << "IN_ATTRIB"; }
        if ((event->mask & IN_CLOSE_WRITE) != 0) { list << "IN_CLOSE_WRITE"; }
        if ((event->mask & IN_CLOSE_NOWRITE) != 0) { list << "IN_CLOSE_NOWRITE"; }
        if ((event->mask & IN_OPEN) != 0) { list << "IN_OPEN"; }
        if ((event->mask & IN_MOVED_FROM) != 0) { list << "IN_MOVED_FROM"; }
        if ((event->mask & IN_MOVED_TO) != 0) { list << "IN_MOVED_TO"; }
        if ((event->mask & IN_CREATE) != 0) { list << "IN_CREATE"; }
        if ((event->mask & IN_DELETE) != 0) { list << "IN_DELETE"; }
        if ((event->mask & IN_DELETE_SELF) != 0) { list << "IN_DELETE_SELF"; }
        if ((event->mask & IN_MOVE_SELF) != 0) { list << "IN_MOVE_SELF"; }
        if ((event->mask & IN_UNMOUNT) != 0) { list << "IN_UNMOUNT"; }
        if ((event->mask & IN_Q_OVERFLOW) != 0) { list << "IN_Q_OVERFLOW"; }
        if ((event->mask & IN_IGNORED) != 0) { list << "IN_IGNORED"; }
        if ((event->mask & IN_ONLYDIR) != 0) { list << "IN_ONLYDIR"; }
        if ((event->mask & IN_DONT_FOLLOW) != 0) { list << "IN_DONT_FOLLOW"; }
        if ((event->mask & IN_MASK_ADD) != 0) { list << "IN_MASK_ADD"; }
        if ((event->mask & IN_ISDIR) != 0) { list << "IN_ISDIR"; }
        if ((event->mask & IN_ONESHOT) != 0) { list << "IN_ONESHOT"; }
        qDebug() << "\nEvent:" 
                 << "\n\twd" << event->wd
                 << "\n\tmask:" << qPrintable("0x" + QString::number(event->mask, 16))
                 << "\n\tcookie:" << event->cookie
                 << "\n\tlen:" << event->len
                 << "\n\tname:" << (event->len ? event->name : "")
                 << "\n\tflags found in mask:" << qPrintable(list.join(" "));


        const int wd = event->wd;
        if (wd <= 0) {
            qDebug() << "INotify event wd was negative!" << wd;
            at += sizeof(inotify_event) + event->len;
            continue;
        }

        if (!_watchDescriptors.contains(wd) && ((event->mask & IN_IGNORED) == 0)) {
            qDebug() << "INotify event wd was not known!" << wd << event->mask << "name:" << (event->len ? QString::fromLocal8Bit(event->name) : "n/a");
            at += sizeof(inotify_event) + event->len;
            continue;
        }

        const QString path = _watchDescriptors[wd];
        qDebug() << "INotify event: wd:" << wd << "mask:" << event->mask << "path:" << path;

        if ((event->mask & (IN_DELETE | IN_DELETE_SELF | IN_MOVE_SELF | IN_UNMOUNT)) != 0) {
            _watchDescriptors.remove(wd);
            inotify_rm_watch(fd, wd);

            QString removed = path;
            if ((event->mask & IN_DELETE_SELF) == 0) {
                removed += QString(event->name);
            }

            if ((event->mask & IN_ISDIR) != 0) {
                qDebug() << "INotify directory changed/moved" << removed;
                //emit directoryRemoved(path);
            } else {
                qDebug() << "INotify file deleted/moved" << removed;
                emit fileRemoved(removed);
            }
        } else if ((event->mask & IN_CREATE) != 0) {
            QString fileName = QString::fromLocal8Bit(event->name);
            if ((event->mask & IN_ISDIR) != 0) {
                emit directoryAdded(QDir(path).absoluteFilePath(fileName));
            } else {
                emit fileAdded(QDir(path).absoluteFilePath(fileName));
            }
        } 
        else if ((event->mask & (IN_CLOSE_WRITE | IN_MODIFY | IN_ATTRIB)) != 0)  {
            emit fileChanged(path);
        } else if ((event->mask & IN_IGNORED) != 0)  {
            _watchDescriptors.remove(wd);
            inotify_rm_watch(fd, wd);
        } else {
            qDebug() << "Unknown/unexpected event:" << event->wd << "mask" << event->mask << "name:?" << (event->len ? QString::fromLocal8Bit(event->name) : "n/a");
        }

        // next inotify event
        at += sizeof(inotify_event) + event->len;
        qDebug() << "looking for next event: event size:" << sizeof(inotify_event)
                 << "\n\t at is:" << qPrintable(QString().sprintf("0x%012x", (qint64)at)) 
                 << "\n\tend is:" << qPrintable(QString().sprintf("0x%012x", (qint64)end));
    }
    _reading = false;
}


}
