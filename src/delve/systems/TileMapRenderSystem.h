#ifndef DELVE_SYSTEMS_TILE_MAP_RENDER_SYSTEM_H
#define DELVE_SYSTEMS_TILE_MAP_RENDER_SYSTEM_H

namespace dagger
{
    class Renderer;
    class AssetManager;
    class Texture;
    struct Viewport;
}

namespace delve
{

class TileMap;
struct FOVComponent;

class TileMapRenderSystem
{
public:
    void render(dagger::Renderer& renderer, const TileMap& tileMap, dagger::Texture& floorTex, dagger::Texture& wallTex,
                const dagger::Viewport& viewport, const FOVComponent* playerFov);
};

} // namespace delve

#endif // DELVE_SYSTEMS_TILE_MAP_RENDER_SYSTEM_H
