TEMPLATE=lib
CONFIG += plugin
include(../../common.pri)

QT += phonon

FORMS += currentlyplaying.ui
SOURCES += Interface.cpp  CurrentlyPlayingScreen.cpp PlayHistory.cpp  PlayList.cpp
HEADERS += Interface.h    CurrentlyPlayingScreen.h   PlayHistory.h    PlayList.h   PlayEntry.h PlayBase.h

LIBS += -lFileDetector -ltag
