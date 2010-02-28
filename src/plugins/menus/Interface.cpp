#include <QtPlugin>
#include "Interface.h"
#include "MenuScreen.h"
#include <Ology/DisplayScreenAction>

namespace Ology {
namespace Plugin {
namespace Menus {

bool Interface::initialize() {
    // TODO: read from settings
    return true;
}

QStringList Interface::screenIds() {
    return QStringList() 
            << "menu-main" 
            << "menu-listen"
            << "menu-watch"
            << "menu-play"
            ;
}

AbstractScreen* Interface::createScreen(const QString &id, QWidget *parent) {
    clearErrorString();
    MenuScreen *screen = new MenuScreen(id, parent);

    // TODO: create screen from settings

    if (id == "menu-main") {
        screen->setMenuActions( QList<AbstractAction*>() 
                                    << new DisplayScreenAction("menu-listen", screen)
                                    << new DisplayScreenAction("menu-watch", screen)
                                    << new DisplayScreenAction("menu-play", screen)
                            );
    } else if (id == "menu-listen") {
        screen->setMenuActions( QList<AbstractAction*>() 
                                << new DisplayScreenAction("music-currently-playing", screen)
                                << new DisplayScreenAction("music-select-playlist", screen)
                                << new DisplayScreenAction("music-manager-playlists", screen)
                            );
    } else if (id == "menu-watch") {
        screen->setMenuActions( QList<AbstractAction*>() 
                                << new DisplayScreenAction("watch-tv", screen)
                                << new DisplayScreenAction("watch-videos", this)
                                << new DisplayScreenAction("watch-movie-trailers", screen)
                            );
    } else if (id == "menu-play") {
        screen->setMenuActions( QList<AbstractAction*>() 
                                << new DisplayScreenAction("menu-listen", screen)
                                << new DisplayScreenAction("menu-watch", screen)
                                << new DisplayScreenAction("games", screen)
                                << new DisplayScreenAction("with-yourself", screen)
                            );
    } else {
        setErrorString(tr("Unknown menu id: %1").arg(id));
        delete screen;
        screen = NULL;
        return NULL;   
    }

    return screen;
}

QList<AbstractAction*> Interface::globalActions() {
    return QList<AbstractAction*>();
}


/* For now:

MainMenu:
- Listen
    - Currently Playing
    - Select Playlist
    - Manager Playlist
- Watch
    - Trailers
    - TV
    - Videos
- Play
    - Music -> Listen
    - Movies -> Watch
    - With Yourself
    - Games
*/




}}}

Q_EXPORT_PLUGIN2("menus", Ology::Plugin::Menus::Interface)
