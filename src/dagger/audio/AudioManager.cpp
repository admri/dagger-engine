#include <audio/AudioManager.h>

#include <audio/AudioResource.h>

#include <SDL3/SDL_error.h>
#include <SDL3/SDL_log.h>
#include <SDL3_mixer/SDL_mixer.h>

namespace dagger
{

AudioManager::~AudioManager()
{
    if (mixer_)
    {
        stopAll();
        MIX_DestroyMixer(mixer_);
        mixer_ = nullptr;
    }
}

bool AudioManager::init()
{
    mixer_ = MIX_CreateMixerDevice(SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK, nullptr);
    if (!mixer_)
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Audio Error: %s", SDL_GetError());
        return false;
    }

    setMasterGain(1.0f);

    return true;
}

MIX_Track* AudioManager::play(const AudioResource& resource, int loops, float gain)
{
    if (!mixer_ || !resource.get()) return nullptr;

    MIX_Track* track = MIX_CreateTrack(mixer_);
    if (!track)
    {
        SDL_LogWarn(SDL_LOG_CATEGORY_AUDIO, "Failed to create track: %s", SDL_GetError());
        return nullptr;
    }

    if (!MIX_SetTrackAudio(track, resource.get()))
    {
        SDL_LogWarn(SDL_LOG_CATEGORY_AUDIO, "Failed to set track audio: %s", SDL_GetError());
    }

    MIX_SetTrackLoops(track, loops);
    MIX_SetTrackGain(track, gain);
    
    if (!MIX_PlayTrack(track, NULL))
    {
        SDL_LogWarn(SDL_LOG_CATEGORY_AUDIO, "Failed to play track: %s", SDL_GetError());
    }

    return track;
}

void AudioManager::stop(MIX_Track* track, int fade_out_ms)
{
    if (track)
    {
        MIX_StopTrack(track, fade_out_ms);
    }
}

void AudioManager::stopAll(int fade_out_ms)
{
    if (mixer_)
    {
        MIX_StopAllTracks(mixer_, fade_out_ms);
    }
}

void AudioManager::pause(MIX_Track* track)
{
    if (track)
    {
        MIX_PauseTrack(track);
    }
}

void AudioManager::resume(MIX_Track* track)
{
    if (track)
    {
        MIX_ResumeTrack(track);
    }
}

void AudioManager::setTrackGain(MIX_Track* track, float gain)
{
    if (track)
    {
        MIX_SetTrackGain(track, gain);
    }
}

void AudioManager::setMasterGain(float gain)
{
    if (mixer_)
    {
        MIX_SetMixerGain(mixer_, gain);
    }
}

} // namespace dagger
