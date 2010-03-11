#include "Search.h"
#include "SearchWorker.h"
#include "WorkerThreadController.h"

namespace FileDetector {

Search::Search(const SearchParameters & parameters, QObject *parent) :
    QObject(parent),
    _parameters(parameters)
{
    // clean up extensions (make sure none start with "." or "*.")
    QList<FileType> temp;
    foreach(FileType ft, _parameters.fileTypes) {
        if      (ft.suffix.isEmpty()) { continue; }
        if      (ft.suffix.startsWith("*.")) { ft.suffix = ft.suffix.mid(2); }
        else if (ft.suffix.startsWith( ".")) { ft.suffix = ft.suffix.mid(1); }
        temp << ft;
    }

    _parameters.fileTypes = temp;
}

Search::Search(const Search* search) :
    QObject(NULL),
    _parameters(search->searchParameters())
{
}


QString Search::errorString(Error code) {
    switch(code) {
        case ErrorWorkerThreadNotRunning: return tr("FileDetector worker thread is not running");
        case ErrorAddingSearchToWorkerThread: return tr("Error adding the search request to the worker thread");
    }

    return tr("Unknown error");
}


void Search::startOneTimeSearch(WorkerThreadController *threadController) {
    threadController->addSearchWorker(new SearchWorker(this, threadController));
}

void Search::startContinuousSearch() {
}


}
