// Copyright 2014 by Konstantin (Kosta) Baumann
//
// Distributed under the MIT license (See accompanying file LICENSE.txt)
//

#include "../cute/cute.hpp"
#include "../cute/cleanup_guard.hpp"

CUTE_TEST("CUTE_ASSERT(true) should pass", "pass") {
    CUTE_ASSERT(true);
}

CUTE_TEST("CUTE_ASSERT(false) should fail", "fail") {
    CUTE_ASSERT(false);
}

CUTE_TEST("CUTE_ASSERT_THROWS() should pass if an exception is thrown", "pass") {
    auto throw_an_exception = []() { throw std::runtime_error("forced exception"); };
    CUTE_ASSERT_THROWS(throw_an_exception());
}

CUTE_TEST("CUTE_ASSERT_THROWS() should fail if no exception is thrown", "fail") {
    auto throw_no_exception = []() { };
    CUTE_ASSERT_THROWS(throw_no_exception());
}

CUTE_TEST("CUTE_ASSERT_THROWS_AS() should pass if the correct exception is thrown", "pass") {
    auto throw_an_exception = []() { throw std::runtime_error("forced exception"); };
    CUTE_ASSERT_THROWS_AS(throw_an_exception(), std::runtime_error);
}

CUTE_TEST("CUTE_ASSERT_THROWS_AS() should pass if a derived excpetion is thrown", "pass") {
    auto throw_an_exception = []() { throw std::runtime_error("forced exception"); };
    CUTE_ASSERT_THROWS_AS(throw_an_exception(), std::exception);
}

CUTE_TEST("CUTE_ASSERT_THROWS_AS() should fail if an excpetion of an unexpected type is thrown", "fail") {
    auto throw_an_exception = []() { throw std::runtime_error("forced exception"); };
    CUTE_ASSERT_THROWS_AS(throw_an_exception(), std::logic_error);
}

CUTE_TEST("CUTE_ASSERT_THROWS_AS() should fail if no excpetion is thrown", "fail") {
    auto throw_no_exception = []() { };
    CUTE_ASSERT_THROWS_AS(throw_no_exception(), std::runtime_error);
}

CUTE_TEST("CUTE_ASSERT_THROWS_NOT() should pass if no exception is thrown", "pass") {
    auto throw_no_exception = []() { };
    CUTE_ASSERT_THROWS_NOT(throw_no_exception());
}

CUTE_TEST("CUTE_ASSERT_THROWS_NOT() should fail if an exception is thrown", "fail") {
    auto throw_an_exception = []() { throw std::runtime_error("forced exception"); };
    CUTE_ASSERT_THROWS_NOT(throw_an_exception());
}

CUTE_TEST("a test case should fail if an uncaught exception is thrown", "fail") {
    throw std::runtime_error("forced exception");
}

CUTE_TEST("a test case should fail if no check is performed in it", "fail") {
    // nothing to do here
}

CUTE_TEST("CUTE_ASSERT() can capture more than one variable with the help of CUTE_CAPTURE()", "fail") {
    auto str1 = std::string("hello");
    auto str2 = std::string("world");
    CUTE_ASSERT(str1 == str2, CUTE_CAPTURE(str1), CUTE_CAPTURE(str2));
}

CUTE_TEST("create a cleanup_guard without a callback/cleanup action", "pass") {
    auto cleanup = cute::cleanup_guard(nullptr);
    CUTE_ASSERT(true);
}

CUTE_TEST("create a cleanup_guard and execute the action at scope exit", "pass") {
    auto value = int(0);

    {
        auto cleanup = cute::cleanup_guard([&] { value = 42; });
    }

    CUTE_ASSERT(value == 42);
}

CUTE_TEST("create a cleanup_guard and execute the action in case of an exception at scope exit", "pass") {
    auto value = int(0);
    auto const exception_message = std::string("forced exception");

    try {
        auto cleanup = cute::cleanup_guard([&] { value = 42; });
        throw std::runtime_error(exception_message);
        cleanup.dismiss();
    } catch(std::runtime_error const& ex) {
        // ensure that only the expected exception has been thrown
        CUTE_ASSERT(ex.what() == exception_message);
    }

    CUTE_ASSERT(value == 42);
}

CUTE_TEST("create a cleanup_guard and do not execute after calling dismiss()", "pass") {
    auto value = int(0);

    {
        auto cleanup = cute::cleanup_guard([&] { value = 42; });
        cleanup.dismiss();
    }

    CUTE_ASSERT(value == 0);
}

CUTE_TEST("create a cleanup_guard and execute the cleanup action explicitly", "pass") {
    auto value = int(0);

    {
        auto cleanup = cute::cleanup_guard([&] { value = 42; });
        CUTE_ASSERT(value == 0);
        cleanup.cleanup();
        CUTE_ASSERT(value == 42);
        value = 0;
    }
    
    // check that the cleanup action was not triggered a second time
    CUTE_ASSERT(value == 0);
}
