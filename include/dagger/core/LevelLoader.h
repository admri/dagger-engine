#ifndef DAGGER_CORE_LEVEL_LOADER_H
#define DAGGER_CORE_LEVEL_LOADER_H

#include <string>

namespace dagger
{
    
class Scene;
class Application;

class LevelLoader
{
public:
    static bool load(const std::string& filepath, Scene& scene, Application& app);
};

} // namespace dagger

#endif // DAGGER_CORE_LEVEL_LOADER_H
