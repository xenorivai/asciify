cmake_minimum_required(VERSION 3.16.3)
project(asciify LANGUAGES CXX)

include_directories(include)
add_executable(main main.cpp)

set_property(
	TARGET main
	APPEND
	PROPERTY ADDITIONAL_MAKE_CLEAN_FILES out.txt out.jpg
)