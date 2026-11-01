# archaeology-04 — Correctness failures

> [English](README.md) | [Español](README-es.md)

> **Status:** drafted · **Concept:** correctness failures · **Prerequisite:** archaeology-03 ·
> **Estimated time:** 90 minutes

This lesson diagnoses six correctness failures in the archived code. Each failure becomes a card
with evidence, symptom, root cause, consequence, the test that catches it, a minimal fix and a
structural fix linked to a Track A rung. The archive is read-only; the tests are isolated
reproductions, not tests of the original.

## What the learner does

- Runs `tests/run_failure_tests.sh` against the archive.
- Reads the six source locations and separates symptom from root cause.
- Writes the six failure cards in `cards/`.
- Reproduces each failure with the test that catches it.
- Proposes a minimal fix and a structural fix for each one.
- Ranks the six failures by cost.
- Records the evidence in `EVIDENCE.md`.

## The archive

```text
https://github.com/DaydreamsDev/DaydreamsDev-guerra-de-caballos-archive
tag v1.0.0
```

The archive is frozen. The lesson reads it; it does not edit it.

## The six failures

| Card | Failure | Variant:line | Severity |
|---|---|---|---|
| failure-001 | Loop bound writes one element past the board | `variant-01:61`, `variant-02:72`, `variant-03:82`, `variant-04:88` | Critical |
| failure-002 | `void main()` | `variant-01:32`, `variant-02:37`, `variant-04:52`; contrast `variant-03:47` | High |
| failure-003 | `srand(time(NULL))` inside the placement retry loop | `variant-01:76`, `variant-02:87`, `variant-03:99`, `variant-04:105` | High |
| failure-004 | Broken pointer initialization | `variant-03:84-86` | High |
| failure-006 | Unbounded name input | `variant-02:46`, `variant-03:56`, `variant-04:62` | Critical |
| failure-005 | Inverted win condition | `variant-00:299` | High |

## Run the tests

From this lesson directory:

```bash
bash tests/run_failure_tests.sh /path/to/archive
```

A reference run on this machine prints:

```text
failure-001=detected
failure-003=detected
failure-004=detected
failure-005=detected
failure-002=detected
failure-006=detected
```

The C++ harness prints the first four lines; the shell script prints the last two. The order is
fixed. `detected` means the pattern was reproduced, not that the original program was executed.
The script exits `0` when all six lines say `detected` and non-zero otherwise.

## Files

| File | Purpose |
|---|---|
| `README.md` | This file. |
| `README-es.md` | Spanish edition. |
| `lesson.md` | The guided lesson. |
| `lesson-es.md` | Spanish edition of the guided lesson. |
| `meta.yml` | Lesson metadata. |
| `EVIDENCE.md` | English evidence record. |
| `EVIDENCE-es.md` | Spanish evidence record. |
| `tests/failure_tests.cpp` | Four isolated reproductions: 001, 003, 004, 006. |
| `tests/run_failure_tests.sh` | Runs the harness and the two source checks: 002, 005. |
| `../../cards/failure-001-loop-bound.md` | Card 001, English. |
| `../../cards/failure-001-loop-bound-es.md` | Card 001, Spanish. |
| `../../cards/failure-002-void-main.md` | Card 002, English. |
| `../../cards/failure-002-void-main-es.md` | Card 002, Spanish. |
| `../../cards/failure-003-seed-in-loop.md` | Card 003, English. |
| `../../cards/failure-003-seed-in-loop-es.md` | Card 003, Spanish. |
| `../../cards/failure-004-broken-initialization.md` | Card 004, English. |
| `../../cards/failure-004-broken-initialization-es.md` | Card 004, Spanish. |
| `../../cards/failure-006-unbounded-name.md` | Card 006, English. |
| `../../cards/failure-006-unbounded-name-es.md` | Card 006, Spanish. |
| `../../cards/failure-005-inverted-win.md` | Card 005, English. |
| `../../cards/failure-005-inverted-win-es.md` | Card 005, Spanish. |

## Acceptance criterion

The lesson's **Self-check (binary)** is the authoritative checklist. `meta.yml` mirrors it for
tooling.
