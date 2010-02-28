#include <QDebug>
#include "CloseScreenAction.h"
#include "ManagerInterface.h"

namespace Ology {

CloseScreenAction::CloseScreenAction(QObject *parent) :
    AbstractAction(parent)
{
    setText("Close screen");
}

void CloseScreenAction::run() {
    qDebug() << "CloseScreenAction triggered";
    OLOGY()->closeCurrentScreen();
}

}
