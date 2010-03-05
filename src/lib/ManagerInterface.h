#ifndef OLOGY_MANAGER_INTERFACE
#define OLOGY_MANAGER_INTERFACE

#include <QApplication> // for qApp
class QWidget;

namespace Ology {
namespace Plugin { 
    class Manager; 
    class InfoInterface; 
    class ScreenInterface; 
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

    Plugin::InfoInterface* coreInfoInterface() { return _coreInfoInterface; }
    Plugin::ScreenInterface* coreScreenInterface() { return _coreScreenInterface; }
    QObject* coreInstance() { return (QObject*)_pseudoPlugin; }

protected:
    Plugin::InfoInterface *_coreInfoInterface;
    Plugin::ScreenInterface *_coreScreenInterface;
    Core::PseudoPluginInterface *_pseudoPlugin;
};

}

#define OLOGY() (dynamic_cast<ManagerInterface*>(qApp))


#endif
