cmake_minimum_required(VERSION 3.16)
project(abstract_data_type_baseline LANGUAGES CXX)

add_executable(abstract-data-type-baseline
    code/main.cpp
    code/game.cpp
    code/board.cpp
    code/random_sequence.cpp
)
target_include_directories(abstract-data-type-baseline PRIVATE code)
target_compile_features(abstract-data-type-baseline PRIVATE cxx_std_20)
target_compile_options(abstract-data-type-baseline PRIVATE -Wall -Wextra -Werror)
set_target_properties(abstract-data-type-baseline PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}")
