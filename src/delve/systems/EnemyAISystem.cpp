#include <systems/EnemyAISystem.h>

#include <components/EnemyAIComponent.h>
#include <components/PlayerInputComponent.h>
#include <world/TileMap.h>

#include <dagger/core/Entity.h>
#include <dagger/core/Scene.h>
#include <dagger/core/components/AnimationComponent.h>
#include <dagger/core/components/BoxColliderComponent.h>
#include <dagger/core/components/SpriteComponent.h>
#include <dagger/core/components/TransformComponent.h>
#include <dagger/core/components/VelocityComponent.h>
#include <dagger/math/Vector2.h>
#include <dagger/utils/StringHash.h>

#include <cmath>
#include <queue>
#include <unordered_map>

using namespace dagger::literals;

namespace delve
{

namespace
{
    using GridPos = dagger::math::Vec2i;

    // A* heuristic (Manhattan distance)
    inline float heuristic(GridPos pos1, GridPos pos2)
    {
        return std::abs(pos1.x - pos2.x) + std::abs(pos1.y - pos2.y);
    }

    bool hasLineOfSight(dagger::math::Vec2f start, dagger::math::Vec2f target, float radius, const TileMap& map)
    {
        float dirX = target.x - start.x;
        float dirY = target.y - start.y;
        
        // We calculate how many tiles we need to cross
        float tileSize = map.getTileSize();
        float spanX = std::abs(dirX) / tileSize;
        float spanY = std::abs(dirY) / tileSize;
        int steps = static_cast<int>(std::max(spanX, spanY)) * 2 + 1;
        
        if (steps == 0) return true;

        float stepX = dirX / static_cast<float>(steps);
        float stepY = dirY / static_cast<float>(steps);

        for (int i = 0; i <= steps; ++i)
        {
            float sampleX = start.x + (stepX * i);
            float sampleY = start.y + (stepY * i);
            
            int minGridX = static_cast<int>(std::floor((sampleX - radius) / tileSize));
            int maxGridX = static_cast<int>(std::floor((sampleX + radius) / tileSize));
            int minGridY = static_cast<int>(std::floor((sampleY - radius) / tileSize));
            int maxGridY = static_cast<int>(std::floor((sampleY + radius) / tileSize));

            for (int x = minGridX; x <= maxGridX; ++x)
            {
                for (int y = minGridY; y <= maxGridY; ++y)
                {
                    if (map.getTile(x, y) == TileType::Wall)
                    {
                        return false; // We hit a wall
                    }
                }
            }
        }
        return true; // There is a line of sight, the path is free
    }

    // Comparator for priority queue
    struct PathNodeCompare
    {
        bool operator()(const std::pair<float, GridPos>& a, const std::pair<float, GridPos>& b) const
        {
            return a.first > b.first; 
        }
    };

