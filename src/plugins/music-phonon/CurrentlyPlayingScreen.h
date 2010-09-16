#ifndef OLOGY_PLUGIN_MENUS_MENU_SCREEN
#define OLOGY_PLUGIN_MENUS_MENU_SCREEN

#include <Ology/AbstractScreen>
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


    void updateCurrentSong();
    void updateProgressBar();

private:
    Interface *_interface;
};


}}}

#endif
