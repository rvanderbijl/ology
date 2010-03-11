#include <Ology/Plugin/ScreenInterface>
#include <Ology/AbstractScreen>
#include <Ology/Core/CloseScreenAction>
#include "../lib/Core/PseudoPluginInterface.h"
#include "../lib/Core/CoreIds.h"

#include <QStringList>
#include <QDebug>

#include "Manager.h"

namespace Ology {

Manager::Manager(int &argc, char** argv) :
    QApplication(argc, argv)
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


    foreach(Plugin::ScreenInterface *si, _pluginManager.screenPlugins()) {
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
        return;
    }

    // Screen->initialize call recursively call displayScreen.
    if (!screen->initialize(Ology::RealUsage)) {
        // TODO: display warning to user
        //       or if another screen has been display, be quiet?
        qWarning() << "Screen failed to initialize:" << screen->id();
        screen->deleteLater();
        return;
    }

    // suspend previous screen
    if (_screens.size()) {
        _screens.top()->suspend();
    }

    // start running this screen
    _screens.push(screen);
    _window.setScreen(screen);
}


void Manager::closeCurrentScreen() {
    AbstractScreen *screen = NULL;

    // kill the current screen (if there is one):
    if (_screens.size() <= 1) { 
        Core::PseudoPluginInterface::CloseMainScreenOption option = _pseudoPlugin->closeMainScreenOptionSetting()->value();
        if (option == Core::PseudoPluginInterface::Quit) { qApp->quit(); }
        else if (option == Core::PseudoPluginInterface::ConfirmQuit) { displayScreen(ID_SCREEN_CONFIRM_QUIT); }
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
