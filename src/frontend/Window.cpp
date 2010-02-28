#include <Ology/AbstractScreen>
#include "Window.h"

namespace Ology {

Window::Window() :
    QWidget()
{
    // TODO: load from settings: full screen, or geometry
    resize(800,600);
    move(40,40);

    setScreen(NULL);
}


void Window::setScreen(AbstractScreen *screen) {
    this->setWindowTitle(tr("Screen: %1").arg(screen ? screen->name() : tr("none")));

    if (!screen) { return; }
    screen->setParent(this);
    screen->resize(this->size());
    screen->raise();
    screen->show();
    screen->run();
}

}
