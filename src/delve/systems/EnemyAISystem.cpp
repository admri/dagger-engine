#include <systems/EnemyAISystem.h>

#include <components/EnemyAIComponent.h>
#include <components/PlayerInputComponent.h>

#include <dagger/core/Entity.h>
#include <dagger/core/Scene.h>
#include <dagger/core/components/AnimationComponent.h>
#include <dagger/core/components/BoxColliderComponent.h>
#include <dagger/core/components/SpriteComponent.h>
#include <dagger/core/components/TransformComponent.h>
#include <dagger/core/components/VelocityComponent.h>
#include <dagger/utils/StringHash.h>

#include <cmath>

using namespace dagger::literals;

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
        [&](dagger::Entity* enemy, auto& transform, auto& velocity, auto& ai, auto& collider)
    {
        float enemyCenterX = transform.x + collider.offsetX + (collider.width / 2.0f);
        float enemyCenterY = transform.y + collider.offsetY + (collider.height / 2.0f);

        float dirX = playerCenterX - enemyCenterX;
        float dirY = playerCenterY - enemyCenterY;
        float distance = std::sqrt(dirX * dirX + dirY * dirY);

        // Movement logic
        if (distance > 0.0f && distance < ai.detectionRadius)
        {
            dirX /= distance;
            dirY /= distance;
            velocity.vx = dirX * ai.speed;
            velocity.vy = dirY * ai.speed;
        }
        else
        {
            velocity.vx = 0.0f;
            velocity.vy = 0.0f;
        }

        // Visual logic
        auto* anim = enemy->getComponent<dagger::AnimationComponent>();
        auto* sprite = enemy->getComponent<dagger::SpriteComponent>();

        if (anim && sprite)
        {
            // Flip texture
            if (velocity.vx < 0.0f)
            {
                sprite->flipX = true;
            }
            else if (velocity.vx > 0.0f)
            {
                sprite->flipX = false;
            }

            // Change animation state
            if (velocity.vx != 0.0f || velocity.vy != 0.0f)
            {
                anim->play("run"_hash);
            }
            else
            {
                anim->play("idle"_hash);
            }
        }
    });
}

} // namespace delve
