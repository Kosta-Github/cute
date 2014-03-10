// Copyright 2014 by Konstantin (Kosta) Baumann
//
// Distributed under the MIT license (See accompanying file LICENSE.txt)
//

#pragma once

#include <stdexcept>
#include <string>

namespace cute {
    namespace detail {

        struct exception : std::runtime_error {
            std::string const file;
            int const line;
            std::string const expr;

            inline exception(std::string msg_, std::string file_, int line_, std::string expr_) :
                std::runtime_error(std::move(msg_)), file(std::move(file_)), line(std::move(line_)), expr(std::move(expr_))
            { }
        };

    } // namespace detail
} // namespace cute
