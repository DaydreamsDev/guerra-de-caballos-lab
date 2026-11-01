cmake_minimum_required(VERSION 3.16)
project(stl_baseline LANGUAGES CXX)

add_executable(stl-baseline
    code/main.cpp
    code/game.cpp
    code/knight.cpp
    code/position.cpp
    code/random_sequence.cpp
)
target_include_directories(stl-baseline PRIVATE code)
target_compile_features(stl-baseline PRIVATE cxx_std_20)
target_compile_options(stl-baseline PRIVATE -Wall -Wextra -Werror)
set_target_properties(stl-baseline PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}")
