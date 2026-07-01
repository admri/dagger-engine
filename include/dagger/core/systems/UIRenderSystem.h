#ifndef DAGGER_CORE_SYSTEMS_UI_RENDER_SYSTEM_H
#define DAGGER_CORE_SYSTEMS_UI_RENDER_SYSTEM_H

#include <dagger/core/components/SpriteComponent.h>
#include <dagger/core/components/TextComponent.h>

#include <vector>

namespace dagger
{

class Renderer;
class AssetManager;
class Scene;

/// @brief Renders screen-space user interface elements (sprites and text) over the game world.
/// Ignores camera/viewport transformations to keep elements pinned to the screen.
class UIRenderSystem
{
public:
    /// @brief Collects, sorts, and draws all entities flagged with UIRenderTag.
    void render(Renderer& renderer, AssetManager& assets, Scene& scene, float alpha);

private:
    struct UINode
    {
        SpriteComponent* sprite = nullptr;
        TextComponent* text = nullptr;
        float screenX = 0.0f;
        float screenY = 0.0f;

        inline int getZIndex() const
        {
            if (sprite) return sprite->zIndex;
            if (text) return text->zIndex;
            return 0;
        }
    };
    
    std::vector<UINode> nodes_;
};

} // namespace dagger

#endif // DAGGER_CORE_SYSTEMS_UI_RENDER_SYSTEM_H
