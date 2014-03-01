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
#include <set>
#include <stdexcept>
#include <string>

#define CUTE_EXPECT(expr) \
    try { \
        ++CUTE_CONTEXT.checks_performed; \
        if(!(expr)) { \
            throw cute::exception(#expr, "failed: test case", CUTE_LOCATION); \
        } \
    } catch(cute::exception const&) { \
        throw; \
    } catch(std::exception const &ex) { \
        throw cute::exception(#expr, "failed: got unexpected exception with message \"" + std::string(ex.what()) + "\"", CUTE_LOCATION); \
    } catch(...) { \
        throw cute::exception(#expr, "failed: got unexpected exception of unknown type", CUTE_LOCATION); \
    }

#define CUTE_EXPECT_THROWS_AS(expr, excpt) \
    for(;;) { \
        try { \
            ++CUTE_CONTEXT.checks_performed; \
            (void)(expr); \
        } catch(excpt const&) { \
            break; \
        } catch(...) { \
        } \
        throw cute::exception(#expr, "failed: didn't get exception of type \"" #excpt "\"", CUTE_LOCATION); \
    }

#define CUTE_EXPECT_THROWS(expr) CUTE_EXPECT_THROWS_AS(expr, std::exception)
#define CUTE_EXPECT_NO_THROW(expr) CUTE_EXPECT(((void)(expr), true))
#define CUTE_LOCATION cute::location(__FILE__, __LINE__)
#define CUTE_CONTEXT cute::context<void>::current()

namespace cute {

    struct test {
        std::string const name;
        std::set<std::string> const tags;
        std::function<void()> const test_case;

        inline test(std::string name_, std::function<void()> test_case_) : name(std::move(name_)), test_case(std::move(test_case_)) { }
        inline test(std::string name_, std::set<std::string> tags_, std::function<void()> test_case_) : name(std::move(name_)), tags(std::move(tags_)), test_case(std::move(test_case_)) { }
    };

    struct location {
        const std::string file;
        const int line;

        inline location(std::string file_, int line_) : file(std::move(file_)), line(std::move(line_)) { }
    };

    inline std::ostream& operator<<(std::ostream& os, location const& where) {
#if defined(__GNUG__)
        return (os << where.file << ":" << where.line);
#else // defined(__GNUG__)
        return (os << where.file << "(" << where.line << ")");
#endif // defined(__GNUG__)
    }

    struct exception : std::runtime_error {
        const std::string kind;
        const location where;

        inline exception(std::string expr_, std::string kind_, location where_) : std::runtime_error(std::move(expr_)), kind(std::move(kind_)), where(std::move(where_)) { }
    };

    inline void report_command_line(std::ostream& os, exception const& ex, std::string const& test) {
        static std::mutex g_mutex; std::lock_guard<std::mutex> lock(g_mutex);

        os << ex.where << ": " << ex.kind << ": " << test << ": " << ex.what() << std::endl;
    }

    template<typename T>
    struct context {
        std::atomic<std::size_t> test_cases;
        std::atomic<std::size_t> test_cases_failed;
        std::atomic<std::size_t> checks_performed;

        inline context() : prev_ctx(g_current) { g_current = this; }
        inline ~context() { assert(g_current == this); g_current = prev_ctx; }
        static inline context& current() { assert(g_current); return *g_current; }

    private:
        static context* g_current;
        context* const prev_ctx;
    };
    template<typename T>
    context<T>* context<T>::g_current = nullptr;

    template<std::size_t N>
    inline std::size_t run(
        test const (&specifications)[N],
        std::set<std::string> const& tags = std::set<std::string>(),
        std::ostream& os = std::cout
    ) {
        context<void> ctx;

        for(auto&& test : specifications) {
            try {
                ++ctx.test_cases;
                test.test_case();
            } catch(exception const& ex) {
                ++ctx.test_cases_failed;
                report_command_line(os, ex, test.name);
            }
        }

        if(ctx.test_cases_failed > 0) {
            os << ctx.test_cases_failed << " out of " << ctx.test_cases << " tests failed (" << ctx.checks_performed << " checks performed)." << std::endl;
        } else {
            os << "All " << ctx.test_cases << " tests passed (" << ctx.checks_performed << " checks performed)." << std::endl;
        }

        return ctx.test_cases_failed;
    }

} // namespace cute
