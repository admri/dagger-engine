#ifndef DELVE_WORLD_TILE_MAP_H
#define DELVE_WORLD_TILE_MAP_H

#include <dagger/graphics/Renderer.h>
#include <dagger/graphics/Texture.h>

#include <utility>
#include <vector>

namespace delve
{

enum class TileType
{
    Empty = 0,
    Floor,
    Wall
};

struct Room
{
    int x, y, width, height;

    int getCenterX() const { return x + width / 2; }
    int getCenterY() const { return y + height / 2; }

    bool intersects(const Room& other) const
    {
        return (x <= other.x + other.width + 1 && x + width + 1 >= other.x &&
                y <= other.y + other.height + 1 && y + height + 1 >= other.y);
    }
};

class TileMap
{
public:
    TileMap(int width, int height, float tileSize);

    void setTile(int x, int y, TileType type);
    TileType getTile(int x, int y) const;

    bool isAreaSolid(float worldX, float worldY, float width, float height) const;
    
    std::pair<float, float> generateDungeon(int maxRooms, int minRoomSize, int maxRoomSize);

    void render(dagger::Renderer& renderer, 
                dagger::Texture& floorTex, 
                dagger::Texture& wallTex, 
                float camX, float camY);

    int getWidth() const { return width_; }
    int getHeight() const { return height_; }
    float getTileSize() const { return tileSize_; }

private:
    int width_;
    int height_;
    float tileSize_;
    
    std::vector<TileType> tiles_;

    void createHorizontalCorridor(int x1, int x2, int y);
    void createVerticalCorridor(int y1, int y2, int x);
};

} // namespace delve

#endif // DELVE_WORLD_TILE_MAP_H
