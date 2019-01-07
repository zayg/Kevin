#include "common/logging/logging.h"
#include "common/logging/sink-flusher.h"

#include <json/value.h>
#include <json/reader.h>
#include <iostream>
#include <fstream>

namespace kevin {
namespace common {

static LoggingManager sLoggingMgr;

void
initLoggingSystem(const std::string &configFile)
{
    LoggingConfig conf(configFile);
    auto err = conf.validate();
    if (err) {
        return err;
    }

    // initialize logging manager
    auto err = sLoggingMgr.init(conf);
    if (!err.ok()) {
        KEVIN_ASSERT(false, err.toString());
    }
}


Error
refreshLoggingSystem(const std::string &configJson)
{
    Json::Value configJsonObj;
    Json::CharReader charReader;
    std::string errStr;
    bool ret = charReader.parse(configJson.c_str(),
            configJson.c_str() + configJson.size(),
            &configJsonObj,
            &errStr);
    if (!ret) {
        return KEVIN_ERROR(KEVIN_LOGGING, EINVAL, std::move(errStr));
    }

    LoggingConf conf(configJsonObj);
    auto err = conf.validate();
    if (err) {
        return err;
    }
    // refresh logging configuration
    return sLoggingMgr.refresh(conf);
}


void
uninitLoggingSystem()
{
    sLoggingMgr.uninit();
}


///////////////////////// Logger /////////////////////////

Logger::Logger(const std::string &identifier)
    : _identifier(identifier), _enabled{ false }, _logMgr(nullptr)
{
    sLoggingMgr.adopt(this);
}


///////////////////////// LoggingManager /////////////////////////

Error
LoggingManager::init(const LoggingConfig &jsonConfRoot)
{
    // TODO(yihao) create all the sinks based on the jsonConf
}


Error
LoggingManager::reinit(const LoggingConfig &jsonConfRoot)
{
    // TODO(yihao) create new sinks based on the new jsonConf
    // TODO(yihao) set correspond sinks to the loggers
}


void
LoggingManager::uninit()
{
    // TODO(yihao) flush all the sinks
}


void
syncTo(Logger *logger)
{ 
}

void
LoggingManager::adopt(Logger *logger)
{
    // Add logger into LoggingManager.
    _loggers[logger->_identifier] = logger;

    // Initialize _sink and _logLevel of logger.
    // TODO(yihao)

    // Enable the logger.
    logger->_logMgr = this;
    logger->_enabled = true;
}

} // namespace common
} // namespace kevin
