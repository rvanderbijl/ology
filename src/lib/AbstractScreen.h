#ifndef OLOGY_SCREEN
#define OLOGY_SCREEN

#include <QWidget>
#include <QMetaType>
#include <Ology/HasErrorString>
#include <Ology/InitializePurpose>

namespace Ology {
    class AbstractAction;

class AbstractScreen : public QWidget, public HasErrorString {
    Q_OBJECT
public:
    enum MusicUsage { MergeSound, GrabSound };
    enum InputUsage { NormalInput, OnlyScreenActionsInput, StrictOnlyScreenActionInput };
    enum ScreenState { ScreenRunning, ScreenSuspended };

    Q_PROPERTY(QString name READ name);
    Q_PROPERTY(QString description READ description);
    Q_PROPERTY(QString errorString READ errorString);
    Q_PROPERTY(Ology::AbstractScreen::MusicUsage musicUsage READ musicUsage)
    Q_PROPERTY(Ology::AbstractScreen::InputUsage inputUsage READ inputUsage)
    Q_PROPERTY(Ology::AbstractScreen::ScreenState screenState READ screenState)
    Q_ENUMS(MusicUsage InputUsage ScreenState)

public:
    /*! Construct a new AbstractScreen.
     *
     * New screens are always created suspended. The Ology system will call run() as appropriate.
     */
    AbstractScreen(QWidget *parent) : QWidget(parent), _screenState(ScreenSuspended) { setAutoFillBackground(true); setEnabled(false); }

    ScreenState screenState() const { return _screenState; }

    /*! Return the MusicUsage.
     * 
     * The default is MergeSound. Override if sound usage is different.
     */
    virtual MusicUsage musicUsage() const { return MergeSound; }

    /*! Return the InputUsage.
     * 
     * The default is NormalInput. Override if input usage is different.
     */
    virtual InputUsage inputUsage() const { return NormalInput; }

    /*! Return the local actions for this screen.
     */
    virtual QList<AbstractAction*> actions() const = 0;

    /*! Return a unique ID for this screen
     */
    virtual QString id() const = 0;
     
    /*! Return the transalated name for this screen
     */
    virtual QString name() const = 0;
     
    /*! Return the transalated description for this screen
     */
    virtual QString description() const = 0;


    /*! initialize the screen
     *
     * initialize the screen, loading whatever data/etc that is required
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

private:
    ScreenState _screenState;
};


};

Q_DECLARE_METATYPE(Ology::AbstractScreen::MusicUsage);
Q_DECLARE_METATYPE(Ology::AbstractScreen::InputUsage);
Q_DECLARE_METATYPE(Ology::AbstractScreen::ScreenState);

#endif
