#include "PseudoPluginInterface.h"

#include "CoreIds.h"
#include "Core/CloseScreenAction.h"
#include "Core/QuitAction.h"
#include "Core/MoreScreenAction.h"
#include "Core/MovementActions.h"
#include "Core/PlayerActions.h"

#include "Core/ConfirmQuitScreen.h"
#include "Core/MoreScreen.h"

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
    
    ADD_ACTION(Core::QuitAction, QKeySequence("Ctrl+Q"));
    ADD_ACTION(Core::CloseScreenAction, Qt::Key_Escape);
    ADD_ACTION(Core::MoreScreenAction, Qt::Key_F1);

    ADD_ACTION(Core::PageUpAction, QKeySequence()); //Qt::Key_PageUp);
    ADD_ACTION(Core::PageDownAction, QKeySequence()); //Qt::Key_PageDown);
    ADD_ACTION(Core::UpAction, QKeySequence()); //Qt::Key_Up);
    ADD_ACTION(Core::DownAction, QKeySequence()); //Qt::Key_Down);
    ADD_ACTION(Core::LeftAction, QKeySequence()); //Qt::Key_Left);
    ADD_ACTION(Core::RightAction, QKeySequence()); //Qt::Key_Right);
    ADD_ACTION(Core::FirstAction, QKeySequence()); //Qt::Key_Home);
    ADD_ACTION(Core::LastAction, QKeySequence()); //Qt::Key_End);
    
    ADD_ACTION(Core::PlayAction, QKeySequence());
    ADD_ACTION(Core::StopAction, QKeySequence());
    ADD_ACTION(Core::NextAction, Qt::CTRL + Qt::Key_Right);
    ADD_ACTION(Core::PrevAction, Qt::CTRL + Qt::Key_Left);
    ADD_ACTION(Core::PlayOrPauseAction, QKeySequence("Ctrl+P"));
    ADD_ACTION(Core::PauseOrUnpauseAction, QKeySequence());
}

QStringList PseudoPluginInterface::screenIds() {
    return QStringList() 
        << Id::Screen::ConfirmQuit
        << Id::Screen::More
        ;
}

AbstractScreen* PseudoPluginInterface::createScreen(const QString &id, QWidget *parent) { 
    if (id == Id::Screen::ConfirmQuit) { return new Core::ConfirmQuitScreen(parent); }
    if (id == Id::Screen::More) { return new Core::MoreScreen(parent); }
    return NULL; 
}

}}

OLOGY_DECLARE_OPERATORS_FOR_ENUM(Ology::Core::PseudoPluginInterface, CloseMainScreenOption);
