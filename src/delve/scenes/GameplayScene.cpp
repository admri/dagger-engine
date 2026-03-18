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

namespace delve
{

void GameplayScene::onInit(dagger::AssetManager& assets)
{
    // Load assets
    auto playerTexture = assets.load<dagger::Texture>("player", "assets/player.png");
    auto enemyTexture = assets.load<dagger::Texture>("enemy", "assets/enemy.png");
    auto uiFont = assets.load<dagger::Font>("ui", "assets/fonts/RobotoMono-Regular.ttf", 16);

    // Camera
    camera_ = createEntity();
    camera_->addComponent<dagger::TransformComponent>();
    camera_->addComponent<dagger::CameraComponent>();

    // Player
    player_ = createEntity();

    auto& pTransform = player_->addComponent<dagger::TransformComponent>();
    pTransform.setPosition(100.0f, 100.0f);

    auto& pSprite = player_->addComponent<dagger::SpriteComponent>();
    pSprite.texture = playerTexture;
    pSprite.zIndex = 10;
    if (auto* texPtr = assets.resolve<dagger::Texture>(playerTexture))
    {
        pSprite.width = texPtr->getWidth();
        pSprite.height = texPtr->getHeight();
    }

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
        movementSystem_.update(dt, *pTransform, *pVelocity);

        if (camera_)
        {
            auto* camTransform = camera_->getComponent<dagger::TransformComponent>();
            if (camTransform)
            {
                camTransform->setPosition(
                    pTransform->x - (800.0f / 2.0f) + (pSprite->width / 2.0f),
                    pTransform->y - (600.0f / 2.0f)+ (pSprite->width / 2.0f)
                );
            }
        }
    }
}

void GameplayScene::onUpdate(float dt, dagger::Application& app)
{
}

} // namespace delve
