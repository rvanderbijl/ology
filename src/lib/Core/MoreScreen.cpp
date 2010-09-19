#include <QDebug>
#include <QPushButton>
#include <QBoxLayout>

#include <Ology/AbstractAction>
#include <Ology/ManagerInterface>
#include "MoreScreen.h"
#include "../SimpleScreenAction.h"

namespace Ology {
namespace Core {

MoreScreen::MoreScreen(QWidget *parent) :
    AbstractDialogScreen(parent),
    _forScreen(NULL)
{
}


bool MoreScreen::initialize(Ology::InitializePurpose initPurpose) {
    if (initPurpose == RealUsage) {
        _forScreen = OLOGY()->currentScreen();
        if (!_forScreen || _forScreen->id() == this->id()) { // avoid showing "More" options screen twice
            return false;
        }

        if (_forScreen->moreActions().isEmpty()) {
            qDebug() << "Screen" << _forScreen->id() << "has no actions for the More Screen (moreActions().isEmpty() == true)";
            return false;
        }

        if (_forScreen->moreActions().size() > 8) {
            qWarning() << "Screen" << _forScreen->id() << "has" << _forScreen->moreActions().size() << "actions, which is quite a lot.";
        }

        foreach(AbstractAction *action, _forScreen->moreActions()) {
            connect(action, SIGNAL(triggered()), SLOT(closeScreen()));
        }
    }

    return AbstractDialogScreen::initialize(initPurpose);
}


}}
