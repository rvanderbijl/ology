#ifndef OLOGY_ACTION_PAGE_UP
#define OLOGY_ACTION_PAGE_UP

#include <Ology/AbstractAction>

namespace Ology {
namespace Core {

class PageUpAction : public AbstractAction {
    Q_OBJECT
    USE_HAS_NAME_DESCRIPTION
public:
    PageUpAction(QObject *parent);
    virtual void run() {}
};

}}

#endif
