#include <QDebug>
#include <QPluginLoader>
#include "ManagerInterface.h"
#include "PluginManager.h"
#include "PluginInterface.h"

namespace Ology {
namespace Plugin {

Manager::Manager(QObject *parent) :
    QObject(parent)
{
}

void Manager::loadPlugins(const QStringList &pluginNames) {
    foreach(const QString &name, pluginNames) {
        // try load the plugin
        QPluginLoader *loader = new QPluginLoader(name, this);
        if (!loader->instance()) {
            qWarning() << "Failed to load plugin:" << name << loader->errorString();
            delete loader;
            continue;
        }

        // inform user of result of loading plugin
        qDebug() << "Plugin file loaded: " << name;
        Plugin::Interface* pi = qobject_cast<Plugin::Interface*>(loader->instance());
        if (pi) {
            qDebug() << "Plugin details:" << pi->name() << pi->version() << pi->description();
        } else {
            qDebug() << "Plugin doesn't provide Ology::PluginInterface";
        }
        _plugins << loader;

        
        // now try to initialize
        if (!pi->initialize()) {
            qWarning() << "Plugin failed to initialize: " << pi->name();
            return;
        }

        // track which screens this plugin provides
        foreach(const QString &screenId, pi->screenIds()) {
            if (_screenFactory.contains(screenId)) {
                qWarning() << "Replacing screenId " << screenId << "factory" << _screenFactory[screenId]->name() << "with" << pi->name();
            }
            _screenFactory[screenId] = pi;
        }
    }
}

QList<Plugin::Interface*> Manager::ologyPlugins() {
    QList<Plugin::Interface*> list;
    foreach(QPluginLoader* loader, _plugins) {
        Plugin::Interface* pi = qobject_cast<Plugin::Interface*>(loader->instance());
        if (pi) { list << pi; }
    }
    return list;
}


AbstractScreen* Manager::createScreen(const QString &id) {
    if (!_screenFactory.contains(id)) {
        qWarning() << "Error creating screen" << id << ": no plugin provides this screen-id";
        return NULL;
    }

    AbstractScreen* screen = _screenFactory[id]->createScreen(id, NULL);// OLOGY()->screenParent());
    if (!screen) {
        qWarning() << "Plugin" << _screenFactory[id]->name() << "failed to create screen:" << id 
                   << "Error:" << _screenFactory[id]->errorString() ;
    }

    return screen;
}


}}
