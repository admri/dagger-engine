#ifndef SOUND_H
#define SOUND_H

#include <SDL3_mixer/SDL_mixer.h>

#include <memory>
#include <string>

struct MIXAudioDeleter
{
    void operator()(MIX_Audio* audio) const
    {
        if (audio) MIX_DestroyAudio(audio);
    }
};

class Sound
{
public:
    bool loadFromFile(MIX_Mixer* mixer, const std::string& path, bool predecode = true);
    MIX_Audio* get() const { return audio_.get(); }

private:
    std::unique_ptr<MIX_Audio, MIXAudioDeleter> audio_;
};

#endif // SOUND_H
