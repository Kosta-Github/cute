// Copyright 2014 by Konstantin (Kosta) Baumann
//
// Distributed under the MIT license (See accompanying file LICENSE.txt)
//

#include "../cute/cute.hpp"

#include <fstream>

CUTE_TEST("detail::create_temp_folder() should create a new folder", "pass") {
    auto folder = cute::temp_folder();
    CUTE_ASSERT(!folder.empty(), CUTE_CAPTURE(folder));
}

CUTE_TEST("the temp folder should be deleted after a test case", "pass") {
    auto folder = cute::temp_folder();

    // create some files in the temp folder
    std::ofstream(folder + "file_1");
    std::ofstream(folder + "file_2");
    std::ofstream(folder + "file_3");

    // we cannot have a test case without an actual check/assert in it => add a dummy check
    CUTE_ASSERT(true);
}

CUTE_TEST("a test should fail if the temp folder could not be deleted afterwards", "fail") {
    auto folder = cute::temp_folder();

    // static => keep a file lock around even after the test
    // in order to verify that the test fails if the temp folder
    // could not completely cleaned up again
    static auto hold_me = std::make_shared<std::ofstream>(folder + "file_1");

    // we cannot have a test case without an actual check/assert in it => add a dummy check
    CUTE_ASSERT(true);
}
