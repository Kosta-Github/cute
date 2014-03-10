cmake_minimum_required(VERSION 2.8)

project(cute CXX)
message("configure: cute unit tests")

add_executable(
    cute_unittests
    main.cpp
    cute_unittests.cpp
    ../cute/context.hpp
    ../cute/cute.hpp
    ../cute/decomposer.hpp
    ../cute/exception.hpp
    ../cute/macros.hpp
    ../cute/test.hpp
    ../cute/test_registry.hpp
    ../cute/test_result.hpp
    ../cute/test_suite_result.hpp
    ../cute/to_string.hpp
    ../cute/utils.hpp
)
     
add_test(
    NAME cute_unittests
    COMMAND cute_unittests
)
