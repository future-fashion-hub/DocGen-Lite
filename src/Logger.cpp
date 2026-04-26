#include "Logger.h"

#include <iostream>

Logger::Logger(LogLevel level) : minLevel_(level) {}

void Logger::info(const std::string& message) const {
    log(LogLevel::Info, message);
}

void Logger::warning(const std::string& message) const {
    log(LogLevel::Warning, message);
}

void Logger::error(const std::string& message) const {
    log(LogLevel::Error, message);
}

void Logger::update(const std::string& message) {
    info(message);
}

void Logger::log(LogLevel level, const std::string& message) const {
    if (static_cast<int>(level) < static_cast<int>(minLevel_)) {
        return;
    }

    const char* label = "INFO";
    if (level == LogLevel::Warning) {
        label = "WARNING";
    } else if (level == LogLevel::Error) {
        label = "ERROR";
    }

    std::cout << "[" << label << "] " << message << '\n';
}
