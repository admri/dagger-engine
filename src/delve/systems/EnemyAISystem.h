#ifndef DELVE_SYSTEMS_ENEMY_AI_SYSTEM_H
#define DELVE_SYSTEMS_ENEMY_AI_SYSTEM_H

namespace dagger
{
    class Scene;
}

namespace delve
{

class MovementSystem;
class TileMap;

class EnemyAISystem
{
public:
    void update(float dt, dagger::Scene& scene, MovementSystem& movementSystem, TileMap& tileMap);
};

} // namespace delve

#endif // DELVE_SYSTEMS_ENEMY_AI_SYSTEM_H
