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

    auto uiFont = assets.load<dagger::Font>("ui", "assets/fonts/RobotoMono-Regular.ttf", 16);
    auto playerTexture = assets.load<dagger::Texture>("player", "assets/player.png");
    auto enemyTexture = assets.load<dagger::Texture>("enemy", "assets/crawl-tiles/dc-mon/orc_knight.png");
    floorTex_ = assets.load<dagger::Texture>("floor", "assets/crawl-tiles/dc-dngn/floor/pebble_brown0.png");
    wallTex_ = assets.load<dagger::Texture>("wall", "assets/crawl-tiles/dc-dngn/wall/lair1.png");
    
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
    camera_ = createEntity();
    camera_->addComponent<dagger::TransformComponent>();
    camera_->addComponent<dagger::CameraComponent>();

    // Player
    player_ = CharacterFactory::createPlayer(*this, playerTexture, spawnPoint.first, spawnPoint.second);

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

    renderSystem_.registerEntity(camera_);
}

void GameplayScene::onFixedUpdate(float dt, dagger::Application& app)
{
    if (!player_) return;

    inputSystem_.update(app, *this, camera_);
    shootingSystem_.update(dt, app, *this);
    movementSystem_.updateAll(dt, *this, *tileMap_);
    enemyAISystem_.update(dt, *this, player_, movementSystem_, *tileMap_);
    combatSystem_.update(dt, app, *this, player_);

    uiSystem_.updateElement<FPSCounterTag>(*this, [&app]() {
        return "FPS: " + std::to_string(app.getFPS());
    });

    auto* pHealth = player_->getComponent<HealthComponent>();
    int playerHealth = pHealth ? pHealth->hp : 0;
    uiSystem_.updateElement<HealthDisplayTag>(*this, [playerHealth]() {
        return "HP: " + std::to_string(playerHealth);
    });

    cameraSystem_.update(app, camera_, player_);
}

void GameplayScene::onUpdate(float dt, dagger::Application& app)
{
}

void GameplayScene::onRender(dagger::Renderer &renderer, dagger::AssetManager &assets, float alpha)
{
    float camX = 0.0f;
    float camY = 0.0f;

    if (camera_)
    {
        auto* camTransform = camera_->getComponent<dagger::TransformComponent>();
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

    renderSystem_.render(renderer, assets, alpha);
}

} // namespace delve
