# archaeology-03 — Reverse-engineer the spec

> [English](README.md) | [Español](README-es.md)

> **Status:** drafted · **Concept:** behavior specification · **Prerequisite:** archaeology-02 ·
> **Estimated time:** 90 minutes

This lesson reverse-engineers the behavior of the archived `variant-01` source. The output is a
behavior spec and at least one captured legacy trace. The modern lab spec is a comparison target,
not evidence for what the 2008 program did.

## What the learner does

- Reads the source and records the rules it actually implements.
- Separates the game rules from the random generator and the console presentation.
- Captures a legacy trace with a fixed seed using `capture_trace.sh`.
- Fills `SPEC-TEMPLATE.md` and `TRACE-TEMPLATE.md`.
- Compares the reconstructed spec with `common/spec/game-rules.md` and records the differences.

## The original

```text
<archive>/original/variant-01-baseline-2008/source/guerra-de-caballos-baseline.cpp
```

The original calls `srand(time(NULL))` and `rand()`. It has no seed argument, no transcript format
and no test harness. The probe patches the seed in a scratch copy; the archive is not edited.

## Capture a trace

From this lesson directory:

```bash
./code/capture_trace.sh /path/to/archive 12345 /tmp/legacy-trace-12345.txt
```

The output reports the seed, output path, run status, stdout lines and stderr lines. The trace is
the raw legacy stdout: a CP437 board and Spanish console messages, not the modern transcript.

## Files

| File | Purpose |
|---|---|
| `README.md` | This file. |
| `README-es.md` | Spanish edition. |
| `lesson.md` | The guided lesson. |
| `lesson-es.md` | Spanish edition of the guided lesson. |
| `SPEC-TEMPLATE.md` | Behavior spec template. |
| `SPEC-TEMPLATE-es.md` | Spanish edition of the spec template. |
| `TRACE-TEMPLATE.md` | Legacy trace template. |
| `TRACE-TEMPLATE-es.md` | Spanish edition of the trace template. |
| `code/capture_trace.sh` | Copies the source, fixes the seed, compiles and runs it. |
| `code/compat/iostream.h` | Compatibility header for the pre-standard include. |
| `code/compat/conio.h` | No-op `getch`. |
| `code/legacy_wrapper.cpp` | Standard `int main` around the renamed legacy entry point. |
| `meta.yml` | Lesson metadata. |
| `EVIDENCE.md` | English evidence record. |
| `EVIDENCE-es.md` | Spanish evidence record. |

## Acceptance criterion

The lesson's **Self-check (binary)** is the authoritative checklist. `meta.yml` mirrors it for
tooling.
