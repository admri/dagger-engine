#ifndef DAGGER_CORE_SYSTEMS_SPRITE_RENDER_SYSTEM_H
#define DAGGER_CORE_SYSTEMS_SPRITE_RENDER_SYSTEM_H

namespace dagger
{

class Renderer;
class AssetManager;
class Scene;
struct FOVComponent;

class SpriteRenderSystem
{
public:
    void render(Renderer& renderer, AssetManager& assets, Scene& scene, float alpha);
};

} // namespace dagger

#endif // DAGGER_CORE_SYSTEMS_SPRITE_RENDER_SYSTEM_H
