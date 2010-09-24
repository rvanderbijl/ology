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
    ~WorkDispatcher();

public slots:
    void dispatchWork();
    void addSearchWorker(FileDetector::SearchWorker* worker);
    void moreWorkFound();

private:
    magic_t _magicCookie;
    QQueue<SearchWorker*> _jobQueue;
    SearchWorker* _currentJob;
    QTimer _nullTimer;
};

}

#endif
