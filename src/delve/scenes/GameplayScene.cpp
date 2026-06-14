#include <scenes/GameplayScene.h>

#include <components/EnemyAIComponent.h>
#include <components/HealthComponent.h>
#include <components/PlayerInputComponent.h>
#include <components/ProjectileComponent.h>
#include <components/Tags.h>
#include <components/WeaponComponent.h>
#include <factories/CharacterFactory.h>
#include <factories/UIFactory.h>
#include <scenes/GameOverScene.h>
#include <systems/CollisionSystem.h>

#include <dagger/assets/AssetManager.h>
#include <dagger/assets/AssetPreloader.h>
#include <dagger/assets/Font.h>
#include <dagger/assets/ResourceHandle.h>
#include <dagger/core/Application.h>
#include <dagger/core/Entity.h>
#include <dagger/core/components/CameraComponent.h>
#include <dagger/core/components/SpriteComponent.h>
#include <dagger/core/components/TextComponent.h>
#include <dagger/core/components/TransformComponent.h>
#include <dagger/core/components/VelocityComponent.h>
#include <dagger/graphics/Texture.h>
#include <dagger/input/Input.h>

#include <cmath>
#include <ctime>

namespace delve
{

void GameplayScene::onInit(dagger::Application& app)
{
    std::srand(static_cast<unsigned int>(std::time(nullptr)));

    // Load assets
    auto& assets = app.getAssets();
    dagger::AssetPreloader::loadFromManifest("manifests/level1.json", assets);

    auto uiFont = assets.get<dagger::Font>("ui");
    auto enemyTexture = assets.get<dagger::Texture>("enemy");

    floorTex_ = assets.get<dagger::Texture>("floor");
    wallTex_ = assets.get<dagger::Texture>("wall");
    
    // UI
    UIFactory::createFPSDisplay(*this, uiFont, 20.0f, 20.0f);
    UIFactory::createHealthDisplay(*this, uiFont, 20.0f, 50.0f);

    // Map
    tileMap_ = std::make_unique<TileMap>(60, 60, 64.0f);

    auto spawnPoint = tileMap_->generateDungeon(15, 5, 10);

    for (int y = 20; y < 30; ++y)
    {
        for (int x = 20; x < 30; ++x)
        {
            tileMap_->setTile(x, y, TileType::Floor);
        }
    }

    // Camera
    auto camera = createEntity();
    camera->addComponent<dagger::TransformComponent>();
    camera->addComponent<dagger::CameraComponent>();

    // Player
    PlayerTextures playerTex;
    playerTex.idle = assets.get<dagger::Texture>("player_idle");
    playerTex.run = assets.get<dagger::Texture>("player_run");
    playerTex.attack = assets.get<dagger::Texture>("player_idle");
    CharacterFactory::createPlayer(*this, playerTex, spawnPoint.first, spawnPoint.second);

    // Enemies
    int numberOfEnemies = 20;
    for (int i = 0; i < numberOfEnemies; ++i)
    {
        int tileX = 0;
        int tileY = 0;
        bool foundValidSpawn = false;

        while (!foundValidSpawn)
        {
            tileX = std::rand() % tileMap_->getWidth();
            tileY = std::rand() % tileMap_->getHeight();

            if (tileMap_->getTile(tileX, tileY) == TileType::Floor)
            {
                foundValidSpawn = true;
            }
        }

        float spawnX = tileX * tileMap_->getTileSize();
        float spawnY = tileY * tileMap_->getTileSize();

        CharacterFactory::createEnemy(*this, enemyTexture, spawnX, spawnY);
    }
}

void GameplayScene::onFixedUpdate(float dt, dagger::Application& app)
{
    shootingSystem_.update(dt, app, *this);
    movementSystem_.updateAll(dt, *this, *tileMap_);
    enemyAISystem_.update(dt, *this, movementSystem_, *tileMap_);
    combatSystem_.update(dt, app, *this);
    cameraSystem_.update(app, *this);
}

void GameplayScene::onUpdate(float dt, dagger::Application& app)
{
    inputSystem_.update(app, *this);

    animationSystem_.update(dt, *this);

    // UI - FPS
    uiSystem_.updateElement<FPSCounterTag>(*this, [&app]() {
        return "FPS: " + std::to_string(app.getFPS());
    });

    // UI - Health
    int playerHealth = 0;
    auto* player = getFirstEntityWith<HealthComponent, PlayerInputComponent>();
    if (player)
    {
        playerHealth = player->getComponent<HealthComponent>()->hp;
    }

    uiSystem_.updateElement<HealthDisplayTag>(*this, [playerHealth]() {
        return "HP: " + std::to_string(playerHealth);
    });
}

void GameplayScene::onRender(dagger::Renderer &renderer, dagger::AssetManager &assets, float alpha)
{
    float camX = 0.0f;
    float camY = 0.0f;

    auto* camera = getFirstEntityWith<dagger::TransformComponent, dagger::CameraComponent>();
    if (camera)
    {
        auto* camTransform = camera->getComponent<dagger::TransformComponent>();
        if (camTransform)
        {
            camX = camTransform->prevX + (camTransform->x - camTransform->prevX) * alpha;
            camY = camTransform->prevY + (camTransform->y - camTransform->prevY) * alpha;
        }
    }

    if (tileMap_)
    {
        auto* floor = assets.resolve<dagger::Texture>(floorTex_);
        auto* wall = assets.resolve<dagger::Texture>(wallTex_);
        if (floor && wall)
        {
            tileMap_->render(renderer, *floor, *wall, camX, camY);
        }
    }

    renderSystem_.render(renderer, assets, *this, alpha);
}

} // namespace delve
