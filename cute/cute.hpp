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
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

#define CUTE_DETAIL_EXPECT(FILE, LINE, EXPR_EVAL, EXPR_TEXT) \
    try { \
        ++cute::test_suite_result::current().checks_performed; \
        if(!(EXPR_EVAL)) { \
            throw cute::detail::exception( \
                "check failed", FILE, LINE, EXPR_TEXT \
            ); \
        } \
    } catch(cute::detail::exception const&) { \
        throw; \
    } catch(std::exception const &ex) { \
        throw cute::detail::exception( \
            "got an unexpected exception with message \"" + std::string(ex.what()) + "\"", FILE, LINE, EXPR_TEXT \
        ); \
    } catch(...) { \
        throw cute::detail::exception( \
            "got an unexpected exception of unknown type", FILE, LINE, EXPR_TEXT \
        ); \
    }

#define CUTE_EXPECT(EXPR) CUTE_DETAIL_EXPECT(__FILE__, __LINE__, EXPR, #EXPR)
#define CUTE_EXPECT_NO_THROW(EXPR) CUTE_DETAIL_EXPECT(__FILE__, __LINE__, ((void)(EXPR), true), #EXPR)

#define CUTE_EXPECT_THROWS_AS(EXPR, EXCEPT) \
    for(;;) { \
        try { \
            ++cute::test_suite_result::current().checks_performed; \
            (void)(EXPR); \
        } catch(EXCEPT const&) { \
            break; \
        } catch(...) { \
        } \
        throw cute::detail::exception( \
            "didn't get an expected exception of type \"" #EXCEPT "\"", __FILE__, __LINE__, #EXPR \
        ); \
    }

#define CUTE_EXPECT_THROWS(EXPR) CUTE_EXPECT_THROWS_AS(EXPR, std::exception)

#define CUTE_INIT() \
    cute::detail::test_registry& cute::detail::test_registry::instance() { static test_registry reg; return reg; } \
    cute::detail::test_suite_result* cute::detail::test_suite_result::g_current = nullptr;

#define CUTE_DETAIL_UNIQUE_NAME_LINE_2(NAME, LINE) NAME##LINE
#define CUTE_DETAIL_UNIQUE_NAME_LINE(NAME, LINE) CUTE_DETAIL_UNIQUE_NAME_LINE_2(NAME, LINE)
#define CUTE_DETAIL_UNIQUE_NAME(NAME) CUTE_DETAIL_UNIQUE_NAME_LINE(NAME, __LINE__)

#define CUTE_TEST(...) \
    static void CUTE_DETAIL_UNIQUE_NAME(cute_detail_test)(); \
    static bool CUTE_DETAIL_UNIQUE_NAME(cute_detail_test_reg) = \
        cute::detail::test_registry::instance().add(cute::detail::test( \
            __FILE__, __LINE__, __VA_ARGS__, CUTE_DETAIL_UNIQUE_NAME(cute_detail_test) \
        ) \
    ); \
    static void CUTE_DETAIL_UNIQUE_NAME(cute_detail_test)()

namespace cute {

    namespace detail {

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

        struct exception : std::runtime_error {
            std::string const file;
            int const line;
            std::string const expr;

            inline exception(std::string msg_, std::string file_, int line_, std::string expr_) :
                std::runtime_error(std::move(msg_)), file(std::move(file_)), line(std::move(line_)), expr(std::move(expr_))
            { }
        };

        struct test_suite_result {
            std::atomic<std::size_t> test_cases;
            std::atomic<std::size_t> test_cases_passed;
            std::atomic<std::size_t> test_cases_failed;
            std::atomic<std::size_t> test_cases_skipped;
            std::atomic<std::size_t> checks_performed;
            std::size_t              duration_ms; // milliseconds

            inline test_suite_result() :
                test_cases(0), test_cases_passed(0), test_cases_failed(0), test_cases_skipped(0),
                checks_performed(0), duration_ms(0), prev_res(g_current)
            {
                g_current = this;
            }
            inline ~test_suite_result() { assert(g_current == this); g_current = prev_res; }

            static inline test_suite_result& current() { assert(g_current); return *g_current; }

