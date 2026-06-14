#ifndef DELVE_COMPONENTS_PROJECTILE_COMPONENT_H
#define DELVE_COMPONENTS_PROJECTILE_COMPONENT_H

#include <dagger/core/Component.h>
#include <dagger/core/Serialization.h>

namespace delve
{

struct ProjectileComponent : public dagger::Component
{
    int damage = 1;
};

DAGGER_SERIALIZE_COMPONENT(ProjectileComponent, damage)

} // namespace delve

#endif // DELVE_COMPONENTS_PROJECTILE_COMPONENT_H
