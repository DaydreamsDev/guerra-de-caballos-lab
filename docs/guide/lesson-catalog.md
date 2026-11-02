# Lesson catalog (catálogo de lecciones)
> [English](lesson-catalog.md) | [Español](lesson-catalog-es.md)

This is the **catalog**, not the path. A learner follows one track; a teacher may pick from the
whole table. Every entry is **independently completable**.

Every entry has a stable ID, one concept, prerequisites, an estimated time, a reading time, and a
**change test** that defines success. The catalog is the source for the `meta.yml` files. Estimated
time is active step time; reading time is in `meta.yml` and the lesson header.

Status legend: `planned` · `drafted` · `in review` · `stable`.

### Status policy

The status describes evidence, not effort.

| Status | Meaning | Required evidence |
|---|---|---|
| `planned` | The ID and scope exist; no learner-facing lesson yet. | Catalog entry only. |
| `drafted` | Both language editions exist and the lesson is complete enough to run. | `lesson.md`, `lesson-es.md`, `meta.yml`, valid links, acceptance criteria. |
| `in review` | The lesson is under review: automated QA passed and an independent pedagogical review is open. | Drafted evidence plus the QA checklist and reviewer findings. |
| `stable` | The lesson passed review, one external learner completed it, and findings are closed. | In-review evidence plus learner evidence and a closed issue list. |

Promotion is not automatic. A lesson stays `drafted` until a reviewer records the evidence. The
catalog and `meta.yml` must agree. Machine tokens stay in English; Spanish prose may display
`borrador` for `drafted`.

---

## 0. Common core (núcleo común)

| ID | Title | Concept | Prereq | Time | Acceptance criteria | Status |
|---|---|---|---|---|---|---|
| [common-00](../../common/lessons/common-00-how-to-learn/lesson.md) | How to learn (cómo aprender) | Metacognition (metacognición) | none | 15 min | One predict–run–compare–explain–record cycle and one next step recorded | drafted |
| [common-01](../../common/lessons/common-01-game-invariant/lesson.md) | The game as an invariant (el juego como invariante) | Specification (especificación) | none | 50 min | Two runs identical; one ambiguity and one binary criterion recorded | drafted |
| [common-02](../../common/lessons/common-02-deterministic-harness/lesson.md) | Deterministic harness (arnés determinista) | Harness construction (construcción del arnés) | common-01 | 60 min | Same seed → same transcript, twice | drafted |
| [common-03](../../common/lessons/common-03-glossary/lesson.md) | Glossary (glosario) | Shared vocabulary (vocabulario compartido) | none | 20 min | Ten terms defined in English and Spanish, one line and one misconception each | drafted |
| [common-04](../../common/lessons/common-04-build-and-run/lesson.md) | Entry point and working directory (punto de entrada y directorio de trabajo) | Entry-point robustness | common-02 | 30 min | One command runs from two directories; same seed gives the same transcript | drafted |
| [common-05](../../common/lessons/common-05-golden-traces/lesson.md) | Golden traces (trazas de referencia) | Behavioral contract (contrato de comportamiento) | common-02 | 45 min | Checksums pass; one trace reproduced; command, output and explanation recorded | drafted |

Rows follow the learning order. `common-03` is a side reference you can read before `common-02`
or `common-04`.

**common-01** is the most important document in the project. If the rules are ambiguous, every
contrast is invalid.

> **Starter route:** `common-00` through `common-05` plus `construction-00`, with no compiler.
> See [starter-route.md](starter-route.md).

---

## 1. Track A — Construction (Construcción)

Mechanism variants (`mechanism-01`…`mechanism-05`) are embedded in
`construction-04`…`construction-07` and `construction-19`; they are not separate lesson IDs.

For design lessons, the acceptance criterion is the **change test**; for the spec lesson
(`construction-00`), it is the **prediction check**.

