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
    struct AnimationSequence
    {
        TextureHandle texture;
        int startFrame;
        int numFrames;
        float frameDuration;
        bool isLooping;
    };

    std::vector<AnimationSequence> sequences;

    // Current state
    uint32_t currentSequence = 0;
    int currentFrameIndex = 0;
    float timer = 0.0f;
    bool isPlaying = false;
};

DAGGER_SERIALIZE_COMPONENT(AnimationComponent, currentSequence, isPlaying)

} // namespace dagger

#endif // DAGGER_CORE_COMPONENTS_ANIMATION_COMPONENT_H
