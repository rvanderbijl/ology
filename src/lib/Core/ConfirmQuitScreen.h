#ifndef OLOGY_CORE_SCREEN_CONFIRM_QUIT
#define OLOGY_CORE_SCREEN_CONFIRM_QUIT

#include <Ology/AbstractScreen>
#include "CoreIds.h"

namespace Ology {
namespace Core {

class ConfirmQuitScreen : public AbstractScreen {
    Q_OBJECT
public:
    ConfirmQuitScreen(QWidget *parent);
    
    virtual QString id() const { return ID_SCREEN_CONFIRM_QUIT; }
    virtual QString name() const { return "Confirm Quit Screen"; }
    virtual QString description() const { return "Ask the user if they really which to quit Ology"; }

    virtual bool initialize(Ology::InitializePurpose initPurpose);

public slots:
    void quit();
    void cancel();
    
};


}}

#endif
