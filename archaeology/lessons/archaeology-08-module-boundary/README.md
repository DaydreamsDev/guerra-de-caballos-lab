# archaeology-08 — Missing module boundary

> [English](README.md) | [Español](README-es.md)

> **Status:** drafted · **Concept:** module boundary · **Prerequisite:** archaeology-05 ·
> **Estimated time:** 90 minutes

This lesson turns a transparent board array into a module with a header and an implementation. The
header exposes operations; the representation is private to `board.cpp`.

## What the learner does

- Reads the archive's single-translation-unit board access.
- Reads the `board.h` / `board.cpp` / `main.cpp` split.
- Runs the module and records the public surface.
- Tries to access the visited array from `main.cpp`; the compiler rejects it.
- Records the boundary: what callers know, what they do not, and what can change without touching
  them.

## The module

```text
code/board.h
code/board.cpp
code/main.cpp
```

`Board` exposes `reset`, `mark`, `is_visited` and `visited_count`. The visited array and the index
arithmetic are private.

## Build and run

```bash
./run.sh
```

Expected output:

```text
visited=2 first=1 second=1
```

## Files

| File | Purpose |
|---|---|
| `README.md` | This file. |
| `README-es.md` | Spanish edition. |
| `lesson.md` | The guided lesson. |
| `lesson-es.md` | Spanish edition of the guided lesson. |
| `code/board.h` | Public interface. |
| `code/board.cpp` | Hidden representation and implementation. |
| `code/main.cpp` | Caller that uses only the public interface. |
| `run.sh` | Compiles and runs the module. |
| `meta.yml` | Lesson metadata. |
| `EVIDENCE.md` | English evidence record. |
| `EVIDENCE-es.md` | Spanish evidence record. |

## Acceptance criterion

The lesson's **Self-check (binary)** is the authoritative checklist. `meta.yml` mirrors it for
tooling.
