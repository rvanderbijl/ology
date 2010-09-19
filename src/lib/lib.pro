TARGET=OlogyLib
include(../common.pri)
TARGET=Ology
TEMPLATE=lib

# support classes
HEADERS += AbstractScreen.h   AbstractAction.h  SimpleAction.h SimpleScreenAction.h
SOURCES += AbstractScreen.cpp
HEADERS += ManagerInterface.h   Setting.h    HasNameDescription.h HasSettings.h HasErrorString.h 
SOURCES += ManagerInterface.cpp Setting.cpp  

# plugins
HEADERS += Plugin/Manager.h   Plugin/InfoInterface.h Plugin/ScreenProviderInterface.h Plugin/PlayerInterface.h
SOURCES += Plugin/Manager.cpp

HEADERS += Core/PseudoPluginInterface.h   CoreIds.h
SOURCES += Core/PseudoPluginInterface.cpp

# core actions
HEADERS += Core/QuitAction.h    Core/DisplayScreenAction.h    Core/CloseScreenAction.h
SOURCES += Core/QuitAction.cpp  Core/DisplayScreenAction.cpp  Core/CloseScreenAction.cpp

HEADERS += Core/PlayerActions.h   Core/MovementActions.h    Core/MoreScreenAction.h
SOURCES += Core/PlayerActions.cpp Core/MovementActions.cpp  Core/MoreScreenAction.cpp

# core screens
FORMS += Core/AbstractDialogScreen.ui
HEADERS += Core/AbstractDialogScreen.h   Core/ConfirmQuitScreen.h   Core/MoreScreen.h
SOURCES += Core/AbstractDialogScreen.cpp Core/ConfirmQuitScreen.cpp Core/MoreScreen.cpp
