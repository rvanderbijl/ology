#include <Ology/AbstractScreen>
#include <Ology/AbstractAction>
#include <Ology/Setting>
#include <Ology/InitializePurpose>
#include <Ology/Plugin/InfoInterface>
#include <Ology/Plugin/ScreenInterface>

#include <QStringList>
#include <QDebug>
#include <QSettings>
#include <QInputDialog>
#include <QFileDialog>
#include <QTreeWidget>
#include <QTreeWidgetItem>
#include <QPluginLoader>
#include <QStringList>

#include "SettingsEditor.h"
#include "TreeWidget.h"


namespace Ology {
namespace SettingsEditor {

Window::Window() :
    QMainWindow(),
    _settings(QSettings::IniFormat, QSettings::UserScope, "ology.org", "ology")
{
    setupUi(this);
    _pluginsLoaded->setEditableColumn(2);
    _tabWidget->setCurrentIndex(0);
}

void Window::fillInValues() {
    _autoLoadPlugins->setChecked( _settings.value("plugins/auto-load").toBool() );
    _pluginDirectory->setText( _settings.value("plugins/search-directory").toString() );
}


void Window::createSettingsEntries(QTreeWidgetItem *parent, HasSettings* hasSettings) {
    QList<AbstractSetting*> settings = hasSettings->settings();
    if (settings.isEmpty()) {
        QTreeWidgetItem *item = new QTreeWidgetItem(parent, QStringList() << "No settings");
        item->setFirstColumnSpanned(true);
        return;
    }

    foreach(AbstractSetting* setting, settings) {
        QTreeWidgetItem *item = new QTreeWidgetItem(parent, QStringList() << setting->name() << setting->displayValue() << setting->description());
        item->setData(0, AS_ROLE, QVariant::fromValue(setting));
        item->setFlags( item->flags() | Qt::ItemIsEditable );
    }
}


void Window::createSettingsEntries(Plugin::ScreenInterface* si, Plugin::InfoInterface *ii) {
    Q_ASSERT(ii);
    Q_ASSERT(si);

    // for plugin's global actions
    QList<AbstractAction*> actions = si->globalActions();
    if (actions.count()) {
        QTreeWidgetItem *pluginActionItem = new QTreeWidgetItem(_actionTree, QStringList() << ii->name() << "" << ii->description());
        foreach(AbstractAction * action, actions) {
            QTreeWidgetItem *actionItem = new QTreeWidgetItem(pluginActionItem, QStringList() << action->name() << "" << action->description());
            createSettingsEntries(actionItem, action);
        }
    }


    // for plugin's screens
    QStringList screenIds = si->screenIds();
    if (screenIds.count()) {
        QTreeWidgetItem *pluginScreenItem = new QTreeWidgetItem(_screenTree, QStringList() << ii->name() << "" << ii->description());
        foreach(const QString &screenId, screenIds) {
            AbstractScreen *screen = OLOGY()->pluginManager()->createScreen(screenId);
            if (!screen) { continue; }

            if (!screen->initialize(Ology::Introspection)) {
                qWarning() << "Error initializing screen for introspection:" << screenId;
                continue;
            }

            QTreeWidgetItem *screenItem = new QTreeWidgetItem(pluginScreenItem, QStringList() << screen->name() << "" << screen->description());
            createSettingsEntries(screenItem, screen);

            // screen's actions:
            actions = screen->actions();
            if (actions.count()) {
                foreach(AbstractAction * action, actions) {
                    QTreeWidgetItem *actionItem = new QTreeWidgetItem(screenItem, QStringList() << action->name() << "" << action->description());
                    createSettingsEntries(actionItem, action);
                }
            }
        }
    }
}


QStringList Window::specifiedPlugins() {
    QStringList specifiedPlugins;
    for(int i = 0; i < _pluginsLoaded->topLevelItemCount(); i++) {
        specifiedPlugins << _pluginsLoaded->topLevelItem(i)->text(0);
    }
    return specifiedPlugins;
}

void Window::reloadPlugins() {
    QStringList specifiedPlugins;
    OLOGY()->pluginManager()->unloadPlugins();
    if (_autoLoadPlugins->isChecked()) {
        OLOGY()->pluginManager()->autoLoadPlugins( Ology::Introspection );
    } else {
        specifiedPlugins = this->specifiedPlugins();
        OLOGY()->pluginManager()->loadPlugins( Ology::Introspection, specifiedPlugins );
    }

    updatedLoadedPlugins(specifiedPlugins);
}

void Window::updatedLoadedPlugins(const QStringList & specifiedPluginsOrig) {
    _pluginsLoaded->clear();
    _actionTree->clear();
    _screenTree->clear();


    Plugin::InfoInterface *cii = OLOGY()->coreInfoInterface(); 
    QTreeWidgetItem *coreItem = new QTreeWidgetItem(_pluginsLoaded, QStringList() << "Internal" << cii->name() << cii->version() << cii->description());
    createSettingsEntries(coreItem, cii);
    createSettingsEntries(OLOGY()->coreScreenInterface(), cii);

    QStringList specifiedPlugins = specifiedPluginsOrig;
    QList<QPluginLoader*> loadedPlugins = OLOGY()->pluginManager()->pluginLoaders();
    foreach(QPluginLoader *loader, loadedPlugins) {
        const QString fileName = QFileInfo(loader->fileName()).fileName();
        specifiedPlugins.removeAll(fileName);

        Plugin::InfoInterface *ii = qobject_cast<Plugin::InfoInterface*>(loader->instance());
        Plugin::ScreenInterface *si = qobject_cast<Plugin::ScreenInterface*>(loader->instance());

        QTreeWidgetItem *item = new QTreeWidgetItem(_pluginsLoaded, QStringList() << fileName << ii->name() << ii->version() << ii->description());
        if (si) {
            QTreeWidgetItem *pluginTreeItem = new QTreeWidgetItem(item, QStringList() << "ScreenInterface implemented");
            pluginTreeItem->setFirstColumnSpanned(true);
        }

        createSettingsEntries(item, ii);
        createSettingsEntries(si, ii);
    }

    foreach(const QString &fileName, specifiedPlugins) {
        QTreeWidgetItem *item = new QTreeWidgetItem(_pluginsLoaded, QStringList() << fileName << "" << "" << "not loaded");
        item->setForeground(0, Qt::red);
        item->setForeground(3, Qt::red);
    }
}


void Window::onPluginDirectoryBrowse() {
    const QString def = _pluginDirectory->text().isEmpty() ? "~/.ology/plugins" : _pluginDirectory->text();
    const QString directory = QFileDialog::getExistingDirectory(this, tr("Select Directory"), def, QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
    if (!directory.isEmpty()) { 
        _pluginDirectory->setText(directory);
    }
}

void Window::onPluginAutoLoadChanged() {
    _settings.setValue("plugins/auto-load", _autoLoadPlugins->isChecked());
    reloadPlugins();
}

void Window::onPluginDirectoryChanged(const QString &dir) {
    _settings.setValue("plugins/search-directory", dir);
    OLOGY()->pluginManager()->setPluginSearchDirectory(dir);
    reloadPlugins();
}


void Window::onPluginAdd() {
    const QString fileName = QInputDialog::getText(this, tr("Add Plugin"), tr("Plugin to load"));
    if (fileName.isEmpty()) { return; }

    QStringList specifiedPlugins;
    specifiedPlugins << fileName;

    QTreeWidgetItemIterator it(_pluginsLoaded);
    while (*it) { specifiedPlugins << (*it)->text(0); it++; }
    _pluginsLoaded->clear();

    updatedLoadedPlugins(specifiedPlugins);
}

void Window::onPluginRemove() {
    QTreeWidgetItem *item = _pluginsLoaded->currentItem();
    if (!item) { return; }
    if (item->parent()) { item = item->parent(); }
    delete item;
    item = NULL;

    reloadPlugins();
}





}}

Q_DECLARE_METATYPE(Ology::AbstractSetting*)
