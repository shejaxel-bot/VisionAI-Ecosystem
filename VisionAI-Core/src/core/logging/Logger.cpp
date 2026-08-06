#include "core/logging/Logger.h"

#include <chrono>
#include <ctime>
#include <iomanip>
#include <iostream>
#include <mutex>

namespace visionai {
namespace core {

static std::mutex g_log_mutex;
static LogLevel g_log_level = LogLevel::Info;

static const char* levelToString(LogLevel l) {
    switch (l) {
        case LogLevel::Debug: return "DEBUG";
        case LogLevel::Info: return "INFO";
        case LogLevel::Warning: return "WARNING";
        case LogLevel::Error: return "ERROR";
        default: return "UNKNOWN";
    }
}

void Logger::setLevel(LogLevel level) { g_log_level = level; }
LogLevel Logger::level() { return g_log_level; }

static std::string timestamp() {
    using namespace std::chrono;
    auto now = system_clock::now();
    auto tt = system_clock::to_time_t(now);
    auto tm = *std::gmtime(&tt);
    std::ostringstream ss;
    ss << std::put_time(&tm, "%Y-%m-%dT%H:%M:%SZ");
    return ss.str();
}

static void logImpl(LogLevel msgLevel, const std::string& message) {
    if (msgLevel < g_log_level) return;
    std::lock_guard<std::mutex> lock(g_log_mutex);
    std::cout << "[" << timestamp() << "] [" << levelToString(msgLevel) << "] " << message << '\n';
}

void Logger::debug(const std::string& message) { logImpl(LogLevel::Debug, message); }
void Logger::info(const std::string& message) { logImpl(LogLevel::Info, message); }
void Logger::warning(const std::string& message) { logImpl(LogLevel::Warning, message); }
void Logger::error(const std::string& message) { logImpl(LogLevel::Error, message); }

} // namespace core
} // namespace visionai
