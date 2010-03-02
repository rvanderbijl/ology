#ifndef OLOGY_SIMPLE_SCREEN_ACTION
#define OLOGY_SIMPLE_SCREEN_ACTION

#include <QAction>
#include <Ology/AbstractAction>
#include <Ology/AbstractScreen>
#include <Ology/HasSettings>

namespace Ology {

class SimpleScreenAction : public AbstractAction
{
    Q_OBJECT
    USE_HAS_NAME_DESCRIPTION 
public:
    SimpleScreenAction(const char *name, const char *description, AbstractScreen *parent) : 
        AbstractAction(parent)
    {
        setTranslationContext(parent->metaObject()->className());
        setUntranslatedName(name);
        setUntranslatedDescription(description);

        parent->registerLocalAction(this);
    }

public slots:
    virtual void run() {}
};


};

#endif
