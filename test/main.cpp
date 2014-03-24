// Copyright 2014 by Konstantin (Kosta) Baumann
//
// Distributed under the MIT license (See accompanying file LICENSE.txt)
//

#include "../cute/cute.hpp"
#include "../cute/reporters/reporter_ide.hpp"
#include "../cute/reporters/reporter_junit.hpp"

#include <iostream>

CUTE_INIT();

int main(int argc, char* argv[]) {
    auto pass_ctx = cute::context();
    pass_ctx.include_tags = "pass";
    pass_ctx.reporters.emplace_back([&](cute::test_result const& res, std::size_t test_index_cur, std::size_t test_index_max) {
        cute::reporter_ide(std::cout, res, test_index_cur, test_index_max);
    });
    auto pass_res = pass_ctx.run();

    auto fail_ctx = cute::context();
    fail_ctx.include_tags = "fail";
    fail_ctx.reporters.emplace_back([&](cute::test_result const& res, std::size_t test_index_cur, std::size_t test_index_max) {
        auto r = res;
        switch(r.result) {
            case cute::result_type::pass:   r.result = cute::result_type::fail; break;
            case cute::result_type::skip:   break;
            case cute::result_type::fail:   r.result = cute::result_type::pass; break;
            case cute::result_type::fatal:  break;
            default:                        assert(false);
        }
        cute::reporter_ide(std::cout, r, test_index_cur, test_index_max);
    });
    auto fail_res = fail_ctx.run();

    // for this unit test in which we check the correct behavior for
    // passing and failing test cases we need to combine both results
    // by inverting the "failed" cases...
    auto tests_all          = pass_res.test_results.size();
    auto tests_passed       = pass_res.test_cases_passed   + fail_res.test_cases_failed;
    auto tests_skipped      = pass_res.test_cases_skipped  + fail_res.test_cases_skipped - tests_all;
    auto tests_failed       = pass_res.test_cases_failed   + fail_res.test_cases_passed;
    auto checks_performed   = pass_res.checks_performed    + fail_res.checks_performed;
    auto duration_ms        = pass_res.duration_ms         + fail_res.duration_ms;

    auto self_test_failed = ((tests_all == 0) || (tests_failed > 0) || (tests_skipped > 0));

    auto& out = (self_test_failed ? std::cerr : std::cout);

    out << std::endl;
    out << "test summary:" << std::endl;
    out << tests_passed     << " out of " << tests_all << " tests passed."  << std::endl;
    out << tests_skipped    << " out of " << tests_all << " tests skipped." << std::endl;
    out << tests_failed     << " out of " << tests_all << " tests failed."  << std::endl;
    out << checks_performed << " checks performed."                         << std::endl;
    out << duration_ms      << " ms used for the whole test suite."         << std::endl;

    cute::reporter_junit(std::cout, pass_res);
    cute::reporter_junit(std::cout, fail_res);

    return (self_test_failed ? EXIT_FAILURE : EXIT_SUCCESS);
}
