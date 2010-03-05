#include "ManagerInterface.h"

#include "Plugin/InfoInterface.h"
#include "Plugin/ScreenInterface.h"

#include "Core/PseudoPluginInterface.h"

#include "Core/CoreIds.h"
#include "Core/CloseScreenAction.h"
#include "Core/QuitAction.h"
#include "Core/ConfirmQuitScreen.h"



namespace Ology {

ManagerInterface::ManagerInterface() :
    _pseudoPlugin(NULL)
{
    _pseudoPlugin = new Core::PseudoPluginInterface();
    _coreInfoInterface = qobject_cast<Plugin::InfoInterface*>(_pseudoPlugin);
    _coreScreenInterface = qobject_cast<Plugin::ScreenInterface*>(_pseudoPlugin);
}

ManagerInterface::~ManagerInterface() {
    delete _pseudoPlugin;
    _pseudoPlugin = NULL;
    _coreInfoInterface = NULL;
    _coreScreenInterface = NULL;
}

}
