#ifndef DAGGER_CORE_LEVEL_LOADER_H
#define DAGGER_CORE_LEVEL_LOADER_H

#include <string>

namespace dagger
{
    
class Scene;
class Application;

/// @brief Utility class responsible for parsing level JSON files and populating Scenes.
class LevelLoader
{
public:
    /// @brief Parses a level layout and spawns all defined prefabs into the target scene.
    /// @return True if the level was loaded successfully, false on failure.
    static bool load(const std::string& filepath, Scene& scene, Application& app);
};

} // namespace dagger

#endif // DAGGER_CORE_LEVEL_LOADER_H
