#ifndef OLOGY_MUSIC_PHONON_PLAYLIST
#define OLOGY_MUSIC_PHONON_PLAYLIST

#include <QList>
#include "Song.h"

namespace Ology {
namespace Plugin {
namespace MusicPhonon {

class PlayList : public QList<Song> {
public:
    enum Type {
        Mixed,
        Artist,
        Album
    };

    PlayList() : _type((Type)-1) {}
    PlayList(const QList<Song> &list) : QList<Song>(list), _type(Mixed) {}


    void setType(Type type) { _type = type; }
    void setName(const QString &name) { _name = name; }
    void setArtist(const QString &artist) { _artist = artist; }
    void setAlbum(const QString &album) { _album = album; }

    Type type() const { return _type; }
    QString name() const { return _name; }
    QString artist() const { return _artist; }
    QString album() const { return _album; }

    void sort();
    PlayList songsByArtist(const QString &artist);
    PlayList songsInAlbum(const QString &artist, const QString &album);

private:
    QString _name;
    QString _artist;
    QString _album;
    Type _type;
};


}}}

Q_DECLARE_METATYPE(Ology::Plugin::MusicPhonon::PlayList);

#endif
