#ifndef DAGGER_CORE_SYSTEMS_RENDER_SYSTEM_H
#define DAGGER_CORE_SYSTEMS_RENDER_SYSTEM_H

namespace dagger
{

class Renderer;
class AssetManager;
class Scene;

class RenderSystem
{
public:
    void render(Renderer& renderer, AssetManager& assets, Scene& scene, float alpha = 0.0f);
};

} // namespace dagger

#endif // DAGGER_CORE_SYSTEMS_RENDER_SYSTEM_H
