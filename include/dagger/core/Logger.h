#ifndef DAGGER_CORE_LOGGER_H
#define DAGGER_CORE_LOGGER_H

#include <format>
#include <string>
#include <string_view>

namespace dagger
{

class Logger
{
public:
    template <typename... Args>
    static void info(std::format_string<Args...> fmt, Args&&... args)
    {
        logInternal(LogLevel::Info, std::format(fmt, std::forward<Args>(args)...));
    }

    template <typename... Args>
    static void warn(std::format_string<Args...> fmt, Args&&... args)
    {
        logInternal(LogLevel::Warn, std::format(fmt, std::forward<Args>(args)...));
    }

    template <typename... Args>
    static void error(std::format_string<Args...> fmt, Args&&... args)
    {
        logInternal(LogLevel::Error, std::format(fmt, std::forward<Args>(args)...));
    }

private:
    enum class LogLevel
    {
        Info,
        Warn,
        Error
    };

    static void logInternal(LogLevel level, const std::string& message);
};

} // namespace dagger

#endif // DAGGER_CORE_LOGGER_H
