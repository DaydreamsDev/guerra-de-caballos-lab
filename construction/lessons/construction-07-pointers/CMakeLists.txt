cmake_minimum_required(VERSION 3.16)
project(pointers_baseline LANGUAGES CXX)

add_executable(pointers-baseline code/pointers-baseline.cpp)
target_compile_features(pointers-baseline PRIVATE cxx_std_20)
target_compile_options(pointers-baseline PRIVATE -Wall -Wextra -Werror)
set_target_properties(pointers-baseline PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}")
