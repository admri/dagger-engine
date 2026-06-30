#include <systems/ShootingSystem.h>

#include <components/ProjectileComponent.h>
#include <components/WeaponComponent.h>
#include <events/GameplayEvents.h>

#include <dagger/assets/AssetManager.h>
#include <dagger/audio/AudioManager.h>
#include <dagger/audio/AudioResource.h>
#include <dagger/core/Application.h>
#include <dagger/core/Scene.h>
#include <dagger/core/Entity.h>
#include <dagger/core/components/BoxColliderComponent.h>
#include <dagger/core/components/SpriteComponent.h>
#include <dagger/core/components/TransformComponent.h>
#include <dagger/core/components/VelocityComponent.h>
#include <dagger/graphics/Texture.h>
#include <dagger/input/Input.h>

#include <cmath>

namespace delve
{

void ShootingSystem::update(float dt, dagger::Application& app, dagger::Scene& scene)
{
    auto shooters = scene.getEntitiesWith<dagger::TransformComponent, dagger::SpriteComponent, WeaponComponent>();
    auto fireballPrefab = app.getAssets().get<dagger::Prefab>("fireball");

    scene.forEachEntityWith<dagger::TransformComponent, dagger::SpriteComponent, WeaponComponent>(
        [&](auto* shooter, auto& transform, auto& sprite, auto& weapon)
    {
        if (weapon.cooldown > 0.0f)
        {
            weapon.cooldown -= dt;
        }

        if (weapon.isTriggerPulled && weapon.cooldown <= 0.0f)
        {
            weapon.cooldown = weapon.fireRate;

            float startX = transform.x + (sprite.width / 2.0f);
            float startY = transform.y + (sprite.height / 2.0f);
            
            float dirX = weapon.targetX - startX;
            float dirY = weapon.targetY - startY;
            float length = std::sqrt(dirX * dirX + dirY * dirY);
            if (length > 0) { dirX /= length; dirY /= length; }

            // Spawning projectiles
            auto fireballHandle = dagger::PrefabFactory::create(fireballPrefab, scene, app.getAssets());
            auto* fireball = scene.getEntity(fireballHandle);

            auto* fTransform = fireball->getComponent<dagger::TransformComponent>();
            if (fTransform)
            {
                fTransform->setPosition(startX - 8.0f, startY - 8.0f); 
            }

            auto* fVelocity = fireball->getComponent<dagger::VelocityComponent>();
            if (fVelocity)
            {
                fVelocity->vx = dirX * weapon.projectileSpeed; 
                fVelocity->vy = dirY * weapon.projectileSpeed;
            }

            auto* fProjectile = fireball->getComponent<ProjectileComponent>();
            if (fProjectile)
            {
                fProjectile->damage = weapon.damage;
            }

            auto* shootSound = app.getAssets().resolve<dagger::Sound>(weapon.shootSound);
            if (shootSound)
            {
                app.getAudio().play(*shootSound, 0, 0.2f);
            }
        }
    });
}

} // namespace delve
