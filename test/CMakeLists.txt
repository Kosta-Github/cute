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

add_executable(
    cute_unittests
    main.cpp
    cute_cleanup_guard_unittests.cpp
    cute_core_unittests.cpp
    cute_decomposer_unittests.cpp
    cute_file_unittests.cpp
    cute_thread_unittests.cpp
    cute_xml_unittests.cpp
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
    ../cute/detail/decomposer.hpp
    ../cute/detail/eval_context.hpp
    ../cute/detail/macros_impl.hpp
    ../cute/detail/test_registry.hpp
    ../cute/detail/utils.hpp
    ../cute/detail/utils_file.hpp
    ../cute/reporters/reporter_dot_progress.hpp
    ../cute/reporters/reporter_ide.hpp
    ../cute/reporters/reporter_junit.hpp
)

target_link_libraries(
    cute_unittests
	${CMAKE_THREAD_LIBS_INIT}
)
     
add_test(
    NAME cute_unittests
    COMMAND cute_unittests
)
