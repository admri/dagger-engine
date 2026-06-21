#ifndef DAGGER_AUDIO_AUDIO_MANAGER_H
#define DAGGER_AUDIO_AUDIO_MANAGER_H

#include <cstdint>

struct MIX_Mixer;
struct MIX_Track;

namespace dagger
{

class AudioResource;

class AudioManager
{
public:
    AudioManager() : mixer_(nullptr) {}
    ~AudioManager();

    [[nodiscard]] bool init();

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
