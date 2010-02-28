#ifndef OLOGY_ACTION_CLOSE_SCREEN
#define OLOGY_ACTION_CLOSE_SCREEN

#include <Ology/AbstractAction>

namespace Ology {
namespace Core {

class CloseScreenAction : public AbstractAction {
    Q_OBJECT
public:
    CloseScreenAction(QObject *parent);
    virtual void run();
};

}}

#endif
