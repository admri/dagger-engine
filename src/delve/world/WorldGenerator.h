#ifndef DELVE_WORLD_WORLD_GENERATOR_H
#define DELVE_WORLD_WORLD_GENERATOR_H

#include <cstdint>

namespace dagger
{
    class AssetManager;
    class Scene;
}

namespace delve
{

struct Chunk;
class TileMap;

class WorldGenerator
{
public:
    WorldGenerator(dagger::AssetManager& assets, uint64_t globalWorldSeed);
    void generateChunk(Chunk& chunk, dagger::Scene& scene, TileMap& tileMap);

private:
    dagger::AssetManager& assets_;
    uint64_t globalSeed_;
};

} // namespace delve

#endif // DELVE_WORLD_WORLD_GENERATOR_H
