#ifndef FILEDETECTOR_SEARCH_WORKER
#define FILEDETECTOR_SEARCH_WORKER

#include <QObject>
#include "SearchParameters.h"
#include "Search.h"

namespace FileDetector {

class SearchWorker : public Search {
public:
    SearchWorker(const Search * search);

public:
    // make this public accessile 
    void searchError(Search::Error error) { Search::searchError(error); }

    void initialize();
    void work();

signals:
    void searchStarted();
    void searchCompleted();

private:
    bool _started;
    QStringList _suffixes;
    QList<QUrl> _found;
    QFileInfoList _todo;
};




}

#endif
