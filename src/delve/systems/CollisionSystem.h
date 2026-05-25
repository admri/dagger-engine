#ifndef DELVE_SYSTEMS_COLLISION_SYSTEM_H
#define DELVE_SYSTEMS_COLLISION_SYSTEM_H

namespace delve
{

class CollisionSystem
{
public:
    static bool checkAABB(float x1, float y1, float w1, float h1,
                          float x2, float y2, float w2, float h2);
};

} // namespace delve

#endif // DELVE_SYSTEMS_COLLISION_SYSTEM_H
