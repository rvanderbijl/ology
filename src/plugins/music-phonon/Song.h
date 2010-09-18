#ifndef OLOGY_MUSIC_URL
#define OLOGY_MUSIC_URL

#include <QUrl>
#include <taglib/fileref.h>
#include <taglib/tag.h>

namespace Ology {
namespace Plugin {
namespace MusicPhonon {


class Song : public QUrl {
public:
    Song(const QUrl & url = QUrl()) : QUrl(url), _tagRef(url.toLocalFile().toAscii()) {}

    QString album() const { return _tagRef.tag()->album().toCString(); }
    QString artist() const { return _tagRef.tag()->artist().toCString(); }
    QString title() const { return _tagRef.tag()->title().toCString(); }

private:
    TagLib::FileRef _tagRef;
};


}}}

Q_DECLARE_METATYPE(Ology::Plugin::MusicPhonon::Song);

#endif
