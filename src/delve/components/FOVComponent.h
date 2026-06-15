#ifndef DELVE_COMPONENTS_FOV_COMPONENT_H
#define DELVE_COMPONENTS_FOV_COMPONENT_H

#include <dagger/core/Component.h>
#include <dagger/core/Serialization.h>

#include <cstdint>
#include <unordered_set>

namespace delve
{

struct FOVComponent : public dagger::Component
{
    float radius = 400.0f;
    float fovAngle = 360.0f;
    int numRays = 360;
    float stepSize = 16.0f;

    std::unordered_set<uint64_t> visibleTiles;
    std::unordered_set<uint64_t> exploredTiles;
};

DAGGER_SERIALIZE_COMPONENT(FOVComponent, radius, fovAngle, numRays, stepSize)

} // namespace delve

#endif // DELVE_COMPONENTS_FOV_COMPONENT_H
