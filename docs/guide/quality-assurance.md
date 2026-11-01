# Quality assurance (QA)

> [English](quality-assurance.md) | [Español](quality-assurance-es.md)

This guide is for maintainers and reviewers. A learner does not need it to use the laboratory.

## Scope

QA covers the repository, the lessons, the pedagogy and the code. A green build is not enough: a
lesson can compile and still fail the learner.

## Three layers

| Layer | Question | Who runs it |
|---|---|---|
| **Structural QA** | Does the repository hold together? | Automated checks and a maintainer |
| **Pedagogical QA** | Does the lesson teach the concept with low friction? | An independent reviewer |
| **Learner validation** | Can a real learner finish it and explain it? | A person who is not the author |

## Structural QA

- Every internal link resolves.
- Every English file has its Spanish pair, and the reverse.
- `meta.yml`, the lesson header and the catalog agree on status.
- Code builds; the reference matches 20/20 traces; lesson-specific traces pass.
- No build directory, `__pycache__`, temporary file or editor backup is committed.
- Code stays within 100 columns; Markdown stays within 100 columns outside tables.
- The only evaluation policy lives in the canonical guides, not in every lesson.
- Every lesson has a **Next step** and an evidence file.

## Pedagogical QA rubric

Review each dimension as **pass / not yet**. A pass needs evidence, not impression.

| Dimension | Question | Pass evidence |
|---|---|---|
| **One concept** | Does the lesson teach one concept? | The concept is named once and the change test isolates it. |
| **Prerequisite fit** | Can the learner start with what the prerequisite taught? | The lesson uses only earlier concepts or defines a new term before use. |
| **Authentic value** | Does the learner produce something an engineer would recognize? | A runnable artifact, a trace, a diagnosis or a review, not a quiz. |
| **Prediction** | Does the learner predict before running? | A written prediction and a recorded gap. |
| **Contrast** | Is the concept visible through a difference? | A with/without pair or a before/after change test. |
| **Cognitive load** | Is the path short enough to hold in working memory? | One concept, signposted steps, no redundant restatement. |
| **Explanation** | Can the learner explain without syntax words? | A prompt that asks for the mechanism in plain language. |
| **Reflection** | Does the learner decide what comes next? | A reflection and a named next step. |
| **Coherence** | Does the lesson fit its track and the catalog? | Prerequisites, vocabulary and links agree with the track guide. |
| **Accessibility** | Is the language plain and the structure predictable? | Short sentences, defined terms, consistent headings, no color-only meaning. |

## Learner validation

A lesson becomes `stable` only after one person who is not the author completes it. The record
should name:

- what they built or diagnosed;
- which criterion was hard to understand;
- where they needed help;
- what they can explain after the lesson;
- the next step they chose.

If the learner cannot finish without the author, the lesson is not stable.

## Status mapping

The authoritative status policy is in the [lesson catalog](lesson-catalog.md). In short:
`drafted` means the content exists; `in review` means structural QA passed and an independent
pedagogical review is open; `stable` means review and learner validation are closed.
