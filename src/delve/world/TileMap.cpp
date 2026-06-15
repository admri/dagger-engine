#include <components/FOVComponent.h>
#include <world/TileMap.h>

#include <algorithm>
#include <cstdlib>

namespace delve
{

TileMap::TileMap(int width, int height, float tileSize)
    : width_(width), height_(height), tileSize_(tileSize)
{
    tiles_.resize(width_ * height_, TileType::Wall);
}

void TileMap::setTile(int x, int y, TileType type)
{
    if (x >= 0 && x < width_ && y >= 0 && y < height_)
    {
        tiles_[y * width_ + x] = type;
    }
}

TileType TileMap::getTile(int x, int y) const
{
    if (x >= 0 && x < width_ && y >= 0 && y < height_)
    {
        return tiles_[y * width_ + x];
    }
    
    return TileType::Wall; 
}

bool TileMap::isAreaSolid(float worldX, float worldY, float width, float height) const
{
    int leftTile = static_cast<int>(worldX / tileSize_);
    int rightTile = static_cast<int>((worldX + width - 0.1f) / tileSize_);
    int topTile = static_cast<int>(worldY / tileSize_);
    int bottomTile = static_cast<int>((worldY + height - 0.1f) / tileSize_);

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

void TileMap::createHorizontalCorridor(int x1, int x2, int y)
{
    for (int x = std::min(x1, x2); x <= std::max(x1, x2); ++x)
    {
        setTile(x, y, TileType::Floor);
    }
}

void TileMap::createVerticalCorridor(int y1, int y2, int x)
{
    for (int y = std::min(y1, y2); y <= std::max(y1, y2); ++y)
    {
        setTile(x, y, TileType::Floor);
    }
}

std::pair<float, float> TileMap::generateDungeon(int maxRooms, int minRoomSize, int maxRoomSize)
{
    std::fill(tiles_.begin(), tiles_.end(), TileType::Wall);
    
    std::vector<Room> rooms;

    for (int i = 0; i < maxRooms; ++i)
    {
        int w = minRoomSize + std::rand() % (maxRoomSize - minRoomSize + 1);
        int h = minRoomSize + std::rand() % (maxRoomSize - minRoomSize + 1);
        int x = 2 + std::rand() % (width_ - w - 4);
        int y = 2 + std::rand() % (height_ - h - 4);

        Room newRoom{x, y, w, h};

        bool failed = false;
        for (const auto& otherRoom : rooms)
        {
            if (newRoom.intersects(otherRoom))
            {
                failed = true;
                break;
            }
        }

        if (!failed)
        {
            for (int ry = newRoom.y; ry < newRoom.y + newRoom.height; ++ry)
            {
                for (int rx = newRoom.x; rx < newRoom.x + newRoom.width; ++rx)
                {
                    setTile(rx, ry, TileType::Floor);
                }
            }
            rooms.push_back(newRoom);
        }
    }

    int wormsPerRoom = 5; 
    int wormLength = 200; 

    for (const auto& room : rooms)
    {
        for (int w = 0; w < wormsPerRoom; ++w)
        {
            int cx = room.getCenterX();
            int cy = room.getCenterY();

            for (int step = 0; step < wormLength; ++step)
            {
                setTile(cx, cy, TileType::Floor);

                int dir = std::rand() % 4;
                if (dir == 0) cx++;
                else if (dir == 1) cx--;
                else if (dir == 2) cy++;
                else if (dir == 3) cy--;

                cx = std::clamp(cx, 1, width_ - 2);
                cy = std::clamp(cy, 1, height_ - 2);
            }
        }
    }

    if (!rooms.empty())
    {
        return { rooms[0].getCenterX() * tileSize_, rooms[0].getCenterY() * tileSize_ };
    }
    
    return { tileSize_, tileSize_ };
}

void TileMap::render(dagger::Renderer& renderer, dagger::Texture& floorTex, dagger::Texture& wallTex, float camX, float camY, const FOVComponent* playerFov)
{
    for (int y = 0; y < height_; ++y)
    {
        for (int x = 0; x < width_; ++x)
        {
            TileType tile = getTile(x, y);
            if (tile == TileType::Empty) continue;

            dagger::Color tileTint = dagger::Colors::White;

            if (playerFov)
            {
                uint64_t tileKey = (static_cast<uint64_t>(x) << 32) | static_cast<uint32_t>(y);
                
                bool isVisible = playerFov->visibleTiles.count(tileKey) > 0;
                bool isExplored = playerFov->exploredTiles.count(tileKey) > 0;

                if (!isVisible && !isExplored)
                {
                    continue; 
                }
                else if (!isVisible && isExplored)
                {
                    tileTint = dagger::Color(140, 140, 140, 255); 
                }
            }

            float screenX = (x * tileSize_) - camX;
            float screenY = (y * tileSize_) - camY;

            if (tile == TileType::Wall)
            {
                renderer.drawTexture(wallTex, { screenX, screenY, tileSize_, tileSize_ }, nullptr, false, tileTint);
            }
            else if (tile == TileType::Floor)
            {
                renderer.drawTexture(floorTex, { screenX, screenY, tileSize_, tileSize_ }, nullptr, false, tileTint);
            }
        }
    }
}

} // namespace delve
