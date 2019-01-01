#include "common/test/kevin-test-repository.h"

using namespace kevin::test;

TestRepository::TestRepository() {}


TestRepository::~TestRepository()
{
    for (const auto &pair : _className2TestFixture) {
        delete pair.second;
    }
}


void
TestRepository::runTest(const std::string &testFixtureName,
        const std::string &testCaseName)
{
    auto nameIter = _name2ClassName.find(testFixtureName);
    if (nameIter == _name2ClassName.end()) {
        throw std::invalid_argument(
                "Test fixture name " + testFixtureName + " not found!");
    }

    auto iter = _className2TestCases.find(nameIter->second);
    if (iter == _className2TestCases.end()) {
        throw std::invalid_argument(
                "Test fixture name " + testFixtureName + " not found!");
    }

    UnitTestFixtureBase *testFixture = _className2TestFixture[nameIter->second]; 
    try {
        testFixture->setUpFixture();
        if (testCaseName == "") {
            for (const auto &pair : iter->second) {
                _runSingleCase(pair.first, pair.second, testFixture);
            }
        } else {
            auto caseIter = iter->second.find(testCaseName);
            if (caseIter == iter->second.end()) {
                throw std::invalid_argument(
                        std::string("Test case name ") + testCaseName + " not found!");
            }
            _runSingleCase(testCaseName, caseIter->second, testFixture);
        }
    } catch (std::exception &e) {
        testFixture->tearDownFixture();
        throw e;
    }
    testFixture->tearDownFixture();
}

void
TestRepository::_runSingleCase(const std::string &testCaseName,
           std::function<void(UnitTestFixtureBase *)> testCase,
           UnitTestFixtureBase *testFixture)
{
    try {
        testCase(testFixture);
    } catch (const std::exception &e) {
        // TODO(yihao) save test running result
    }
}
