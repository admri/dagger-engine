#include <factories/CharacterFactory.h>

#include <components/EnemyAIComponent.h>
#include <components/HealthComponent.h>
#include <components/PlayerInputComponent.h>
#include <components/WeaponComponent.h>

#include <dagger/core/Entity.h>
#include <dagger/core/Scene.h>
#include <dagger/core/components/SpriteComponent.h>
#include <dagger/core/components/TransformComponent.h>
#include <dagger/core/components/VelocityComponent.h>

namespace delve
{

dagger::Entity* CharacterFactory::createPlayer(dagger::Scene& scene, dagger::TextureHandle texture, float x, float y)
{
    auto player = scene.createEntity();

    auto& pTransform = player->addComponent<dagger::TransformComponent>();
    pTransform.setPosition(x, y);

    auto& pSprite = player->addComponent<dagger::SpriteComponent>();
    pSprite.texture = texture;
    pSprite.zIndex = 10;
    pSprite.width = 48.0f;
    pSprite.height = 48.0f;

    player->addComponent<dagger::VelocityComponent>();
    player->addComponent<PlayerInputComponent>();
    player->addComponent<WeaponComponent>();

    auto& pHealth = player->addComponent<HealthComponent>();
    pHealth.hp = 3;
    pHealth.maxHp = 3;

    return player;
}

dagger::Entity* CharacterFactory::createEnemy(dagger::Scene& scene, dagger::TextureHandle texture, float x, float y)
{
    auto enemy = scene.createEntity();

    auto& enemyTransform = enemy->addComponent<dagger::TransformComponent>();
    enemyTransform.setPosition(x, y);

    auto& enemySprite = enemy->addComponent<dagger::SpriteComponent>();
    enemySprite.texture = texture;
    enemySprite.zIndex = 9;
    enemySprite.width = 48.0f;
    enemySprite.height = 48.0f;

    enemy->addComponent<dagger::VelocityComponent>();
    enemy->addComponent<EnemyAIComponent>();

    auto& eHealth = enemy->addComponent<HealthComponent>();
    eHealth.hp = 2;
    
    return enemy;
}

} // namespace delve
