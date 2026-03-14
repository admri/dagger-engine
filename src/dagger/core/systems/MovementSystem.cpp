#include <core/systems/MovementSystem.h>

#include <core/components/TransformComponent.h>
#include <core/components/VelocityComponent.h>

namespace dagger
{

void MovementSystem::update(float dt, TransformComponent& transform, VelocityComponent& velocity)
{
    transform.prevX = transform.x;
    transform.prevY = transform.y;

    // Apply velocity to position
    transform.x += velocity.vx * dt;
    transform.y += velocity.vy * dt;
}

} // namespace dagger
