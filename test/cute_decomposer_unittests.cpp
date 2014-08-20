// Copyright 2014 by Konstantin (Kosta) Baumann
//
// Distributed under the MIT license (See accompanying file LICENSE.txt)
//

#include "../cute/cute.hpp"

CUTE_TEST("test decomposer for operator ==", "pass") {
    auto a = 1;
    auto b = 2;
    auto decomp = CUTE_DETAIL_DECOMPOSE(a == b);
    CUTE_ASSERT(decomp == "1 == 2", CUTE_CAPTURE(decomp));
}

CUTE_TEST("test decomposer for operator !=", "pass") {
    auto a = 1;
    auto b = 2;
    auto decomp = CUTE_DETAIL_DECOMPOSE(a != b);
    CUTE_ASSERT(decomp == "1 != 2", CUTE_CAPTURE(decomp));
}

CUTE_TEST("test decomposer for operator <", "pass") {
    auto a = 1;
    auto b = 2;
    auto decomp = CUTE_DETAIL_DECOMPOSE(a < b);
    CUTE_ASSERT(decomp == "1 < 2", CUTE_CAPTURE(decomp));
}

CUTE_TEST("test decomposer for operator >", "pass") {
    auto a = 1;
    auto b = 2;
    auto decomp = CUTE_DETAIL_DECOMPOSE(a > b);
    CUTE_ASSERT(decomp == "1 > 2", CUTE_CAPTURE(decomp));
}

CUTE_TEST("test decomposer for operator <=", "pass") {
    auto a = 1;
    auto b = 2;
    auto decomp = CUTE_DETAIL_DECOMPOSE(a <= b);
    CUTE_ASSERT(decomp == "1 <= 2", CUTE_CAPTURE(decomp));
}

CUTE_TEST("test decomposer for operator >=", "pass") {
    auto a = 1;
    auto b = 2;
    auto decomp = CUTE_DETAIL_DECOMPOSE(a >= b);
    CUTE_ASSERT(decomp == "1 >= 2", CUTE_CAPTURE(decomp));
}

CUTE_TEST("test decomposer for operator &&", "pass") {
    auto a = 1;
    auto b = 2;
    auto decomp = CUTE_DETAIL_DECOMPOSE(a && b);
    CUTE_ASSERT(decomp == "1 && 2", CUTE_CAPTURE(decomp));
}

CUTE_TEST("test decomposer for operator ||", "pass") {
    auto a = 1;
    auto b = 2;
    auto decomp = CUTE_DETAIL_DECOMPOSE(a || b);
    CUTE_ASSERT(decomp == "1 || 2", CUTE_CAPTURE(decomp));
}
