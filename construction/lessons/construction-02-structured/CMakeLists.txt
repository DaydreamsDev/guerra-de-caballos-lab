cmake_minimum_required(VERSION 3.16)
project(structured_baseline LANGUAGES CXX)

add_executable(structured-baseline code/structured-baseline.cpp)
target_compile_features(structured-baseline PRIVATE cxx_std_20)
target_compile_options(structured-baseline PRIVATE -Wall -Wextra -Werror)
set_target_properties(structured-baseline PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}")
