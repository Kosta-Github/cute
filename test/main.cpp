// Copyright 2014 by Konstantin (Kosta) Baumann
//
// Distributed under the MIT license (See accompanying file LICENSE.txt)
//

#include "cute_unittests.hpp"

int main(int argc, char* argv[]) {
    auto normal_reporter = [](cute::report const& rep) {
        cute::command_line_reporter(rep.pass ? std::cout : std::cerr, rep);
    };
    auto pass_ctx = cute::run(tests, normal_reporter, { "pass" });
    if(pass_ctx->test_cases_failed > 0) {
        std::cerr << pass_ctx->test_cases_failed << " out of " << pass_ctx->test_cases << " tests failed (" << pass_ctx->checks_performed << " checks performed)." << std::endl;
    } else {
        std::cout << "All " << pass_ctx->test_cases << " tests passed (" << pass_ctx->checks_performed << " checks performed)." << std::endl;
    }

    auto inverse_reporter = [](cute::report const& rep) {
        auto r = rep; r.pass = !r.pass;
        cute::command_line_reporter(r.pass ? std::cout : std::cerr, r);
    };
    auto fail_ctx = cute::run(tests, inverse_reporter, { "fail" });
    if(fail_ctx->test_cases_failed > 0) {
        std::cerr << fail_ctx->test_cases_failed << " out of " << fail_ctx->test_cases << " tests failed (" << fail_ctx->checks_performed << " checks performed)." << std::endl;
    } else {
        std::cout << "All " << fail_ctx->test_cases << " tests passed (" << fail_ctx->checks_performed << " checks performed)." << std::endl;
    }

    if(pass_ctx->test_cases_failed > 0) { return EXIT_FAILURE; }
    if(fail_ctx->test_cases_failed < fail_ctx->test_cases) { return EXIT_FAILURE; }
    return EXIT_SUCCESS;
}
