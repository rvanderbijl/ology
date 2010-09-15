#ifndef OLOGY_ACTION_UP
#define OLOGY_ACTION_UP

#include <Ology/AbstractAction>

namespace Ology {
namespace Core {

class UpAction : public AbstractAction {
    Q_OBJECT
    USE_HAS_NAME_DESCRIPTION
public:
    UpAction(QObject *parent);
    virtual void run() {}
};

}}

#endif
