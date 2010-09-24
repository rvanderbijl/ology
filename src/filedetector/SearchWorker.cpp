#include <QDebug>
#include "SearchWorker.h"
#include "INotifyWorker.h"
#include "WorkerThreadController.h"

uint qHash(const QUrl &url) { 
    return qHash(url.toString());
}

namespace FileDetector {

SearchWorker::SearchWorker(const Search * search, WorkerThreadController *c) :
    QObject(NULL), // null becasue a move-to-thread is done on this object
    _searchParameters(search->searchParameters()),
    _threadController(c),
    _started(false),
    _iworker(new INotifyWorker(this))
{
    qRegisterMetaType<FileDetector::SearchWorker*>("FileDetector::SearchWorker*");
    // pass through the worker signals to the "real" search object's signals
    connect(this, SIGNAL(searchError(FileDetector::Search::Error)), search, SIGNAL(searchError(FileDetector::Search::Error)));
    connect(this, SIGNAL(progress(int, int)), search, SIGNAL(progress(int, int)));
    connect(this, SIGNAL(filesAdded(const QList<QUrl> &)), search, SIGNAL(filesAdded(const QList<QUrl> &)));
    connect(this, SIGNAL(filesRemoved(const QList<QUrl> &)), search, SIGNAL(filesRemoved(const QList<QUrl> &)));


    connect(_iworker, SIGNAL(directoryAdded(const QDir &)), SLOT(onDirectoryAdded(const QDir &)));
    connect(_iworker, SIGNAL(fileAdded(const QFileInfo &)), SLOT(onFileAdded(const QFileInfo &)));
    connect(_iworker, SIGNAL(fileChanged(const QFileInfo &)), SLOT(onFileChanged(const QFileInfo &)));
    connect(_iworker, SIGNAL(fileRemoved(const QFileInfo &)), SLOT(onFileRemoved(const QFileInfo &)));
}




void SearchWorker::initialize() {
    foreach(FileType ft, _searchParameters.fileTypes) {
        _suffixes.append("*." + ft.suffix); 
    }

    // ::work() will process this dir, even if Recurse is not set in search criteria.
    //     (recurse will simply not look into this directory's sub-directories!)
    _todo.append( QFileInfo(_searchParameters.directory.absolutePath()) );

    emit searchStarted();
}


bool SearchWorker::work() {
    if (_todo.isEmpty()) {
        qDebug("SearchWorker: todo empty");
        emit searchCompleted();
        return false;
    }

    QFileInfo fi = _todo.takeFirst();
    qDebug() << "SearchWorker: checking:" << fi.absoluteFilePath();

    const QUrl url = QUrl("file://" + fi.absoluteFilePath());
    bool alreadyFound = _found.contains(url);


    // the file could have been removed since we first saw it
    if (!fi.exists()) {
        if (alreadyFound) {
            _found.remove(url);
            emit filesRemoved(QList<QUrl>() << url);
        }
        return true; 
    }

    // if this is a directory, find all matching file-extensions
    //    (always process directories, but filter out sub-directories based on recurse- search criteria.
    //     this allows the doWork() to add a directory to the todo list to start the job)
    if (fi.isDir()) {
        QDir dir = fi.absoluteFilePath(); // use file path, other we get the parent directory
        QDir::Filters filters = QDir::Files|QDir::NoDotAndDotDot;
        if ( _searchParameters.searchCriteria.testFlag(FileDetector::Recursive     )) { filters |= QDir::AllDirs      ; }
        if (!_searchParameters.searchCriteria.testFlag(FileDetector::IgnoreCase    )) { filters |= QDir::CaseSensitive; }
        if (!_searchParameters.searchCriteria.testFlag(FileDetector::FollowSymLinks)) { filters |= QDir::NoSymLinks   ; }
        QFileInfoList files = dir.entryInfoList(_suffixes, filters);
        _todo.append( files );
        _iworker->addDirectory(dir);
        return true;
    }

    if (!alreadyFound) {
        // only watch the file once
        _iworker->addFile(fi);
    }

    // find search key that matches to check mime-type and read/write requirements
    QString ext = fi.suffix();
    QString completeExt = fi.completeSuffix();
    QString mimeType;

    foreach(const FileType &fileType, _searchParameters.fileTypes) {
        //qDebug() << "File:" << fi.fileName() << ext << completeExt << "required suffix:" << fileType.suffix;
        if (!fileType.suffix.isEmpty() && !(fileType.suffix == ext || fileType.suffix == completeExt)) {
            // doesn't match, so go to next file-type
            continue;
        }
        //qDebug() << "matches suffix test";

        // if fails fileCriteria tests, it will never pass, even for other file-types
        if (_searchParameters.fileCriteria.testFlag(FileDetector::FileReadable)) { if (!fi.isReadable()) { break; } }
        if (_searchParameters.fileCriteria.testFlag(FileDetector::FileWritable)) { if (!fi.isWritable()) { break; } }

        //qDebug() << "matches filecriteria  test";
        if (fileType.mimeTypes.count()) {
            // only determine mime-type of file once
            if (mimeType.isEmpty()) {
                mimeType = QString(magic_file(_magicCookie, fi.absoluteFilePath().toAscii()));
                if (mimeType.isEmpty()) { mimeType = "unknown"; }
            }

            // continue into next file-info: mime-type may match there
            if (!fileType.mimeTypes.contains(mimeType)) { 
                if (alreadyFound) {
                    // no longer matches search-parameters, emit fileRemoved
                    _found.remove(url);
                    emit filesRemoved(QList<QUrl>() << url);
                }
                continue; 
            }
            // TODO: check globing
        }

        //qDebug() << "matches mimetype test";

        // okay, passes all tests for this searchKey. Only needs to match one key
        if (!alreadyFound) {
            _found.insert( url );
            emit filesAdded( QList<QUrl>() << url );
        }
        return true;
    }

    // weird, none of the keys actually match. They should if the are in the todo list.
    // But it could be because the original SearchKey was removed after the directory was listed
    return true;
}





void SearchWorker::onDirectoryAdded(const QDir &newDirectory) {
    // if search is not recursive, we never care about new directories
    if (_searchParameters.searchCriteria.testFlag(FileDetector::Recursive)) {
        _todo.append(QFileInfo(newDirectory.absolutePath()));
        emit moreWorkFound();
    }
}

void SearchWorker::onFileAdded(const QFileInfo &file) {
    qDebug() << "Detected file added:" << file.absoluteFilePath();
    _todo.append(file);
    emit moreWorkFound();
}

void SearchWorker::onFileChanged(const QFileInfo &file) {
    qDebug() << "Detected file changed:" << file.absoluteFilePath();
    _todo.append(file);
    emit moreWorkFound();
}

void SearchWorker::onFileRemoved(const QFileInfo &file) {
    qDebug() << "Detected file removed:" << file.absoluteFilePath();
    const QUrl url = QUrl("file://" + file.absoluteFilePath());
    if (_found.remove(url)) {
        emit filesRemoved(QList<QUrl>() << url);
    }
}


}
