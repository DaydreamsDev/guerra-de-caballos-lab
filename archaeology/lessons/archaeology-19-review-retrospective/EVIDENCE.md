# archaeology-19 — Evidence: review checklist and retrospective

> This is your record. Write it for your future self.
> Record what you observed, not what you expected the lesson to want.

## 1. Setup

- Date:
- Archive path:
- Tag checked out:
- Commit hash (`git rev-parse HEAD`):
- Compiler and version (`c++ --version`):
- Shell and version:
- Working directory:
- Scratch directory used:
- Portfolio location:
- Anything unexpected in the setup:

## 2. Prediction — write before the sealed pass

Do not edit this table after Step 3. The gap between prediction and observation is the learning
signal; repairing the prediction destroys it.

| # | Question | My prediction | How I would know I am wrong |
|---:|---|---|---|
| 1 | How many of the ten expected failures will the checklist find in 20 minutes? | | |
| 2 | Which family will produce the most findings? | | |
| 3 | Which family will produce the fewest? | | |
| 4 | Which finding will be the first high-severity one, and at what minute? | | |
| 5 | Which item will produce a false positive, and why? | | |

One falsifiable sentence about the checklist's value in this sample:

>

## 3. Portfolio inventory

Count artifacts, not lessons read.

```bash
cd archaeology/lessons
find . -name 'EVIDENCE.md' -o -name 'EVIDENCE-es.md' | sort
find . -name '*.md' | wc -l
grep -rl 'failure-0' --include='*.md' . | sort
```

| Lesson | Artifact I produced | Evidence state | Reusable? |
|---|---|---|---|
| archaeology-00 | | | |
| archaeology-01 | | | |
| archaeology-02 | | | |
| archaeology-03 | | | |
| archaeology-04 | | | |
| archaeology-05 | | | |
| archaeology-06 | | | |
| archaeology-07 | | | |
| archaeology-08 | | | |
| archaeology-09 | | | |
| archaeology-10 | | | |
| archaeology-11 | | | |
| archaeology-12 | | | |
| archaeology-13 | | | |
| archaeology-14 | | | |
| archaeology-15 | | | |
| archaeology-16 | | | |
| archaeology-17 | | | |
| archaeology-18 | | | |

Missing artifacts and why:

>

The three artifacts I would trust in a real review:

1.
2.
3.

## 4. Template audit

Read `REVIEW-CHECKLIST.md` and record the items that are not directly observable.

| Template item | Why it is vague | Evidence that would settle it | My rewrite |
|---|---|---|---|
| Boundaries checked. | | | |
| Global state mapped. | | | |
| Representation hidden. | | | |
| Errors are values or documented blockers. | | | |
| Tree is clean. | | | |
| | | | |
| | | | |

Which template items already pass the two-reviewer test:

Which template item is the most dangerous because it sounds complete:

>

## 5. My checklist

At least 15 rows. Every row needs all eight columns.

| ID | Family | Question | Evidence | Pass criterion | Class | Rung | Severity if failed |
|---|---|---|---|---|---|---|---|
| C-01 | Correctness | | | | | | |
| C-02 | Correctness | | | | | | |
| C-03 | Correctness | | | | | | |
| D-01 | Design | | | | | | |
| D-02 | Design | | | | | | |
| D-03 | Design | | | | | | |
| R-01 | Robustness | | | | | | |
| R-02 | Robustness | | | | | | |
| R-03 | Robustness | | | | | | |
| P-01 | Practice | | | | | | |
| P-02 | Practice | | | | | | |
| P-03 | Practice | | | | | | |
| E-01 | Evidence | | | | | | |
| E-02 | Evidence | | | | | | |
| E-03 | Evidence | | | | | | |
| E-04 | Evidence | | | | | | |
| | | | | | | | |
| | | | | | | | |

Coverage matrix:

| Family | Items | Minimum met? | Failure IDs covered | Track A rungs used |
|---|---|---:|---|---|
| Correctness | | 3 | | |
| Design | | 3 | | |
| Robustness | | 3 | | |
| Professional practice | | 3 | | |
| Evidence | | 3 | | |

Class coverage:

| Class | Items | Covered? |
|---|---|---|
| Correctness | | |
| Design | | |
| Robustness | | |
| Professional practice | | |

Quality gate:

| Test | Item that failed | What I changed |
|---|---|---|
| Two-reviewer test | | |
| Evidence test | | |
| Pass test | | |
| Class test | | |
| Reuse test | | |

The two items I applied to another codebase:

1.
2.

What happened:

## 6. Sealed sample

- Variant chosen:
- Why this variant:
- Source path:
- Scratch path:
- Line count (`wc -l`):
- Catalog opened during the timed pass? (It must be no.)

Copy command:

```bash
mkdir -p build/review-sample
cp original/variant-02-records-2008/source/guerra-de-caballos-records.cpp \
   build/review-sample/sample.cpp
```

Anchors I verified before the pass:

| Anchor | Line | Verified? |
|---|---:|---|
| Entry point | 37 | |
| Off-by-one loop (`i <= 64`) | 72 | |
| First `conio.h` or `iostream.h` include | | |
| First `system("color")` call | | |
| First `cout` inside movement logic | | |
| Seed call | | |

