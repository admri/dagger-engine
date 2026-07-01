#include <world/WorldGenerator.h>

#include <world/ChunkManager.h>
#include <world/TileMap.h>

#include <dagger/assets/AssetManager.h>
#include <dagger/assets/PrefabFactory.h>
#include <dagger/assets/ResourceHandle.h>
#include <dagger/core/Scene.h>
#include <dagger/core/components/BoxColliderComponent.h>
#include <dagger/core/components/TransformComponent.h>
#include <dagger/math/PerlinNoise.h>

#include <random>
#include <utility>

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

    // List of safe tiles for spawning enemies
    std::vector<std::pair<int, int>> validFloorTiles;

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

            // If we have a floor, add it as a potential enemy spawn position (avoiding player spawn position)
            if (type == TileType::Floor && !(globalTileX >= -5 && globalTileX <= 5 && globalTileY >= -5 && globalTileY <= 5))
            {
                validFloorTiles.push_back({globalTileX, globalTileY});
            }
        }
    }

    // End chunk generation if there is no place to spawn enemies
    if (validFloorTiles.empty()) return;

    // Enemy spawning;
    int enemiesToSpawn = enemyCountDist(rng);

    // Clamp if there are too many enemies
    if (enemiesToSpawn > validFloorTiles.size())
    {
        enemiesToSpawn = validFloorTiles.size();
    }

    auto enemyPrefab = assets_.get<dagger::Prefab>("enemy");

    std::uniform_int_distribution<size_t> tileDist(0, validFloorTiles.size() - 1);

    for (int i = 0; i < enemiesToSpawn; ++i)
    {
        // Choose random floor tile
        size_t randomIndex = tileDist(rng);
        auto [tileX, tileY] = validFloorTiles[randomIndex];

        // Remove the tile from the list to avoid spawning two enemies on top of each other
        validFloorTiles[randomIndex] = validFloorTiles.back();
        validFloorTiles.pop_back();
        
        // Update distribution
        if (!validFloorTiles.empty())
        {
            tileDist = std::uniform_int_distribution<size_t>(0, validFloorTiles.size() - 1);
        }

        dagger::ResourceHandle<dagger::Entity> enemyHandle = dagger::PrefabFactory::create(enemyPrefab, scene, assets_);
        dagger::Entity* enemy = scene.getEntity(enemyHandle);

        if (enemy && enemy->hasComponent<dagger::TransformComponent>())
        {
            float spawnX = tileX * ChunkManager::TILE_SIZE;
            float spawnY = tileY * ChunkManager::TILE_SIZE;

            if (auto* collider = enemy->getComponent<dagger::BoxColliderComponent>())
            {
                spawnX += (ChunkManager::TILE_SIZE - collider->width) / 2.0f;
                spawnY += (ChunkManager::TILE_SIZE - collider->height) / 2.0f;
            }

            enemy->getComponent<dagger::TransformComponent>()->setPosition(spawnX, spawnY);
            chunk.activeEntities.push_back(enemyHandle);
        }
    }
}

} // namespace delve
