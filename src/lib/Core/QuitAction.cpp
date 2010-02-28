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
    qRegisterMetaTypeStreamOperators<Ology::Core::QuitAction::ConfirmQuitOption>("Ology::Core::QuitAction::ConfirmQuitOption");

    setText(tr("Quit Ology"));
    registerSetting(&_confirmQuitSetting);
}

void QuitAction::run() {
    qDebug() << "QuitAction triggered";
    if (_confirmQuitSetting.value() == ConfirmQuit) {
        // TODO: confirm-quit screen
    } else {
        qApp->quit();
    }
}

QDataStream &operator<<(QDataStream &out, const Ology::Core::QuitAction::ConfirmQuitOption &value) {
    switch(value) {
        case Ology::Core::QuitAction::Quit: out << "Quit";
        case Ology::Core::QuitAction::ConfirmQuit: out << "ConfirmQuit";
    }
    return out;
}

QDataStream &operator>>(QDataStream &in, Ology::Core::QuitAction::ConfirmQuitOption &value) {
    char *buf = NULL;
    in >> buf;

    const QString str(buf);
    if (str == "Quit") { value = Ology::Core::QuitAction::Quit; }
    else if (str == "ConfirmQuit") { value = Ology::Core::QuitAction::ConfirmQuit; }
    else { qWarning() << "Invalid ConfirmQuitOption:" << buf; }

    delete buf;
    buf = NULL;

    return in;
}

}}
