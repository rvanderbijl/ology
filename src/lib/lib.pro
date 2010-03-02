TARGET=OlogyLib
include(../common.pri)
TARGET=Ology
TEMPLATE=lib

# support classes
HEADERS += AbstractScreen.h AbstractAction.h  SimpleScreenAction.h
HEADERS += ManagerInterface.h   Setting.h    HasNameDescription.h HasSettings.h HasErrorString.h 
SOURCES += ManagerInterface.cpp Setting.cpp  

# plugins
HEADERS += Plugin/Manager.h   Plugin/InfoInterface.h Plugin/ScreenInterface.h
SOURCES += Plugin/Manager.cpp

HEADERS += CoreInterface.h   Core/CoreIds.h
SOURCES += CoreInterface.cpp

# core actions
HEADERS += Core/QuitAction.h    Core/DisplayScreenAction.h    Core/CloseScreenAction.h
SOURCES += Core/QuitAction.cpp  Core/DisplayScreenAction.cpp  Core/CloseScreenAction.cpp

# core screens
HEADERS += Core/ConfirmQuitScreen.h  
SOURCES += Core/ConfirmQuitScreen.cpp
