#include <scenes/GameplayScene.h>

#include <components/PlayerInputComponent.h>

#include <dagger/assets/AssetManager.h>
#include <dagger/assets/Font.h>
#include <dagger/core/Application.h>
#include <dagger/core/Entity.h>
#include <dagger/core/components/CameraComponent.h>
#include <dagger/core/components/SpriteComponent.h>
#include <dagger/core/components/TextComponent.h>
#include <dagger/core/components/TransformComponent.h>
#include <dagger/core/components/VelocityComponent.h>
#include <dagger/graphics/Texture.h>
#include "GameplayScene.h"

namespace delve
{

void GameplayScene::onInit(dagger::AssetManager& assets)
{
    // Load assets
    auto playerTexture = assets.load<dagger::Texture>("player", "assets/player.png");
    auto enemyTexture = assets.load<dagger::Texture>("enemy", "assets/enemy.png");
    auto uiFont = assets.load<dagger::Font>("ui", "assets/fonts/RobotoMono-Regular.ttf", 16);

    floorTex_ = assets.load<dagger::Texture>("floor", "assets/grass4.png");
    wallTex_ = assets.load<dagger::Texture>("wall", "assets/water1.png");

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
    player_ = createEntity();

    auto& pTransform = player_->addComponent<dagger::TransformComponent>();
    pTransform.setPosition(spawnPoint.first, spawnPoint.second);

    auto& pSprite = player_->addComponent<dagger::SpriteComponent>();
    pSprite.texture = playerTexture;
    pSprite.zIndex = 10;
    /*if (auto* texPtr = assets.resolve<dagger::Texture>(playerTexture))
    {
        pSprite.width = texPtr->getWidth();
        pSprite.height = texPtr->getHeight();
    }*/
    pSprite.width = 48.0f;
    pSprite.height = 48.0f;

    player_->addComponent<dagger::VelocityComponent>();
    player_->addComponent<PlayerInputComponent>();

    // Enemy
    dagger::Entity* enemy = createEntity();

    auto& enemyTransform = enemy->addComponent<dagger::TransformComponent>();
    enemyTransform.setPosition(50.0f, 50.0f);

    auto& enemySprite = enemy->addComponent<dagger::SpriteComponent>();
    enemySprite.texture = enemyTexture;
    enemySprite.zIndex = 9;

    // FPS Display
    fpsDisplay_ = createEntity();

    auto& uiTransform = fpsDisplay_->addComponent<dagger::TransformComponent>();
    uiTransform.setPosition(20.0f, 20.0f);

    auto& textComp = fpsDisplay_->addComponent<dagger::TextComponent>();
    textComp.setFont(uiFont);
    textComp.setColor(dagger::Colors::White);
    textComp.setText("FPS: 0");
    textComp.zIndex = 100;

    renderSystem_.registerEntity(camera_);
    renderSystem_.registerEntity(player_);
    renderSystem_.registerEntity(enemy);
    renderSystem_.registerEntity(fpsDisplay_);
}

void GameplayScene::onFixedUpdate(float dt, dagger::Application& app)
{
    if (fpsDisplay_)
    {
        auto* textComp = fpsDisplay_->getComponent<dagger::TextComponent>();
        textComp->setText("FPS: " + std::to_string(app.getFPS()));
    }

    if (!player_) return;

    auto* pTransform = player_->getComponent<dagger::TransformComponent>();
    auto* pSprite = player_->getComponent<dagger::SpriteComponent>();
    auto* pVelocity = player_->getComponent<dagger::VelocityComponent>();
    auto* pInput = player_->getComponent<PlayerInputComponent>();

    if (pTransform && pSprite && pVelocity && pInput)
    {
        inputSystem_.update(app.getInput(), *pVelocity, *pInput);
        movementSystem_.update(dt, *pTransform, *pVelocity, *tileMap_, pSprite->width, pSprite->height);

        if (camera_)
        {
            auto* camTransform = camera_->getComponent<dagger::TransformComponent>();
            if (camTransform)
            {
                camTransform->prevX = camTransform->x;
                camTransform->prevY = camTransform->y;

                float screenW = static_cast<float>(app.getWindowWidth());
                float screenH = static_cast<float>(app.getWindowHeight());

                camTransform->x = pTransform->x - (screenW / 2.0f) + (pSprite->width / 2.0f);
                camTransform->y = pTransform->y - (screenH / 2.0f) + (pSprite->height / 2.0f);
            }
        }
    }
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
