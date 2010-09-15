#include <QDebug>
#include "SearchWorker.h"
#include "WorkerThreadController.h"

namespace FileDetector {

SearchWorker::SearchWorker(const Search * search, WorkerThreadController *c) :
    Search(search),
    _threadController(c),
    _started(false)
{
    qRegisterMetaType<FileDetector::SearchWorker*>("FileDetector::SearchWorker*");
    // pass through the worker signals to the "real" search object's signals
    connect(this, SIGNAL(searchError(FileDetector::Search::Error)), search, SIGNAL(searchError(FileDetector::Search::Error)));
    connect(this, SIGNAL(oneTimeSearchStarted()), search, SIGNAL(oneTimeSearchStarted()));
    connect(this, SIGNAL(oneTimeSearchCompleted()), search, SIGNAL(oneTimeSearchCompleted()));
    connect(this, SIGNAL(continuousSearchStarted()), search, SIGNAL(continuousSearchStarted()));
    connect(this, SIGNAL(continuousSearchVerified()), search, SIGNAL(continuousSearchVerified()));
    connect(this, SIGNAL(progress(int, int)), search, SIGNAL(progress(int, int)));
    connect(this, SIGNAL(filesAdded(const QList<QUrl> &)), search, SIGNAL(filesAdded(const QList<QUrl> &)));
    connect(this, SIGNAL(filesRemoved(const QList<QUrl> &)), search, SIGNAL(filesRemoved(const QList<QUrl> &)));
}




void SearchWorker::initialize() {
    foreach(FileType ft, _parameters.fileTypes) {
        _suffixes.append("*." + ft.suffix); 
    }

    // ::work() will process this dir, even if Recurse is not set in search criteria.
    //     (recurse will simply not look into this directory's sub-directories!)
    _todo.append( QFileInfo(_parameters.directory.absolutePath()) );

    emit searchStarted();
}


bool SearchWorker::work() {
    if (_todo.isEmpty()) {
        qDebug("todo empty");
        emit searchCompleted();
        return false;
    }

    QFileInfo fi = _todo.takeFirst();

    // the file could have been removed since we first saw it
    if (!fi.exists()) { return true; }

    // if this is a directory, find all matching file-extensions
    //    (always process directories, but filter out sub-directories based on recurse- search criteria.
    //     this allows the doWork() to add a directory to the todo list to start the job)
    if (fi.isDir()) {
        QDir dir = fi.absoluteFilePath(); // use file path, other we get the parent directory
        QDir::Filters filters = QDir::Files|QDir::NoDotAndDotDot;
        if ( _parameters.searchCriteria.testFlag(FileDetector::Recursive     )) { filters |= QDir::AllDirs      ; }
        if (!_parameters.searchCriteria.testFlag(FileDetector::IgnoreCase    )) { filters |= QDir::CaseSensitive; }
        if (!_parameters.searchCriteria.testFlag(FileDetector::FollowSymLinks)) { filters |= QDir::NoSymLinks   ; }
        QFileInfoList files = dir.entryInfoList(_suffixes, filters);
        _todo.append( files );
        return true;
    }


    // find search key that matches to check mime-type and read/write requirements
    QString ext = fi.suffix();
    QString completeExt = fi.completeSuffix();
    QString mimeType;

    foreach(const FileType &fileType, _parameters.fileTypes) {
        //qDebug() << "File:" << fi.fileName() << ext << completeExt << "required suffix:" << fileType.suffix;
        if (!fileType.suffix.isEmpty() && !(fileType.suffix == ext || fileType.suffix == completeExt)) {
            // doesn't match, so go to next file-type
            continue;
        }
        //qDebug() << "matches suffix test";

        // if fails fileCriteria tests, it will never pass, even for other file-types
        if (_parameters.fileCriteria.testFlag(FileDetector::FileReadable)) { if (!fi.isReadable()) { break; } }
        if (_parameters.fileCriteria.testFlag(FileDetector::FileWritable)) { if (!fi.isWritable()) { break; } }

        //qDebug() << "matches filecriteria  test";
        if (fileType.mimeTypes.count()) {
            // only determine mime-type of file once
            if (mimeType.isEmpty()) {
                mimeType = QString(magic_file(_threadController->magicCookie(), fi.absoluteFilePath().toAscii()));
                if (mimeType.isEmpty()) { mimeType = "unknown"; }
            }

            // continue into next file-info: mime-type may match there
            if (!fileType.mimeTypes.contains(mimeType)) { continue; }
            // TODO: check globing
        }

        //qDebug() << "matches mimetype test";

        // okay, passes all tests for this searchKey. Only needs to match one key
        _found.append( QUrl("file://" + fi.absoluteFilePath()) );
        emit filesAdded( QList<QUrl>() << QUrl("file://" + fi.absoluteFilePath()) );
        return true;
    }

    // weird, none of the keys actually match. They should if the are in the todo list.
    // But it could be because the original SearchKey was removed after the directory was listed
    return true;
}

}
