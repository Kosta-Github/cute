// Copyright 2014 by Konstantin (Kosta) Baumann
//
// Distributed under the MIT license (See accompanying file LICENSE.txt)
//

#pragma once

#include "exception.hpp"
#include "macros.hpp"
#include "test.hpp"
#include "test_registry.hpp"
#include "test_result.hpp"
#include "test_suite_result.hpp"

#include <memory>

namespace cute {

    struct context {
        std::function<void(test_result const& rep)> reporter;
        std::string include_tags;
        std::string exclude_tags;

        inline std::unique_ptr<test_suite_result> run() const {
            return run(detail::test_registry::instance().tests);
        }

        inline std::unique_ptr<test_suite_result> run(
            std::vector<test> const& tests
        ) const {
            auto const time_start_all = detail::time_now();

            auto res = std::unique_ptr<test_suite_result>(new test_suite_result());

            auto const incl_tags = detail::parse_tags(include_tags);
            auto const excl_tags = detail::parse_tags(exclude_tags);

            for(auto&& test : tests) {
                ++res->test_cases;

                if(detail::skip_test(test.tags, incl_tags, excl_tags)) {
                    ++res->test_cases_skipped;
                    continue;
                }

                auto rep = test_result(test.name, true, "", "", test.file, test.line, 0);
                auto const time_start = detail::time_now();

                try {
                    auto const count_start = res->checks_performed.load();

                    --res->checks_performed; // decr by one since CUTE_DETAIL_ASSERT() below will increment it again
                    CUTE_DETAIL_ASSERT(((void)test.test_case(), true), test.file, test.line, "", cute::captures(), cute::captures());

                    auto const count_end = res->checks_performed.load();
                    if(count_start == count_end) {
                        throw cute::detail::exception("no check performed in test case", test.file, test.line, "");
                    }

                    ++res->test_cases_passed;
                } catch(detail::exception const& ex) {
                    rep.pass    = false;
                    rep.file    = ex.file;
                    rep.line    = ex.line;
                    rep.reason  = ex.what();
                    rep.expr    = ex.expr;

                    for(auto&& c : ex.caps.list) {
                        rep.captures.emplace_back(c);
                    }

                    ++res->test_cases_failed;
                }

                auto const time_end = detail::time_now();
                rep.duration_ms = detail::time_diff_ms(time_start, time_end);

                if(reporter) { reporter(rep); }
            }

            auto const time_end_all = detail::time_now();
            res->duration_ms = detail::time_diff_ms(time_start_all, time_end_all);

            return res;
        }

    };

} // namespace cute
