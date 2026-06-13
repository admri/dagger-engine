#include <systems/MovementSystem.h>

#include <components/PlayerInputComponent.h>
#include <components/ProjectileComponent.h>
#include <world/TileMap.h>

#include <dagger/core/Scene.h>
#include <dagger/core/Entity.h>
#include <dagger/core/components/TransformComponent.h>
#include <dagger/core/components/VelocityComponent.h>
#include <dagger/core/components/SpriteComponent.h>

namespace delve
{

bool MovementSystem::update(float dt, dagger::TransformComponent& transform, dagger::VelocityComponent& velocity,
                            const TileMap& tileMap, float width, float height)
{
    bool hitWall = false;

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
        hitWall = true;
    }

    float targetY = transform.y + velocity.vy * dt;

    if (!tileMap.isAreaSolid(transform.x, targetY, width, height))
    {
        transform.y = targetY;
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
    scene.forEachEntityWith<dagger::TransformComponent, dagger::VelocityComponent, dagger::SpriteComponent, PlayerInputComponent>(
        [&](auto* player)
    {
        auto* t = player->getComponent<dagger::TransformComponent>();
        auto* v = player->getComponent<dagger::VelocityComponent>();
        auto* s = player->getComponent<dagger::SpriteComponent>();
        
        update(dt, *t, *v, map, s->width, s->height);
    });

    // Projectile movement
    scene.forEachEntityWith<dagger::TransformComponent, dagger::VelocityComponent, dagger::SpriteComponent, ProjectileComponent>(
        [&](auto* projectile)
    {
        if (!projectile->isActive()) return;

        auto* t = projectile->getComponent<dagger::TransformComponent>();
        auto* v = projectile->getComponent<dagger::VelocityComponent>();
        auto* s = projectile->getComponent<dagger::SpriteComponent>();

        bool didHit = update(dt, *t, *v, map, s->width, s->height);
        
        if (didHit)
        {
            projectile->setActive(false);
        }
    });
}

} // namespace delve
