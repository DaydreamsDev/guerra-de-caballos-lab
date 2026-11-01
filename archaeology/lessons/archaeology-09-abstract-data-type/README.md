# archaeology-09 — Missing abstract data type

> [English](README.md) | [Español](README-es.md)

> **Status:** drafted · **Concept:** abstract data type · **Prerequisite:** archaeology-08 ·
> **Estimated time:** 90 minutes

This lesson turns the board into an opaque type. `main.cpp` sees only operations; the representation
lives in one of two implementations. The same caller compiles and runs against both.

## What the learner does

- Reads the opaque `board.h`.
- Runs the caller against `board_array.cpp`.
- Runs the same caller against `board_bitmask.cpp`.
- Compares the output and confirms that `main.cpp` did not change.
- Records what the type hides and what a representation swap now costs.

## The module

```text
code/board.h
code/board_array.cpp
code/board_bitmask.cpp
code/main.cpp
```

`board.h` declares `struct Board;` and the operations. `board_array.cpp` stores a `bool[64]`.
`board_bitmask.cpp` stores one `std::uint64_t`. `main.cpp` includes only `board.h`.

## Build and run

```bash
./run.sh
```

Expected output:

```text
array=visited=2 first=1 second=1
bitmask=visited=2 first=1 second=1
match=yes
```

## Files

| File | Purpose |
|---|---|
| `README.md` | This file. |
| `README-es.md` | Spanish edition. |
| `lesson.md` | The guided lesson. |
| `lesson-es.md` | Spanish edition of the guided lesson. |
| `code/board.h` | Opaque type and operations. |
| `code/board_array.cpp` | Array representation. |
| `code/board_bitmask.cpp` | Bitmask representation. |
| `code/main.cpp` | Caller that uses only the operations. |
| `run.sh` | Compiles both representations and compares the output. |
| `meta.yml` | Lesson metadata. |
| `EVIDENCE.md` | English evidence record. |
| `EVIDENCE-es.md` | Spanish evidence record. |

## Acceptance criterion

The lesson's **Self-check (binary)** is the authoritative checklist. `meta.yml` mirrors it for
tooling.
