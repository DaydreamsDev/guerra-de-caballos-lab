# archaeology-16 — Platform lock-in

> [English](README.md) | [Español](README-es.md)

> **Status:** drafted · **Concept:** platform lock-in and adapters · **Prerequisite:**
> archaeology-01 · **Estimated time:** 60 minutes

This lesson isolates the console calls that lock the archive to Windows. A small `platform` adapter
provides color, clear and key input through a portable interface; the caller no longer includes
`conio.h` or calls `system("color")` directly.

## What the learner does

- Reads the archive's `conio.h`, `iostream.h`, `system("color 1f")`, `system("cls")` and `getch()`.
- Reads the `platform.h` interface and the POSIX implementation.
- Runs the adapter and confirms the caller builds without platform headers.
- Records what the adapter hides and what it does not guarantee across terminals.

## The adapter

```text
code/platform.h
code/platform.cpp
code/main.cpp
```

`platform.h` exposes `set_console_color`, `clear_console` and `read_console_key`. The implementation
uses ANSI escape sequences and `std::getchar`; it does not include `conio.h`.

## Build and run

```bash
./run.sh
```

Expected output:

```text
adapter=portable
```

The program writes ANSI escapes before `portable=yes`; the runner checks the marker.

## Files

| File | Purpose |
|---|---|
| `README.md` | This file. |
| `README-es.md` | Spanish edition. |
| `lesson.md` | The guided lesson. |
| `lesson-es.md` | Spanish edition of the guided lesson. |
| `code/platform.h` | Portable console interface. |
| `code/platform.cpp` | POSIX/ANSI implementation. |
| `code/main.cpp` | Caller that uses the interface. |
| `run.sh` | Compiles and runs the adapter. |
| `meta.yml` | Lesson metadata. |
| `EVIDENCE.md` | English evidence record. |
| `EVIDENCE-es.md` | Spanish evidence record. |

## Acceptance criterion

The lesson's **Self-check (binary)** is the authoritative checklist. `meta.yml` mirrors it for
tooling.
