#include <QDebug>
#include "MovementActions.h"

namespace Ology {
namespace Core {

UpAction::UpAction(QObject *parent) :
    AbstractAction(Id::Action::Up, GlobalAction, parent)
{
    setTranslationContext(metaObject()->className());
    setUntranslatedName("Up");
    setUntranslatedDescription("Go up");
    setText(tr("Go up"));
}

DownAction::DownAction(QObject *parent) :
    AbstractAction(Id::Action::Down, GlobalAction, parent)
{
    setTranslationContext(metaObject()->className());
    setUntranslatedName("Down");
    setUntranslatedDescription("Go down");
    setText(tr("Go down"));
}

LeftAction::LeftAction(QObject *parent) :
    AbstractAction(Id::Action::Left, GlobalAction, parent)
{
    setTranslationContext(metaObject()->className());
    setUntranslatedName("Left");
    setUntranslatedDescription("Go left");
    setText(tr("Go left"));
}

RightAction::RightAction(QObject *parent) :
    AbstractAction(Id::Action::Right, GlobalAction, parent)
{
    setTranslationContext(metaObject()->className());
    setUntranslatedName("Right");
    setUntranslatedDescription("Go right");
    setText(tr("Go right"));
}

PageUpAction::PageUpAction(QObject *parent) :
    AbstractAction(Id::Action::PageUp, GlobalAction, parent)
{
    setTranslationContext(metaObject()->className());
    setUntranslatedName("PageUp");
    setUntranslatedDescription("Go one page up");
    setText(tr("Go one page up"));
}

PageDownAction::PageDownAction(QObject *parent) :
    AbstractAction(Id::Action::PageDown, GlobalAction, parent)
{
    setTranslationContext(metaObject()->className());
    setUntranslatedName("PageDown");
    setUntranslatedDescription("Go one page down");
    setText(tr("Go one page down"));
}

FirstAction::FirstAction(QObject *parent) :
    AbstractAction(Id::Action::First, GlobalAction, parent)
{
    setTranslationContext(metaObject()->className());
    setUntranslatedName("First");
    setUntranslatedDescription("Go to first item");
    setText(tr("Go to first item"));
}

LastAction::LastAction(QObject *parent) :
    AbstractAction(Id::Action::Last, GlobalAction, parent)
{
    setTranslationContext(metaObject()->className());
    setUntranslatedName("Last");
    setUntranslatedDescription("Go to last item");
    setText(tr("Go to last item"));
}


}}
