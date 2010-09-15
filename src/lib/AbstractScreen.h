#ifndef OLOGY_ABSTRACT_SCREEN
#define OLOGY_ABSTRACT_SCREEN

#include <QMetaType>
#include <QWidget>
#include <Ology/HasNameDescription>
#include <Ology/HasSettings>
#include <Ology/HasErrorString>
#include <Ology/InitializePurpose>

namespace Ology {
    class AbstractAction;
    class SimpleScreenAction;

class AbstractScreen :
    public QWidget, 
    public HasNameDescription,
    public HasSettings, 
    public HasErrorString
{
    Q_OBJECT
public:
    enum SoundUsage { MergeSound, GrabSound };
    enum InputUsage { NormalInput, OnlyScreenActionsInput, StrictOnlyScreenActionInput };
    enum ScreenState { ScreenRunning, ScreenSuspended };

    Q_PROPERTY(QString name READ name);
    Q_PROPERTY(QString description READ description);
    Q_PROPERTY(QString errorString READ errorString);
    Q_PROPERTY(Ology::AbstractScreen::SoundUsage musicUsage READ musicUsage)
    Q_PROPERTY(Ology::AbstractScreen::InputUsage inputUsage READ inputUsage)
    Q_PROPERTY(Ology::AbstractScreen::ScreenState screenState READ screenState)
    Q_ENUMS(SoundUsage InputUsage ScreenState)

public:
    /*! Construct a new AbstractScreen.
     *
     * New screens are always created suspended. The Ology system will call run() as appropriate.
     */
    AbstractScreen(QWidget *parent) : QWidget(parent), _screenState(ScreenSuspended) { setAutoFillBackground(true); setEnabled(false); hide(); }

    /*! Return if this screen is running or suspended
     */
    ScreenState screenState() const { return _screenState; }

    /*! Return the SoundUsage.
     * 
     * The default is MergeSound. Override if sound usage is different.
     */
    virtual SoundUsage musicUsage() const { return MergeSound; }

    /*! Return the InputUsage.
     * 
     * The default is NormalInput. Override if input usage is different.
     */
    virtual InputUsage inputUsage() const { return NormalInput; }

    /*! Return the local actions for this screen.
     */
    virtual QList<AbstractAction*> actions() const { return _actions; }

    /*! Return a unique ID for this screen
     */
    virtual QString id() const = 0;
     
    /*! initialize the screen
     *
     * Initialize the screen, loading whatever data/etc that is required. This function will only
     * be called once. If this object is called with with Introspectdion purpose, it will never
     * be run().
     *
     * If the screen can not be displayed, return false. Ology will display errorString() to the user.
     */
    virtual bool initialize(Ology::InitializePurpose purpose) = 0;

public slots:
    /*! suspend is called when a child-screen is created and displayed.
     *
     * Subclases can pause animations, etc until resume is called. It
     * is not guaranteed that resume() will be called. 
     *
     * Implementers should call the AbstractScreen::suspend().
     *
     * \sa run()
     */
    virtual void suspend() { setScreenState(ScreenSuspended); setEnabled(false); }

    /*! run is called when this screen becomes the active screen.
     *
     * Implementers should call the AbstractScreen::run().
     * \sa suspend()
     */
    virtual void run() { setScreenState(ScreenRunning); setEnabled(true); }

protected:
    void setScreenState(ScreenState screenState) { _screenState = screenState; }
    void registerLocalAction(AbstractAction *action) { _actions << action; addAction((QAction*)action); }
    friend class SimpleScreenAction;

private:
    ScreenState _screenState;
    QList<AbstractAction*> _actions;
};


};

Q_DECLARE_METATYPE(Ology::AbstractScreen::SoundUsage);
Q_DECLARE_METATYPE(Ology::AbstractScreen::InputUsage);
Q_DECLARE_METATYPE(Ology::AbstractScreen::ScreenState);

#endif
