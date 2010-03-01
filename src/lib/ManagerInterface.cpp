#include "ManagerInterface.h"

#include "Core/CloseScreenAction.h"
#include "Core/QuitAction.h"
#include "Plugin/InfoInterface.h"
#include "Plugin/ScreenInterface.h"

//#include "Core/ConfirmQuitScreen.h" // TODO


namespace Ology {

class CoreInfoInterface : public Plugin::InfoInterface {
public:
    virtual QString name() const { return "Core Psuedo Plugin"; }
    virtual QString version() const { return "1.0"; }
    virtual QString description() const { return "Psuedo plugin providing core screens and actions"; }
    virtual bool initialize(Ology::InitializePurpose) { return true; }
};

class CoreScreenInterface : public Plugin::ScreenInterface {
public:
    CoreScreenInterface() {
        AbstractAction *action = NULL;
        
        action = new Core::CloseScreenAction(&_owner);
        action->setShortcut(Qt::Key_Escape);
        _actions << action;

        action = new Core::QuitAction(&_owner);
        action->setShortcut(QKeySequence("CTRL-Q"));
        _actions << action;
    }

    virtual ~CoreScreenInterface() {}

    virtual QStringList screenIds() { return QStringList(); }// TODO return "core-confirm-quit"; }
    virtual AbstractScreen* createScreen(const QString &id, QWidget *parent) { 
        Q_UNUSED(id);
        Q_UNUSED(parent);
        return NULL; 
    }
    virtual QList<AbstractAction*> globalActions() { return _actions; }

private:
    QList<AbstractAction*> _actions;
    QObject _owner;
};


ManagerInterface::ManagerInterface() :
    _coreInfoInterface(new CoreInfoInterface()),
    _coreScreenInterface(new CoreScreenInterface())
{
}

ManagerInterface::~ManagerInterface() {
    delete _coreInfoInterface  ;
    delete _coreScreenInterface;
    _coreInfoInterface   = NULL;
    _coreScreenInterface = NULL;
}

}