| ID | Title | Concept | Prereq | Time | Acceptance criterion | Status |
|---|---|---|---|---|---|---|
| [construction-00](../../construction/lessons/construction-00-read-the-rules/lesson.md) | Read the rules, run the reference baseline | Specification | common-01, common-02 | 50 min | Predict the next move of a seeded game | drafted |
| [construction-01](../../construction/lessons/construction-01-non-structured/lesson.md) | Non-structured programming (programación no estructurada) | Arbitrary control flow | construction-00 | 70 min | Add “skip turn”; count rewired jumps | drafted |
| [construction-02](../../construction/lessons/construction-02-structured/lesson.md) | Structured programming (programación estructurada) | Sequence, selection, iteration | construction-01 | 65 min | Same change, localized; no `goto` | drafted |
| [construction-03](../../construction/lessons/construction-03-functions/lesson.md) | Functions and parameters (funciones y parámetros) | Scope and decomposition | construction-02 | 45 min | Extract a move without globals | drafted |
| [construction-04](../../construction/lessons/construction-04-records/lesson.md) | Records (registros) | Aggregation | construction-03 | 55 min | Add `captured` to a knight | drafted |
| [construction-05](../../construction/lessons/construction-05-array-of-records/lesson.md) | Array of records (arreglo de estructuras) | Collection and traversal | construction-04 | 62 min | Traverse the board without 64 variables | drafted |
| [construction-06](../../construction/lessons/construction-06-references/lesson.md) | References (referencias) | Aliasing without addresses | construction-05 | 45 min | Mutate a record through a parameter | drafted |
| [construction-07](../../construction/lessons/construction-07-pointers/lesson.md) | Pointers (punteros) | Indirection and lifetime | construction-06 | 88 min | Access a cell through a pointer; explain the row stride; traces unchanged | drafted |
| [construction-08](../../construction/lessons/construction-08-modular/lesson.md) | Modular programming (programación modular) | Information hiding | construction-03, construction-07 | 92 min | Board 8×8 → 10×10; only the board module changes | drafted |
| [construction-09](../../construction/lessons/construction-09-abstract-data-type/lesson.md) | Abstract data type (tipo abstracto de datos, TAD) | Representation independence | construction-08 | 97 min | Swap board representation; callers compile; traces pass | drafted |
| [construction-10](../../construction/lessons/construction-10-adt-invariants/lesson.md) | Abstract data type invariants (invariantes del TAD) | Validity and lifecycle | construction-09 | 60 min | Out-of-range writes rejected; board_is_valid true; guard removal exposes invalid state | drafted |
| [construction-11](../../construction/lessons/construction-11-encapsulation/lesson.md) | Encapsulation (encapsulación) | Class with invariant | construction-10 | 60 min | Private state and operations preserve the invariant; public state exposes an invalid board | drafted |
| [construction-12](../../construction/lessons/construction-12-polymorphism/lesson.md) | Polymorphism (polimorfismo) | Late binding (ligadura dinámica) | construction-11 | 80 min | Add a Piece subclass; zero loop edits; original traces unchanged | drafted |
| [construction-13](../../construction/lessons/construction-13-composition/lesson.md) | Composition (composición) | Composition over inheritance | construction-12 | 68 min | Replace inheritance with delegation; behavior and 20 traces unchanged | drafted |
| [construction-14](../../construction/lessons/construction-14-generic-functions/lesson.md) | Generic functions (funciones genéricas) | Type parameters | construction-12 or construction-13 | 68 min | One move generator for two piece types | drafted |
| [construction-15](../../construction/lessons/construction-15-generic-classes/lesson.md) | Generic classes and concepts (clases genéricas y conceptos) | Constraints | construction-14 | 81 min | Same engine template instantiates 8×8 and 10×10; 8×8 traces pass | drafted |
| [construction-16](../../construction/lessons/construction-16-ownership/lesson.md) | Ownership and resource lifetime (propiedad) | Lifetime | construction-15 | 70 min | unique_ptr ownership; copy deleted; move available; 20 traces pass | drafted |
| [construction-17](../../construction/lessons/construction-17-stl/lesson.md) | Standard Template Library and algorithms (algoritmos) | Iteration abstraction | construction-05, construction-16 | 68 min | Same transcript after replacing three traversals | drafted |
| [construction-18](../../construction/lessons/construction-18-functional/lesson.md) | Functional style (funcional) | Purity and immutability | construction-17 | 85 min | Undo/redo free via board history | drafted |
| [construction-19](../../construction/lessons/construction-19-data-oriented/lesson.md) | Data-oriented design (orientado a datos) | Locality | construction-05, construction-17 | 85 min | 10,000 games; measured profile | drafted |
| [construction-20](../../construction/lessons/construction-20-hexagonal/lesson.md) | Hexagonal (hexagonal) | Ports and adapters | construction-08, construction-17 | 92 min | Same core, console + test adapters | drafted |
| [construction-21](../../construction/lessons/construction-21-concurrency/lesson.md) | Concurrency (concurrencia) | Shared state vs messages | construction-12, construction-20 | 115 min | Race detected and removed; determinism kept | drafted |
| [construction-22](../../construction/lessons/construction-22-error-contracts/lesson.md) | Error contracts (errores) | Recovery | construction-09, construction-21 | 75 min | Invalid move handled without `exit` | drafted |
| [construction-23](../../construction/lessons/construction-23-port/lesson.md) | Port to another language (portar) | Syntax is disposable | construction-09 or construction-12 | 195 min | Same traces from Python/Rust | drafted |
| [construction-24](../../construction/lessons/construction-24-capstone/lesson.md) | Capstone: cost report (informe de costes) | Tradeoff justification | construction-12, construction-15 | 175 min | Defend a design choice with numbers | drafted |

