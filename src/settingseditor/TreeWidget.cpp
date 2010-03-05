#include "TreeWidget.h"
#include "ItemDelegate.h"

namespace Ology {
namespace SettingsEditor {

TreeWidget::TreeWidget(QWidget *parent) :
    QTreeWidget(parent),
    _editableColumn(1)
{
    ItemDelegate *id = new ItemDelegate(this);
    setItemDelegate(id);
}

bool TreeWidget::edit(const QModelIndex &index, EditTrigger trigger, QEvent *event) {
    QTreeWidgetItem *item = itemFromIndex(index);
    if (!item || !item->parent()) { return false; } // all settings are child of the owner
    if (item->data(_editableColumn, AS_ROLE).isNull()) { return false; }

    if (index.column() == _editableColumn) {
        qDebug("starting edit");
        return QTreeWidget::edit(index, trigger, event);
    } else {
        return false;
    }
}


}}
