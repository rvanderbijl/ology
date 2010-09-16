#include <QtPlugin>
#include <Phonon/AudioOutput>
#include <FileDetector/Search>
#include <Ology/SimpleAction>

#include "ActionIds.h"
#include "Interface.h"
#include "CurrentlyPlayingScreen.h"

namespace Ology {
namespace Plugin {
namespace MusicPhonon {

using namespace FileDetector;


Interface::Interface() : 
    _mediaPlayer(NULL),
    _fileDetectorController(NULL),
    _shuffleSetting(tr("music-phonon"), tr("shuffle"), RandomShuffle, metaObject()->className(), "Shuffle", "Shuffle music setting", this),
    _repeatSetting(tr("music-phonon"), tr("repeat"), RepeatAll, metaObject()->className(), "Repeat", "Repeat playlist setting", this)
{
    qRegisterMetaTypeStreamOperators<Ology::Plugin::MusicPhonon::Interface::Shuffle>("Ology::Plugin::MusicPhonon::Interface::Shuffle");
    qRegisterMetaTypeStreamOperators<Ology::Plugin::MusicPhonon::Interface::Repeat>("Ology::Plugin::MusicPhonon::Interface::Repeat");

    registerSetting(&_shuffleSetting);
    registerSetting(&_repeatSetting);
}


bool Interface::initialize(Ology::InitializePurpose initPurpose) {
    if (initPurpose == Ology::RealUsage) {
        _mediaPlayer = new Phonon::MediaObject(this);

        Phonon::AudioOutput *audioOutput = new Phonon::AudioOutput(Phonon::VideoCategory, this);
        Phonon::createPath(_mediaPlayer, audioOutput);
        connect(_mediaPlayer, SIGNAL(aboutToFinish()), SLOT(next()));

        _fileDetectorController = new FileDetector::WorkerThreadController(this);
        connect(_fileDetectorController, SIGNAL(dispatcherReady()), SLOT(onFileDetectorThreadReady()));
        _fileDetectorController->start();
    }
    

    SimpleAction *actionPlay = new SimpleAction(Id::Action::MusicPhononPlay, "Play", "Continue playing music", this);
    SimpleAction *actionStop = new SimpleAction(Id::Action::MusicPhononStop, "Stop", "Stop playing music", this);
    SimpleAction *actionNext = new SimpleAction(Id::Action::MusicPhononNext, "Next song", "Skip to the next song", this);
    SimpleAction *actionPrev = new SimpleAction(Id::Action::MusicPhononPrev, "Previous song", "Go back to the previous song", this);

    // TODO: actions: toggle shuffle, set-shuffle-none, set-shuffle-random 
    // TODO: actions: toggle repeat, set-repeat-none, set-repeat-all

    // only connect actions if they're going to be used ...
    if (initPurpose == Ology::RealUsage) {
        // TODO:
        actionPlay->setShortcut(QKeySequence("Ctrl+P"));
        actionStop->setShortcut(QKeySequence("Ctrl+S"));
        actionNext->setShortcut(QKeySequence("Ctrl+L"));
        actionPrev->setShortcut(QKeySequence("Ctrl+H"));

        connect(actionPlay, SIGNAL(triggered()), SLOT(play()));
        connect(actionStop, SIGNAL(triggered()), SLOT(stop()));
        connect(actionNext, SIGNAL(triggered()), SLOT(next()));
        connect(actionPrev, SIGNAL(triggered()), SLOT(prev()));
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
        _masterMusicList.append(url);
        _currentPlayList.append(PlayEntry(_masterMusicList.size() - 1, _currentPlayList.size() - 1));
    }
}


MusicUrl Interface::currentSong() const { 
    if (_history.isEmpty()) { return MusicUrl(); }
    return song(_history.current());
}
MusicUrl Interface::song(const PlayEntry & entry) const {
    return entry.isNull() ? MusicUrl() : _masterMusicList[entry.masterListIndex()];
}

void Interface::play() {
    if (_mediaPlayer->queue().isEmpty()) { 
        next(); 
        const PlayEntry pe = _currentPlayList.current();
        const QUrl url = _masterMusicList[pe.masterListIndex()];
        _mediaPlayer->enqueue( Phonon::MediaSource(url) );
    }
    _mediaPlayer->play();
}

void Interface::stop() {
    _mediaPlayer->stop();
}

void Interface::next() {
    if (_currentPlayList.isEmpty()) { return; } // no songs

    // Verify there is a next song to goto:
    // If we've gone into the history, we always have a next song. 
    // If we're in Repeat-All mode, we always have a next song.
    // Otherwise we divide the size of the history compared to
    //    the size of the playlist. If there is a remainder (% operator),
    //    then there are more songs. Else we've gone through all entries
    //    in the play-list. 
    //    (Exception: if no history, we've not started, so there is a next.)
    // (This works for both shuffle modes because for both modes we can only go through the list once.)
    if (_currentPlayList.isEmpty()) { return; }
    bool hasNext = false;
    if (_history.hasNext()) { hasNext = true; }
    else if (_repeatSetting == RepeatAll) { hasNext = true; }
    else {
        if (_shuffleSetting == RandomShuffle) {
            hasNext = (_history.size() < _currentPlayList.size());
        } else {
            hasNext = _currentPlayList.hasNext();
        }
    }
    if (!hasNext) { return; }


    // okay, switch to the next song
    PlayEntry nextSong;
    if (_history.hasNext()) {
        nextSong = _history.next();
    } else {
        // next in play list
        nextSong = _shuffleSetting == RandomShuffle ? _currentPlayList.random() : _currentPlayList.next();
        _history.append(nextSong);
    }

    const QUrl url = _masterMusicList[nextSong.masterListIndex()];

    if (_mediaPlayer->state() == Phonon::PlayingState) {
        Phonon::MediaSource source(url);
        _mediaPlayer->enqueue(source);
        _mediaPlayer->setCurrentSource(source);
        _mediaPlayer->play();
        _mediaPlayer->clearQueue();
    }
}

void Interface::prev() {
    if (_currentPlayList.isEmpty()) { return; } // no songs

    // can we go to a previous song?
    bool hasPrevious = false;
    if (_history.hasPrevious()) { hasPrevious = true; }
    else if (_repeatSetting == RepeatAll) { hasPrevious = true; }
    else {
        if (_shuffleSetting == RandomShuffle) {
            hasPrevious = true;
        } else {
            hasPrevious = _currentPlayList.hasPrevious();
        }
    }
    if (!hasPrevious) { return; }
    

    // figure out what the previous song is
    PlayEntry entry;
    if (_history.hasPrevious()) {
        entry = _history.previous();
        _currentPlayList.setCurrent(entry);
    } else {
        entry = _shuffleSetting == RandomShuffle ? _currentPlayList.random() : _currentPlayList.previous();
        _history.prepend(entry);
    }

    const QUrl url = _masterMusicList[entry.masterListIndex()];

    // play it
    if (_mediaPlayer->state() == Phonon::PlayingState) {
        Phonon::MediaSource source(url);
        _mediaPlayer->enqueue(source);
        _mediaPlayer->setCurrentSource(source);
        _mediaPlayer->play();
        _mediaPlayer->clearQueue();
    }
}




}}}

OLOGY_DECLARE_OPERATORS_FOR_ENUM(Ology::Plugin::MusicPhonon::Interface, Shuffle);
OLOGY_DECLARE_OPERATORS_FOR_ENUM(Ology::Plugin::MusicPhonon::Interface, Repeat);
Q_EXPORT_PLUGIN2("music-phonon", Ology::Plugin::MusicPhonon::Interface)
