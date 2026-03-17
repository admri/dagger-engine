#ifndef DAGGER_AUDIO_AUDIO_SYSTEM_H
#define DAGGER_AUDIO_AUDIO_SYSTEM_H

struct MIX_Mixer;

namespace dagger
{

class AudioSystem
{
public:
    AudioSystem()
        : mixer_(nullptr) {}
    ~AudioSystem();

    bool init();

    MIX_Mixer* getMixer() { return mixer_; }

private:
    MIX_Mixer* mixer_;
};

} // namespace dagger

#endif // DAGGER_AUDIO_AUDIO_SYSTEM_H
