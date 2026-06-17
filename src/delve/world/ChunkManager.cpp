#include <world/ChunkManager.h>

#include <components/Tags.h>

#include <dagger/core/Logger.h>
#include <dagger/core/Scene.h>
#include <dagger/core/components/TransformComponent.h>

#include <algorithm>
#include <cmath>
#include <unordered_set>

namespace delve
{

ChunkPos ChunkManager::worldToChunkPos(float worldX, float worldY)
{
    float chunkPixelSize = CHUNK_SIZE * TILE_SIZE;

    int cx = static_cast<int>(std::floor(worldX / chunkPixelSize));
    int cy = static_cast<int>(std::floor(worldY / chunkPixelSize));

    return ChunkPos{ cx, cy };
}

void ChunkManager::update(dagger::Scene& scene)
{
    std::unordered_set<ChunkPos> requiredChunks;

    scene.forEachEntityWith<dagger::TransformComponent, WorldObserverTag>(
        [&](auto* entity, auto& transform, auto& observer)
    {
        ChunkPos observerPos = worldToChunkPos(transform.x, transform.y);

        // Calculate the 3x3 grid around the player
        for (int dy = -1; dy <= 1; dy++)
        {
            for (int dx = -1; dx <= 1; dx++)
            {
                requiredChunks.insert({ observerPos.x + dx, observerPos.y + dy });
            }
        }
    });

    // Unload unused chunks
    for (const auto& oldPos : activeChunkPositions_)
    {
        if (requiredChunks.find(oldPos) == requiredChunks.end())
        {
            //dagger::Logger::info("Unloading chunk at [{},{}]", oldPos.x, oldPos.y);

            Chunk& chunkToKill = chunks_[oldPos];
            for (auto* entity : chunkToKill.activeEntities)
            {
                if (entity)
                {
                    entity->setActive(false);
                }
            }

            chunks_.erase(oldPos);
        }
    }

    // Load new chunks
    for (const auto& reqPos : requiredChunks)
    {
        if (chunks_.find(reqPos) == chunks_.end())
        {
            //dagger::Logger::info("Generating new chunk at [{},{}]", reqPos.x, reqPos.y);
            chunks_[reqPos] = {
                .position = reqPos,
                .isGenerated = true
            };
        }
    }

    activeChunkPositions_.assign(requiredChunks.begin(), requiredChunks.end());
}

} // namespace delve
