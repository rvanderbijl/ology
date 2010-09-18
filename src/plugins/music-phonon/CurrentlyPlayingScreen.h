#ifndef OLOGY_PLUGIN_MENUS_MENU_SCREEN
#define OLOGY_PLUGIN_MENUS_MENU_SCREEN

#include <QTimer>
#include <Ology/AbstractScreen>
#include <Ology/Setting>
#include "ui_currentlyplaying.h"

namespace Ology {
namespace Plugin {
namespace MusicPhonon {
class Interface;

class CurrentlyPlayingScreen : public AbstractScreen, private Ui::CurrentlyPlayingWidget {
    Q_OBJECT
public:
    CurrentlyPlayingScreen(Interface *interface, QWidget *parent);
    
    virtual QString id() const { return "music-currently-playing"; }
    virtual QString name() const { return "Music Currently Playing"; }
    virtual QString description() const { return "Shows currently playing music and allow basic control of which song is playing"; }

    virtual bool initialize(Ology::InitializePurpose initPurpose);

private slots:
    void onActionSelectSongPrev();
    void onActionSelectSongNext();
    void onActionSelectSongPageUp();
    void onActionSelectSongPageDown();
    void onActionSelectSongFirst();
    void onActionSelectSongLast();


    void onSongProgressChanged(qint64 progress);
    void onSongLengthChanged(qint64 length);
    void updateCurrentSong();
    void updateProgressBarText();

    void maybeStartResetViewTimer();
    void resetViewToCurrentSong();

private:
    Interface *_interface;
    QTimer _resetViewTimer;
    Setting<int> _resetViewTimeout;
};


}}}

#endif
