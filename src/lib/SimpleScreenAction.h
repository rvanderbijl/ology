#ifndef OLOGY_SIMPLE_SCREEN_ACTION
#define OLOGY_SIMPLE_SCREEN_ACTION

#include <QAction>
#include <Ology/SimpleAction>
#include <Ology/AbstractScreen>
#include <Ology/HasSettings>

namespace Ology {

class SimpleScreenAction : public SimpleAction {
    Q_OBJECT
public:
    SimpleScreenAction(const QString &id, const char *name, const char *description, AbstractScreen *parent) : 
        SimpleAction(id, name, description, parent)
    {
        parent->registerLocalAction(this);
    }
};


}

#endif
