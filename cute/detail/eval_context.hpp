// Copyright 2014 by Konstantin (Kosta) Baumann
//
// Distributed under the MIT license (See accompanying file LICENSE.txt)
//

#pragma once

#include "exception.hpp"
#include "../test.hpp"
#include "../test_result.hpp"
#include "../test_suite_result.hpp"
#include "../utils_file.hpp"

#include <vector>

namespace cute {
    namespace detail {

        struct eval_context {
            eval_context() :
                reporters(nullptr), current_test(nullptr),
                test_cases(0), test_cases_passed(0),
                test_cases_failed(0), test_cases_skipped(0),
                checks_performed(0), duration_ms(0),
                m_previous(g_current)
            {
                g_current = this;
            }

            ~eval_context() {
                assert(g_current == this);
                g_current = m_previous;
            }

            std::vector<std::function<void(test_result const& rep)>> const* reporters;

            test const* current_test;

            std::atomic<std::size_t> test_cases;
            std::atomic<std::size_t> test_cases_passed;
            std::atomic<std::size_t> test_cases_failed;
            std::atomic<std::size_t> test_cases_skipped;
            std::atomic<std::size_t> checks_performed;
            std::atomic<std::size_t> duration_ms;

            std::mutex exceptions_mutex;
            std::vector<exception> exceptions;
            inline void add_exception(exception ex, bool throw_exception = true) {
                std::lock_guard<std::mutex> lock(exceptions_mutex);
                exceptions.emplace_back(std::move(ex));
                if(throw_exception) {
                    throw exceptions.back();
                }
            }

            std::mutex temp_folder_mutex;
            std::string temp_folder;
            inline std::string create_temp_folder() {
                std::lock_guard<std::mutex> lock(temp_folder_mutex);
                if(!temp_folder.empty()) { return temp_folder; }
                temp_folder = cute::detail::create_temp_folder();
                return temp_folder;
            }
            inline bool delete_temp_folder() {
                std::lock_guard<std::mutex> lock(temp_folder_mutex);
                if(temp_folder.empty()) { return true; }
                auto res = cute::detail::delete_folder(temp_folder);
                temp_folder.clear();
                return res;
            }

            inline test_suite_result result() const {
                return test_suite_result(test_cases, test_cases_passed, test_cases_failed, test_cases_skipped, checks_performed, duration_ms);
            }

            static eval_context* g_current;
            eval_context* const m_previous;
            inline static eval_context& current() { assert(g_current); return *g_current; }
        };

    } // namespace detail
} // namespace cute
