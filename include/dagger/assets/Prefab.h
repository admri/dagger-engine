#ifndef DAGGER_ASSETS_PREFAB_H
#define DAGGER_ASSETS_PREFAB_H

#include <dagger/core/Serialization.h>

#include <functional>
#include <string>
#include <vector>

namespace dagger
{

class Entity;
class AssetManager;

using CompiledBuilderFn = std::function<void(Entity*, AssetManager&)>;

struct Prefab
{
    nlohmann::json data;
    std::vector<CompiledBuilderFn> compiledBuilders;

    bool load(const std::string& filepath);

    void destroy() {}
};

} // namespace dagger

#endif // DAGGER_ASSETS_PREFAB_H
