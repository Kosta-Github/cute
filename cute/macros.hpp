// Copyright 2014 by Konstantin (Kosta) Baumann
//
// Distributed under the MIT license (See accompanying file LICENSE.txt)
//

#pragma once

#define CUTE_DETAIL_UNIQUE_NAME_LINE_2(NAME, LINE) NAME##LINE
#define CUTE_DETAIL_UNIQUE_NAME_LINE(NAME, LINE) CUTE_DETAIL_UNIQUE_NAME_LINE_2(NAME, LINE)
#define CUTE_DETAIL_UNIQUE_NAME(NAME) CUTE_DETAIL_UNIQUE_NAME_LINE(NAME, __LINE__)

#define CUTE_DETAIL_ASSERT(FILE, LINE, EXPR_EVAL, ...) \
    try { \
        ++cute::test_suite_result::current().checks_performed; \
        if(!(EXPR_EVAL)) { \
            throw cute::detail::exception( \
                "test failed", FILE, LINE, __VA_ARGS__ \
            ); \
        } \
    } catch(cute::detail::exception const&) { \
        throw; \
    } catch(std::exception const &ex) { \
        throw cute::detail::exception( \
            "got an unexpected exception with message \"" + std::string(ex.what()) + "\"", FILE, LINE, __VA_ARGS__ \
        ); \
    } catch(...) { \
        throw cute::detail::exception( \
            "got an unexpected exception of unknown type", FILE, LINE, __VA_ARGS__ \
        ); \
    }

#define CUTE_DETAIL_ASSERT_THROWS_AS(EXPR, EXCEPT) \
    for(;;) { \
        try { \
            ++cute::test_suite_result::current().checks_performed; \
            (void)(EXPR); \
        } catch(EXCEPT const&) { \
            break; \
        } catch(...) { \
        } \
        throw cute::detail::exception( \
            "didn't get an expected exception of type \"" #EXCEPT "\"", __FILE__, __LINE__, cute::capture(#EXPR, "") \
        ); \
    }

#define CUTE_DETAIL_INIT() \
    cute::detail::test_registry& cute::detail::test_registry::instance() { static test_registry reg; return reg; } \
    cute::test_suite_result* cute::test_suite_result::g_current = nullptr;


#define CUTE_DETAIL_TEST(...) \
    static void CUTE_DETAIL_UNIQUE_NAME(cute_detail_test)(); \
    static bool CUTE_DETAIL_UNIQUE_NAME(cute_detail_test_reg) = \
        cute::detail::test_registry::instance().add(cute::test( \
            __FILE__, __LINE__, CUTE_DETAIL_UNIQUE_NAME(cute_detail_test), __VA_ARGS__ \
        ) \
    ); \
    static void CUTE_DETAIL_UNIQUE_NAME(cute_detail_test)()

#define CUTE_DETAIL_DECOMPOSE(EXPR) \
    static_cast<std::string>(cute::detail::decomposer()->* EXPR)

#define CUTE_ASSERT(EXPR, ...)              CUTE_DETAIL_ASSERT(__FILE__, __LINE__, EXPR,                 CUTE_CAPTURE(EXPR),     __VA_ARGS__)
#define CUTE_ASSERT_THROWS_NOT(EXPR)        CUTE_DETAIL_ASSERT(__FILE__, __LINE__, ((void)(EXPR), true), cute::capture(#EXPR, ""))
#define CUTE_ASSERT_THROWS(EXPR)            CUTE_DETAIL_ASSERT_THROWS_AS(EXPR, std::exception)
#define CUTE_ASSERT_THROWS_AS(EXPR, EXCEPT) CUTE_DETAIL_ASSERT_THROWS_AS(EXPR, EXCEPT)

#define CUTE_CAPTURE(EXPR) cute::capture(#EXPR, CUTE_DETAIL_DECOMPOSE(EXPR))

#define CUTE_INIT() CUTE_DETAIL_INIT()

#define CUTE_TEST(...) CUTE_DETAIL_TEST(__VA_ARGS__)
