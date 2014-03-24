// Copyright 2014 by Konstantin (Kosta) Baumann
//
// Distributed under the MIT license (See accompanying file LICENSE.txt)
//

#pragma once

#include "../test_result.hpp"

#include <ostream>

namespace cute {

    namespace detail {

        inline std::string ide_make_file_line_string(
            std::string file,
            int line
        ) {
#if defined(__GNUG__)
            file += ":" + std::to_string(line) + ": ";
#else // defined(__GNUG__)
            file += "(" + std::to_string(line) + "): ";
#endif // defined(__GNUG__)
            return file;
        }

    } // namespace detail

    inline void reporter_ide(
        std::ostream& os,
        test_result const& res,
        std::size_t test_index_cur,
        std::size_t test_index_max
    ) {
        auto type = std::string();
        switch(res.result) {
            case result_type::pass:     type = "pass: ";  break;
            case result_type::skip:     type = "skip: ";  break;
            case result_type::fail:     type = "error: "; break;
            case result_type::fatal:    type = "fatal: "; break;
            default:                    assert(false);
        }

        auto test_header = detail::ide_make_file_line_string(res.test.file, res.test.line) + type;
        os << test_header << res.test.name << " [" << (test_index_cur + 1) << "/" << test_index_max << "]" << std::endl;

        if(res.result != result_type::skip) {
            os << test_header << "    duration:   " << res.duration_ms << " ms" << std::endl;
        }

        if(auto ex = res.excp.get()) {
            auto ex_header = detail::ide_make_file_line_string(ex->file, ex->line) + type;

            os << ex_header << "    reason:     " << ex->what() << std::endl;
            if(!ex->expr.empty()) { os << ex_header << "    expression: " << ex->expr << std::endl; }

            for(auto&& c : ex->captures.list) {
                os << ex_header << "    with:       " << c.name;
                if(c.name != c.value) { os << " => " << c.value; }
                os << std::endl;
            }
        }
    }

} // namespace cute
