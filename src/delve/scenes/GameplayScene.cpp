#include <scenes/GameplayScene.h>

#include <components/FOVComponent.h>
#include <components/HealthComponent.h>
#include <components/PlayerInputComponent.h>
#include <components/Tags.h>

#include <dagger/assets/AssetManager.h>
#include <dagger/assets/ResourceHandle.h>
#include <dagger/core/Application.h>
#include <dagger/core/Entity.h>
#include <dagger/core/LevelLoader.h>
#include <dagger/core/components/CameraComponent.h>
#include <dagger/core/components/TransformComponent.h>
#include <dagger/graphics/Texture.h>
#include <dagger/graphics/Viewport.h>
#include <dagger/input/Input.h>
#include <dagger/input/KeyCodes.h>

#include <cstdlib>
#include <ctime>

namespace delve
{

void GameplayScene::onInit(dagger::Application& app)
{
    std::srand(static_cast<unsigned int>(std::time(nullptr)));

    dagger::LevelLoader::load("levels/level_1.json", *this, app);

    // Assets
    auto& assets = app.getAssets();
    floorTex_ = assets.get<dagger::Texture>("floor");
    wallTex_  = assets.get<dagger::Texture>("wall");

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

    // Player
    auto playerPrefab = assets.get<dagger::Prefab>("player");
    auto* player = dagger::PrefabFactory::create(playerPrefab, *this, assets);

    if (player && player->hasComponent<dagger::TransformComponent>())
    {
        player->getComponent<dagger::TransformComponent>()->setPosition(spawnPoint.first, spawnPoint.second);
    }

    // Enemies
    auto enemyPrefab = assets.get<dagger::Prefab>("enemy");
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

        auto* enemy = dagger::PrefabFactory::create(enemyPrefab, *this, assets);
        if (enemy && enemy->hasComponent<dagger::TransformComponent>())
        {
            enemy->getComponent<dagger::TransformComponent>()->setPosition(spawnX, spawnY);
        }
    }
}

void GameplayScene::onFixedUpdate(float dt, dagger::Application& app)
{
    chunkManager_.update(*this);
    shootingSystem_.update(dt, app, *this);
    enemyAISystem_.update(dt, *this);
    movementSystem_.updateAll(dt, *this, *tileMap_);
    dagger::CollisionSystem::update(*this, frameCollisions_);
    combatSystem_.update(dt, app, *this, frameCollisions_);

    if (tileMap_)
    {
        fovSystem_.update(dt, *this, *tileMap_);
    }

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

    if (app.getInput().isKeyPressed(dagger::Key::F3))
    {
        renderDebugHitboxes_ = !renderDebugHitboxes_;
    }
}

void GameplayScene::onRender(dagger::Renderer &renderer, dagger::AssetManager &assets, float alpha)
{
    dagger::Viewport viewport;
    
    viewport.windowWidth = renderer.getWidth();
    viewport.windowHeight = renderer.getHeight();

    auto* camera = getFirstEntityWith<dagger::TransformComponent, dagger::CameraComponent>();
    if (camera)
    {
        auto* camTransform = camera->getComponent<dagger::TransformComponent>();
        viewport.camX = camTransform->prevX + (camTransform->x - camTransform->prevX) * alpha;
        viewport.camY = camTransform->prevY + (camTransform->y - camTransform->prevY) * alpha;
    }

    const FOVComponent* playerFov = nullptr;
    auto* player = getFirstEntityWith<FOVComponent, PlayerInputComponent>();
    if (player)
    {
        playerFov = player->getComponent<FOVComponent>();
    }

    if (tileMap_)
    {
        auto* floor = assets.resolve<dagger::Texture>(floorTex_);
        auto* wall = assets.resolve<dagger::Texture>(wallTex_);

        if (floor && wall)
        {
            tileMapRenderSystem_.render(renderer, *tileMap_, *floor, *wall, viewport, playerFov);
        }
    }

    spriteRenderSystem_.render(renderer, assets, *this, viewport, alpha);

    if (renderDebugHitboxes_)
    {
        debugRenderSystem_.render(renderer, *this, viewport);
    }

    uiRenderSystem_.render(renderer, assets, *this, alpha);
}

} // namespace delve
