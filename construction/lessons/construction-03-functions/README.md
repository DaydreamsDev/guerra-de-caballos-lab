# construction-03 — Functions and parameters baseline

> [English](README.md) | [Español](README-es.md)

> **Status:** drafted · **Concept:** scope and decomposition · **Prerequisite:** construction-02 ·
> **Estimated time:** 45 minutes

This directory holds the **after** artifact for `construction-03`. It has the same observable
behavior as `construction-02`, but the game is decomposed into functions with parameters. The
positions, move counter, winner and visited board are no longer controlled by one large `main`.

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
| `code/functions-baseline.cpp` | The game decomposed into functions with parameters. |
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
    -o build/functions-baseline code/functions-baseline.cpp
```

The binary is written to `build/functions-baseline`.

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

## Why this baseline uses functions and parameters

- `parseArguments` receives the raw arguments and writes the seed and player
  name through references.
- `initializePositions` and `initializeRolls` write their results through references.
- `tryMove` receives both knight positions, the move number and the visited board; it returns an
  outcome and writes the destination through references.
- `playTurn` receives the positions, the visited board, the move counter and the winner through
  references.
- `playGame` owns the loop and receives the state it needs.
- `printTurnHeader`, the attempt printers and `printMove` receive every value they print.

The only remaining global is `randomState`, because the generator is a process-wide sequence. The
game state moved out of global scope.

## Change test

Apply **R-SKIP**: at the start of a turn, if the number of turns started is a positive multiple of
five, the active knight makes no move. The turn is still announced with its `turn=` and `order=`
fields; no `try=` line and no `move=` line follow. Play passes to the other knight, and the skipped
turn does not count toward the 200 successful moves.

Predict the cost before editing. Identify the smallest function boundary that owns the turn, and
count how many callers must change. Compare the result with `construction-02`.

## Acceptance criterion

The learner can extract one move decision into a function with parameters and show that no global
game state is needed to test that decision.
