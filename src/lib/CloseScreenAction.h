#ifndef OLOGY_ACTION_DISPLAY_SCREEN
#define OLOGY_ACTION_DISPLAY_SCREEN

#include <Ology/AbstractAction>

namespace Ology {

class CloseScreenAction : public AbstractAction {
    Q_OBJECT

public:
    CloseScreenAction(QObject *parent);
    virtual void run();
};

}

#endif
