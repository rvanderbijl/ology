#ifndef OLOGY_PLAY_HISTORY
#define OLOGY_PLAY_HISTORY

#include <QUrl>
#include <QList>

#include "PlayBase.h"
#include "PlayEntry.h"

namespace Ology {
namespace Plugin {
namespace MusicPhonon {

/*! PlayHistory keeps track of the urls played.
 *
 * Acts similarly to a browser back/forward mechanism. 
 * There is a "current" entry. It is possible to move the current forward and back through the history.
 *
 * The list may contain the same url multiple times.
 */
class PlayHistory : private PlayBase<PlayEntry> {
public:
    PlayHistory() {}

    /*! Adds a new entry after the current entry. The new entry becomes the current entry.
     * If the current entry is not the last entry in the history, all entries after the current entry are replaced with this new entry.
     */
    void append(const PlayEntry &entry);

    /*! Adds a new entry at the begining of the history. The new entry becomes the current entry.
     * The history must be empty or the current entry must be on the first entry prior to calling.
     */
    void prepend(const PlayEntry &entry);

    /*! Clears the history
     */
    void clear() { PlayBase<PlayEntry>::clear(); _current = -1; } 

    /*! Returns true if there is no history. 
     */
    bool isEmpty() const { return PlayBase<PlayEntry>::isEmpty(); }


    /*! Returns the list of all entries
     */
    //PlayList list() const { return (PlayList)*this; }

    PlayEntry current() const { return PlayBase<PlayEntry>::current(); }
    PlayEntry previous() { return PlayBase<PlayEntry>::previous(); }
    PlayEntry next() { return PlayBase<PlayEntry>::next(); }
    bool hasCurrent() const { return PlayBase<PlayEntry>::hasCurrent(); }
    bool hasPrevious() const { return PlayBase<PlayEntry>::hasPrevious(); }
    bool hasNext() const { return PlayBase<PlayEntry>::hasNext(); }
    int size() const { return PlayBase<PlayEntry>::size(); }
};


}}}


#endif
