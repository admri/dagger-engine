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
        [dt](auto* entity)
    {
        auto* anim = entity->getComponent<AnimationComponent>();
        auto* sprite = entity->getComponent<SpriteComponent>();

        if (!anim->isPlaying) return;

        if (anim->currentSequence >= anim->sequences.size()) return;

        anim->timer += dt;

        auto& sequence = anim->sequences[anim->currentSequence];

        sprite->texture = sequence.texture;

        if (anim->timer >= sequence.frameDuration)
        {
            anim->timer -= sequence.frameDuration; 
            
            anim->currentFrameIndex++;

            // On animation end
            if (anim->currentFrameIndex >= sequence.numFrames)
            {
                if (sequence.isLooping)
                {
                    anim->currentFrameIndex = 0;
                }
                else
                {
                    anim->currentFrameIndex = sequence.numFrames - 1; // End on last frame
                    anim->isPlaying = false;
                }
            }

            // Move source rect
            sprite->srcX = (sequence.startFrame + anim->currentFrameIndex) * sprite->srcWidth;
        }
    });
}

} // namespace dagger
