#ifndef OLOGY_ACTION_RIGHT
#define OLOGY_ACTION_RIGHT

#include <Ology/AbstractAction>

namespace Ology {
namespace Core {

class RightAction : public AbstractAction {
    Q_OBJECT
    USE_HAS_NAME_DESCRIPTION
public:
    RightAction(QObject *parent);
    virtual void run() {}
};

}}

#endif
