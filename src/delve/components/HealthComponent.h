#ifndef DELVE_HEALTH_COMPONENT_H
#define DELVE_HEALTH_COMPONENT_H

#include <dagger/core/Component.h>

namespace delve
{

struct HealthComponent : public dagger::Component
{
    int hp = 3;
    int maxHp = 3;
    float invulnerabilityTimer = 0.0f;
};

} // namespace delve

#endif // DELVE_HEALTH_COMPONENT_H
