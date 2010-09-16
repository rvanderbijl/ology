#include "PseudoPluginInterface.h"

#include "CoreIds.h"
#include "Core/CloseScreenAction.h"
#include "Core/QuitAction.h"
#include "Core/ConfirmQuitScreen.h"
#include "Core/PageUpAction.h"
#include "Core/PageDownAction.h"
#include "Core/UpAction.h"
#include "Core/DownAction.h"
#include "Core/LeftAction.h"
#include "Core/RightAction.h"
#include "Core/FirstAction.h"
#include "Core/LastAction.h"


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

    #define ADD_ACTION(class, shortcut) { \
        class *action = new class(this); \
        action->setShortcut(shortcut); \
        _actions << action; \
    }
    
    ADD_ACTION(Core::CloseScreenAction, Qt::Key_Escape);
    ADD_ACTION(Core::QuitAction, QKeySequence("Ctrl+Q"));
    ADD_ACTION(Core::PageUpAction, Qt::Key_PageUp);
    ADD_ACTION(Core::PageDownAction, Qt::Key_PageDown);
    ADD_ACTION(Core::UpAction, Qt::Key_Up);
    ADD_ACTION(Core::DownAction, Qt::Key_Down);
    ADD_ACTION(Core::LeftAction, Qt::Key_Left);
    ADD_ACTION(Core::RightAction, Qt::Key_Right);
    ADD_ACTION(Core::FirstAction, Qt::Key_Home);
    ADD_ACTION(Core::LastAction, Qt::Key_End);
}

QStringList PseudoPluginInterface::screenIds() {
    return QStringList() 
        << Id::Screen::ConfirmQuit
        ;
}

AbstractScreen* PseudoPluginInterface::createScreen(const QString &id, QWidget *parent) { 
    if (id == Id::Screen::ConfirmQuit) { return new Core::ConfirmQuitScreen(parent); }
    return NULL; 
}

}}

OLOGY_DECLARE_OPERATORS_FOR_ENUM(Ology::Core::PseudoPluginInterface, CloseMainScreenOption);
