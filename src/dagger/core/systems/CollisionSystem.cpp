#include <core/systems/CollisionSystem.h>

#include <dagger/core/Entity.h>
#include <dagger/core/Scene.h>
#include <dagger/core/components/BoxColliderComponent.h>
#include <dagger/core/components/TransformComponent.h>

namespace dagger
{

bool CollisionSystem::checkAABB(float x1, float y1, float w1, float h1,
                                float x2, float y2, float w2, float h2)
{
    return (x1 < x2 + w2 &&
            x1 + w1 > x2 &&
            y1 < y2 + h2 &&
            y1 + h1 > y2);
}

void CollisionSystem::update(Scene& scene, std::vector<CollisionEvent>& outCollisions)
{
    outCollisions.clear();

    static std::vector<Entity*> collidables;
    collidables.clear();

    scene.forEachEntityWith<TransformComponent, BoxColliderComponent>(
        [&](auto* entity)
    {
        collidables.push_back(entity);
    });

    for (size_t i = 0; i < collidables.size(); ++i)
    {
        for (size_t j = i + 1; j < collidables.size(); ++j)
        {
            auto* entA = collidables[i];
            auto* entB = collidables[j];

            auto* tA = entA->getComponent<TransformComponent>();
            auto* cA = entA->getComponent<BoxColliderComponent>();

            auto* tB = entB->getComponent<TransformComponent>();
            auto* cB = entB->getComponent<BoxColliderComponent>();

            if (checkAABB(
                tA->x + cA->offsetX, tA->y + cA->offsetY, cA->width, cA->height,
                tB->x + cB->offsetX, tB->y + cB->offsetY, cB->width, cB->height))
            {
                outCollisions.push_back({ entA, entB });
            }
        }
    }
}

} // namespace dagger
