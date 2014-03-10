// Copyright 2014 by Konstantin (Kosta) Baumann
//
// Distributed under the MIT license (See accompanying file LICENSE.txt)
//

#pragma once

#include <atomic>
#include <cassert>

namespace cute {

    struct test_suite_result {
        std::atomic<std::size_t> test_cases;
        std::atomic<std::size_t> test_cases_passed;
        std::atomic<std::size_t> test_cases_failed;
        std::atomic<std::size_t> test_cases_skipped;
        std::atomic<std::size_t> checks_performed;
        std::size_t              duration_ms; // milliseconds

        inline test_suite_result() :
            test_cases(0), test_cases_passed(0), test_cases_failed(0), test_cases_skipped(0),
            checks_performed(0), duration_ms(0), prev_res(g_current)
        {
            g_current = this;
        }
        inline ~test_suite_result() { assert(g_current == this); g_current = prev_res; }

        static inline test_suite_result& current() { assert(g_current); return *g_current; }

    private:
        static test_suite_result* g_current;
        test_suite_result* const prev_res;
    };

} // namespace cute
