#pragma once

#include "log_level.h"
#include "logger_interface.h"

#include <atomic>

namespace Logging
{
    class ConsoleLogger : public LoggerInterface
    {
    public:
        ConsoleLogger(LogLevel logLevel);
        virtual ~ConsoleLogger() = default;
        virtual LogLevel GetLogLevel(void) const override { return _logLevel; }
        void SetLogLevel(LogLevel logLevel) { _logLevel.store(logLevel); }
        virtual void Log(LogLevel logLevel, const std::stringstream& message) override;

    private:
        std::atomic<LogLevel> _logLevel;
    };
} // namespace Logging