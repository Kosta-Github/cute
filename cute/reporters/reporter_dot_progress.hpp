// Copyright 2014 by Konstantin (Kosta) Baumann
//
// Distributed under the MIT license (See accompanying file LICENSE.txt)
//

#pragma once

#include "reporter_ide.hpp"

#include <iostream>

namespace cute {

    inline void reporter_dot_progress_to_stream(
        std::ostream& out,
        test_result const& res,
        std::size_t test_index_cur,
        std::size_t test_index_max
    ) {
        switch(res.result) {
            case result_type::pass:     out << "."; return;
            case result_type::skip:     out << "s"; return;
            case result_type::fail:     out << "f"; return;
            case result_type::fatal:    out << "F"; return;
            default:                    assert(false); return;
        }
    }

    inline void reporter_dot_progress(
        test_result const& res,
        std::size_t test_index_cur,
        std::size_t test_index_max
    ) {
        reporter_dot_progress_to_stream(std::cout, res, test_index_cur, test_index_max);
    }

    inline void reporter_dot_progress_summary_to_stream(
        std::ostream& out,
        test_suite_result const& results
    ) {
        for(size_t i = 0, iEnd = results.test_results.size(); i < iEnd; ++i) {
            auto&& test = results.test_results[i];
            switch(test.result) {
                case result_type::pass:
                case result_type::skip:     break;
                case result_type::fail:
                case result_type::fatal:    reporter_ide_to_stream(out, test, i, iEnd); break;
                default:                    assert(false);
            }
        }

        reporter_ide_summary_to_stream(out, results);
    }

    inline void reporter_dot_progress_summary(
        test_suite_result const& results
    ) {
        reporter_dot_progress_summary_to_stream((results.test_cases_failed > 0) ? std::cout : std::cerr, results);
    }

} // namespace cute
