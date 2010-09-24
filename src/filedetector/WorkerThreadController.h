#ifndef FILEDETECTOR_WORKER_THREAD_CONTROLLER
#define FILEDETECTOR_WORKER_THREAD_CONTROLLER

#include <QThread>

extern "C" {
#include <magic.h>
}

namespace FileDetector {
class SearchWorker;
class WorkDispatcher;

class WorkerThreadController : public QThread {
    Q_OBJECT
public:
    WorkerThreadController(QObject *parent);
    ~WorkerThreadController();

    void addSearchWorker(SearchWorker *searchWorker);
    void removeSearchWorker(SearchWorker *searchWorker);

signals:
    void dispatcherReady();

protected:
    virtual void run();

private:
    WorkDispatcher *_dispatcher;
};

}

#endif
