#include <QDebug>
#include <QPushButton>
#include <Ology/AbstractAction>

#include "MenuScreen.h"

namespace Ology {
namespace Plugin {
namespace Menus {

MenuScreen::MenuScreen(const QString &id, QWidget *parent) :
    AbstractScreen(parent),
    _screenId(id)
{
}


QList<AbstractAction*> MenuScreen::actions() const {
    return QList<AbstractAction*>();
}

bool MenuScreen::initialize() {
    setupUi(this);

    QBoxLayout *layout = qobject_cast<QBoxLayout*>(_groupBox->layout());

    int i = 0; 
    foreach(AbstractAction* action, _menuActions) {
        qDebug() << "Adding button:" << action->text();
        QPushButton *button = new QPushButton( action->text(), _groupBox );
        connect(button, SIGNAL(clicked()), action, SLOT(trigger()));
        button->show();
        layout->insertWidget(1 + i++, button);
    }

    return true;
}

}}}
