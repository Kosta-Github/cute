// Copyright 2014 by Konstantin (Kosta) Baumann
//
// Distributed under the MIT license (See accompanying file LICENSE.txt)
//

#include "cute_unittests.hpp"

static void console_reporter(std::string const& test, bool pass, std::string const& file, int line, std::string const& msg, std::string const& expr, std::size_t duration_ms) {
    cute::command_line_reporter(pass ? std::cout : std::cerr, test, pass, file, line, msg, expr, duration_ms);
}

int main(int argc, char* argv[]) {
    auto pass_ctx = cute::run(tests, &console_reporter, { "pass" }, { "fail" });
    if(pass_ctx->test_cases_failed > 0) {
        std::cerr << pass_ctx->test_cases_failed << " out of " << pass_ctx->test_cases << " tests failed (" << pass_ctx->checks_performed << " checks performed)." << std::endl;
    } else {
        std::cout << "All " << pass_ctx->test_cases << " tests passed (" << pass_ctx->checks_performed << " checks performed)." << std::endl;
    }

    auto fail_ctx = cute::run(tests, &console_reporter, { "fail" }, { "pass" });
    if(fail_ctx->test_cases_failed > 0) {
        std::cerr << fail_ctx->test_cases_failed << " out of " << fail_ctx->test_cases << " tests failed (" << fail_ctx->checks_performed << " checks performed)." << std::endl;
    } else {
        std::cout << "All " << fail_ctx->test_cases << " tests passed (" << fail_ctx->checks_performed << " checks performed)." << std::endl;
    }

    if(pass_ctx->test_cases_failed > 0) { return EXIT_FAILURE; }
    if(fail_ctx->test_cases_failed < fail_ctx->test_cases) { return EXIT_FAILURE; }
    return EXIT_SUCCESS;
}
