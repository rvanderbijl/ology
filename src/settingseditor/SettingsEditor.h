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
    class HasSettings;
    class AbstractScreen;
    class AbstractAction;

namespace SettingsEditor {
   

class Window : public QMainWindow, public Ui::SettingsEditor {
    Q_OBJECT
public:
    Window();
    void fillInValues();

    void createSettingsEntries(QTreeWidgetItem *parent, HasSettings* hasSetting);
    void createSettingsEntries(Plugin::ScreenProviderInterface* si, Plugin::InfoInterface* plugin);


    QStringList specifiedPlugins();
    void updatedLoadedPlugins(const QStringList & specifiedPlugins);

// plugin settings
public slots:
    void reloadPlugins();
    void onPluginDirectoryBrowse();
    void onPluginDirectoryChanged(const QString &dir);
    void onPluginAdd();
    void onPluginRemove();


   
private:
    QSettings _settings;
};


}}

#endif
