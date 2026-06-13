#include <systems/PlayerInputSystem.h>

#include <components/PlayerInputComponent.h>
#include <components/WeaponComponent.h>

#include <dagger/core/Application.h>
#include <dagger/core/Entity.h>
#include <dagger/core/Scene.h>
#include <dagger/core/components/TransformComponent.h>
#include <dagger/core/components/VelocityComponent.h>
#include <dagger/input/Input.h>
#include <dagger/input/KeyCodes.h>

#include <cmath>

namespace delve
{

void PlayerInputSystem::update(dagger::Application& app, dagger::Scene& scene, dagger::Entity* camera)
{
    auto& input = app.getInput();

    auto players = scene.getEntitiesWith<PlayerInputComponent, dagger::VelocityComponent>();

    for (auto* player : players)
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
        
        // Normalizace diagonálního pohybu
        if (vel->vx != 0.0f && vel->vy != 0.0f)
        {
            float length = std::sqrt(vel->vx * vel->vx + vel->vy * vel->vy);
            vel->vx = (vel->vx / length) * pInput->speed;
            vel->vy = (vel->vy / length) * pInput->speed;
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
                
                if (camera)
                {
                    auto* cTransform = camera->getComponent<dagger::TransformComponent>();
                    if (cTransform) { 
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
    }
}

} // namespace delve
