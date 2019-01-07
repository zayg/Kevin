#include "common/logging/logging-conf.h"

using kevin::common;

    // parse json configuration object
    Json::Value jsonConfRoot;
    if (!configFile.empty()) {
        std::ifstream ifs;
        ifs.exceptions(std::ifstream::failbit | std::ifstream::badbit);
        try {
            ifs.open(configFile, std::ifstream::in);
            Json::Reader reader;
            if (!reader.parse(ifs, jsonConfRoot)) {
                KEVIN_ASSERT(false, "Failed to parse logging conf");
            }
        } catch (const std::ifstream::failure &e) {
            KEVIN_ASSERT(false, e.what());
        }
    }

