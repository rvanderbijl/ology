#ifndef FILEDETCTOR_INOTIFY_WORKER
#define FILEDETCTOR_INOTIFY_WORKER

#include <QObject>
#include <QDir>
#include <QFileInfo>
#include <QHash>

extern "C" {
#include <sys/inotify.h>
}

namespace FileDetector {

class INotifyWorker : public QObject {
    Q_OBJECT
public:
    INotifyWorker(QObject *parent);

public:
    void addDirectory(const QDir &dir);
    void addFile(const QFileInfo &file);

signals:
    void directoryAdded(const QDir &dir);
    void fileAdded(const QFileInfo &file);
    void fileRemoved(const QFileInfo &file);
    void fileChanged(const QFileInfo &file);

private slots:
    void readINotify(int fd);

private:
    int _fd;
    QHash<int, QString> _watchDescriptors;
    bool _reading;
};


}

#endif
