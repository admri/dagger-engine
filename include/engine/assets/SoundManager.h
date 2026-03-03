#ifndef SOUND_MANAGER_H
#define SOUND_MANAGER_H

#include <assets/ResourceHandle.h>
#include <audio/Sound.h>

#include <SDL3_mixer/SDL_mixer.h>

#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

namespace engine
{

class SoundManager
{
public:
    SoundManager() = default;
    SoundManager(const SoundManager&) = delete;
    ~SoundManager();

    bool init();

    SoundHandle loadSound(const std::string& id, const std::string& path, bool predecode = true);
    SoundHandle getHandle(const std::string& id) const;
    void unloadSound(const std::string& id);
    void clear();

    Sound* resolve(SoundHandle handle);

private:
    struct Slot
    {
        Sound sound;
        uint32_t generation = 0;
        bool active = false;
    };

    MIX_Mixer* mixer_ = nullptr;
    std::vector<Slot> slots_;
    std::unordered_map<std::string, SoundHandle> nameToHandle_;
};

} // namespace engine

#endif // SOUND_MANAGER_H
