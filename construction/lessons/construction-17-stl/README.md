# construction-17 — STL and algorithms baseline

> [English](README.md) | [Español](README-es.md)

> **Status:** drafted · **Concept:** iteration abstraction ·
> **Prerequisite:** construction-05, construction-16 · **Estimated time:** 68 minutes

This directory holds the **after** artifact for `construction-17`. It has the same observable
behavior as `construction-16`, but move generation, move ordering and board reset use the C++20
ranges algorithms instead of hand-written loops.

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
| `code/game_engine.h` | Engine with ranges-based move ordering. |
| `code/board.h` | `Board` using `std::array` and `std::ranges::fill`. |
| `code/move_generator.h` | `MoveCandidate` and ranges-based generation. |
| `code/knight.h` | `Knight` with a `Position` member. |
| `code/knight.cpp` | Delegation to `Position` and role name. |
| `code/position.h` | `Position` value type. |
| `code/position.cpp` | Row and column state. |
| `code/random_sequence.h` | Public random sequence interface. |
| `code/random_sequence.cpp` | Hidden generator state. |
| `CMakeLists.txt` | C++20 build with `-Wall -Wextra -Werror`. |
| `run.sh` | Compiles when needed and runs the binary. |

## The algorithms

- `Board::reset` uses `std::ranges::fill`.
- `generate_move_candidates` uses `std::ranges::transform` over `std::views::iota`.
- `GameEngine::buildMoveOrder` uses `std::ranges::transform` over `std::views::iota`.
- The attempt loop remains: it has early exit, and an algorithm would obscure the turn contract.

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

Replace one hand-written traversal with a ranges algorithm without changing the transcript. The
move generation and board reset are the safe examples; the attempt loop is the counterexample.

## Acceptance criterion

The learner can remove a traversal loop, use an algorithm, and show that the transcript is
unchanged.
