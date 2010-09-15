#include <Ology/AbstractScreen>
#include <Ology/AbstractAction>
#include <Ology/Setting>
#include <Ology/InitializePurpose>
#include <Ology/Plugin/InfoInterface>
#include <Ology/Plugin/ScreenProviderInterface>

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
#include "Manager.h"
#include "../lib/Core/PseudoPluginInterface.h"


namespace Ology {
namespace SettingsEditor {

Window::Window() :
    QMainWindow(),
    _settings(QSettings::IniFormat, QSettings::UserScope, "ology.org", "ology")
{
    setupUi(this);
    _pluginTree->setEditableColumn(2);
    _tabWidget->setCurrentIndex(0);

    Manager *m = qobject_cast<Manager*>(qApp);
    connect(m->pseudoPlugin()->autoLoadPlugins(), SIGNAL(valueChanged()), SLOT(reloadPlugins()), Qt::QueuedConnection);
}

void Window::fillInValues() {
    //_autoLoadPlugins->setChecked( _settings.value("plugins/auto-load").toBool() );
    _pluginDirectory->setText( _settings.value("plugins/search-directory").toString() );
}


void Window::createSettingsEntries(QTreeWidgetItem *parent, HasSettings* hasSettings) {
    const bool forPluginTree = (parent->treeWidget() == _pluginTree);
    QList<AbstractSetting*> settings = hasSettings->settings();
    if (settings.isEmpty()) {
        QTreeWidgetItem *item = new QTreeWidgetItem(parent, QStringList() << tr("No settings"));
        item->setFirstColumnSpanned(true);
        return;
    }

    foreach(AbstractSetting* setting, settings) {
        QTreeWidgetItem *item = new QTreeWidgetItem(parent, 
                   forPluginTree ? QStringList() << "" << setting->name() << setting->displayValue() << setting->description() // extra column for plugin-filename
                                 : QStringList()       << setting->name() << setting->displayValue() << setting->description());
        item->setIcon(forPluginTree ? 1 : 0, QIcon(":icon/icon_setting.png")); // name column
        item->setData(forPluginTree ? 2 : 1, AS_ROLE, QVariant::fromValue(setting)); // value column
        item->setFlags( item->flags() | Qt::ItemIsEditable );
    }
}


void Window::createSettingsEntries(Plugin::ScreenProviderInterface* si, Plugin::InfoInterface *ii) {
    Q_ASSERT(ii);
    Q_ASSERT(si);

    // for plugin's global actions
    QList<AbstractAction*> actions = si->globalActions();
    QTreeWidgetItem *pluginActionItem = new QTreeWidgetItem(_actionTree, QStringList() << ii->name() << "" << ii->description());
    pluginActionItem->setIcon(0, QIcon(":icon/icon_plugin.png"));
    if (actions.count()) {
        foreach(AbstractAction * action, actions) {
            QTreeWidgetItem *actionItem = new QTreeWidgetItem(pluginActionItem, QStringList() << action->name() << "" << action->description());
            actionItem->setIcon(0, QIcon(":icon/icon_action.png"));
            createSettingsEntries(actionItem, action);
        }
    } else {
        new QTreeWidgetItem(pluginActionItem, QStringList() << tr("No actions"));

    }


    // for plugin's screens
    QStringList screenIds = si->screenIds();
    if (screenIds.count()) {
        QTreeWidgetItem *pluginScreenItem = new QTreeWidgetItem(_screenTree, QStringList() << ii->name() << "" << ii->description());
        pluginScreenItem->setIcon(0, QIcon(":icon/icon_plugin.png"));
        foreach(const QString &screenId, screenIds) {
            AbstractScreen *screen = OLOGY()->pluginManager()->createScreen(screenId);
            if (!screen) { continue; }

            if (!screen->initialize(Ology::Introspection)) {
                qWarning() << "Error initializing screen for introspection:" << screenId;
                continue;
            }

            QTreeWidgetItem *screenItem = new QTreeWidgetItem(pluginScreenItem, QStringList() << screen->name() << "" << screen->description());
            QTreeWidgetItem *screenSettingsItem = new QTreeWidgetItem(screenItem, QStringList() << tr("Settings"));
            QTreeWidgetItem *screenActionsItem = new QTreeWidgetItem(screenItem, QStringList() << tr("Actions"));

            screenItem->setIcon(0, QIcon(":icon/icon_screen.png"));
            screenSettingsItem->setIcon(0, QIcon(":icon/icon_setting.png"));
            screenActionsItem->setIcon(0, QIcon(":icon/icon_action.png"));

            createSettingsEntries(screenSettingsItem, screen);

            // screen's actions:
            actions = screen->actions();
            if (actions.count()) {
                foreach(AbstractAction * action, actions) {
                    QTreeWidgetItem *actionItem = new QTreeWidgetItem(screenActionsItem, QStringList() << action->name() << "" << action->description());
                    actionItem->setIcon(0, QIcon(":icon/icon_action.png"));
                    createSettingsEntries(actionItem, action);
                }
            } else {
                new QTreeWidgetItem(screenActionsItem, QStringList() << tr("No actions"));
            }
        }
    }
}


QStringList Window::specifiedPlugins() {
    QStringList specifiedPlugins;
    for(int i = 0; i < _pluginTree->topLevelItemCount(); i++) {
        const QString name = _pluginTree->topLevelItem(i)->text(0);
        if (name != "Internal") {
            specifiedPlugins << name;
        }
    }
    return specifiedPlugins;
}

void Window::reloadPlugins() {
    QStringList specifiedPlugins;
    OLOGY()->pluginManager()->unloadPlugins();



    Manager *m = qobject_cast<Manager*>(qApp);
    const bool autoLoad = m->pseudoPlugin()->autoLoadPlugins()->variantValue().toBool();

    qDebug() << "Reload: auto=" << autoLoad;

    _addPluginButton->setEnabled(!autoLoad);
    _removePluginButton->setEnabled(!autoLoad);

    if (autoLoad) {
        OLOGY()->pluginManager()->autoLoadPlugins( Ology::Introspection );
    } else {
        specifiedPlugins = this->specifiedPlugins();
        OLOGY()->pluginManager()->loadPlugins( Ology::Introspection, specifiedPlugins );
    }

    updatedLoadedPlugins(specifiedPlugins);
}

void Window::updatedLoadedPlugins(const QStringList & specifiedPluginsOrig) {
    _pluginTree->clear();
    _actionTree->clear();
    _screenTree->clear();


    Plugin::InfoInterface *cii = OLOGY()->coreInfoInterface(); 
    QTreeWidgetItem *coreItem = new QTreeWidgetItem(_pluginTree, QStringList() << "Internal" << cii->name() << cii->version() << cii->description());
    coreItem->setIcon(0, QIcon(":icon/icon_plugin.png"));
    createSettingsEntries(coreItem, cii);
    createSettingsEntries(OLOGY()->coreScreenProviderInterface(), cii);

    QStringList specifiedPlugins = specifiedPluginsOrig;
    QList<QPluginLoader*> loadedPlugins = OLOGY()->pluginManager()->pluginLoaders();
    foreach(QPluginLoader *loader, loadedPlugins) {
        const QString fileName = QFileInfo(loader->fileName()).fileName();
        specifiedPlugins.removeAll(fileName);

        Plugin::InfoInterface *ii = qobject_cast<Plugin::InfoInterface*>(loader->instance());
        Plugin::ScreenProviderInterface *si = qobject_cast<Plugin::ScreenProviderInterface*>(loader->instance());

        QTreeWidgetItem *item = new QTreeWidgetItem(_pluginTree, QStringList() << fileName << ii->name() << ii->version() << ii->description());
        item->setIcon(0, QIcon(":icon/icon_plugin.png"));
        if (si) {
            QTreeWidgetItem *pluginTreeItem = new QTreeWidgetItem(item, QStringList() << "ScreenProviderInterface implemented");
            pluginTreeItem->setFirstColumnSpanned(true);
        }

        createSettingsEntries(item, ii);
        createSettingsEntries(si, ii);
    }

    foreach(const QString &fileName, specifiedPlugins) {
        QTreeWidgetItem *item = new QTreeWidgetItem(_pluginTree, QStringList() << fileName << "" << "" << "not loaded");
        item->setForeground(0, Qt::red);
        item->setForeground(3, Qt::red);
    }

    _pluginTree->resizeColumnToContents(0);
    _actionTree->resizeColumnToContents(0);
    _screenTree->resizeColumnToContents(0);
}


void Window::onPluginDirectoryBrowse() {
    const QString def = _pluginDirectory->text().isEmpty() ? "~/.ology/plugins" : _pluginDirectory->text();
    const QString directory = QFileDialog::getExistingDirectory(this, tr("Select Directory"), def, QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
    if (!directory.isEmpty()) { 
        _pluginDirectory->setText(directory);
    }
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

    QTreeWidgetItemIterator it(_pluginTree);
    while (*it) { specifiedPlugins << (*it)->text(0); it++; }
    _pluginTree->clear();

    updatedLoadedPlugins(specifiedPlugins);
}

void Window::onPluginRemove() {
    QTreeWidgetItem *item = _pluginTree->currentItem();
    if (!item) { return; }
    if (item->parent()) { item = item->parent(); }
    delete item;
    item = NULL;

    reloadPlugins();
}


}}
