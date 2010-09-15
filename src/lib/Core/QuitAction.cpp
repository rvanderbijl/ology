#include <QDebug>
#include <Ology/ManagerInterface>
#include "CoreIds.h"
#include "QuitAction.h"

namespace Ology {
namespace Core {

QuitAction::QuitAction(QObject *parent) :
    AbstractAction(Id::Action::Quit, GlobalAction, parent),
    _confirmQuitSetting(tr("action-quit"), tr("confirm-quit"), ConfirmQuit,
                        metaObject()->className(), "Confirm Quit", "Ask user to confirm that Ology should quit", this)
{
    qRegisterMetaTypeStreamOperators<Ology::Core::QuitAction::ConfirmQuitOption>("Ology::Core::QuitAction::ConfirmQuitOption");

    setTranslationContext(metaObject()->className());
    setUntranslatedName("Quit Ology");
    setUntranslatedDescription("Quit Ology, possbily ask confirmation");

    setText(tr("Quit Ology"));
    registerSetting(&_confirmQuitSetting);
}

void QuitAction::run() {
    qDebug() << "QuitAction triggered";
    if (_confirmQuitSetting.value() == ConfirmQuit) {
        OLOGY()->displayScreen(Id::Screen::ConfirmQuit); 
    } else {
        qApp->quit();
    }
}

OLOGY_DECLARE_OPERATORS_FOR_ENUM(Ology::Core::QuitAction, ConfirmQuitOption);

}}
