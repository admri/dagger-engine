#ifndef DEBUG_SCENE_H
#define DEBUG_SCENE_H

#include <engine/core/Scene.h>

namespace engine
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

} // namespace engine

#endif // DEBUG_SCENE_H
