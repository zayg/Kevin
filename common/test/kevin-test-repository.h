#pragma once

#include <exception>
#include <functional>
#include <iostream>
#include <map>
#include <string>
#include <utility>

namespace kevin {
namespace test {

class UnitTestFixtureBase {
public:
    virtual ~UnitTestFixtureBase() {};
    virtual void setUpFixture() = 0;
    virtual void tearDownFixture() = 0;
    virtual void setUpCase() = 0;
    virtual void tearDownCase() = 0;
};

class TestRepository {
public:
    TestRepository();
    ~TestRepository();

    void runTest(const std::string &testFixtureName,
            const std::string &testCaseName);

    template <class TestFixtureType>
    void
    registerTestFixture(const std::string &testFixtureName)
    {
        std::string testFixtureClassName = typeid(TestFixtureType).name();
        _name2ClassName[testFixtureName] = testFixtureClassName;
        _className2TestFixture[testFixtureClassName] = new TestFixtureType();
    }

    template <class TestFixtureType>
    void
    registerTestCase(const std::string &testCaseName,
            std::function<void (TestFixtureType *)> &&testCaseFunc)
    {
        // find test fixture name in _className2TestFixture
        std::string testFixtureClassName = typeid(TestFixtureType).name();
        //TestFixtureType *testFixture = nullptr;
        //for (const auto &pair : _className2TestFixture) {
        //    std::cerr << pair.first << std::endl;
        //    testFixture = dynamic_cast<TestFixtureType *>(pair.second);
        //    if (testFixture != nullptr) {
        //        break;
        //    }
        //}

        // add new test case
        _className2TestCases[testFixtureClassName][testCaseName] =
            [testCaseFunc, testCaseName] (UnitTestFixtureBase *testFixture) {
                try {
                    testFixture->setUpCase();
                    testCaseFunc(dynamic_cast<TestFixtureType *>(testFixture));
                } catch (const std::exception &e) {
                    testFixture->tearDownCase();
                    std::cerr << "Case " << testCaseName << " got an exception:"
                        << std::endl << e.what();
                    throw e;
                }
                testFixture->tearDownCase();
            };
    }

private:
   void _runSingleCase(const std::string &testCaseName,
           std::function<void(UnitTestFixtureBase *)> testCase,
           UnitTestFixtureBase *testFixture);

private:
    std::map<std::string, std::string> _name2ClassName;
    std::map<std::string, UnitTestFixtureBase*> _className2TestFixture;
    std::map<std::string,
             std::map<std::string, std::function<void(UnitTestFixtureBase *)>>> _className2TestCases;
};

extern TestRepository *gTestRepo;

template <class TestFixtureType>
void
registerTestFixture(const std::string &testFixtureName)
{
    if (gTestRepo == nullptr) {
        gTestRepo = new TestRepository();
    }
    gTestRepo->registerTestFixture<TestFixtureType>(testFixtureName);
}

template <class TestFixtureType>
void
registerTestCase(const std::string &testCaseName,
        std::function<void (TestFixtureType *)> &&testCaseFunc)
{
    if (gTestRepo == nullptr) {
        gTestRepo = new TestRepository();
    }
    gTestRepo->registerTestCase<TestFixtureType>(testCaseName,
            std::move(testCaseFunc));
}

} // namespace common
} // namespace kevin
