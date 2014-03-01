// Copyright 2014 by Konstantin (Kosta) Baumann
//
// Distributed under the MIT license (See accompanying file LICENSE.txt)
//

#include "cute_unittests.hpp"

int main(int argc, char* argv[]) {
    auto pass_ctx = cute::run(tests, std::cout, { "pass" }, { "fail" });
    auto fail_ctx = cute::run(tests, std::cout, { "fail" }, { "pass" });

    if(pass_ctx->test_cases_failed > 0) { return EXIT_FAILURE; }
    if(fail_ctx->test_cases_failed < fail_ctx->test_cases) { return EXIT_FAILURE; }
    return EXIT_SUCCESS;
}
