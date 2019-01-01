#include "common/test/kevin-test.h"

#include <map>
#include <string>

namespace kevin {
namespace test {

TestRepository *gTestRepo = nullptr;

void
runTestByKevin(const std::string &testFixture, const std::string &testCase)
{
    if (gTestRepo == nullptr) {
        gTestRepo = new TestRepository();
    }
    gTestRepo->runTest(testFixture, testCase);    
}



} // namespace common
} // namespace kevin
