#include <QDebug>
#include "DisplayScreenAction.h"
#include "ManagerInterface.h"

namespace Ology {
namespace Core {

DisplayScreenAction::DisplayScreenAction(QObject *parent) :
    AbstractAction(parent)
{
    setTranslationContext(metaObject()->className());
    setUntranslatedName("Display Screen");
    setUntranslatedDescription("Display a screen by it's screen-id");
}

DisplayScreenAction::DisplayScreenAction(const QString &screenId, QObject *parent) :
    AbstractAction(parent),
    _screenId(screenId)
{
    setText(tr("Display Screen: %1").arg(screenId));
}

void DisplayScreenAction::run() {
    if (_screenId.isEmpty()) {
        qWarning() << "DisplayScreenAction::run() called with empty screenId!";
        return;
    }

    qDebug() << "DisplayScreenAction triggered: " << screenId(); 
    OLOGY()->displayScreen(screenId());
}

}}
