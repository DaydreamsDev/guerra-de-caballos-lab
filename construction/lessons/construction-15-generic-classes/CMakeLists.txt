cmake_minimum_required(VERSION 3.16)
project(generic_classes_baseline LANGUAGES CXX)

add_executable(generic-classes-baseline
    code/main.cpp
    code/game.cpp
    code/knight.cpp
    code/position.cpp
    code/random_sequence.cpp
)
target_include_directories(generic-classes-baseline PRIVATE code)
target_compile_features(generic-classes-baseline PRIVATE cxx_std_20)
target_compile_options(generic-classes-baseline PRIVATE -Wall -Wextra -Werror)
set_target_properties(generic-classes-baseline PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}")
