#ifndef DAGGER_CORE_ENTITY_H
#define DAGGER_CORE_ENTITY_H

#include <dagger/assets/ResourceHandle.h>
#include <dagger/core/Component.h>

#include <memory>
#include <typeinfo>
#include <type_traits>
#include <unordered_map>
#include <utility>
#include <vector>

namespace dagger
{

using EntityHandle = ResourceHandle<Entity>;

/// @brief Represents a unique object in the game world. 
/// Acts purely as a container for components. Logic should be handled by Systems.
class Entity
{
public:
    virtual ~Entity() = default;

    /// @brief Instantiates and attaches a new component to this entity.
    /// @tparam T The component type (must inherit from dagger::Component).
    /// @return A reference to the newly created component.
    template <typename T, typename... Args>
    T& addComponent(Args&&... args) 
    {
        static_assert(std::is_base_of<Component, T>::value, "T must inherit from dagger::Component");

        auto newComponent = std::make_unique<T>(std::forward<Args>(args)...);
        newComponent->owner = this;

        size_t typeHash = typeid(T).hash_code();
        T* rawPtr = newComponent.get();
        components_[typeHash] = std::move(newComponent);

        return *rawPtr;
    }

    /// @brief Retrieves a component of the specified type.
    /// @return Pointer to the component, or nullptr if the entity doesn't have it.
    template <typename T>
    T* getComponent() 
    {
        size_t typeHash = typeid(T).hash_code();
        auto it = components_.find(typeHash);
        if (it != components_.end()) 
        {
            return static_cast<T*>(it->second.get());
        }
        return nullptr;
    }

    /// @brief Checks if the entity currently possesses the specified component.
    template <typename T>
    bool hasComponent() const 
    {
        return components_.count(typeid(T).hash_code()) > 0;
    }

    /// @brief Removes the specified component from the entity.
    /// @return True if the component was found and removed, false otherwise.
    template <typename T>
    bool removeComponent() 
    {
        return components_.erase(typeid(T).hash_code()) > 0;
    }

    inline float getX() { return x_; }
    inline float getY() { return y_; }
    
    inline bool isActive() const { return isActive_; }
    inline void setActive(bool active) { isActive_ = active; }

private:
    float x_ = 0.0f;
    float y_ = 0.0f;
    bool isActive_ = true;

    std::unordered_map<size_t, std::unique_ptr<Component>> components_;
};

} // namespace dagger

#endif // DAGGER_CORE_ENTITY_H
