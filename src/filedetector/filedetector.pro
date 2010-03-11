TEMPLATE = lib
TARGET = FileDetector
include(../common.pri)

QT += sql

HEADERS += Criteria.h  FileType.h SearchParameters.h
HEADERS += Search.h
SOURCES += Search.cpp

HEADERS += WorkerThreadController.h   WorkDispatcher.h   SearchWorker.h
SOURCES += WorkerThreadController.cpp WorkDispatcher.cpp SearchWorker.cpp

LIBS += -lmagic
