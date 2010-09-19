#include <QDebug>
#include <Ology/ManagerInterface>
#include <Ology/AbstractScreen>
#include "CoreIds.h"
#include "MoreScreenAction.h"

namespace Ology {
namespace Core {

MoreScreenAction::MoreScreenAction(QObject *parent) :
    AbstractAction(Id::Action::MoreScreen, GlobalAction, parent)
{
    setTranslationContext(metaObject()->className());
    setUntranslatedName("More actions");
    setUntranslatedDescription("Show more actions available on the current screen");
    setText(tr("More actions"));
}

void MoreScreenAction::run() {
    if (OLOGY()->currentScreen()->id() != Id::Screen::More) {
        qDebug() << "MoreScreenAction triggered";
        OLOGY()->displayScreen(Id::Screen::More); 
    } else {
        qDebug() << "MoreScreenAction: not displaying 'MoreScreen' for the 'MoreScreen'";
    }
}

}}
