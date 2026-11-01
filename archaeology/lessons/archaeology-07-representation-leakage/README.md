# archaeology-07 — Representation leakage and parallel arrays

> [English](README.md) | [Español](README-es.md)

> **Status:** drafted · **Concept:** representation leakage and records · **Prerequisite:**
> archaeology-05 · **Estimated time:** 60 minutes

This lesson replaces loose knight variables and a parallel board array with a `Knight` record and a
`Board` value. The before and after programs run the same scenario and must print the same summary.

## What the learner does

- Reads the parallel-array version and the record version.
- Runs both and compares the output.
- Names every caller that had to change.
- Records the cost: files, lines, new concepts and the comparison result.

## The baseline

```text
code/parallel_arrays.cpp
code/record.cpp
```

`parallel_arrays.cpp` uses `playerRow`, `playerColumn`, `machineRow`, `machineColumn` and a
`std::array<bool, 64>`.

`record.cpp` uses `Knight`, a `Board` class with a private visited array and `visited_count()`.

Both print:

```text
player=2,3 machine=3,4 visited=3
```

## Run the comparison

```bash
./run.sh
```

Expected output:

```text
parallel=player=2,3 machine=3,4 visited=3
record=player=2,3 machine=3,4 visited=3
match=yes
```

## Files

| File | Purpose |
|---|---|
| `README.md` | This file. |
| `README-es.md` | Spanish edition. |
| `lesson.md` | The guided lesson. |
| `lesson-es.md` | Spanish edition of the guided lesson. |
| `code/parallel_arrays.cpp` | The parallel-array version. |
| `code/record.cpp` | The record and board version. |
| `run.sh` | Compiles both and compares the output. |
| `meta.yml` | Lesson metadata. |
| `EVIDENCE.md` | English evidence record. |
| `EVIDENCE-es.md` | Spanish evidence record. |

## Acceptance criterion

The lesson's **Self-check (binary)** is the authoritative checklist. `meta.yml` mirrors it for
tooling.
