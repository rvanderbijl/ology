#ifndef OLOGY_CORE_INTERFACE
#define OLOGY_CORE_INTERFACE

#include <QObject>
#include <Ology/Plugin/InfoInterface>
#include <Ology/Plugin/ScreenInterface>
#include <Ology/Setting>


namespace Ology {
namespace Core {

class PseudoPluginInterface :
    public QObject,
    public Plugin::InfoInterface, 
    public Plugin::ScreenInterface 
{
    Q_OBJECT
    Q_INTERFACES( Ology::Plugin::InfoInterface Ology::Plugin::ScreenInterface );
    Q_ENUMS( CloseMainScreenOption )
public:
    enum CloseMainScreenOption { Ignore, Quit, ConfirmQuit };


public:
    PseudoPluginInterface();
    virtual ~PseudoPluginInterface() {}

public:
    virtual QString name() const { return "Core Psuedo Plugin Interface"; }
    virtual QString version() const { return "1.0"; }
    virtual QString description() const { return "Psuedo plugin providing core screens and actions"; }
    virtual bool initialize(Ology::InitializePurpose) { return true; }

public:
    virtual QStringList screenIds();
    virtual AbstractScreen* createScreen(const QString &id, QWidget *parent);
    virtual QList<AbstractAction*> globalActions() { return _actions; }

private:
    QList<AbstractAction*> _actions;

    Setting<CloseMainScreenOption> _closeMainScreen;
};

}}

Q_DECLARE_METATYPE(Ology::Core::PseudoPluginInterface::CloseMainScreenOption);

#endif
