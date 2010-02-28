#ifndef OLOGY_MANAGER_INTERFACE
#define OLOGY_MANAGER_INTERFACE

#include <QApplication> // for qApp
class QWidget;

namespace Ology {
namespace Plugin { class Manager; }

class ManagerInterface {
public:
    virtual Plugin::Manager* pluginManager() const = 0;
    virtual QWidget* screenParent() const = 0;
    virtual void displayScreen(const QString &id) = 0;
    virtual void closeCurrentScreen() = 0;
};

}

#define OLOGY() (dynamic_cast<ManagerInterface*>(qApp))


#endif
