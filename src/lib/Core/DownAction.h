#ifndef OLOGY_ACTION_DOWN
#define OLOGY_ACTION_DOWN

#include <Ology/AbstractAction>

namespace Ology {
namespace Core {

class DownAction : public AbstractAction {
    Q_OBJECT
    USE_HAS_NAME_DESCRIPTION
public:
    DownAction(QObject *parent);
    virtual void run() {}
};

}}

#endif
