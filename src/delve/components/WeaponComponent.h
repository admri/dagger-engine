#ifndef DELVE_COMPONENTS_WEAPON_COMPONENT_H
#define DELVE_COMPONENTS_WEAPON_COMPONENT_H

#include <dagger/core/Component.h>

namespace delve
{

struct WeaponComponent : public dagger::Component
{
    float cooldown = 0.0f;
    float fireRate = 0.25f;
    float projectileSpeed = 600.0f;
    int damage = 1;

    bool isTriggerPulled = false;
    float targetX = 0.0f;
    float targetY = 0.0f;
};

} // namespace delve

#endif // DELVE_COMPONENTS_WEAPON_COMPONENT_H
