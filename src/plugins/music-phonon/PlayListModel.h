#ifndef OLOGOY_MUSIC_PHONON_PLAYLIST_MODEL
#define OLOGOY_MUSIC_PHONON_PLAYLIST_MODEL

#include <QAbstractItemModel>

#include "PlayList.h"

namespace Ology {
namespace Plugin {
namespace MusicPhonon {

class Interface;

class PlayListModel : public QAbstractItemModel {
    Q_OBJECT
public:
    PlayListModel(const PlayList &list, Interface *interface, QObject *parent);


    virtual QModelIndex index ( int row, int column, const QModelIndex & parent = QModelIndex() ) const;
    virtual int columnCount ( const QModelIndex & parent = QModelIndex() ) const;
    virtual QVariant data ( const QModelIndex & index, int role = Qt::DisplayRole ) const;
    virtual QModelIndex parent ( const QModelIndex & index ) const;
    virtual int rowCount ( const QModelIndex & parent = QModelIndex() ) const;

private:
    Interface *_interface;
    PlayList _playList;
};


}}}

#endif
