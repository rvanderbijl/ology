#include <QDebug>
#include "PageDownAction.h"

namespace Ology {
namespace Core {

PageDownAction::PageDownAction(QObject *parent) :
    AbstractAction(Id::Action::PageDown, GlobalAction, parent)
{
    setTranslationContext(metaObject()->className());
    setUntranslatedName("PageDown");
    setUntranslatedDescription("Go one page down");
    setText(tr("Go one page down"));
}


}}
