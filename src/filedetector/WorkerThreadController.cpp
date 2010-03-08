#include "WorkDispatcher.h"
#include "WorkerThreadController.h"
#include "SearchWorker.h"

namespace FileDetector {

WorkerThreadController::WorkerThreadController(QObject *parent) : 
    QThread(parent),
    _dispatcher(NULL)
{}

WorkerThreadController::~WorkerThreadController() {
    if (isRunning()) {
        quit();
        wait(); 
    } 
}

void WorkerThreadController::addSearchWorker(SearchWorker *searchWorker) {
    if (!_dispatcher) {
        emit searchWorker->searchError(Search::ErrorWorkerThreadNotRunning);
        return;
    }

    searchWorker->moveToThread(this);
    const bool b = QMetaObject::invokeMethod(_dispatcher, "addSearchWorker", Qt::QueuedConnection, Q_ARG(FileDetector::SearchWorker*, searchWorker));
    if (!b) {
        emit searchWorker->searchError(Search::ErrorAddingSearchToWorkerThread);
        return;
    }
}


void WorkerThreadController::run() {
    WorkDispatcher dispatcher;
    _dispatcher = &dispatcher;
    exec();
    _dispatcher = NULL;
}


}
