#ifndef OLOGY_PLUGIN_INFO_INTERFACE
#define OLOGY_PLUGIN_INFO_INTERFACE

#include <QStringList>
#include <Ology/HasErrorString>
#include <Ology/HasSettings>
#include <Ology/InitializePurpose>

class QWidget;

namespace Ology {
namespace Plugin {

class InfoInterface : public HasSettings, public HasErrorString {
public:
    virtual QString name() const = 0;
    virtual QString version() const = 0;
    virtual QString description() const = 0;

    /*! initialize is automatically called after the plugin has been loaded.
     *
     * Implementations should load their settings. No screens or actions from
     * this plugin will be used until initialize() returns true.
     */
    virtual bool initialize(Ology::InitializePurpose purpose) = 0;
};


}}

Q_DECLARE_INTERFACE(Ology::Plugin::InfoInterface, "org.Ology.Plugin.InfoInterface");

#endif
