# construction-24 — Capstone: cost report

> [English](README.md) | [Español](README-es.md)

> **Status:** drafted · **Concept:** tradeoff justification · **Prerequisite:** construction-12 and
> construction-15 · **Estimated time:** 175 minutes

This is the Track A capstone. It has no code baseline of its own. The learner takes two existing
rungs, applies the same change to both, measures the cost, and writes a one-page report that
defends a design choice with numbers.

## The change

Add both of these to two different rungs:

- a third piece type;
- a 10×10 board.

Keep the transcript contract. All 20 golden traces must still pass on the 8×8 path.

## What the learner does

1. Choose two rungs. Recommended pairs: `construction-12` and `construction-13`, or
   `construction-14` and `construction-15`.
2. Copy each rung to a scratch directory outside the lesson directory. Do not edit the lesson
   baseline.
3. Predict the cost before touching the code.
4. Apply the same change to both copies.
5. Measure files touched, lines changed, new concepts, tests broken, compile time, binary size and
   the 20 traces.
6. Write the report with [`REPORT-TEMPLATE.md`](REPORT-TEMPLATE.md).
7. Defend the verdict: which rung was cheaper for this change, and why.

## Measuring a copy

The helper script counts source files and lines and runs the 20 golden traces:

```bash
./measure.sh ../construction-12-polymorphism
./measure.sh /tmp/capstone/rung-a
```

It prints:

```text
files=10 lines=540 traces=20/20
```

The counts include every `.h` and `.cpp` file directly under `code/`. A benchmark or probe file is
counted too; say so in the report.

## Files

| File | Purpose |
|---|---|
| `README.md` | This file. |
| `README-es.md` | Spanish edition. |
| `lesson.md` | The guided capstone. |
| `lesson-es.md` | Spanish edition of the guided capstone. |
| `REPORT-TEMPLATE.md` | The one-page report template. |
| `REPORT-TEMPLATE-es.md` | Spanish edition of the template. |
| `measure.sh` | Counts files and lines and checks the 20 traces. |
| `meta.yml` | Lesson metadata. |
| `EVIDENCE.md` | English evidence record. |
| `EVIDENCE-es.md` | Spanish evidence record. |

## Acceptance criterion

The learner delivers a one-page report that states a prediction, reports measured numbers for two
rungs, explains which number decided the verdict, and names the next change they would make. Both
scratch copies pass all 20 golden traces on the unchanged 8×8 path.
