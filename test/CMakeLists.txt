cmake_minimum_required(VERSION 2.8)

project(cute CXX C)
message("configure: cute unit tests")

# check for "noexcept" support
include(CheckCXXSourceCompiles)
CHECK_CXX_SOURCE_COMPILES(
    "struct A { A() noexcept { } }; int main(int argc, const char* argv[]) { return 0; }"
    CUTE_HAS_NOEXCEPT
)
if(CUTE_HAS_NOEXCEPT)
    add_definitions(-DCUTE_HAS_NOEXCEPT)
endif(CUTE_HAS_NOEXCEPT)

set(
    SRC_CUTE
    ../cute/capture.hpp
    ../cute/cleanup_guard.hpp
    ../cute/context.hpp
    ../cute/cute.hpp
    ../cute/exception.hpp
    ../cute/macros.hpp
    ../cute/test.hpp
    ../cute/test_result.hpp
    ../cute/test_suite_result.hpp
    ../cute/tick.hpp
    ../cute/thread.hpp
    ../cute/to_string.hpp
)

set(
    SRC_CUTE_DETAIL
    ../cute/detail/decomposer.hpp
    ../cute/detail/eval_context.hpp
    ../cute/detail/macros_impl.hpp
    ../cute/detail/test_registry.hpp
    ../cute/detail/utils.hpp
    ../cute/detail/utils_file.hpp
)

set(
    SRC_CUTE_REPORTERS
    ../cute/reporters/reporter_dot_progress.hpp
    ../cute/reporters/reporter_ide.hpp
    ../cute/reporters/reporter_junit.hpp
)

set(
    SRC_CUTE_UNITTESTS
    main.cpp
    cute_cleanup_guard_unittests.cpp
    cute_core_unittests.cpp
    cute_decomposer_unittests.cpp
    cute_file_unittests.cpp
    cute_thread_unittests.cpp
    cute_xml_unittests.cpp
)

add_executable(
    cute_unittests
    ${SRC_CUTE_UNITTESTS}
    ${SRC_CUTE}
    ${SRC_CUTE_DETAIL}
    ${SRC_CUTE_REPORTERS}
)

target_link_libraries(
    cute_unittests
    ${CMAKE_THREAD_LIBS_INIT}
)

source_group(cute                    FILES ${SRC_CUTE})
source_group(cute\\detail            FILES ${SRC_CUTE_DETAIL})
source_group(cute\\reporters         FILES ${SRC_CUTE_REPORTERS})
source_group(unittests               FILES ${SRC_CUTE_UNITTESTS})

add_test(
    NAME cute_unittests
    COMMAND cute_unittests
)
