#ifndef OLOGY_MORE_SCREEN_ACTION
#define OLOGY_MORE_SCREEN_ACTION

#include <QMetaType>
#include <Ology/AbstractAction>
#include <Ology/Setting>

namespace Ology {
namespace Core {

class MoreScreenAction : public AbstractAction {
    Q_OBJECT
    USE_HAS_NAME_DESCRIPTION
public:
    MoreScreenAction(QObject *parent);
    virtual void run();
};

}}

#endif
