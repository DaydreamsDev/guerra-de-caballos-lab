cmake_minimum_required(VERSION 3.16)
project(reference_baseline LANGUAGES CXX)

find_package(Threads REQUIRED)

add_executable(reference-baseline
    code/main.cpp
    code/game.cpp
    code/console_transcript.cpp
    code/knight.cpp
    code/position.cpp
    code/random_sequence.cpp
)
target_include_directories(reference-baseline PRIVATE code)
target_compile_features(reference-baseline PRIVATE cxx_std_20)
target_compile_options(reference-baseline PRIVATE -Wall -Wextra -Werror)
set_target_properties(reference-baseline PROPERTIES
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

add_executable(parallel-runner
    code/parallel_runner.cpp
    code/knight.cpp
    code/position.cpp
    code/random_sequence.cpp
)
target_include_directories(parallel-runner PRIVATE code)
target_compile_features(parallel-runner PRIVATE cxx_std_20)
target_compile_options(parallel-runner PRIVATE -Wall -Wextra -Werror)
target_link_libraries(parallel-runner PRIVATE Threads::Threads)
set_target_properties(parallel-runner PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}")

add_executable(error-probe
    code/error_probe.cpp
    code/knight.cpp
    code/position.cpp
    code/random_sequence.cpp
)
target_include_directories(error-probe PRIVATE code)
target_compile_features(error-probe PRIVATE cxx_std_20)
target_compile_options(error-probe PRIVATE -Wall -Wextra -Werror)
set_target_properties(error-probe PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}")
