#ifndef VISIONAI_CORE_LOGGER_H
#define VISIONAI_CORE_LOGGER_H

#include <string>

namespace visionai {
namespace core {

enum class LogLevel { Debug = 0, Info, Warning, Error };

class Logger {
public:
    static void setLevel(LogLevel level);
    static LogLevel level();

    static void debug(const std::string& message);
    static void info(const std::string& message);
    static void warning(const std::string& message);
    static void error(const std::string& message);
};

} // namespace core
} // namespace visionai

#endif // VISIONAI_CORE_LOGGER_H
