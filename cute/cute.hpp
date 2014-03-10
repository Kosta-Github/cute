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

} // namespace cute
