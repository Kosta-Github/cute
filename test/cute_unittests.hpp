// Copyright 2014 by Konstantin (Kosta) Baumann
//
// Distributed under the MIT license (See accompanying file LICENSE.txt)
//

#include "../cute/cute.hpp"

static cute::test const tests[] = {
    { CUTE_TEST_CASE("CUTE_EXPECT(true) should pass", { "pass" }) {
        CUTE_EXPECT(true);
    }},

    { CUTE_TEST_CASE("CUTE_EXPECT(false) should fail", { "fail" }) {
        CUTE_EXPECT(false);
    }},

    { CUTE_TEST_CASE("CUTE_EXPECT_THROWS() should pass if an exception is thrown", { "pass" }) {
        auto throw_an_exception = []() { throw std::runtime_error("forced exception"); };
        CUTE_EXPECT_THROWS(throw_an_exception());
    }},

    { CUTE_TEST_CASE("CUTE_EXPECT_THROWS() should fail if no exception is thrown", { "fail" }) {
        auto throw_no_exception = []() { };
        CUTE_EXPECT_THROWS(throw_no_exception());
    }},

    { CUTE_TEST_CASE("CUTE_EXPECT_THROWS_AS() should pass if the correct exception is thrown", { "pass" }) {
        auto throw_an_exception = []() { throw std::runtime_error("forced exception"); };
        CUTE_EXPECT_THROWS_AS(throw_an_exception(), std::runtime_error);
    }},

    { CUTE_TEST_CASE("CUTE_EXPECT_THROWS_AS() should pass if a derived excpetion is thrown", { "pass" }) {
        auto throw_an_exception = []() { throw std::runtime_error("forced exception"); };
        CUTE_EXPECT_THROWS_AS(throw_an_exception(), std::exception);
    }},

    { CUTE_TEST_CASE("CUTE_EXPECT_THROWS_AS() should fail if an excpetion of an unexpected type is thrown", { "fail" }) {
        auto throw_an_exception = []() { throw std::runtime_error("forced exception"); };
        CUTE_EXPECT_THROWS_AS(throw_an_exception(), std::logic_error);
    }},

    { CUTE_TEST_CASE("CUTE_EXPECT_THROWS_AS() should fail if no excpetion is thrown", { "fail" }) {
        auto throw_no_exception = []() { };
        CUTE_EXPECT_THROWS_AS(throw_no_exception(), std::runtime_error);
    }},

    { CUTE_TEST_CASE("CUTE_EXPECT_NO_THROW() should pass if no exception is thrown", { "pass" }) {
        auto throw_no_exception = []() { };
        CUTE_EXPECT_NO_THROW(throw_no_exception());
    }},

    { CUTE_TEST_CASE("CUTE_EXPECT_NO_THROW() should fail if an exception is thrown", { "fail" }) {
        auto throw_an_exception = []() { throw std::runtime_error("forced exception"); };
        CUTE_EXPECT_NO_THROW(throw_an_exception());
    }},

    { CUTE_TEST_CASE("a test case should fail if an uncaught exception is thrown", { "fail" }) {
        throw std::runtime_error("forced exception");
    }},

    { CUTE_TEST_CASE("a test case should fail if no check is performed in it", { "fail" }) {
        // nothing to do here
    }}
};