#ifndef DAGGER_CORE_SYSTEMS_COLLISION_SYSTEM_H
#define DAGGER_CORE_SYSTEMS_COLLISION_SYSTEM_H

#include <vector>

namespace dagger
{

class Scene;
class Entity;

struct CollisionEvent
{
    Entity* entityA;
    Entity* entityB;
};

class CollisionSystem
{
public:
    static void update(Scene& scene, std::vector<CollisionEvent>& outCollisions);

private:
    static bool checkAABB(float x1, float y1, float w1, float h1,
                          float x2, float y2, float w2, float h2);
};

} // namespace dagger

#endif // DAGGER_CORE_SYSTEMS_COLLISION_SYSTEM_H
