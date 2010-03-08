#include "SearchWorker.h"

namespace FileDetector {

SearchWorker::SearchWorker(const Search * search) :
    Search(search),
    _started(false)
{
    // pass through the worker signals to the "real" search object's signals
    connect(this, SIGNAL(searchError(FileDetector::Search::Error);), search, SIGNAL(searchError(FileDetector::Search::Error);));
    connect(this, SIGNAL(oneTimeSearchStarted();), search, SIGNAL(oneTimeSearchStarted();));
    connect(this, SIGNAL(oneTimeSearchCompleted();), search, SIGNAL(oneTimeSearchCompleted();));
    connect(this, SIGNAL(continuousSearchStarted();), search, SIGNAL(continuousSearchStarted();));
    connect(this, SIGNAL(continuousSearchVerified();), search, SIGNAL(continuousSearchVerified();));
    connect(this, SIGNAL(progress(int, int);), search, SIGNAL(progress(int, int);));
    connect(this, SIGNAL(filesAdded(const QString &, const QList<QUrl> &);), search, SIGNAL(filesAdded(const QString &, const QList<QUrl> &);));
    connect(this, SIGNAL(filesRemoved(const QString &, const QList<QUrl> &);), search, SIGNAL(filesRemoved(const QString &, const QList<QUrl> &);));
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


void SearchWorker::work() {
    if (_todo.isEmpty()) {
        emit searchCompleted();
        return;
    }

    QFileInfo fi = _todo.takeFirst();

    // the file could have been removed since we first saw it
    if (!fi.exists()) { return; }

    // if this is a directory, find all matching file-extensions
    //    (always process directories, but filter out sub-directories based on recurse- search criteria.
    //     this allows the doWork() to add a directory to the todo list to start the job)
    if (fi.isDir()) {
        QDir dir = fi.absolutePath();
        QDir::Filters filters = QDir::Files;
        if ( _parameters.searchCriteria.testFlag(FileDetector::Recursive     )) { filters |= QDir::AllDirs      ; }
        if (!_parameters.searchCriteria.testFlag(FileDetector::IgnoreCase    )) { filters |= QDir::CaseSensitive; }
        if (!_parameters.searchCriteria.testFlag(FileDetector::FollowSymLinks)) { filters |= QDir::NoSymLinks   ; }
        _todo.append( dir.entryInfoList(_suffixes, filters) );
        return;
    }


    // find search key that matches to check mime-type and read/write requirements
    QString ext = fi.suffix();
    QString completeExt = fi.completeSuffix();
    QString mimeType;

    foreach(const FileType &fileType, _parameters.fileTypes) {
        if (!fileType.suffix.isEmpty() || !(fileType.suffix == ext || fileType.suffix == completeExt)) {
            // doesn't match, so go to next file-type
            continue;
        }

        // if fails fileCriteria tests, it will never pass, even for other file-types
        if (_parameters.fileCriteria.testFlag(FileDetector::FileReadable)) { if (!fi.isReadable()) { break; } }
        if (_parameters.fileCriteria.testFlag(FileDetector::FileWritable)) { if (!fi.isWritable()) { break; } }

        if (fileType.mimeTypes.count()) {
            // only determine mime-type of file once
            if (mimeType.isEmpty()) {
                mimeType = QString(magic_file(_magicCookie, fi.absoluteFilePath().toAscii()));
                if (mimeType.isEmpty()) { mimeType = "unknown"; }
            }

            // continue into next file-info: mime-type may match there
            if (!fileType.mimeTypes.contains(mimeType)) { continue; }
            // TODO: check globing
        }

        // okay, passes all tests for this searchKey. Only needs to match one key
        _found.append( QUrl("file://" + fi.absoluteFilePath()) );
        return;
    }

    // weird, none of the keys actually match. They should if the are in the todo list.
    // But it could be because the original SearchKey was removed after the directory was listed
}

}
