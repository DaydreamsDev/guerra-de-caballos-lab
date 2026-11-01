# construction-08 — Modular baseline

> [English](README.md) | [Español](README-es.md)

> **Status:** drafted · **Concept:** information hiding · **Prerequisite:** construction-03 ·
> **Estimated time:** 90 minutes

This directory holds the **after** artifact for `construction-08`. It has the same observable
behavior as the earlier baselines for the 8×8 game, but the source is split into modules with
headers and separate translation units.

## Modules

| Module | Interface | Hidden implementation |
|---|---|---|
| `random_sequence` | seed, random upper bound, random integer | multiplier, increment, state |
| `board` | row count, column count, reset, visited query, visited write | cell representation, active and maximum dimensions |
| `game` | `run_game(seed, playerName)` | knights, move order, turn loop, transcript |
| `main` | command-line parsing | — |

`main.cpp` includes only `game.h`. `game.cpp` includes `board.h` and `random_sequence.h`. A caller
of the board interface cannot see `BoardCell`.

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
| `code/board.h` | Public board interface. |
| `code/board.cpp` | Hidden board representation and dimensions. |
| `code/random_sequence.h` | Public random sequence interface. |
| `code/random_sequence.cpp` | Hidden generator state. |
| `CMakeLists.txt` | C++20 build with `-Wall -Wextra -Werror`. |
| `run.sh` | Compiles when needed and runs the binary. |

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

Change the active board dimensions in `board.cpp` from 8×8 to 10×10. Count which files must change
and which files must recompile. The behavior changes, so the 8×8 golden traces no longer apply; the
lesson is about the change surface, not trace preservation.

## Acceptance criterion

The learner can change the board size in the board module and show that `game.cpp` and `main.cpp`
require no edits.
