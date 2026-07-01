#ifndef DAGGER_CORE_GAME_EVENT_H
#define DAGGER_CORE_GAME_EVENT_H

namespace dagger
{

/// @brief Core engine event types for global application state changes.
enum class GameEventType
{
    ToggleVSync,
    Quit
};

/// @brief Represents a global system-level event passed to Application::onEvent.
struct GameEvent
{
    GameEventType type;
};

} // namespace dagger

#endif // DAGGER_CORE_GAME_EVENT_H
