cmake_minimum_required(VERSION 3.16)
project(board_cells_baseline LANGUAGES CXX)

add_executable(board-cells-baseline code/board-cells-baseline.cpp)
target_compile_features(board-cells-baseline PRIVATE cxx_std_20)
target_compile_options(board-cells-baseline PRIVATE -Wall -Wextra -Werror)
set_target_properties(board-cells-baseline PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}")
