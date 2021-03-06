#ifndef OLOGY_MANAGER
#define OLOGY_MANAGER

#include <QApplication>
#include <QStack>
#include <Ology/Plugin/Manager>
#include <Ology/ManagerInterface>

#include "SettingsEditor.h"

namespace Ology {
namespace SettingsEditor {

class Manager : public QApplication, public ManagerInterface {
    Q_OBJECT
public:
    Manager(int &argc, char**argv);
    
    // Manager Interface
    virtual Plugin::Manager* pluginManager() const { return const_cast<Plugin::Manager*>(&_pluginManager); }
    virtual QWidget* screenParent() const { return const_cast<Window*>(&_window); }
    virtual void displayScreen(const QString &id) { qWarning() << "SettingsEditor trying to display screen:" << id; }
    virtual void closeCurrentScreen() { qWarning() << "SettingsEditor trying to close current screen"; }
    virtual AbstractScreen* currentScreen() const { qWarning() << "SettingsEditor trying to get current screen!"; return NULL; }

    Core::PseudoPluginInterface* pseudoPlugin() { return _pseudoPlugin; }


private:
    Plugin::Manager _pluginManager;
    Window _window;
};


}}

#endif
