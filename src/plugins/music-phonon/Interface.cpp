#include <QtPlugin>
#include <QMutexLocker>
#include <QtConcurrentRun>
#include <QFutureWatcher>

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
    _resortMasterSongListTimer.setSingleShot(true);
    _resortMasterSongListTimer.setInterval(100);
    connect(&_resortMasterSongListTimer, SIGNAL(timeout()), SLOT(resortMasterSongList()));
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
    SimpleAction *actionPlayAll = new SimpleAction(Id::Action::MusicPhonon::PlayAll, "Play All", "Set the play list to all songs", this);


    // TODO: actions: toggle shuffle, set-shuffle-none, set-shuffle-random 
    // TODO: actions: toggle repeat, set-repeat-none, set-repeat-all

    // only connect actions if they're going to be used ...
    if (initPurpose == Ology::RealUsage) {
        actionToggleShuffle->setShortcut(QKeySequence("1"));
        actionToggleRepeatAll->setShortcut(QKeySequence("2"));
        actionPlayArtist->setShortcut(QKeySequence("3"));
        actionPlayAlbum->setShortcut(QKeySequence("4"));
        actionPlayAll->setShortcut(QKeySequence("5"));

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

        connect(actionPlayArtist, SIGNAL(triggered()), this, SLOT(playCurrentArtist()));
        connect(actionPlayAlbum, SIGNAL(triggered()), this, SLOT(playCurrentAlbum()));

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

    FileDetector::Search *search = new FileDetector::Search(p, _fileDetectorController, this);
    connect(search, SIGNAL(filesAdded(const QList<QUrl>&)), SLOT(onFilesFound(const QList<QUrl>&)));
    connect(search, SIGNAL(filesRemoved(const QList<QUrl>&)), SLOT(onFilesRemoved(const QList<QUrl>&)));
    search->startSearch();
}

void Interface::onFilesFound(const QList<QUrl>& files) {
    foreach(const QUrl &url, files) {
        qDebug() << "Found song, queuing to add to master song list:" << url;
        QtConcurrent::run(this, &Interface::addSong, url);
    }

}


void Interface::addSong(const QUrl &url) {
    QMutexLocker locker(&_addedListMutex);
    _addedList.append(Song(url));
    _resortMasterSongListTimer.start();
}

void Interface::onFilesRemoved(const QList<QUrl>& files) {
    foreach(const QUrl &url, files) {
        qDebug() << "Removing file to master song list:" << url;
        _masterSongList.removeAll(url);
        // TODO: remove from current play list
    }
}

void Interface::resortMasterSongList() {
    // don't run the sort twice at the same time
    if (findChildren<QFutureWatcher<void>*>().count()) {
        _resortMasterSongListTimer.start();
        return;
    }
        
    QMutexLocker locker(&_addedListMutex);
    qDebug() << "Preparing to sort new master song list (in a separate thread)";
    _tempList = _masterSongList + _addedList;
    _addedList.clear();
    locker.unlock();

    QFutureWatcher<void> *fw = new QFutureWatcher<void>(this);
    connect(fw, SIGNAL(finished()), SLOT(masterSongListResorted())); 
    connect(fw, SIGNAL(finished()), fw, SLOT(deleteLater())); 
    fw->setFuture( QtConcurrent::run(&_tempList, &PlayList::sort) );
}

void Interface::masterSongListResorted() {
    qDebug() << "Master song list updated and sorted";
    _masterSongList = _tempList; // drops the list name

    PlayList list = _player->playList();
    if (list.isEmpty() || (list.type() == PlayList::Mixed && list.name() == tr("All music"))) {
        qDebug() << "Updating playlist to new master list of" << _masterSongList.count() << "songs";
        PlayList newList(_masterSongList);
        newList.setName(tr("All music"));
        _player->setPlayList(newList);
    }

    if (list.type() == PlayList::Artist) {
        PlayList newList = PlayList(_masterSongList).songsByArtist(list.artist());
        newList.setType(list.type());
        newList.setArtist(list.artist());
        newList.setName(list.name());
        if (newList.size() > _player->playList().size()) {
            qDebug() << "Updating playlist to include newly found matching songs";
            _player->setPlayList(newList);
        }
    }

    if (list.type() == PlayList::Album) {
        PlayList newList = PlayList(_masterSongList).songsInAlbum(list.artist(), list.album());
        newList.setType(list.type());
        newList.setArtist(list.artist());
        newList.setAlbum(list.album());
        newList.setName(list.name());
        if (newList.size() > _player->playList().size()) {
            qDebug() << "Updating playlist to include newly found matching songs";
            _player->setPlayList(newList);
        }
    }
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

void Interface::playCurrentArtist() {
    const Song currentSong = _player->currentSong();
    if (currentSong.isEmpty()) { return; }
    playArtist(currentSong.artist());
}

void Interface::playCurrentAlbum() {
    Song currentSong = _player->currentSong();
    if (currentSong.isEmpty()) { return; }
    playAlbum(currentSong.artist(), currentSong.album());
}

void Interface::playArtist(const QString &artist) {
    QList<Song> songs;
    PlayList list = PlayList(_masterSongList).songsByArtist(artist);
    _player->setPlayList(list);
}

void Interface::playAlbum(const QString &artist, const QString &album) {
    QList<Song> songs;
    PlayList list = PlayList(_masterSongList).songsInAlbum(artist, album);
    _player->setPlayList(list);
}

}}}

Q_EXPORT_PLUGIN2("music-phonon", Ology::Plugin::MusicPhonon::Interface)
