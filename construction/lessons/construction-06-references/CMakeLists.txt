cmake_minimum_required(VERSION 3.16)
project(references_baseline LANGUAGES CXX)

add_executable(references-baseline code/references-baseline.cpp)
target_compile_features(references-baseline PRIVATE cxx_std_20)
target_compile_options(references-baseline PRIVATE -Wall -Wextra -Werror)
set_target_properties(references-baseline PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}")