### Track A path (recommended)

`construction-00 → construction-01 → construction-02 → construction-03 → construction-04 →
construction-05 → construction-06 → construction-07 → construction-08 → construction-09 →
construction-10 → construction-11 → construction-12 → construction-14 → construction-15`

Lenses (pick two; `construction-20` is the default first): `construction-16`,
`construction-17 → construction-18`, `construction-20`.
Capstone: `construction-24`.

---

## 2. Track B — Archaeology (Arqueología)

| ID | Title | Failure class | Prereq | Time | Acceptance criteria | Status |
|---|---|---|---|---|---|---|
| [archaeology-00](../../archaeology/lessons/archaeology-00-orientation/lesson.md) | Orientation: read a legacy codebase | — | common-04 | 45 min | Code map + one-sentence purpose | drafted |
| [archaeology-01](../../archaeology/lessons/archaeology-01-build-today/lesson.md) | Build and run legacy code | — | common-04 | 62 min | Runnable build or documented blocker | drafted |
| [archaeology-02](../../archaeology/lessons/archaeology-02-code-inventory/lesson.md) | Code inventory and call graph | — | archaeology-01 | 60 min | Inventory + call graph with evidence | drafted |
| [archaeology-03](../../archaeology/lessons/archaeology-03-reverse-spec/lesson.md) | Reverse-engineer the spec | — | archaeology-02 | 90 min | Spec + traces from the original | drafted |
| [archaeology-04](../../archaeology/lessons/archaeology-04-correctness-failures/lesson.md) | Correctness failures (failure-001…failure-006) | correctness | archaeology-03 | 100 min | 6 failure cards + tests | drafted |
| [archaeology-05](../../archaeology/lessons/archaeology-05-global-state/lesson.md) | Global state and hidden coupling | design | archaeology-04 | 60 min | Map of mutations per global | drafted |
| [archaeology-06](../../archaeology/lessons/archaeology-06-duplication-switch/lesson.md) | Duplication and the eight-case switch | design | archaeology-04 | 60 min | One extraction, behavior preserved | drafted |
| [archaeology-07](../../archaeology/lessons/archaeology-07-representation-leakage/lesson.md) | Representation leakage and parallel arrays | design | archaeology-05 | 60 min | Record introduced; callers updated | drafted |
| [archaeology-08](../../archaeology/lessons/archaeology-08-module-boundary/lesson.md) | Missing module boundary | design | archaeology-05 | 90 min | Header + implementation; internals hidden | drafted |
| [archaeology-09](../../archaeology/lessons/archaeology-09-abstract-data-type/lesson.md) | Missing abstract data type | design | archaeology-08 | 90 min | Opaque type; representation swap | drafted |
| [archaeology-10](../../archaeology/lessons/archaeology-10-polymorphism/lesson.md) | Missing polymorphism | design | archaeology-09 | 90 min | Switch replaced by dispatch | drafted |
| [archaeology-11](../../archaeology/lessons/archaeology-11-input-output/lesson.md) | Input and output and testability | design | archaeology-09 | 60 min | Core runs without console | drafted |
| [archaeology-12](../../archaeology/lessons/archaeology-12-naming-comments/lesson.md) | Naming and comments | professional-practice | archaeology-04 | 60 min | Rename + comment rewrite with reasons | drafted |
| [archaeology-13](../../archaeology/lessons/archaeology-13-documentation/lesson.md) | Technical documentation for maintainers | professional-practice | archaeology-03 | 60 min | README + spec + build/run/test instructions | drafted |
| [archaeology-14](../../archaeology/lessons/archaeology-14-tests-determinism/lesson.md) | Tests and determinism | professional-practice | archaeology-03 | 90 min | Golden traces + seed control | drafted |
| [archaeology-15](../../archaeology/lessons/archaeology-15-static-analysis/lesson.md) | Static analysis and compiler warnings | professional-practice | archaeology-04 | 60 min | Warnings triaged; zero warnings; behavior preserved | drafted |
| [archaeology-16](../../archaeology/lessons/archaeology-16-platform-lock-in/lesson.md) | Platform lock-in | robustness | archaeology-01 | 60 min | Portable build; adapter for console | drafted |
| [archaeology-17](../../archaeology/lessons/archaeology-17-code-review/lesson.md) | Code review of a legacy function | professional-practice | archaeology-04 | 60 min | 3 findings + severity + one test | drafted |
| [archaeology-18](../../archaeology/lessons/archaeology-18-refactor-workshop/lesson.md) | Refactor workshop | all failure classes | archaeology-04, archaeology-05, archaeology-08, archaeology-09 | 180 min | Legacy → Track A rung, traces equal | drafted |
| [archaeology-19](../../archaeology/lessons/archaeology-19-review-retrospective/lesson.md) | Review checklist and retrospective | — | archaeology-18 | 90 min | Reusable checklist; final retro | drafted |

