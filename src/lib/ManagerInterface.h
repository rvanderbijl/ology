#ifndef OLOGY_MANAGER_INTERFACE
#define OLOGY_MANAGER_INTERFACE

#include <QApplication> // for qApp
class QWidget;

namespace Ology {
class AbstractAction;
class AbstractScreen;
class AbstractPlayer;

namespace Plugin { 
    class Manager; 
    class InfoInterface; 
    class ScreenProviderInterface; 
}
namespace Core {
    class PseudoPluginInterface;
}

class ManagerInterface {
public:
    ManagerInterface();
    ~ManagerInterface();

    virtual Plugin::Manager* pluginManager() const = 0;
    virtual QWidget* screenParent() const = 0;
    virtual void displayScreen(const QString &id) = 0;
    virtual void closeCurrentScreen() = 0;
    virtual AbstractScreen* currentScreen() const = 0;
    virtual AbstractPlayer* currentPlayer() const { return _currentPlayer; }
    virtual void setCurrentPlayer(AbstractPlayer *player);


    Plugin::InfoInterface* coreInfoInterface() { return _coreInfoInterface; }
    Plugin::ScreenProviderInterface* coreScreenProviderInterface() { return _coreScreenProviderInterface; }
    QObject* coreInstance() { return (QObject*)_pseudoPlugin; }

    AbstractAction* action(const QString &id) const;

protected:
    Plugin::InfoInterface *_coreInfoInterface;
    Plugin::ScreenProviderInterface *_coreScreenProviderInterface;
    Core::PseudoPluginInterface *_pseudoPlugin;
    AbstractPlayer *_currentPlayer;
};

}

#define OLOGY() (dynamic_cast<ManagerInterface*>(qApp))


#endif
