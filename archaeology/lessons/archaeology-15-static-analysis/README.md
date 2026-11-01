# archaeology-15 — Static analysis and compiler warnings

> [English](README.md) | [Español](README-es.md)

> **Status:** drafted · **Concept:** static analysis · **Prerequisite:** archaeology-04 ·
> **Estimated time:** 60 minutes

This lesson uses compiler warnings as evidence. The learner compiles a small bench with warnings,
triages each warning, fixes it without changing behavior, and reaches a clean build with
`-Wall -Wextra -Werror`.

## What the learner does

- Compiles `warnings.cpp` with `-Wall -Wextra` and reads every warning.
- Classifies each warning: real defect, harmless but removable, or false positive.
- Compares `warnings.cpp` and `fixed.cpp`.
- Verifies the output is identical and the fixed build has zero warnings.

## Files

| File | Purpose |
|---|---|
| `README.md` | This file. |
| `README-es.md` | Spanish edition. |
| `lesson.md` | The guided lesson. |
| `lesson-es.md` | Spanish edition of the guided lesson. |
| `code/warnings.cpp` | The bench with warnings. |
| `code/fixed.cpp` | The behavior-preserving fixed bench. |
| `run.sh` | Compiles both and compares output. |
| `meta.yml` | Lesson metadata. |
| `EVIDENCE.md` | English evidence record. |
| `EVIDENCE-es.md` | Spanish evidence record. |

## Acceptance criterion

The lesson's **Self-check (binary)** is the authoritative checklist. `meta.yml` mirrors it for
tooling.
