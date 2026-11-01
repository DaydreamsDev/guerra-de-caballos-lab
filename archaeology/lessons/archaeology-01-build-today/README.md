# archaeology-01 — Build and run legacy code

> [English](README.md) | [Español](README-es.md)

> **Status:** drafted · **Concept:** build and run legacy code · **Prerequisite:** common-03 ·
> **Estimated time:** 62 minutes

This lesson takes the archived `variant-01` source and attempts to build and run it on a modern
Linux machine. The archive is not edited. A small compatibility probe is built in a scratch
directory, and the result is recorded as either a runnable build or a documented blocker.

## What the learner does

- Predicts what the legacy console program will do.
- Tries a modern build and records the errors.
- Uses the compatibility probe in `code/` to compile the unchanged source.
- Runs the probe with a timeout and records the output, the errors and the differences from the
  prediction.
- Writes the evidence, including what the probe does not prove.

## The original

```text
<archive>/original/variant-01-baseline-2008/source/guerra-de-caballos-baseline.cpp
```

The source is 930 lines, uses `#include<iostream.h>`, `#include<conio.h>`, `void main()`,
`system("color 1f")`, `system("cls")`, `getch()`, `srand(time(NULL))` and `rand()`.

## The probe

| File | Purpose |
|---|---|
| `code/compat/iostream.h` | Maps the pre-standard header to `<iostream>` and `using namespace std`. |
| `code/compat/conio.h` | Provides a no-op `getch()`. |
| `code/legacy_wrapper.cpp` | Calls the renamed legacy entry point from a standard `int main`. |
| `code/build_probe.sh` | Copies the archive source to a temporary directory, compiles, links and runs it. |

The probe does not edit the archive. It renames `main` to `legacy_main` with
`-Dmain=legacy_main` and supplies a wrapper. It uses `-fpermissive` because the historical source is
not modern C++.

## Run the probe

From this lesson directory:

```bash
./code/build_probe.sh /path/to/archive
```

On this machine the output is:

```text
source=<archive>/original/variant-01-baseline-2008/source/guerra-de-caballos-baseline.cpp
compile=ok
run_status=0
stdout_lines=614
stderr_lines=26
stderr_first=sh: línea 1: color: orden no encontrada
```

The line counts vary because the program uses `time(NULL)` and `rand()`. The stderr lines are the
Windows `color` and `cls` commands failing on Linux. The stdout is a CP437 board rendered as
replacement characters, not the modern transcript.

## What the probe proves

- The source can be compiled with a compatibility layer.
- The program can run to completion with `getch()` as a no-op.
- The Windows console calls fail on Linux.
- The output is not deterministic and is not the modern transcript contract.

## What the probe does not prove

- That the historical code is portable.
- That the output is correct.
- That the historical toolchain was Microsoft Visual C++ 6.0; that claim is documented in the
  private vault and is not independently verifiable from the public archive.
- That the modern transcript can be reproduced from this source without a separate reconstruction.

## Acceptance criterion

The lesson's **Self-check (binary)** is the authoritative checklist. `meta.yml` mirrors it for
tooling.
