#ifndef DAGGER_CORE_SCENE_H
#define DAGGER_CORE_SCENE_H

#include <dagger/assets/ResourceHandle.h>
#include <dagger/core/Entity.h>
#include <dagger/core/EventBus.h>
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

/// @brief Represents a distinct game state (e.g., Level, Main Menu).
/// Acts as the main ECS registry, owning all entities, components, and the local EventBus.
class Scene
{
public:
    virtual ~Scene() = default;

    /// @brief Called when the scene is loaded. Initialize systems and load level data here.
    virtual void onInit(Application& app) = 0;

    /// @brief Called at a fixed interval (e.g., 60 times a second). Ideal for physics updates.
    virtual void onFixedUpdate(float dt, Application& app) {}

    /// @brief Called every frame. Ideal for input handling and rendering interpolation.
    virtual void onUpdate(float dt, Application& app) {}

    /// @brief Called during the rendering phase.
    virtual void onRender(Renderer& renderer, AssetManager& assets, float alpha = 0.0f) {}

    /// @brief Retrieves the local message bus used for inter-system communication in this scene.
    EventBus& getEventBus() { return eventBus_; }

    /// @brief Allocates a new empty entity in the scene.
    /// @return A generational handle used to safely reference the entity.
    EntityHandle createEntity()
    {
        uint32_t index = 0;
        if (!freeSlots_.empty())
        {
            index = freeSlots_.back();
            freeSlots_.pop_back();
        }
        else
        {
            index = static_cast<uint32_t>(entities_.size());
            entities_.push_back(EntitySlot{});
        }

        EntitySlot& slot = entities_[index];
        slot.entity = std::make_unique<Entity>();
        slot.generation++;
        slot.active = true;

        return EntityHandle{ index, slot.generation };
    }

    /// @brief Helper method to quickly instantiate a UI text element.
    EntityHandle createText(const std::string& text, float x, float y, 
                            FontHandle font, Color color = Colors::White)
    {
        EntityHandle handle = createEntity();
        Entity* entity = getEntity(handle);
        
        auto& tTransform = entity->addComponent<TransformComponent>();
        tTransform.setPosition(x, y);
        
        auto& tText = entity->addComponent<TextComponent>();
        tText.setFont(font);
        tText.setColor(color);
        tText.setText(text);
        tText.zIndex = 100;

        entity->addComponent<UIRenderTag>();

        return handle;
    }

    /// @brief Marks an entity for destruction. It will be safely cleaned up at the end of the frame.
    void destroyEntity(EntityHandle handle)
    {
         if (!handle.isValid() || handle.index >= entities_.size()) return;

         EntitySlot& slot = entities_[handle.index];
         if (slot.active && slot.generation == handle.generation)
         {
             slot.active = false;
             if (slot.entity) slot.entity->setActive(false); 
         }
    }

    /// @brief Resolves an entity handle into a raw pointer. 
    /// @return Pointer to the entity, or nullptr if the entity was destroyed or the handle is invalid.
    Entity* getEntity(EntityHandle handle) const
    {
        if (!handle.isValid() || handle.index >= entities_.size())
        {
            return nullptr;
        }

        const EntitySlot& slot = entities_[handle.index];
        
        if (slot.active && slot.generation == handle.generation)
        {
            return slot.entity.get();
        }

        return nullptr;
    }
    
    /// @brief Finds the first active entity that contains all the specified components.
    template <typename... Components>
    Entity* getFirstEntityWith()
    {
        for (const auto& slot : entities_)
        {
            if (slot.active && slot.entity && slot.entity->isActive() && 
               (slot.entity->hasComponent<Components>() && ...))
            {
                return slot.entity.get();
            }
        }
        return nullptr;
    }

    /// @brief Retrieves all active entities possessing the specified components (pre-allocated vector version).
    template <typename... Components>
    void getEntitiesWith(std::vector<Entity*>& outEntities)
    {
        outEntities.clear();

        for (const auto& slot : entities_)
        {
            if (slot.active && slot.entity && slot.entity->isActive() && 
               (slot.entity->hasComponent<Components>() && ...))
            {
                outEntities.push_back(slot.entity.get());
            }
        }
    }

    /// @brief Retrieves all active entities possessing the specified components (allocating version).
    template <typename... Components>
    std::vector<Entity*> getEntitiesWith()
    {
        std::vector<Entity*> result;
        getEntitiesWith<Components...>(result);
        return result;
    }

    /// @brief Iterates over all active entities matching the component signature.
    /// This is the primary method used by Systems to execute game logic.
    template <typename... Components, typename Func>
    void forEachEntityWith(Func action)
    {
        size_t count = entities_.size();

        for (size_t i = 0; i < count; ++i)
        {
            EntitySlot& slot = entities_[i];

            if (slot.active && slot.entity && slot.entity->isActive() && 
               (slot.entity->hasComponent<Components>() && ...))
            {
                action(slot.entity.get(), *(slot.entity->getComponent<Components>())...);
            }
        }
    }

    /// @brief Internal garbage collection. Cleans up entities marked for destruction.
    void cleanup()
    {
        for (uint32_t i = 0; i < entities_.size(); ++i)
        {
            EntitySlot& slot = entities_[i];
            
            if (slot.active && slot.entity && !slot.entity->isActive())
            {
                slot.active = false;
            }

            if (!slot.active && slot.entity != nullptr)
            {
                slot.entity.reset();
                freeSlots_.push_back(i);
            }
        }
    }

private:
    EventBus eventBus_;

    struct EntitySlot
    {
        std::unique_ptr<Entity> entity;
        uint32_t generation = 0;
        bool active = false;
    };

    std::vector<EntitySlot> entities_;
    std::vector<uint32_t> freeSlots_;
};

} // namespace dagger

#endif // DAGGER_CORE_SCENE_H
