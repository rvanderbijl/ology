#include <QDebug>
#include "AbstractPlayer.h"
#include "PlayerActions.h"
#include "ManagerInterface.h"

namespace Ology {
namespace Core {

PlayAction::PlayAction(QObject *parent) :
    AbstractAction(Id::Action::Play, GlobalAction, parent)
{
    setTranslationContext(metaObject()->className());
    setUntranslatedName("Play");
    setUntranslatedDescription("Play (using the most recently used player)");
    setText(tr("Play"));
}

StopAction::StopAction(QObject *parent) :
    AbstractAction(Id::Action::Down, GlobalAction, parent)
{
    setTranslationContext(metaObject()->className());
    setUntranslatedName("Stop");
    setUntranslatedDescription("Stop (the currently active player)");
    setText(tr("Stop"));
}

NextAction::NextAction(QObject *parent) :
    AbstractAction(Id::Action::Next, GlobalAction, parent)
{
    setTranslationContext(metaObject()->className());
    setUntranslatedName("Next");
    setUntranslatedDescription("Next (on the most recently used player)");
    setText(tr("Next"));
}

PrevAction::PrevAction(QObject *parent) :
    AbstractAction(Id::Action::Prev, GlobalAction, parent)
{
    setTranslationContext(metaObject()->className());
    setUntranslatedName("Previous");
    setUntranslatedDescription("Previous (on the most recently used player)");
    setText(tr("Previous"));
}

PlayOrPauseAction::PlayOrPauseAction(QObject *parent) :
    AbstractAction(Id::Action::PlayOrPause, GlobalAction, parent)
{
    setTranslationContext(metaObject()->className());
    setUntranslatedName("Play or Pause");
    setUntranslatedDescription("Play or pause (the most recently used player)");
    setText(tr("Play/Pause"));
}

PauseOrUnpauseAction::PauseOrUnpauseAction(QObject *parent) :
    AbstractAction(Id::Action::PauseOrUnpause, GlobalAction, parent)
{
    setTranslationContext(metaObject()->className());
    setUntranslatedName("Pause or unpause");
    setUntranslatedDescription("Pause or unpause (the most recently used player");
    setText(tr("Pause"));
}


#define PLAYER() OLOGY()->currentPlayer()
#define SHOW_ACTION(action) \
    qDebug() << PLAYER()->id() << "is currently" \
             << (PLAYER()->isPlaying() ? "playing" : (PLAYER()->isPaused() ? "paused" : "stopped")) \
             << "-> invoking" << action;

void PlayAction::run() {
    if (PLAYER()) {
        SHOW_ACTION("Play");
        PLAYER()->play();
    }
}
void StopAction::run() {
    if (PLAYER()) {
        SHOW_ACTION("Stop");
        PLAYER()->stop();
    }
}
void NextAction::run() {
    if (PLAYER()) {
        SHOW_ACTION("Next");
        PLAYER()->next();
    }
}
void PrevAction::run() {
    if (PLAYER()) {
        SHOW_ACTION("Prev");
        PLAYER()->prev();
    }
}
void PlayOrPauseAction::run() {
    if (PLAYER()) {
        SHOW_ACTION("Play or pause");
        PLAYER()->playOrPause();
    }
}
void PauseOrUnpauseAction::run() {
    if (PLAYER()) {
        SHOW_ACTION("Pause or unpause");
        PLAYER()->pauseOrUnpause();
    }
}

}}
