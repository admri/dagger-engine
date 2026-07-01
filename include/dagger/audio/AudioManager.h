#ifndef DAGGER_AUDIO_AUDIO_MANAGER_H
#define DAGGER_AUDIO_AUDIO_MANAGER_H

#include <cstdint>

struct MIX_Mixer;
struct MIX_Track;

namespace dagger
{

class AudioResource;

/// @brief Core service responsible for initializing the audio device and managing playback.
class AudioManager
{
public:
    AudioManager() : mixer_(nullptr) {}
    ~AudioManager();

    /// @brief Initializes the underlying audio subsystem.
    [[nodiscard]] bool init();

    /// @brief Plays an audio resource on the first available channel.
    /// @param resource The Sound or Music resource to play.
    /// @param loops Number of times to loop (0 = play once, -1 = infinite).
    /// @param gain Volume multiplier (0.0 to 1.0).
    /// @return Pointer to the active track, or nullptr if playback failed.
    MIX_Track* play(const AudioResource& resource, int loops = 0, float gain = 1.0f);
    
    void stop(MIX_Track* track, int fade_out_ms = 0);
    void stopAll(int fade_out_ms = 0);

    void pause(MIX_Track* track);
    void resume(MIX_Track* track);

    void setTrackGain(MIX_Track* track, float gain);
    void setMasterGain(float gain);

    MIX_Mixer* getMixer() { return mixer_; }

private:
    MIX_Mixer* mixer_;
};

} // namespace dagger

#endif // DAGGER_AUDIO_AUDIO_MANAGER_H
