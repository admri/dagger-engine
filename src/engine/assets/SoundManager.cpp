#include <assets/SoundManager.h>

#include <SDL3/SDL_error.h>
#include <SDL3/SDL_log.h>

namespace engine
{

SoundManager::~SoundManager()
{
    slots_.clear();

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
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION,
                     "Failed to open audio mixer: %s\n",
                     SDL_GetError());
        return false;
    }

    return true;
}

SoundHandle SoundManager::loadSound(const std::string& id, const std::string& path, bool predecode)
{
    auto it = nameToHandle_.find(id);
    if (it != nameToHandle_.end())
    {
        return it->second;
    }

    for (size_t i = 0; i < slots_.size(); ++i)
    {
        if (!slots_[i].active)
        {
            if (!slots_[i].sound.loadFromFile(mixer_, path, predecode))
            {
                SDL_LogError(SDL_LOG_CATEGORY_APPLICATION,
                            "Failed to load sound %s from %s\n",
                            id.c_str(),
                            path.c_str());
                return SoundHandle{};
            }

            slots_[i].active = true;
            slots_[i].generation++;

            SoundHandle handle = { static_cast<uint32_t>(i), slots_[i].generation };
            nameToHandle_[id] = handle;
            return handle;
        }
    }

    Slot newSlot;
    if (!newSlot.sound.loadFromFile(mixer_, path, predecode))
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION,
                    "Failed to load sound %s from %s\n",
                    id.c_str(),
                    path.c_str());
        return SoundHandle{};
    }

    newSlot.active = true;
    newSlot.generation = 1;

    slots_.push_back(std::move(newSlot));

    uint32_t newIndex = static_cast<uint32_t>(slots_.size() - 1);
    SoundHandle handle = { newIndex, 1 };

    nameToHandle_[id] = handle;
    return handle;
}

SoundHandle SoundManager::getHandle(const std::string& id) const
{
    auto it = nameToHandle_.find(id);
    if (it != nameToHandle_.end())
    {
        return it->second;
    }
    return SoundHandle{};
}

void SoundManager::unloadSound(const std::string& id)
{
    auto it = nameToHandle_.find(id);
    if (it == nameToHandle_.end()) return;

    SoundHandle handle = it->second;

    if (handle.isValid() && handle.index < slots_.size())
    {
        Slot& slot = slots_[handle.index];

        if (slot.active && slot.generation == handle.generation)
        {
            slot.active = false;
        }
    }

    nameToHandle_.erase(id);
}

void SoundManager::clear()
{
    slots_.clear();
}

Sound* SoundManager::resolve(SoundHandle handle)
{
    if (!handle.isValid() || handle.index >= slots_.size())
    {
        return nullptr;
    }

    Slot& slot = slots_[handle.index];

    if (slot.active && slot.generation == handle.generation)
    {
        return &slot.sound;
    }

    return nullptr;
}

} // namespace engine
