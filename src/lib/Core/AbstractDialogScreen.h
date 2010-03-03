#ifndef OLOGY_CORE_ABSTRACT_DIALOG_SCREEN
#define OLOGY_CORE_ABSTRACT_DIALOG_SCREEN

#include <Ology/AbstractScreen>
#include "ui_AbstractDialogScreen.h"

namespace Ology {
namespace Core {

class AbstractDialogScreen : public AbstractScreen {
    Q_OBJECT
public:
    AbstractDialogScreen(QWidget *parent);
    
    virtual bool initialize(Ology::InitializePurpose initPurpose);

    virtual QString dialogText() const = 0;

private:
    Ui::AbstractDialogScreen _ui;
};


}}

#endif
