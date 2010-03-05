#ifndef OLOGY_SETTINGS_EDITOR_ITEM_DELEGATE
#define OLOGY_SETTINGS_EDITOR_ITEM_DELEGATE

#include <QItemDelegate>


namespace Ology {
namespace SettingsEditor {

class ItemDelegate : public QItemDelegate {
    Q_OBJECT
public:
    ItemDelegate(QObject *parent);
    virtual QWidget* createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const;
    //virtual void setEditorData(QWidget *editor, const QModelIndex &index) const;
    virtual void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const;
};


}}

#endif
