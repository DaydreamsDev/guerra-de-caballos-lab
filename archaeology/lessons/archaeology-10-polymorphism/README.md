# archaeology-10 — Missing polymorphism

> [English](README.md) | [Español](README-es.md)

> **Status:** drafted · **Concept:** polymorphism · **Prerequisite:** archaeology-09 ·
> **Estimated time:** 90 minutes

This lesson replaces a switch on piece kind with virtual dispatch. A `Piece` interface has two
implementations; a switch baseline computes the same deltas. The test compares both for 400 cases.

## What the learner does

- Reads the `Piece` interface and the two implementations.
- Runs the comparison between the switch and the dispatch path.
- Confirms 400 comparisons and zero mismatches.
- Adds a third piece in a scratch copy and counts the edits in each path.
- Records what dispatch bought and what it cost.

## The baseline

```text
code/polymorphism.cpp
```

`Knight` and `MirrorKnight` implement `Piece::move`. `delta_with_switch` keeps the switch path for
comparison. `main` compares both paths for 2 pieces × 8 moves × 25 positions = 400 cases.

## Build and run

```bash
./run.sh
```

Expected output:

```text
comparisons=400 mismatches=0
```

## Files

| File | Purpose |
|---|---|
| `README.md` | This file. |
| `README-es.md` | Spanish edition. |
| `lesson.md` | The guided lesson. |
| `lesson-es.md` | Spanish edition of the guided lesson. |
| `code/polymorphism.cpp` | Interface, implementations, switch baseline and comparison. |
| `run.sh` | Compiles and runs the baseline. |
| `meta.yml` | Lesson metadata. |
| `EVIDENCE.md` | English evidence record. |
| `EVIDENCE-es.md` | Spanish evidence record. |

## Acceptance criterion

The lesson's **Self-check (binary)** is the authoritative checklist. `meta.yml` mirrors it for
tooling.
