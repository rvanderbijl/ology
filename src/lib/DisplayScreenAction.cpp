#include <QDebug>
#include "DisplayScreenAction.h"
#include "ManagerInterface.h"

namespace Ology {

DisplayScreenAction::DisplayScreenAction(QObject *parent) :
    AbstractAction(parent)
{
}

DisplayScreenAction::DisplayScreenAction(const QString &screenId, QObject *parent) :
    AbstractAction(parent),
    _screenId(screenId)
{
    setText("Display Screen: " + screenId);
}

void DisplayScreenAction::run() {
    if (_screenId.isEmpty()) {
        qWarning() << "DisplayScreenAction::run() called with empty screenId!";
        return;
    }

    qDebug() << "DisplayScreenAction triggered: " << screenId(); 
    OLOGY()->displayScreen(screenId());
}

}
