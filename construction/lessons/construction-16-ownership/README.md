# construction-16 — Ownership baseline

> [English](README.md) | [Español](README-es.md)

> **Status:** drafted · **Concept:** resource lifetime · **Prerequisite:** construction-15 ·
> **Estimated time:** 70 minutes

This directory holds the **after** artifact for `construction-16`. It has the same observable
behavior as `construction-15` for the 8×8 game, but the engine owns the board and both knights
through `std::unique_ptr`. Copying is deleted; moving is explicit.

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
| `code/game.cpp` | Instantiates `GameEngine<8, 8>`. |
| `code/game_engine.h` | `GameEngine` with `std::unique_ptr` ownership. |
| `code/board.h` | `Board<RowCount, ColumnCount>` class template. |
| `code/move_generator.h` | `MoveCandidate` and the generic move generator. |
| `code/knight.h` | `Knight` with a `Position` member. |
| `code/knight.cpp` | Delegation to `Position` and role name. |
| `code/position.h` | `Position` value type. |
| `code/position.cpp` | Row and column state. |
| `code/random_sequence.h` | Public random sequence interface. |
| `code/random_sequence.cpp` | Hidden generator state. |
| `CMakeLists.txt` | C++20 build with `-Wall -Wextra -Werror`. |
| `run.sh` | Compiles when needed and runs the binary. |

## Ownership

`GameEngine` owns:

```cpp
std::unique_ptr<Board<RowCount, ColumnCount>> board;
std::unique_ptr<Knight> playerKnight;
std::unique_ptr<Knight> machineKnight;
```

It deletes copy construction and copy assignment, and keeps move construction and move assignment.
There is no raw `new` or `delete` in the engine.

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

Compile a probe with:

```cpp
static_assert(!std::is_copy_constructible_v<GameEngine<8, 8>>);
static_assert(!std::is_copy_assignable_v<GameEngine<8, 8>>);
static_assert(std::is_move_constructible_v<GameEngine<8, 8>>);
```

Run the engine and confirm no raw `new` or `delete` appears in the source. If a leak detector is
available, run it; if not, record that the ownership is structural and the detector is unavailable.

## Acceptance criterion

The learner can show that the engine owns its resources through `unique_ptr`, that copying is
explicitly deleted, that moving is available, and that the 20 traces still pass.
