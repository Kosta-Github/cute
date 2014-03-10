// Copyright 2014 by Konstantin (Kosta) Baumann
//
// Distributed under the MIT license (See accompanying file LICENSE.txt)
//

#pragma once

#include <string>

namespace cute {

    struct capture {
        std::string const name;
        std::string const value;

        inline capture(std::string name_ = "", std::string value_ = "") : name(std::move(name_)), value(std::move(value_)) { }
    };

} // namespace cute
