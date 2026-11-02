# Track B — Archaeology (Arqueología)
> [English](README.md) | [Español](README-es.md)

**Question:** *What is wrong with this real code, why does it matter, and what should it look like?*

This track uses the archived legacy code as a real system. The learner diagnoses defects, explains
their consequences, writes a failing test, applies the smallest safe repair, and verifies that the
behavior is preserved.

The autopsy is blameless: context before judgment. It is not a joke, not a roast and not a
mockery; the goal is professional diagnosis.

## Context

The source was written by a second-semester systems engineering student at the National University
of Colombia, Bogotá, for an OOP course that followed basic programming. That is the only context
this track needs; the lessons focus on the engineering.

## What the learner does

- Reads the real source and its documentation.
- Reproduces a failure.
- Writes a test that captures it.
- Applies the smallest repair.
- Confirms that the golden traces still pass.
- Records the evidence.

## Scope

The minimum first-release scope is `archaeology-00` through `archaeology-10`, plus the first 15
failure cards. The full track is drafted through `archaeology-19`.

## Status

`archaeology-00` through `archaeology-19` are drafted. Track B lessons are complete. The
first-release failure cards are 15 of 15. See [Failure cards](cards/README.md).

- [Orientation: read a legacy codebase](lessons/archaeology-00-orientation/lesson.md)
- [Build and run legacy code](lessons/archaeology-01-build-today/lesson.md)
- [Code inventory and call graph](lessons/archaeology-02-code-inventory/lesson.md)
- [Reverse-engineer the spec](lessons/archaeology-03-reverse-spec/lesson.md)
- [Correctness failures](lessons/archaeology-04-correctness-failures/lesson.md)
- [Global state and hidden coupling](lessons/archaeology-05-global-state/lesson.md)
- [Duplication and the eight-case
  switch](lessons/archaeology-06-duplication-switch/lesson.md) (optional)
- [Representation leakage](lessons/archaeology-07-representation-leakage/lesson.md) (optional)
- [Missing module boundary](lessons/archaeology-08-module-boundary/lesson.md)
- [Missing abstract data type](lessons/archaeology-09-abstract-data-type/lesson.md)
- [Missing polymorphism](lessons/archaeology-10-polymorphism/lesson.md) (optional)
- [Input, output, and testability](lessons/archaeology-11-input-output/lesson.md) (optional)
- [Naming and comments](lessons/archaeology-12-naming-comments/lesson.md) (optional)
- [Technical documentation for
  maintainers](lessons/archaeology-13-documentation/lesson.md) (optional)
- [Tests and determinism](lessons/archaeology-14-tests-determinism/lesson.md)
- [Static analysis and compiler warnings](lessons/archaeology-15-static-analysis/lesson.md)
- [Platform lock-in](lessons/archaeology-16-platform-lock-in/lesson.md) (optional)
- [Code review of a legacy function](lessons/archaeology-17-code-review/lesson.md)
- [Refactor workshop](lessons/archaeology-18-refactor-workshop/lesson.md)
- [Review checklist and retrospective](lessons/archaeology-19-review-retrospective/lesson.md)

**Optional lessons (safe to skip):** `archaeology-06`, `archaeology-07`,
`archaeology-10`, `archaeology-11`, `archaeology-12`, `archaeology-13`,
`archaeology-16`.

The spine reaches the retrospective without them. Take them for the complete ladder or skip them
for the shortest path. No core lesson depends on an optional lesson.

See [`../docs/guide/archaeology-track.md`](../docs/guide/archaeology-track.md) and
[`../docs/guide/lesson-catalog.md`](../docs/guide/lesson-catalog.md).
