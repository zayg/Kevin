#pragma once

#include "common/error.h"
#include "common/logging/sink.h"
#include "common/logging/logging-conf.h"

#include <atomic>
#include <map>
#include <memory>
#include <string>
#include <utility>

namespace kevin {
namespace common {


Error initLoggingSystem(const std::string &configFile);
Error refreshLoggingSystem(const std::string &configJson);
void uninitLoggingSystem();


enum LogLevel : short {
    DEBUG = 0,
    INFO,
    WARN,
    ERRO,
    FATAL,
};

class LoggingManager;
class Logger {
public:
    Logger(const std::string &identifier);

    // Write data into the sink, filtering by the logLevel.
    <template LogLevel lvl, typename ...Args>
    void log(char *fmt, const Args &&...args) {
        // Filtering all smaller log levels.
        if (_logLevel < lvl) {
            return;
        }

        std::shared_ptr<Sink> sink = _sink.lock();
        while (sink == nullptr) {
            // Original sink is invalidated by configuration reload.
            // Sync with Logging Manager for the newest configuration.
            _logMgr->syncTo(this);
            sink = _sink.lock();
        }
        sink->pour(fmt, std::forward<Args>(args)...);
    }

private:
    // Configuration identifier for identifying the right logging sink.
    const std::string _identifier;

    // Logging level for logging filtering at runtime.
    // Changable during runtime.
    std::atomic<LogLevel> _logLevel;
    // Logging sink for saving logging into different destination.
    // Changable during runtime.
    std::weak_ptr<Sink> _sink;
    // Logger is enabled only after registerd to the static LoggingManager
    std::atomic<bool> _enabled;
    LoggingManager *_logMgr;

    friend class LoggingManager;
};


class SinkFlusher;
class LoggingManager {
public:
    Error init(const LoggingConfig &conf);
    Error reinit(const LoggingConfig &conf);
    void uninit();

    void syncTo(Logger *logger);
    void adopt(Logger *logger);

private:
    // Save all loggers corresponding to a configuration string identifier.
    // LoggingManager doesn't own the lifecycle of the loggers registered here.
    std::map<std::string, Logger *> _loggers;

    std::unique_ptr<SinkFlusher> _flusher;
};


} // namespace common
} // namespace kevin
