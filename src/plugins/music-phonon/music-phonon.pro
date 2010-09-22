TEMPLATE=lib
CONFIG += plugin
include(../../common.pri)

QT += phonon

FORMS += CurrentlyPlayingScreen.ui
SOURCES += Interface.cpp  CurrentlyPlayingScreen.cpp Player.cpp PlayList.cpp
HEADERS += Interface.h    CurrentlyPlayingScreen.h   Player.h   PlayList.h   Song.h

LIBS += -lFileDetector -ltag
