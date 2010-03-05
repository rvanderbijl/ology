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
    _object(NULL)
{
    _object = new Core::PseudoPluginInterface();
    _coreInfoInterface = qobject_cast<Plugin::InfoInterface*>(_object);
    _coreScreenInterface = qobject_cast<Plugin::ScreenInterface*>(_object);
}

ManagerInterface::~ManagerInterface() {
    delete _object;
    _object = NULL;
    _coreInfoInterface = NULL;
    _coreScreenInterface = NULL;
}

}
