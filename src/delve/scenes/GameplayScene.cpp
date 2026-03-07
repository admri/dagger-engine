#include <scenes/GameplayScene.h>

#include <dagger/assets/AssetManager.h>
#include <dagger/assets/Font.h>
#include <dagger/core/Application.h>
#include <dagger/core/Entity.h>
#include <dagger/core/components/SpriteComponent.h>
#include <dagger/core/components/TextComponent.h>
#include <dagger/core/components/TransformComponent.h>
#include <dagger/graphics/Texture.h>
#include <dagger/input/Input.h>
#include <dagger/input/KeyCodes.h>

namespace delve
{

void GameplayScene::onInit(dagger::AssetManager& assets)
{
    // Load assets
    dagger::TextureHandle playerTexture = assets.load<dagger::Texture>("player", "assets/player.png");
    dagger::FontHandle uiFont = assets.load<dagger::Font>("ui", "assets/fonts/RobotoMono-Regular.ttf", 16);

    // Player
    dagger::Entity* player_ = createEntity();

    auto& pTransform = player_->addComponent<dagger::TransformComponent>();
    pTransform.x = 50.0f;
    pTransform.y = 50.0f;

    auto& pSprite = player_->addComponent<dagger::SpriteComponent>();
    pSprite.texture = playerTexture;
    pSprite.zIndex = 10;

    renderSystem_.registerEntity(player_);
    playerTransform_ = &pTransform;

    // FPS Display
    dagger::Entity* fpsDisplay_ = createEntity();

    auto& uiTransform = fpsDisplay_->addComponent<dagger::TransformComponent>();
    uiTransform.x = 20.0f;
    uiTransform.y = 20.0f;

    auto& textComp = fpsDisplay_->addComponent<dagger::TextComponent>();
    textComp.setFont(uiFont);
    textComp.setColor(dagger::Colors::White);
    textComp.setText("FPS: 0");
    textComp.zIndex = 100;

    renderSystem_.registerEntity(fpsDisplay_);
    fpsTextComp_ = &textComp;
}

void GameplayScene::onFixedUpdate(float dt, dagger::Application& app)
{
    dagger::Input& input = app.getInput();

    if (playerTransform_)
    {
        playerTransform_->prevX = playerTransform_->x;
        playerTransform_->prevY = playerTransform_->y;

        float& x = playerTransform_->x;
        float& y = playerTransform_->y;
        float speed = 300.0f;
        
        if (input.isKeyDown(dagger::Key::W)) y -= speed * dt;
        if (input.isKeyDown(dagger::Key::S)) y += speed * dt;
        if (input.isKeyDown(dagger::Key::A)) x -= speed * dt;
        if (input.isKeyDown(dagger::Key::D)) x += speed * dt;
    }
}

void GameplayScene::onUpdate(float dt, dagger::Application& app)
{
    if (fpsTextComp_)
    {
        fpsTextComp_->setText("FPS: " + std::to_string(app.getFPS()));
    }
}

} // namespace delve
