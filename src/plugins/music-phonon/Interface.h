#ifndef OLOGY_PLUGIN_MENUS_INTERFACE
#define OLOGY_PLUGIN_MENUS_INTERFACE

#include <QObject>
#include <QUrl>
#include <Ology/Setting>
#include <Ology/Plugin/ScreenProviderInterface>
#include <Ology/Plugin/PlayerInterface>
#include <Ology/Plugin/InfoInterface>

#include <FileDetector/WorkerThreadController>

#include <Phonon/MediaObject>

#include "MusicUrl.h"
#include "PlayHistory.h"
#include "PlayList.h"

namespace Ology {
namespace Plugin {
namespace MusicPhonon {

class Interface : 
    public QObject,
    public Ology::Plugin::InfoInterface,
    public Ology::Plugin::PlayerInterface,
    public Ology::Plugin::ScreenProviderInterface
{
    Q_OBJECT
    Q_INTERFACES(Ology::Plugin::InfoInterface Ology::Plugin::ScreenProviderInterface Ology::Plugin::PlayerInterface)

public:
    enum Shuffle { NoShuffle, RandomShuffle };
    enum Repeat { RepeatNone, RepeatAll };
    Q_ENUMS(Shuffle Repeat)

public:
    Interface();

    virtual QString name() const { return "Music-Phonon"; }
    virtual QString version() const { return "0.1"; }
    virtual QString description() const { return "Music player using Qt Phonon"; }

    virtual bool initialize(Ology::InitializePurpose initPurpose);
    virtual QStringList screenIds() ;
    virtual AbstractScreen* createScreen(const QString &id, QWidget *parent);
    virtual QList<AbstractAction*> globalActions();

    Phonon::MediaObject* mediaPlayer() { return _mediaPlayer; }
    MusicUrl currentSong() const;

// player interface:
public slots:
    virtual void play();
    virtual void stop();
    virtual void next();
    virtual void prev();

private slots:
    void onFileDetectorThreadReady();
    void onFilesFound(const QList<QUrl>& files);


    int getNextSongIndex();

private:
    Phonon::MediaObject *_mediaPlayer;
    FileDetector::WorkerThreadController *_fileDetectorController;

    Setting<Shuffle> _shuffleSetting;
    Setting<Repeat> _repeatSetting;

    QList<MusicUrl> _masterMusicList;
    PlayList _currentPlayList;
    PlayHistory _history;
};


}}}

Q_DECLARE_METATYPE(Ology::Plugin::MusicPhonon::Interface::Shuffle);
Q_DECLARE_METATYPE(Ology::Plugin::MusicPhonon::Interface::Repeat);

#endif
