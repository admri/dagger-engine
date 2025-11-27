#include <audio/SoundManager.h>

#include <SDL3/SDL_error.h>
#include <SDL3/SDL_log.h>

SoundManager::~SoundManager()
{
    clear();

    if (mixer_)
    {
        MIX_DestroyMixer(mixer_);
        mixer_ = nullptr;
    }
}

bool SoundManager::init()
{
    if (mixer_)
    {
        return true;
    }
    
    mixer_ = MIX_CreateMixerDevice(SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK, NULL);
    if (!mixer_)
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to open audio mixer: %s\n", SDL_GetError());
        return false;
    }

    return true;
}

Sound* SoundManager::loadSound(const std::string& id, const std::string& path, bool predecode)
{
    auto it = sounds_.find(id);
    if (it != sounds_.end())
    {
        return it->second.get();
    }

    auto sound = std::make_unique<Sound>();
    if (!sound->loadFromFile(mixer_, path))
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to load sound %s from %s\n", id.c_str(), path.c_str());
        return nullptr;
    }

    Sound* soundPtr = sound.get();
    sounds_[id] = std::move(sound);

    return soundPtr;
}

Sound* SoundManager::getSound(const std::string& id) const
{
    auto it = sounds_.find(id);
    if (it != sounds_.end())
    {
        return it->second.get();
    }
    return nullptr;
}

void SoundManager::unloadSound(const std::string& id)
{
    sounds_.erase(id);
}

void SoundManager::clear()
{
    sounds_.clear();
}