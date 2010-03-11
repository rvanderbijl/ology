#include <QDebug>

#include "WorkDispatcher.h"
#include "WorkerThreadController.h"
#include "SearchWorker.h"

namespace FileDetector {

WorkerThreadController::WorkerThreadController(QObject *parent) : 
    QThread(parent),
    _magicCookie(0),
    _dispatcher(NULL)
{
    qRegisterMetaType< QList<QUrl> >("QList<QUrl>");

    _magicCookie = magic_open(MAGIC_MIME|MAGIC_PRESERVE_ATIME);
    if (0 != magic_load(_magicCookie, NULL)) { // load default database 
        qWarning() << "Error loading magic:" << magic_error(_magicCookie);
    }
}

WorkerThreadController::~WorkerThreadController() {
    magic_close(_magicCookie);
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
