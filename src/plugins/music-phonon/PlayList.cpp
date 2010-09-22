#include "PlayList.h"
#include "Interface.h"

namespace Ology {
namespace Plugin {
namespace MusicPhonon {

bool artistAlbumTrackLessThan(const Song &s1, const Song &s2) {
    if (s1.artist() < s2.artist()) { return true; }
    if (s1.artist() > s2.artist()) { return false; }
    if (s1.album() < s2.album()) { return true; }
    if (s1.album() > s2.album()) { return false; }
    if (s1.track() < s2.track()) { return true; }
    return false;
}

bool artistTitleLessThan(const Song &s1, const Song &s2) {
    if (s1.artist() < s2.artist()) { return true; }
    if (s1.artist() > s2.artist()) { return false; }
    if (s1.title() < s2.title()) { return true; }
    return false;
}

void PlayList::sort() {
    switch(_type) {
        case Mixed : qSort(this->begin(), this->end(), artistTitleLessThan     ); return;
        case Artist: qSort(this->begin(), this->end(), artistAlbumTrackLessThan); return;
        case Album : qSort(this->begin(), this->end(), artistAlbumTrackLessThan); return;
    }
}

PlayList PlayList::songsByArtist(const QString &artist) {
    PlayList songs;
    songs.setType(Artist);
    songs.setArtist(artist);
    songs.setName(Interface::tr("Artist: %1").arg(artist));

    foreach(const Song &song, *this) {
        if (song.artist() == artist) {
            songs << song;
        }
    }

    if (songs.isEmpty()) {
        qDebug() << "No songs found from this artist!";
    }

    return songs;
}

PlayList PlayList::songsInAlbum(const QString &artist, const QString &album) {
    PlayList songs;
    songs.setType(Album);
    songs.setArtist(artist);
    songs.setAlbum(artist);
    songs.setName(Interface::tr("Album: %1").arg(album));

    foreach(const Song &song, *this) {
        if ((song.artist() == artist) && (song.album() == album)) {
            songs << song;
        }
    }

    if (songs.isEmpty()) {
        qDebug() << "No songs found from this album!";
    }

    songs.sort(); // should be pretty fast: albums are small
    return songs;
}

}}}
