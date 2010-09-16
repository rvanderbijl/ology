TEMPLATE=lib
CONFIG += plugin
include(../../common.pri)

QT += phonon

FORMS += currentlyplaying.ui
SOURCES += Interface.cpp  CurrentlyPlayingScreen.cpp PlayListModel.cpp PlayHistory.cpp  PlayList.cpp
HEADERS += Interface.h    CurrentlyPlayingScreen.h   PlayListModel.h   PlayHistory.h    PlayList.h   PlayEntry.h PlayBase.h

LIBS += -lFileDetector -ltag
