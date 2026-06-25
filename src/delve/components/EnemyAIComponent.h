#ifndef DELVE_COMPONENTS_ENEMY_AI_COMPONENT_H
#define DELVE_COMPONENTS_ENEMY_AI_COMPONENT_H

#include <dagger/core/Component.h>
#include <dagger/core/Serialization.h>
#include <dagger/math/Vector2.h>

#include <utility>
#include <vector>

namespace delve
{

struct EnemyAIComponent : public dagger::Component
{
    std::vector<dagger::math::Vec2i> currentPath; 

    float speed = 120.0f;
    float detectionRadius = 400.0f;

    float pathRecalculateTimer = 0.0f;
    float pathRecalculateInterval = 0.1f;
};

DAGGER_SERIALIZE_COMPONENT(EnemyAIComponent, speed, detectionRadius, pathRecalculateInterval)

} // namespace delve

#endif // DELVE_COMPONENTS_ENEMY_AI_COMPONENT_H
