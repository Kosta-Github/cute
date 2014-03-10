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
    namespace detail {

        struct exception : std::runtime_error {
            std::string const file;
            int const line;
            captures const caps;

            inline exception(
                std::string msg_,
                std::string file_,
                int line_,
                captures caps1_ = captures(),
                captures caps2_ = captures()
            ) :
                std::runtime_error(std::move(msg_)),
                file(std::move(file_)),
                line(std::move(line_)),
                caps(std::move(caps1_), std::move(caps2_))
            { }
        };

    } // namespace detail
} // namespace cute
