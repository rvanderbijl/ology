#ifndef FILEDETECTOR_SEARCH_WORKER
#define FILEDETECTOR_SEARCH_WORKER

#include <QObject>
#include <QMetaType>
#include <QSet>
#include <QUrl>
#include "SearchParameters.h"
#include "Search.h"

extern "C" {
#include <magic.h>
}

namespace FileDetector {
    class WorkerThreadController;
    class INotifyWorker;

class SearchWorker : public QObject {
    Q_OBJECT
public:
    SearchWorker(const Search * search, WorkerThreadController *controller);

public:
    void initialize();
    bool work();

    // work-dispatcher's magic-cookie
    void setMagicCookie(magic_t cookie) {
        _magicCookie = cookie;
    }

signals:
    void moreWorkFound();
    void searchStarted();
    void searchCompleted();

    void searchError(FileDetector::Search::Error error);
    void progress(int i, int total);
    void filesAdded(const QList<QUrl> &files);
    void filesRemoved(const QList<QUrl> &files);

private slots:
    void onDirectoryAdded(const QDir &newDirectory);
    void onFileAdded(const QFileInfo &file);
    void onFileChanged(const QFileInfo &file);
    void onFileRemoved(const QFileInfo &file);

private:
    friend class WorkerThreadController;
    SearchParameters _searchParameters;
    WorkerThreadController *_threadController;
    magic_t _magicCookie;
    bool _started;
    QStringList _suffixes;
    QSet<QUrl> _found;
    QFileInfoList _todo;
    INotifyWorker *_iworker;
};


}


Q_DECLARE_METATYPE(FileDetector::SearchWorker*);

#endif
