cmake_minimum_required(VERSION 3.16)
project(modular_baseline LANGUAGES CXX)

add_executable(modular-baseline
    code/main.cpp
    code/game.cpp
    code/board.cpp
    code/random_sequence.cpp
)
target_include_directories(modular-baseline PRIVATE code)
target_compile_features(modular-baseline PRIVATE cxx_std_20)
target_compile_options(modular-baseline PRIVATE -Wall -Wextra -Werror)
set_target_properties(modular-baseline PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}")
