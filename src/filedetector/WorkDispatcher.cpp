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

WorkDispatcher::~WorkDispatcher() {
    magic_close(_magicCookie);
}

void WorkDispatcher::addSearchWorker(FileDetector::SearchWorker* worker) {
    qDebug("dispatcher added search worker");
    worker->setParent(this);
    worker->setMagicCookie(_magicCookie);
    worker->initialize();
    connect(worker, SIGNAL(moreWorkFound()), SLOT(moreWorkFound()));
    _jobQueue.append(worker);
    _nullTimer.start();
}

void WorkDispatcher::moreWorkFound() {
    SearchWorker *worker = qobject_cast<SearchWorker*>(sender());
    if (worker) {
        if (_currentJob == worker || _jobQueue.contains(worker)) {
            return;
        }
        qDebug() << "WorkDispatcher: searchWorker has more work to do";
        _jobQueue.append(worker);
        _nullTimer.start();
    } else {
        qDebug() << "MoreWorkFound - but for whom? (sender() is not a SearchWorker)";
    }
}

void WorkDispatcher::dispatchWork() {
    if (_currentJob) {
        if (_currentJob->work()) {
            _nullTimer.start();
        } else {
            _currentJob = NULL;
        }
        return;
    } else {
        if (_jobQueue.isEmpty()) { 
            _nullTimer.stop(); 
            return; 
        }
        qDebug("work dispatcher starting new job");

        _currentJob = _jobQueue.takeFirst();
        _nullTimer.start();
    }
}



}
