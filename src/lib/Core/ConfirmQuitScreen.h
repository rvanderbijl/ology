#ifndef OLOGY_CORE_SCREEN_CONFIRM_QUIT
#define OLOGY_CORE_SCREEN_CONFIRM_QUIT

#include "AbstractDialogScreen.h"
#include "CoreIds.h"

namespace Ology {
namespace Core {

class ConfirmQuitScreen : public AbstractDialogScreen {
    Q_OBJECT
public:
    ConfirmQuitScreen(QWidget *parent);
    
    virtual QString id() const { return Id::Screen::ConfirmQuit; }
    virtual QString name() const { return "Confirm Quit Screen"; }
    virtual QString description() const { return "Ask the user if they really which to quit Ology"; }
    virtual QString dialogText() const { return tr("Are you sure you wish to cancel?"); }

    virtual bool initialize(Ology::InitializePurpose initPurpose);


public slots:
    void quit();
    void cancel();
    
};


}}

#endif
