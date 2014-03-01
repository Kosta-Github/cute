// Copyright 2014 by Konstantin (Kosta) Baumann
//
// Distributed under the MIT license (See accompanying file LICENSE.txt)
//

#pragma once

#include <atomic>
#include <cassert>
#include <cstddef>
#include <functional>
#include <iostream>
#include <mutex>
#include <set>
#include <stdexcept>
#include <string>

#define CUTE_EXPECT(expr) \
    try { \
        ++cute::context::current().checks_performed; \
        if(!(expr)) { \
            throw cute::detail::exception(#expr, "failed: test case", CUTE_LOCATION); \
        } \
    } catch(cute::detail::exception const&) { \
        throw; \
    } catch(std::exception const &ex) { \
        throw cute::detail::exception(#expr, "failed: got unexpected exception with message \"" + std::string(ex.what()) + "\"", CUTE_LOCATION); \
    } catch(...) { \
        throw cute::detail::exception(#expr, "failed: got unexpected exception of unknown type", CUTE_LOCATION); \
    }

#define CUTE_EXPECT_THROWS_AS(expr, excpt) \
    for(;;) { \
        try { \
            ++cute::context::current().checks_performed; \
            (void)(expr); \
        } catch(excpt const&) { \
            break; \
        } catch(...) { \
        } \
        throw cute::detail::exception(#expr, "failed: didn't get exception of type \"" #excpt "\"", CUTE_LOCATION); \
    }

#define CUTE_EXPECT_THROWS(expr) CUTE_EXPECT_THROWS_AS(expr, std::exception)
#define CUTE_EXPECT_NO_THROW(expr) CUTE_EXPECT(((void)(expr), true))
#define CUTE_LOCATION cute::location(__FILE__, __LINE__)
#define CUTE_TEST_CASE(...) CUTE_LOCATION, __VA_ARGS__, []()
namespace cute {

    struct location {
        const std::string file;
        const int line;

        inline location(std::string file_, int line_) : file(std::move(file_)), line(std::move(line_)) { }
    };

    inline std::ostream& operator<<(std::ostream& os, location const& loc) {
#if defined(__GNUG__)
        return (os << loc.file << ":" << loc.line);
#else // defined(__GNUG__)
        return (os << loc.file << "(" << loc.line << ")");
#endif // defined(__GNUG__)
    }

    namespace detail {

        struct exception : std::runtime_error {
            const std::string kind;
            const location loc;

            inline exception(std::string expr_, std::string kind_, location loc_) : std::runtime_error(std::move(expr_)), kind(std::move(kind_)), loc(std::move(loc_)) { }
        };

        template<typename T>
        struct context_impl {
            std::atomic<std::size_t> test_cases;
            std::atomic<std::size_t> test_cases_failed;
            std::atomic<std::size_t> checks_performed;

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
        location const loc;
        std::string const name;
        std::set<std::string> const tags;
        std::function<void()> const test_case;

        inline test(location loc_, std::string name_, std::function<void()> test_case_) : loc(std::move(loc_)), name(std::move(name_)), test_case(std::move(test_case_)) { }
        inline test(location loc_, std::string name_, std::set<std::string> tags_, std::function<void()> test_case_) : loc(std::move(loc_)), name(std::move(name_)), tags(std::move(tags_)), test_case(std::move(test_case_)) { }
    };
    
    inline void report_command_line(std::ostream& os, detail::exception const& ex, std::string const& test) {
        static std::mutex g_mutex; std::lock_guard<std::mutex> lock(g_mutex);

        os << ex.loc << ": " << ex.kind << ": " << test << ": " << ex.what() << std::endl;
    }
    
    template<std::size_t N>
    inline std::unique_ptr<cute::context> run(
        test const (&specifications)[N],
        std::ostream& os = std::cout,
        std::set<std::string> const& include_tags = std::set<std::string>(),
        std::set<std::string> const& exclude_tags = std::set<std::string>()
    ) {
        auto ctx = std::unique_ptr<context>(new context());

        for(auto&& test : specifications) {
            if(detail::skip_test(test.tags, include_tags, exclude_tags)) {
                continue;
            }

            try {
                ++ctx->test_cases;
                auto const start_count = ctx->checks_performed.load();
                --ctx->checks_performed; // since CUTE_EXPECT_NO_THROWS() below increases the counter as well
                CUTE_EXPECT_NO_THROW(test.test_case());
                auto const end_count = ctx->checks_performed.load();

                std::cout << start_count << " / " << end_count << std::endl;

                if(start_count == end_count) {
                    throw cute::detail::exception("", "failed: no check performed in test case", test.loc);
                }
            } catch(detail::exception const& ex) {
                ++ctx->test_cases_failed;
                report_command_line(os, ex, test.name);
            }
        }

        if(ctx->test_cases_failed > 0) {
            os << ctx->test_cases_failed << " out of " << ctx->test_cases << " tests failed (" << ctx->checks_performed << " checks performed)." << std::endl;
        } else {
            os << "All " << ctx->test_cases << " tests passed (" << ctx->checks_performed << " checks performed)." << std::endl;
        }

        return ctx;
    }

} // namespace cute
