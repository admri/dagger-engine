#include <systems/MovementSystem.h>

#include <world/TileMap.h>

#include <dagger/core/components/TransformComponent.h>
#include <dagger/core/components/VelocityComponent.h>

namespace delve
{

void MovementSystem::update(float dt, dagger::TransformComponent& transform, dagger::VelocityComponent& velocity,
                            const TileMap& tileMap, float width, float height)
{
    transform.prevX = transform.x;
    transform.prevY = transform.y;

    float targetX = transform.x + velocity.vx * dt;
    
    if (!tileMap.isAreaSolid(targetX, transform.y, width, height))
    {
        transform.x = targetX;
    }
    else
    {
        velocity.vx = 0.0f; 
    }

    float targetY = transform.y + velocity.vy * dt;

    if (!tileMap.isAreaSolid(transform.x, targetY, width, height))
    {
        transform.y = targetY;
    }
    else
    {
        velocity.vy = 0.0f; 
    }
}

} // namespace delve
