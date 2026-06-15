#ifndef DAGGER_CORE_SCENE_H
#define DAGGER_CORE_SCENE_H

#include <dagger/assets/ResourceHandle.h>
#include <dagger/core/Entity.h>
#include <dagger/core/components/Tags.h>
#include <dagger/core/components/TextComponent.h>
#include <dagger/core/components/TransformComponent.h>
#include <dagger/graphics/Color.h>

#include <algorithm>
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
    virtual void onRender(Renderer& renderer, AssetManager& assets, float alpha = 0.0f) {}

    Entity* createEntity()
    {
        auto entity = std::make_unique<Entity>();
        Entity* ptr = entity.get();
        pendingEntities_.push_back(std::move(entity));
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

        entity->addComponent<UIRenderTag>();

        return entity;
    }
    
    template <typename... Components>
    Entity* getFirstEntityWith()
    {
        for (const auto& entity : entities_)
        {
            if (entity->isActive() && (entity->hasComponent<Components>() && ...))
            {
                return entity.get();
            }
        }
        return nullptr;
    }

    template <typename... Components>
    void getEntitiesWith(std::vector<Entity*>& outEntities)
    {
        outEntities.clear();

        for (const auto& entity : entities_)
        {
            if (entity->isActive() && (entity->hasComponent<Components>() && ...))
            {
                outEntities.push_back(entity.get());
            }
        }
    }

    template <typename... Components>
    std::vector<Entity*> getEntitiesWith()
    {
        std::vector<Entity*> result;
        getEntitiesWith<Components...>(result);
        return result;
    }

    template <typename... Components, typename Func>
    void forEachEntityWith(Func action)
    {
        for (const auto& entity : entities_)
        {
            if (entity->isActive() && (entity->hasComponent<Components>() && ...))
            {
                action(entity.get(), *(entity->getComponent<Components>())...);
            }
        }
    }

    void cleanup()
    {
        if (!pendingEntities_.empty())
        {
            for (auto& newEntity : pendingEntities_)
            {
                entities_.push_back(std::move(newEntity));
            }
            pendingEntities_.clear();
        }

        entities_.erase(std::remove_if(entities_.begin(), entities_.end(),
            [](const std::unique_ptr<Entity>& entity) {
                return !entity->isActive();
            }), entities_.end()
        );
    }

protected:
    std::vector<std::unique_ptr<Entity>> entities_;
    std::vector<std::unique_ptr<Entity>> pendingEntities_;
};

} // namespace dagger

#endif // DAGGER_CORE_SCENE_H
