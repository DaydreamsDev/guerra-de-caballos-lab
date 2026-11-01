# construction-04 — Records baseline

> [English](README.md) | [Español](README-es.md)

> **Status:** drafted · **Concept:** aggregation (agregación) · **Prerequisite:** construction-03 ·
> **Estimated time:** 55 minutes

This directory holds the **after** artifact for `construction-04`. It has the same observable
behavior as `construction-03`, but the two knights are represented by a `Knight` record instead of
separate row and column variables.

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
| `code/records-baseline.cpp` | The game with a `Knight` record and record parameters. |
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
    -o build/records-baseline code/records-baseline.cpp
```

The binary is written to `build/records-baseline`.

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

## Why this baseline uses a record

- `Knight` groups the row and column that always travel together.
- `initializePositions`, `printGameHeader`, `playTurn`, `playGame` and `tryMove` receive a `Knight`
  reference or a `const Knight` reference.
- The record can gain a field without changing every function signature.
- The record is the unit a learner can inspect, copy and compare.

The change test is adding `captured` to `Knight`. The compiler and the call sites tell you what the
record actually localizes.

## Change test

Add one field to `Knight`:

```cpp
bool captured;
```

Initialise it for both knights and decide whether the game must read it. Count the function
signatures that change. Compare the count with the separate-variable version in `construction-03`.

## Acceptance criterion

The learner can add `captured` to the record and show which call sites changed and which did not.
