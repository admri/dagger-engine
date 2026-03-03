#ifndef DEBUG_SCENE_H
#define DEBUG_SCENE_H

#include <core/Scene.h>

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

#endif // DEBUG_SCENE_H
