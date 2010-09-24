#include "Search.h"
#include "SearchWorker.h"
#include "WorkerThreadController.h"

namespace FileDetector {

Search::Search(const SearchParameters & parameters, WorkerThreadController * wtc, QObject *parent) :
    QObject(parent),
    _parameters(parameters),
    _threadController(wtc),
    _worker(NULL)
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

Search::~Search() {
    if (_worker) {
        stopSearch();
    }
}

QString Search::errorString(Error code) {
    switch(code) {
        case ErrorWorkerThreadNotRunning: return tr("FileDetector worker thread is not running");
        case ErrorAddingSearchToWorkerThread: return tr("Error adding the search request to the worker thread");
    }

    return tr("Unknown error");
}

void Search::startSearch() {
    _worker = new SearchWorker(this, _threadController);
    _threadController->addSearchWorker(_worker);
}

void Search::stopSearch() {
    if (_worker) {
        _threadController->removeSearchWorker(_worker);
        delete _worker;
        _worker = NULL;
    }
}


}
