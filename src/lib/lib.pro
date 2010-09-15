TARGET=OlogyLib
include(../common.pri)
TARGET=Ology
TEMPLATE=lib

# support classes
HEADERS += AbstractScreen.h AbstractAction.h  SimpleAction.h SimpleScreenAction.h
HEADERS += ManagerInterface.h   Setting.h    HasNameDescription.h HasSettings.h HasErrorString.h 
SOURCES += ManagerInterface.cpp Setting.cpp  

# plugins
HEADERS += Plugin/Manager.h   Plugin/InfoInterface.h Plugin/ScreenProviderInterface.h
SOURCES += Plugin/Manager.cpp

HEADERS += Core/PseudoPluginInterface.h   Core/CoreIds.h
SOURCES += Core/PseudoPluginInterface.cpp

# core actions
HEADERS += Core/QuitAction.h    Core/DisplayScreenAction.h    Core/CloseScreenAction.h
SOURCES += Core/QuitAction.cpp  Core/DisplayScreenAction.cpp  Core/CloseScreenAction.cpp

HEADERS += Core/UpAction.h   Core/DownAction.h   Core/LeftAction.h   Core/RightAction.h
SOURCES += Core/UpAction.cpp Core/DownAction.cpp Core/LeftAction.cpp Core/RightAction.cpp
HEADERS += Core/PageUpAction.h   Core/PageDownAction.h   Core/FirstAction.h   Core/LastAction.h   
SOURCES += Core/PageUpAction.cpp Core/PageDownAction.cpp Core/FirstAction.cpp Core/LastAction.cpp

# core screens
FORMS += Core/AbstractDialogScreen.ui
HEADERS += Core/AbstractDialogScreen.h   Core/ConfirmQuitScreen.h  
SOURCES += Core/AbstractDialogScreen.cpp Core/ConfirmQuitScreen.cpp
