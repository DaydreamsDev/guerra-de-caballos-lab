# Common core (núcleo común)
> [English](README.md) | [Español](README-es.md)

The common core is the contract shared by both tracks. It is built before the first lesson.

Two orders, not one:

**Build order** — the artifacts, in dependency order:

```text
spec → harness → golden traces → first lesson
```

**Learning order** — the lessons, in the order a learner takes them:

```text
common-00 → common-01 → common-02 → common-03 → common-04 → common-05
```

If you are following the [Starter route](../docs/guide/starter-route.md), this core plus
`construction-00` is the whole route.

## Lessons

| ID | Lesson | Concept | Status |
|---|---|---|---|
| `common-00` | [How to learn](lessons/common-00-how-to-learn/lesson.md) (cómo aprender) | Metacognition | Drafted |
| `common-01` | [The game as an invariant](lessons/common-01-game-invariant/lesson.md) (el juego como invariante) | Specification | Drafted |
| `common-02` | [Deterministic harness](lessons/common-02-deterministic-harness/lesson.md) (arnés determinista) | Reproducibility | Drafted |
| `common-03` | [Glossary](lessons/common-03-glossary/lesson.md) (glosario) | Shared vocabulary | Drafted |
| `common-04` | [Build and run](lessons/common-04-build-and-run/lesson.md) (compilar y ejecutar) | Build reproducibility | Drafted |
| `common-05` | [Golden traces](lessons/common-05-golden-traces/lesson.md) (trazas de referencia) | Behavioral contract | Drafted |

`common-01` is the most important document in the project. If the rules are ambiguous, every
contrast is invalid.

## Directory plan

| Path | Purpose |
|---|---|
| `common/spec/` | The rules, written once, independent of code. |
| `common/harness/` | Runs a variant with a seed and captures the transcript. |
| `common/traces/` | Golden outputs for the trace set. |
| [`docs/guide/glossary.md`](../docs/guide/glossary.md) | Canonical terms in English and Spanish. |

## Engineering check

Every artifact lesson uses the same cycle:

```text
predict → run → compare → explain → record
```

The gap between prediction and observation is the learning signal. It is never graded.

## Exit criteria

- Two different builds produce identical traces.
- A newcomer can run the common core in under ten minutes.
- The rules, glossary and trace set are complete enough to support the first lesson.

See [`../docs/guide/lesson-catalog.md`](../docs/guide/lesson-catalog.md).
