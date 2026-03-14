#ifndef DELVE_COMPONENTS_PLAYER_INPUT_COMPONENT_H
#define DELVE_COMPONENTS_PLAYER_INPUT_COMPONENT_H

#include <dagger/core/Component.h>

namespace delve
{

struct PlayerInputComponent : dagger::Component
{
    float speed = 300.0f;
};

} // namespace delve

#endif // DELVE_COMPONENTS_PLAYER_INPUT_COMPONENT_H
