#ifndef OLOGY_QUIT_ACTION
#define OLOGY_QUIT_ACTION

#include <QMetaType>
#include <Ology/AbstractAction>
#include <Ology/Setting>

namespace Ology {
namespace Core {

class QuitAction : public AbstractAction {
    Q_OBJECT
public:
    enum ConfirmQuitOption { Quit, ConfirmQuit };

    QuitAction(QObject *parent);
    virtual void run();

private:
    Setting<ConfirmQuitOption> _confirmQuitSetting;
};

}}

Q_DECLARE_METATYPE(Ology::Core::QuitAction::ConfirmQuitOption);

#endif
