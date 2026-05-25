#ifndef DELVE_COMPONENTS_PROJECTILE_COMPONENT_H
#define DELVE_COMPONENTS_PROJECTILE_COMPONENT_H

#include <dagger/core/Component.h>

namespace delve
{

struct ProjectileComponent : public dagger::Component
{
    bool active = true; 
};

} // namespace delve

#endif // DELVE_COMPONENTS_PROJECTILE_COMPONENT_H
