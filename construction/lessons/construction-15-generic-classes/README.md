# construction-15 — Generic classes and concepts baseline

> [English](README.md) | [Español](README-es.md)

> **Status:** drafted · **Concept:** constraints · **Prerequisite:** construction-14 ·
> **Estimated time:** 90 minutes

This directory holds the **after** artifact for `construction-15`. It has the same observable
behavior as `construction-14` for the 8×8 game, but the board and the game engine are class
templates, and a C++20 concept constrains the piece type.

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
| `code/game_engine.h` | `PositionedPiece` concept and `GameEngine<RowCount, ColumnCount>`. |
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

## The generic boundary

`Board<RowCount, ColumnCount>` owns a fixed maximum storage and exposes the active dimensions.
`GameEngine<RowCount, ColumnCount>` owns one board and two knights and runs the game at those
dimensions.

`PositionedPiece` requires:

```cpp
{ constPiece.row() } -> std::convertible_to<int>;
{ constPiece.column() } -> std::convertible_to<int>;
{ piece.set_position(1, 1) } -> std::same_as<void>;
{ constPiece.role_name() } -> std::convertible_to<const char *>;
```

`static_assert(PositionedPiece<Knight>)` makes the constraint part of the build.

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

Compile and run `GameEngine<8, 8>` and `GameEngine<10, 10>` from the same template. The 8×8 engine
must reproduce all 20 traces. The 10×10 engine must compile and run; its transcript changes because
the board changed.

## Acceptance criterion

The learner can instantiate the same engine template for 8×8 and 10×10 and explain which part is
generic, which part is constrained, and which behavior changes with the dimensions.
