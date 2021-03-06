#ifndef OLOGY_PLUGIN_MENUS_MENU_SCREEN
#define OLOGY_PLUGIN_MENUS_MENU_SCREEN

#include <Ology/AbstractScreen>
#include "ui_MenuScreen.h"

class QPushButton;

namespace Ology {
namespace Plugin {
namespace Menus {

class MenuScreen : public AbstractScreen, public Ui::MenuScreen {
    Q_OBJECT
public:
    MenuScreen(const QString &id, QWidget *parent);
    
    void setMenuActions(const QList<AbstractAction*> &actions) { _menuActions = actions; }

    virtual QString id() const { return "menu-"+_screenId; }
    virtual QString name() const { return "Menu " + _screenId; }
    virtual QString description() const { return "Menu screen " + _screenId; }

    virtual void run();

    virtual QList<AbstractAction*> actions() const;
    virtual bool initialize(Ology::InitializePurpose initPurpose);

private slots:
     void setButtonToRestore();
    
private:
    QString _screenId;
    QList<AbstractAction*> _menuActions;
    QPushButton *_restoreButton;
};


}}}

#endif
