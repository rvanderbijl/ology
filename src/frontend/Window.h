#ifndef OLOGY_WINDOW
#define OLOGY_WINDOW

#include <QWidget>

namespace Ology  {
    class AbstractScreen;

class Window : public QWidget {
    Q_OBJECT
public:
    Window();
    void setScreen(AbstractScreen *screen);

};

}

#endif
