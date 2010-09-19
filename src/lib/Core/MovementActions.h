#ifndef OLOGY_MOVEMENT_ACTIONS
#define OLOGY_MOVEMENT_ACTIONS

#include <Ology/AbstractAction>

namespace Ology {
namespace Core {

class UpAction : public AbstractAction {
    Q_OBJECT
    USE_HAS_NAME_DESCRIPTION
public:
    UpAction(QObject *parent);
    virtual void run() {}
};

class DownAction : public AbstractAction {
    Q_OBJECT
    USE_HAS_NAME_DESCRIPTION
public:
    DownAction(QObject *parent);
    virtual void run() {}
};

class LeftAction : public AbstractAction {
    Q_OBJECT
    USE_HAS_NAME_DESCRIPTION
public:
    LeftAction(QObject *parent);
    virtual void run() {}
};

class RightAction : public AbstractAction {
    Q_OBJECT
    USE_HAS_NAME_DESCRIPTION
public:
    RightAction(QObject *parent);
    virtual void run() {}
};

class PageUpAction : public AbstractAction {
    Q_OBJECT
    USE_HAS_NAME_DESCRIPTION
public:
    PageUpAction(QObject *parent);
    virtual void run() {}
};

class PageDownAction : public AbstractAction {
    Q_OBJECT
    USE_HAS_NAME_DESCRIPTION
public:
    PageDownAction(QObject *parent);
    virtual void run() {}
};

class FirstAction : public AbstractAction {
    Q_OBJECT
    USE_HAS_NAME_DESCRIPTION
public:
    FirstAction(QObject *parent);
    virtual void run() {}
};

class LastAction : public AbstractAction {
    Q_OBJECT
    USE_HAS_NAME_DESCRIPTION
public:
    LastAction(QObject *parent);
    virtual void run() {}
};

}}

#endif
