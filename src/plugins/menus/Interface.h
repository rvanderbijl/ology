#ifndef OLOGY_PLUGIN_MENUS_INTERFACE
#define OLOGY_PLUGIN_MENUS_INTERFACE

#include <QObject>
#include <Ology/Plugin/ScreenInterface>
#include <Ology/Plugin/InfoInterface>

namespace Ology {
namespace Plugin {
namespace Menus {

class Interface : 
    public QObject,
    public Ology::Plugin::InfoInterface,
    public Ology::Plugin::ScreenInterface 
{
    Q_OBJECT
    Q_INTERFACES(Ology::Plugin::InfoInterface Ology::Plugin::ScreenInterface)
public:
    Interface() {}

    virtual QString name() const { return "Menus"; }
    virtual QString version() const { return "0.1"; }
    virtual QString description() const { return "Generic list of menus"; }

    virtual bool initialize(Ology::InitializePurpose initPurpose);
    virtual QStringList screenIds() ;
    virtual AbstractScreen* createScreen(const QString &id, QWidget *parent);
    virtual QList<AbstractAction*> globalActions();

private:

};


}}}

#endif
