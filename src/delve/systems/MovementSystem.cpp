#include <systems/MovementSystem.h>

#include <components/EnemyAIComponent.h>
#include <components/PlayerInputComponent.h>
#include <components/ProjectileComponent.h>
#include <world/TileMap.h>

#include <dagger/core/Scene.h>
#include <dagger/core/Entity.h>
#include <dagger/core/components/BoxColliderComponent.h>
#include <dagger/core/components/TransformComponent.h>
#include <dagger/core/components/VelocityComponent.h>

namespace delve
{

bool MovementSystem::update(float dt, dagger::TransformComponent& transform, dagger::VelocityComponent& velocity,
                            const TileMap& tileMap, float width, float height, float offsetX, float offsetY)
{
    bool hitWall = false;

    transform.prevX = transform.x;
    transform.prevY = transform.y;

    float targetHitboxX = transform.x + offsetX + velocity.vx * dt;
    float currentHitboxY = transform.y + offsetY;
    
    if (!tileMap.isAreaSolid(targetHitboxX, currentHitboxY, width, height))
    {
        transform.x += velocity.vx * dt;
    }
    else
    {
        velocity.vx = 0.0f;
        hitWall = true;
    }

    float currentHitboxX = transform.x + offsetX;
    float targetHitboxY = transform.y + offsetY + velocity.vy * dt;

    if (!tileMap.isAreaSolid(currentHitboxX, targetHitboxY, width, height))
    {
        transform.y += velocity.vy * dt;
    }
    else
    {
        velocity.vy = 0.0f; 
        hitWall = true;
    }

    return hitWall;
}

void MovementSystem::updateAll(float dt, dagger::Scene& scene, TileMap& map)
{
    // Player movement
    scene.forEachEntityWith<dagger::TransformComponent, dagger::VelocityComponent, dagger::BoxColliderComponent, PlayerInputComponent>(
        [&](auto* player, auto& transform, auto& velocity, auto& collider, auto&)
    {
        update(dt, transform, velocity, map,
               collider.width, collider.height, collider.offsetX, collider.offsetY);
    });

    // Projectile movement
    scene.forEachEntityWith<dagger::TransformComponent, dagger::VelocityComponent, dagger::BoxColliderComponent, ProjectileComponent>(
        [&](auto* projectile, auto& transform, auto& velocity, auto& collider, auto&)
    {
        if (!projectile->isActive()) return;

        bool didHit = update(dt, transform, velocity, map,
                             collider.width, collider.height, collider.offsetX, collider.offsetY);
        
        if (didHit)
        {
            projectile->setActive(false);
        }
    });

    // Enemy movement
    scene.forEachEntityWith<dagger::TransformComponent, dagger::VelocityComponent, dagger::BoxColliderComponent, EnemyAIComponent>(
        [&](auto* enemy, auto& transform, auto& velocity, auto& collider, auto&)
    {
        update(dt, transform, velocity, map,
               collider.width, collider.height, collider.offsetX, collider.offsetY);
    });
}

} // namespace delve
