#include <stdlib.h>
#include <Phonon/AudioOutput>
#include <QFileInfo>
#include "Player.h"

namespace Ology {
namespace Plugin {
namespace MusicPhonon {


Player::Player(QObject *parent) :
    QAbstractItemModel(parent),
    _mediaPlayer(NULL),
    _shuffleSetting(tr("music-phonon"), tr("shuffle"), RandomShuffle, metaObject()->className(), "Shuffle", "Shuffle music setting", this),
    _repeatSetting(tr("music-phonon"), tr("repeat"), RepeatAll, metaObject()->className(), "Repeat", "Repeat playlist setting", this),
    _currentSongIndex(0),
    _currentHistoryIndex(0)
{
    qRegisterMetaTypeStreamOperators<Ology::Plugin::MusicPhonon::Player::Shuffle>("Ology::Plugin::MusicPhonon::Player::Shuffle");
    qRegisterMetaTypeStreamOperators<Ology::Plugin::MusicPhonon::Player::Repeat>("Ology::Plugin::MusicPhonon::Player::Repeat");


    _mediaPlayer = new Phonon::MediaObject(this);
    _mediaPlayer->setTickInterval(200);
    Phonon::AudioOutput *audioOutput = new Phonon::AudioOutput(Phonon::MusicCategory, this);
    Phonon::createPath(_mediaPlayer, audioOutput);

    connect(_mediaPlayer, SIGNAL(aboutToFinish()), SLOT(next()));
    connect(_mediaPlayer, SIGNAL(tick(qint64)), SIGNAL(songProgressChanged(qint64)));
    connect(_mediaPlayer, SIGNAL(totalTimeChanged(qint64)), SIGNAL(songLengthChanged(qint64)));
}

// AbstractItemModel functions
QVariant Player::headerData(int section, Qt::Orientation orientation, int role) const {
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole) {
        switch(section) {
            case 0: return tr("Artist");
            case 1: return tr("Albumn");
            case 2: return tr("Title");
        }
    }
    return QAbstractItemModel::headerData(section, orientation, role);
}
int Player::columnCount( const QModelIndex & parent  ) const {
    Q_UNUSED(parent);
    return 3;
}
int Player::rowCount( const QModelIndex & parent ) const {
    return parent.isValid() ? 0 : _playList.count();
}
QModelIndex Player::index( int row, int column, const QModelIndex & parent ) const {
    Q_UNUSED(parent);
    return createIndex(row, column, (void*)NULL);
}
QModelIndex Player::parent( const QModelIndex & index ) const {
    Q_UNUSED(index);
    return QModelIndex();
}

QVariant Player::data( const QModelIndex & index, int role ) const {
    if (role == Qt::DisplayRole) {
        Song song = _playList[index.row()];
        switch(index.column()) {
            case 0: return song.artist();
            case 1: return song.album();
            case 2: {
                const QString title = song.title();
                return title.isEmpty() ? QFileInfo(song.toLocalFile()).fileName() : title;
            }
        }
    }

    if (role == Qt::UserRole) {
        return QVariant::fromValue(_playList[index.row()]);
    }
    // catch-all
    return QVariant();
}


// Actions 
void Player::toggleShuffle() {
    _shuffleSetting = (_shuffleSetting == RandomShuffle) ? NoShuffle : RandomShuffle;
}
void Player::toggleRepeatAll() {
    _repeatSetting = (_repeatSetting == RepeatAll) ? RepeatNone : RepeatAll;
}
void Player::setRandomShuffle() {
    _shuffleSetting = RandomShuffle;
}
void Player::setNoShuffle() {
    _shuffleSetting = NoShuffle;
}
void Player::setRepeatAll() {
    _repeatSetting = RepeatAll;
}
void Player::setNoRepeatAll() {
    _repeatSetting = RepeatNone;
}


void Player::setPlayList(const QList<Song> &playList) {
    beginResetModel();
    _playList = playList;
    _history.clear();
    _currentSongIndex = 0;
    _currentHistoryIndex = 0;
    endResetModel();
    // TODO: if playing restart
}

QModelIndex Player::currentSongIndex() const {
    if (_playList.isEmpty()) { return QModelIndex(); }
    return index(_currentSongIndex, 0);
    
}
Song Player::currentSong() const {
    if (_playList.isEmpty()) { return Song(); }
    return _playList[_currentSongIndex];
}

// Player
void Player::play() {
    if (_playList.isEmpty()) { return; }
    if (_mediaPlayer->queue().isEmpty()) { 
        next(); 
        const Song song = _playList[_currentSongIndex];
        _mediaPlayer->enqueue( Phonon::MediaSource(song) );
    }
    _mediaPlayer->play();
}

void Player::play(const QModelIndex &index) {
    if (!index.isValid()) { return; }

    _currentSongIndex = index.row();
    if (_currentHistoryIndex < (_history.size() - 1)) {
        _history.erase(_history.begin() + _currentHistoryIndex + 1, _history.end());
    }
    _history.append(_currentSongIndex);
    _currentHistoryIndex = _history.size() - 1;


    Song song = _playList[_currentSongIndex];
    emit currentSongChanged(song);

    Phonon::MediaSource source(song);
    _mediaPlayer->enqueue(source);
    _mediaPlayer->setCurrentSource(source);
    _mediaPlayer->play();
    _mediaPlayer->clearQueue();
}

void Player::stop() {
    _mediaPlayer->stop();
}

void Player::next() {
    if (_playList.isEmpty()) { return; } // no songs

    // Verify there is a next song to goto:
    // If we've gone into the history, we always have a next song. 
    // If we're in Repeat-All mode, we always have a next song.
    // Otherwise we divide the size of the history compared to
    //    the size of the playlist. If there is a remainder (% operator),
    //    then there are more songs. Else we've gone through all entries
    //    in the play-list. 
    //    (Exception: if no history, we've not started, so there is a next.)
    // (This works for both shuffle modes because for both modes we can only go through the list once.)
    bool hasNext = false;
    if (_currentHistoryIndex != (_history.size() -1)) { hasNext = true; }
    else if (_repeatSetting == RepeatAll) { hasNext = true; }
    else {
        if (_shuffleSetting == RandomShuffle) {
            hasNext = (_history.size() < _playList.size());
        } else {
            hasNext = (_currentSongIndex < (_playList.size() - 1));
        }
    }
    if (!hasNext) { return; }


    // okay, switch to the next song
    bool fromHistory = false;
    int nextSongIndex = 0;
    if (_currentHistoryIndex < (_history.size() -1)) { 
        nextSongIndex = _history[ ++_currentHistoryIndex ];
        fromHistory = true;
    } else {
        // next in play list
        nextSongIndex = (_shuffleSetting == RandomShuffle) ? (rand() % _playList.size()) : ++_currentSongIndex;
    }

    if (nextSongIndex >= _playList.size()) {
        qDebug() << "Invalid (next) song index!";
        return;
    }

    _currentSongIndex = nextSongIndex;
    if (!fromHistory) {
        _history.append(nextSongIndex);
        _currentHistoryIndex++;
    }

    Song nextSong = _playList[ _currentSongIndex ];
    if (nextSong.isEmpty()) {
        qDebug() << "Next song is empty!";
        return;
    }

    emit currentSongChanged(nextSong);

    // play it(?)
    if (_mediaPlayer->state() == Phonon::PlayingState) {
        Phonon::MediaSource source(nextSong);
        _mediaPlayer->enqueue(source);
        _mediaPlayer->setCurrentSource(source);
        _mediaPlayer->play();
        _mediaPlayer->clearQueue();
    }
}


void Player::prev() {
    if (_playList.isEmpty()) { return; } // no songs

    // can we go to a previous song?
    bool hasPrevious = false;
    if (_currentHistoryIndex > 0) { hasPrevious = true; }
    else if (_repeatSetting == RepeatAll) { hasPrevious = true; }
    else {
        if (_shuffleSetting == RandomShuffle) {
            hasPrevious = true;
        } else {
            hasPrevious = (_currentSongIndex > 0);
        }
    }
    if (!hasPrevious) { return; }
    

    // figure out what the previous song is
    int prevSongIndex =  0;
    if (_currentHistoryIndex > 0) {
        prevSongIndex = _history[--_currentHistoryIndex];
    } else {
        // next in play list
        if (_shuffleSetting == RandomShuffle) {
            prevSongIndex = (rand() % _playList.size());
        } else {
            prevSongIndex = ((_currentSongIndex == 0) ? _playList.size() : _currentSongIndex) - 1;
        }
    }

    // sanity check
    if (prevSongIndex >= _playList.size()) {
        qDebug() << "Invalid (prev) history!";
        return;
    }

    // okay, switch to this song
    _currentSongIndex = prevSongIndex;
    Song prevSong = _playList[ _currentSongIndex ];
    if (prevSong.isEmpty()) {
        qDebug() << "Prev song is empty!";
        return;
    }

    emit currentSongChanged(prevSong);

    // play it(?)
    if (_mediaPlayer->state() == Phonon::PlayingState) {
        Phonon::MediaSource source(prevSong);
        _mediaPlayer->enqueue(source);
        _mediaPlayer->setCurrentSource(source);
        _mediaPlayer->play();
        _mediaPlayer->clearQueue();
    }
}



}}}

OLOGY_DECLARE_OPERATORS_FOR_ENUM(Ology::Plugin::MusicPhonon::Player, Shuffle);
OLOGY_DECLARE_OPERATORS_FOR_ENUM(Ology::Plugin::MusicPhonon::Player, Repeat);
