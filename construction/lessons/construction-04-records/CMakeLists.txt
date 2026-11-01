cmake_minimum_required(VERSION 3.16)
project(records_baseline LANGUAGES CXX)

add_executable(records-baseline code/records-baseline.cpp)
target_compile_features(records-baseline PRIVATE cxx_std_20)
target_compile_options(records-baseline PRIVATE -Wall -Wextra -Werror)
set_target_properties(records-baseline PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}")
