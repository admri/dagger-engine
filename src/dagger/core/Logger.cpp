#include <core/Logger.h>

#include <SDL3/SDL_log.h>

namespace dagger
{

void Logger::logInternal(LogLevel level, const std::string& message)
{
    switch (level)
    {
        case LogLevel::Info:
            SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_INFO, "%s", message.c_str());
            break;
        case LogLevel::Warn:
            SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_WARN, "%s", message.c_str());
            break;
        case LogLevel::Error:
            SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_ERROR, "%s", message.c_str());
            break;
    }
}

} // namespace dagger
