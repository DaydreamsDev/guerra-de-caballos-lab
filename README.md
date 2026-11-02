# Guerra de Caballos — Learning Lab
> [English](README.md) | [Español](README-es.md)

> **Start here:** [START-HERE.md](START-HERE.md)

> One game. Many ways of thinking. Same behavior, different design.

This is a learning laboratory built around one invariant: the original console game
*Guerra de Caballos*. Every variant implements the same rules; only the design decisions change. The
game is the vehicle, never the subject.

The learning method is described in [How learning works](docs/guide/how-learning-works.md).

## The archive is the evidence

The historical source, its defects and its documented class gap live in the frozen archive:

```text
https://github.com/DaydreamsDev/DaydreamsDev-guerra-de-caballos-archive
```

Pinned version:

```text
v1.0.0
```

The archive is evidence. This laboratory is the modern learning layer. The lab does not alter the
archive.

See [`docs/archive.md`](docs/archive.md).

## Two paths

| Track | Name | Question it answers | Audience |
|---|---|---|---|
| **A** | **Construction** (Construcción) | *How do I build the same thing with a different way of thinking?* | Anyone who wants concepts, from zero to advanced |
| **B** | **Archaeology** (Arqueología) | *What is wrong with this real code, why does it matter, and what should it look like?* | Juniors, reviewers, maintainers, professionals |

The tracks are pedagogically independent but technically coupled through the common core: the
specification, the deterministic harness, the golden traces and the glossary.

## Before the tracks

If you have little or no software engineering experience, start with the
[Starter route](docs/guide/starter-route.md): seven lessons, no compiler, about four and a half
hours. You can enter Track A or Track B afterwards.

## How to start

1. Follow [`START-HERE.md`](START-HERE.md) for the guided order.
2. Read [`docs/guide/conceptual-framework.md`](docs/guide/conceptual-framework.md) and
   [`docs/guide/how-learning-works.md`](docs/guide/how-learning-works.md).
3. Start the common core with [`common-00`](common/lessons/common-00-how-to-learn/lesson.md) —
   how to learn — then follow the core in order.
4. Choose Track A or Track B after the common core. A learner may do either track first.

## The invariant

All variants must produce the same game transcript for the same seed. That is what makes the
comparison valid. Without determinism and golden traces, the variants are different programs.

The common core is:

- `common/spec/` — the rules, written once, independent of code.
- `common/harness/` — runs a variant with a seed and captures the transcript.
- `common/traces/` — golden outputs.
- [`docs/guide/glossary.md`](docs/guide/glossary.md) — canonical terms in English and Spanish.

## Design principles

1. **One variable at a time.** Each lesson changes exactly one critical aspect.
2. **Same behavior, different design.** The game never changes; the cost of the next change does.
3. **Concept before syntax.** Syntax is the dialect that forces the concept, not the concept itself.
4. **Evidence over opinion.** Golden traces, acceptance criteria and measurable change costs.
5. **Failure is data.** Track B diagnoses causes and costs.
6. **Small artifacts.** 150–300 lines per teaching variant, not the original 900-line file.
7. **Bilingual by design.** Every learner-facing document ships in English and Spanish; both are
   first-class.
8. **Predict before running.** The gap between expectation and observation is the learning signal.

## Language policy

- Every learner-facing document ships in English and Spanish.
- English uses the canonical filename; Spanish uses the `-es.md` suffix.
- Neither language is a prerequisite for the other.
- Key concepts keep their Spanish term in parentheses in English, and their English term in
  parentheses in Spanish.
- Code identifiers stay in English in both editions.
- The MIT license text remains in English; the Spanish README explains it.

## Repository map

| Path | Purpose |
|---|---|
| [`docs/guide/`](docs/guide/) | Learner guide: concepts, pedagogy, tracks, catalog, verification and glossary. |
| [`docs/archive.md`](docs/archive.md) | The frozen archive version this lab references. |
| [`common/`](common/) | Shared specification, harness, traces, glossary and build. |
| [`construction/`](construction/) | Track A: the construction ladder. |
| [`archaeology/`](archaeology/) | Track B: the legacy autopsy and failure cards. |
| [`reference/`](reference/) | The "should be" implementation produced by Track A. |

## Current status

- **Learner guide:** available.
- **Common core:** six lessons drafted; reference harness and twenty golden traces available.
- **Track A lessons:** `construction-00` through `construction-24` drafted; Track A is complete.
- **Track B lessons:** `archaeology-00` through `archaeology-19` drafted; Track B lessons are
  complete.
- **Track B failure cards:** 15 of 15 first-release cards drafted.
- **Modern class version:** covered by `construction-11`, `construction-12` and
  `construction-13`.
- **Reference implementation:** implemented; matches 20/20 golden traces.
- **Starter route:** available; seven lessons, no compiler.

The order that must not be inverted:

```text
spec → harness → golden traces → first lesson → first learner → more lessons
```

## License

The project is released under the MIT license. See [`LICENSE`](LICENSE) and
[`CITATION.cff`](CITATION.cff).
