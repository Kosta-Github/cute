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

        auto header = res.file;
#if defined(__GNUG__)
        header += ":" + std::to_string(res.line) + ": ";
#else // defined(__GNUG__)
        header += "(" + std::to_string(res.line) + "): ";
#endif // defined(__GNUG__)
        header += (res.pass ? "pass: " : "error: ");

        os << header;
        if(!res.msg.empty()) { os << res.msg << ": "; }
        os << res.test;
        if(!res.expr.empty()) { os << ": \'" << res.expr << "\'"; }
        os << " [duration: " << res.duration_ms << " ms]";
        os << std::endl;

        for(auto&& c : res.captures) {
            os << header << "    with: " << c.name;
            if(!c.value.empty()) { os << " => " << c.value; }
            os << std::endl;
        }

        return os;
    }

} // namespace cute
