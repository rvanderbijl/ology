#ifndef OLOGY_PLAY_ENTRY
#define OLOGY_PLAY_ENTRY

#include <QUrl>

namespace Ology {
namespace Plugin {
namespace MusicPhonon {


class PlayEntry {
public:
    PlayEntry(int masterListIndex = -1, int playListIndex = -1) : _masterListIndex(masterListIndex), _playListIndex(playListIndex) {}

    int masterListIndex() const { return _masterListIndex; }
    int playListIndex() const { return _playListIndex; } 

    bool isNull() const { return _masterListIndex == -1; }
    bool isValid() const { return _masterListIndex != -1; }

private:
    int _masterListIndex;
    int _playListIndex;
};



}}}


#endif
