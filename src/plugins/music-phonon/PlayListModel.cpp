#include <QFileInfo>
#include "PlayListModel.h"
#include "Interface.h"

namespace Ology {
namespace Plugin {
namespace MusicPhonon {


PlayListModel::PlayListModel(const PlayList &list, Interface* interface, QObject *parent) :
    QAbstractItemModel(parent),
    _interface(interface),
    _playList(list)
{
}

int PlayListModel::columnCount( const QModelIndex & parent  ) const {
    Q_UNUSED(parent);
    return 3;
}
int PlayListModel::rowCount( const QModelIndex & parent ) const {
    return parent.isValid() ? 0 : _playList.count();
}
QModelIndex PlayListModel::index( int row, int column, const QModelIndex & parent ) const {
    Q_UNUSED(parent);
    return createIndex(row, column, (void*)NULL);
}
QModelIndex PlayListModel::parent( const QModelIndex & index ) const {
    Q_UNUSED(index);
    return QModelIndex();
}

QVariant PlayListModel::data( const QModelIndex & index, int role ) const {
    if (role == Qt::DisplayRole) {
        MusicUrl musicUrl = _interface->song(_playList[index.row()]);
        switch(index.column()) {
            case 0: return musicUrl.artist();
            case 1: return musicUrl.album();
            case 2: {
                const QString title = musicUrl.title();
                return title.isEmpty() ? QFileInfo(musicUrl.toLocalFile()).fileName() : title;
            }
        }
    }
    // catch-all
    return QVariant();
}



}}}
