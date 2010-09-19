#include <QDebug>
#include <QPushButton>
#include <QBoxLayout>

#include <Ology/AbstractAction>
#include "AbstractDialogScreen.h"
#include "../SimpleScreenAction.h"

namespace Ology {
namespace Core {

AbstractDialogScreen::AbstractDialogScreen(QWidget *parent) :
    AbstractScreen(parent)
{
    setAutoFillBackground(true); 

    QPalette p = palette();
    p.setColor(QPalette::Window, QColor(0xff, 0x00, 0x00, 0x0f));
    setPalette(p);
}


bool AbstractDialogScreen::initialize(Ology::InitializePurpose initPurpose) {
    if (initPurpose != Ology::RealUsage) { return true; }

    _ui.setupUi(this);
    _ui.dialogText->setText(dialogText());

    QList<AbstractAction*> actions = this->actions();
    foreach(AbstractAction *action, actions) {
        QPushButton *button = new QPushButton(action->name(), _ui.dialog);
        connect(button, SIGNAL(clicked()), action, SLOT(trigger()));
        _ui.dialogLayout->addWidget(button);
    }

    return true;
}


void AbstractDialogScreen::run() {
    AbstractScreen::run();

    for(int i = 0; i < _ui.dialogLayout->count(); i++) {
        QLayoutItem *layoutItem = _ui.dialogLayout->itemAt(i);
        if (layoutItem && qobject_cast<QPushButton*>(layoutItem->widget())) {
            layoutItem->widget()->setFocus();
            break;
        }
    }
}

}}
