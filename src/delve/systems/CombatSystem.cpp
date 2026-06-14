#include <systems/CombatSystem.h>

#include <components/EnemyAIComponent.h>
#include <components/HealthComponent.h>
#include <components/PlayerInputComponent.h>
#include <components/ProjectileComponent.h>
#include <scenes/GameOverScene.h>

#include <dagger/core/Application.h>
#include <dagger/core/Scene.h>
#include <dagger/core/Entity.h>
#include <dagger/core/systems/CollisionSystem.h>

namespace delve
{

void CombatSystem::update(float dt, dagger::Application& app, dagger::Scene& scene, const std::vector<dagger::CollisionEvent>& collisions)
{
    auto* player = scene.getFirstEntityWith<dagger::TransformComponent, PlayerInputComponent>();
    if (!player) return;

    auto* pHealth = player->getComponent<HealthComponent>();
    if (pHealth && pHealth->invulnerabilityTimer > 0.0f)
    {
        pHealth->invulnerabilityTimer -= dt;
    }

    bool playerDied = false;

    for (const auto& collision : collisions)
    {
        dagger::Entity* entA = collision.entityA;
        dagger::Entity* entB = collision.entityB;

        // ==================
        // Player vs Enemy
        // ==================
        if (pHealth->invulnerabilityTimer <= 0.0f)
        {
            bool isPlayerA = entA->hasComponent<PlayerInputComponent>();
            bool isPlayerB = entB->hasComponent<PlayerInputComponent>();
            bool isEnemyA = entA->hasComponent<EnemyAIComponent>();
            bool isEnemyB = entB->hasComponent<EnemyAIComponent>();

            if ((isPlayerA && isEnemyB) || (isPlayerB && isEnemyA))
            {
                pHealth->hp -= 1;
                pHealth->invulnerabilityTimer = 1.0f;

                if (pHealth->hp <= 0 && !playerDied)
                {
                    app.getSceneManager().replace(std::make_unique<GameOverScene>());
                    playerDied = true;
                }
            }
        }

        // =======================
        // Projectile vs Enemy
        // =======================
        bool isProjA = entA->hasComponent<ProjectileComponent>();
        bool isProjB = entB->hasComponent<ProjectileComponent>();
        bool isEnemyA = entA->hasComponent<EnemyAIComponent>();
        bool isEnemyB = entB->hasComponent<EnemyAIComponent>();

        if ((isProjA && isEnemyB) || (isProjB && isEnemyA))
        {
            dagger::Entity* projectile = isProjA ? entA : entB;
            dagger::Entity* enemy = isEnemyA ? entA : entB;

            if (projectile->isActive() && enemy->isActive())
            {
                auto* projData = projectile->getComponent<ProjectileComponent>();
                auto* eHealth = enemy->getComponent<HealthComponent>();

                eHealth->hp -= projData->damage;
                
                if (eHealth->hp <= 0)
                {
                    enemy->setActive(false);
                }
                
                projectile->setActive(false);
            }
        }
    }
}

} // namespace delve
