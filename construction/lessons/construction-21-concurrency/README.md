# construction-21 — Concurrency baseline

> [English](README.md) | [Español](README-es.md)

> **Status:** drafted · **Concept:** shared state vs messages · **Prerequisite:** construction-12 ·
> **Estimated time:** 120 minutes

This directory holds the **after** artifact for `construction-21`. It keeps the hexagonal core from
`construction-20` and adds two things: the random sequence becomes per-engine state, and a parallel
runner compares shared mutable results with a mutex and with per-thread results.

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
| `code/game_engine.h` | Core engine with a per-engine `RandomSequence`. |
| `code/random_sequence.h` | `RandomSequence` class; one state per engine. |
| `code/random_sequence.cpp` | The 64-bit LCG with no global state. |
| `code/transcript_port.h` | The port from `construction-20`. |
| `code/console_transcript.h` | Console adapter declaration. |
| `code/console_transcript.cpp` | Console adapter; the only transcript printer. |
| `code/recording_transcript.h` | Test adapter; records winner and move count. |
| `code/adapter_probe.cpp` | Runs one game with the recording adapter. |
| `code/parallel_runner.cpp` | Sequential, race, mutex and merge modes for 16,000 games. |
| `code/board.h` | Board state and invariant. |
| `code/move_generator.h` | Pure move-candidate generation. |
| `code/knight.h` | `Knight` with a `Position` member. |
| `code/knight.cpp` | Delegation to `Position` and role name. |
| `code/position.h` | `Position` value type. |
| `code/position.cpp` | Row and column state. |
| `CMakeLists.txt` | C++20 build for the baseline, the probe and the runner. |
| `run.sh` | Compiles when needed and runs the console baseline. |

## The shared-state change

`construction-20` used a global `randomState` inside `random_sequence.cpp`. Two threads running two
games would write the same variable. `construction-21` moves the state into a `RandomSequence`
object owned by each `GameEngine`. The LCG sequence is unchanged, so the console transcript is
unchanged; the ownership is what makes parallel games deterministic.

The parallel runner has four modes:

- `sequential` — one game at a time; the reference checksum.
- `race` — a shared `nextIndex` and a pre-sized result vector, without synchronization.
- `mutex` — the same shared vector protected by `std::mutex`.
- `merge` — one result vector per thread, merged after the join; no shared mutable result state.

## Build and run

```bash
./run.sh --seed 21 --name Learner
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

## Run the concurrency experiment

```bash
c++ -std=c++20 -O2 -pthread -Icode -o parallel-runner \
    code/parallel_runner.cpp code/knight.cpp code/position.cpp code/random_sequence.cpp
./parallel-runner --mode sequential
./parallel-runner --mode race
./parallel-runner --mode mutex
./parallel-runner --mode merge
```

On this machine, 16,000 games produce:

| Mode | Written | Checksum |
|---|---:|---:|
| sequential | 16,000 | 2826026307342797653 |
| race | 14,719–14,821 | varies |
| mutex | 16,000 | 2826026307342797653 |
| merge | 16,000 | 2826026307342797653 |

The race mode loses writes and its checksum changes between runs. The mutex and merge modes match
the sequential checksum. The race is intentionally undefined behavior; run it more than once to see
the result move.

## Change test

Run `sequential`, then `race`, then `mutex` and `merge`. Record the written count and checksum.
Remove the race by protecting the shared index or by giving each thread its own result vector. Show
that the fixed checksum matches the sequential checksum and that the 20 golden traces still pass.

## Acceptance criterion

The learner can reproduce a lost-write race, remove it with shared-state protection or per-thread
results, and show that parallel execution is deterministic and the transcript is unchanged.
