#pragma once

#include <map>

namespace kevin {
namespace common {

enum class SinkType : short {
    FILE_SINK = 0,
    CONSOLE_SINK,
    DEV_NULL_SINK,
};

class Sink {
public:
    Sink(const std::string &name);
    virtual void pour();
    virtual void flush();
private:
    Sink *_impl;     
};

} // namespace common
} // namespace kevin

