cmake_minimum_required(VERSION 3.16)
project(non_structured_baseline LANGUAGES CXX)

add_executable(non-structured-baseline code/non-structured-baseline.cpp)
target_compile_features(non-structured-baseline PRIVATE cxx_std_20)
target_compile_options(non-structured-baseline PRIVATE -Wall -Wextra -Werror)
set_target_properties(non-structured-baseline PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}")
