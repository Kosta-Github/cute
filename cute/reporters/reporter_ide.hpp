// Copyright 2014 by Konstantin (Kosta) Baumann
//
// Distributed under the MIT license (See accompanying file LICENSE.txt)
//

#pragma once

#include "../test_result.hpp"

#include <ostream>

namespace cute {

    inline void reporter_ide(std::ostream& os, test_result const& res) {
        auto header = res.file;
#if defined(__GNUG__)
        header += ":" + std::to_string(res.line) + ": ";
#else // defined(__GNUG__)
        header += "(" + std::to_string(res.line) + "): ";
#endif // defined(__GNUG__)
        switch(res.result) {
            case result_type::pass:     header += "pass: ";  break;
            case result_type::fail:     header += "error: "; break;
            case result_type::fatal:    header += "fatal: "; break;
            default:                    assert(false);
        }

        os << header << res.test << std::endl;

        if(res.result != result_type::pass) {
            if(!res.reason.empty()) { os << header << "    reason:     "    << res.reason   << std::endl; }
            if(!res.expr.empty())   { os << header << "    expression: "    << res.expr     << std::endl; }

            for(auto&& c : res.captures) {
                os << header << "    with:       " << c.name;
                if(c.name != c.value) { os << " => " << c.value; }
                os << std::endl;
            }
        }

        os << header << "    duration:   "    << res.duration_ms << " ms" << std::endl;
    }

} // namespace cute
