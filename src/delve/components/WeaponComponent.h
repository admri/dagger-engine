#ifndef DELVE_COMPONENTS_WEAPON_COMPONENT_H
#define DELVE_COMPONENTS_WEAPON_COMPONENT_H

#include <dagger/assets/ResourceHandle.h>
#include <dagger/core/Component.h>
#include <dagger/core/Serialization.h>

namespace delve
{

struct WeaponComponent : public dagger::Component
{
    float cooldown = 0.0f;
    float fireRate = 0.25f;
    float projectileSpeed = 600.0f;
    int damage = 1;

    dagger::SoundHandle shootSound;

    bool isTriggerPulled = false;
    float targetX = 0.0f;
    float targetY = 0.0f;
};

DAGGER_SERIALIZE_COMPONENT(WeaponComponent, fireRate, projectileSpeed, damage)

} // namespace delve

#endif // DELVE_COMPONENTS_WEAPON_COMPONENT_H
