# construction-22 — Error contracts baseline

> [English](README.md) | [Español](README-es.md)

> **Status:** drafted · **Concept:** recovery · **Prerequisite:** construction-09 ·
> **Estimated time:** 60 minutes

This directory holds the **after** artifact for `construction-22`. It keeps the concurrency core
from `construction-21` and turns the invalid-move path into an explicit value. The engine never
calls `exit`, throws, or kills the process; it returns a `MoveResult`, and the caller decides what
to do.

## What the program implements

- The exact 64-bit linear congruential generator from `common/spec/game-rules.md`.
- The 8×8 board, both knights, starting positions and starting rolls.
- Move priority, legal and illegal moves, visited cells, capture, trap and the 200-move draw.
- The transcript format from `common/spec/transcript-format.md`, byte for byte.

The console program accepts `--seed INTEGER --name TEXT` in either order and writes the transcript
to standard output.

## Files

| File | Purpose |
|---|---|
| `code/main.cpp` | Parses the command line and calls `run_game`. |
| `code/game.h` | Public game interface. |
| `code/game.cpp` | Wires the console adapter to the core. |
| `code/game_engine.h` | Core engine; `tryMove` returns `MoveResult`. |
| `code/move_result.h` | `MoveOutcome` and `MoveResult`; the error contract. |
| `code/error_probe.cpp` | Calls `evaluate_move` with invalid, capture and legal moves. |
| `code/random_sequence.h` | `RandomSequence` class; one state per engine. |
| `code/random_sequence.cpp` | The 64-bit LCG with no global state. |
| `code/transcript_port.h` | The port from `construction-20`. |
| `code/console_transcript.h` | Console adapter declaration. |
| `code/console_transcript.cpp` | Console adapter; the only transcript printer. |
| `code/recording_transcript.h` | Test adapter; records winner and move count. |
| `code/adapter_probe.cpp` | Runs one game with the recording adapter. |
| `code/parallel_runner.cpp` | Sequential, race, mutex and merge modes. |
| `code/board.h` | Board state and invariant. |
| `code/move_generator.h` | Pure move-candidate generation. |
| `code/knight.h` | `Knight` with a `Position` member. |
| `code/knight.cpp` | Delegation to `Position` and role name. |
| `code/position.h` | `Position` value type. |
| `code/position.cpp` | Row and column state. |
| `CMakeLists.txt` | C++20 build for the baseline, the probes and the runner. |
| `run.sh` | Compiles when needed and runs the console baseline. |

## The contract

`MoveOutcome` has four values:

- `outsideBoard`
- `visited`
- `capture`
- `legal`

`MoveResult` carries the outcome and the destination coordinates:

```cpp
struct MoveResult
{
    MoveOutcome outcome;
    int destinationRow;
    int destinationColumn;
};
```

`GameEngine::tryMove` returns `MoveResult` instead of writing through two output parameters. The
public `evaluate_move` exposes the same contract without running a game. `playTurn` switches on
`moveResult.outcome` and continues, reports, or accepts the move. No branch calls `exit`.

## Build and run

```bash
./run.sh --seed 22 --name Learner
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

## Run the error probe

```bash
c++ -std=c++20 -O2 -Icode -o error-probe \
    code/error_probe.cpp code/knight.cpp code/position.cpp code/random_sequence.cpp
./error-probe
```

The probe prints:

```text
outside=0 capture=2 legal=3
```

The values are the enum values. The probe exits with status 0; an invalid move is a value, not a
process failure.

## Change test

Call `evaluate_move` with a move outside the board, a capture and a legal move. Record the outcome
and the destination. Then make the caller decide: continue the game, report the attempt, or stop.
Show that no path calls `exit` and that the 20 traces still pass.

## Acceptance criterion

The learner can handle an invalid move through a returned value, let the caller choose the recovery,
and show that the process continues and the transcript is unchanged.
