cmake_minimum_required(VERSION 3.16)
project(ownership_baseline LANGUAGES CXX)

add_executable(ownership-baseline
    code/main.cpp
    code/game.cpp
    code/knight.cpp
    code/position.cpp
    code/random_sequence.cpp
)
target_include_directories(ownership-baseline PRIVATE code)
target_compile_features(ownership-baseline PRIVATE cxx_std_20)
target_compile_options(ownership-baseline PRIVATE -Wall -Wextra -Werror)
set_target_properties(ownership-baseline PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}")
