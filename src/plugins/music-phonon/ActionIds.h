#ifndef OLOGY_PLUGIN_MUSIC_PHONON_ACTION_IDS
#define OLOGY_PLUGIN_MUSIC_PHONON_ACTION_IDS

#include <QString>

namespace Ology {
namespace Id {
namespace Action {

const QString MusicPhononPlay("music-phonon-play");
const QString MusicPhononStop("music-phonon-stop");
const QString MusicPhononNext("music-phonon-next");
const QString MusicPhononPrev("music-phonon-prev");

const QString MusicPhononToggleShuffle("music-phonon-toggle-shuffle");
const QString MusicPhononToggleRepeatAll("music-phonon-toggle-repeat-all");
const QString MusicPhononSetRandomShuffle("music-phonon-set-random-shuffle");
const QString MusicPhononSetNoShuffle("music-phonon-set-no-shuffle");
const QString MusicPhononSetRepeatAll("music-phonon-set-repeat-all");
const QString MusicPhononSetNoRepeatAll("music-phonon-set-no-repeat-all");

const QString MusicPhononPlayArtist("music-phonon-play-artist");
const QString MusicPhononPlayAlbum("music-phonon-play-album");

}}}
#endif