### Track B path (recommended)

`archaeology-00 → archaeology-01 → archaeology-02 → archaeology-03 → archaeology-04 → archaeology-05
→ archaeology-08 → archaeology-09 → archaeology-14 → archaeology-15 → archaeology-17 →
archaeology-18 → archaeology-19`

Optional lenses: `archaeology-06`, `archaeology-07`, `archaeology-10`, `archaeology-11`,
`archaeology-12`, `archaeology-13`, `archaeology-16`.

The path is a **spine**, not a prison: a reviewer can jump to the failure class they need.

> **Class gap and scope:** the historical class rung is missing, and the modern class version is
> specified here in `construction-11`, `construction-12` and `construction-13`. Every other step not
> present in the originals — modular programming, abstract data types, object-oriented programming,
> generic programming and the lenses — was never intended to be worked out in the legacy files. They
> are curriculum extensions, not missing historical artifacts. See
> [`archaeology-track.md`](archaeology-track.md) for the scope note.

---

## 3. Minimum viable product (scope)

| Track | Included | Why |
|---|---|---|
| Common | common-00 … common-05 | Without spec, harness and a way to learn, no lesson is valid |
| A | construction-00 … construction-09 | Covers control flow, aggregation, indirection, modularity, abstract data type — the conceptual core |
| B | archaeology-00 … archaeology-10 + first 15 failure cards | Covers correctness, design and the professional gaps that matter most |

All lessons are now drafted; the MVP remains the minimum first-release scope. All fifteen
failure cards are drafted.

---

## 4. Dependency graph (textual)

The `Prereq` column above is authoritative. This is a high-level map, not a second specification.

```
Common core — learning order
  common-00 learn ─► common-01 spec ─► common-02 harness
  common-02 harness ─┬─► common-04 build ─► common-05 traces
                     └─► common-05 traces
  common-03 glossary ─► before common-02 or common-04

Track A — paradigm ladder
  construction-00 ─► construction-01 ─► construction-02 ─► construction-03 ─►
  construction-04 ─► construction-05 ─► construction-06 ─► construction-07
  construction-03, construction-07 ─► construction-08 ─► construction-09 ─►
  construction-10 ─► construction-11 ─► construction-12
  construction-12 ─► construction-13, construction-14
  construction-14 ─► construction-15 ─► construction-16
  construction-05, construction-16 ─► construction-17
  construction-17 ─► construction-18, construction-19, construction-20
  construction-12, construction-20 ─► construction-21
  construction-09, construction-21 ─► construction-22
  construction-09 or construction-12 ─► construction-23
  construction-12, construction-15 ─► construction-24

Track B — autopsy spine
  archaeology-00 (common-04); archaeology-01 (common-04)
    ─► archaeology-02 ─► archaeology-03 ─► archaeology-04
    ─► archaeology-05 ─► archaeology-08 ─► archaeology-09
  archaeology-03 ─► archaeology-13, archaeology-14
  archaeology-04 ─► archaeology-06, archaeology-12
  archaeology-05 ─► archaeology-07
  archaeology-09 ─► archaeology-10, archaeology-11
  archaeology-01 ─► archaeology-16
  archaeology-04 ─► archaeology-15, archaeology-17
  archaeology-04, archaeology-05, archaeology-08, archaeology-09 ─► archaeology-18 ─► archaeology-19
```

Read the table for the exact edges. The map exists to show the shape, not to replace the
prerequisites.

---

## 5. Lesson card (ficha de lección)

Every design lesson, when written, starts from this card:

```yaml
id: construction-04
slug: records
title: "Records (Registros)"
track: A
concept: "Aggregation (agregación)"
paradigm: "Structured (programación estructurada)"
syntax: "struct"
artifact: "construction/code/mechanism-02-records/"
question: "Can I add a field without touching every signature?"
prereq: [construction-03]
time_min: 60
change_test: "Add 'captured' to a knight; count touched files."
engineering_check: "Predict which signatures change; run the acceptance script; explain the diff."
evidence:
  - working modified variant
  - prediction note and its comparison with the observation
  - one-paragraph explanation without using the word 'struct'
misconceptions:
  - "A struct is just a bag of variables."
  - "Records are for large programs only."
languages: [en, es]
status: drafted
```

The card is the contract. If a lesson cannot fill `change_test`, it is not a lesson; it is an
article.
