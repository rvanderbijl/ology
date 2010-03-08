#ifndef FILEDETECTOR_WORKER_THREAD_CONTROLLER
#define FILEDETECTOR_WORKER_THREAD_CONTROLLER

#include <QThread>

namespace FileDetector {
class SearchWorker;
class WorkDispatcher;

class WorkerThreadController : public QThread {
    Q_OBJECT
public:
    WorkerThreadController(QObject *parent);
    ~WorkerThreadController();

    void addSearchWorker(SearchWorker *searchWorker);

protected:
    virtual void run();

private:
    WorkDispatcher *_dispatcher;
};

}

#endif
