# Track A — Construction (Construcción)

> [English](construction-track.md) | [Español](construction-track-es.md)

> Build the same game through a sequence of design decisions. The game never changes; the cost of
> the next change is the lesson.

## Goal and audience

**Goal:** the learner can choose a design and justify it by the cost of the next change.

**Verification:** acceptance criteria, a runnable artifact and an evidence record.

**Audience:** from motivated beginner to intermediate developer. No C++ expertise is required; each
lesson teaches the minimum syntax its concept needs.

**Prerequisite:** variables, conditionals and loops. If the learner cannot read a `for`, start with
`construction-00`.

The catalog is authoritative for IDs, prerequisites, time and status:
[lesson catalog](lesson-catalog.md).

---

## How the track works

Every lesson changes one design decision and keeps the behavior fixed. The learner:

1. predicts the effect of the change before running;
2. runs both versions with the same seed;
3. compares the prediction with the observation;
4. explains the gap in plain language;
5. records the evidence and the next step.

The loop is the same in every lesson: **predict → run → compare → explain → record**.

## The ladder

The lessons are grouped by the kind of design decision they teach. The order inside a group is the
recommended order; the catalog has the exact prerequisites.

### Core ladder

| ID | Concept | Change test |
|---|---|---|
| construction-00 | Specification | Predict the next move of a seeded game |
| construction-01 | Arbitrary control flow | Add “skip turn”; count rewired jumps |
| construction-02 | Structured control flow | Same change, localized; no `goto` |
| construction-03 | Scope and decomposition | Extract a move without globals |
| construction-04 | Aggregation | Add `captured` to a knight |
| construction-05 | Collection and traversal | Traverse the board without 64 variables |
| construction-06 | Aliasing without addresses | Mutate a record through a parameter |
| construction-07 | Indirection and lifetime | Access a cell through a pointer; explain the row stride |
| construction-08 | Information hiding | Board 8×8 → 10×10; only the board module changes |
| construction-09 | Representation independence | Swap the board representation; callers compile |

### Class and generic ladder

| ID | Concept | Change test |
|---|---|---|
| construction-10 | Validity and lifecycle | Out-of-range writes rejected; guard removal exposes invalid state |
| construction-11 | Class with invariant | Private state preserves the invariant; public state exposes it |
| construction-12 | Late binding | Add a Piece subclass; zero loop edits |
| construction-13 | Composition over inheritance | Replace inheritance with delegation; traces unchanged |
| construction-14 | Type parameters | One move generator for two piece types |
| construction-15 | Constraints | Same engine instantiates 8×8 and 10×10; 8×8 traces pass |

### Lenses

| ID | Concept | Change test |
|---|---|---|
| construction-16 | Ownership and lifetime | `unique_ptr` ownership; copy deleted; move available |
| construction-17 | Iteration abstraction | Same transcript after replacing three traversals |
| construction-18 | Purity and immutability | Undo/redo free via board history |
| construction-19 | Locality | 10,000 games; measured profile |
| construction-20 | Ports and adapters | Same core, console and test adapters |
| construction-21 | Shared state vs messages | Race detected and removed; determinism kept |
| construction-22 | Recovery | Invalid move handled without `exit` |

### Port and capstone

| ID | Concept | Change test |
|---|---|---|
| construction-23 | Syntax is disposable | Same traces from the Python port |
| construction-24 | Tradeoff justification | Defend a design choice with numbers |

## Recommended path

`construction-00 → construction-01 → construction-02 → construction-03 → construction-04 →
construction-05 → construction-06 → construction-07 → construction-08 → construction-09 →
construction-10 → construction-11 → construction-12 → construction-14 → construction-15`

After the spine, pick two lenses. `construction-20` is the default first lens; if you pick
`construction-18`, read `construction-17` first. Then take the capstone:
`construction-24`.

The path is a recommendation, not a gate. The catalog's `Prereq` column is authoritative.

---

## Rules

1. **One variable.** Exactly one critical aspect changes per lesson. Everything else stays fixed.
2. **Same behavior.** Every variant passes the same golden traces. If behavior changes, the lesson
   is invalid.
3. **Measure the cost.** Record files touched, lines changed, concepts introduced and tests broken.
   Artifact size is measured, not prescribed: later lessons are larger because the design is larger.
4. **Forced by syntax.** If the variant compiles without the concept, the concept is decoration.
5. **Predict before running.** The gap between prediction and observation is the lesson.

---

## The contrast-pair protocol

Every concept lesson ships two runnable versions. Setup lessons (`common-00`…`common-05`), the port
(`construction-23`) and the capstone (`construction-24`) use acceptance criteria instead.

The learner does six things:

1. **Predict** the behavior of both versions without running them; write it down.
2. **Run** both with the same seed; compare transcripts.
3. **Compare** the prediction with the observation; name the exact difference.
4. **Diff** the structure: which files, functions and data changed?
5. **Explain** the difference in one sentence, without naming the syntax.
6. **Measure** the change test on both.

The lesson is complete when the learner can say *what decision* changed and *what it bought*, not
*what keyword* appeared.

---

## The change test

A change test is a small requirement applied to two versions. It measures **cost**, not correctness.

Predict the cost first, then measure it. The difference between the two is the calibration signal: a
learner who cannot estimate the cost does not yet see the coupling.

Record four numbers:

| Metric | Why |
|---|---|
| Files touched | Coupling |
| Lines changed | Blast radius |
| New concepts required | Learning cost |
| Tests broken | Safety net |

Example: add a third piece type.

| Version | Files | Lines | New concepts | Tests broken |
|---|---|---|---|---|
| A switch-based rung | 3 | ~120 | 0 | 2 |
| A dispatch-based rung | 1 new + 1 edit | ~40 | 2 | 0 |

The lesson is not “dispatch is better”. The lesson is “this change cost 40 lines instead of 120 and
bought a new concept”. The next change may reverse the verdict.

---

## Where to start

- New learner: [START-HERE.md](../../START-HERE.md).
- Lesson index: [lesson catalog](lesson-catalog.md).
- Evidence rules: [verification](verification.md).
- Pedagogy: [how learning works](how-learning-works.md).
