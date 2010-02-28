#ifndef OLOGY_QUIT_ACTION
#define OLOGY_QUIT_ACTION

#include <Ology/AbstractAction>

namespace Ology {

class QuitAction : public AbstractAction {
    Q_OBJECT
public:
    enum ConfirmQuit { Quit, ConfirmQuit };

    QuitAction(QObject *parent);
    virtual void run();

private:
    Setting<ConfirmQuit> _confirmQuitSetting;
};

}

#endif
