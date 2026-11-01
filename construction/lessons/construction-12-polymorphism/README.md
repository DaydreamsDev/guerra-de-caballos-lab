# construction-12 — Polymorphism baseline

> [English](README.md) | [Español](README-es.md)

> **Status:** drafted · **Concept:** late binding · **Prerequisite:** construction-11 ·
> **Estimated time:** 80 minutes

This directory holds the **after** artifact for `construction-12`. It has the same observable
behavior as `construction-11`, but the two knights are accessed through a `Piece` interface and the
game loop uses virtual dispatch instead of concrete knight calls.

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
| `code/piece.h` | Abstract `Piece` interface. |
| `code/knight_piece.h` | `KnightPiece` implementation of `Piece`. |
| `code/knight_piece.cpp` | Knight state and virtual method implementations. |
| `code/board.h` | `Board` class with private invariant. |
| `code/board.cpp` | Board representation and operations. |
| `code/random_sequence.h` | Public random sequence interface. |
| `code/random_sequence.cpp` | Hidden generator state. |
| `CMakeLists.txt` | C++20 build with `-Wall -Wextra -Werror`. |
| `run.sh` | Compiles when needed and runs the binary. |

## The interface

`piece.h` declares:

```cpp
class Piece
{
public:
    virtual ~Piece() = default;
    virtual int row() const = 0;
    virtual int column() const = 0;
    virtual void set_position(int row, int column) = 0;
    virtual const char *role_name() const = 0;
};
```

`game.cpp` holds `Piece *` and `const Piece *` and calls only those virtual operations. It never
switches on a concrete piece type.

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

Add a new `Piece` subclass in a scratch file, implement the four virtual operations, and substitute
it at the construction site in `run_game`. The game loop must need zero edits.

## Acceptance criterion

The learner can add a new `Piece` type and show that the game loop, the turn logic and the
transcript code require no edits.
