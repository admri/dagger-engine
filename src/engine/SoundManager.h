#ifndef SOUND_MANAGER_H
#define SOUND_MANAGER_H

#include "Sound.h"

#include <SDL3_mixer/SDL_mixer.h>

#include <memory>
#include <string>
#include <unordered_map>

class SoundManager
{
public:
    SoundManager();
    SoundManager(const SoundManager&) = delete;
    ~SoundManager();

    Sound* loadSound(const std::string& id, const std::string& path, bool predecode = true);
    Sound* getSound(const std::string& id) const;
    void unloadSound(const std::string& id);

    void clear();

private:
    MIX_Mixer* mixer_ = nullptr;
    std::unordered_map<std::string, std::unique_ptr<Sound>> sounds_;
};

#endif // SOUND_MANAGER_H
