# construction-23 — Port baseline

> [English](README.md) | [Español](README-es.md)

> **Status:** drafted · **Concept:** syntax is disposable · **Prerequisite:** construction-09 or
> construction-12 · **Estimated time:** 180 minutes

This directory holds the **after** artifact for `construction-23`: a Python port of the game. It
produces the same transcript as the C++ baseline, byte for byte, from the same twenty seeds.

## What the port implements

- The exact 64-bit linear congruential generator from `common/spec/game-rules.md`.
- The 8×8 board, both knights, starting positions and starting rolls.
- Move priority, legal and illegal moves, visited cells, capture, trap and the 200-move draw.
- The transcript format from `common/spec/transcript-format.md`, byte for byte.

## Files

| File | Purpose |
|---|---|
| `code/game.py` | The port: random sequence, board rules, game loop and transcript. |
| `code/main.py` | Argument parsing and process entry point. |
| `run.sh` | Runs `main.py` with the system Python interpreter. |
| `README.md` | This file. |
| `README-es.md` | Spanish edition. |

## The port

The C++ baseline has a `Board` class, `Knight` objects, a `TranscriptPort`, two adapters and a
`MoveResult`. The Python port keeps the rules and drops the ceremony:

- `RandomSequence` is a small class with one `state` integer.
- Positions are `tuple[int, int]`.
- The visited cells are a `set[tuple[int, int]]`.
- The transcript is a list of strings joined at the end.
- The game loop is one function, `run_game`.

The LCG constants, the move order, the capture rule, the visited rule, the trap and the 200-move
draw are the same. The transcript is the same. The types, the ownership and the compile-time checks
are not.

## Build and run

Python 3.10 or newer is required. The port uses `tuple[int, int]` and `str | None`.

```bash
./run.sh --seed 23 --name Learner
```

No build step and no third-party package are required.

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

Run the Python port against the same 20 seeds as the C++ baseline. Compare every transcript. Then
change one rule in the port only, observe which traces fail, and restore it. The syntax is
disposable; the rules and the transcript are the contract.

## Acceptance criterion

The learner can run the Python port, match all 20 golden traces, and name at least three things that
changed in the port and three things that did not.
