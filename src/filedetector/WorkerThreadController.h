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
    magic_t magicCookie() { return _magicCookie; }

signals:
    void dispatcherReady();

protected:
    virtual void run();

private:
    magic_t _magicCookie;
    WorkDispatcher *_dispatcher;
};

}

#endif
