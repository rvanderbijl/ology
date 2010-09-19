#include "ManagerInterface.h"
#include "AbstractPlayer.h"

#include "Plugin/Manager.h"
#include "Plugin/InfoInterface.h"
#include "Plugin/ScreenProviderInterface.h"

#include "Core/PseudoPluginInterface.h"

#include "CoreIds.h"
#include "Core/CloseScreenAction.h"
#include "Core/QuitAction.h"
#include "Core/ConfirmQuitScreen.h"



namespace Ology {

ManagerInterface::ManagerInterface() :
    _coreInfoInterface(NULL),
    _coreScreenProviderInterface(NULL),
    _pseudoPlugin(NULL),
    _currentPlayer(NULL)
{
    _pseudoPlugin = new Core::PseudoPluginInterface();
    _coreInfoInterface = qobject_cast<Plugin::InfoInterface*>(_pseudoPlugin);
    _coreScreenProviderInterface = qobject_cast<Plugin::ScreenProviderInterface*>(_pseudoPlugin);
}

ManagerInterface::~ManagerInterface() {
    delete _pseudoPlugin;
    _pseudoPlugin = NULL;
    _coreInfoInterface = NULL;
    _coreScreenProviderInterface = NULL;
}

AbstractAction* ManagerInterface::action(const QString &id) const {
    AbstractAction* action = _coreScreenProviderInterface->action(id);
    if (action) { return action; }

    foreach(Plugin::ScreenProviderInterface* screenProviderInterface, pluginManager()->screenProviderPlugins()) {
        AbstractAction* action = screenProviderInterface->action(id);
        if (action) { return action; }
    }

    return NULL;
}

void ManagerInterface::setCurrentPlayer(AbstractPlayer *player) {
    if (_currentPlayer && (_currentPlayer != player)) {
        _currentPlayer->stop();
    }
    _currentPlayer = player; 
}

}