## 7. Timed pass

- Start time:
- End time:
- Actual duration:
- Did I stop at the timer? (It must be yes.)

| ID | Finding | Evidence (file:line or command) | Class | Severity | Checklist item | Verdict |
|---|---|---|---|---|---|---|
| F-01 | | | | | | confirmed / false positive / missed |
| F-02 | | | | | | |
| F-03 | | | | | | |
| F-04 | | | | | | |
| F-05 | | | | | | |
| F-06 | | | | | | |
| F-07 | | | | | | |
| F-08 | | | | | | |
| F-09 | | | | | | |
| F-10 | | | | | | |
| F-11 | | | | | | |
| F-12 | | | | | | |

Findings I noticed after the timer:

>

## 8. Comparison with the catalog

| Expected | Failure | Detected? | My evidence | Checklist item that should have caught it |
|---|---|---|---|---|
| failure-001 | `i <= 64` over a 64-element array | | | |
| failure-002 | `void main()` | | | |
| failure-003 | `srand(time(NULL))` inside a retry loop | | | |
| failure-007 | Global mutable state | | | |
| failure-008 | Eight hand-copied `case` bodies | | | |
| failure-009 | Parallel arrays or loose knight variables | | | |
| failure-012 | Input and output inside movement logic | | | |
| failure-015 | `conio.h`, `iostream.h`, `system("color")` | | | |
| failure-016 | No tests, no harness, no seed | | | |
| failure-019 | Comments that narrate or lie | | | |

Unexpected findings not in the catalog:

>

## 9. Measurement

| Metric | Formula | My result |
|---|---|---|
| Detected | Confirmed expected failures | |
| Missed | Expected failures not detected | |
| False positives | Reported findings the evidence did not support | |
| Detection rate | Detected / 10 | |
| Precision | Real / reported | |
| Time to first high-severity finding | Minutes from start | |

Which item produced the most value per minute:

Which item produced the false positive, and how I would rewrite it:

>

Which missed failure was a checklist gap:

Which missed failure was a reviewer gap:

What the sample cannot prove:

- It does not prove the checklist catches every failure class.
- It does not prove the detection rate generalizes to another codebase.
- It does not prove the archive is correct or incorrect.
- It does not replace a test, a sanitizer or a trace.
- It does not measure false negatives outside the ten expected failures.

## 10. Retrospective

The track in one table:

| Phase | Lessons | What it taught | Artifact |
|---|---|---|---|
| Context | 00–03 | | |
| Correctness | 04 | | |
| Design | 05–11 | | |
| Practice | 12–15 | | |
| Environment | 16 | | |
| Teaching | 17 | | |
| Synthesis | 18 | | |
| Review | 19 | | |

Four questions:

- Strongest diagnosis, with evidence:
- Weakest diagnosis, with the missing evidence:
- Under-diagnosed class, and why:
- Over-diagnosed class, and why:

Keep / stop / start:

| Keep | Stop | Start |
|---|---|---|
| | | |

What changed in how I read code:

>

Blameless self-review — what made a weak item feel sufficient:

>

## 11. Claim audit

| # | Claim from my portfolio | State | Evidence | What would change it |
|---:|---|---|---|---|
| 1 | | verified / documented / inferred / unknown | | |
| 2 | | | | |
| 3 | | | | |
| 4 | | | | |
| 5 | | | | |

If three or more are inferred or unknown, what that says about the portfolio:

>

## 12. Explanation

The 60-second version, written out:

>

Three things the checklist cannot do:

1.
2.
3.

What I would change before using it on another codebase:

>

The Track A rung I would study next, and why:

>

## 13. Archive integrity

- `git status --short` in the archive:
- Did I edit `original/`? (It must be no.)
- Did I apply any patch inside the archive? (It must be no.)
- Where did I keep my scratch copy?
- Did I edit the lesson's `code/` directory? (It must be no.)

## 14. Acceptance checklist

- [ ] I inventoried the portfolio and named the missing artifacts.
- [ ] I audited the template and rewrote at least five vague items.
- [ ] My checklist has at least 15 items across the five families.
- [ ] Every item names a question, evidence, a pass criterion, a class and a rung.
- [ ] I applied the checklist to a sealed variant in a scratch copy.
- [ ] I recorded the timed pass, the findings and the evidence for each finding.
- [ ] I computed detection, misses and false positives.
- [ ] I stated the limits of the sample.
- [ ] I wrote the retrospective with one keep, one stop and one start.
- [ ] I audited five claims as verified, documented, inferred or unknown.
- [ ] I explained the checklist in 60 seconds and named three limits.
- [ ] I linked each family to a failure ID and a Track A rung.
- [ ] The archive is unmodified.

## 15. Next step

- Continue to Track A: `construction-02`, `construction-08`, `construction-09`, `construction-12`,
  `construction-14`, `construction-20` or `construction-24`.
- Or review a different codebase with this checklist and add one failure card the catalog lacks.
- If the detection rate surprised you, run a second reviewer on the same sample and compare.
- If an item kept failing the reuse test, move its project nouns into the evidence column.
- If the retrospective found no weak diagnosis, look again; a perfect record is usually a weak
  audit.
