cmake_minimum_required(VERSION 3.16)
project(functional_baseline LANGUAGES CXX)

add_executable(functional-baseline
    code/main.cpp
    code/game.cpp
    code/knight.cpp
    code/position.cpp
    code/random_sequence.cpp
)
target_include_directories(functional-baseline PRIVATE code)
target_compile_features(functional-baseline PRIVATE cxx_std_20)
target_compile_options(functional-baseline PRIVATE -Wall -Wextra -Werror)
set_target_properties(functional-baseline PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}")
