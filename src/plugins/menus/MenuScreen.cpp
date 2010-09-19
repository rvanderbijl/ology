#include <QDebug>
#include <QPushButton>
#include <Ology/AbstractAction>

#include "MenuScreen.h"

namespace Ology {
namespace Plugin {
namespace Menus {

MenuScreen::MenuScreen(const QString &id, QWidget *parent) :
    AbstractScreen(parent),
    _screenId(id),
    _restoreButton(NULL)
{
    hide();
}


QList<AbstractAction*> MenuScreen::actions() const {
    return QList<AbstractAction*>();
}

bool MenuScreen::initialize(Ology::InitializePurpose initPurpose) {
    Q_UNUSED(initPurpose);
    setupUi(this);

    QBoxLayout *layout = qobject_cast<QBoxLayout*>(_groupBox->layout());

    int i = 0; 
    foreach(AbstractAction* action, _menuActions) {
        qDebug() << "Adding button:" << action->text();
        QPushButton *button = new QPushButton( action->text(), _groupBox );
        button->setAutoFillBackground(true);
        connect(button, SIGNAL(clicked()), action, SLOT(trigger()));
        connect(button, SIGNAL(clicked()), this, SLOT(setButtonToRestore()));
        button->show();
        layout->insertWidget(1 + i++, button); // 1+ is for the spacer 
    }

    return true;
}


 void MenuScreen::setButtonToRestore() {
     if (qobject_cast<QPushButton*>(sender())) {
         _restoreButton = qobject_cast<QPushButton*>(sender());
     }
 }

void MenuScreen::run() {
    AbstractScreen::run();

    if (_restoreButton) {
        _restoreButton->setFocus();
    } else {
        QBoxLayout *layout = qobject_cast<QBoxLayout*>(_groupBox->layout());
        QLayoutItem *layoutItem = layout->itemAt(1);
        if (layoutItem && layoutItem->widget()) {
            layoutItem->widget()->setFocus();
        }
    }
}

}}}
