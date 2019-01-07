#include "common/error.h"

#include <string.h>

namespace kevin {
namespace common {

static char sErrorDef[static_cast<int>(ErrorDef::KEVIN_LAST)][64] = {
   "KEVIN_NO_ERROR"
};

std::string
errorDefToString(ErrorDef errDef)
{
    return sErrorDef[static_cast<int>(errDef)];
}


Error::Error(ErrorDef catagory, int errno, const std::string &&message)
    : m_catagory(catagory), m_errno(errno) {
    auto len = message.size();
    // only support message with length than sizeof(m_msg) - 1, which is 255
    // bytes.
    if (len > sizeof(m_msg) - 1) {
        len = sizeof(m_msg) - 1;
    }
    ::memcpy(m_msg, message.c_str(), len);
}


} // namespace common
} // namespace kevin
