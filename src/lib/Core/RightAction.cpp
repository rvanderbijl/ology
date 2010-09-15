#include <QDebug>
#include "RightAction.h"

namespace Ology {
namespace Core {

RightAction::RightAction(QObject *parent) :
    AbstractAction(Id::Action::Right, GlobalAction, parent)
{
    setTranslationContext(metaObject()->className());
    setUntranslatedName("Right");
    setUntranslatedDescription("Go right");
    setText(tr("Go right"));
}


}}
