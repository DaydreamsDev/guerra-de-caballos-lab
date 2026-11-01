# Track A — Construction (Construcción)
> [English](README.md) | [Español](README-es.md)

**Question:** *How do I build the same thing with a different way of thinking?*

This track builds the same game behavior through a sequence of design decisions. The game is the
invariant; the cost of the next change is the lesson.

## What the learner does

- Starts from a running artifact.
- Predicts the effect of one design change.
- Implements only that change.
- Compares the observed behavior with the prediction.
- Explains the gap.
- Records the evidence.

## Scope

The minimum first-release scope is `construction-00` through `construction-09`:

- control flow;
- aggregation;
- indirection;
- modularity;
- abstract data type.

The full ladder is drafted through `construction-24`. The modern class version is covered by:

- `construction-11` — encapsulation.
- `construction-12` — polymorphism.
- `construction-13` — composition.

The historical class rung is missing in the archive. These lessons are modern curriculum
extensions, not reconstructed historical code.

## Status

`construction-00` through `construction-24` are drafted:
[Read the rules, run the reference baseline](lessons/construction-00-read-the-rules/lesson.md),
[Non-structured programming](lessons/construction-01-non-structured/lesson.md),
[Structured programming](lessons/construction-02-structured/lesson.md),
[Functions and parameters](lessons/construction-03-functions/lesson.md),
[Records](lessons/construction-04-records/lesson.md),
[Array of records](lessons/construction-05-array-of-records/lesson.md),
[References](lessons/construction-06-references/lesson.md),
[Pointers](lessons/construction-07-pointers/lesson.md),
[Modular programming](lessons/construction-08-modular/lesson.md),
[Abstract data type](lessons/construction-09-abstract-data-type/lesson.md),
[Abstract data type invariants](lessons/construction-10-adt-invariants/lesson.md),
[Encapsulation](lessons/construction-11-encapsulation/lesson.md),
[Polymorphism](lessons/construction-12-polymorphism/lesson.md),
[Composition](lessons/construction-13-composition/lesson.md),
[Generic functions](lessons/construction-14-generic-functions/lesson.md),
[Generic classes and concepts](lessons/construction-15-generic-classes/lesson.md),
[Ownership and resource lifetime](lessons/construction-16-ownership/lesson.md),
[Standard Template Library and algorithms](lessons/construction-17-stl/lesson.md),
[Functional style](lessons/construction-18-functional/lesson.md),
[Data-oriented design](lessons/construction-19-data-oriented/lesson.md),
[Hexagonal](lessons/construction-20-hexagonal/lesson.md),
[Concurrency](lessons/construction-21-concurrency/lesson.md),
[Error contracts](lessons/construction-22-error-contracts/lesson.md),
[Port to another language](lessons/construction-23-port/lesson.md) and
[Capstone: cost report](lessons/construction-24-capstone/lesson.md). Track A is complete.

See [`../docs/guide/construction-track.md`](../docs/guide/construction-track.md) and
[`../docs/guide/lesson-catalog.md`](../docs/guide/lesson-catalog.md).
