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
            std::vector<capture> captures;

            inline exception(
                std::string msg_,
                std::string file_,
                int line_,
                capture cap1_ = capture(),
                capture cap2_ = capture(),
                capture cap3_ = capture(),
                capture cap4_ = capture(),
                capture cap5_ = capture()
            ) :
                std::runtime_error(std::move(msg_)),
                file(std::move(file_)),
                line(std::move(line_))
            {
                if(!cap1_.name.empty()) { captures.emplace_back(std::move(cap1_)); }
                if(!cap2_.name.empty()) { captures.emplace_back(std::move(cap2_)); }
                if(!cap3_.name.empty()) { captures.emplace_back(std::move(cap3_)); }
                if(!cap4_.name.empty()) { captures.emplace_back(std::move(cap4_)); }
                if(!cap5_.name.empty()) { captures.emplace_back(std::move(cap5_)); }
            }
        };

    } // namespace detail
} // namespace cute
