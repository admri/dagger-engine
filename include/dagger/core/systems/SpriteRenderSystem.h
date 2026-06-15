#ifndef DAGGER_CORE_SYSTEMS_SPRITE_RENDER_SYSTEM_H
#define DAGGER_CORE_SYSTEMS_SPRITE_RENDER_SYSTEM_H

#include <dagger/graphics/Color.h>

#include <vector>

namespace dagger
{

class Renderer;
class AssetManager;
class Scene;
struct Viewport;
struct SpriteComponent;

class SpriteRenderSystem
{
public:
    void render(Renderer& renderer, AssetManager& assets, Scene& scene, const Viewport& viewport, float alpha);

private:
    struct SpriteNode
    {
        SpriteComponent* sprite;
        Color tint;
        float screenX;
        float screenY;
    };
    
    std::vector<SpriteNode> nodes_;
};

} // namespace dagger

#endif // DAGGER_CORE_SYSTEMS_SPRITE_RENDER_SYSTEM_H
