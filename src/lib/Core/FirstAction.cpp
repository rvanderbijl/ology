#include <QDebug>
#include "FirstAction.h"

namespace Ology {
namespace Core {

FirstAction::FirstAction(QObject *parent) :
    AbstractAction(Id::Action::First, GlobalAction, parent)
{
    setTranslationContext(metaObject()->className());
    setUntranslatedName("First");
    setUntranslatedDescription("Go to first item");
    setText(tr("Go to first item"));
}


}}
