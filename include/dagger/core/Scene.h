#ifndef DAGGER_CORE_SCENE_H
#define DAGGER_CORE_SCENE_H

#include <dagger/assets/ResourceHandle.h>
#include <dagger/core/Entity.h>
#include <dagger/core/components/TextComponent.h>
#include <dagger/core/components/TransformComponent.h>
#include <dagger/core/systems/RenderSystem.h>
#include <dagger/graphics/Color.h>

#include <memory>
#include <vector>

namespace dagger
{

class Application;
class Renderer;
class AssetManager;
class Input;

class Scene
{
public:
    virtual ~Scene() = default;

    virtual void onInit(Application& app) = 0;
    virtual void onFixedUpdate(float dt, Application& app) {}
    virtual void onUpdate(float dt, Application& app) {}
    virtual void onRender(Renderer& renderer, AssetManager& assets, float alpha = 0.0f)
    {
        renderSystem_.render(renderer, assets, alpha);
    }

    Entity* createEntity()
    {
        auto entity = std::make_unique<Entity>();
        Entity* ptr = entity.get();
        entities_.push_back(std::move(entity));
        return ptr;
    }

    Entity* createText(const std::string& text, float x, float y, 
                       FontHandle font, Color color = Colors::White)
    {
        Entity* entity = createEntity();
        
        auto& tTransform = entity->addComponent<TransformComponent>();
        tTransform.setPosition(x, y);
        
        auto& tText = entity->addComponent<TextComponent>();
        tText.setFont(font);
        tText.setColor(color);
        tText.setText(text);
        tText.zIndex = 100;

        renderSystem_.registerEntity(entity);
        return entity;
    }

protected:
    std::vector<std::unique_ptr<Entity>> entities_;
    RenderSystem renderSystem_;
};

} // namespace dagger

#endif // DAGGER_CORE_SCENE_H
