#ifndef OLOGY_PLAY_LIST
#define OLOGY_PLAY_LIST

#include <QList>

#include "PlayBase.h"
#include "PlayEntry.h"

namespace Ology {
namespace Plugin {
namespace MusicPhonon {

/*! PlayList is an ordered list of QUrls to be played.
 *
 * There is a "current" entry. The next() and previous() functions modify this value.
 *
 * The list may contain the same url multiple times.
 */
class PlayList : public PlayBase<PlayEntry> {
public:
    /*! Returns the current entry. 
     *  If there is no current entry an empty QUrl is returned.
     */
    PlayEntry current() const { return PlayEntry(PlayBase<PlayEntry>::current().allMusicPosition(), _current); }

    /*! Move the current entry to the previous entry
     *  If there is no next entry an empty QUrl is returned.
     */
    PlayEntry previous() { return PlayEntry(PlayBase<PlayEntry>::previous().allMusicPosition(), _current); }

    /*! Move the current entry to the next entry
     *  If there is no next entry an empty QUrl is returned.
     */
    PlayEntry next() { return PlayEntry(PlayBase<PlayEntry>::previous().allMusicPosition(), _current); }

    /*! Move the current entry to a random entry.
     *  Uses rand(). Does not seed the random number generator.
     */
    PlayEntry random();

    void setCurrent(const PlayEntry &entry);

    bool isValid(const PlayEntry &entry);
};


}}}


#endif
