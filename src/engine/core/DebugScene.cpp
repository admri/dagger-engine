#include <core/DebugScene.h>

#include <assets/AssetManager.h>
#include <core/TextObject.h>

#include <string>

namespace engine
{

void DebugScene::init(AssetManager& assets)
{
    assets.loadFont("fpsDisplay", "assets/fonts/RobotoMono-Regular.ttf", 16);
    FontHandle fpsFontHandle = assets.getFontHandle("fpsDisplay");

    auto fpsDisplay = std::make_unique<TextObject>();
    fpsDisplay->setFont(fpsFontHandle);
    fpsDisplay->setColor(Colors::Black);
    fpsDisplay->setPosition(10.0f, 10.0f);
    fpsDisplay->setText("FPS: 0");

    fpsDisplay_ = fpsDisplay.get();
    addGameObject(std::move(fpsDisplay));
}

void DebugScene::updateFPS(int currentFPS)
{
    if (fpsDisplay_)
    {
        fpsDisplay_->setText("FPS: " + std::to_string(currentFPS));
    }
}

} // namespace engine
