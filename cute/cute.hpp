// Copyright 2014 by Konstantin (Kosta) Baumann
//
// Distributed under the MIT license (See accompanying file LICENSE.txt)
//

#pragma once

#include <atomic>
#include <cassert>
#include <chrono>
#include <cstddef>
#include <functional>
#include <iostream>
#include <memory>
#include <mutex>
#include <set>
#include <stdexcept>
#include <string>

#define CUTE_EXPECT_IMPL(EXPR_EVAL, EXPR_TEXT, FILE, LINE) \
    try { \
        ++cute::context::current().checks_performed; \
        if(!(EXPR_EVAL)) { \
            throw cute::detail::exception("check failed", EXPR_TEXT, FILE, LINE); \
        } \
    } catch(cute::detail::exception const&) { \
        throw; \
    } catch(std::exception const &ex) { \
        throw cute::detail::exception("got an unexpected exception with message \"" + std::string(ex.what()) + "\"", EXPR_TEXT, FILE, LINE); \
    } catch(...) { \
        throw cute::detail::exception("got an unexpected exception of unknown type", EXPR_TEXT, FILE, LINE); \
    }

#define CUTE_EXPECT(EXPR) CUTE_EXPECT_IMPL(EXPR, #EXPR, __FILE__, __LINE__)
#define CUTE_EXPECT_NO_THROW(EXPR) CUTE_EXPECT_IMPL(((void)(EXPR), true), #EXPR, __FILE__, __LINE__)

#define CUTE_EXPECT_THROWS_AS(expr, excpt) \
    for(;;) { \
        try { \
            ++cute::context::current().checks_performed; \
            (void)(expr); \
        } catch(excpt const&) { \
            break; \
        } catch(...) { \
        } \
        throw cute::detail::exception("didn't get an expected exception of type \"" #excpt "\"", #expr, __FILE__, __LINE__); \
    }

#define CUTE_EXPECT_THROWS(expr) CUTE_EXPECT_THROWS_AS(expr, std::exception)
#define CUTE_TEST_CASE(...) __FILE__, __LINE__, __VA_ARGS__, []()

namespace cute {

    namespace detail {

        struct exception : std::runtime_error {
            std::string const expr;
            std::string const file;
            int const line;

            inline exception(std::string msg_, std::string expr_, std::string file_, int line_) : std::runtime_error(std::move(msg_)), expr(std::move(expr_)), file(std::move(file_)), line(std::move(line_)) { }
        };

        template<typename T>
        struct context_impl {
            std::atomic<std::size_t> test_cases;
            std::atomic<std::size_t> test_cases_failed;
            std::atomic<std::size_t> checks_performed;
            std::size_t              duration_ms; // milliseconds

            inline context_impl() : test_cases(0), test_cases_failed(0), checks_performed(0), prev_ctx(g_current) { g_current = this; }
            inline ~context_impl() { assert(g_current == this); g_current = prev_ctx; }

            static inline context_impl& current() { assert(g_current); return *g_current; }

        private:
            static context_impl* g_current;
            context_impl* const prev_ctx;
        };
        template<typename T>
        context_impl<T>* context_impl<T>::g_current = nullptr;

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

    } // namespace detail

    typedef detail::context_impl<void> context;

    struct test {
        std::string const file;
        int const line;
        std::string const name;
        std::set<std::string> const tags;
        std::function<void()> const test_case;

        inline test(std::string file_, int line_, std::string name_, std::function<void()> test_case_) : file(std::move(file_)), line(std::move(line_)), name(std::move(name_)), test_case(std::move(test_case_)) { }
        inline test(std::string file_, int line_, std::string name_, std::set<std::string> tags_, std::function<void()> test_case_) : file(std::move(file_)), line(std::move(line_)), name(std::move(name_)), tags(std::move(tags_)), test_case(std::move(test_case_)) { }
    };
    
    inline std::ostream& command_line_reporter(std::ostream& os, std::string const& test, bool pass, std::string const& file, int line, std::string const& msg, std::string const& expr, std::size_t duration_ms) {
        static std::mutex g_mutex; std::lock_guard<std::mutex> lock(g_mutex);

#if defined(__GNUG__)
        os << file << ":" << line << ": ";
#else // defined(__GNUG__)
        os << file << "(" << line << "): ");
#endif // defined(__GNUG__)
        os << (pass ? "pass" : "error") << ": ";
        if(!msg.empty()) { os << msg << ": "; }
        os << test;
        if(!expr.empty()) { os << ": \'" << expr << "\'"; }
        os << " [duration: " << duration_ms << " ms]";
        os << std::endl;

        return os;
    }

    typedef std::function<void(std::string const& test, bool pass, std::string const& file, int line, std::string const& msg, std::string const& expr, std::size_t duration_ms)> reporter;

    template<std::size_t N>
    inline std::unique_ptr<cute::context> run(
        test const (&specifications)[N],
        reporter rep,
        std::set<std::string> const& include_tags = std::set<std::string>(),
        std::set<std::string> const& exclude_tags = std::set<std::string>()
    ) {
        auto ctx = std::unique_ptr<context>(new context());

        auto time_start_all = std::chrono::high_resolution_clock::now();

        for(auto&& test : specifications) {
            if(detail::skip_test(test.tags, include_tags, exclude_tags)) {
                continue;
            }

            auto pass = true;
            auto file = test.file;
            auto line = test.line;
            auto msg  = std::string();
            auto expr = std::string();
            auto time_start = std::chrono::high_resolution_clock::now();

            try {
                ++ctx->test_cases;
                auto const count_start = ctx->checks_performed.load();

                --ctx->checks_performed; // decr by one since CUTE_EXPECT_IMPL() below will increment it again
                CUTE_EXPECT_IMPL(((void)test.test_case(), true), "", test.file, test.line);

                auto const count_end = ctx->checks_performed.load();
                if(count_start == count_end) {
                    throw cute::detail::exception("no check performed in test case", "", test.file, test.line);
                }
            } catch(detail::exception const& ex) {
                ++ctx->test_cases_failed;

                pass = false;
                file = ex.file;
                line = ex.line;
                msg  = ex.what();
                expr = ex.expr;
            }

            auto time_end = std::chrono::high_resolution_clock::now();
            auto duration_ms = std::chrono::duration_cast<std::chrono::milliseconds>(time_end - time_start).count();

            if(rep) { rep(test.name, pass, file, line, msg, expr, duration_ms); }
        }

        auto time_end_all = std::chrono::high_resolution_clock::now();
        auto duration_all_ms = std::chrono::duration_cast<std::chrono::milliseconds>(time_end_all - time_start_all).count();
        ctx->duration_ms = static_cast<decltype(ctx->duration_ms)>(duration_all_ms);

        return ctx;
    }

} // namespace cute
