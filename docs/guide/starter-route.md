# Starter route

> [English](starter-route.md) | [Español](starter-route-es.md)

> **For:** people with little or no software engineering experience.
> **Time:** 270 minutes (~4 h 30), seven lessons.
> **Requires:** Python 3.10+, Bash 4+, `grep`, `diff`, `cmp`, `sha256sum`, and a text editor.

## What this route is

The starter route is a bounded first pass through the laboratory: seven lessons in a fixed order,
each one ending with a written record. It teaches the three ideas the rest of the lab depends on — a
specification, a deterministic harness and golden traces — without asking you to build a program
from scratch or to read a large codebase.

## What you need

- Python 3.10 or newer, and Bash 4 or newer.
- `grep`, `diff`, `cmp` and `sha256sum` (standard on Linux and macOS).
- A plain text editor.

No compiler, no CMake, no build step.

## The route

Seven lessons, in this order. The times are estimates.

| # | Lesson | Time | Concept | Evidence |
|---|---|---|---|---|
| 1 | [common-00 — How to learn](../../common/lessons/common-00-how-to-learn/lesson.md) | 15 min | Metacognition | [EVIDENCE.md](../../common/lessons/common-00-how-to-learn/EVIDENCE.md) |
| 2 | [common-01 — The game as an invariant](../../common/lessons/common-01-game-invariant/lesson.md) | 50 min | Specification | [EVIDENCE.md](../../common/lessons/common-01-game-invariant/EVIDENCE.md) |
| 3 | [common-02 — Deterministic harness](../../common/lessons/common-02-deterministic-harness/lesson.md) | 60 min | Harness | [EVIDENCE.md](../../common/lessons/common-02-deterministic-harness/EVIDENCE.md) |
| 4 | [common-03 — Glossary](../../common/lessons/common-03-glossary/lesson.md) | 20 min | Glossary | [EVIDENCE.md](../../common/lessons/common-03-glossary/EVIDENCE.md) |
| 5 | [common-04 — Entry point and working directory](../../common/lessons/common-04-build-and-run/lesson.md) | 30 min | Entry point | [EVIDENCE.md](../../common/lessons/common-04-build-and-run/EVIDENCE.md) |
| 6 | [common-05 — Golden traces](../../common/lessons/common-05-golden-traces/lesson.md) | 45 min | Golden traces | [EVIDENCE.md](../../common/lessons/common-05-golden-traces/EVIDENCE.md) |
| 7 | [construction-00 — Read the rules](../../construction/lessons/construction-00-read-the-rules/lesson.md) | 50 min | Specification | [EVIDENCE.md](../../construction/lessons/construction-00-read-the-rules/EVIDENCE.md) |

## The flow

```text
START-HERE.md
├─ Full lab ── sections 0–2
│              └─ Track A or Track B
└─ Starter route ── 7 lessons, in the table order
                   ├─ exit: seven EVIDENCE.md files
                   └─ next: Track A or Track B
```

## How to work each lesson

Every lesson runs the same five steps.

1. **Predict.** Write what you expect to happen before you touch the keyboard.
2. **Run.** Execute the command the lesson gives you, exactly as written.
3. **Compare.** Put prediction and output side by side; use `diff` when the output is long.
4. **Explain.** Say in plain words why they differ, naming the line or value that surprised you.
5. **Record.** Write the prediction, the command and the comparison in the lesson's `EVIDENCE.md`.

Do not write the prediction after seeing the result.

## Pacing

Two sessions of about 2 h 15 each, or four of about 1 h.

## Exit criteria

- [ ] Seven `EVIDENCE.md` files are completed, one per lesson.
- [ ] `sha256sum -c checksums.txt` reports all twenty files OK when run from `common/traces/`.
- [ ] One trace is reproduced byte for byte with an empty `diff`.
- [ ] The same seed gives the same transcript from two working directories.
- [ ] One move-order prediction is compared with the actual transcript.
- [ ] You can explain why `run.sh` works from any directory.

## What you will be able to do

- Read a specification and separate a rule from a value.
- Run a deterministic program and prove that it is reproducible.
- Verify an artifact with checksums and diffs.
- Read a small program and explain its entry point and its data flow.
- Build a portfolio of evidence for every lesson.

## After the route

- **Track A** continues at `construction-01` and requires C++20.
- **Track B** starts at `archaeology-00`; its build check requires C++20.

## FAQ

**Do I need C++?** Not for this route. Track A and Track B require C++20 once you continue.

**Can I skip `common-03`?** Read it when a term blocks you; later lessons assume its vocabulary.

**Is this the whole lab?** No. The lab has two full tracks beyond this route.
