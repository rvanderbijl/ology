#include "CoreInterface.h"

#include "Core/CoreIds.h"
#include "Core/CloseScreenAction.h"
#include "Core/QuitAction.h"
#include "Core/ConfirmQuitScreen.h"


namespace Ology {

CoreInterface::CoreInterface() {
    AbstractAction *action = NULL;
    
    action = new Core::CloseScreenAction(this);
    action->setShortcut(Qt::Key_Escape);
    _actions << action;

    action = new Core::QuitAction(this);
    action->setShortcut(QKeySequence("Ctrl+Q"));

    _actions << action;
}

QStringList CoreInterface::screenIds() {
    return QStringList() 
        << ID_SCREEN_CONFIRM_QUIT
        ;
}

AbstractScreen* CoreInterface::createScreen(const QString &id, QWidget *parent) { 
    if (id == ID_SCREEN_CONFIRM_QUIT) { return new Core::ConfirmQuitScreen(parent); }
    return NULL; 
}

}
