#include <systems/CombatSystem.h>

#include <components/EnemyAIComponent.h>
#include <components/HealthComponent.h>
#include <components/PlayerInputComponent.h>
#include <components/ProjectileComponent.h>
#include <systems/CollisionSystem.h>
#include <scenes/GameOverScene.h>

#include <dagger/core/Application.h>
#include <dagger/core/Scene.h>
#include <dagger/core/Entity.h>
#include <dagger/core/components/TransformComponent.h>
#include <dagger/core/components/SpriteComponent.h>

namespace delve {

void CombatSystem::update(float dt, dagger::Application& app, dagger::Scene& scene)
{
    auto* player = scene.getFirstEntityWith<dagger::TransformComponent, PlayerInputComponent>();
    if (!player) return;

    auto* pTransform = player->getComponent<dagger::TransformComponent>();
    auto* pSprite = player->getComponent<dagger::SpriteComponent>();

    if (!pTransform || !pSprite) return;

    auto* pHealth = player->getComponent<HealthComponent>();
    if (pHealth && pHealth->invulnerabilityTimer > 0.0f)
    {
        pHealth->invulnerabilityTimer -= dt;
    }

    bool playerDied = false;

    // ==================
    // Player vs Enemies
    // ==================
    if (pHealth->invulnerabilityTimer <= 0.0f)
    {
        scene.forEachEntityWith<dagger::TransformComponent, dagger::SpriteComponent, EnemyAIComponent, HealthComponent>(
            [&](auto* enemy)
        {
            if (playerDied || !enemy->isActive()) return;

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
                    playerDied = true;
                }
            }
        });
    }

    if (playerDied) return;

    // =======================
    // Projectiles vs Enemies
    // =======================
    scene.forEachEntityWith<dagger::TransformComponent, dagger::SpriteComponent, ProjectileComponent>(
        [&](auto* projectile)
    {
        if (!projectile->isActive()) return;

        auto* projTransform = projectile->getComponent<dagger::TransformComponent>();
        auto* projSprite = projectile->getComponent<dagger::SpriteComponent>();
        auto* projData = projectile->getComponent<ProjectileComponent>();

        scene.forEachEntityWith<dagger::TransformComponent, dagger::SpriteComponent, EnemyAIComponent, HealthComponent>(
            [&](auto* enemy)
        {
             if (!projectile->isActive() || !enemy->isActive()) return;

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
                
                projectile->setActive(false);
            }
        });
    });
}

} // namespace delve
