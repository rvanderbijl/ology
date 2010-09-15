#ifndef FILEDETECTOR_SEARCH_WORKER
#define FILEDETECTOR_SEARCH_WORKER

#include <QObject>
#include <QMetaType>
#include "SearchParameters.h"
#include "Search.h"

namespace FileDetector {
    class WorkerThreadController;

class SearchWorker : public Search {
    Q_OBJECT
public:
    SearchWorker(const Search * search, WorkerThreadController *controller);

public:
    // make this public accessile 
    void searchError(Search::Error error) { Search::searchError(error); }

    void initialize();
    bool work();

signals:
    void searchStarted();
    void searchCompleted();

private:
    WorkerThreadController *_threadController;
    bool _started;
    QStringList _suffixes;
    QList<QUrl> _found;
    QFileInfoList _todo;
};




}

Q_DECLARE_METATYPE(FileDetector::SearchWorker*);

#endif
