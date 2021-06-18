#include "console_log.h"
#include <iostream>

using namespace Logging;

ConsoleLogger::ConsoleLogger(LogLevel logLevel) :
    _logLevel(logLevel)
{
}

void ConsoleLogger::Log(LogLevel logLevel, const std::stringstream& message)
{
    if(_logLevel > logLevel)
        std::cout << message.str();
}