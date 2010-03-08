#include <QDebug>
#include <QSqlError>


#include "Worker.h"

namespace FileDetector {

Worker::Worker() :
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


void Worker::setStorageFile( const QString &fileName ) {
    if (_db.isOpen()) {
        QSqlDatabase::removeDatabase(_db.connectionName());
        _db.close(); 
    }

    _db = QSqlDatabase::addDatabase("QSQLITE", "FileDetector");
    _db.setDatabaseName(fileName);
    if (!_db.open()) {
        qWarning() << "Error opening sqlite database!" << _db.lastError().text();
    }
}





void Worker::doWork() {
    if (_currentJob.type == Job::Search) { doSearchWork(); _nullTimer.start(); return; }
    //if (_currentJob.type == Job::Verify) { doVerifyWork(); _nullTimer.start(); return; }
    if (_currentJob.type == Job::None) {
        if (_jobQueue.isEmpty()) { _nullTimer.stop(); return; }
        _currentJob = _jobQueue.takeFirst();

        if (_currentJob.type == Job::Search) {
        }

        _nullTimer.start();
        return;
    }
}

void Worker::doSearchWork() {
}



}
