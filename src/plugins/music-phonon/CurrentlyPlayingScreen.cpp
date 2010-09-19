#include <QDebug>
#include <QFileInfo>
#include <QTime>
#include <QKeyEvent>
#include <QPushButton>
#include <Ology/AbstractAction>
#include <Ology/SimpleAction>
#include <Ology/ManagerInterface>

#include "Interface.h"
#include "Player.h"
#include "CurrentlyPlayingScreen.h"
#include "ActionIds.h"

namespace Ology {
namespace Plugin {
namespace MusicPhonon {

CurrentlyPlayingScreen::CurrentlyPlayingScreen(Interface *interface, QWidget *parent) :
    AbstractScreen(parent),
    _interface(interface),
    _resetViewTimeout(tr("music-phonon"), tr("reset-view-timeout"), 90*1000, 
                      metaObject()->className(), "Reset view timeout", "The amount of time to reset the song list to the current song (in ms)", this)
{
    _resetViewTimer.setSingleShot(true);
    _resetViewTimer.setInterval(_resetViewTimeout.value());
    connect(&_resetViewTimer, SIGNAL(timeout()), SLOT(resetViewToCurrentSong()));
    hide();
}

QList<AbstractAction*> CurrentlyPlayingScreen::moreActions() const {
    QList<AbstractAction*> list;
    list << _interface->action(Id::Action::MusicPhononPlay)
         << _interface->action(Id::Action::MusicPhononStop)
         << _interface->action(Id::Action::MusicPhononNext)
         << _interface->action(Id::Action::MusicPhononPrev)
         << _interface->action(Id::Action::MusicPhononToggleShuffle)
         << _interface->action(Id::Action::MusicPhononToggleRepeatAll)
         << _interface->action(Id::Action::MusicPhononPlayArtist)
         << _interface->action(Id::Action::MusicPhononPlayAlbum)
         ;
    return list;
}

bool CurrentlyPlayingScreen::initialize(Ology::InitializePurpose initPurpose) {
    SimpleAction *actionSelectSongPrev = new SimpleAction("prev", "Select Previous Song", "", this);
    SimpleAction *actionSelectSongNext = new SimpleAction("next", "Select Next Song", "", this);
    SimpleAction *actionSelectSongPageUp = new SimpleAction("page-up", "Move select a page up", "", this);
    SimpleAction *actionSelectSongPageDown = new SimpleAction("page-down", "Move select a page down", "", this);
    SimpleAction *actionSelectSongFirst = new SimpleAction("first", "Select first song", "", this);
    SimpleAction *actionSelectSongLast = new SimpleAction("last", "Select last song", "", this);

    if (initPurpose == Ology::RealUsage) {
        setupUi(this);

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

        connect(this->playPausePushButton, SIGNAL(clicked()), _interface->player(), SLOT(play()));
        connect(this->stopPushButton, SIGNAL(clicked()), _interface->player(), SLOT(stop()));
        connect(this->nextPushButton, SIGNAL(clicked()), _interface->player(), SLOT(next()));
        connect(this->prevPushButton, SIGNAL(clicked()), _interface->player(), SLOT(prev()));

        currentPlayListTreeView->setModel(_interface->player());
        connect(currentPlayListTreeView, SIGNAL(activated(const QModelIndex &)), _interface->player(), SLOT(play(const QModelIndex&)));
        connect(currentPlayListTreeView, SIGNAL(activated(const QModelIndex &)), &_resetViewTimer, SLOT(stop()));
        connect(currentPlayListTreeView->selectionModel(), SIGNAL(currentChanged(const QModelIndex &, const QModelIndex &)), SLOT(maybeStartResetViewTimer()));

        connect(_interface->player(), SIGNAL(currentSongChanged(const Ology::Plugin::MusicPhonon::Song &)), SLOT(updateCurrentSong()));
        connect(_interface->player(), SIGNAL(songProgressChanged(qint64)), SLOT(onSongProgressChanged(qint64)));
        connect(_interface->player(), SIGNAL(songLengthChanged(qint64)), SLOT(onSongLengthChanged(qint64)));

        connect(_interface->player()->repeatSetting(), SIGNAL(valueChanged()), SLOT(updatePlayListInfo()));
        connect(_interface->player()->shuffleSetting(), SIGNAL(valueChanged()), SLOT(updatePlayListInfo()));
        connect(_interface->player(), SIGNAL(modelReset()), SLOT(updatePlayListInfo()));
        connect(_interface->player(), SIGNAL(rowsInserted(const QModelIndex &, int, int)), SLOT(updatePlayListInfo()));
        connect(_interface->player(), SIGNAL(rowsRemoved(const QModelIndex &, int, int)), SLOT(updatePlayListInfo()));

        updatePlayListInfo();
        updateProgressBarText();
        updateCurrentSong();
    }

    return true;
}

void CurrentlyPlayingScreen::onActionSelectSongPrev() {
    qDebug() << "Select previous song";
    QKeyEvent *event = new QKeyEvent(QEvent::KeyPress, Qt::Key_Up, Qt::NoModifier);
    QApplication::sendEvent(currentPlayListTreeView, event);
}
void CurrentlyPlayingScreen::onActionSelectSongNext() {
    qDebug() << "Select next song";
    QKeyEvent *event = new QKeyEvent(QEvent::KeyPress, Qt::Key_Down, Qt::NoModifier);
    QApplication::sendEvent(currentPlayListTreeView, event);
}
void CurrentlyPlayingScreen::onActionSelectSongPageUp() {
    qDebug() << "Select song page up";
    QKeyEvent *event = new QKeyEvent(QEvent::KeyPress, Qt::Key_PageUp, Qt::NoModifier);
    QApplication::sendEvent(currentPlayListTreeView, event);
}
void CurrentlyPlayingScreen::onActionSelectSongPageDown() {
    qDebug() << "Select song page down";
    QKeyEvent *event = new QKeyEvent(QEvent::KeyPress, Qt::Key_PageDown, Qt::NoModifier);
    QApplication::sendEvent(currentPlayListTreeView, event);
}
void CurrentlyPlayingScreen::onActionSelectSongFirst() {
    qDebug() << "Select song page first";
    QKeyEvent *event = new QKeyEvent(QEvent::KeyPress, Qt::Key_Home, Qt::NoModifier);
    QApplication::sendEvent(currentPlayListTreeView, event);
}
void CurrentlyPlayingScreen::onActionSelectSongLast() {
    qDebug() << "Select song page last";
    QKeyEvent *event = new QKeyEvent(QEvent::KeyPress, Qt::Key_End, Qt::NoModifier);
    QApplication::sendEvent(currentPlayListTreeView, event);
}

void CurrentlyPlayingScreen::maybeStartResetViewTimer() {
    Song song = _interface->player()->currentSong();
    if (song != currentPlayListTreeView->currentIndex().data(Qt::UserRole).value<Song>()) {
        _resetViewTimer.start();
    }
}

void CurrentlyPlayingScreen::resetViewToCurrentSong() {
    Q_ASSERT(currentPlayListTreeView);
    Q_ASSERT(currentPlayListTreeView->model());
    currentPlayListTreeView->setCurrentIndex( _interface->player()->currentSongIndex() );
}


void CurrentlyPlayingScreen::updateProgressBarText() {
    Song song = _interface->player()->currentSong();
    if (song.isEmpty()) { 
        qDebug() << "No song ... ";
        songProgressBar->setFormat("");
        return;
    }

    const int currentMs = songProgressBar->value();
    const int totalMs = songProgressBar->maximum();

    QTime current(0,0,0,0), total(0,0,0,0);
    current = current.addMSecs(currentMs);
    total = total.addMSecs(totalMs);

    // TODO: if the file longer than one hour, display it correctly!
    songProgressBar->setFormat( tr("%p% (%1 of %2)").arg(current.toString("m:ss"))
                                                    .arg(  total.toString("m:ss")) );
}

void CurrentlyPlayingScreen::onSongProgressChanged(qint64 progress) {
    songProgressBar->setValue( progress );
    updateProgressBarText();
}
void CurrentlyPlayingScreen::onSongLengthChanged(qint64 length) {
    songProgressBar->setMaximum( length );
    updateProgressBarText();
}


void CurrentlyPlayingScreen::updateCurrentSong() {
    Song song = _interface->player()->currentSong();
    if (!_resetViewTimer.isActive()) {
        resetViewToCurrentSong();
    }
    
    if (song.isEmpty()) {
        artistLabel->setText(tr("Artist: %1").arg( tr("No song playing") ));
        titleLabel->setText(tr("Title: %1").arg( tr("No song playing") ));
    } else {
        const QString artist = song.artist(); 
        const QString title = song.title();
        artistLabel->setText(tr("Artist: %1").arg(artist.isEmpty() ? tr("Artist tag missing") : artist ));
        titleLabel->setText(tr("Title: %1").arg(title.isEmpty() ? QFileInfo(song.toLocalFile()).fileName() : title));
    }
}

void CurrentlyPlayingScreen::updatePlayListInfo() {
    Setting<Player::Repeat> *repeat = dynamic_cast<Setting<Player::Repeat>*>(_interface->player()->repeatSetting());
    Setting<Player::Shuffle> *shuffle = dynamic_cast<Setting<Player::Shuffle>*>(_interface->player()->shuffleSetting());

    currentPlayListGroupBox->setTitle(tr("Current PlayList: %1, %2 Songs, %3, %4")
          .arg(_interface->player()->playListName())
          .arg(_interface->player()->playList().size())
          .arg(repeat->value() == Player::RepeatAll ? tr("Repeat all") : tr("No repeat"))
          .arg(shuffle->value() == Player::RandomShuffle ? tr("Random Shuffle") : tr("In order")));
}

}}}
