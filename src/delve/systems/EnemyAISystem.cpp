#include <systems/EnemyAISystem.h>

#include <components/EnemyAIComponent.h>
#include <systems/MovementSystem.h>
#include <world/TileMap.h>

#include <dagger/core/Entity.h>
#include <dagger/core/Scene.h>
#include <dagger/core/components/TransformComponent.h>
#include <dagger/core/components/SpriteComponent.h>
#include <dagger/core/components/VelocityComponent.h>

namespace delve
{

void EnemyAISystem::update(float dt, dagger::Scene& scene, dagger::Entity* player, 
                           MovementSystem& movementSystem, TileMap& tileMap)
{
    if (!player) return;

    auto* pTransform = player->getComponent<dagger::TransformComponent>();
    if (!pTransform) return;

    auto enemies = scene.getEntitiesWith<dagger::TransformComponent, 
                                         dagger::VelocityComponent, 
                                         dagger::SpriteComponent, 
                                         EnemyAIComponent>();

    for (auto* enemy : enemies)
    {
        auto* eTransform = enemy->getComponent<dagger::TransformComponent>();
        auto* eVelocity = enemy->getComponent<dagger::VelocityComponent>();
        auto* eSprite = enemy->getComponent<dagger::SpriteComponent>();
        auto* eAI = enemy->getComponent<EnemyAIComponent>();

        float dirX = pTransform->x - eTransform->x;
        float dirY = pTransform->y - eTransform->y;
        float distance = std::sqrt(dirX * dirX + dirY * dirY);

        if (distance > 0.0f && distance < eAI->detectionRadius)
        {
            dirX /= distance;
            dirY /= distance;
            eVelocity->vx = dirX * eAI->speed;
            eVelocity->vy = dirY * eAI->speed;
        }
        else
        {
            eVelocity->vx = 0.0f;
            eVelocity->vy = 0.0f;
        }

        movementSystem.update(dt, *eTransform, *eVelocity, tileMap, eSprite->width, eSprite->height);
    }
}

} // namespace delve
