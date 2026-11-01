# construction-11 — Encapsulation baseline

> [English](README.md) | [Español](README-es.md)

> **Status:** drafted · **Concept:** class with invariant · **Prerequisite:** construction-10 ·
> **Estimated time:** 60 minutes

This directory holds the **after** artifact for `construction-11`. It has the same observable
behavior as `construction-10`, but `Board` is now a class: the data is private and every public
operation preserves the invariant.

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
| `code/board.h` | `Board` class with public operations and private state. |
| `code/board.cpp` | Private representation and invariant-preserving operations. |
| `code/random_sequence.h` | Public random sequence interface. |
| `code/random_sequence.cpp` | Hidden generator state. |
| `CMakeLists.txt` | C++20 build with `-Wall -Wextra -Werror`. |
| `run.sh` | Compiles when needed and runs the binary. |

## The class boundary

`Board` exposes only:

```cpp
Board();
int row_count() const;
int column_count() const;
bool is_valid() const;
void reset();
bool was_visited(int row, int column) const;
void mark_visited(int row, int column);
```

The array, dimensions and representation are private. `game.cpp` owns a `Board` by value and can
only call the public operations.

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

In a scratch copy of `board.h`, move `visited` to the public section. Compile a probe that writes
`board.visited[0][0] = true` and then calls `board.is_valid()`. The invalid state is now
constructible. Restore `private` and repeat; the write no longer compiles.

## Acceptance criterion

The learner can show that no public field or operation can break the board invariant, and that
making the state public makes the invalid state constructible.
