#ifndef DELVE_SYSTEMS_LIGHTING_RENDER_SYSTEM_H
#define DELVE_SYSTEMS_LIGHTING_RENDER_SYSTEM_H

#include <dagger/graphics/Color.h>
#include <dagger/graphics/Rect.h>
#include <dagger/graphics/Renderer.h>
#include <dagger/graphics/Texture.h>

namespace dagger
{
    class Application;
    class Scene;
    struct Viewport;
}

namespace delve
{

class TileMap;

class LightingRenderSystem
{
public:
    struct alignas(16) LightUniforms 
    {
        float lightX;
        float lightY;
        float lightRadius;
        float aspectRatio;
    };

    void init(dagger::Application& app);
    void render(dagger::Renderer& renderer, dagger::Scene& scene, const TileMap& tileMap, const dagger::Viewport& viewport);

private:
    dagger::Texture occlusionTarget_;
    dagger::Renderer::ShaderData lightingShader_;
};

} // namespace delve

#endif // DELVE_SYSTEMS_LIGHTING_RENDER_SYSTEM_H
