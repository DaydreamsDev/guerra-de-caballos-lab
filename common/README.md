# Common core (núcleo común)
> [English](README.md) | [Español](README-es.md)

The common core is the contract shared by both tracks. It is built before the first lesson.

The order:

```text
spec → harness → golden traces → first lesson
```

## Lessons

| ID | Lesson | Concept | Status |
|---|---|---|---|
| `common-00` | [The game as an invariant](lessons/common-00-game-invariant/lesson.md) (el juego como invariante) | Specification | Drafted |
| `common-01` | [Deterministic harness](lessons/common-01-deterministic-harness/lesson.md) (arnés determinista) | Reproducibility | Drafted |
| `common-02` | [Glossary](lessons/common-02-glossary/lesson.md) (glosario) | Shared vocabulary | Drafted |
| `common-03` | [Build and run](lessons/common-03-build-and-run/lesson.md) (compilar y ejecutar) | Build reproducibility | Drafted |
| `common-04` | [Golden traces](lessons/common-04-golden-traces/lesson.md) (trazas de referencia) | Behavioral contract | Drafted |
| `common-05` | [How to learn](lessons/common-05-how-to-learn/lesson.md) (cómo aprender) | Metacognition | Drafted |

`common-00` is the most important document in the project. If the rules are ambiguous, every
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
