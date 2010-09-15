#ifndef OLOGY_SIMPLE_ACTION
#define OLOGY_SIMPLE_ACTION

#include <QAction>
#include <Ology/AbstractAction>
#include <Ology/HasSettings>

namespace Ology {

class SimpleAction : public AbstractAction {
    Q_OBJECT
    USE_HAS_NAME_DESCRIPTION 
public:
    SimpleAction(const QString &id, const char *name, const char *description, QObject *parent) : 
        AbstractAction(id, ScreenSpecificAction, parent)
    {
        setTranslationContext(parent->metaObject()->className());
        setUntranslatedName(name);
        setUntranslatedDescription(description);
    }

public slots:
    // use triggered signal instead of run
    virtual void run() {}
};


};

#endif
