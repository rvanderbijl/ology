#ifndef OLOGY_MANAGER
#define OLOGY_MANAGER

#include <QApplication>
#include <QStack>
#include <Ology/Plugin/Manager>
#include <Ology/ManagerInterface>
#include "Window.h"

namespace Ology {
    class AbstractScreen;
   

class Manager : public QApplication, public ManagerInterface {
    Q_OBJECT
public:
    Manager(int &argc, char**argv);
    
    virtual QWidget* screenParent()  { return &_window; }
    virtual void displayScreen(const QString &id);
    virtual void closeCurrentScreen();
   
private:
    QStack<AbstractScreen*> _screens;
    Plugin::Manager _pluginManager;
    Window _window;
};


}

#endif
