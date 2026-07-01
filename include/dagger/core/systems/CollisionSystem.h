#ifndef DAGGER_CORE_SYSTEMS_COLLISION_SYSTEM_H
#define DAGGER_CORE_SYSTEMS_COLLISION_SYSTEM_H

#include <vector>

namespace dagger
{

class Scene;
class Entity;

/// @brief Data payload representing a detected overlap between two entities.
struct CollisionEvent
{
    Entity* entityA;
    Entity* entityB;
};

/// @brief Performs AABB (Axis-Aligned Bounding Box) collision detection between active entities.
class CollisionSystem
{
public:
    /// @brief Evaluates all colliders in the scene and populates the provided list with detected overlaps.
    /// @param outCollisions Vector to store the detected collision events for this frame.
    static void update(Scene& scene, std::vector<CollisionEvent>& outCollisions);

private:
    /// @brief Low-level mathematical check for rectangle intersection.
    static bool checkAABB(float x1, float y1, float w1, float h1,
                          float x2, float y2, float w2, float h2);
};

} // namespace dagger

#endif // DAGGER_CORE_SYSTEMS_COLLISION_SYSTEM_H