        private:
            static test_suite_result* g_current;
            test_suite_result* const prev_res;
        };

        struct test_registry {
            std::vector<test> tests;
            inline bool add(test t) {
                tests.emplace_back(std::move(t)); return true;
            }

            static test_registry& instance();
        };

        struct test_result {
            std::string test;
            bool pass;
            std::string msg;
            std::string expr;
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

    }

    inline std::string to_string(std::nullptr_t const&)     { return "nullptr"; }
    inline std::string to_string(std::string    const& str) { return ('\"' + str + '\"'); }
    inline std::string to_string(char const*    const& str) { return (str ? ('\"' + std::string(str) + '\"') : "nullptr"); }
    inline std::string to_string(char           const& c)   { return ('\'' + std::string(1, c) + '\''); }
    inline std::string to_string(bool           const& b)   { return (b ? "true" : "false"); }

    template<typename T>
    inline auto to_string(T const& value) -> std::string { std::ostringstream os; os << value; return os.str(); }

    template<typename T>
    inline std::string to_string(std::shared_ptr<T> const& p) { std::ostringstream os; os << p.get(); return os.str(); }

    template<typename T>
    inline std::string to_string(std::unique_ptr<T> const& p) { std::ostringstream os; os << p.get(); return os.str(); }

    template<typename T>
    inline std::string to_string(std::weak_ptr<T> const& p) { std::ostringstream os; os << p.get(); return os.str(); }

    namespace detail {

        struct decomposer {

            template<typename T>
            struct expression {
                const T obj;

                inline expression(T obj_) : obj(std::move(obj_)) { }

                inline operator std::string() const { return to_string(obj); }

                template<typename R> inline std::string operator!() const { std::ostringstream os; os << '!' << to_string(obj); return os.str(); }

                template<typename R> inline std::string operator==(R const& rhs) const { return bin_op_to_string("==", rhs); }
                template<typename R> inline std::string operator!=(R const& rhs) const { return bin_op_to_string("!=", rhs); }
                template<typename R> inline std::string operator< (R const& rhs) const { return bin_op_to_string("<" , rhs); }
                template<typename R> inline std::string operator<=(R const& rhs) const { return bin_op_to_string("<=", rhs); }
                template<typename R> inline std::string operator> (R const& rhs) const { return bin_op_to_string(">" , rhs); }
                template<typename R> inline std::string operator>=(R const& rhs) const { return bin_op_to_string(">=", rhs); }

                template<typename R> inline std::string operator&&(R const& rhs) const { return bin_op_to_string("&&", rhs); }
                template<typename R> inline std::string operator||(R const& rhs) const { return bin_op_to_string("||", rhs); }

                template<typename L>
                inline std::string unary_op_to_string(std::string const& op, L const& lhs) { std::ostringstream os; os << op << ' ' << to_string(lhs); return os.str(); }

                template<typename R>
                inline std::string bin_op_to_string(char const* op, R const& rhs) const {
                    std::ostringstream os; os << to_string(obj) << ' ' << op << ' ' << to_string(rhs); return os.str();
                }
                
            };

            template<typename T>
            inline expression<T const&> operator->*(T const& op) {
                return expression<T const&>(op);
            }
        };











        
    } // namespace detail

    using detail::test;
    using detail::test_result;
    using detail::test_suite_result;

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

    inline std::unique_ptr<test_suite_result> run(
        std::vector<test> const& tests,
        std::function<void(test_result const& rep)> reporter = nullptr,
        std::string const& include_tags = "",
        std::string const& exclude_tags = ""
    ) {
        auto const time_start_all = detail::time_now();

        auto ctx = std::unique_ptr<test_suite_result>(new test_suite_result());

        auto const incl_tags = detail::parse_tags(include_tags);
        auto const excl_tags = detail::parse_tags(exclude_tags);

        for(auto&& test : tests) {
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
                CUTE_DETAIL_EXPECT(test.file, test.line, ((void)test.test_case(), true), "");

                auto const count_end = ctx->checks_performed.load();
                if(count_start == count_end) {
                    throw cute::detail::exception("no check performed in test case", test.file, test.line, "");
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
