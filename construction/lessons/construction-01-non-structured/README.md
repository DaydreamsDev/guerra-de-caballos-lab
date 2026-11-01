# construction-01 — Non-structured programming baseline

> [English](README.md) | [Español](README-es.md)

> **Status:** drafted · **Concept:** arbitrary control flow · **Prerequisite:** construction-00 ·
> **Estimated time:** 70 minutes

This directory holds the **before** artifact for `construction-01`. It is intentionally
non-structured: the game state lives in globals, and the game loop and the move attempts are driven
by labels and `goto`. It is not a style recommendation. Its purpose is to make arbitrary control
flow observable before `construction-02` replaces it with sequence, selection and iteration.

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
| `code/non-structured-baseline.cpp` | The game: one `main`, globals, labels and `goto`. |
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
    -o build/non-structured-baseline code/non-structured-baseline.cpp
```

The binary is written to `build/non-structured-baseline`.

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

All 20 traces from `seed-0001.txt` through `seed-0020.txt` match byte for byte with GCC 16.2.1.
`cmp` prints nothing on success.

## Why this baseline is non-structured

The observable behavior is correct. The control flow is not readable without a map:

- `gameLoopStart` checks the terminal condition, draws the move order and prints the turn header.
- `attemptStart` tries one move and jumps to `attemptOutside`, `attemptCapture`, `attemptVisited` or
  `attemptLegal`.
- `attemptAdvance` increments the attempt index and jumps back to `attemptStart`.
- `noLegalMove` assigns the winner and jumps back to `gameLoopStart`.
- A successful move mutates the active knight in place and jumps back to `gameLoopStart`.

There is one entry and many re-entry points: the legal-move branch, the capture branch and the
trapped branch all re-enter the loop from different points. The reader must reconstruct the global
state and both knight positions from the label that was reached, not from a function signature.

The file has no comments by project standard, no abbreviations, no single-letter variable names,
and no line longer than 100 columns. Those rules make the code readable at the token level. They do
not repair the control flow, which is the point of the lesson.

## Change test

Apply **R-SKIP**: at the start of a turn, if the number of turns started is a positive multiple of
five, the active knight makes no move. The turn is still announced with its `turn=` and `order=`
fields; no `try=` line and no `move=` line follow. Play passes to the other knight, and the skipped
turn does not count toward the 200 successful moves.

Predict the cost before editing. Count the labels and jumps that must be rewired. The turn is
currently switched inside `attemptLegal`, while terminal transitions happen in `attemptCapture` and
`noLegalMove`. A skip path needs a new transition that agrees with all three. Record the files
touched, lines changed, new concepts required and traces broken, then compare the numbers with the
structured variant in `construction-02`.

## Acceptance criterion

The learner can explain why this flow is unreadable **without** using the word “bad”, naming the
specific jumps that make the state hard to reconstruct.
