#ifndef OLOGY_ACTION_PAGE_DOWN
#define OLOGY_ACTION_PAGE_DOWN

#include <Ology/AbstractAction>

namespace Ology {
namespace Core {

class PageDownAction : public AbstractAction {
    Q_OBJECT
    USE_HAS_NAME_DESCRIPTION
public:
    PageDownAction(QObject *parent);
    virtual void run() {}
};

}}

#endif
