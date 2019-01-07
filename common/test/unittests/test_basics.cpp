#include "common/test/kevin-test.h"

using namespace kevin::test;

class TestFixtureA : public UnitTestFixture<TestFixtureA> {
public:
    void TestCase1() {
        KEVIN_TEST_ASSERT_EQ(1+1, 2);
    }
    KEVIN_TEST_CASE(TestCase1);

    void TestCase2() {
        KEVIN_TEST_ASSERT_EQ(1+2, 3);
    }
    KEVIN_TEST_CASE(TestCase2);
};
KEVIN_TEST_FIXTURE(TestFixtureA);
