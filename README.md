cute: C++ unit test executor [![Build Status](https://travis-ci.org/Kosta-Github/cute.png)](https://travis-ci.org/Kosta-Github/cute)
============================
Feature set:
- uses C++11 features
- no external dependencies
- header-only
- thread-safe
- supports tags for test case filtering
- can use custom reporters (provided: simple ide/console reporter, JUnit XML reporter)
- provides capabilities for mocking classes and interfaces (planned)


status: alpha
=============
The development has just started and is not considered to be stable at the moment.


sample usage
============
A very simple failing test case would look like this:
```C++
CUTE_TEST("Test two strings for equality") {
    auto str1 = std::string("hello");
    auto str2 = std::string("world");
    CUTE_ASSERT(str1 == str2);
}
```
This would produce the following output with the provided IDE reporter:
```
.../cute_tests.cpp:64: error: Test two strings for equality
.../cute_tests.cpp:64: error:     duration:   0 ms
.../cute_tests.cpp:67: error:     reason:     validation failed: str1 == str2
.../cute_tests.cpp:67: error:     with:       str1 == str2 => "hello" == "world"
```

Capturing additional values and reporting them in case of an error can be achieved by adding some `CUTE_CAPTURE()` calls to `CUTE_ASSERT()`:
```C++
CUTE_TEST("Capture additional values") {
    auto str1 = std::string("hello");
    auto str2 = std::string("world");
    auto value = 42;
    CUTE_ASSERT(str1 == str2,
        CUTE_CAPTURE(str1.length()),
        CUTE_CAPTURE(str2.capacity()),
        CUTE_CAPTURE(value)
    );
}
```
This would produce the following output with the provided IDE reporter:
```
.../cute_tests.cpp:70: error: Capture additional values
.../cute_tests.cpp:70: error:     duration:   0 ms
.../cute_tests.cpp:79: error:     reason:     validation failed: str1 == str2
.../cute_tests.cpp:79: error:     with:       str1 == str2 => "hello" == "world"
.../cute_tests.cpp:79: error:     with:       str1.size() => 5
.../cute_tests.cpp:79: error:     with:       str2.capacity() => 22
.../cute_tests.cpp:79: error:     with:       value => 42
```

The following macros can be used for validation within a test case:
- `CUTE_ASSERT(ex)`: verifies that the given boolean expression `ex` evaluates to `true`.
- `CUTE_ASSERT_THROWS_NOT(ex)`: verifies that evaluating the given expression `ex` will not throw an `exception`.
- `CUTE_ASSERT_THROWS(ex)`: verifies that evaluating the given expression `ex` will throw an `exception`.
- `CUTE_ASSERT_THROWS_AS(ex, excp)`: verifies that evaluating the given expression `ex` will throw an `exception` of type `excp` or derived from it.

Each validation macro can be extended by several `CUTE_CAPTURE()` occurrences in order to capture additional values and provide more context information in case of a failure.
 
Note that `cute` stops a test case by throwing an `exception` as soon as a violation of a `CUTE_ASSERT*()` is detected.

If a `CUTE_TEST` does not trigger any validation during it's execution the test is reported as failed as well, since this usually indicates an error in test implementation.

test runners and reporters
==========================
A test runner using the provided simple IDE reporter would like like this:
```C++
#include "../cute/cute.hpp"
#include "../cute/reporters/reporter_ide.hpp"

CUTE_INIT(); // initialize the cute framework

int main_with_ide_reporter(int argc, char* argv[]) {
    // create the test suite execution context
    auto context = cute::context();

    // print out the status of each test during the execution of the test suite
    context.reporters.emplace_back(cute::reporter_ide);

    // run the test suite
    auto results = context.run();

    // print out the test suite result summary
    cute::reporter_ide_summary(results);

    // exit with an exit code indicating success or failure for the test suite
    return ((results.test_cases_failed > 0) ? EXIT_FAILURE : EXIT_SUCCESS);
}
```

A test runner using the provided `JUnit` reporter would like like this:
```C++
#include "../cute/cute.hpp"
#include "../cute/reporters/reporter_junit.hpp"

CUTE_INIT(); // initialize the cute framework

int main_with_junit_reporter(int argc, char* argv[]) {
    // create the test suite execution context
    auto context = cute::context();

    // run the test suite
    auto results = context.run();

    // print out the test suite result summary in JUnit XML format
    cute::reporter_junit_summary(std::cout, results);

    // exit with an exit code indicating success or failure for the test suite
    return ((results.test_cases_failed > 0) ? EXIT_FAILURE : EXIT_SUCCESS);
}
```

Both reporters could also be combined, e.g., by writing the test results into a separate `JUnit` XML results file.

related work
============
Based on the following ideas and frameworks:
- [lest](https://github.com/martinmoene/lest)
- [catch](https://github.com/philsquared/Catch)
- [turtle](http://turtle.sourceforge.net/)


dependencies
============
- [cmake](http://cmake.org/) (only required for building and executing the self-test unit test suite)
