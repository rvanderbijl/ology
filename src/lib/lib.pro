TARGET=OlogyLib
include(../common.pri)
TARGET=Ology
TEMPLATE=lib

HEADERS += AbstractScreen.h AbstractAction.h ManagerInterface.h

HEADERS += PluginManager.h   PluginInterface.h
SOURCES += PluginManager.cpp

HEADERS += DisplayScreenAction.h     CloseScreenAction.h
SOURCES += DisplayScreenAction.cpp   CloseScreenAction.cpp
