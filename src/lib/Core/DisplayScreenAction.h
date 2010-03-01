#ifndef OLOGY_ACTION_DISPLAY_SCREEN
#define OLOGY_ACTION_DISPLAY_SCREEN

#include <Ology/AbstractAction>

namespace Ology {
namespace Core {

class DisplayScreenAction : public AbstractAction {
    Q_OBJECT

    Q_PROPERTY(QString screenId READ screenId WRITE setScreenId)

public:
    DisplayScreenAction(QObject *parent);
    DisplayScreenAction(const QString &screenId, QObject *parent);

    virtual QString name() const { return tr("Display screen %1").arg(screenId()); }
    virtual QString description() const { return tr("Display screen %1").arg(screenId()); }

    virtual void run();

    QString screenId() const { return _screenId; }
    void setScreenId(const QString &id) { _screenId = id; }

private:
    QString _screenId;
};

}}

#endif
