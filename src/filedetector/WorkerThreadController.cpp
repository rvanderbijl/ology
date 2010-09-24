#include <QDebug>

#include "WorkDispatcher.h"
#include "WorkerThreadController.h"
#include "SearchWorker.h"

namespace FileDetector {

WorkerThreadController::WorkerThreadController(QObject *parent) : 
    QThread(parent),
    _dispatcher(NULL)
{
    qRegisterMetaType< QList<QUrl> >("QList<QUrl>");
}

WorkerThreadController::~WorkerThreadController() {
    if (isRunning()) {
        quit();
        wait(); 
    } 
}

void WorkerThreadController::addSearchWorker(SearchWorker *searchWorker) {
    if (!_dispatcher) {
        qDebug("thread not running");
        emit searchWorker->searchError(Search::ErrorWorkerThreadNotRunning);
        return;
    }

    searchWorker->moveToThread(this);
    const bool b = QMetaObject::invokeMethod(_dispatcher, "addSearchWorker", Qt::QueuedConnection, Q_ARG(FileDetector::SearchWorker*, searchWorker));
    if (!b) {
        qDebug("failed to add");
        emit searchWorker->searchError(Search::ErrorAddingSearchToWorkerThread);
        return;
    }
}


void WorkerThreadController::run() {
    WorkDispatcher dispatcher;
    _dispatcher = &dispatcher;
    emit dispatcherReady();
    exec();
    _dispatcher = NULL;
}


}
