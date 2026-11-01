# construction-09 — Abstract data type baseline

> [English](README.md) | [Español](README-es.md)

> **Status:** drafted · **Concept:** representation independence
> **Prerequisite:** construction-08 · **Estimated time:** 90 minutes

This directory holds the **after** artifact for `construction-09`. It has the same observable
behavior as `construction-08` for the 8×8 game, but `Board` is an abstract data type: the header
exposes an incomplete type and operations, while the representation lives in `board.cpp`.

## What the program implements

- The exact 64-bit linear congruential generator from `common/spec/game-rules.md`.
- The 8×8 board, both knights, starting positions and starting rolls.
- Move priority, legal and illegal moves, visited cells, capture, trap and the 200-move draw.
- The transcript format from `common/spec/transcript-format.md`, byte for byte.

The program accepts `--seed INTEGER --name TEXT` in either order and writes the transcript to
standard output.

## Files

| File | Purpose |
|---|---|
| `code/main.cpp` | Parses the command line and calls `run_game`. |
| `code/game.h` | Public game interface. |
| `code/game.cpp` | Game rules, turn loop and transcript. |
| `code/board.h` | Opaque `Board` type and operations. |
| `code/board.cpp` | Hidden representation and dimensions. |
| `code/random_sequence.h` | Public random sequence interface. |
| `code/random_sequence.cpp` | Hidden generator state. |
| `CMakeLists.txt` | C++20 build with `-Wall -Wextra -Werror`. |
| `run.sh` | Compiles when needed and runs the binary. |

## The ADT boundary

`board.h` declares only:

```cpp
struct Board;
Board *board_create();
void board_destroy(Board *board);
int board_row_count(const Board *board);
int board_column_count(const Board *board);
void board_reset(Board *board);
bool board_was_visited(const Board *board, int row, int column);
void board_mark_visited(Board *board, int row, int column);
```

`game.cpp` cannot see `Board::visited`, the array dimensions or the storage strategy. It can only
call the operations.

## Build and run

```bash
./run.sh --seed 17 --name Learner
```

`run.sh` uses CMake when it is installed. When CMake is not available it compiles all translation
units directly with the compiler named by `CXX`, or `c++` by default.

## Verify the golden traces

From this directory:

```bash
for trace in ../../../common/traces/seed-*.txt; do
    traceName="$(basename "${trace}" .txt)"
    seedText="${traceName#seed-}"
    seedNumber="$((10#${seedText}))"
    ./run.sh --seed "${seedNumber}" --name Learner | cmp - "${trace}"
done
```

All 20 traces from `seed-0001.txt` through `seed-0020.txt` match byte for byte.

## Change test

Replace the fixed array inside `board.cpp` with a different representation, for example a
`std::vector<std::vector<bool>>` or a one-dimensional vector with an index function. Change only
`board.cpp`. `game.cpp`, `main.cpp` and `board.h` must remain unchanged.

## Acceptance criterion

The learner can swap the board representation inside `board.cpp` and show that every caller still
compiles and all 20 traces still pass.
