#include <systems/PlayerInputSystem.h>

#include <components/PlayerInputComponent.h>

#include <dagger/core/components/VelocityComponent.h>
#include <dagger/input/Input.h>
#include <dagger/input/KeyCodes.h>

#include <cmath>

namespace delve
{

void PlayerInputSystem::update(dagger::Input& input, dagger::VelocityComponent& vel, const PlayerInputComponent& playerInput)
{
    vel.vx = 0.0f;
    vel.vy = 0.0f;

    if (input.isKeyDown(dagger::Key::W)) vel.vy -= playerInput.speed;
    if (input.isKeyDown(dagger::Key::S)) vel.vy += playerInput.speed;
    if (input.isKeyDown(dagger::Key::A)) vel.vx -= playerInput.speed;
    if (input.isKeyDown(dagger::Key::D)) vel.vx += playerInput.speed;
    
    // Diagonal movement normalization
    if (vel.vx != 0.0f && vel.vy != 0.0f)
    {
        float length = std::sqrt(vel.vx * vel.vx + vel.vy * vel.vy);
        vel.vx = (vel.vx / length) * playerInput.speed;
        vel.vy = (vel.vy / length) * playerInput.speed;
    }
}

} // namespace delve
