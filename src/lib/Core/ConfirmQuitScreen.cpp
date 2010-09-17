#include <QDebug>
#include <QPushButton>
#include <QBoxLayout>

#include <Ology/AbstractAction>
#include <Ology/ManagerInterface>
#include "ConfirmQuitScreen.h"
#include "../SimpleScreenAction.h"

namespace Ology {
namespace Core {

ConfirmQuitScreen::ConfirmQuitScreen(QWidget *parent) :
    AbstractDialogScreen(parent)
{
    setAutoFillBackground(true); 
    QPalette p = palette();
    p.setColor(QPalette::Window, QColor(0xff, 0x00, 0x00, 0x0f));
    setPalette(p);
}


bool ConfirmQuitScreen::initialize(Ology::InitializePurpose initPurpose) {
    SimpleScreenAction *quitAction   = new SimpleScreenAction("action-quit", "Quit", "Actually quit Ology", this);
    SimpleScreenAction *cancelAction = new SimpleScreenAction("action-cancel", "Cancel", "Don't quit Ology", this);

    if (initPurpose == RealUsage) {
        connect(quitAction, SIGNAL(triggered()), SLOT(quit()));
        connect(cancelAction, SIGNAL(triggered()), SLOT(cancel()));
    }

    return AbstractDialogScreen::initialize(initPurpose);
}

void ConfirmQuitScreen::quit() {
    qApp->quit();
}
void ConfirmQuitScreen::cancel() {
    OLOGY()->closeCurrentScreen();
}

}}
