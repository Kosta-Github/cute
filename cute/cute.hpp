// Copyright 2014 by Konstantin (Kosta) Baumann
//
// Distributed under the MIT license (See accompanying file LICENSE.txt)
//

#pragma once

#include "decomposer.hpp"
#include "exception.hpp"
#include "macros.hpp"
#include "test.hpp"
#include "test_registry.hpp"
#include "test_result.hpp"
#include "test_suite_result.hpp"
#include "utils.hpp"

#include <iostream>
#include <mutex>

namespace cute {

    inline std::ostream& command_line_reporter(std::ostream& os, test_result const& res) {
        static std::mutex g_mutex; std::lock_guard<std::mutex> lock(g_mutex);

#if defined(__GNUG__)
        os << res.file << ":" << res.line << ": ";
#else // defined(__GNUG__)
        os << res.file << "(" << res.line << "): ";
#endif // defined(__GNUG__)
        os << (res.pass ? "pass" : "error") << ": ";
        if(!res.msg.empty()) { os << res.msg << ": "; }
        os << res.test;
        if(!res.expr.empty()) { os << ": \'" << res.expr << "\'"; }
        os << " [duration: " << res.duration_ms << " ms]";
        os << std::endl;

        return os;
    }

    inline std::unique_ptr<test_suite_result> run(
        std::vector<test> const& tests,
        std::function<void(test_result const& rep)> reporter = nullptr,
        std::string const& include_tags = "",
        std::string const& exclude_tags = ""
    ) {
        auto const time_start_all = detail::time_now();

        auto ctx = std::unique_ptr<test_suite_result>(new test_suite_result());

        auto const incl_tags = detail::parse_tags(include_tags);
        auto const excl_tags = detail::parse_tags(exclude_tags);

        for(auto&& test : tests) {
            ++ctx->test_cases;

            if(detail::skip_test(test.tags, incl_tags, excl_tags)) {
                ++ctx->test_cases_skipped;
                continue;
            }

            auto rep = test_result(test.name, true, "", "", test.file, test.line, 0);
            auto const time_start = detail::time_now();

            try {
                auto const count_start = ctx->checks_performed.load();

                --ctx->checks_performed; // decr by one since CUTE_EXPECT_IMPL() below will increment it again
                CUTE_DETAIL_EXPECT(test.file, test.line, ((void)test.test_case(), true), "");

                auto const count_end = ctx->checks_performed.load();
                if(count_start == count_end) {
                    throw cute::detail::exception("no check performed in test case", test.file, test.line, "");
                }

                ++ctx->test_cases_passed;
            } catch(detail::exception const& ex) {
                rep.pass = false;
                rep.file = ex.file;
                rep.line = ex.line;
                rep.msg  = ex.what();
                rep.expr = ex.expr;

                ++ctx->test_cases_failed;
            }

            auto const time_end = detail::time_now();
            rep.duration_ms = detail::time_diff_ms(time_start, time_end);

            if(reporter) { reporter(rep); }
        }

        auto const time_end_all = detail::time_now();
        ctx->duration_ms = detail::time_diff_ms(time_start_all, time_end_all);

        return ctx;
    }

} // namespace cute
