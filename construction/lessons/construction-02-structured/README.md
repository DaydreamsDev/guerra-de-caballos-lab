# construction-02 — Structured programming baseline

> [English](README.md) | [Español](README-es.md)

> **Status:** drafted · **Concept:** sequence, selection and iteration · **Prerequisite:**
> construction-01 · **Estimated time:** 65 minutes

This directory holds the **after** artifact for `construction-02`. It has the same observable
behavior as `construction-01`, but the game loop and the move attempts use sequence, selection and
iteration instead of labels and `goto`.

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
| `code/structured-baseline.cpp` | The game with loops, conditionals and no `goto`. |
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
    -o build/structured-baseline code/structured-baseline.cpp
```

The binary is written to `build/structured-baseline`.

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

## Why this baseline is structured

- The game loop is one `while` with one terminal condition.
- The move attempts are one `for` loop with one exit condition.
- Each branch either continues the attempt loop or ends the game loop.
- There is no label and no `goto`.
- The state that controls the next iteration is visible in the loop condition and the assignments
  immediately before it.

The helper functions for the random generator and cell conversion remain. `construction-03` is about
extracting the game logic into functions with parameters; this lesson isolates control flow.

## Change test

Apply **R-SKIP**: at the start of a turn, if the number of turns started is a positive multiple of
five, the active knight makes no move. The turn is still announced with its `turn=` and `order=`
fields; no `try=` line and no `move=` line follow. Play passes to the other knight, and the skipped
turn does not count toward the 200 successful moves.

Predict the cost before editing. Count the locations that must change and compare them with the same
change in `construction-01`. Record the files touched, lines changed, new concepts required and
traces broken.

## Acceptance criterion

The learner can apply the same R-SKIP requirement to both baselines and explain why the structured
version localizes the change while the non-structured version scatters it.
