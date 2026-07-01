#ifndef DAGGER_CORE_FACTORIES_PREFAB_FACTORY_H
#define DAGGER_CORE_FACTORIES_PREFAB_FACTORY_H

#include <dagger/assets/ResourceHandle.h>
#include <dagger/core/Entity.h>

#include <nlohmann/json_fwd.hpp>

#include <functional>
#include <string>
#include <unordered_map>
#include <vector>

namespace dagger
{

class Scene;
class AssetManager;

using ComponentBuilderFn = std::function<void(Entity*, const nlohmann::json&, AssetManager&)>;

/// @brief Factory responsible for translating JSON Prefabs into active ECS Entities.
class PrefabFactory
{
public:
    /// @brief Instantiates a new entity in the scene based on a loaded Prefab.
    /// @param handle Handle to the loaded prefab resource.
    /// @return Handle to the newly created entity in the scene.
    static EntityHandle create(PrefabHandle handle, Scene& scene, AssetManager& assets);

    /// @brief Registers a custom deserialization lambda for a specific component type.
    /// @param name The JSON key representing this component (e.g., "WeaponComponent").
    /// @param builder The function that applies the JSON data to the entity.
    static void registerComponent(const std::string& name, ComponentBuilderFn builder);

    /// @brief Automatically registers a standard POD component using nlohmann::json serialization.
    template <typename T>
    static void registerComponent(const std::string& name)
    {
        registerComponent(name, [](Entity* e, const nlohmann::json& j, AssetManager&) {
            j.get_to(e->addComponent<T>());
        });
    }

    /// @brief Registers an empty tag component (contains no data, only flags an entity).
    template <typename T>
    static void registerTag(const std::string& name)
    {
        registerComponent(name, [](Entity* e, const nlohmann::json&, AssetManager&) {
            e->addComponent<T>();
        });
    }

    /// @brief Registers all internal engine components (Transform, Sprite, etc.).
    static void registerCoreComponents();

    /// @brief Pre-compiles a prefab's JSON data into fast builder functions to speed up instantiation.
    static void compile(Prefab& prefab, const nlohmann::json& data);

private:
    static std::unordered_map<std::string, ComponentBuilderFn> builders_;
};

} // namespace dagger

#endif // DAGGER_CORE_FACTORIES_PREFAB_FACTORY_H
