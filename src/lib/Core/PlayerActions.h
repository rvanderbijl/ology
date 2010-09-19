#ifndef OLOGY_PLAYER_ACTIONS
#define OLOGY_PLAYER_ACTIONS

#include <Ology/AbstractAction>

namespace Ology {
namespace Core {

class PlayAction : public AbstractAction {
    Q_OBJECT
    USE_HAS_NAME_DESCRIPTION
public:
    PlayAction(QObject *parent);
    virtual void run();
};

class StopAction : public AbstractAction {
    Q_OBJECT
    USE_HAS_NAME_DESCRIPTION
public:
    StopAction(QObject *parent);
    virtual void run();
};

class NextAction : public AbstractAction {
    Q_OBJECT
    USE_HAS_NAME_DESCRIPTION
public:
    NextAction(QObject *parent);
    virtual void run();
};

class PrevAction : public AbstractAction {
    Q_OBJECT
    USE_HAS_NAME_DESCRIPTION
public:
    PrevAction(QObject *parent);
    virtual void run();
};

class PlayOrPauseAction : public AbstractAction {
    Q_OBJECT
    USE_HAS_NAME_DESCRIPTION
public:
    PlayOrPauseAction(QObject *parent);
    virtual void run();
};

class PauseOrUnpauseAction : public AbstractAction {
    Q_OBJECT
    USE_HAS_NAME_DESCRIPTION
public:
    PauseOrUnpauseAction(QObject *parent);
    virtual void run();
};


}}

#endif
