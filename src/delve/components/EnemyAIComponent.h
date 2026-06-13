#ifndef DELVE_ENEMY_AI_COMPONENT_H
#define DELVE_ENEMY_AI_COMPONENT_H

#include <dagger/core/Component.h>

namespace delve
{

struct EnemyAIComponent : public dagger::Component
{
    float speed = 120.0f;
    float detectionRadius = 400.0f;
};

} // namespace delve

#endif // DELVE_ENEMY_AI_COMPONENT_H
