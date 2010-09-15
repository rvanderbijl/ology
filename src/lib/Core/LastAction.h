#ifndef OLOGY_ACTION_LAST
#define OLOGY_ACTION_LAST

#include <Ology/AbstractAction>

namespace Ology {
namespace Core {

class LastAction : public AbstractAction {
    Q_OBJECT
    USE_HAS_NAME_DESCRIPTION
public:
    LastAction(QObject *parent);
    virtual void run() {}
};

}}

#endif
