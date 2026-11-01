cmake_minimum_required(VERSION 3.16)
project(composition_baseline LANGUAGES CXX)

add_executable(composition-baseline
    code/main.cpp
    code/game.cpp
    code/board.cpp
    code/knight.cpp
    code/position.cpp
    code/random_sequence.cpp
)
target_include_directories(composition-baseline PRIVATE code)
target_compile_features(composition-baseline PRIVATE cxx_std_20)
target_compile_options(composition-baseline PRIVATE -Wall -Wextra -Werror)
set_target_properties(composition-baseline PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}")
