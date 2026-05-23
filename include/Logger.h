#pragma once

#include <string>

#include "Observer.h"

/**
 * @brief Console logger used by parser and generator.
 */
class Logger : public Observer {
public:
    /// @brief Message severity.
    enum class LogLevel {
        Info,
        Warning,
        Error
    };

    /// @brief Creates logger with minimum output level.
    /// @param level Minimum emitted level.
    explicit Logger(LogLevel level = LogLevel::Info);

    /// @brief Logs informational message.
    /// @param message Text message.
    void info(const std::string& message) const;
    /// @brief Logs warning message.
    /// @param message Text message.
    void warning(const std::string& message) const;
    /// @brief Logs error message.
    /// @param message Text message.
    void error(const std::string& message) const;
    /// @brief Observer callback that forwards event to info log.
    /// @param message Event message.
    void update(const std::string& message) override;

private:
    void log(LogLevel level, const std::string& message) const;

    LogLevel minLevel_;
};
