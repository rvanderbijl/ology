#include <stdlib.h>

#include "PlayList.h"

namespace Ology {
namespace Plugin {
namespace MusicPhonon {


PlayEntry PlayList::random() {
    if (this->isEmpty()) { return PlayEntry(); } // avoid crash if empty
    _current = (rand() % this->size()); // will be between 0 and this->size(): thus a valid entry
    return current();
}

void PlayList::setCurrent(const PlayEntry &entry) {
    if (!isValid(entry)) { return; }
    _current = entry.playListIndex();
}

bool PlayList::isValid(const PlayEntry &entry) {
    if (!entry.isValid()) { return false; } // the url in the entry is valid, right?
    if ((entry.playListIndex() < 0) || (entry.playListIndex() >= this->size())) { return false; }
    return entry.masterListIndex() == at(entry.playListIndex()).masterListIndex();
}


}}}
