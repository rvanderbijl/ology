#ifndef OLOGOY_MUSIC_PHONON_PLAYLIST_MODEL
#define OLOGOY_MUSIC_PHONON_PLAYLIST_MODEL

#include <QAbstractItemModel>
#include <Phonon/MediaObject>

#include <Ology/AbstractPlayer>
#include <Ology/Setting>
#include "PlayList.h"
#include "Song.h"

namespace Ology {
namespace Plugin {
namespace MusicPhonon {

class Interface;

class Player : public QAbstractItemModel, public AbstractPlayer {
    Q_OBJECT
public:
    enum Shuffle { NoShuffle, RandomShuffle };
    enum Repeat { RepeatNone, RepeatAll };
    Q_ENUMS(Shuffle Repeat)

public:
    Player(QObject *parent);

    AbstractSetting *shuffleSetting() { return &_shuffleSetting; }
    AbstractSetting *repeatSetting() { return &_repeatSetting; }

// QAbstractItemModel: so that views can use this to show the currently play list
public:
    virtual QVariant headerData ( int section, Qt::Orientation orientation, int role = Qt::DisplayRole ) const;
    virtual QModelIndex index ( int row, int column, const QModelIndex & parent = QModelIndex() ) const;
    virtual int columnCount ( const QModelIndex & parent = QModelIndex() ) const;
    virtual QVariant data ( const QModelIndex & index, int role = Qt::DisplayRole ) const;
    virtual QModelIndex parent ( const QModelIndex & index ) const;
    virtual int rowCount ( const QModelIndex & parent = QModelIndex() ) const;

// useful interface
signals:
    void startedPlaying();
    void currentSongChanged(const Ology::Plugin::MusicPhonon::Song &newSong);

    void songProgressChanged(qint64 ms);
    void songLengthChanged(qint64 ms);

public:
    QModelIndex currentSongIndex() const;
    Song currentSong() const;
    PlayList playList() { return _playList; }
    void setPlayList(const PlayList &playList);


// Actions
public slots:
    virtual void play(const QModelIndex &index); 
    void toggleShuffle();
    void toggleRepeatAll();
    void setRandomShuffle();
    void setNoShuffle();
    void setRepeatAll();
    void setNoRepeatAll();

// Player interface:
public:
    virtual QString id() const { return "music-phonon-player"; }
    virtual bool isPlaying();
    virtual bool isPaused();
public slots:
    virtual void play();
    virtual void pause();
    virtual void unpause();
    virtual void stop();
    virtual void next();
    virtual void prev();
#ifdef Q_MOC_RUN
    void playOrPause();
    void pauseOrUnpause();
#endif




private:
    Interface *_interface;

    Phonon::MediaObject *_mediaPlayer;
    Setting<Shuffle> _shuffleSetting;
    Setting<Repeat> _repeatSetting;

    PlayList _playList;
    QList<int> _history;
    int _currentSongIndex;
    int _currentHistoryIndex;
};


}}}

Q_DECLARE_METATYPE(Ology::Plugin::MusicPhonon::Player::Shuffle);
Q_DECLARE_METATYPE(Ology::Plugin::MusicPhonon::Player::Repeat);

#endif
