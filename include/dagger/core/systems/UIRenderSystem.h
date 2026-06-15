#ifndef DAGGER_CORE_SYSTEMS_UI_RENDER_SYSTEM_H
#define DAGGER_CORE_SYSTEMS_UI_RENDER_SYSTEM_H

namespace dagger
{

class Renderer;
class AssetManager;
class Scene;

class UIRenderSystem
{
public:
    void render(Renderer& renderer, AssetManager& assets, Scene& scene, float alpha);
};

} // namespace dagger

#endif // DAGGER_CORE_SYSTEMS_UI_RENDER_SYSTEM_H
