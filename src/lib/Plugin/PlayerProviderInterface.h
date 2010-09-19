#ifndef OLOGY_PLUGIN_PLAYERPROVICER_INTERFACE
#define OLOGY_PLUGIN_PLAYERPROVICER_INTERFACE

#include <QtPlugin>

namespace Ology {
    class AbstractPlayer;

namespace Plugin {

class PlayerProviderInterface {
public:
    virtual AbstractPlayer* player() const = 0;
};


}}

Q_DECLARE_INTERFACE(Ology::Plugin::PlayerProviderInterface, "org.Ology.Plugin.PlayerProviderInterface");

#endif
