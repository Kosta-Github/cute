// Copyright 2014 by Konstantin (Kosta) Baumann
//
// Distributed under the MIT license (See accompanying file LICENSE.txt)
//

#pragma once

#include "capture.hpp"

#include <stdexcept>
#include <string>
#include <vector>

namespace cute {

    struct exception : std::runtime_error {
        std::string const file;
        int const line;
        std::string const expr;
        cute::captures const captures;

        inline exception(
            std::string msg_,
            std::string file_,
            int line_,
            std::string expr_,
            cute::captures caps1_ = cute::captures(),
            cute::captures caps2_ = cute::captures()
        ) :
            std::runtime_error(std::move(msg_)),
            file(std::move(file_)),
            line(std::move(line_)),
            expr(std::move(expr_)),
            captures(std::move(caps1_), std::move(caps2_))
        { }
    };

} // namespace cute
