#include <QDebug>
#include "CloseScreenAction.h"
#include "ManagerInterface.h"

namespace Ology {
namespace Core {

CloseScreenAction::CloseScreenAction(QObject *parent) :
    AbstractAction(parent)
{
    setText(tr("Close screen"));
}

void CloseScreenAction::run() {
    qDebug() << "CloseScreenAction triggered";
    OLOGY()->closeCurrentScreen();
}

}}
