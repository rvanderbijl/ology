#include "TreeWidget.h"

namespace Ology {
namespace SettingsEditor {

bool TreeWidget::edit(const QModelIndex &index, EditTrigger trigger, QEvent *event) {
    QTreeWidgetItem *item = itemFromIndex(index);
    if (!item || !item->parent()) { return false; } // all settings are child of the owner
    if (item->data(0, AS_ROLE).isNull()) { return false; }

    if (index.column() == _editableColumn) {
        return QTreeWidget::edit(index, trigger, event);
    } else {
        return false;
    }
}


}}
