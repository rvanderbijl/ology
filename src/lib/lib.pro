TARGET=OlogyLib
include(../common.pri)
TARGET=Ology
TEMPLATE=lib

# support classes
HEADERS += AbstractScreen.h AbstractAction.h ManagerInterface.h
HEADERS += Setting.h    HasSettings.h
SOURCES += Setting.cpp  

# plugins
HEADERS += Plugin/Manager.h   Plugin/InfoInterface.h Plugin/ScreenInterface.h
SOURCES += Plugin/Manager.cpp

# global actions
HEADERS += Core/QuitAction.h    Core/DisplayScreenAction.h    Core/CloseScreenAction.h
SOURCES += Core/QuitAction.cpp  Core/DisplayScreenAction.cpp  Core/CloseScreenAction.cpp
