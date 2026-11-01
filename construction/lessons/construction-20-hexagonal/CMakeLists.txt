cmake_minimum_required(VERSION 3.16)
project(hexagonal_baseline LANGUAGES CXX)

add_executable(hexagonal-baseline
    code/main.cpp
    code/game.cpp
    code/console_transcript.cpp
    code/knight.cpp
    code/position.cpp
    code/random_sequence.cpp
)
target_include_directories(hexagonal-baseline PRIVATE code)
target_compile_features(hexagonal-baseline PRIVATE cxx_std_20)
target_compile_options(hexagonal-baseline PRIVATE -Wall -Wextra -Werror)
set_target_properties(hexagonal-baseline PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}")

add_executable(adapter-probe
    code/adapter_probe.cpp
    code/knight.cpp
    code/position.cpp
    code/random_sequence.cpp
)
target_include_directories(adapter-probe PRIVATE code)
target_compile_features(adapter-probe PRIVATE cxx_std_20)
target_compile_options(adapter-probe PRIVATE -Wall -Wextra -Werror)
set_target_properties(adapter-probe PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}")
