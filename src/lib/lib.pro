TARGET=OlogyLib
include(../common.pri)
TARGET=Ology
TEMPLATE=lib

# support classes
HEADERS += AbstractScreen.h AbstractAction.h 
HEADERS += ManagerInterface.h   Setting.h    HasNameDescription.h HasSettings.h HasErrorString.h 
SOURCES += ManagerInterface.cpp Setting.cpp  

# plugins
HEADERS += Plugin/Manager.h   Plugin/InfoInterface.h Plugin/ScreenInterface.h
SOURCES += Plugin/Manager.cpp

# global actions
HEADERS += Core/QuitAction.h    Core/DisplayScreenAction.h    Core/CloseScreenAction.h
SOURCES += Core/QuitAction.cpp  Core/DisplayScreenAction.cpp  Core/CloseScreenAction.cpp
