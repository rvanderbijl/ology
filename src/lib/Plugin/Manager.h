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
public:
    Manager(QObject *parent = 0);

    void loadPlugins(Ology::InitializePurpose purpose, const QStringList &pluginNames);

    QList<InfoInterface*>   ologyPlugins();
    QList<ScreenInterface*> screenPlugins();

    AbstractScreen* createScreen(const QString &id);

private:
     QList<QPluginLoader*> _plugins;
     QHash<QString, QObject*> _screenFactory;
};


}}

#endif
