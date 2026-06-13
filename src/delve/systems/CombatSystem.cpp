#include <systems/CombatSystem.h>

#include <components/EnemyAIComponent.h>
#include <components/HealthComponent.h>
#include <components/ProjectileComponent.h>
#include <systems/CollisionSystem.h>
#include <scenes/GameOverScene.h>

#include <dagger/core/Application.h>
#include <dagger/core/Scene.h>
#include <dagger/core/Entity.h>
#include <dagger/core/components/TransformComponent.h>
#include <dagger/core/components/SpriteComponent.h>

namespace delve {

void CombatSystem::update(float dt, dagger::Application& app, dagger::Scene& scene, dagger::Entity* player)
{
    if (!player) return;

    auto* pTransform = player->getComponent<dagger::TransformComponent>();
    auto* pSprite = player->getComponent<dagger::SpriteComponent>();

    if (!pTransform || !pSprite) return;

    auto* pHealth = player->getComponent<HealthComponent>();
    if (pHealth && pHealth->invulnerabilityTimer > 0.0f)
    {
        pHealth->invulnerabilityTimer -= dt;
    }

    // Find all active enemies in the scene
    auto enemies = scene.getEntitiesWith<dagger::TransformComponent, dagger::SpriteComponent, EnemyAIComponent, HealthComponent>();

    // ==================
    // Player vs Enemies
    // ==================
    if (pHealth->invulnerabilityTimer <= 0.0f)
    {
        for (auto* enemy : enemies)
        {
            if (!enemy->isActive()) continue;

            auto* eTransform = enemy->getComponent<dagger::TransformComponent>();
            auto* eSprite = enemy->getComponent<dagger::SpriteComponent>();

            if (CollisionSystem::checkAABB(pTransform->x, pTransform->y, pSprite->width, pSprite->height,
                                           eTransform->x, eTransform->y, eSprite->width, eSprite->height))
            {
                pHealth->hp -= 1;
                pHealth->invulnerabilityTimer = 1.0f;

                if (pHealth->hp <= 0)
                {
                    app.setScene(std::make_unique<GameOverScene>());
                    return;
                }
                break;
            }
        }
    }

    // =======================
    // Projectiles vs Enemies
    // =======================
    auto projectiles = scene.getEntitiesWith<dagger::TransformComponent, dagger::SpriteComponent, ProjectileComponent>();

    for (auto* proj : projectiles)
    {
        if (!proj->isActive()) continue;

        auto* projTransform = proj->getComponent<dagger::TransformComponent>();
        auto* projSprite = proj->getComponent<dagger::SpriteComponent>();
        auto* projData = proj->getComponent<ProjectileComponent>();

        for (auto* enemy : enemies)
        {
            if (!enemy->isActive()) continue;

            auto* eTransform = enemy->getComponent<dagger::TransformComponent>();
            auto* eSprite = enemy->getComponent<dagger::SpriteComponent>();

            if (CollisionSystem::checkAABB(projTransform->x, projTransform->y, projSprite->width, projSprite->height,
                                           eTransform->x, eTransform->y, eSprite->width, eSprite->height))
            {
                auto* eHealth = enemy->getComponent<HealthComponent>();
                eHealth->hp -= projData->damage;
                
                if (eHealth->hp <= 0)
                {
                    enemy->setActive(false);
                }
                
                proj->setActive(false);
                break;
            }
        }
    }
}

} // namespace delve
