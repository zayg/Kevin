# Overview
Ktest will be the testing framework of Kevin. Functionalities will be supported:

* Support different macros to ASSERT/EXPECT some values/exceptions.
* Support dividing test cases into Fixture. Each TestFixture has its own Setup/Teardown-Fixture and Setup/Teardown-Case.
* Support some pattern matching to run specific test fixtures or cases only.
* Support macros to inject friend test classes into dev classes only when compiling with Ktest.