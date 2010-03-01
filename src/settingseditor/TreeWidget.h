#ifndef OLOGY_SETTINGS_EDITOR_TREE_WIDGET
#define OLOGY_SETTINGS_EDITOR_TREE_WIDGET

#include <QTreeWidget>

#define AS_ROLE (Qt::UserRole+1)

namespace Ology {
namespace SettingsEditor {

class TreeWidget : public QTreeWidget {
public:
    TreeWidget(QWidget *parent) : QTreeWidget(parent), _editableColumn(1) {}
    virtual bool edit(const QModelIndex &index, EditTrigger trigger, QEvent *event);

    void setEditableColumn(int column) { _editableColumn = column; }
private:
    int _editableColumn;
};

}}

#endif
