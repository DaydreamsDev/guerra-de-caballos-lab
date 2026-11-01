# construction-05 — Array of records baseline

> [English](README.md) | [Español](README-es.md)

> **Status:** drafted · **Concept:** collection and traversal · **Prerequisite:** construction-04 ·
> **Estimated time:** 62 minutes

This directory holds the **after** artifact for `construction-05`. It has the same observable
behavior as `construction-04`, but the board is an array of `BoardCell` records instead of an array
of booleans.

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
| `code/board-cells-baseline.cpp` | The game with a two-dimensional array of `BoardCell` records. |
| `CMakeLists.txt` | C++20 build with `-Wall -Wextra -Werror`. |
| `run.sh` | Compiles when needed and runs the binary. |

## Build and run

```bash
./run.sh --seed 17 --name Learner
```

`run.sh` uses CMake when it is installed. When CMake is not available it compiles directly with the
compiler named by `CXX`, or `c++` by default:

```bash
mkdir -p build
g++ -std=c++20 -Wall -Wextra -Werror -O2 \
    -o build/board-cells-baseline code/board-cells-baseline.cpp
```

The binary is written to `build/board-cells-baseline`.

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

## Why this baseline uses an array of records

- The board is `BoardCell boardCells[9][9]`.
- Each cell owns its visited state as a field.
- `tryMove`, `playTurn` and `playGame` pass the board as one parameter.
- A traversal is a loop over rows and columns, not a list of 64 names.
- The board can gain another per-cell field without adding a parallel array.

## Change test

Add one field to `BoardCell`:

```cpp
int moveNumber;
```

Record the successful move number when a cell becomes visited. Then write one traversal that finds
the highest recorded move number. Count how many parallel arrays or 64 variables the same fact would
require without the record.

## Acceptance criterion

The learner can traverse the board with two loops, find the last move number, and explain why no
parallel array is needed.
