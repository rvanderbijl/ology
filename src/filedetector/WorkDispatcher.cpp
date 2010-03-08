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
    connect(&_nullTimer, SIGNAL(timeout()), SLOT(doWork()));
}



void WorkDispatcher::dispatchWork() {
    if (_currentJob) {
        _currentJob->work();
        return;
    } else {
        if (_jobQueue.isEmpty()) { 
            _nullTimer.stop(); 
            return; 
        }

        _currentJob = _jobQueue.takeFirst();
        _nullTimer.start();
    }
}



}
