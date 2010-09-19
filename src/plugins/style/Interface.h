#ifndef OLOGY_PLUGIN_STYLE_INTERFACE
#define OLOGY_PLUGIN_STYLE_INTERFACE

#include <QObject>
#include <Ology/Plugin/InfoInterface>

namespace Ology {
namespace Plugin {
namespace Style {

class Player;

class Interface : 
    public QObject,
    public Ology::Plugin::InfoInterface
{
    Q_OBJECT
    Q_INTERFACES(Ology::Plugin::InfoInterface)

public:
    Interface();

    virtual QString name() const { return "Style"; }
    virtual QString version() const { return "0.1"; }
    virtual QString description() const { return "Sets UI style"; }

    virtual bool initialize(Ology::InitializePurpose initPurpose);
};


}}}


#endif
