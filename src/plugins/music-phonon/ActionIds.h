#ifndef OLOGY_PLUGIN_MUSIC_PHONON_ACTION_IDS
#define OLOGY_PLUGIN_MUSIC_PHONON_ACTION_IDS

#include <QString>

namespace Ology {
namespace Id {
namespace Action {
namespace MusicPhonon {

const QString Play("music-phonon-play");
const QString Stop("music-phonon-stop");
const QString Next("music-phonon-next");
const QString Prev("music-phonon-prev");
const QString PlayOrPause("music-phonon-play-or-pause");
const QString PauseOrUnpause("music-phonon-pause-or-unpause");

const QString ToggleShuffle("music-phonon-toggle-shuffle");
const QString ToggleRepeatAll("music-phonon-toggle-repeat-all");
const QString SetRandomShuffle("music-phonon-set-random-shuffle");
const QString SetNoShuffle("music-phonon-set-no-shuffle");
const QString SetRepeatAll("music-phonon-set-repeat-all");
const QString SetNoRepeatAll("music-phonon-set-no-repeat-all");

const QString PlayArtist("music-phonon-play-artist");
const QString PlayAlbum("music-phonon-play-album");

}}}}
#endif
