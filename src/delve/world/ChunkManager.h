#ifndef DELVE_WORLD_CHUNK_MANAGER_H
#define DELVE_WORLD_CHUNK_MANAGER_H

#include <unordered_map>
#include <vector>

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

struct Chunk
{
    ChunkPos position;
    bool isGenerated = false;
};

class ChunkManager
{
public:
    static constexpr int CHUNK_SIZE = 16;
    static constexpr float TILE_SIZE = 64.0f;

    void update(float playerWorldX, float playerWorldY);

private:
    std::unordered_map<ChunkPos, Chunk> chunks_;
    std::vector<ChunkPos> activeChunkPositions_; 

    ChunkPos worldToChunkPos(float worldX, float worldY);
};

} // namespace delve

#endif // DELVE_WORLD_CHUNK_MANAGER_H
