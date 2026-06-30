#ifndef DELVE_EVENTS_GAMEPLAY_EVENTS_H
#define DELVE_EVENTS_GAMEPLAY_EVENTS_H

#include <dagger/assets/ResourceHandle.h>

namespace delve
{

struct ProjectileFiredEvent
{
    float startX;
    float startY;
    dagger::SoundHandle audio;
};

} // namespace delve

#endif // DELVE_EVENTS_GAMEPLAY_EVENTS_H
