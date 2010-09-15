#ifndef OLOGY_CORE_PSEUDO_PLUGIN_INTERFACE
#define OLOGY_CORE_PSEUDO_PLUGIN_INTERFACE

#include <QObject>
#include <Ology/Plugin/InfoInterface>
#include <Ology/Plugin/ScreenProviderInterface>
#include <Ology/Setting>

namespace Ology {
namespace Core {

class PseudoPluginInterface :
    public QObject,
    public Plugin::InfoInterface, 
    public Plugin::ScreenProviderInterface 
{
    Q_OBJECT
    Q_INTERFACES( Ology::Plugin::InfoInterface Ology::Plugin::ScreenProviderInterface );
    Q_ENUMS( CloseMainScreenOption )
public:
    enum CloseMainScreenOption { Ignore, Quit, ConfirmQuit };


public:
    PseudoPluginInterface();
    virtual ~PseudoPluginInterface() {}

    Setting<CloseMainScreenOption> * closeMainScreenOptionSetting() { return &_closeMainScreenOption; }
    Setting<bool>* autoLoadPlugins() { return &_autoLoadPlugins; }

private:
    QList<AbstractAction*> _actions;
    Setting<CloseMainScreenOption> _closeMainScreenOption;
    Setting<bool> _autoLoadPlugins;




// Plugin::InfoInterface
public:
    virtual QString name() const { return "Core Psuedo Plugin Interface"; }
    virtual QString version() const { return "1.0"; }
    virtual QString description() const { return "Psuedo plugin providing core screens and actions"; }
    virtual bool initialize(Ology::InitializePurpose) { return true; }

// Plugin::ScreenProviderInterface
public:
    virtual QStringList screenIds();
    virtual AbstractScreen* createScreen(const QString &id, QWidget *parent);
    virtual QList<AbstractAction*> globalActions() { return _actions; }

};

}}

Q_DECLARE_METATYPE(Ology::Core::PseudoPluginInterface::CloseMainScreenOption);

#endif
