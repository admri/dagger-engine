#include <systems/ShootingSystem.h>

#include <components/ProjectileComponent.h>
#include <components/WeaponComponent.h>

#include <dagger/assets/AssetManager.h>
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
            auto* fireball = scene.createEntity();

            auto& fTransform = fireball->addComponent<dagger::TransformComponent>();
            fTransform.setPosition(startX - 8.0f, startY - 8.0f); 

            auto& fSprite = fireball->addComponent<dagger::SpriteComponent>();
            fSprite.texture = app.getAssets().get<dagger::Texture>("fireball"); 
            fSprite.width = 16.0f; fSprite.height = 16.0f; fSprite.zIndex = 5;

            auto& fCollider = fireball->addComponent<dagger::BoxColliderComponent>();
            fCollider.width = 16.0f;
            fCollider.height = 16.0f;

            auto& fVelocity = fireball->addComponent<dagger::VelocityComponent>();
            fVelocity.vx = dirX * weapon.projectileSpeed; 
            fVelocity.vy = dirY * weapon.projectileSpeed;
            
            auto& fProjectile = fireball->addComponent<ProjectileComponent>();
            fProjectile.damage = weapon.damage;
        }
    });
}

} // namespace delve
