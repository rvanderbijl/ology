#include <QDebug>
#include "QuitAction.h"
#include "ManagerInterface.h"

namespace Ology {
namespace Core {

QuitAction::QuitAction(QObject *parent) :
    AbstractAction(parent),
    _confirmQuitSetting(tr("action-quit"),
                        tr("confirm-quit"),
                        tr("Ask user to confirm that Ology should quit"),
                        ConfirmQuit)
{
    setText(tr("Quit Ology"));
    registerSetting(_confirmQuitSetting);
}

void QuitAction::run() {
    qDebug() << "QuitAction triggered";
    if (_confirmQuitSetting.value() == ConfirmQuit) {
    } else {
        qApp->quit();
    }
}

}}
