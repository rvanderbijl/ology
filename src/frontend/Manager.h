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
    
    virtual Plugin::Manager* pluginManager() const { return const_cast<Plugin::Manager*>(&_pluginManager); }
    virtual QWidget* screenParent() const { return const_cast<Window*>(&_window); }
    virtual AbstractScreen* currentScreen() const;

public slots:
    virtual void displayScreen(const QString &id);
    virtual void closeCurrentScreen();
   
private:
    QStack<AbstractScreen*> _screens;
    Plugin::Manager _pluginManager;
    Window _window;
    bool _pausedPlayerForScreen;
};


}

#endif
