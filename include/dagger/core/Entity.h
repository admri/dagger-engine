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

class Entity
{
public:
    virtual ~Entity() = default;

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

    template <typename T>
    bool hasComponent() const 
    {
        return components_.count(typeid(T).hash_code()) > 0;
    }

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
