#include <QDebug>
#include <QPluginLoader>
#include <QLibrary>
#include <QDir>
#include <QFileInfo>

#include "ManagerInterface.h"
#include "Manager.h"
#include "InfoInterface.h"
#include "ScreenProviderInterface.h"

namespace Ology {
namespace Plugin {

Manager::Manager(QObject *parent) :
    QObject(parent)
{
}

void Manager::autoLoadPlugins( Ology::InitializePurpose purpose ) {
    QStringList files;

    QDir dir(pluginSearchDirectory());
    foreach(QFileInfo fi, dir.entryInfoList(QDir::Files|QDir::Executable)) {
        if (QLibrary::isLibrary(fi.fileName())) {
            files << fi.fileName();
        }
    }

    loadPlugins(purpose, files);
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
        Plugin::ScreenProviderInterface* si = qobject_cast<Plugin::ScreenProviderInterface*>(loader->instance());
        if (si) {
            qDebug() << "Plugin" << ii->name() << "provides ScreenProviderInterface";
            registerScreens(loader->instance());
        }
    }
}

void Manager::unloadPlugin(InfoInterface *plugin) {
    foreach(QPluginLoader* loader, _plugins) {
        Plugin::InfoInterface* ii = qobject_cast<Plugin::InfoInterface*>(loader->instance());
        if (ii == plugin) {
            loader->unload();
            QHash<QString, QPluginLoader*>::key_type key;
            foreach(key, _screenFactory.keys(loader)) {
                _screenFactory.remove(key);
            }
            _plugins.removeAll(loader);
            delete loader;
            loader = NULL;
            return;
        }
    }
}
void Manager::unloadPlugins() {
    foreach(QPluginLoader* loader, _plugins) {
        loader->unload();
        delete loader;
        loader = NULL;
    }
    _screenFactory.clear();
    _plugins.clear();
    registerScreens(OLOGY()->coreInstance());
}

QList<Plugin::InfoInterface*> Manager::ologyPlugins() const {
    QList<Plugin::InfoInterface*> list;
    list << OLOGY()->coreInfoInterface();
    foreach(QPluginLoader* loader, _plugins) {
        Plugin::InfoInterface* ii = qobject_cast<Plugin::InfoInterface*>(loader->instance());
        if (ii) { list << ii; }
    }
    return list;
}
QList<ScreenProviderInterface*> Manager::screenProviderPlugins() const {
    QList<Plugin::ScreenProviderInterface*> list;
    list << OLOGY()->coreScreenProviderInterface();
    foreach(QPluginLoader* loader, _plugins) {
        Plugin::ScreenProviderInterface* si = qobject_cast<Plugin::ScreenProviderInterface*>(loader->instance());
        if (si) { list << si; }
    }
    return list;
}


void Manager::registerScreens(QObject *object) {
    Plugin::InfoInterface* ii = qobject_cast<Plugin::InfoInterface*>(object);
    Plugin::ScreenProviderInterface* si = qobject_cast<Plugin::ScreenProviderInterface*>(object);

    foreach(const QString &screenId, si->screenIds()) {
        if (_screenFactory.contains(screenId)) {
            Plugin::InfoInterface* otherIi = qobject_cast<Plugin::InfoInterface*>(_screenFactory[screenId]);
            qWarning() << "Replacing screenId " << screenId << "factory" << otherIi->name() << "with" << ii->name();
        }
        _screenFactory[screenId] = object;
    }
}

AbstractScreen* Manager::createScreen(const QString &id) {
    if (!_screenFactory.contains(id)) {
        qWarning() << "Error creating screen" << id << ": no plugin provides this screen-id";
        return NULL;
    }

    Plugin::ScreenProviderInterface* si = qobject_cast<Plugin::ScreenProviderInterface*>(_screenFactory[id]);
    AbstractScreen* screen = si->createScreen(id, OLOGY()->screenParent());
    if (!screen) {
        Plugin::InfoInterface* ii = qobject_cast<Plugin::InfoInterface*>(_screenFactory[id]);
        qWarning() << "Plugin" << ii->name() << "failed to create screen:" << id 
                   << "Error:" << ii->errorString() ;
    }

    return screen;
}


}}
