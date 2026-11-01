# archaeology-18 — Refactor workshop

> [English](README.md) | [Español](README-es.md)

> **Status:** drafted · **Concept:** all failure classes · **Prerequisites:** archaeology-04,
> archaeology-05, archaeology-08, archaeology-09 · **Estimated time:** 180 minutes

This is the Track B capstone. The other lessons end with a diagnosis and a proposed structural fix.
This one makes you pay for it: take one legacy failure, move it to a Track A rung, keep the behavior
identical, and report what the move cost.

The workshop is a frame, not a script. You choose the failure and the rung; the lesson fixes the
method, the evidence and the acceptance criterion.

## What the learner does

- Chooses one failure from `archaeology-04`, `archaeology-05`, `archaeology-08` or `archaeology-09`.
- Promotes it to a runnable bench that writes a transcript, and records that transcript as trace A.
- Chooses a target rung: `construction-08`, `construction-09`, `construction-11`,
  `construction-12` or `construction-13`.
- Writes the change list and predicts the cost before touching the code.
- Refactors in small steps, keeping trace A byte-identical after each one.
- Verifies the build, the traces, the determinism and, where it applies, the sanitizer.
- Measures the cost and writes the verdict.
- Fills `WORKSHOP-REPORT.md` (or the Spanish edition) and `EVIDENCE.md`.

## The two proofs

A refactor owes two proofs, not one:

| Proof | Question | Evidence |
|---|---|---|
| Behavior preservation | Did anything the program does change? | Trace A byte-identical before and after |
| Capability gain | Did the change remove a class of failure? | The target rung's own change test from `lesson-catalog.md` |

A refactor that preserves the traces and buys nothing is a rename. A refactor that buys something
and changes the output is a rewrite. The workshop accepts only the first shape.

## The workshop at a glance

```text
README.md
README-es.md
lesson.md
lesson-es.md
meta.yml
EVIDENCE.md
EVIDENCE-es.md
WORKSHOP-REPORT.md
WORKSHOP-REPORT-es.md
```

`WORKSHOP-REPORT.md` is the deliverable: baseline, target, plan, refactor, verification, cost table,
verdict. `EVIDENCE.md` is the measurement log: commands, raw output, counts, diffs, and the boundary
between what you verified and what you inferred. The report argues; the evidence proves.

## Acceptance criterion

The lesson's **Self-check (binary)** is the authoritative checklist. `meta.yml` mirrors it for
tooling.
