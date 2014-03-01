// Copyright 2014 by Konstantin (Kosta) Baumann
//
// Distributed under the MIT license (See accompanying file LICENSE.txt)
//

#include "../cute/cute.hpp"

static const cute::test tests[] = {
    {
        "CUTE_EXPECT(true) should pass", []() {
            CUTE_EXPECT(true);
        }
    },
    {
        "CUTE_EXPECT(false) should fail (failure expected)", []() {
            CUTE_EXPECT(false);
        }
    },
    {
        "create a test case with no tags", []() {
            CUTE_EXPECT(true);
        }
    },
    {
        "create a test case with a single tag", { "tag" }, []() {
            CUTE_EXPECT(true);
        }
    },
    {
        "create a test case with several tags", { "tag 1", "tag 2", "tag 3" }, []() {
            CUTE_EXPECT(true);
        }
    },
    {
        "CUTE_EXPECT_THROWS() should pass if an exception is thrown", []() {
            auto throw_an_exception = []() { throw std::runtime_error("forced exception"); };
            CUTE_EXPECT_THROWS(throw_an_exception());
        }
    },
    {
        "CUTE_EXPECT_THROWS() should fail if no exception is thrown (failure expected)", []() {
            auto throw_no_exception = []() { };
            CUTE_EXPECT_THROWS(throw_no_exception);
        }
    },
    {
        "CUTE_EXPECT_THROWS_AS() should pass if the correct exception is thrown", []() {
            auto throw_an_exception = []() { throw std::runtime_error("forced exception"); };
            CUTE_EXPECT_THROWS_AS(throw_an_exception(), std::runtime_error);
        }
    },
    {
        "CUTE_EXPECT_THROWS_AS() should pass if a derived excpetion is thrown", []() {
            auto throw_an_exception = []() { throw std::runtime_error("forced exception"); };
            CUTE_EXPECT_THROWS_AS(throw_an_exception(), std::exception);
        }
    },
    {
        "CUTE_EXPECT_THROWS_AS() should fail if an excpetion of an unexpected type is thrown (failure expected)", []() {
            auto throw_an_exception = []() { throw std::runtime_error("forced exception"); };
            CUTE_EXPECT_THROWS_AS(throw_an_exception(), std::logic_error);
        }
    },
    {
        "CUTE_EXPECT_THROWS_AS() should fail if no excpetion is thrown (failure expected)", []() {
            auto throw_no_exception = []() { };
            CUTE_EXPECT_THROWS_AS(throw_no_exception(), std::runtime_error);
        }
    },
    {
        "CUTE_EXPECT_NO_THROW() should pass if no exception is thrown", []() {
            auto throw_no_exception = []() { };
            CUTE_EXPECT_NO_THROW(throw_no_exception());
        }
    },
    {
        "CUTE_EXPECT_NO_THROW() should fail if an exception is thrown (failure expected)", []() {
            auto throw_an_exception = []() { throw std::runtime_error("forced exception"); };
            CUTE_EXPECT_NO_THROW(throw_an_exception());
        }
    }
};
