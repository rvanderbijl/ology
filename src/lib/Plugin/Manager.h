#ifndef OLOGY_PLUGIN_MANGER
#define OLOGY_PLUGIN_MANGER

#include <QObject>
#include <QStringList>
#include <QHash>

#include <Ology/InitializePurpose>

class QPluginLoader;

namespace Ology {
    class AbstractScreen;

namespace Plugin {
    class InfoInterface;
    class ScreenInterface;

class Manager : public QObject {
    Q_OBJECT
public:
    Manager(QObject *parent = 0);
    virtual ~Manager() {}

    QString pluginSearchDirectory() const { return _pluginSearchDirectory; }
    void setPluginSearchDirectory(const QString &dir) { _pluginSearchDirectory = dir; }

    void autoLoadPlugins(Ology::InitializePurpose purpose);
    void loadPlugins(Ology::InitializePurpose purpose, const QStringList &pluginNames);
    void unloadPlugin(Ology::Plugin::InfoInterface* plugin);
    void unloadPlugins();

    QList<QPluginLoader*>   pluginLoaders() const { return _plugins; }
    QList<InfoInterface*>   ologyPlugins() const;
    QList<ScreenInterface*> screenPlugins() const;

    AbstractScreen* createScreen(const QString &id);

    void registerScreens(QObject *instance);


    QString _pluginSearchDirectory;
    QList<QPluginLoader*> _plugins;
    QHash<QString, QObject*> _screenFactory;
};


}}

#endif
