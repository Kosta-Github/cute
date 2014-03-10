// Copyright 2014 by Konstantin (Kosta) Baumann
//
// Distributed under the MIT license (See accompanying file LICENSE.txt)
//

#pragma once

#include <cstdint>
#include <string>

namespace cute {

    struct test_result {
        std::string test;
        bool pass;
        std::string msg;
        std::string expr;
        std::vector<std::pair<std::string, std::string>> expansions;
        std::string file;
        int line;
        std::int64_t duration_ms;

        inline test_result(
            std::string test_ = "", bool pass_ = true, std::string msg_ = "", std::string expr_ = "",
            std::string file_ = "", int line_ = 0, std::size_t duration_ms_ = 0
        ) :
            test(std::move(test_)), pass(std::move(pass_)), msg(std::move(msg_)), expr(std::move(expr)),
            file(std::move(file_)), line(std::move(line_)), duration_ms(std::move(duration_ms_))
        { }
    };

} // namespace cute
