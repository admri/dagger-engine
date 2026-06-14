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

class PrefabFactory
{
public:
    static Entity* create(PrefabHandle handle, Scene& scene, AssetManager& assets);

    static void registerComponent(const std::string& name, ComponentBuilderFn builder);

    template <typename T>
    static void registerComponent(const std::string& name)
    {
        registerComponent(name, [](Entity* e, const nlohmann::json& j, AssetManager&) {
            j.get_to(e->addComponent<T>());
        });
    }

    template <typename T>
    static void registerTag(const std::string& name)
    {
        registerComponent(name, [](Entity* e, const nlohmann::json&, AssetManager&) {
            e->addComponent<T>();
        });
    }

    static void registerCoreComponents();

    static void compile(Prefab& prefab, const nlohmann::json& data);

private:
    static std::unordered_map<std::string, ComponentBuilderFn> builders_;
};

} // namespace dagger

#endif // DAGGER_CORE_FACTORIES_PREFAB_FACTORY_H
