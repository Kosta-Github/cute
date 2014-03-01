cmake_minimum_required(VERSION 2.8)

project(cute CXX)
message("configure: cute unit tests")

add_executable(
	cute_unittests
	main.cpp
	cute_unittests.hpp
    ../cute/cute.hpp
)
	 
add_test(
	NAME cute_unittests
	COMMAND cute_unittests
)
