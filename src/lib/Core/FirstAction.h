#ifndef OLOGY_ACTION_FIRST
#define OLOGY_ACTION_FIRST

#include <Ology/AbstractAction>

namespace Ology {
namespace Core {

class FirstAction : public AbstractAction {
    Q_OBJECT
    USE_HAS_NAME_DESCRIPTION
public:
    FirstAction(QObject *parent);
    virtual void run() {}
};

}}

#endif
