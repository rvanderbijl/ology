#include <QDebug>
#include <QSqlError>

#include "SearchWorker.h"
#include "WorkDispatcher.h"

namespace FileDetector {

WorkDispatcher::WorkDispatcher() :
    _magicCookie(0)
{
    _magicCookie = magic_open(MAGIC_MIME|MAGIC_PRESERVE_ATIME);
    if (0 != magic_load(_magicCookie, NULL)) { // load default database 
        qWarning() << "Error loading magic:" << magic_error(_magicCookie);
    }

    _nullTimer.setInterval(0);
    _nullTimer.setSingleShot(true);
    connect(&_nullTimer, SIGNAL(timeout()), SLOT(dispatchWork()));
}

void WorkDispatcher::addSearchWorker(FileDetector::SearchWorker* worker) {
    qDebug("dispatcher added search worker");
    _jobQueue.append(worker);
    _nullTimer.start();
}


void WorkDispatcher::dispatchWork() {
    if (_currentJob) {
        if (_currentJob->work()) {
            _nullTimer.start();
        }
        return;
    } else {
        if (_jobQueue.isEmpty()) { 
            _nullTimer.stop(); 
            return; 
        }
        qDebug("work dispatcher starting new job");

        _currentJob = _jobQueue.takeFirst();
        _currentJob->initialize();
        _nullTimer.start();
    }
}



}
