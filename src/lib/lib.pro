TARGET=OlogyLib
include(../common.pri)
TARGET=Ology
TEMPLATE=lib

HEADERS += AbstractScreen.h AbstractAction.h ManagerInterface.h

HEADERS += Plugin/Manager.h   Plugin/InfoInterface.h Plugin/ScreenInterface.h
SOURCES += Plugin/Manager.cpp

HEADERS += DisplayScreenAction.h     CloseScreenAction.h
SOURCES += DisplayScreenAction.cpp   CloseScreenAction.cpp
