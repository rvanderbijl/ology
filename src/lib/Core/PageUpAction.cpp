#include <QDebug>
#include "PageUpAction.h"

namespace Ology {
namespace Core {

PageUpAction::PageUpAction(QObject *parent) :
    AbstractAction(Id::Action::PageUp, GlobalAction, parent)
{
    setTranslationContext(metaObject()->className());
    setUntranslatedName("PageUp");
    setUntranslatedDescription("Go one page up");
    setText(tr("Go one page up"));
}


}}
