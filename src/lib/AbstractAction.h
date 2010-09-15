#ifndef OLOGY_ABSTRACT_ACTION
#define OLOGY_ABSTRACT_ACTION

#include <QAction>
#include <Ology/HasNameDescription>
#include <Ology/HasSettings>
#include <Ology/GlobalActionIds>

namespace Ology {

class AbstractAction : 
    public QAction,
    public HasNameDescription,
    public HasSettings
{
    Q_OBJECT
    Q_PROPERTY(Ology::AbstractAction::Type type READ type)
    Q_PROPERTY(QString id READ id)
    Q_ENUMS(Type)

public:
    enum Type { ScreenSpecificAction, GlobalAction, HighPriorityAction };
    virtual Type type() const { return _type; }
    QString id() const { return _id; }

public:
    AbstractAction(const QString &id, Type type, QObject *parent) :
        QAction(parent),
        _id(id),
        _type(type)
    {
        QObject::connect(this, SIGNAL(triggered()), SLOT(run()));
    }

public slots:
    virtual void run() = 0;

private:
    QString _id;
    Type _type;
};


};

Q_DECLARE_METATYPE(Ology::AbstractAction::Type);

#endif
