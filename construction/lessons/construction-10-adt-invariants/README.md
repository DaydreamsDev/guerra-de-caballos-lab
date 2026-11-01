# construction-10 — ADT invariants baseline

> [English](README.md) | [Español](README-es.md)

> **Status:** drafted · **Concept:** validity and lifecycle · **Prerequisite:** construction-09 ·
> **Estimated time:** 60 minutes

This directory holds the **after** artifact for `construction-10`. It has the same observable
behavior as `construction-09`, but the `Board` ADT now exposes and enforces an invariant:
`board_is_valid` checks that no cell outside the active board area is marked visited.

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
| `code/board.h` | Opaque `Board` type, operations and validity check. |
| `code/board.cpp` | Hidden representation, bounds checks and invariant. |
| `code/random_sequence.h` | Public random sequence interface. |
| `code/random_sequence.cpp` | Hidden generator state. |
| `CMakeLists.txt` | C++20 build with `-Wall -Wextra -Werror`. |
| `run.sh` | Compiles when needed and runs the binary. |

## The invariant

`board.h` adds:

```cpp
bool board_is_valid(const Board *board);
```

The implementation enforces:

- a created board is non-null;
- only cells inside the active area can be marked visited;
- `board_mark_visited` ignores out-of-range rows and columns;
- `board_was_visited` returns false for out-of-range rows and columns;
- `board_reset` clears the active area;
- `board_is_valid` returns false if any cell outside the active area is visited.

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

In a scratch copy of `board.cpp`, remove the bounds guard from `board_mark_visited`. Call
`board_mark_visited(0, 0)` and then `board_is_valid`. The invalid state is now constructible.
Restore the guard and repeat; the invalid state is rejected.

## Acceptance criterion

The learner can show that an out-of-range write is rejected, that `board_is_valid` stays true, and
that removing the guard makes the invalid state visible.
