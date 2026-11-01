# construction-20 — Hexagonal baseline

> [English](README.md) | [Español](README-es.md)

> **Status:** drafted · **Concept:** ports and adapters · **Prerequisite:** construction-08 ·
> **Estimated time:** 90 minutes

This directory holds the **after** artifact for `construction-20`. It has the same observable
behavior as `construction-17`, but the game core no longer writes to the console. The core emits
events through a `TranscriptPort`, and two adapters implement that port: a console adapter for the
real transcript and a recording adapter for tests.

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
| `code/game.cpp` | Wires the console adapter to the core. |
| `code/game_engine.h` | Core engine; depends on `TranscriptPort`, not on `stdio`. |
| `code/transcript_port.h` | The port: game header, turn, attempt, move, trap and winner events. |
| `code/console_transcript.h` | Console adapter declaration. |
| `code/console_transcript.cpp` | Console adapter; the only file that prints the transcript. |
| `code/recording_transcript.h` | Test adapter; records event counts and the winner. |
| `code/adapter_probe.cpp` | Runs one game with the recording adapter and prints the counts. |
| `code/board.h` | Board state and invariant. |
| `code/move_generator.h` | Pure move-candidate generation. |
| `code/knight.h` | `Knight` with a `Position` member. |
| `code/knight.cpp` | Delegation to `Position` and role name. |
| `code/position.h` | `Position` value type. |
| `code/position.cpp` | Row and column state. |
| `code/random_sequence.h` | Public random sequence interface. |
| `code/random_sequence.cpp` | Hidden generator state. |
| `CMakeLists.txt` | C++20 build for the baseline and the adapter probe. |
| `run.sh` | Compiles when needed and runs the console baseline. |

## The port and the adapters

The port has one method per transcript event:

- `game_header`
- `turn_header`
- `illegal_attempt`
- `capture_attempt`
- `legal_attempt`
- `move`
- `no_legal_move`
- `winner`

The core calls only those methods. `ConsoleTranscript` prints them in the exact transcript format.
`RecordingTranscript` increments counters and stores the winner. The core includes no `<cstdio>` and
contains no `printf`.

## Build and run

```bash
./run.sh --seed 20 --name Learner
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

## Run the test adapter

```bash
c++ -std=c++20 -O2 -Icode -o adapter-probe \
    code/adapter_probe.cpp code/knight.cpp code/position.cpp code/random_sequence.cpp
./adapter-probe
```

The probe runs seed 1 and prints the event counts. The core is identical to the console run; only
the adapter changes.

## Change test

Run the same seed through the console adapter and the recording adapter. The console transcript must
be byte-identical to the golden trace. The recording adapter must report the same number of turns,
attempts, moves and the same winner. Then add one new event to the port and implement it in both
adapters without touching the core.

## Acceptance criterion

The learner can run the same core with two adapters, show the console transcript is unchanged, and
add one port event without editing the game loop.
