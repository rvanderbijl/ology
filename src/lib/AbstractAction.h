#ifndef OLOGY_ACTION
#define OLOGY_ACTION

#include <QAction>

namespace Ology {

class AbstractAction : public QAction {
    Q_OBJECT
    Q_PROPERTY(Ology::AbstractAction::Type type READ type)
    Q_ENUMS(Type)

public:
    enum Type { ScreenSpecific, Global, HighPriority };
    virtual Type type() const { return ScreenSpecific; }

public:
    AbstractAction(QObject *parent) : QAction(parent) {
        QObject::connect(this, SIGNAL(triggered()), SLOT(run()));
    }

public slots:
    virtual void run() = 0;
};


};

Q_DECLARE_METATYPE(Ology::AbstractAction::Type);

#endif
