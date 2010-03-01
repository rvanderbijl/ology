#include <Ology/AbstractScreen>
#include <Ology/Core/CloseScreenAction>

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
    _pluginManager.loadPlugins( Ology::RealUsage, QStringList() << "libmenus.so" );



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
    if (_screens.size() <= 1) { return; }
    screen = _screens.pop();
    screen->suspend();
    screen->deleteLater();

    // if there was no previous screen, abort:
    if (!_screens.size()) {
        _window.setScreen(NULL);
        return; 
    }

    // restore the previous screen
    screen = _screens.top();
    _window.setScreen(screen);
}

}
