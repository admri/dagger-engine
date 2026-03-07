#ifndef DAGGER_CORE_SYSTEMS_RENDER_SYSTEM_H
#define DAGGER_CORE_SYSTEMS_RENDER_SYSTEM_H

#include <dagger/core/components/TextComponent.h>

#include <vector>

namespace dagger
{

class Renderer;
class AssetManager;

class Entity;

struct SpriteComponent;
struct TextComponent;
struct TransformComponent;

struct RenderNode
{
    TransformComponent* transform;
    SpriteComponent* sprite = nullptr;
    TextComponent* text = nullptr;

    inline int getZIndex() const
    {
        if (sprite) return sprite->zIndex;
        if (text) return text->zIndex;
        return 0;
    }
};

class RenderSystem
{
public:
    void render(Renderer& renderer, AssetManager& assets);
    void registerEntity(Entity* sprite);

private:
    std::vector<RenderNode> nodes_;
};

} // namespace dagger

#endif // DAGGER_CORE_SYSTEMS_RENDER_SYSTEM_H
