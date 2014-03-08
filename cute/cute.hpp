// Copyright 2014 by Konstantin (Kosta) Baumann
//
// Distributed under the MIT license (See accompanying file LICENSE.txt)
//

#pragma once

#include <atomic>
#include <cassert>
#include <chrono>
#include <cstddef>
#include <cstdint>
#include <functional>
#include <iostream>
#include <memory>
#include <mutex>
#include <set>
#include <stdexcept>
#include <string>

#define CUTE_EXPECT_IMPL(EXPR_EVAL, EXPR_TEXT, FILE, LINE) \
    try { \
        ++cute::test_suite_result::current().checks_performed; \
        if(!(EXPR_EVAL)) { \
            throw cute::detail::exception("check failed", EXPR_TEXT, FILE, LINE); \
        } \
    } catch(cute::detail::exception const&) { \
        throw; \
    } catch(std::exception const &ex) { \
        throw cute::detail::exception( \
            "got an unexpected exception with message \"" + std::string(ex.what()) + "\"", EXPR_TEXT, FILE, LINE \
        ); \
    } catch(...) { \
        throw cute::detail::exception("got an unexpected exception of unknown type", EXPR_TEXT, FILE, LINE); \
    }

#define CUTE_EXPECT(EXPR) CUTE_EXPECT_IMPL(EXPR, #EXPR, __FILE__, __LINE__)
#define CUTE_EXPECT_NO_THROW(EXPR) CUTE_EXPECT_IMPL(((void)(EXPR), true), #EXPR, __FILE__, __LINE__)

#define CUTE_EXPECT_THROWS_AS(expr, excpt) \
    for(;;) { \
        try { \
            ++cute::test_suite_result::current().checks_performed; \
            (void)(expr); \
        } catch(excpt const&) { \
            break; \
        } catch(...) { \
        } \
        throw cute::detail::exception( \
            "didn't get an expected exception of type \"" #excpt "\"", #expr, __FILE__, __LINE__ \
        ); \
    }

#define CUTE_EXPECT_THROWS(expr) CUTE_EXPECT_THROWS_AS(expr, std::exception)
#define CUTE_TEST_CASE(...) __FILE__, __LINE__, __VA_ARGS__, []()

namespace cute {

    namespace detail {

        struct exception : std::runtime_error {
            std::string const expr;
            std::string const file;
            int const line;

            inline exception(std::string msg_, std::string expr_, std::string file_, int line_) :
                std::runtime_error(std::move(msg_)), expr(std::move(expr_)), file(std::move(file_)), line(std::move(line_))
            { }
        };

        template<typename T>
        struct test_suite_result {
            std::atomic<std::size_t> test_cases;
            std::atomic<std::size_t> test_cases_passed;
            std::atomic<std::size_t> test_cases_failed;
            std::atomic<std::size_t> test_cases_skipped;
            std::atomic<std::size_t> checks_performed;
            std::size_t              duration_ms; // milliseconds

            inline test_suite_result() :
                test_cases(0), test_cases_passed(0), test_cases_failed(0), test_cases_skipped(0),
                checks_performed(0), duration_ms(0), prev_ctx(g_current)
            {
                g_current = this;
            }
            inline ~test_suite_result() { assert(g_current == this); g_current = prev_ctx; }

            static inline test_suite_result& current() { assert(g_current); return *g_current; }

        private:
            static test_suite_result* g_current;
            test_suite_result* const prev_ctx;
        };
        template<typename T>
        test_suite_result<T>* test_suite_result<T>::g_current = nullptr;

        inline auto trim(std::string const& s) -> std::string {
            auto start = 0;
            while(s[start] == ' ') { ++start; }
            if(start >= s.size()) { return ""; }
            auto end = s.size() - 1;
            while(s[end] == ' ') { --end; }
            return s.substr(start, end - start + 1);
        }

        inline auto parse_tags(std::string const& tags) -> std::set<std::string> {
            std::set<std::string> res;

            auto start = std::string::size_type(0);
            while(start < tags.size()) {
                auto end = tags.find(',', start);
                auto tag = trim(tags.substr(start, end - start));
                if(!tag.empty()) { res.insert(std::move(tag)); }
                start = ((end < tags.npos) ? end + 1 : tags.npos);
            }

            return res;
        }

