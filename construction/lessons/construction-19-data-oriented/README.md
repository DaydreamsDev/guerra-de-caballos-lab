# construction-19 — Layout baseline

> [English](README.md) | [Español](README-es.md)

> **Status:** drafted · **Concept:** layout · **Prerequisite:** construction-05 ·
> **Estimated time:** 85 minutes

This directory holds the **after** artifact for `construction-19`. It has the same observable
behavior as `construction-17`, and it keeps the same ownership model and method style. Only the
board's representation changes: visited cells are bit-packed, and an occupant table makes capture a
lookup.

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
| `code/game_engine.h` | Engine with `unique_ptr` board and knights; ownership unchanged. |
| `code/board.h` | `Board<8, 8>` with bit-packed visited words and an occupant table. |
| `code/move_generator.h` | Pure move-candidate generation. |
| `code/knight.h` | `Knight` with a `Position` member. |
| `code/knight.cpp` | Delegation to `Position` and role name. |
| `code/position.h` | `Position` value type. |
| `code/position.cpp` | Row and column state. |
| `code/random_sequence.h` | Public random sequence interface. |
| `code/random_sequence.cpp` | Hidden generator state. |
| `code/benchmark.cpp` | Runs 10,000 games with the transcript redirected to `/dev/null`. |
| `CMakeLists.txt` | C++20 build for the baseline and the benchmark. |
| `run.sh` | Compiles when needed and runs the baseline. |

## The data layout

`Board<8, 8>` keeps the same public methods as in `construction-17`. Its private members are:

- `std::array<std::uint64_t, 4> visitedWords` — one bit per cell.
- `std::array<std::uint8_t, 256> occupant` — zero, player or machine.

`reset`, `was_visited`, `mark_visited`, `occupant_at`, `set_occupant` and `clear_occupant` are
methods, and both arrays stay private. The engine still owns the board and both knights through
three `std::unique_ptr` members. There is no ownership change in this baseline.

## Build and run

```bash
./run.sh --seed 19 --name Learner
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

## Profile 10,000 games

Build the benchmark:

```bash
c++ -std=c++20 -O2 -Icode -o benchmark \
    code/benchmark.cpp code/knight.cpp code/position.cpp code/random_sequence.cpp
./benchmark
```

The benchmark redirects standard output to `/dev/null` and reports elapsed milliseconds on standard
error. On this machine the result is about 270–290 ms for 10,000 games, against about 270–285 ms for
`construction-17`. The ranges overlap, so the difference is noise.

The structural numbers are just as honest: `sizeof(Board<8, 8>)` grows from 256 to 288,
`sizeof(GameEngine<8, 8>)` stays 24, and both baselines make 30,000 heap allocations for 10,000
games (three per game). `game.o` text grows from 4,656 to 4,961 bytes. The win is a different
representation, not fewer allocations and not a faster transcript.

## Change test

Run 10,000 games and record six measurements: `sizeof(Board<8, 8>)`, `sizeof(GameEngine<8, 8>)`,
heap allocations, elapsed milliseconds, `game.o` text, and the number of matching traces. Compare
them with `construction-17`. Explain which numbers moved and which did not.

## Acceptance criterion

The learner can run 10,000 games, report the profile, and explain why the board grew by 32 bytes
while the engine size, the allocation count and the wall-clock time did not move.
