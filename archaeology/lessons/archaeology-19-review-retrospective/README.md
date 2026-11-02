# archaeology-19 — Review checklist and retrospective

> [English](README.md) | [Español](README-es.md)

> **Status:** drafted · **Concept:** review and retrospective · **Prerequisite:** archaeology-18 ·
> **Estimated time:** 90 minutes

This is the final Track B lesson. It does not diagnose a new defect. It turns the track into a
reusable instrument: a review checklist that another reviewer can apply to unfamiliar code, and a
retrospective that separates what you learned from what you merely read.

The archive gives you a finite set of failures. The review habit is what survives the archive.

The lesson is done when another
reviewer can apply your checklist and reproduce your verdict from your evidence.

## What the learner does

- Inspects the portfolio produced in `archaeology-00` through `archaeology-18`.
- Audits the shipped `REVIEW-CHECKLIST.md` and rewrites vague items into observable tests.
- Builds a reusable checklist across correctness, design, robustness, professional practice and
  evidence.
- Applies the checklist to a sealed archive variant in a scratch copy, under a timebox.
- Compares detected, missed and false-positive findings with the failure catalog.
- Writes a retrospective on the whole track: what to keep, stop and start.

## The instrument

```text
REVIEW-CHECKLIST.md      the starting template
REVIEW-CHECKLIST-es.md   Spanish edition of the template
```

The template is deliberately short. It names the five families and gives one-line prompts. Your job
is to turn each prompt into an item that two reviewers can apply and reach the same verdict from the
same evidence.

An item is usable when it names:

1. the question;
2. the evidence that answers it;
3. the pass criterion;
4. the failure class;
5. the Track A rung that prevents the class.

## The five families

| Family | Question | Example failure IDs | Track A anchor |
|---|---|---|---|
| Correctness | Does it do what it claims? | failure-001…failure-006 | `construction-02`, `construction-04`, `construction-05` |
| Design | Does the structure make change expensive? | failure-007…failure-014 | `construction-08`, `construction-09`, `construction-12` |
| Robustness | Does it work only in one environment? | failure-006, failure-015 | `construction-08`, `construction-20` |
| Professional practice | Can another person build, trust and maintain it? | failure-016…failure-019 | `common-02`, `common-04`, `common-05` |
| Evidence | Can the claim be checked? | all classes | `common-03`, `common-05` |

## Files

| File | Purpose |
|---|---|
| `README.md` | This file. |
| `README-es.md` | Spanish edition. |
| `lesson.md` | The guided lesson. |
| `lesson-es.md` | Spanish edition of the guided lesson. |
| `REVIEW-CHECKLIST.md` | The checklist template the learner audits and extends. |
| `REVIEW-CHECKLIST-es.md` | Spanish edition of the template. |
| `meta.yml` | Lesson metadata. |
| `EVIDENCE.md` | English evidence record. |
| `EVIDENCE-es.md` | Spanish evidence record. |

## Acceptance criterion

The lesson's **Self-check (binary)** is the authoritative checklist. `meta.yml` mirrors it for
tooling.
