# construction-06 — References baseline

> [English](README.md) | [Español](README-es.md)

> **Status:** drafted · **Concept:** aliasing without addresses
> **Prerequisite:** construction-05 · **Estimated time:** 45 minutes

This directory holds the **after** artifact for `construction-06`. It has the same observable
behavior as `construction-05`, but aliasing is explicit: the active knight is selected as a
reference, the other knight as a `const` reference, and a move mutates the selected record through
a reference parameter.

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
| `code/references-baseline.cpp` | The game with explicit reference aliases and a mutating parameter. |
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
    -o build/references-baseline code/references-baseline.cpp
```

The binary is written to `build/references-baseline`.

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

## What the references do

- `selectActiveKnight` returns a `Knight &`, so the caller gets an alias to the player or machine
  record.
- `selectOtherKnight` returns a `const Knight &`, so the caller can read but not mutate the other
  record.
- `moveKnight(Knight &knight, ...)` mutates the caller's record through a reference parameter.
- No pointer, address-of operator or manual dereference is needed.
- The reference makes the alias explicit at the function boundary and in the local name.

## Change test

Change `moveKnight` to take `Knight knight` by value. Run the same seed and observe that the
transcript no longer shows the move. Restore the reference and observe that it does. Count the
signatures and call sites that the change touches.

## Acceptance criterion

The learner can mutate a record through a reference parameter and explain why the same function
with a by-value parameter cannot change the caller's record.
