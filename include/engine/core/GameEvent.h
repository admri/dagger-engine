#ifndef GAME_EVENT_H
#define GAME_EVENT_H

enum class GameEventType
{
    ToggleVSync,
    Quit
};

struct GameEvent
{
    GameEventType type;
};

#endif // GAME_EVENT_H