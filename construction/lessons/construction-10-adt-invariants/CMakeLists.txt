cmake_minimum_required(VERSION 3.16)
project(adt_invariants_baseline LANGUAGES CXX)

add_executable(adt-invariants-baseline
    code/main.cpp
    code/game.cpp
    code/board.cpp
    code/random_sequence.cpp
)
target_include_directories(adt-invariants-baseline PRIVATE code)
target_compile_features(adt-invariants-baseline PRIVATE cxx_std_20)
target_compile_options(adt-invariants-baseline PRIVATE -Wall -Wextra -Werror)
set_target_properties(adt-invariants-baseline PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}")
