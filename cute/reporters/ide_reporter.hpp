// Copyright 2014 by Konstantin (Kosta) Baumann
//
// Distributed under the MIT license (See accompanying file LICENSE.txt)
//

#pragma once

#include "../test_result.hpp"

#include <iostream>
#include <mutex>

namespace cute {

    inline std::ostream& ide_reporter(std::ostream& os, test_result const& res) {
        static std::mutex g_mutex; std::lock_guard<std::mutex> lock(g_mutex);

        auto header = res.file;
#if defined(__GNUG__)
        header += ":" + std::to_string(res.line) + ": ";
#else // defined(__GNUG__)
        header += "(" + std::to_string(res.line) + "): ";
#endif // defined(__GNUG__)
        header += (res.pass ? "pass: " : "error: ");

        os << header << res.test << " [duration: " << res.duration_ms << " ms]" << std::endl;

        if(res.pass) {
            if(!res.reason.empty()) { os << header << "    reason:     "    << res.reason   << std::endl; }
            if(!res.expr.empty())   { os << header << "    expression: "    << res.expr     << std::endl; }

            for(auto&& c : res.captures) {
                os << header << "    with:       " << c.name;
                if(!c.value.empty()) { os << " => " << c.value; }
                os << std::endl;
            }
        }

        return os;
    }

} // namespace cute
