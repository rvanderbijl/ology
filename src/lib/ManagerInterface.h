#ifndef OLOGY_MANAGER_INTERFACE
#define OLOGY_MANAGER_INTERFACE

#include <QApplication> // for qApp
class QWidget;

namespace Ology {

class ManagerInterface {
public:
    virtual QWidget* screenParent() = 0;
    virtual void displayScreen(const QString &id) = 0;
    virtual void closeCurrentScreen() = 0;
};

}

#define OLOGY() (dynamic_cast<ManagerInterface*>(qApp))


#endif
