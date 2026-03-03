#ifndef AUDIO_SYSTEM_H
#define AUDIO_SYSTEM_H

#include <SDL3_mixer/SDL_mixer.h>

namespace engine
{

class AudioSystem
{
public:
    AudioSystem()
        : mixer_(nullptr) {}
    ~AudioSystem()
    {
        if (mixer_)
        {
            MIX_DestroyMixer(mixer_);
            mixer_ = nullptr;
        }
    }

    bool init()
    {
        mixer_ = MIX_CreateMixerDevice(SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK, NULL);
        if (!mixer_)
        {
            SDL_LogError(
                SDL_LOG_CATEGORY_APPLICATION,
                "Audio Error: %s",
                SDL_GetError());
            return false;
        }
        return true;
    }

    MIX_Mixer* getMixer()
    {
        return mixer_;
    }

private:
    MIX_Mixer* mixer_;
};

} // namespace engine

#endif // AUDIO_SYSTEM_H
