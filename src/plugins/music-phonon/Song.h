#ifndef OLOGY_MUSIC_PHONON_SONG
#define OLOGY_MUSIC_PHONON_SONG

#include <QDebug>
#include <QUrl>
#include <QMetaType>
#include <taglib/fileref.h>
#include <taglib/tag.h>

namespace Ology {
namespace Plugin {
namespace MusicPhonon {


class Song : public QUrl {
public:
    Song(const QUrl & url = QUrl()) : QUrl(url), _track(0) {
        if (url.isEmpty()) { return; }
        TagLib::FileRef tagRef(url.toLocalFile().toAscii());
        if (!tagRef.isNull()) {
            _artist = tagRef.tag()->artist().toCString();
            _album = tagRef.tag()->album().toCString();
            _title = tagRef.tag()->title().toCString();
            _track = tagRef.tag()->track();
        } else {
            qWarning() << "Unable to read tags from file:" << url;
        }
    }

    QString artist() const { return _artist; }
    QString title() const { return _title; }
    QString album() const { return _album; }
    uint track() const { return _track; }

private:
    QString _artist;
    QString _album;
    QString _title;
    uint _track;
};


}}}

Q_DECLARE_METATYPE(Ology::Plugin::MusicPhonon::Song);

#endif
