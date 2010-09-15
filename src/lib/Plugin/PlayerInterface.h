#ifndef OLOGY_PLUGIN_PLAYER_INTERFACE
#define OLOGY_PLUGIN_PLAYER_INTERFACE

#include <QStringList>
#include <Ology/HasErrorString>
#include <Ology/AbstractAction>

class QWidget;

namespace Ology {
    class AbstractScreen;
    class AbstractAction;

namespace Plugin {

class PlayerInterface {
public:
    virtual void stop() = 0;
    virtual void play() = 0;
    virtual void next() = 0;
    virtual void prev() = 0;


};


}}

Q_DECLARE_INTERFACE(Ology::Plugin::PlayerInterface, "org.Ology.Plugin.PlayerInterface");

#endif
