TEMPLATE=lib
CONFIG += plugin
include(../../common.pri)

QT += phonon

#FORMS += 
SOURCES += Interface.cpp 
HEADERS += Interface.h   

LIBS += -lFileDetector
