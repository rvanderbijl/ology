#include <QDebug>
#include "UpAction.h"

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


}}
