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

/// @brief Handles rendering, frustum culling, and z-sorting of all world-space sprites.
class SpriteRenderSystem
{
public:
    /// @brief Collects, sorts, and issues draw calls for all visible sprite components.
    /// @param alpha Interpolation factor between the previous and current fixed physics steps.
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
