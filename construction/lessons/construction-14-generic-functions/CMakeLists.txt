cmake_minimum_required(VERSION 3.16)
project(generic_functions_baseline LANGUAGES CXX)

add_executable(generic-functions-baseline
    code/main.cpp
    code/game.cpp
    code/board.cpp
    code/knight.cpp
    code/position.cpp
    code/random_sequence.cpp
)
target_include_directories(generic-functions-baseline PRIVATE code)
target_compile_features(generic-functions-baseline PRIVATE cxx_std_20)
target_compile_options(generic-functions-baseline PRIVATE -Wall -Wextra -Werror)
set_target_properties(generic-functions-baseline PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}")
