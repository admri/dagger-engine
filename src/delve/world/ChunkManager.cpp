#include <world/ChunkManager.h>

#include <dagger/core/Logger.h>

#include <algorithm>
#include <cmath>

namespace delve
{

ChunkPos ChunkManager::worldToChunkPos(float worldX, float worldY)
{
    float chunkPixelSize = CHUNK_SIZE * TILE_SIZE;

    int cx = static_cast<int>(std::floor(worldX / chunkPixelSize));
    int cy = static_cast<int>(std::floor(worldY / chunkPixelSize));

    return ChunkPos{ cx, cy };
}

void ChunkManager::update(float playerWorldX, float playerWorldY)
{
    ChunkPos playerPos = worldToChunkPos(playerWorldX, playerWorldY);

    // Calculate the 3x3 grid around the player
    std::vector<ChunkPos> requiredChunks;
    for (int dy = -1; dy <= 1; dy++)
    {
        for (int dx = -1; dx <= 1; dx++)
        {
            requiredChunks.emplace_back(ChunkPos{ playerPos.x + dx, playerPos.y + dy });
        }
    }

    // Unload unused chunks
    for (const auto& oldPos : activeChunkPositions_)
    {
        auto it = std::find(requiredChunks.begin(), requiredChunks.end(), oldPos);
        if (it == requiredChunks.end())
        {
            chunks_.erase(oldPos);
        }
    }

    // Load new chunks
    for (const auto& reqPos : requiredChunks)
    {
        if (chunks_.find(reqPos) == chunks_.end())
        {
            dagger::Logger::info("Generating new chunk at [{},{}]", reqPos.x, reqPos.y);
            chunks_[reqPos] = {
                .position = reqPos,
                .isGenerated = true
            };
        }
    }

    activeChunkPositions_ = requiredChunks;
}

} // namespace delve
