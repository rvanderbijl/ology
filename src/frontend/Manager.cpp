#include <Ology/Plugin/ScreenProviderInterface>
#include <Ology/AbstractScreen>
#include <Ology/AbstractPlayer>
#include <Ology/Core/CloseScreenAction>
#include <Ology/CoreIds>
#include "../lib/Core/PseudoPluginInterface.h"

#include <QStringList>
#include <QDebug>

#include "Manager.h"

namespace Ology {

Manager::Manager(int &argc, char** argv) :
    QApplication(argc, argv),
    _pausedPlayerForScreen(false)
{
    setOrganizationName("ology.org");
    setApplicationName("ology");
    addLibraryPath(".");

    if (_pseudoPlugin->autoLoadPlugins()->value()) {
        _pluginManager.autoLoadPlugins(Ology::RealUsage);
    } else {
        _pluginManager.loadPlugins( Ology::RealUsage, QStringList() << "libmenus.so" );
    }
    _pluginManager.registerScreens(OLOGY()->coreInstance());


    foreach(Plugin::ScreenProviderInterface *si, _pluginManager.screenProviderPlugins()) {
        foreach(AbstractAction *action, si->globalActions()) {
            qDebug() << "Adding action:" << action->name();
            _window.addAction(action);
        }
    }


/*
    QAction *exitAct = new QAction(tr("E&xit"), this);
    exitAct->setShortcut(tr("Ctrl+Q"));
    exitAct->setStatusTip(tr("Exit the application"));
    action->setShortcutContext(Qt::ApplicationShortcut);
    connect(exitAct, SIGNAL(triggered()), qApp, SLOT(quit()));
*/


    _window.show();
}

void Manager::displayScreen(const QString &id) {
    AbstractScreen *screen = _pluginManager.createScreen(id);
    if (!screen) {
        // TODO: display warning to user
        qWarning() << "Cannot find/create screen:" << id;
        return;
    }

    // Screen->initialize can call recursively call displayScreen.
    if (!screen->initialize(Ology::RealUsage)) {
        // TODO: display warning to user
        //       or if another screen has been displayed (because of recursive call), be quiet?
        qWarning() << "Screen failed to initialize:" << screen->id();
        screen->deleteLater();
        return;
    }

    // suspend previous screen
    if (_screens.size()) {
        _screens.top()->suspend();
    }

    // pause playing if screen grabs the sound
    if (currentPlayer()) {
        if (currentPlayer()->isPlaying() && screen->soundUsage() == AbstractScreen::GrabSound) {
            // TODO: disable actions to restart playing?
            currentPlayer()->pause();
            _pausedPlayerForScreen = true;
        } else if (currentPlayer()->isPaused() && screen->soundUsage() != AbstractScreen::GrabSound) {
            if (_pausedPlayerForScreen == true) {
                currentPlayer()->unpause();
                _pausedPlayerForScreen = false;
            }
        }
    }

    // start running this screen
    _screens.push(screen);
    _window.setScreen(screen);
}


AbstractScreen* Manager::currentScreen() const {
    return _screens.size() ? _screens.top() : NULL;
}


void Manager::closeCurrentScreen() {
    AbstractScreen *screen = NULL;

    // kill the current screen (if there is one):
    if (_screens.size() <= 1) { 
        Core::PseudoPluginInterface::CloseMainScreenOption option = _pseudoPlugin->closeMainScreenOptionSetting()->value();
        if (option == Core::PseudoPluginInterface::Quit) { qApp->quit(); }
        else if (option == Core::PseudoPluginInterface::ConfirmQuit) { displayScreen(Id::Screen::ConfirmQuit); }
        return;
    }
    screen = _screens.pop();
    screen->suspend();
    screen->deleteLater();

    // restore the previous screen
    screen = _screens.top();
    _window.setScreen(screen);
}

}
