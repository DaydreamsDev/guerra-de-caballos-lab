# construction-14 — Generic functions baseline

> [English](README.md) | [Español](README-es.md)

> **Status:** drafted · **Concept:** type parameters · **Prerequisite:** construction-12 or
> construction-13 · **Estimated time:** 68 minutes

This directory holds the **after** artifact for `construction-14`. It has the same observable
behavior as `construction-13`, but the eight move candidates are produced by one function template
instead of being computed inline in the turn loop.

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
| `code/move_generator.h` | `MoveCandidate` and the function template. |
| `code/knight.h` | `Knight` with a `Position` member. |
| `code/knight.cpp` | Delegation to `Position` and role name. |
| `code/position.h` | `Position` value type. |
| `code/position.cpp` | Row and column state. |
| `code/board.h` | `Board` class with private invariant. |
| `code/board.cpp` | Board representation and operations. |
| `code/random_sequence.h` | Public random sequence interface. |
| `code/random_sequence.cpp` | Hidden generator state. |
| `CMakeLists.txt` | C++20 build with `-Wall -Wextra -Werror`. |
| `run.sh` | Compiles when needed and runs the binary. |

## The template

`move_generator.h` declares:

```cpp
template <typename PieceType>
std::array<MoveCandidate, moveCandidateCount> generate_move_candidates(const PieceType &piece);
```

Any type with `row()` and `column()` can use the same generator. `game.cpp` calls it with `Knight`.
The turn loop no longer computes offsets.

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

Create a small type with `row()` and `column()` and call `generate_move_candidates` with both that
type and `Knight`. The same template instantiation must compile for both; no second generator.

## Acceptance criterion

The learner can use one move generator for two piece types and show that the game loop and all 20
traces remain unchanged.
