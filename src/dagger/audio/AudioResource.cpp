#include <audio/AudioResource.h>

#include <SDL3/SDL_error.h>
#include <SDL3/SDL_log.h>
#include <SDL3_mixer/SDL_mixer.h>

namespace dagger
{

void MIXAudioDeleter::operator()(MIX_Audio* audio) const
{
    if (audio)
    {
        MIX_DestroyAudio(audio);
    }
}

bool AudioResource::loadInternal(MIX_Mixer* mixer, const std::string& path, bool predecode)
{
    if (!mixer)
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "MIX_Mixer is null! Cannot load audio.");
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

} // namespace dagger
