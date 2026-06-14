#include <systems/PlayerInputSystem.h>

#include <components/PlayerInputComponent.h>
#include <components/WeaponComponent.h>
#include <scenes/PauseScene.h>
#include <types/PlayerAnim.h>

#include <dagger/core/Application.h>
#include <dagger/core/Entity.h>
#include <dagger/core/Scene.h>
#include <dagger/core/components/AnimationComponent.h>
#include <dagger/core/components/CameraComponent.h>
#include <dagger/core/components/SpriteComponent.h>
#include <dagger/core/components/TransformComponent.h>
#include <dagger/core/components/VelocityComponent.h>
#include <dagger/input/Input.h>
#include <dagger/input/KeyCodes.h>

#include <cmath>

namespace delve
{

void PlayerInputSystem::update(dagger::Application& app, dagger::Scene& scene)
{
    auto& input = app.getInput();

    scene.forEachEntityWith<PlayerInputComponent, dagger::VelocityComponent>(
        [&](dagger::Entity* player)
    {
        auto* vel = player->getComponent<dagger::VelocityComponent>();
        auto* pInput = player->getComponent<PlayerInputComponent>();

        // =========
        // Keyboard
        // =========
        vel->vx = 0.0f;
        vel->vy = 0.0f;

        if (input.isKeyDown(dagger::Key::W)) vel->vy -= pInput->speed;
        if (input.isKeyDown(dagger::Key::S)) vel->vy += pInput->speed;
        if (input.isKeyDown(dagger::Key::A)) vel->vx -= pInput->speed;
        if (input.isKeyDown(dagger::Key::D)) vel->vx += pInput->speed;
        
        // Normalize diagonal movement
        if (vel->vx != 0.0f && vel->vy != 0.0f)
        {
            float length = std::sqrt(vel->vx * vel->vx + vel->vy * vel->vy);
            vel->vx = (vel->vx / length) * pInput->speed;
            vel->vy = (vel->vy / length) * pInput->speed;
        }

        // Animation handling
        auto* anim = player->getComponent<dagger::AnimationComponent>();
        auto* sprite = player->getComponent<dagger::SpriteComponent>();

        if (anim && sprite)
        {
            // Mirror the texture
            if (vel->vx < 0.0f)
            {
                sprite->flipX = true;
            }
            else if (vel->vx > 0.0f)
            {
                sprite->flipX = false;
            }

            // Switch states
            if (vel->vx != 0.0f || vel->vy != 0.0f)
            {
                if (anim->currentSequence != PlayerAnim::Run)
                {
                    anim->currentSequence = PlayerAnim::Run;
                    anim->currentFrameIndex = 0;
                    anim->timer = 0.0f;
                }
            }
            else
            {
                if (anim->currentSequence != PlayerAnim::Idle)
                {
                    anim->currentSequence = PlayerAnim::Idle;
                    anim->currentFrameIndex = 0;
                    anim->timer = 0.0f;
                }
            }
        }

        // Pause key
        if (input.isKeyPressed(dagger::Key::P))
        {
            app.getSceneManager().push(std::make_unique<PauseScene>());
        }

        // ======
        // Mouse
        // ======
        auto* weapon = player->getComponent<WeaponComponent>();
        if (weapon)
        {
            if (input.isLeftMouseDown())
            {
                weapon->isTriggerPulled = true;
                
                int mouseX, mouseY;
                input.getMousePosition(&mouseX, &mouseY);
                
                float camX = 0.0f; 
                float camY = 0.0f;

                auto* camera = scene.getFirstEntityWith<dagger::TransformComponent, dagger::CameraComponent>();
                if (camera)
                {
                    auto* cTransform = camera->getComponent<dagger::TransformComponent>();
                    if (cTransform)
                    { 
                        camX = cTransform->x; 
                        camY = cTransform->y; 
                    }
                }
                
                weapon->targetX = mouseX + camX;
                weapon->targetY = mouseY + camY;
            }
            else
            {
                weapon->isTriggerPulled = false;
            }
        }
    });
}

} // namespace delve
