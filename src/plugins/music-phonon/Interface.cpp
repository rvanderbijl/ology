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


bool Interface::initialize(Ology::InitializePurpose initPurpose) {
    SimpleAction *actionPlay = new SimpleAction(Id::Action::MusicPhononPlay, "Play", "Continue playing music", this);
    SimpleAction *actionStop = new SimpleAction(Id::Action::MusicPhononStop, "Stop", "Stop playing music", this);
    SimpleAction *actionNext = new SimpleAction(Id::Action::MusicPhononNext, "Next song", "Skip to the next song", this);
    SimpleAction *actionPrev = new SimpleAction(Id::Action::MusicPhononPrev, "Previous song", "Go back to the previous song", this);

    SimpleAction *actionToggleShuffle    = new SimpleAction(Id::Action::MusicPhononToggleShuffle, "Toggle Shuffle", "Toggle the random shuffle option", this);
    SimpleAction *actionToggleRepeatAll  = new SimpleAction(Id::Action::MusicPhononToggleRepeatAll, "Toggle Repeat-All", "Toggle the repeat all option", this);
    SimpleAction *actionSetRandomShuffle = new SimpleAction(Id::Action::MusicPhononSetRandomShuffle, "Set Random Shuffle", "Turn on random shuffle", this);
    SimpleAction *actionSetNoShuffle     = new SimpleAction(Id::Action::MusicPhononSetNoShuffle, "Set No Shuffle", "Turn off random shuffle", this);
    SimpleAction *actionSetRepeatAll     = new SimpleAction(Id::Action::MusicPhononSetRepeatAll, "Set Repeat-All", "Turn on the repeat all option", this);
    SimpleAction *actionSetNoRepeatAll   = new SimpleAction(Id::Action::MusicPhononSetNoRepeatAll, "Set no Repeat-All", "Turn off the repeat all option", this);


    // TODO: actions: toggle shuffle, set-shuffle-none, set-shuffle-random 
    // TODO: actions: toggle repeat, set-repeat-none, set-repeat-all

    // only connect actions if they're going to be used ...
    if (initPurpose == Ology::RealUsage) {
        actionPlay->setShortcut(QKeySequence("Ctrl+P"));
        actionStop->setShortcut(QKeySequence("Ctrl+S"));
        actionNext->setShortcut(QKeySequence("Ctrl+L"));
        actionPrev->setShortcut(QKeySequence("Ctrl+H"));

        connect(actionPlay, SIGNAL(triggered()), _player, SLOT(play()));
        connect(actionStop, SIGNAL(triggered()), _player, SLOT(stop()));
        connect(actionNext, SIGNAL(triggered()), _player, SLOT(next()));
        connect(actionPrev, SIGNAL(triggered()), _player, SLOT(prev()));

        connect(actionToggleShuffle, SIGNAL(triggered()), _player, SLOT(toggleShuffle()));
        connect(actionToggleRepeatAll, SIGNAL(triggered()), _player, SLOT(toggleRepeatAll()));
        connect(actionSetRandomShuffle, SIGNAL(triggered()), _player, SLOT(setRandomShuffle()));
        connect(actionSetNoShuffle, SIGNAL(triggered()), _player, SLOT(setNoShuffle()));
        connect(actionSetRepeatAll, SIGNAL(triggered()), _player, SLOT(setRepeatAll()));
        connect(actionSetNoRepeatAll, SIGNAL(triggered()), _player, SLOT(setNoRepeatAll()));

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
    _player->setPlayList(_masterSongList);
}


}}}

Q_EXPORT_PLUGIN2("music-phonon", Ology::Plugin::MusicPhonon::Interface)
