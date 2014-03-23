// Copyright 2014 by Konstantin (Kosta) Baumann
//
// Distributed under the MIT license (See accompanying file LICENSE.txt)
//

#pragma once

#include "detail/utils_file.hpp"

#include <atomic>
#include <cassert>
#include <mutex>

namespace cute {

    struct test_suite_result {
        std::size_t const test_cases;
        std::size_t const test_cases_passed;
        std::size_t const test_cases_failed;
        std::size_t const test_cases_skipped;
        std::size_t const checks_performed;
        std::size_t const duration_ms; // milliseconds
        std::vector<test_result> const test_results;

        inline test_suite_result(
            std::size_t test_cases_,
            std::size_t test_cases_passed_,
            std::size_t test_cases_failed_,
            std::size_t test_cases_skipped_,
            std::size_t checks_performed_,
            std::size_t duration_ms_,
            std::vector<test_result> test_results_
        ) :
            test_cases(test_cases_), test_cases_passed(test_cases_passed_),
            test_cases_failed(test_cases_failed_), test_cases_skipped(test_cases_skipped_),
            checks_performed(checks_performed_), duration_ms(duration_ms_),
            test_results(std::move(test_results_))
        {
        }
    };

} // namespace cute
