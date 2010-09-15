#include <QDebug>
#include <QPushButton>
#include <Ology/AbstractAction>
#include <Ology/SimpleAction>
#include <Ology/ManagerInterface>

#include "Interface.h"
#include "CurrentlyPlayingScreen.h"

namespace Ology {
namespace Plugin {
namespace MusicPhonon {

CurrentlyPlayingScreen::CurrentlyPlayingScreen(Interface *interface, QWidget *parent) :
    AbstractScreen(parent),
    _interface(interface)
{
    hide();
}


bool CurrentlyPlayingScreen::initialize(Ology::InitializePurpose initPurpose) {
    Q_UNUSED(initPurpose);
    setupUi(this);

    SimpleAction *actionSongPrev = new SimpleAction("prev", "Select Previous Song", "", this);
    SimpleAction *actionSongNext = new SimpleAction("next", "Select Next Song", "", this);
    SimpleAction *actionSongPageUp = new SimpleAction("page-up", "Move select a page up", "", this);
    SimpleAction *actionSongPageDown = new SimpleAction("page-down", "Move select a page down", "", this);
    SimpleAction *actionSongFirst = new SimpleAction("first", "Select first song", "", this);
    SimpleAction *actionSongLast = new SimpleAction("last", "Select last song", "", this);

    if (initPurpose == Ology::RealUsage) {
        connect(OLOGY()->action(Id::Action::Up), SIGNAL(triggered()), actionSongPrev, SLOT(trigger()));
        connect(OLOGY()->action(Id::Action::Down), SIGNAL(triggered()), actionSongNext, SLOT(trigger()));
        connect(OLOGY()->action(Id::Action::PageUp), SIGNAL(triggered()), actionSongPageUp, SLOT(trigger()));
        connect(OLOGY()->action(Id::Action::PageDown), SIGNAL(triggered()), actionSongPageDown, SLOT(trigger()));
        connect(OLOGY()->action(Id::Action::First), SIGNAL(triggered()), actionSongFirst, SLOT(trigger()));
        connect(OLOGY()->action(Id::Action::Last), SIGNAL(triggered()), actionSongLast, SLOT(trigger()));

        connect(this->selectSongNextPushButton, SIGNAL(clicked()), actionSongNext, SLOT(trigger()));
        connect(this->selectSongPrevPushButton, SIGNAL(clicked()), actionSongPrev, SLOT(trigger()));
        connect(this->selectSongPageUpPushButton, SIGNAL(clicked()), actionSongPageUp, SLOT(trigger()));
        connect(this->selectSongPageDownPushButton, SIGNAL(clicked()), actionSongPageDown, SLOT(trigger()));
        connect(this->selectSongFirstPushButton, SIGNAL(clicked()), actionSongFirst, SLOT(trigger()));
        connect(this->selectSongLastPushButton, SIGNAL(clicked()), actionSongLast, SLOT(trigger()));

        connect(actionSongPrev,     SIGNAL(triggered()), SLOT(onActionSongPrev()));
        connect(actionSongNext,     SIGNAL(triggered()), SLOT(onActionSongNext()));
        connect(actionSongPageUp,   SIGNAL(triggered()), SLOT(onActionSongPageUp()));
        connect(actionSongPageDown, SIGNAL(triggered()), SLOT(onActionSongPageDown()));
        connect(actionSongFirst,    SIGNAL(triggered()), SLOT(onActionSongFirst()));
        connect(actionSongLast,     SIGNAL(triggered()), SLOT(onActionSongLast()));
    }


    connect(this->playPausePushButton, SIGNAL(clicked()), _interface, SLOT(play()));
    connect(this->nextPushButton, SIGNAL(clicked()), _interface, SLOT(next()));
    connect(this->prevPushButton, SIGNAL(clicked()), _interface, SLOT(prev()));

    return true;
}

void CurrentlyPlayingScreen::onActionSongPrev() {
}
void CurrentlyPlayingScreen::onActionSongNext() {
}
void CurrentlyPlayingScreen::onActionSongPageUp() {
}
void CurrentlyPlayingScreen::onActionSongPageDown() {
}
void CurrentlyPlayingScreen::onActionSongFirst() {
}
void CurrentlyPlayingScreen::onActionSongLast() {
}

}}}
