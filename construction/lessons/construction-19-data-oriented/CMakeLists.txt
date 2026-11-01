cmake_minimum_required(VERSION 3.16)
project(data_oriented_baseline LANGUAGES CXX)

add_executable(data-oriented-baseline
    code/main.cpp
    code/game.cpp
    code/knight.cpp
    code/position.cpp
    code/random_sequence.cpp
)
target_include_directories(data-oriented-baseline PRIVATE code)
target_compile_features(data-oriented-baseline PRIVATE cxx_std_20)
target_compile_options(data-oriented-baseline PRIVATE -Wall -Wextra -Werror)
set_target_properties(data-oriented-baseline PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}")

add_executable(data-oriented-benchmark
    code/benchmark.cpp
    code/knight.cpp
    code/position.cpp
    code/random_sequence.cpp
)
target_include_directories(data-oriented-benchmark PRIVATE code)
target_compile_features(data-oriented-benchmark PRIVATE cxx_std_20)
target_compile_options(data-oriented-benchmark PRIVATE -Wall -Wextra -Werror)
set_target_properties(data-oriented-benchmark PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}")
