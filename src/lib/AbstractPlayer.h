#ifndef OLOGY_PLUGIN_ABSTRACT_PLAYER
#define OLOGY_PLUGIN_ABSTRACT_PLAYER

namespace Ology {

class AbstractPlayer {
public:
    /*! id() returns the id this player
     */
    virtual QString id() const = 0;

    /*! stop() stops playing.
     *
     * It may remember the current position in the stream, but is not required.
     *
     * If isPaused() returns true, it is recommended that the player changes state so that isPaused returns false (but, of course, should not start playing).
     */
    virtual void stop() = 0;

    /*! play() start playing.
     *
     * If the player was previously paused, this should unpause it, resuming from the paused position if possible. 
     *
     * \sa stop pause playOrPause
     */
    virtual void play() = 0;
    
    /*! pause() pauses playing.
     *
     * If possible, the current position in the stream should be remembered for when playing resumes.
     *
     * If the player is not playing, then this does nothing.
     *
     * This function should *NOT* be used to implement standard "pause" button functionality. Instead, use pauseOrUnpause.
     *
     * \sa unpause pauseOrUnpause
     */
    virtual void pause() = 0;

    /*! unpause() resumes playing.
     *
     * If possible, the current position in the stream should be remembered for when playing resumes.
     *
     * If the player is not paused, then this does nothing.
     *
     * \sa pause pauseOrUnpause
     */
    virtual void unpause() = 0;

    /*! next() goes to the next stream.
     *
     * The interpretation of this function depends on the player-instance. For example, a music player will move to the next song,
     * while a radio player will move to the next station. A tv player might do nothing, and a dvd player may move to the next chapter.
     * If appropriate, this function might not do anything.
     */
    virtual void next() = 0;

    /*! prev() goes to the previous stream.
     *
     * The interpretation of this function depends on the player-instance. For example, a music player will move to the previous song,
     * while a radio player will move to the previous station. A tv player might do nothing, and a dvd player may move to the previous chapter.
     * If appropriate, this function might not do anything.
     */
    virtual void prev() = 0;

    /*! isPlaying() returns true if the player is currently playing. 
     *
     * A paused player should return false.
     */
    virtual bool isPlaying() = 0;

    /*! isPaused() returns true if the player is currently paused. 
     */
    virtual bool isPaused() = 0;

    /*! playOrPause() handles the situation where the play button is also the pause button.
     *
     * If the player is playing, it is paused, otherwise playing is started (either by unpausing or starting play).
     */
    virtual void playOrPause() {
        if (isPlaying() || isPaused()) {
            pauseOrUnpause();
        } else {
            play();
        }
    }

    /*! pauseOrUnpause() pauses or unpauses playing as appropriate. 
     *
     * Actions for the standard "pause" button functionality should use this function (instead of pause()).
     *
     * The default implementation uses pause and unpause as appropriate.
     *
     * If the player is not playing and is not paused, then this does nothing.
     */
    virtual void pauseOrUnpause() {
        if (isPlaying()) {
            pause();
        } else if (isPaused()) {
            unpause();
        }
    }
};

}

#endif
