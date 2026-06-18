#ifndef DELVE_WORLD_TILE_MAP_H
#define DELVE_WORLD_TILE_MAP_H

#include <cstdint>
#include <unordered_map>

namespace delve
{

struct FOVComponent;

enum class TileType
{
    Empty = 0,
    Floor,
    Wall
};

class TileMap
{
public:
    TileMap(float tileSize);

    void setTile(int x, int y, TileType type);
    TileType getTile(int x, int y) const;
    bool isAreaSolid(float worldX, float worldY, float width, float height) const;

    float getTileSize() const { return tileSize_; }
    static uint64_t getKey(int x, int y)
    {
        return (static_cast<uint64_t>(static_cast<uint32_t>(x)) << 32) | static_cast<uint32_t>(y);
    }

private:
    float tileSize_;    
    std::unordered_map<uint64_t, TileType> tiles_;
};

} // namespace delve

#endif // DELVE_WORLD_TILE_MAP_H
