// Copyright 2014 by Konstantin (Kosta) Baumann
//
// Distributed under the MIT license (See accompanying file LICENSE.txt)
//

#pragma once

#include "capture.hpp"

#include <cstdint>
#include <string>
#include <vector>

namespace cute {

    enum class result_type {
        pass,
        fail,
        fatal
    };

    struct test_result {
        std::string test;
        result_type result;
        std::string reason;
        std::string expr;
        std::vector<capture> captures;
        std::string file;
        int line;
        std::int64_t duration_ms;

        inline test_result(
            std::string test_ = "", result_type result_ = result_type::pass, std::string reason_ = "", std::string expr_ = "",
            std::string file_ = "", int line_ = 0, std::size_t duration_ms_ = 0
        ) :
            test(std::move(test_)), result(std::move(result_)), reason(std::move(reason_)), expr(std::move(expr)),
            file(std::move(file_)), line(std::move(line_)), duration_ms(std::move(duration_ms_))
        { }
    };

} // namespace cute
