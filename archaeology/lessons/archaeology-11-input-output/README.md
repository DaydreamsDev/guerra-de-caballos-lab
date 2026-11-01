# archaeology-11 — Input, output, and testability

> [English](README.md) | [Español](README-es.md)

> **Status:** drafted · **Concept:** input/output separation and testability · **Prerequisite:**
> archaeology-09 · **Estimated time:** 60 minutes

This lesson separates a game core from the console. The core returns a `GameResult`; a console
adapter prints it; a test adapter calls the core without any console.

## What the learner does

- Reads the core interface and the two adapters.
- Runs the console path and counts the lines.
- Runs the test path and checks the winner and move count.
- Records what the core can do without a console and what still couples it to a transcript.

## The core

```text
code/core.h
code/core.cpp
code/console.h
code/console.cpp
code/test.cpp
code/main.cpp
```

`run_game` returns `GameResult{lines, winner, moveCount}`. It includes no `<cstdio>` and calls no
printing function. `console.cpp` is the only file that prints the transcript; `test.cpp` prints its
own test report. `test.cpp` calls `run_game` directly.

This is a small bench, not the full horse game. It isolates the I/O boundary.

## Build and run

```bash
./run.sh
```

Expected output:

```text
console_lines=8
test_output=lines=8 winner=player moves=3
test=ok
match=yes
```

## Files

| File | Purpose |
|---|---|
| `README.md` | This file. |
| `README-es.md` | Spanish edition. |
| `lesson.md` | The guided lesson. |
| `lesson-es.md` | Spanish edition of the guided lesson. |
| `code/core.h` | Core result type and entry point. |
| `code/core.cpp` | Core logic; no console. |
| `code/console.h` | Console adapter declaration. |
| `code/console.cpp` | Console adapter; the only printer. |
| `code/test.cpp` | Test adapter. |
| `code/main.cpp` | Console entry point. |
| `run.sh` | Compiles and runs both adapters. |
| `meta.yml` | Lesson metadata. |
| `EVIDENCE.md` | English evidence record. |
| `EVIDENCE-es.md` | Spanish evidence record. |

## Acceptance criterion

The lesson's **Self-check (binary)** is the authoritative checklist. `meta.yml` mirrors it for
tooling.
