// Copyright 2014 by Konstantin (Kosta) Baumann
//
// Distributed under the MIT license (See accompanying file LICENSE.txt)
//

#include "../cute/cute.hpp"

CUTE_TEST("a passed check in a separate thread is detected", "pass") {
    auto t = cute::thread([&]() { CUTE_ASSERT(true); });
    t.join();
}

CUTE_TEST("a failed check in a separate thread is detected", "fail") {
    auto t = cute::thread([&]() { CUTE_ASSERT(false); });
    t.join();
}
