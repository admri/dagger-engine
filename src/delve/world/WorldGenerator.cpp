#include <world/WorldGenerator.h>

#include <world/ChunkManager.h>
#include <world/TileMap.h>

#include <dagger/assets/AssetManager.h>
#include <dagger/assets/PrefabFactory.h>
#include <dagger/assets/ResourceHandle.h>
#include <dagger/core/Scene.h>
#include <dagger/core/components/TransformComponent.h>
#include <dagger/math/PerlinNoise.h>

#include <random>

namespace delve
{

WorldGenerator::WorldGenerator(dagger::AssetManager& assets, uint64_t globalWorldSeed)
    : assets_(assets), globalSeed_(globalWorldSeed)
{
}

void WorldGenerator::generateChunk(Chunk& chunk, dagger::Scene& scene, TileMap& tileMap)
{
    uint64_t positionHash = (static_cast<uint64_t>(chunk.position.x) << 32) | static_cast<uint32_t>(chunk.position.y);
    uint64_t chunkSeed = positionHash ^ globalSeed_;
    std::mt19937_64 rng(chunkSeed);

    std::uniform_int_distribution<int> enemyCountDist(0, 4);

    // Map generation setup
    float startTileX = chunk.position.x * ChunkManager::CHUNK_SIZE;
    float startTileY = chunk.position.y * ChunkManager::CHUNK_SIZE;

    dagger::math::PerlinNoise noiseGenerator(globalSeed_);

    float scale = 0.05f;       // Zoom level (smaller = larger cave structures)
    float tunnelWidth = 0.06f; // Threshold for carving out floors vs walls

    for (int y = 0; y < ChunkManager::CHUNK_SIZE; ++y)
    {
        for (int x = 0; x < ChunkManager::CHUNK_SIZE; ++x)
        {
            int globalTileX = startTileX + x;
            int globalTileY = startTileY + y;

            // Generate fractal noise
            float noiseValue = noiseGenerator.fractal(globalTileX, globalTileY, 2, 0.5f, scale);

            // Carve tunnels (values close to 0.5 become floors, rest becomes walls)
            bool isTunnel = std::abs(noiseValue - 0.5f) < tunnelWidth;
            TileType type = isTunnel ? TileType::Floor : TileType::Wall;

            // Safe zone around [0,0] for player spawning
            if (globalTileX >= -2 && globalTileX <= 2 && globalTileY >= -2 && globalTileY <= 2)
            {
                type = TileType::Floor;
            }

            tileMap.setTile(globalTileX, globalTileY, type);
        }
    }

    // Enemy spawning
    float chunkPixelSize = ChunkManager::CHUNK_SIZE * ChunkManager::TILE_SIZE;
    float startPixelX = chunk.position.x * chunkPixelSize;
    float startPixelY = chunk.position.y * chunkPixelSize;

    std::uniform_real_distribution<float> posDist(0.0f, chunkPixelSize);

    int enemiesToSpawn = enemyCountDist(rng);
    auto enemyPrefab = assets_.get<dagger::Prefab>("enemy");

    for (int i = 0; i < enemiesToSpawn; ++i)
    {
        float randomX = startPixelX + posDist(rng);
        float randomY = startPixelY + posDist(rng);

        dagger::ResourceHandle<dagger::Entity> enemyHandle = dagger::PrefabFactory::create(enemyPrefab, scene, assets_);
        dagger::Entity* enemy = scene.getEntity(enemyHandle);

        if (enemy && enemy->hasComponent<dagger::TransformComponent>())
        {
            enemy->getComponent<dagger::TransformComponent>()->setPosition(randomX, randomY);
            chunk.activeEntities.push_back(enemyHandle);
        }
    }
}

} // namespace delve
