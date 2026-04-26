#pragma once

#include <string>

#include "Observer.h"

class Logger : public Observer {
public:
    enum class LogLevel {
        Info,
        Warning,
        Error
    };

    explicit Logger(LogLevel level = LogLevel::Info);

    void info(const std::string& message) const;
    void warning(const std::string& message) const;
    void error(const std::string& message) const;
    void update(const std::string& message) override;

private:
    void log(LogLevel level, const std::string& message) const;

    LogLevel minLevel_;
};
