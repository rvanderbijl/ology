#ifndef OLOGY_PLUGIN_MANGER
#define OLOGY_PLUGIN_MANGER

#include <QObject>
#include <QStringList>
#include <QHash>

class QPluginLoader;

namespace Ology {
    class AbstractScreen;

namespace Plugin {
    class Interface;

class Manager : public QObject {
public:
    Manager(QObject *parent = 0);

    void loadPlugins(const QStringList &pluginNames);
    QList<Interface*> ologyPlugins();


    AbstractScreen* createScreen(const QString &id);

private:
     QList<QPluginLoader*> _plugins;
     QHash<QString, Interface*> _screenFactory;
};


}}

#endif
