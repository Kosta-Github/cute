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

    auto inverse_reporter = [](cute::report const& rep) {
        auto r = rep; r.pass = !r.pass;
        cute::command_line_reporter(r.pass ? std::cout : std::cerr, r);
    };
    auto fail_ctx = cute::run(tests, inverse_reporter, { "fail" });

    // for this unit test in which we check the correct behavior for
    // passing and failing test cases we need to combine both results
    // by inverting the "failed" cases...
    auto tests_all = pass_ctx->test_cases.load();
    auto tests_passed = pass_ctx->test_cases_passed + fail_ctx->test_cases_failed;
    auto tests_failed = pass_ctx->test_cases_failed + fail_ctx->test_cases_passed;
    auto tests_skipped = pass_ctx->test_cases_skipped + fail_ctx->test_cases_skipped - tests_all;
    auto checks_performed = pass_ctx->checks_performed + fail_ctx->checks_performed;
    auto duration_ms = pass_ctx->duration_ms + fail_ctx->duration_ms;

    auto self_test_failed = ((tests_failed > 0) || (tests_skipped > 0));

    auto& out = (self_test_failed ? std::cerr : std::cout);

    out << std::endl;
    out << "test summary:" << std::endl;
    out << tests_passed << " out of " << tests_all << " tests passed." << std::endl;
    out << tests_failed << " out of " << tests_all << " tests failed." << std::endl;
    out << tests_skipped << " out of " << tests_all << " tests skipped." << std::endl;
    out << checks_performed << " checks performed." << std::endl;
    out << duration_ms << " ms used for the whole test suite." << std::endl;

    return (self_test_failed ? EXIT_FAILURE : EXIT_SUCCESS);
}
