# failure-015 — `conio.h`, `iostream.h` and `system("color")` bind the build to Windows

> [English](failure-015-platform-lock-in.md) | [Español](failure-015-platform-lock-in-es.md)

- **Class:** robustness
- **Severity:** high
- **Concept violated:** a build depends on its stated environment, not on an accident
- **Archive:** tag `v1.0.0`,
  `original/variant-01-baseline-2008/source/guerra-de-caballos-baseline.cpp`

## Evidence

- **Variant:** `variant-01-baseline-2008`. The same shape repeats in every 2008 variant.
- **Lines:** 11, 12, 32, 38, 42–43, 781–864.
- **Source:**

```cpp
#include<conio.h>                          // line 11
#include<iostream.h>                       // line 12

void  main()                                // line 32
{
    system("color 1f");                     // line 38
    getch();                                // line 42
    system("cls");                          // line 43
    // ...
    getch();                                // lines 781, 806, 835, 861
    system("cls");                          // lines 784, 809, 838, 864
}
```

`conio.h` is a DOS/Windows console header. `iostream.h` is the pre-standard iostream header.
`void main()` is not a standard entry point. `system("color 1f")` and `system("cls")` are
Windows shell commands; `getch()` reads a key without echo and is not standard C++.

## Symptom

The archive does not compile on Linux or macOS. The first error is:

```text
fatal error: conio.h: No such file or directory
```

A modern Microsoft compiler also rejects `iostream.h`, because the header was removed from the
standard. On a POSIX shell, `system("cls")` starts a shell, fails to find `cls`, prints an
error and exits 127; the archive ignores the status and continues as if the screen had been
cleared.

## Root cause

The code targets one compiler, one operating system and one console. `conio.h` was the
available key reader, `system("color 1f")` the shortest color path, and `system("cls")` the
screen management the target offered. Those were local choices for a local program.

The artifact never named its platform, so the boundary was missing until the code left the
machine. The lock-in has three layers: the language dialect (`iostream.h`, `void main`), the
platform API (`conio.h`, `getch`) and the terminal behavior (`color`, `cls`).

## Consequence

A reader who clones the repository on Linux cannot build the artifact at all. The failure is
visible at compile time, which makes it cheaper than a memory error, but it blocks every other
experiment. The runtime commands fail silently where they do not exist, so a partial port can
compile and still behave differently.

For a published archive, a failed build is a high-severity defect. For a one-machine demo, the
same code is defensible. The severity depends on the stated context.

## Test that catches it

`archaeology/lessons/archaeology-16-platform-lock-in/run.sh` builds a caller against a
three-function console port:

```text
adapter=portable
```

The lesson then writes a second adapter and links the **same** `main.cpp` against it:

```text
portable=yes
```

The caller source does not change. The ANSI adapter writes 30 bytes; the plain adapter writes
13. The lesson proves the caller has no platform header with `grep` and `nm -C -u`, and shows
that `read_console_key` is declared, defined and never called.

That is interface portability, not behavior identity. The key contract is unverified.

## Minimal fix

Replace the console calls in the caller with a small port:

```cpp
void set_console_color();
void clear_console();
int read_console_key();
```

Put the platform code in `platform.cpp`. The caller includes only `platform.h` and the
standard library. This removes the compile-time lock and gives a reviewer one file to audit.

It does not fix `iostream.h`, `void main` or the CP437 glyphs. A console adapter is not a
language migration.

## Structural fix

State the capabilities the port assumes, add a no-op adapter for tests and pipes, and let the
caller choose the adapter. If the program needs real capability negotiation, use a terminal
library (`ncurses`, `PDCurses`) instead of hand-written escapes. Keep the theme and the pause
policy in the caller or a presentation layer, not inside the adapter.

Then a test can link the no-op adapter and assert the caller's behavior without a terminal.

Track A rungs: [`construction-08-modular`][c08] for the module boundary and
[`construction-20-hexagonal`][c20] for ports and adapters. `common-04` is the golden-trace
counterpart: freeze the console output before swapping the adapter.

[c08]: ../../construction/lessons/construction-08-modular/lesson.md
[c20]: ../../construction/lessons/construction-20-hexagonal/lesson.md

## Confidence

High that the six constructs are in the archive and that they block a modern build. High that
the adapter makes the caller compile without `conio.h`. Medium on behavior identity: the lesson
measures 30 versus 13 bytes and never calls the key function.

## Note on the archive

This card is evidence, not a repair of the archive. The archive stays read-only; the port and
the adapters belong in a scratch copy or in `reference/`.
