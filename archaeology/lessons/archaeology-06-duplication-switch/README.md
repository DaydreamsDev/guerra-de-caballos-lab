# archaeology-06 — Duplication and the eight-case switch

> [English](README.md) | [Español](README-es.md)

> **Status:** drafted · **Concept:** duplication and extraction · **Prerequisite:** archaeology-04 ·
> **Estimated time:** 60 minutes

This lesson isolates the eight-case movement switch from the archived code and replaces the repeated
case bodies with one table. The behavior is compared before and after; the test must report zero
mismatches.

## What the learner does

- Reads the original eight-case structure and counts the repeated deltas.
- Runs the baseline, which keeps both `apply_switch` and `apply_table`.
- Confirms that all 200 move/position comparisons match.
- Removes the switch in a scratch copy and keeps the table; the test still passes.
- Records the extraction cost: lines, cases, table entries and test result.

## The baseline

```text
code/movement.cpp
```

The baseline has:

- `apply_switch` — eight explicit `case` bodies.
- `apply_table` — one `std::array<Delta, 8>` and one indexed lookup.
- `main` — compares both functions for 8 moves × 25 positions = 200 comparisons.

The original switch also contains bounds checks, visited checks and capture logic. This baseline
isolates the duplicated delta calculation; it does not pretend to be the whole rule.

## Build and run

From this directory:

```bash
./run.sh
```

Expected output:

```text
comparisons=200 mismatches=0
```

## Files

| File | Purpose |
|---|---|
| `README.md` | This file. |
| `README-es.md` | Spanish edition. |
| `lesson.md` | The guided lesson. |
| `lesson-es.md` | Spanish edition of the guided lesson. |
| `code/movement.cpp` | Switch and table implementations plus the comparison test. |
| `run.sh` | Compiles and runs the baseline. |
| `meta.yml` | Lesson metadata. |
| `EVIDENCE.md` | English evidence record. |
| `EVIDENCE-es.md` | Spanish evidence record. |

## Acceptance criterion

The lesson's **Self-check (binary)** is the authoritative checklist. `meta.yml` mirrors it for
tooling.
