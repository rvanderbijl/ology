#include <QtPlugin>
#include <FileDetector/Search>
#include <Ology/SimpleAction>

#include "ActionIds.h"
#include "Interface.h"
#include "Player.h"
#include "CurrentlyPlayingScreen.h"

namespace Ology {
namespace Plugin {
namespace MusicPhonon {

using namespace FileDetector;


Interface::Interface() : 
    _fileDetectorController(NULL),
    _player(new Player(this))
{
    registerSetting(_player->shuffleSetting());
    registerSetting(_player->repeatSetting());
}

AbstractPlayer* Interface::player() const {
    return const_cast<Player*>(_player); 
}

bool Interface::initialize(Ology::InitializePurpose initPurpose) {
    SimpleAction *actionPlay = new SimpleAction(Id::Action::MusicPhonon::Play, "Play", "Continue playing music", this);
    SimpleAction *actionStop = new SimpleAction(Id::Action::MusicPhonon::Stop, "Stop", "Stop playing music", this);
    SimpleAction *actionNext = new SimpleAction(Id::Action::MusicPhonon::Next, "Next song", "Skip to the next song", this);
    SimpleAction *actionPrev = new SimpleAction(Id::Action::MusicPhonon::Prev, "Previous song", "Go back to the previous song", this);
    SimpleAction *actionPlayOrPause = new SimpleAction(Id::Action::MusicPhonon::PlayOrPause, "Previous song", "Go back to the previous song", this);
    SimpleAction *actionPauseOrUnpause = new SimpleAction(Id::Action::MusicPhonon::PlayOrPause, "Previous song", "Go back to the previous song", this);

    SimpleAction *actionToggleShuffle    = new SimpleAction(Id::Action::MusicPhonon::ToggleShuffle, "Toggle Shuffle", "Toggle the random shuffle option", this);
    SimpleAction *actionToggleRepeatAll  = new SimpleAction(Id::Action::MusicPhonon::ToggleRepeatAll, "Toggle Repeat-All", "Toggle the repeat all option", this);
    SimpleAction *actionSetRandomShuffle = new SimpleAction(Id::Action::MusicPhonon::SetRandomShuffle, "Set Random Shuffle", "Turn on random shuffle", this);
    SimpleAction *actionSetNoShuffle     = new SimpleAction(Id::Action::MusicPhonon::SetNoShuffle, "Set No Shuffle", "Turn off random shuffle", this);
    SimpleAction *actionSetRepeatAll     = new SimpleAction(Id::Action::MusicPhonon::SetRepeatAll, "Set Repeat-All", "Turn on the repeat all option", this);
    SimpleAction *actionSetNoRepeatAll   = new SimpleAction(Id::Action::MusicPhonon::SetNoRepeatAll, "Set no Repeat-All", "Turn off the repeat all option", this);

    SimpleAction *actionPlayArtist = new SimpleAction(Id::Action::MusicPhonon::PlayArtist, "Play Artist", "Set the play list to all songs from the current artist", this);
    SimpleAction *actionPlayAlbum = new SimpleAction(Id::Action::MusicPhonon::PlayAlbum, "Play Album", "Set the play list to all songs from the current album", this);


    // TODO: actions: toggle shuffle, set-shuffle-none, set-shuffle-random 
    // TODO: actions: toggle repeat, set-repeat-none, set-repeat-all

    // only connect actions if they're going to be used ...
    if (initPurpose == Ology::RealUsage) {
        actionToggleShuffle->setShortcut(QKeySequence("1"));
        actionToggleRepeatAll->setShortcut(QKeySequence("2"));
        actionPlayArtist->setShortcut(QKeySequence("3"));
        actionPlayAlbum->setShortcut(QKeySequence("4"));

        connect(actionPlay, SIGNAL(triggered()), _player, SLOT(play()));
        connect(actionStop, SIGNAL(triggered()), _player, SLOT(stop()));
        connect(actionNext, SIGNAL(triggered()), _player, SLOT(next()));
        connect(actionPrev, SIGNAL(triggered()), _player, SLOT(prev()));
        connect(actionPlayOrPause, SIGNAL(triggered()), _player, SLOT(playOrPause()));
        connect(actionPauseOrUnpause, SIGNAL(triggered()), _player, SLOT(pauseOrUnpause()));

        connect(actionToggleShuffle, SIGNAL(triggered()), _player, SLOT(toggleShuffle()));
        connect(actionToggleRepeatAll, SIGNAL(triggered()), _player, SLOT(toggleRepeatAll()));
        connect(actionSetRandomShuffle, SIGNAL(triggered()), _player, SLOT(setRandomShuffle()));
        connect(actionSetNoShuffle, SIGNAL(triggered()), _player, SLOT(setNoShuffle()));
        connect(actionSetRepeatAll, SIGNAL(triggered()), _player, SLOT(setRepeatAll()));
        connect(actionSetNoRepeatAll, SIGNAL(triggered()), _player, SLOT(setNoRepeatAll()));

        connect(actionPlayArtist, SIGNAL(triggered()), this, SLOT(playArtist()));
        connect(actionPlayAlbum, SIGNAL(triggered()), this, SLOT(playAlbum()));

        _fileDetectorController = new FileDetector::WorkerThreadController(this);
        connect(_fileDetectorController, SIGNAL(dispatcherReady()), SLOT(onFileDetectorThreadReady()));
        _fileDetectorController->start();
    }

    return true;
}


void Interface::onFileDetectorThreadReady() {
    FileDetector::SearchParameters p;
    p.fileTypes << FileType("mp3") << FileType("ogg");
    p.directory = QDir::home();
    p.directory.cd("Music");
    p.searchCriteria = FileDetector::Recursive|FileDetector::IgnoreCase;

    FileDetector::Search *search = new FileDetector::Search(p, this);
    connect(search, SIGNAL(filesAdded(const QList<QUrl>&)), SLOT(onFilesFound(const QList<QUrl>&)));
    search->startOneTimeSearch(_fileDetectorController);
}

QStringList Interface::screenIds() {
    return QStringList() << "music-currently-playing" ;
}

AbstractScreen* Interface::createScreen(const QString &id, QWidget *parent) {
    if (id == "music-currently-playing") {
        return new CurrentlyPlayingScreen(this, parent);
    }
    return NULL;
}

QList<AbstractAction*> Interface::globalActions() {
    return findChildren<AbstractAction*>();
}

void Interface::onFilesFound(const QList<QUrl>& files) {
    foreach(const QUrl &url, files) {
        qDebug() << "file found:" << url;
        _masterSongList.append(url);
    }
    _player->setPlayList(_masterSongList, tr("All music"));
}

void Interface::playArtist() {
    const Song currentSong = _player->currentSong();
    if (currentSong.isEmpty()) { return; }

    QList<Song> newList;
    foreach(const Song &song, _masterSongList) {
        if (song.artist() == currentSong.artist()) {
            newList << song;
        }
    }

    if (newList.isEmpty()) {
        qDebug() << "No songs from this artist!";
        return;
    }

    _player->setPlayList(newList, tr("Artist: %1").arg(currentSong.artist()));
}

void Interface::playAlbum() {
    Song currentSong = _player->currentSong();
    if (currentSong.isEmpty()) { return; }

    QList<Song> newList;
    foreach(const Song &song, _masterSongList) {
        if ((song.artist() == currentSong.artist()) &&
            (song.album() == currentSong.album())) 
        {
            newList << song;
        }
    }

    if (newList.isEmpty()) {
        qDebug() << "No songs from this album!";
        return;
    }

    _player->setPlayList(newList, tr("Album: %1").arg(currentSong.album()));
}

}}}

Q_EXPORT_PLUGIN2("music-phonon", Ology::Plugin::MusicPhonon::Interface)
