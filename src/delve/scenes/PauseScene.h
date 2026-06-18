#ifndef DELVE_SCENES_PAUSE_SCENE_H
#define DELVE_SCENES_PAUSE_SCENE_H

#include <scenes/GameplayScene.h>

#include <dagger/assets/AssetManager.h>
#include <dagger/core/Application.h>
#include <dagger/core/Scene.h>
#include <dagger/core/components/TransformComponent.h>
#include <dagger/core/systems/UIRenderSystem.h>
#include <dagger/graphics/Renderer.h>
#include <dagger/input/Input.h>
#include <dagger/input/KeyCodes.h>

namespace delve
{

class PauseScene : public dagger::Scene
{
public:
    void onInit(dagger::Application& app) override
    {
        auto uiFont = app.getAssets().load<dagger::Font>("ui", "fonts/RobotoMono-Regular.ttf", 24);

        auto titleHandle = createEntity();
        auto* title = getEntity(titleHandle);
        auto& tTransform = title->addComponent<dagger::TransformComponent>();
        tTransform.setPosition(250.0f, 250.0f);
        
        auto& tText = title->addComponent<dagger::TextComponent>();
        tText.setFont(uiFont);
        tText.setColor(dagger::Colors::White);
        tText.setText("Game Paused\nPress P to unpause");
        tText.zIndex = 100;
    }

    void onUpdate(float dt, dagger::Application& app) override
    {
        if (app.getInput().isKeyPressed(dagger::Key::P))
        {
            app.getSceneManager().pop();
        }
    }

    void onRender(dagger::Renderer& renderer, dagger::AssetManager& assets, float alpha = 0.0f) override
    {
        uiRenderSystem_.render(renderer, assets, *this, alpha);
    }

private:
    dagger::UIRenderSystem uiRenderSystem_;
};

} // namespace delve

#endif // DELVE_SCENES_PAUSE_SCENE_H
