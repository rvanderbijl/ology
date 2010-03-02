#ifndef OLOGY_CORE_INTERFACE
#define OLOGY_CORE_INTERFACE

#include <QObject>
#include "Plugin/InfoInterface.h"
#include "Plugin/ScreenInterface.h"


namespace Ology {

class CoreInterface :
    public QObject,
    public Plugin::InfoInterface, 
    public Plugin::ScreenInterface 
{
    Q_OBJECT
    Q_INTERFACES( Ology::Plugin::InfoInterface Ology::Plugin::ScreenInterface );
public:
    CoreInterface();
    virtual ~CoreInterface() {}

public:
    virtual QString name() const { return "Core Psuedo Plugin"; }
    virtual QString version() const { return "1.0"; }
    virtual QString description() const { return "Psuedo plugin providing core screens and actions"; }
    virtual bool initialize(Ology::InitializePurpose) { return true; }

public:
    virtual QStringList screenIds();
    virtual AbstractScreen* createScreen(const QString &id, QWidget *parent);
    virtual QList<AbstractAction*> globalActions() { return _actions; }

private:
    QList<AbstractAction*> _actions;
};

}

#endif
