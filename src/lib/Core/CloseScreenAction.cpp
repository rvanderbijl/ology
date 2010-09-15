#include <QDebug>
#include "CloseScreenAction.h"
#include "ManagerInterface.h"

namespace Ology {
namespace Core {

CloseScreenAction::CloseScreenAction(QObject *parent) :
    AbstractAction(Id::Action::CloseScreen, GlobalAction, parent)
{
    setTranslationContext(metaObject()->className());
    setUntranslatedName("Close Screen");
    setUntranslatedDescription("Close the current screen, go back to the previous screen");
    setText(tr("Close screen"));
}

void CloseScreenAction::run() {
    qDebug() << "CloseScreenAction triggered";
    OLOGY()->closeCurrentScreen();
}

}}
