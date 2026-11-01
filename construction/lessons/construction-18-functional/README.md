# construction-18 — Functional style baseline

> [English](README.md) | [Español](README-es.md)

> **Status:** drafted · **Concept:** purity and immutability · **Prerequisite:** construction-17 ·
> **Estimated time:** 85 minutes

This directory holds the **after** artifact for `construction-18`. It has the same observable
behavior as `construction-17`, but the engine keeps a history of immutable board snapshots. Undo and
redo are value copies, not a replay of the move rules.

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
| `code/game_engine.h` | Engine with board history, undo and redo. |
| `code/board.h` | `Board` using value semantics. |
| `code/move_generator.h` | Pure move-candidate generation. |
| `code/knight.h` | `Knight` with a `Position` member. |
| `code/knight.cpp` | Delegation to `Position` and role name. |
| `code/position.h` | `Position` value type. |
| `code/position.cpp` | Row and column state. |
| `code/random_sequence.h` | Public random sequence interface. |
| `code/random_sequence.cpp` | Hidden generator state. |
| `CMakeLists.txt` | C++20 build with `-Wall -Wextra -Werror`. |
| `run.sh` | Compiles when needed and runs the binary. |

## The functional boundary

- `generate_move_candidates` is a pure function of a piece.
- `Board` is a value type; a snapshot is a copy.
- `GameEngine` stores `std::vector<Board<RowCount, ColumnCount>> history`.
- `undo_last_move` assigns the previous snapshot.
- `redo_last_move` assigns the next snapshot.
- A new move after undo erases the redo branch.

The game loop still mutates the current board; the history boundary is what becomes immutable.

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

Run a game, record `history_size()`, call `undo_last_move()`, then `redo_last_move()`. Both return
true and the board returns to the same state. Compare the cost with implementing undo by replaying
the move rules.

## Acceptance criterion

The learner can undo and redo a move through board history and show that all 20 traces still pass.
