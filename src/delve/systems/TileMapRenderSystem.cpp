#include <systems/TileMapRenderSystem.h>

#include <components/FOVComponent.h>
#include <world/TileMap.h>

#include <dagger/assets/AssetManager.h>
#include <dagger/assets/ResourceHandle.h>
#include <dagger/graphics/Renderer.h>
#include <dagger/graphics/Texture.h>
#include <dagger/graphics/Viewport.h>

#include <algorithm>
#include <cmath>

namespace delve
{

void TileMapRenderSystem::render(dagger::Renderer& renderer, const TileMap& tileMap, dagger::Texture& floorTex, dagger::Texture& wallTex,
                                 const dagger::Viewport& viewport, const FOVComponent* playerFov)
{
    float tileSize = tileMap.getTileSize();

    // Frustum Culling
    int startX = static_cast<int>(std::floor(viewport.camX / tileSize));
    int startY = static_cast<int>(std::floor(viewport.camY / tileSize));
    
    int endX = static_cast<int>(std::floor((viewport.camX + viewport.windowWidth) / tileSize)) + 1;
    int endY = static_cast<int>(std::floor((viewport.camY + viewport.windowHeight) / tileSize)) + 1;

    for (int y = startY; y <= endY; ++y)
    {
        for (int x = startX; x < endX; ++x)
        {
            TileType tile = tileMap.getTile(x, y);
            if (tile == TileType::Empty) continue;

            dagger::Color tileTint = dagger::Colors::White;

            float screenX = (x * tileSize) - viewport.camX;
            float screenY = (y * tileSize) - viewport.camY;

            if (tile == TileType::Wall)
            {
                renderer.drawTexture(wallTex, { screenX, screenY, tileSize, tileSize }, nullptr, false, tileTint);
            }
            else if (tile == TileType::Floor)
            {
                renderer.drawTexture(floorTex, { screenX, screenY, tileSize, tileSize }, nullptr, false, tileTint);
            }
        }
    }
}

} // namespace delve
