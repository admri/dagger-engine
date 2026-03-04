#ifndef DAGGER_CORE_DEBUG_SCENE_H
#define DAGGER_CORE_DEBUG_SCENE_H

#include <dagger/core/Scene.h>

namespace dagger
{

class AssetManager;
class TextObject;

class DebugScene : public Scene
{
public:
    void init(AssetManager& assets) override;

    void updateFPS(int currentFPS);

private:
    TextObject* fpsDisplay_ = nullptr;
};

} // namespace dagger

#endif // DAGGER_CORE_DEBUG_SCENE_H
