#include <world/TileMap.h>

#include <components/FOVComponent.h>

#include <algorithm>
#include <cmath>
#include <cstdlib>

namespace delve
{

TileMap::TileMap(float tileSize) : tileSize_(tileSize) {}

void TileMap::setTile(int x, int y, TileType type)
{
    tiles_[getKey(x, y)] = type;
}

TileType TileMap::getTile(int x, int y) const
{
    auto it = tiles_.find(getKey(x, y));
    if (it != tiles_.end())
    {
        return it->second;
    }
    return TileType::Wall;
}

bool TileMap::isAreaSolid(float worldX, float worldY, float width, float height) const
{
    int leftTile = static_cast<int>(std::floor(worldX / tileSize_));
    int rightTile = static_cast<int>(std::floor((worldX + width - 0.1f) / tileSize_));
    int topTile = static_cast<int>(std::floor(worldY / tileSize_));
    int bottomTile = static_cast<int>(std::floor((worldY + height - 0.1f) / tileSize_));

    for (int y = topTile; y <= bottomTile; y++)
    {
        for (int x = leftTile; x <= rightTile; x++)
        {
            if (getTile(x, y) == TileType::Wall)
            {
                return true;
            }
        }
    }
    return false;
}

} // namespace delve
