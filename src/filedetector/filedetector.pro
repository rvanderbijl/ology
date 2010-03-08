TEMPLATE = lib
include(../common.pri)

QT += sql

HEADERS += Criteria.h  FileType.h SearchParameters.h
HEADERS += Interface.h   Search.h
SOURCES += Interface.cpp Search.cpp

HEADERS += WorkerThreadController.h   WorkDispatcher.h   SearchWorker.h
SOURCES += WorkerThreadController.cpp WorkDispatcher.cpp SearchWorker.cpp

