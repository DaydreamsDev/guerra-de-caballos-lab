# archaeology-14 — Tests and determinism

> [English](README.md) | [Español](README-es.md)

> **Status:** drafted · **Concept:** tests and determinism · **Prerequisite:** archaeology-03 ·
> **Estimated time:** 90 minutes

This lesson turns a non-reproducible program into one with seed control and a golden trace. The
baseline uses a portable 64-bit generator; the lesson explains why the archive's `rand()` cannot be
a portable golden trace without a platform note.

## What the learner does

- Reads the archive's `srand(time(NULL))` and `rand()` calls.
- Runs the seed-controlled baseline twice with the same seed.
- Compares both runs with the golden trace.
- Runs a different seed and explains why the output changes.
- Records what a portable trace needs and what a captured legacy trace cannot promise.

## The baseline

```text
code/seed_control.cpp
code/tests/golden-seed-1.txt
```

The program accepts `--seed` and uses the lab's 64-bit LCG. The golden file is the exact output for
seed 1.

## Build and run

```bash
./run.sh
```

Expected output:

```text
first=seed=1
move=1 position=2
move=2 position=5
move=3 position=0
winner=player
moves=3
same=yes
golden=yes
```

## Files

| File | Purpose |
|---|---|
| `README.md` | This file. |
| `README-es.md` | Spanish edition. |
| `lesson.md` | The guided lesson. |
| `lesson-es.md` | Spanish edition of the guided lesson. |
| `code/seed_control.cpp` | Seed-controlled baseline. |
| `code/tests/golden-seed-1.txt` | Golden output for seed 1. |
| `run.sh` | Runs twice and compares with the golden trace. |
| `meta.yml` | Lesson metadata. |
| `EVIDENCE.md` | English evidence record. |
| `EVIDENCE-es.md` | Spanish evidence record. |

## Acceptance criterion

The lesson's **Self-check (binary)** is the authoritative checklist. `meta.yml` mirrors it for
tooling.
