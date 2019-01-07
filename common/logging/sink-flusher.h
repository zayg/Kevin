#pragma once

#include "common/logging/sink.h"
#include <memory>
#include <vector>

namespace kevin {
namespace common {

class SinkFlushManager{
public:
private:
    std::vector<std::shared_ptr<Sink>> _sinks;
};

} // namespace common
} // namespace kevin

