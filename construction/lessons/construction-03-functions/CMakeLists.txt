cmake_minimum_required(VERSION 3.16)
project(functions_baseline LANGUAGES CXX)

add_executable(functions-baseline code/functions-baseline.cpp)
target_compile_features(functions-baseline PRIVATE cxx_std_20)
target_compile_options(functions-baseline PRIVATE -Wall -Wextra -Werror)
set_target_properties(functions-baseline PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}")
