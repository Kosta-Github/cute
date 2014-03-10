// Copyright 2014 by Konstantin (Kosta) Baumann
//
// Distributed under the MIT license (See accompanying file LICENSE.txt)
//

#pragma once

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
    cute::test_suite_result* cute::test_suite_result::g_current = nullptr;

#define CUTE_DETAIL_UNIQUE_NAME_LINE_2(NAME, LINE) NAME##LINE
#define CUTE_DETAIL_UNIQUE_NAME_LINE(NAME, LINE) CUTE_DETAIL_UNIQUE_NAME_LINE_2(NAME, LINE)
#define CUTE_DETAIL_UNIQUE_NAME(NAME) CUTE_DETAIL_UNIQUE_NAME_LINE(NAME, __LINE__)

#define CUTE_TEST(...) \
    static void CUTE_DETAIL_UNIQUE_NAME(cute_detail_test)(); \
    static bool CUTE_DETAIL_UNIQUE_NAME(cute_detail_test_reg) = \
        cute::detail::test_registry::instance().add(cute::test( \
            __FILE__, __LINE__, __VA_ARGS__, CUTE_DETAIL_UNIQUE_NAME(cute_detail_test) \
        ) \
    ); \
    static void CUTE_DETAIL_UNIQUE_NAME(cute_detail_test)()

#define CUTE_DECOMPOSE(EXPR) static_cast<std::string>(cute::detail::decomposer()->* EXPR)
