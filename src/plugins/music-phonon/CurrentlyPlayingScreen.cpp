#include <QDebug>
#include <QFileInfo>
#include <QTime>
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
    updateProgressBar();
    updateCurrentSong();

    SimpleAction *actionSelectSongPrev = new SimpleAction("prev", "Select Previous Song", "", this);
    SimpleAction *actionSelectSongNext = new SimpleAction("next", "Select Next Song", "", this);
    SimpleAction *actionSelectSongPageUp = new SimpleAction("page-up", "Move select a page up", "", this);
    SimpleAction *actionSelectSongPageDown = new SimpleAction("page-down", "Move select a page down", "", this);
    SimpleAction *actionSelectSongFirst = new SimpleAction("first", "Select first song", "", this);
    SimpleAction *actionSelectSongLast = new SimpleAction("last", "Select last song", "", this);

    if (initPurpose == Ology::RealUsage) {
        connect(OLOGY()->action(Id::Action::Up), SIGNAL(triggered()), actionSelectSongPrev, SLOT(trigger()));
        connect(OLOGY()->action(Id::Action::Down), SIGNAL(triggered()), actionSelectSongNext, SLOT(trigger()));
        connect(OLOGY()->action(Id::Action::PageUp), SIGNAL(triggered()), actionSelectSongPageUp, SLOT(trigger()));
        connect(OLOGY()->action(Id::Action::PageDown), SIGNAL(triggered()), actionSelectSongPageDown, SLOT(trigger()));
        connect(OLOGY()->action(Id::Action::First), SIGNAL(triggered()), actionSelectSongFirst, SLOT(trigger()));
        connect(OLOGY()->action(Id::Action::Last), SIGNAL(triggered()), actionSelectSongLast, SLOT(trigger()));

        connect(this->selectSongNextPushButton, SIGNAL(clicked()), actionSelectSongNext, SLOT(trigger()));
        connect(this->selectSongPrevPushButton, SIGNAL(clicked()), actionSelectSongPrev, SLOT(trigger()));
        connect(this->selectSongPageUpPushButton, SIGNAL(clicked()), actionSelectSongPageUp, SLOT(trigger()));
        connect(this->selectSongPageDownPushButton, SIGNAL(clicked()), actionSelectSongPageDown, SLOT(trigger()));
        connect(this->selectSongFirstPushButton, SIGNAL(clicked()), actionSelectSongFirst, SLOT(trigger()));
        connect(this->selectSongLastPushButton, SIGNAL(clicked()), actionSelectSongLast, SLOT(trigger()));

        connect(actionSelectSongPrev,     SIGNAL(triggered()), SLOT(onActionSelectSongPrev()));
        connect(actionSelectSongNext,     SIGNAL(triggered()), SLOT(onActionSelectSongNext()));
        connect(actionSelectSongPageUp,   SIGNAL(triggered()), SLOT(onActionSelectSongPageUp()));
        connect(actionSelectSongPageDown, SIGNAL(triggered()), SLOT(onActionSelectSongPageDown()));
        connect(actionSelectSongFirst,    SIGNAL(triggered()), SLOT(onActionSelectSongFirst()));
        connect(actionSelectSongLast,     SIGNAL(triggered()), SLOT(onActionSelectSongLast()));

        _interface->mediaPlayer()->setTickInterval(200);
        connect(_interface->mediaPlayer(), SIGNAL(tick(qint64)), SLOT(updateProgressBar()));
        connect(_interface->mediaPlayer(), SIGNAL(totalTimeChanged(qint64)), SLOT(updateProgressBar()));
        connect(_interface->mediaPlayer(), SIGNAL(currentSourceChanged(const Phonon::MediaSource &)), SLOT(updateCurrentSong()));

        connect(this->playPausePushButton, SIGNAL(clicked()), _interface, SLOT(play()));
        connect(this->nextPushButton, SIGNAL(clicked()), _interface, SLOT(next()));
        connect(this->prevPushButton, SIGNAL(clicked()), _interface, SLOT(prev()));
    }

    return true;
}

void CurrentlyPlayingScreen::onActionSelectSongPrev() {
}
void CurrentlyPlayingScreen::onActionSelectSongNext() {
}
void CurrentlyPlayingScreen::onActionSelectSongPageUp() {
}
void CurrentlyPlayingScreen::onActionSelectSongPageDown() {
}
void CurrentlyPlayingScreen::onActionSelectSongFirst() {
}
void CurrentlyPlayingScreen::onActionSelectSongLast() {
}



void CurrentlyPlayingScreen::updateProgressBar() {
    const int currentMs = _interface->mediaPlayer()->currentTime();
    const int totalMs =_interface->mediaPlayer()->totalTime();

    QTime current(0,0,0,0), total(0,0,0,0);
    current = current.addMSecs(currentMs);
    total = total.addMSecs(totalMs);

    songProgressBar->setValue( currentMs );
    songProgressBar->setMaximum( totalMs );

    // TODO: if the file longer than one hour, display it correctly!
    songProgressBar->setFormat( tr("%p% (%1 of %2)").arg(current.toString("m:ss"))
                                                    .arg(  total.toString("m:ss")) );
}

void CurrentlyPlayingScreen::updateCurrentSong() {
    MusicUrl musicUrl = _interface->currentSong();
    
    if (musicUrl.isEmpty()) {
        artistLabel->setText(tr("Artist: %1").arg( tr("No song playing") ));
        titleLabel->setText(tr("Title: %1").arg( tr("No song playing") ));
    } else {
        const QString artist = musicUrl.artist(); 
        const QString title = musicUrl.title();
        artistLabel->setText(tr("Artist: %1").arg(artist.isEmpty() ? tr("Artist tag missing") : artist ));
        titleLabel->setText(tr("Title: %1").arg(title.isEmpty() ? QFileInfo(musicUrl.toLocalFile()).fileName() : title));
    }
}


}}}
