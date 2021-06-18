#pragma once

#include <string>
#include <sstream>
#include "log_level.h"

namespace Logging
{
    class LoggerInterface
    {
    public:
        virtual ~LoggerInterface() = default;
        virtual LogLevel GetLogLevel(void) const = 0;
        virtual void Log(LogLevel logLevel, const std::stringstream& message) = 0;
    };
}