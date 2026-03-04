#ifndef DAGGER_CORE_GAME_EVENT_H
#define DAGGER_CORE_GAME_EVENT_H

namespace dagger
{

enum class GameEventType
{
    ToggleVSync,
    Quit
};

struct GameEvent
{
    GameEventType type;
};

} // namespace dagger

#endif // DAGGER_CORE_GAME_EVENT_H
