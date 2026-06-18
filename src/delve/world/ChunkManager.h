#ifndef DELVE_WORLD_CHUNK_MANAGER_H
#define DELVE_WORLD_CHUNK_MANAGER_H

#include <dagger/core/Entity.h>

#include <unordered_map>
#include <vector>

namespace dagger
{
    class Scene;
}

namespace delve
{

struct ChunkPos
{
    int x;
    int y;

    bool operator==(const ChunkPos& other) const {
        return x == other.x && y == other.y;
    }
};

} // namespace delve

namespace std
{
    // Implement the std::hash function for ChunkPos
    // to allow for unordered map with ChunkPos as keys
    template <>
    struct hash<delve::ChunkPos>
    {
        size_t operator()(const delve::ChunkPos& pos) const
        {
            return std::hash<int>()(pos.x) ^ (std::hash<int>()(pos.y) << 1);
        }
    };
}

namespace delve
{

class WorldGenerator;
class TileMap;

struct Chunk
{
    ChunkPos position;
    bool isGenerated = false;
    std::vector<dagger::EntityHandle> activeEntities;
};

class ChunkManager
{
public:
    static constexpr int CHUNK_SIZE = 16;
    static constexpr float TILE_SIZE = 64.0f;

    void update(dagger::Scene& scene, WorldGenerator& generator, TileMap& tileMap);

private:
    std::unordered_map<ChunkPos, Chunk> chunks_;
    std::vector<ChunkPos> activeChunkPositions_; 

    ChunkPos worldToChunkPos(float worldX, float worldY);
};

} // namespace delve

#endif // DELVE_WORLD_CHUNK_MANAGER_H
