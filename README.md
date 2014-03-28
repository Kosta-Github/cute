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


related work
============
Based on the following ideas and frameworks:
- [lest](https://github.com/martinmoene/lest)
- [catch](https://github.com/philsquared/Catch)
- [turtle](http://turtle.sourceforge.net/)


dependencies
============
- [cmake](http://cmake.org/) (only required for building and executing the self-test unit test suite)