        inline bool skip_test(
            std::set<std::string> const& test_tags,
            std::set<std::string> const& include_tags,
            std::set<std::string> const& exclude_tags
        ) {
            if(test_tags.empty()) { return false; }

            for(auto&& include : include_tags) {
                if(test_tags.find(include) == test_tags.end()) { return true; }
            }

            for(auto&& exclude : exclude_tags) {
                if(test_tags.find(exclude) != test_tags.end()) { return true; }
            }

            return false;
        }

        inline auto time_now() -> decltype(std::chrono::high_resolution_clock::now()) {
            return std::chrono::high_resolution_clock::now();
        }

        template<typename T>
        inline std::int64_t time_diff_ms(T&& start, T&& end) {
            auto diff = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
            return static_cast<std::int64_t>(diff);
        }

    } // namespace detail

    typedef detail::test_suite_result<void> test_suite_result;

    struct test {
        std::string const name;
        std::set<std::string> const tags;
        std::function<void()> const test_case;
        std::string const file;
        int const line;

        inline test(std::string file_, int line_, std::string name_, std::function<void()> test_case_) :
            name(std::move(name_)), test_case(std::move(test_case_)), file(std::move(file_)), line(std::move(line_))
        { }
        inline test(std::string file_, int line_, std::string name_, std::string const& tags_, std::function<void()> test_case_) :
            name(std::move(name_)), tags(detail::parse_tags(tags_)), test_case(std::move(test_case_)), file(std::move(file_)), line(std::move(line_))
        { }
    };
    
    struct test_result {
        std::string test;
        bool pass;
        std::string msg;
        std::string expr;
        std::string file;
        int line;
        std::int64_t duration_ms;

        inline test_result(std::string test_ = "", bool pass_ = true, std::string msg_ = "", std::string expr_ = "",
            std::string file_ = "", int line_ = 0, std::size_t duration_ms_ = 0
        ) :
            test(std::move(test_)), pass(std::move(pass_)), msg(std::move(msg_)), expr(std::move(expr)),
            file(std::move(file_)), line(std::move(line_)), duration_ms(std::move(duration_ms_))
        { }
    };

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

    template<std::size_t N>
    inline std::unique_ptr<test_suite_result> run(
        test const (&specifications)[N],
        std::function<void(test_result const& rep)> reporter = nullptr,
        std::string const& include_tags = "",
        std::string const& exclude_tags = ""
    ) {
        auto const time_start_all = detail::time_now();

        auto ctx = std::unique_ptr<test_suite_result>(new test_suite_result());

        auto const incl_tags = detail::parse_tags(include_tags);
        auto const excl_tags = detail::parse_tags(exclude_tags);

        for(auto&& test : specifications) {
            ++ctx->test_cases;

            if(detail::skip_test(test.tags, incl_tags, excl_tags)) {
                ++ctx->test_cases_skipped;
                continue;
            }

            auto rep = test_result(test.name, true, "", "", test.file, test.line, 0);
            auto const time_start = detail::time_now();

            try {
                auto const count_start = ctx->checks_performed.load();

                --ctx->checks_performed; // decr by one since CUTE_EXPECT_IMPL() below will increment it again
                CUTE_EXPECT_IMPL(((void)test.test_case(), true), "", test.file, test.line);

                auto const count_end = ctx->checks_performed.load();
                if(count_start == count_end) {
                    throw cute::detail::exception("no check performed in test case", "", test.file, test.line);
                }

                ++ctx->test_cases_passed;
            } catch(detail::exception const& ex) {
                rep.pass = false;
                rep.file = ex.file;
                rep.line = ex.line;
                rep.msg  = ex.what();
                rep.expr = ex.expr;

                ++ctx->test_cases_failed;
            }

            auto const time_end = detail::time_now();
            rep.duration_ms = detail::time_diff_ms(time_start, time_end);

            if(reporter) { reporter(rep); }
        }

        auto const time_end_all = detail::time_now();
        ctx->duration_ms = detail::time_diff_ms(time_start_all, time_end_all);

        return ctx;
    }

} // namespace cute
