#pragma once
#include <errno.h>
#include <string>

#define KEVIN_OK kevin::common::Error();
#define KEVIN_ERROR(catagory, errno, message) \
    kevin::common::Error(kevin::common:: ## catagory, errno, message) \

namespace kevin {
namespace common {

enum class ErrorDef {
    KEVIN_NO_ERROR = 0,
    // always to be the last one
    KEVIN_LAST,
};

std::string errorDefToString(ErrorDef errDef);

class Error {
public:
    Error(ErrorDef catagory, int errno, const std::string &&message);

    // explicitly declare these constructors for readability
    Error(const Error &) = default;
    Error(Error &&) = default;
    Error & operator=(const Error &) = default;

    inline std::string toString() {
        std::string str;
        str.reserve(256);
        str.append("{ Catagory: ").append(errorDefToString(m_catagory))
            .append(", Error: `").append(::strerror(m_errno)).append("`")
            .append(", Message: `").append(m_msg).append("` }");
        return str;
    }

private:
    ErrorDef m_catagory = ErrorDef::KEVIN_NO_ERROR;
    int m_errno = 0;
    char m_msg[256] = {0};
};

} // namespace common
} // namespace kevin
