#ifndef OLOGY_ACTION_LEFT
#define OLOGY_ACTION_LEFT

#include <Ology/AbstractAction>

namespace Ology {
namespace Core {

class LeftAction : public AbstractAction {
    Q_OBJECT
    USE_HAS_NAME_DESCRIPTION
public:
    LeftAction(QObject *parent);
    virtual void run() {}
};

}}

#endif
