#include <factories/CharacterFactory.h>

#include <components/EnemyAIComponent.h>
#include <components/HealthComponent.h>
#include <components/PlayerInputComponent.h>
#include <components/WeaponComponent.h>
#include <types/PlayerAnim.h>

#include <dagger/core/Entity.h>
#include <dagger/core/Scene.h>
#include <dagger/core/components/AnimationComponent.h>
#include <dagger/core/components/CameraTargetComponent.h>
#include <dagger/core/components/SpriteComponent.h>
#include <dagger/core/components/TransformComponent.h>
#include <dagger/core/components/VelocityComponent.h>

namespace delve
{

dagger::Entity* CharacterFactory::createPlayer(dagger::Scene& scene, const PlayerTextures& textures, float x, float y)
{
    auto player = scene.createEntity();

    auto& pTransform = player->addComponent<dagger::TransformComponent>();
    pTransform.setPosition(x, y);

    auto& sprite = player->addComponent<dagger::SpriteComponent>();
    sprite.texture = textures.idle;
    sprite.zIndex = 10;

    sprite.hasSrcRect = true;
    sprite.srcX = 0.0f;
    sprite.srcY = 0.0f;
    sprite.srcWidth = 16.0f;
    sprite.srcHeight = 28.0f;  

    float scale = 2.0f; 
    sprite.width = 16.0f * scale;
    sprite.height = 28.0f * scale;

    auto& anim = player->addComponent<dagger::AnimationComponent>();
    anim.sequences.resize(PlayerAnim::Count);
    anim.sequences[PlayerAnim::Idle]   = { textures.idle, 0, 4, 0.2f, true };
    anim.sequences[PlayerAnim::Run]    = { textures.run, 0, 4, 0.1f, true };
    anim.sequences[PlayerAnim::Attack] = { textures.attack, 0, 2, 0.05f, false };
    anim.currentSequence = PlayerAnim::Idle;
    anim.isPlaying = true;

    player->addComponent<dagger::VelocityComponent>();
    player->addComponent<PlayerInputComponent>();
    player->addComponent<WeaponComponent>();
    player->addComponent<dagger::CameraTargetComponent>();

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
