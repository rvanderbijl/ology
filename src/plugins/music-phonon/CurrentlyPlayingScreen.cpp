#include <QDebug>
#include <QFileInfo>
#include <QTime>
#include <QKeyEvent>
#include <QPushButton>
#include <Ology/AbstractAction>
#include <Ology/SimpleAction>
#include <Ology/ManagerInterface>

#include "Interface.h"
#include "CurrentlyPlayingScreen.h"
#include "PlayListModel.h"

namespace Ology {
namespace Plugin {
namespace MusicPhonon {

CurrentlyPlayingScreen::CurrentlyPlayingScreen(Interface *interface, QWidget *parent) :
    AbstractScreen(parent),
    _interface(interface),
    _resetViewTimeout(tr("music-phonon"), tr("reset-view-timeout"), 2*60*1000, metaObject()->className(), "Reset view timeout", "The amount of time to reset the song list to the current  song (in ms)", this)
{
    _resetViewTimer.setSingleShot(true);
    _resetViewTimer.setInterval(_resetViewTimeout.value());
    connect(&_resetViewTimer, SIGNAL(timeout()), SLOT(resetViewToCurrentSong()));
    hide();
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

        connect(this->playPausePushButton, SIGNAL(clicked()), _interface, SLOT(play()));
        connect(this->nextPushButton, SIGNAL(clicked()), _interface, SLOT(next()));
        connect(this->prevPushButton, SIGNAL(clicked()), _interface, SLOT(prev()));

        PlayListModel *model = new PlayListModel(_interface->currentPlayList(), _interface, this);
        currentPlayListTreeView->setModel(model);
        connect(currentPlayListTreeView, SIGNAL(activated(const QModelIndex &)), SLOT(onSongActivated(const QModelIndex&)));
        connect(currentPlayListTreeView, SIGNAL(activated(const QModelIndex &)), &_resetViewTimer, SLOT(stop()));
        connect(currentPlayListTreeView->selectionModel(), SIGNAL(currentChanged(const QModelIndex &, const QModelIndex &)), SLOT(maybeStartResetViewTimer()));

        _interface->mediaPlayer()->setTickInterval(200);

        connect(_interface->mediaPlayer(), SIGNAL(tick(qint64)), SLOT(updateProgressBar()));
        connect(_interface->mediaPlayer(), SIGNAL(totalTimeChanged(qint64)), SLOT(updateProgressBar()));
        connect(_interface->mediaPlayer(), SIGNAL(currentSourceChanged(const Phonon::MediaSource &)), SLOT(updateCurrentSong()));
        connect(_interface, SIGNAL(currentSongChanged()), SLOT(updateCurrentSong()));

        updateProgressBar();
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
    PlayEntry entry = _interface->currentSong();
    if (entry.playListIndex() != currentPlayListTreeView->currentIndex().row()) {
        _resetViewTimer.start();
    }
}

void CurrentlyPlayingScreen::resetViewToCurrentSong() {
    PlayEntry entry = _interface->currentSong();
    Q_ASSERT(currentPlayListTreeView);
    Q_ASSERT(currentPlayListTreeView->model());
    currentPlayListTreeView->setCurrentIndex( currentPlayListTreeView->model()->index(entry.playListIndex(), 0) );
}

void CurrentlyPlayingScreen::onSongActivated(const QModelIndex& index) {
    PlayEntry entry = index.data(Qt::UserRole).value<PlayEntry>();
    _interface->play(entry);
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
    MusicUrl musicUrl = _interface->song(_interface->currentSong());
    if (!_resetViewTimer.isActive()) {
        resetViewToCurrentSong();
    }
    
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
