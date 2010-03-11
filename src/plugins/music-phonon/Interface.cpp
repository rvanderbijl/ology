#include <QtPlugin>
#include <Phonon/AudioOutput>
#include <FileDetector/Search>
#include <Ology/SimpleAction>

#include "Interface.h"

namespace Ology {
namespace Plugin {
namespace MusicPhonon {

using namespace FileDetector;

bool Interface::initialize(Ology::InitializePurpose initPurpose) {
    Q_UNUSED(initPurpose);

    if (initPurpose == Ology::RealUsage) {
        _mediaPlayer = new Phonon::MediaObject(this);

        Phonon::AudioOutput *audioOutput = new Phonon::AudioOutput(Phonon::VideoCategory, this);
        Phonon::createPath(_mediaPlayer, audioOutput);



        _fileDetectorController = new FileDetector::WorkerThreadController(this);
        _fileDetectorController->start();

        FileDetector::SearchParameters p;
        p.fileTypes << FileType("mp3") << FileType("ogg");
        p.directory = QDir::home();
        p.directory.cd("Music");
        p.searchCriteria = FileDetector::Recursive|FileDetector::IgnoreCase;

        FileDetector::Search *search = new FileDetector::Search(p, this);
        connect(search, SIGNAL(filesAdded(const QList<QUrl>&)), SLOT(filesFound(const QList<QUrl>&)));
        search->startOneTimeSearch(_fileDetectorController);
    }
    

    SimpleAction *actionPlay = new SimpleAction("Play", "Continue playing music", this);
    SimpleAction *actionStop = new SimpleAction("Stop", "Stop playing music", this);
    SimpleAction *actionNext = new SimpleAction("Next song", "Skip to the next song", this);
    SimpleAction *actionPrev = new SimpleAction("Previous song", "Go back to the previous song", this);

    actionPlay->setShortcut(QKeySequence("Ctrl+P"));


    // only connect actions if they're going to be used ...
    if (initPurpose == Ology::RealUsage) {
        connect(actionPlay, SIGNAL(triggered()), _mediaPlayer, SLOT(play()));
        connect(actionStop, SIGNAL(triggered()), _mediaPlayer, SLOT(stop()));
        connect(actionNext, SIGNAL(triggered()), this, SLOT(nextSong()));
        connect(actionPrev, SIGNAL(triggered()), this, SLOT(previousSong()));
    }

    return true;
}

QStringList Interface::screenIds() {
    return QStringList() << "music-phonon" ;
}

AbstractScreen* Interface::createScreen(const QString &id, QWidget *parent) {
    Q_UNUSED(id);
    Q_UNUSED(parent);
    return NULL;
}

QList<AbstractAction*> Interface::globalActions() {
    return findChildren<AbstractAction*>();
}




void Interface::filesFound(const QList<QUrl>& files) {
    _files.append(files);
}

void Interface::nextSong() {
}

void Interface::previousSong() {
}




}}}

Q_EXPORT_PLUGIN2("music-phonon", Ology::Plugin::MusicPhonon::Interface)
