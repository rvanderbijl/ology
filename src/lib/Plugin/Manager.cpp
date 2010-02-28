#include <QDebug>
#include <QPluginLoader>
#include "ManagerInterface.h"
#include "Manager.h"
#include "InfoInterface.h"
#include "ScreenInterface.h"

namespace Ology {
namespace Plugin {

Manager::Manager(QObject *parent) :
    QObject(parent)
{
}

void Manager::loadPlugins(Ology::InitializePurpose purpose, const QStringList &pluginNames) {
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
        Plugin::InfoInterface* ii = qobject_cast<Plugin::InfoInterface*>(loader->instance());
        if (ii) {
            qDebug() << "Plugin details:" << ii->name() << ii->version() << ii->description();
        } else {
            qDebug() << "Plugin doesn't provide Ology::Plugin::InfoInterface";
        }
        _plugins << loader;

        
        // now try to initialize
        if (!ii->initialize(purpose)) {
            qWarning() << "Plugin failed to initialize: " << ii->name();
            return;
        }


        // Check Screen Interface
        Plugin::ScreenInterface* si = qobject_cast<Plugin::ScreenInterface*>(loader->instance());
        if (si) {
            qDebug() << "Plugin" << ii->name() << "provides ScreenInterface";
            foreach(const QString &screenId, si->screenIds()) {
                if (_screenFactory.contains(screenId)) {
                    Plugin::InfoInterface* otherIi = qobject_cast<Plugin::InfoInterface*>(_screenFactory[screenId]);
                    qWarning() << "Replacing screenId " << screenId << "factory" << otherIi->name() << "with" << ii->name();
                }
                _screenFactory[screenId] = loader->instance();
            }
        }
    }
}

QList<Plugin::InfoInterface*> Manager::ologyPlugins() {
    QList<Plugin::InfoInterface*> list;
    foreach(QPluginLoader* loader, _plugins) {
        Plugin::InfoInterface* ii = qobject_cast<Plugin::InfoInterface*>(loader->instance());
        if (ii) { list << ii; }
    }
    return list;
}
QList<ScreenInterface*> Manager::screenPlugins() {
    QList<Plugin::ScreenInterface*> list;
    foreach(QPluginLoader* loader, _plugins) {
        Plugin::ScreenInterface* si = qobject_cast<Plugin::ScreenInterface*>(loader->instance());
        if (si) { list << si; }
    }
    return list;
}


AbstractScreen* Manager::createScreen(const QString &id) {
    if (!_screenFactory.contains(id)) {
        qWarning() << "Error creating screen" << id << ": no plugin provides this screen-id";
        return NULL;
    }

    Plugin::ScreenInterface* si = qobject_cast<Plugin::ScreenInterface*>(_screenFactory[id]);
    AbstractScreen* screen = si->createScreen(id, OLOGY()->screenParent());
    if (!screen) {
        Plugin::InfoInterface* ii = qobject_cast<Plugin::InfoInterface*>(_screenFactory[id]);
        qWarning() << "Plugin" << ii->name() << "failed to create screen:" << id 
                   << "Error:" << ii->errorString() ;
    }

    return screen;
}


}}