#include <QDebug>
#include "LastAction.h"

namespace Ology {
namespace Core {

LastAction::LastAction(QObject *parent) :
    AbstractAction(Id::Action::Last, GlobalAction, parent)
{
    setTranslationContext(metaObject()->className());
    setUntranslatedName("Last");
    setUntranslatedDescription("Go to last item");
    setText(tr("Go to last item"));
}


}}
