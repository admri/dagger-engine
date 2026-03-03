#ifndef GAME_EVENT_H
#define GAME_EVENT_H

namespace engine
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

} // namespace engine

#endif // GAME_EVENT_H
