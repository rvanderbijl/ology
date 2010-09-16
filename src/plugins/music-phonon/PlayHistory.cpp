#include "PlayHistory.h"

namespace Ology {
namespace Plugin {
namespace MusicPhonon {


void PlayHistory::prepend(const PlayEntry &entry) {
    if (!entry.isValid()) { return; }
    if (entry.playListIndex() < 0) { return; }

    // add entry, update current entry to this entry.
    PlayBase<PlayEntry>::prepend(entry);
    _current = 0;
}

void PlayHistory::append(const PlayEntry &entry) {
    if (!entry.isValid()) { return; }
    if (entry.playListIndex() < 0) { return; }

    // maybe remove all entries after the current entry 
    if (hasNext()) {
        this->erase(begin() + _current + 1, end());
    }

    // add entry, update current entry to this entry.
    PlayBase<PlayEntry>::append(entry);
    _current = this->size() - 1;
}




}}}
