#ifndef OLOGY_PLUGIN_INTERFACE
#define OLOGY_PLUGIN_INTERFACE

#include <QStringList>
#include <Ology/HasErrorString>

class QWidget;

namespace Ology {
    class AbstractScreen;
    class AbstractAction;

namespace Plugin {

class Interface : public HasErrorString {
public:
    virtual QString name() const = 0;
    virtual QString version() const = 0;
    virtual QString description() const = 0;

    /*! initialize is automatically called after the plugin has been loaded.
     *
     * Implementations should load their settings. No screens or actions from
     * this plugin will be used until initialize() returns true.
     */
    virtual bool initialize() = 0;

    /*! screenNames returns the list of possible screen IDs.
     *
     * This list may be empty if the plugin doesn't provide any screens.
     */
    virtual QStringList screenIds() = 0;

    /*! createScreen should create a new instance of the specified screen
     *
     * The system gaurantees that name will always be a value from screenNames.
     * A new instance of the screen should be created, even if the screen already
     * has an instance. The function AbstractScreen::initialize() will be called
     * to see if this screen can currently be displayed.
     */
    virtual AbstractScreen* createScreen(const QString &id, QWidget *parent) = 0;


    /*! globalActions() should return a list of the global (and high-priority)
     * actions provided by this plugin.
     *
     * This list may be empty if no global actions are provided by this plugin.
     */
    virtual QList<AbstractAction*> globalActions() = 0;
};


}}

Q_DECLARE_INTERFACE(Ology::Plugin::Interface, "org.ology.plugin.interface");

#endif
