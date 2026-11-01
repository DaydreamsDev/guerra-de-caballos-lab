# construction-07 — Pointers baseline

> [English](README.md) | [Español](README-es.md)

> **Status:** drafted · **Concept:** indirection and lifetime · **Prerequisite:** construction-06 ·
> **Estimated time:** 90 minutes

This directory holds the **after** artifact for `construction-07`. It has the same observable
behavior as `construction-06`, but the knight aliases and the board cell access use pointers. The
transcript is unchanged; the source makes the indirection and the row-major layout visible.

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
| `code/pointers-baseline.cpp` | The game with pointer aliases and pointer-based cell access. |
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
    -o build/pointers-baseline code/pointers-baseline.cpp
```

The binary is written to `build/pointers-baseline`.

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

## What the pointers do

- `selectActiveKnight` returns `Knight *`; the caller holds an address, not an alias.
- `selectOtherKnight` returns `const Knight *`; the pointee cannot be mutated through it.
- `moveKnight(Knight *, ...)` mutates the pointee, not a copy.
- `getBoardCell` returns `BoardCell *`; the caller dereferences with `->`.
- The board remains a two-dimensional array; the pointer exposes the row-major address of one cell.

## Change test

Rewrite one board access as pointer arithmetic inside a row:

```cpp
BoardCell *rowStart = boardCells[row];
BoardCell *cell = rowStart + column;
```

Then explain what would break if the row width changed from 9 to another value. Count the places
that assume the row-major layout, and compare the count with the references baseline in
`construction-06`.

## Acceptance criterion

The learner can access a board cell through a pointer, explain the row-major stride, and show that
the transcript stays unchanged while the source exposes the layout.
