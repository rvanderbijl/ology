#ifndef OLOGY_CONF_TOOL_WINDOW
#define OLOGY_CONF_TOOL_WINDOW

#include <QMainWindow>
#include <QStack>
#include <QSettings>
#include <QDebug>

#include <Ology/Plugin/Manager>
#include <Ology/ManagerInterface>

#include "ui_SettingsEditor.h"

namespace Ology {
namespace SettingsEditor {
    class AbstractScreen;
    class AbstractAction;
   

class Window : public QMainWindow, public Ui::SettingsEditor {
    Q_OBJECT
public:
    Window();
    
    void fillInValues();


    QStringList specifiedPlugins();
    void reloadPlugins();
    void updatedLoadedPlugins(const QStringList & specifiedPlugins);

// plugin settings
public slots:
    void onPluginDirectoryBrowse();
    void onPluginDirectoryChanged(const QString &dir);
    void onPluginAutoLoadChanged();
    void onPluginAdd();
    void onPluginRemove();


   
private:
    QSettings _settings;
};


}}

#endif
