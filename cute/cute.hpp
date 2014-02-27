// Copyright 2014 by Konstantin (Kosta) Baumann
//
// Distributed under the MIT license (See accompanying file LICENSE)

#pragma once

#include <cstddef>
#include <functional>
#include <iostream>
#include <stdexcept>
#include <string>

#define CUTE_EXPECT(expr) \
    try { if(!(expr))               { throw cute::failure(CUTE_LOCATION, #expr); } } \
    catch(cute::exception const&)   { throw; } \
    catch(std::exception const &ex) { throw cute::exception(#expr, "failed: got unexpected exception with message \"" + std::string(ex.what()) + "\"", CUTE_LOCATION); } \
    catch(...)                      { throw cute::exception(#expr, "failed: got unexpected exception of unknown type", CUTE_LOCATION); }

#define CUTE_EXPECT_THROWS_AS(expr, excpt) \
    for(;;) { \
        try { (expr); } catch(excpt const&) { break; } catch(...) { } \
        throw cute::exception(#expr, "failed: didn't get exception of type \"" #except "\"", CUTE_LOCATION); \
    }

#define CUTE_EXPECT_THROWS(expr) CUTE_EXPECT_THROWS_AS(expr, std::exception)
#define CUTE_EXPECT_NO_THROW(expr) CUTE_EXPECT((expr, true))

#define CUTE_LOCATION cute::location(__FILE__, __LINE__)

namespace cute {

struct test {
    const std::string name;
    const std::set<std::string> tags;
    const std::function<void()> behaviour;
};

struct location {
    const char* const file;
    const int line;
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

    exception(std::string expr_, std::string kind_, location where_) :
        std::runtime_error(std::move(expr_)), kind(std::move(kind_)), where(std::move(where_)))
    { }
};

inline void report(std::ostream& os, message const& e, std::string const& test) {
    os << e.where << ": " << e.kind << ": " << test << ": " << e.what() << std::endl;
}

template<std::size_t N>
std::size_t run(
    test const (&specifications)[N],
    std::set<std::string> const& tags,
    std::ostream& os = std::cout
) {
    std::size_t failures = 0;

    for(auto&& test : specifications) {
        try {
            test.behaviour();
        } catch(exception const& ex) {
            ++failures;
            report(os, ex, test.name);
        }
    }

    if(failures > 0) {
        os << failures << " out of " << N << " " << "tests failed." << std::endl;
    } else {
    	os << "All " << N << " tests passed." << std::endl;
    }

    return failures;
}

} // namespace cute
