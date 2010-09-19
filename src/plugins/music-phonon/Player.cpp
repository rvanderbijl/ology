#include <stdlib.h>
#include <Phonon/AudioOutput>
#include <Ology/ManagerInterface>
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
            case 1: return tr("Album");
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

    if (index.row() == _currentSongIndex) {
        if (role == Qt::FontRole) {
            QFont font; // = QAbstractItemModel::data(index, role).value<QFont>();
            font.setItalic(true);
            return font;
        }
        if (role == Qt::ForegroundRole) {
            return QBrush(Qt::blue);
        }
    }

    if (role == Qt::UserRole) {
        return QVariant::fromValue(_playList[index.row()]);
    }

    // catch-all
    return QVariant(); //QAbstractItemModel::data(index, role);
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


void Player::setPlayList(const QList<Song> &playList, const QString & name) {
    const Song currentSong = this->currentSong();

    beginResetModel();
    _playListName = name;
    _playList = playList;
    _history.clear();
    _currentSongIndex = _playList.indexOf(currentSong);
    _currentHistoryIndex = 0;
    endResetModel();

    if (_currentSongIndex == -1) {
        _currentSongIndex = 0;
        if (this->isPlaying()) {
            // if current song is not in the new list, and we are playing, stop playing this song and start on the new list
            stop();
            _mediaPlayer->clearQueue();
            play();
        } else {
            _mediaPlayer->clearQueue();
            if (_playList.isEmpty()) { return; }
            const Song song = _playList[_currentSongIndex];
            _mediaPlayer->enqueue( Phonon::MediaSource(song) );
            //qDebug() << "Enqueueing" << song << "queue size:" << _mediaPlayer->queue().size();
        }
    } else {
        //qDebug() << "Existing song found, at" << _currentSongIndex;
    }
}

QModelIndex Player::currentSongIndex() const {
    if (_playList.isEmpty()) { return QModelIndex(); }
    return index(_currentSongIndex == -1 ? 0 : _currentSongIndex, 0);
    
}
Song Player::currentSong() const {
    if (_playList.isEmpty()) { return Song(); }
    return _playList[_currentSongIndex == -1 ? 0 : _currentSongIndex];
}

// Player
bool Player::isPlaying() {
    return (_mediaPlayer && _mediaPlayer->state() == Phonon::PlayingState);
}
bool Player::isPaused() {
    return (_mediaPlayer && _mediaPlayer->state() == Phonon::PausedState);
}
void Player::play() {
    if (_playList.isEmpty()) { return; }
    if (_mediaPlayer->currentSource().type() == Phonon::MediaSource::Invalid && _mediaPlayer->queue().isEmpty()) { 
        next(); 
        const Song song = _playList[_currentSongIndex];
        _mediaPlayer->enqueue( Phonon::MediaSource(song) );
    }
    OLOGY()->setCurrentPlayer(this);
    _mediaPlayer->play();
}

void Player::pause() {
    if (isPlaying()) {
        _mediaPlayer->pause();
    }
}
void Player::unpause() {
    if (isPaused()) {
        _mediaPlayer->play();
    }
}

void Player::play(const QModelIndex &index) {
    if (!index.isValid()) { return; }

    int oldIndex = _currentSongIndex;
    _currentSongIndex = index.row();
    if (_currentHistoryIndex < (_history.size() - 1)) {
        _history.erase(_history.begin() + _currentHistoryIndex + 1, _history.end());
    }
    _history.append(_currentSongIndex);
    _currentHistoryIndex = _history.size() - 1;

    Song song = _playList[_currentSongIndex];
    emit dataChanged(this->index(oldIndex,0), this->index(oldIndex,columnCount()-1));
    emit dataChanged(this->index(_currentSongIndex,0), this->index(_currentSongIndex,columnCount()-1));
    emit currentSongChanged(song);

    OLOGY()->setCurrentPlayer(this);
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

    // check if there is a next song
    bool hasNext = false;
    if (_shuffleSetting == RandomShuffle) {
        if (_repeatSetting == RepeatAll) { hasNext = true; }
        else if (_currentHistoryIndex != (_history.size() -1)) { hasNext = true; }

        //  TODO: this doesn't really mean we've played each song in the playlist,
        //        it just means we've played _playList.size() number of songs.
        //        Make the rand() function better so that we are guaranteed to hit each number before seeing a duplicate?
        else { hasNext = (_history.size() < _playList.size()); } 

    } else {
        if (_repeatSetting == RepeatAll) { hasNext = true; }
        else { hasNext = (_currentSongIndex < (_playList.size() - 1)); }
    }
    if (!hasNext) { return; }


    // okay, figure out which song is next 
    bool fromHistory = false;
    int nextSongIndex = 0;
    if (_shuffleSetting == RandomShuffle) {
        if (_currentHistoryIndex < (_history.size() - 1)) {
            nextSongIndex = _history[++_currentHistoryIndex];
            fromHistory = true;
        } else {
            nextSongIndex = (rand() % _playList.size());
        }
    } else {
        // next in play list (with wrap if RepeatAll)
        if ((_currentSongIndex+1) >= _playList.size()) {
            if (_repeatSetting == RepeatAll) {
                nextSongIndex = 0;
            } else {
                return;
            }
        } else {
            nextSongIndex = _currentSongIndex + 1;
        }
    }


    if (nextSongIndex >= _playList.size()) {
        qDebug() << "Invalid (next) song index!";
        return;
    }

    const int oldIndex = _currentSongIndex;
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

    emit dataChanged(index(oldIndex,0), index(oldIndex,columnCount()-1));
    emit dataChanged(index(_currentSongIndex,0), index(_currentSongIndex,columnCount()-1));
    emit currentSongChanged(nextSong);

    // play it(?)
    if (this->isPlaying()) {
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
    if (_shuffleSetting == RandomShuffle) {
        if (_repeatSetting == RepeatAll) { hasPrevious = true; }
        else if (_currentHistoryIndex > 0) { hasPrevious = true; }
    } else {
        if (_repeatSetting == RepeatAll) { hasPrevious = true; }
        else { hasPrevious = (_currentSongIndex > 0); }
    }
    if (!hasPrevious) { return; }
    

    // figure out what the previous song is
    int prevSongIndex =  0;
    if (_shuffleSetting == RandomShuffle) {
        if (_currentHistoryIndex > 0) {
            prevSongIndex = _history[--_currentHistoryIndex];
        } else {
            prevSongIndex = (rand() % _playList.size());
            _history.prepend(prevSongIndex);
        }
    } else {
        if (_repeatSetting == RepeatAll) {
            prevSongIndex = ((_currentSongIndex == 0) ? _playList.size() : _currentSongIndex) - 1;
        } else {
            prevSongIndex = qMax(0, _currentSongIndex - 1);
        }
    }

    // sanity check
    if (prevSongIndex >= _playList.size()) {
        qDebug() << "Invalid (prev) history!";
        return;
    }

    // okay, switch to this song
    const int oldIndex = _currentSongIndex;
    _currentSongIndex = prevSongIndex;
    Song prevSong = _playList[ _currentSongIndex ];
    if (prevSong.isEmpty()) {
        qDebug() << "Prev song is empty!";
        return;
    }

    emit dataChanged(index(oldIndex,0), index(oldIndex,columnCount()-1));
    emit dataChanged(index(_currentSongIndex,0), index(_currentSongIndex,columnCount()-1));
    emit currentSongChanged(prevSong);

    // play it(?)
    if (this->isPlaying()) {
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
