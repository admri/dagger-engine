#include <audio/AudioSystem.h>

#include <SDL3/SDL_error.h>
#include <SDL3/SDL_log.h>
#include <SDL3_mixer/SDL_mixer.h>

namespace dagger
{

AudioSystem::~AudioSystem()
{
    if (mixer_)
    {
        MIX_DestroyMixer(mixer_);
        mixer_ = nullptr;
    }
}

bool AudioSystem::init()
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

} // namespace dagger
