#pragma once

#include "common/test/kevin-test-repository.h"

#include <exception>
#include <iostream>
#include <string>

// Utility MACROs for writing test logics.
#define KEVIN_TEST_ASSERT(x) \
    if (!(x)) { \
        throw std::runtime_error(std::string(# x) + " failed!"); \
    }

#define KEVIN_TEST_ASSERT_EQ(x, y) KEVIN_ASSERT((x) == (y))

// TODO(yihao) not supported yet
#define KEVIN_EXPECT
#define KEVIN_EXPECT_EQ

//// Utility MACROs for completing a test case.
//// An example of how to write you own tests.
//
// class TestFixtureA : public UnitTestFixture<TestFixtureA> {
// public:
//     void TestCase1() {
//         KEVIN_EXPECT_EQ(1+1, 2);
//     }
//     KEVIN_TEST_CASE(TestCase1);
//
//     void TestCase2() {
//         KEVIN_EXPECT_EQ(1+2, 3);
//     }
//     KEVIN_TEST_CASE(TestCase2);
// };
// KEVIN_TEST_FIXTURE(TestFixtureA);
#define KEVIN_TEST_FIXTURE(TestFixtureType) \
    struct Register ## TestFixtureType { \
        Register ## TestFixtureType () { \
            kevin::test::registerTestFixture<TestFixtureType>( \
                    #TestFixtureType); \
        } \
    }; \
    static Register ## TestFixtureType s ## TestFixtureType; \

#define KEVIN_TEST_CASE(TestCaseName) \
    struct Register ## TestCaseName { \
        Register ## TestCaseName() { \
            kevin::test::registerTestCase<TestFixtureType>(#TestCaseName, \
                    &TestFixtureType::TestCaseName); \
        } \
    } m ## TestCaseName; \

namespace kevin {
namespace test {

// Run kevin test cases.
// testFixture name and testCase name are provided for running a subset of test
// cases.
void runTestByKevin(const std::string &testFixture, const std::string &testCase);

template <class TestFixture>
class UnitTestFixture : public UnitTestFixtureBase {
public:
    using TestFixtureType = TestFixture;
    virtual void setUpFixture() {};
    virtual void tearDownFixture() {};
    virtual void setUpCase() {};
    virtual void tearDownCase() {};
};

} // namespace test 
} // namespace kevin
