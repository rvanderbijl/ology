#include <QDebug>
#include "DownAction.h"

namespace Ology {
namespace Core {

DownAction::DownAction(QObject *parent) :
    AbstractAction(Id::Action::Down, GlobalAction, parent)
{
    setTranslationContext(metaObject()->className());
    setUntranslatedName("Down");
    setUntranslatedDescription("Go down");
    setText(tr("Go down"));
}


}}
