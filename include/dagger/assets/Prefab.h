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

/// @brief Represents an entity blueprint loaded from JSON.
/// Caches both the raw JSON data and compiled builder functions for fast entity instantiation.
struct Prefab
{
    nlohmann::json data;
    std::vector<CompiledBuilderFn> compiledBuilders;

    /// @brief Loads and parses the prefab JSON from disk.
    bool load(const std::string& filepath);

    void destroy() {}
};

} // namespace dagger

#endif // DAGGER_ASSETS_PREFAB_H
