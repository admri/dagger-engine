#ifndef DAGGER_AUDIO_AUDIO_RESOURCE_H
#define DAGGER_AUDIO_AUDIO_RESOURCE_H

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

/// @brief Base wrapper class for an audio file loaded into memory.
class AudioResource
{
public:
    virtual ~AudioResource() = default;

    AudioResource() = default;
    AudioResource(AudioResource&&) noexcept = default;
    AudioResource& operator=(AudioResource&&) noexcept = default;

    AudioResource(const AudioResource&) = delete;
    AudioResource& operator=(const AudioResource&) = delete;
    
    MIX_Audio* get() const { return audio_.get(); }
    void destroy() { audio_.reset(); }

protected: 
    bool loadInternal(MIX_Mixer* mixer, const std::string& path, bool predecode);
    
    std::unique_ptr<MIX_Audio, MIXAudioDeleter> audio_;
};

/// @brief A short audio file (e.g., SFX) fully decoded into RAM for low-latency playback.
class Sound : public AudioResource
{
public:
    bool load(MIX_Mixer* mixer, const std::string& path)
    {
        return loadInternal(mixer, path, true); // predecode = true
    }
};

/// @brief A long audio file (e.g., BGM) streamed directly from disk to save RAM.
class Music : public AudioResource
{
public:
    bool load(MIX_Mixer* mixer, const std::string& path)
    {
        return loadInternal(mixer, path, false); // predecode = false
    }
};

} // namespace dagger

#endif // DAGGER_AUDIO_AUDIO_RESOURCE_H
