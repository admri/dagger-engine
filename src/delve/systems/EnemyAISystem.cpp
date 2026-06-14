#include <systems/EnemyAISystem.h>

#include <components/EnemyAIComponent.h>
#include <components/PlayerInputComponent.h>

#include <dagger/core/Entity.h>
#include <dagger/core/Scene.h>
#include <dagger/core/components/BoxColliderComponent.h>
#include <dagger/core/components/TransformComponent.h>
#include <dagger/core/components/VelocityComponent.h>

#include <cmath>

namespace delve
{

void EnemyAISystem::update(float dt, dagger::Scene& scene)
{
    auto* player = scene.getFirstEntityWith<dagger::TransformComponent, PlayerInputComponent, dagger::BoxColliderComponent>();
    if (!player) return;

    auto* pTransform = player->getComponent<dagger::TransformComponent>();
    auto* pCollider = player->getComponent<dagger::BoxColliderComponent>();

    float playerCenterX = pTransform->x + pCollider->offsetX + (pCollider->width / 2.0f);
    float playerCenterY = pTransform->y + pCollider->offsetY + (pCollider->height / 2.0f);

    scene.forEachEntityWith<dagger::TransformComponent, dagger::VelocityComponent, EnemyAIComponent, dagger::BoxColliderComponent>(
        [&](auto* enemy)
    {
        auto* eTransform = enemy->getComponent<dagger::TransformComponent>();
        auto* eVelocity = enemy->getComponent<dagger::VelocityComponent>();
        auto* eAI = enemy->getComponent<EnemyAIComponent>();
        auto* eCollider = enemy->getComponent<dagger::BoxColliderComponent>();

        float enemyCenterX = eTransform->x + eCollider->offsetX + (eCollider->width / 2.0f);
        float enemyCenterY = eTransform->y + eCollider->offsetY + (eCollider->height / 2.0f);

        float dirX = playerCenterX - enemyCenterX;
        float dirY = playerCenterY - enemyCenterY;
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
    });
}

} // namespace delve
