#ifndef OLOGY_MUSIC_PHONON_SONG
#define OLOGY_MUSIC_PHONON_SONG

#include <QUrl>
#include <QMetaType>
#include <taglib/fileref.h>
#include <taglib/tag.h>

namespace Ology {
namespace Plugin {
namespace MusicPhonon {


class Song : public QUrl {
public:
    Song(const QUrl & url = QUrl()) : QUrl(url), _tagRef(url.toLocalFile().toAscii()) {}

    QString artist() const { return _tagRef.tag()->artist().toCString(); }
    QString title() const { return _tagRef.tag()->title().toCString(); }
    QString album() const { return _tagRef.tag()->album().toCString(); }
    uint track() const { return _tagRef.tag()->track(); }

private:
    TagLib::FileRef _tagRef;
};


}}}

Q_DECLARE_METATYPE(Ology::Plugin::MusicPhonon::Song);

#endif
