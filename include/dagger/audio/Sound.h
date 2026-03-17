#ifndef DAGGER_AUDIO_SOUND_H
#define DAGGER_AUDIO_SOUND_H

#include <memory>
#include <string>

struct MIX_Mixer;
struct MIX_Audio;

namespace dagger
{

struct MIXAudioDeleter
{
    void operator()(MIX_Audio* audio) const;
};

class Sound
{
public:
    bool load(MIX_Mixer* mixer, const std::string& path, bool predecode = true);
    MIX_Audio* get() const { return audio_.get(); }
    void destroy();

private:
    std::unique_ptr<MIX_Audio, MIXAudioDeleter> audio_;
};

} // namespace dagger

#endif // DAGGER_AUDIO_SOUND_H
