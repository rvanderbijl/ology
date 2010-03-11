#ifndef OLOGY_PLUGIN_MENUS_INTERFACE
#define OLOGY_PLUGIN_MENUS_INTERFACE

#include <QObject>
#include <QUrl>
#include <Ology/Plugin/ScreenInterface>
#include <Ology/Plugin/InfoInterface>

#include <FileDetector/WorkerThreadController>

#include <Phonon/MediaObject>

namespace Ology {
namespace Plugin {
namespace MusicPhonon {

class Interface : 
    public QObject,
    public Ology::Plugin::InfoInterface,
    public Ology::Plugin::ScreenInterface 
{
    Q_OBJECT
    Q_INTERFACES(Ology::Plugin::InfoInterface Ology::Plugin::ScreenInterface)
public:
    Interface() : _mediaPlayer(NULL), _fileDetectorController(NULL) {}

    virtual QString name() const { return "Music-Phonon"; }
    virtual QString version() const { return "0.1"; }
    virtual QString description() const { return "Music player using Qt Phonon"; }

    virtual bool initialize(Ology::InitializePurpose initPurpose);
    virtual QStringList screenIds() ;
    virtual AbstractScreen* createScreen(const QString &id, QWidget *parent);
    virtual QList<AbstractAction*> globalActions();

private slots:
    void filesFound(const QList<QUrl>& files);

    void nextSong();
    void previousSong();

private:
    Phonon::MediaObject *_mediaPlayer;
    FileDetector::WorkerThreadController *_fileDetectorController;
    QList<QUrl> _files;
};


}}}

#endif
