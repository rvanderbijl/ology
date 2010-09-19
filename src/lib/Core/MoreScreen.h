#ifndef OLOGY_CORE_SCREEN_MORE
#define OLOGY_CORE_SCREEN_MORE

#include "AbstractDialogScreen.h"
#include "CoreIds.h"

namespace Ology {
namespace Core {

class MoreScreen : public AbstractDialogScreen {
    Q_OBJECT
public:
    MoreScreen(QWidget *parent);
    
    virtual QString id() const { return Id::Screen::More; }
    virtual QString name() const { return "Show more options"; }
    virtual QString description() const { return "Show more options/actions available to the user on this screen"; }
    virtual QString dialogText() const { return tr("More actions:"); }

    virtual bool initialize(Ology::InitializePurpose initPurpose);

    virtual QList<AbstractAction*> actions() const { return _forScreen ? _forScreen->moreActions() : QList<AbstractAction*>(); }

private:
    AbstractScreen *_forScreen;
};


}}

#endif
