#ifndef DELVE_COMPONENTS_ENEMY_AI_COMPONENT_H
#define DELVE_COMPONENTS_ENEMY_AI_COMPONENT_H

#include <dagger/core/Component.h>
#include <dagger/core/Serialization.h>

namespace delve
{

struct EnemyAIComponent : public dagger::Component
{
    float speed = 120.0f;
    float detectionRadius = 400.0f;
};

DAGGER_SERIALIZE_COMPONENT(EnemyAIComponent, speed, detectionRadius)

} // namespace delve

#endif // DELVE_COMPONENTS_ENEMY_AI_COMPONENT_H
