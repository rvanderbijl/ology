#ifndef OLOGY_PLAY_ENTRY
#define OLOGY_PLAY_ENTRY

#include <QUrl>

namespace Ology {
namespace Plugin {
namespace MusicPhonon {


class PlayEntry {
public:
    PlayEntry(int position = -1, int listIndex = -1) : _allMusicPosition(position), _index(listIndex) {}

    int allMusicPosition() const { return _allMusicPosition; }
    int index() const { return _index; } 

    bool isNull() const { return _allMusicPosition == -1; }
    bool isValid() const { return _allMusicPosition != -1; }

private:
    int _allMusicPosition;
    int _index;
};


}}}


#endif
