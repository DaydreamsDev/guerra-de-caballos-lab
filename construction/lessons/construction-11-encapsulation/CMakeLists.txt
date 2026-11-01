cmake_minimum_required(VERSION 3.16)
project(encapsulation_baseline LANGUAGES CXX)

add_executable(encapsulation-baseline
    code/main.cpp
    code/game.cpp
    code/board.cpp
    code/random_sequence.cpp
)
target_include_directories(encapsulation-baseline PRIVATE code)
target_compile_features(encapsulation-baseline PRIVATE cxx_std_20)
target_compile_options(encapsulation-baseline PRIVATE -Wall -Wextra -Werror)
set_target_properties(encapsulation-baseline PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}")
