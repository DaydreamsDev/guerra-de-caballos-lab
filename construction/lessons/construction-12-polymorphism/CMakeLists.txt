cmake_minimum_required(VERSION 3.16)
project(polymorphism_baseline LANGUAGES CXX)

add_executable(polymorphism-baseline
    code/main.cpp
    code/game.cpp
    code/board.cpp
    code/knight_piece.cpp
    code/random_sequence.cpp
)
target_include_directories(polymorphism-baseline PRIVATE code)
target_compile_features(polymorphism-baseline PRIVATE cxx_std_20)
target_compile_options(polymorphism-baseline PRIVATE -Wall -Wextra -Werror)
set_target_properties(polymorphism-baseline PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}")
