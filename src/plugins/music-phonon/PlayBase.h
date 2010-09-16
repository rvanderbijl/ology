#ifndef OLOGY_PLAY_BASE
#define OLOGY_PLAY_BASE

#include <QList>

#include "PlayEntry.h"

namespace Ology {
namespace Plugin {
namespace MusicPhonon {

/*! PlayBase is an ordered list of type T.
 *
 * There is a "current" entry. The next() and previous() functions modify this value.
 *
 * The list may contain the same T multiple times.
 */
template<class T>
class PlayBase : public QList<T> {
public:
    /*! Returns the current entry. 
     *  If there is no current entry an empty T is returned.
     */
    T current() const;

    /*! Move the current entry to the previous entry
     *  If there is no next entry an empty T is returned.
     */
    T previous();

    /*! Move the current entry to the next entry
     *  If there is no next entry an empty T is returned.
     */
    T next();

    /*! Returns true if there is a current entry. 
     */
    bool hasCurrent() const;

    /*! Returns true if there is an entry before the current entry.
     */
    bool hasPrevious() const;

    /*! Returns true if there is an entry after the current entry.
     */
    bool hasNext() const;

protected:
    // iterator of ourselves, the QLinkedList<QUrl>:
    mutable int _current;
};



// CHECK_CURRENT makes sure that the current pointer is on a valid list entry
// Because _current is mutable, it works even in const functions. Perhaps this is a bad hack.
#define CHECK_CURRENT() \
    if (_current > this->size()) { _current = this->size() -1; } \
    if (_current < 0 && this->size()) { _current = 0; }

template<class T>
inline bool PlayBase<T>::hasPrevious() const {
    CHECK_CURRENT();
    if (this->isEmpty()) { return false; }
    return _current > 0;
}

template<class T>
inline bool PlayBase<T>::hasNext() const {
    CHECK_CURRENT();
    if (this->isEmpty()) { return false; }
    return _current < (this->size() - 1);
}

template<class T>
inline T PlayBase<T>::current() const {
    CHECK_CURRENT();
    if (this->isEmpty()) { return T(); }
    return QList<T>::at(_current);
}

template<class T>
inline T PlayBase<T>::previous() {
    CHECK_CURRENT();
    if (!hasPrevious()) { return T(); }
    _current--;
    return QList<T>::at(_current);
}

template<class T>
inline T PlayBase<T>::next() {
    CHECK_CURRENT();
    if (!hasNext()) { return T(); }
    _current++;
    return QList<T>::at(_current);
}

#undef CHECK_CURRENT

}}}

#endif
