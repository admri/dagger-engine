#include <audio/Sound.h>

#include <SDL3/SDL_error.h>
#include <SDL3/SDL_log.h>

namespace engine
{

bool Sound::load(MIX_Mixer* mixer, const std::string& path, bool predecode)
{
    if (!mixer)
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION,
                     "MIX_Mixer is null!");
        return false;
    }

    audio_.reset(MIX_LoadAudio(mixer, path.c_str(), predecode));
    if (!audio_)
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION,
                     "Failed to load audio from %s: %s",
                     path.c_str(),
                     SDL_GetError());
        return false;
    }

    return true;
}

void Sound::destroy()
{
    if (!audio_) return;

    audio_.reset();
}

} // namespace engine
