TEMPLATE=lib
CONFIG += plugin
include(../../common.pri)

QT += phonon

FORMS += CurrentlyPlayingScreen.ui
SOURCES += Interface.cpp  CurrentlyPlayingScreen.cpp Player.cpp 
HEADERS += Interface.h    CurrentlyPlayingScreen.h   Player.h   Song.h

LIBS += -lFileDetector -ltag
