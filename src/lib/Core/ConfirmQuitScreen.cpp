#include <QDebug>
#include <Ology/AbstractAction>
#include <Ology/ManagerInterface>

#include <QPushButton>
#include <QBoxLayout>

#include "ConfirmQuitScreen.h"
#include "../SimpleScreenAction.h"

namespace Ology {
namespace Core {

ConfirmQuitScreen::ConfirmQuitScreen(QWidget *parent) :
    AbstractScreen(parent)
{
    setAutoFillBackground(true); 
    QPalette p = palette();
    p.setColor(QPalette::Window, QColor(0xff, 0x00, 0x00, 0x0f));
    setPalette(p);
}


bool ConfirmQuitScreen::initialize(Ology::InitializePurpose initPurpose) {
    Q_UNUSED(initPurpose);

    /*
    QBoxLayout *layout = qobject_cast<QBoxLayout*>(_groupBox->layout());

    int i = 0; 
    foreach(AbstractAction* action, _menuActions) {
        qDebug() << "Adding button:" << action->text();
        QPushButton *button = new QPushButton( action->text(), _groupBox );
        connect(button, SIGNAL(clicked()), action, SLOT(trigger()));
        button->show();
        layout->insertWidget(1 + i++, button);
    }
    */

    // TODO: create two actions: quit / cancel
    SimpleScreenAction *quitAction   = new SimpleScreenAction("Quit", "Actually quit Ology", this);
    SimpleScreenAction *cancelAction = new SimpleScreenAction("Cancel", "Don't quit Ology", this);
    connect(quitAction, SIGNAL(triggered()), SLOT(quit()));
    connect(cancelAction, SIGNAL(triggered()), SLOT(cancel()));

    QPushButton *quitButton = new QPushButton(tr("Quit"), this);
    QPushButton *cancelButton = new QPushButton(tr("Cancel"), this);
    connect(quitButton, SIGNAL(clicked()), quitAction, SLOT(trigger()));
    connect(cancelButton, SIGNAL(clicked()), cancelAction, SLOT(trigger()));

    quitButton->setMaximumWidth(100);
    cancelButton->setMaximumWidth(100);

    QBoxLayout *layout = new QVBoxLayout(this);
    layout->addItem(new QSpacerItem(20, 208, QSizePolicy::Minimum, QSizePolicy::Expanding));
    layout->addWidget(cancelButton);
    layout->addWidget(quitButton);
    layout->addItem(new QSpacerItem(20, 208, QSizePolicy::Minimum, QSizePolicy::Expanding));

    return true;
}

void ConfirmQuitScreen::quit() {
    qApp->quit();
}
void ConfirmQuitScreen::cancel() {
    OLOGY()->closeCurrentScreen();
}

}}
