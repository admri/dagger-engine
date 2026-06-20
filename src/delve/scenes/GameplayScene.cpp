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
    srand(time(0));

    // Assets
    auto& assets = app.getAssets();
    dagger::LevelLoader::load("levels/level_1.json", *this, app);
    floorTex_ = assets.get<dagger::Texture>("floor");
    wallTex_ = assets.get<dagger::Texture>("wall");

    // World
    worldGenerator_ = std::make_unique<WorldGenerator>(assets, std::rand());
    tileMap_ = std::make_unique<TileMap>(64.0f);

    // Player
    auto playerPrefab = assets.get<dagger::Prefab>("player");
    dagger::EntityHandle playerHandle = dagger::PrefabFactory::create(playerPrefab, *this, assets);
    auto* player = getEntity(playerHandle);

    if (player && player->hasComponent<dagger::TransformComponent>())
    {
        float spawnX = tileMap_->getTileSize() / 2.0f;
        float spawnY = tileMap_->getTileSize() / 2.0f;

        player->getComponent<dagger::TransformComponent>()
            ->setPosition(spawnX, spawnY);
    }

    lightingRenderSystem_.init(app);
}

void GameplayScene::onFixedUpdate(float dt, dagger::Application& app)
{
    if (worldGenerator_)
    {
        chunkManager_.update(*this, *worldGenerator_, *tileMap_);
    }
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
    if (app.getInput().isKeyPressed(dagger::Key::F2) && player)
    {
        static dagger::BoxColliderComponent pCollider;
        if (player->hasComponent<dagger::BoxColliderComponent>())
        {
            pCollider = *player->getComponent<dagger::BoxColliderComponent>();
            player->removeComponent<dagger::BoxColliderComponent>();
        }
        else
        {
            player->addComponent<dagger::BoxColliderComponent>(pCollider);
        }
    }
}

void GameplayScene::onRender(dagger::Renderer &renderer, dagger::AssetManager &assets, float alpha)
{
    dagger::Viewport viewport{
        .windowWidth = renderer.getWidth(),
        .windowHeight = renderer.getHeight()
    };

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

    if (tileMap_)
    {
        lightingRenderSystem_.render(renderer, *this, *tileMap_, viewport);
    }

    if (renderDebugHitboxes_)
    {
        debugRenderSystem_.render(renderer, *this, viewport);
    }

    uiRenderSystem_.render(renderer, assets, *this, alpha);
}

} // namespace delve
