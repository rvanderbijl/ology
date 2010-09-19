#ifndef OLOGY_PLUGIN_MENUS_INTERFACE
#define OLOGY_PLUGIN_MENUS_INTERFACE

#include <QObject>
#include <QUrl>
#include <Ology/Setting>
#include <Ology/Plugin/ScreenProviderInterface>
#include <Ology/Plugin/PlayerProviderInterface>
#include <Ology/Plugin/InfoInterface>

#include <FileDetector/WorkerThreadController>

#include "Song.h"

namespace Ology {
namespace Plugin {
namespace MusicPhonon {

class Player;

class Interface : 
    public QObject,
    public Ology::Plugin::InfoInterface,
    public Ology::Plugin::PlayerProviderInterface,
    public Ology::Plugin::ScreenProviderInterface
{
    Q_OBJECT
    Q_INTERFACES(Ology::Plugin::InfoInterface Ology::Plugin::ScreenProviderInterface Ology::Plugin::PlayerProviderInterface)


public:
    Interface();

    virtual QString name() const { return "Music-Phonon"; }
    virtual QString version() const { return "0.1"; }
    virtual QString description() const { return "Music player using Qt Phonon"; }

    virtual bool initialize(Ology::InitializePurpose initPurpose);
    virtual QStringList screenIds() ;
    virtual AbstractScreen* createScreen(const QString &id, QWidget *parent);
    virtual QList<AbstractAction*> globalActions();

    virtual Player* realPlayer() const { return _player; }
    virtual AbstractPlayer* player() const;

// player interface:
/*
public slots:
    virtual void play();
    virtual void play(const PlayEntry &entry);
    virtual void stop();
    virtual void next();
    virtual void prev();
*/

private slots:
    void onFileDetectorThreadReady();
    void onFilesFound(const QList<QUrl>& files);

    void playArtist();
    void playAlbum();

private:
    FileDetector::WorkerThreadController *_fileDetectorController;
    QList<Song> _masterSongList;
    Player *_player;
};


}}}

#endif
