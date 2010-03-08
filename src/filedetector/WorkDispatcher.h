#ifndef FILEDETECTOR_WORK_DISPATCHER
#define FILEDETECTOR_WORK_DISPATCHER

#include <QReadWriteLock>
#include <QObject>
#include <QQueue>
#include <QTimer>
#include <QUrl>
#include <QSqlDatabase>
#include <QHash>

// lib-magic and inotify
extern "C" {
#include <magic.h>
#include <sys/inotify.h>
}



namespace FileDetector {
   class SearchWorker;

class WorkDispatcher : public QObject {
    Q_OBJECT
public:
    WorkDispatcher();

    Q_INVOKABLE void addSearchWorker(FileDetector::SearchWorker* worker);

public slots:
    void dispatchWork();

private:
    magic_t _magicCookie;
    QQueue<SearchWorker*> _jobQueue;
    SearchWorker* _currentJob;
    QTimer _nullTimer;
};

}

#endif
