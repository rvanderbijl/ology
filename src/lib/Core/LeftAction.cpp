#include <QDebug>
#include "LeftAction.h"

namespace Ology {
namespace Core {

LeftAction::LeftAction(QObject *parent) :
    AbstractAction(Id::Action::Left, GlobalAction, parent)
{
    setTranslationContext(metaObject()->className());
    setUntranslatedName("Left");
    setUntranslatedDescription("Go left");
    setText(tr("Go left"));
}


}}
