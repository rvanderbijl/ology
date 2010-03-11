#include "PseudoPluginInterface.h"

#include "Core/CoreIds.h"
#include "Core/CloseScreenAction.h"
#include "Core/QuitAction.h"
#include "Core/ConfirmQuitScreen.h"


namespace Ology {
namespace Core {

PseudoPluginInterface::PseudoPluginInterface() :
    _closeMainScreenOption(tr("core"), tr("close-main-screen-result"), ConfirmQuit,
                           metaObject()->className(), "Close main screen result", "The result of closing the main screen", this),
    _autoLoadPlugins(tr("core"), tr("autoload-plugins"), true,
                           metaObject()->className(), "Autoload plugins", "Automatically load all plugins that can be found", this)
{

    qRegisterMetaTypeStreamOperators<Ology::Core::PseudoPluginInterface::CloseMainScreenOption>("Ology::Core::PseudoPluginInterface::CloseMainScreenOption");

    registerSetting(&_closeMainScreenOption);
    registerSetting(&_autoLoadPlugins);


    AbstractAction *action = NULL;
    
    action = new Core::CloseScreenAction(this);
    action->setShortcut(Qt::Key_Escape);
    _actions << action;

    action = new Core::QuitAction(this);
    action->setShortcut(QKeySequence("Ctrl+Q"));

    _actions << action;
}

QStringList PseudoPluginInterface::screenIds() {
    return QStringList() 
        << ID_SCREEN_CONFIRM_QUIT
        ;
}

AbstractScreen* PseudoPluginInterface::createScreen(const QString &id, QWidget *parent) { 
    if (id == ID_SCREEN_CONFIRM_QUIT) { return new Core::ConfirmQuitScreen(parent); }
    return NULL; 
}

}}

OLOGY_DECLARE_OPERATORS_FOR_ENUM(Ology::Core::PseudoPluginInterface, CloseMainScreenOption);
