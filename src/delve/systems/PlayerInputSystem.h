#ifndef DELVE_SYSTEMS_PLAYER_INPUT_SYSTEM_H
#define DELVE_SYSTEMS_PLAYER_INPUT_SYSTEM_H

namespace dagger
{
    class Input;
    class VelocityComponent;
}

namespace delve
{

struct PlayerInputComponent;

class PlayerInputSystem
{
public:
    void update(dagger::Input& input, dagger::VelocityComponent& vel, const PlayerInputComponent& playerInput);
};

} // namespace delve

#endif // DELVE_SYSTEMS_PLAYER_INPUT_SYSTEM_H
