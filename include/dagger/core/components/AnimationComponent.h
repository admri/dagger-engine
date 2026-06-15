#ifndef DAGGER_CORE_COMPONENTS_ANIMATION_COMPONENT_H
#define DAGGER_CORE_COMPONENTS_ANIMATION_COMPONENT_H

#include <dagger/assets/ResourceHandle.h>
#include <dagger/core/Component.h>
#include <dagger/core/Serialization.h>

#include <cstdint>
#include <vector>

namespace dagger
{

struct AnimationComponent : public Component
{
    struct Sequence
    {
        uint32_t nameHash;
        TextureHandle texture;
        int startFrame;
        int numFrames;
        float frameDuration;
        bool isLooping;
    };

    std::vector<Sequence> sequences;

    // Current state
    uint32_t currentSequenceHash = 0;
    int currentFrameIndex = 0;
    float timer = 0.0f;
    bool isPlaying = true;

    void play(uint32_t targetHash)
    {
        if (currentSequenceHash == targetHash) return;

        for (const auto& seq : sequences)
        {
            if (seq.nameHash == targetHash)
            {
                currentSequenceHash = targetHash;
                currentFrameIndex = 0;
                timer = 0.0f;
                isPlaying = true;
                return;
            }
        }
    }
};

DAGGER_SERIALIZE_COMPONENT(AnimationComponent, isPlaying)

} // namespace dagger

#endif // DAGGER_CORE_COMPONENTS_ANIMATION_COMPONENT_H
