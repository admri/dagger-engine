#ifndef DELVE_TYPES_PLAYER_ANIM_H
#define DELVE_TYPES_PLAYER_ANIM_H

#include <cstdint>

namespace delve
{

enum PlayerAnim : uint32_t
{
    Idle,
    Run,
    Attack,
    Count
};

} // namespace delve

#endif // DELVE_TYPES_PLAYER_ANIM_H
