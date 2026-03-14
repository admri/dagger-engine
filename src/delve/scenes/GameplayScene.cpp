#include <scenes/GameplayScene.h>

#include <components/PlayerInputComponent.h>

#include <dagger/assets/AssetManager.h>
#include <dagger/assets/Font.h>
#include <dagger/core/Application.h>
#include <dagger/core/Entity.h>
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
    dagger::TextureHandle playerTexture = assets.load<dagger::Texture>("player", "assets/player.png");
    dagger::FontHandle uiFont = assets.load<dagger::Font>("ui", "assets/fonts/RobotoMono-Regular.ttf", 16);

    // Player
    player_ = createEntity();

    auto& pTransform = player_->addComponent<dagger::TransformComponent>();
    pTransform.x = 50.0f;
    pTransform.y = 50.0f;

    auto& pSprite = player_->addComponent<dagger::SpriteComponent>();
    pSprite.texture = playerTexture;
    pSprite.zIndex = 10;

    player_->addComponent<dagger::VelocityComponent>();
    player_->addComponent<PlayerInputComponent>();

    // FPS Display
    fpsDisplay_ = createEntity();

    auto& uiTransform = fpsDisplay_->addComponent<dagger::TransformComponent>();
    uiTransform.x = 20.0f;
    uiTransform.y = 20.0f;

    auto& textComp = fpsDisplay_->addComponent<dagger::TextComponent>();
    textComp.setFont(uiFont);
    textComp.setColor(dagger::Colors::White);
    textComp.setText("FPS: 0");
    textComp.zIndex = 100;

    renderSystem_.registerEntity(player_);
    renderSystem_.registerEntity(fpsDisplay_);
}

void GameplayScene::onFixedUpdate(float dt, dagger::Application& app)
{
    if (!player_) return;

    auto* pTransform = player_->getComponent<dagger::TransformComponent>();
    auto* pVelocity = player_->getComponent<dagger::VelocityComponent>();
    auto* pInput = player_->getComponent<PlayerInputComponent>();

    if (pTransform && pVelocity && pInput)
    {
        inputSystem_.update(app.getInput(), *pVelocity, *pInput);
        movementSystem_.update(dt, *pTransform, *pVelocity);
    }
}

void GameplayScene::onUpdate(float dt, dagger::Application& app)
{
    if (fpsDisplay_)
    {
        auto* textComp = fpsDisplay_->getComponent<dagger::TextComponent>();
        textComp->setText("FPS: " + std::to_string(app.getFPS()));
    }
}

} // namespace delve
