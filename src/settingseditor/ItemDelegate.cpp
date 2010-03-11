#include <QDebug>
#include <Ology/Setting>
#include "ItemDelegate.h"
#include "TreeWidget.h"


namespace Ology {
namespace SettingsEditor {

ItemDelegate::ItemDelegate(QObject *parent) : 
    QItemDelegate(parent) 
{}

QWidget * ItemDelegate::createEditor( QWidget * parent, const QStyleOptionViewItem & option, const QModelIndex & index ) const {
    QVariant v = index.data(AS_ROLE);
    if (v.userType() == qMetaTypeId<Ology::AbstractSetting*>()) {
        qDebug() << "is abstract-setting";
        AbstractSetting* setting = v.value<Ology::AbstractSetting*>();
        QWidget *w = setting->createEditor(parent);
        //w->installEventFilter(this);
        if (w) { return w; }
        // else fall through ...
        qDebug() << "has no special editor";
    }


    return QItemDelegate::createEditor(parent, option, index);
}


void ItemDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const {
    qDebug() << "setModelData";
    QVariant v = index.data(AS_ROLE);
    if (v.userType() == qMetaTypeId<Ology::AbstractSetting*>()) {
        qDebug() << "setModelData is abstract setting";
        AbstractSetting* setting = v.value<Ology::AbstractSetting*>();
        bool b = setting->takeValueFromEditor(editor);
        if (b) { 
            qDebug() << "take value succeeded";
            model->setData(index, setting->displayValue(), Qt::DisplayRole);
            return; 
        }
    }

    QItemDelegate::setModelData(editor, model, index);
}


}}