    auto findPath(GridPos start, GridPos goal, const TileMap& tileMap) -> std::vector<GridPos>
    {
        std::priority_queue<
            std::pair<float, GridPos>, 
            std::vector<std::pair<float, GridPos>>, 
            PathNodeCompare
        > open_set;
        open_set.push({0.0f, start});

        std::unordered_map<GridPos, GridPos> cameFrom;
        std::unordered_map<GridPos, float> costSoFar;

        cameFrom[start] = start;
        costSoFar[start] = 0.0f;

        // Each direction with its cost (diagonals have cost of sqrt(2))
        const std::vector<std::pair<GridPos, float>> directions = {
            {{1, 0}, 1.0f},   {{-1, 0},  1.0f},   {{0, 1},  1.0f},   {{0, -1}, 1.0f},
            {{1, 1}, 1.414f}, {{-1, -1}, 1.414f}, {{1, -1}, 1.414f}, {{-1, 1}, 1.414f}
        };

        bool found = false;

        while (!open_set.empty())
        {
            GridPos current = open_set.top().second;
            open_set.pop();

            // Check if we have found our goal
            if (current == goal)
            {
                found = true;
                break;
            }

            // Otherwise, go in each direction
            for (const auto& dirPair : directions)
            {
                // Get the next position
                GridPos dir = dirPair.first;
                float stepCost = dirPair.second;
                GridPos next = current + dir;

                // Check if next position is a wall
                if (tileMap.getTile(next.x, next.y) == TileType::Wall)
                {
                    continue;
                }

                if (dir.x != 0 && dir.y != 0)
                {
                    if (tileMap.getTile(current.x + dir.x, current.y) == TileType::Wall ||
                        tileMap.getTile(current.x, current.y + dir.y) == TileType::Wall)
                    {
                        continue; 
                    }
                }

                // Update cost for new step
                float newCost = costSoFar[current] + stepCost;

                // Re-calculate priority and update open positions if we have found a better cost
                if (costSoFar.find(next) == costSoFar.end() || newCost < costSoFar[next])
                {
                    costSoFar[next] = newCost;
                    float priority = newCost + heuristic(next, goal);
                    open_set.push({priority, next});
                    cameFrom[next] = current;
                }
            }
        }

        // Reconstruct the path from goal to start
        std::vector<GridPos> path;
        if (found)
        {
            GridPos current = goal;
            while (current != start)
            {
                path.push_back(current);
                current = cameFrom[current];
            }
        }

        return path;
    }
}

void EnemyAISystem::update(float dt, dagger::Scene& scene, TileMap& tileMap)
{
    auto* player = scene.getFirstEntityWith<dagger::TransformComponent, PlayerInputComponent, dagger::BoxColliderComponent>();
    if (!player) return;

    auto* pTransform = player->getComponent<dagger::TransformComponent>();
    auto* pCollider = player->getComponent<dagger::BoxColliderComponent>();

    float playerCenterX = pTransform->x + pCollider->offsetX + (pCollider->width / 2.0f);
    float playerCenterY = pTransform->y + pCollider->offsetY + (pCollider->height / 2.0f);

    // Player grid position (end)
    int playerGridX = static_cast<int>(std::floor(playerCenterX / tileMap.getTileSize()));
    int playerGridY = static_cast<int>(std::floor(playerCenterY / tileMap.getTileSize()));

    scene.forEachEntityWith<dagger::TransformComponent, dagger::VelocityComponent, EnemyAIComponent, dagger::BoxColliderComponent>(
        [&](dagger::Entity* enemy, auto& transform, auto& velocity, auto& ai, auto& collider)
    {
        float enemyCenterX = transform.x + collider.offsetX + (collider.width / 2.0f);
        float enemyCenterY = transform.y + collider.offsetY + (collider.height / 2.0f);

        // Direct distance to player
        float dirX = playerCenterX - enemyCenterX;
        float dirY = playerCenterY - enemyCenterY;
        float distanceToPlayer = std::sqrt(dirX * dirX + dirY * dirY);

        // Enemy grid position (start)
        int enemyGridX = static_cast<int>(std::floor(enemyCenterX / tileMap.getTileSize()));
        int enemyGridY = static_cast<int>(std::floor(enemyCenterY / tileMap.getTileSize()));

        // Calculate A* path only in intervals (to save on performance)
        ai.pathRecalculateTimer += dt;
        if (distanceToPlayer < ai.detectionRadius && ai.pathRecalculateTimer >= ai.pathRecalculateInterval)
        {
            ai.pathRecalculateTimer = 0.0f;
            ai.currentPath = findPath({enemyGridX, enemyGridY}, {playerGridX, playerGridY}, tileMap);
        }

        // Following the path
        velocity.vx = 0.0f;
        velocity.vy = 0.0f;

        if (!ai.currentPath.empty() && distanceToPlayer < ai.detectionRadius)
        {
            // String pulling
            int targetIndex = ai.currentPath.size() - 1;

            float enemyRadius = (collider.width / 2.0f) * 0.8f;
            
            for (int i = 0; i < static_cast<int>(ai.currentPath.size()); ++i)
            {
                float tileX = (ai.currentPath[i].x * tileMap.getTileSize()) + (tileMap.getTileSize() / 2.0f);
                float tileY = (ai.currentPath[i].y * tileMap.getTileSize()) + (tileMap.getTileSize() / 2.0f);
                
                if (hasLineOfSight({enemyCenterX, enemyCenterY}, {tileX, tileY}, enemyRadius, tileMap))
                {
                    targetIndex = i;
                    break; 
                }
            }

            while (static_cast<int>(ai.currentPath.size()) - 1 > targetIndex)
            {
                ai.currentPath.pop_back();
            }
            // ---------------------------------------------------

            auto nextTile = ai.currentPath.back();
            float targetX = (nextTile.x * tileMap.getTileSize()) + (tileMap.getTileSize() / 2.0f);
            float targetY = (nextTile.y * tileMap.getTileSize()) + (tileMap.getTileSize() / 2.0f);

            float toNextX = targetX - enemyCenterX;
            float toNextY = targetY - enemyCenterY;
            float distToNext = std::sqrt(toNextX * toNextX + toNextY * toNextY);

            if (distToNext < 5.0f) 
            {
                ai.currentPath.pop_back();
            }
            else
            {
                toNextX /= distToNext;
                toNextY /= distToNext;
                velocity.vx = toNextX * ai.speed;
                velocity.vy = toNextY * ai.speed;
            }
        }

        // Visual logic (animations / sprite flipping)
        auto* anim = enemy->getComponent<dagger::AnimationComponent>();
        auto* sprite = enemy->getComponent<dagger::SpriteComponent>();

        if (anim && sprite)
        {
            // Check enemy's direction
            if (velocity.vx < 0.0f) sprite->flipX = true;
            else if (velocity.vx > 0.0f) sprite->flipX = false;

            // Check if enemy is moving
            if (velocity.vx != 0.0f || velocity.vy != 0.0f) anim->play("run"_hash);
            else anim->play("idle"_hash);
        }
    });
}

} // namespace delve
