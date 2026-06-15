#include <core/systems/AnimationSystem.h>

#include <assets/ResourceHandle.h>
#include <core/Entity.h>
#include <core/Scene.h>
#include <core/components/AnimationComponent.h>
#include <core/components/SpriteComponent.h>

namespace dagger
{

void AnimationSystem::update(float dt, Scene& scene)
{
    scene.forEachEntityWith<AnimationComponent, SpriteComponent>(
        [dt](auto* entity, auto& anim, auto& sprite)
    {
        if (!anim.isPlaying) return;

        anim.timer += dt;

        AnimationComponent::Sequence* activeSeq = nullptr;
        for (auto& seq : anim.sequences)
        {
            if (seq.nameHash == anim.currentSequenceHash)
            {
                activeSeq = &seq;
                break;
            }
        }

        if (!activeSeq || activeSeq->numFrames == 0) return;

        sprite.texture = activeSeq->texture;

        if (anim.timer >= activeSeq->frameDuration)
        {
            anim.timer -= activeSeq->frameDuration; 
            anim.currentFrameIndex++;

            // On animation end
            if (anim.currentFrameIndex >= activeSeq->numFrames)
            {
                if (activeSeq->isLooping)
                {
                    anim.currentFrameIndex = 0;
                }
                else
                {
                    anim.currentFrameIndex = activeSeq->numFrames - 1; // End on last frame
                    anim.isPlaying = false;
                }
            }

            // Move source rect
            sprite.srcX = (activeSeq->startFrame + anim.currentFrameIndex) * sprite.srcWidth;
        }
    });
}

} // namespace dagger
