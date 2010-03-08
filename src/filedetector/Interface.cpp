#include "Interface.h"
#include "WorkerThreadController.h"

namespace FileDetector {



Interface::Interface(QObject *parent) :
    QObject(parent),
    _threadController(new WorkerThreadController(this))
{
    _threadController->setObjectName("FileDetector Worker Thread");
    _threadController->start();
}

Interface::~Interface() {
}





}
