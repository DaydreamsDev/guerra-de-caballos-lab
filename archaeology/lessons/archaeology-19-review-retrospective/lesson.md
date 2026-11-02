# archaeology-19 — Review checklist and retrospective

> [English](lesson.md) | [Español](lesson-es.md)

> **Status:** drafted · **Concept:** review and retrospective · **Prerequisite:** archaeology-18 ·
> **Estimated time:** 90 minutes
> **Reading:** ~23 minutes

## Why this lesson exists

Track B taught you to diagnose one failure at a time. That is necessary and it is not enough. A
reviewer who can find an off-by-one but cannot say what class it belongs to will keep finding the
same off-by-one in every file. A reviewer who can name the class, price the consequence and point to
the design that prevents it has built an instrument that transfers.

The archive is finite. It contains a known set of failures, documented in the track guide and
diagnosed across the preceding lessons. You will not review this archive for the rest of your
career. You will review code you have never seen, written by people you cannot ask, under
constraints you did not share. The checklist is how the track leaves the archive.

There is a second reason. A checklist is easy to write badly. "Boundaries checked" sounds
professional and decides nothing. Two reviewers can tick it and disagree about the same file. The
work of this lesson is to turn prompts into tests: a question, an evidence source, a pass criterion,
a failure class and a Track A rung. That is the difference between a wish list and an instrument.

The retrospective is the other half. Twenty lessons produce a lot of activity and not enough
judgment about the activity. The retrospective asks what changed in how you read code, which class
you still under-diagnose, which claim you made too early, and what you will do differently on the
next review. It is about your process, not your worth.

The lesson is done when another reviewer can
take your checklist, apply it to a file you have never seen, and reproduce your verdict from your
evidence.

## What you will be able to do

By the end of this lesson you can:

- audit a review checklist and identify items that are not directly observable;
- rewrite a vague prompt as a question with evidence, a pass criterion and a failure class;
- build a reusable checklist across correctness, design, robustness, professional practice and
  evidence;
- link every family to the failure IDs it catches and the Track A rung that prevents the class;
- apply the checklist to an unfamiliar variant under a timebox and record what it found;
- distinguish a missed finding from a false positive and compute a detection rate;
- state the limits of a one-variant, one-reviewer sample;
- run an evidence-based retrospective on your own review process;
- audit claims as verified, documented, inferred or unknown;
- explain the checklist in 60 seconds and name what it cannot prove;
- keep the archive read-only while you work.

## The concept: a checklist is an instrument, not a wish list

### Five properties of a usable item

A checklist item is a small test. It is not a topic and it is not an opinion. Five properties make
it usable.

| Property | Question it answers | Bad example | Good example |
|---|---|---|---|
| **Observable** | Can two reviewers reach the same verdict? | "Code is clean." | "No compiler warning under the shipped flags." |
| **Attributable** | Where is the evidence? | "Globals are bad." | "Every mutable namespace-scope variable has a writer table and a test that sets it twice." |
| **Classified** | Which failure class is this? | "Improve structure." | "Design: hidden temporal coupling." |
| **Actionable** | What do I do when it fails? | "Refactor." | "Name the writers; extract one function; rerun the traces." |
| **Reusable** | Does it work outside this archive? | "Check `tablero` bounds." | "Check every loop that writes an array against the array's element count." |

The fifth property is the one learners skip. A checklist tied to `tablero`, `jugada` or
`variant-02` is a diagnosis of this archive, not a review instrument. Put project names in the
evidence column, never in the question.

### The five families

The families are not a taxonomy of taste. They answer five different questions, and a review that
skips one of them produces a predictable blind spot.

| Family | Question | What a miss costs | Typical failure IDs | Track A anchor |
|---|---|---|---|---|
| **Correctness** | Does it do what it claims? | Wrong behavior, silent corruption, lost data. | failure-001…failure-006 | `construction-02`, `construction-04`, `construction-05` |
| **Design** | Does the structure make change expensive? | Every rule change touches many places; tests stay impossible. | failure-007…failure-014 | `construction-08`, `construction-09`, `construction-12`, `construction-14`, `construction-20` |
| **Robustness** | Does it work only in one environment? | Build failure, portability debt, unhandled input. | failure-006, failure-015 | `construction-08`, `construction-20`, `construction-22` |
| **Professional practice** | Can another person build, trust and maintain it? | Nobody can reproduce, trust or safely change the artifact. | failure-016…failure-019 | `common-02`, `common-04`, `common-05`, `construction-24` |
| **Evidence** | Can the claim be checked? | The review becomes opinion; unknowns hide as facts. | all classes | `common-03`, `common-05` |

The boundaries overlap on purpose. `failure-006` (unbounded input) is both a correctness failure
and a robustness failure. The checklist may list an item in one family and cross-reference another;
it must not pretend the overlap does not exist.

### Why evidence is its own family

A finding without a source is an opinion with a file name. Evidence has four states, and the
checklist must force you to choose one:

| State | Meaning | Example |
|---|---|---|
| **Verified** | You reproduced it or read the exact line. | `variant-02...cpp:72` has `i <= 64` over a 64-element array. |
| **Documented** | The artifact says it, but you did not reproduce it. | The build status report names Visual C++ 6.0. |
| **Inferred** | You reasoned from other evidence. | The encoding is CP1252 because byte 219 is not valid UTF-8. |
| **Unknown** | You did not check, or the public archive cannot answer. | Whether the original executable ran on its target machine. |

The evidence family is not a sixth technical area. It is the rule that keeps the other four honest.
Every finding in your review gets one of these labels. A checklist that does not ask for the label
will produce confident prose and weak claims.

### Review is not testing

The checklist, the test and the golden trace answer different questions. A review can catch a class
that no test covers; a test can catch a detail that no reviewer would read.

| Instrument | Strength | Limit |
|---|---|---|
| **Review checklist** | Finds classes, design debt and missing evidence across the whole artifact. | Cannot prove runtime behavior; depends on reviewer discipline. |
| **Unit test** | Proves one behavior on one input, repeatably. | Only covers the cases someone thought to write. |
| **Golden trace** | Proves a transcript is unchanged across a refactor. | Only covers the paths the trace exercises. |

The professional move is not to choose one. It is to say which claim each instrument supports. "The
loop writes past the array" is a review finding plus a sanitizer result, not a test that passes.

### False positives, false negatives and the cost of each

Two numbers matter when you apply a checklist.

- **Detection** is the share of expected failures the checklist found: `detected / expected`.
- **Precision** is the share of reported findings that were real: `real / reported`.

A checklist that reports everything has perfect detection and terrible precision. A checklist that
reports only certainties has perfect precision and misses the class it was built for. Neither number
is a grade. They are a description of the instrument, and they tell you which item to rewrite.

Record both. A missed finding is a checklist defect or a reviewer defect; a false positive is
usually a vague pass criterion. The next iteration fixes the item, not the reviewer's character.

### Severity is consequence, not ugliness

Use the track's four levels. A finding is not high because the code looks bad; it is high because
the consequence is high in a named context.

| Level | Meaning | Action |
|---|---|---|
| **Critical** | Undefined behavior, data loss, security, legal blocker. | Fix before publishing. |
| **High** | Wrong behavior, non-reproducible results, hard incident. | Fix in the minimal patch. |
| **Medium** | Expensive change, fragile structure. | Structural fix, scheduled. |
| **Low** | Readability, naming, comments. | Improve when touching the file. |

Every finding in your application pass gets a level and one sentence of context. "High because a
silent out-of-bounds write can corrupt the adjacent array" is a price. "High because it is ugly" is
not.

### Stop rules

A review without stop rules becomes a rewrite. Write these down before the timed pass.

1. **Timebox.** Twenty minutes for the first pass. Stop when the timer ends, even if the file is
   unfinished. The unfinished part is data.
2. **No repair during review.** Record the finding and move on. The minimal fix is a separate step,
   and Track B already taught you why.
3. **High-severity stop.** If you find undefined behavior or data loss, mark it and continue the
   pass. Do not disappear into one bug.
4. **Two-source rule.** A finding needs a line, a command or a document. If you cannot point to one,
   label it inferred or unknown.
5. **One reviewer, one pass.** Do not re-read the same file three times to improve the score. A
   second pass is a second data point, not a correction of the first.

### Retrospective: process, not score

The retrospective is a review of the review. It has four questions:

- **What happened?** The timeline and the artifacts, without interpretation.
- **So what?** Which diagnosis was strongest, which was weakest, which class you under-diagnosed.
- **Now what?** One habit to keep, one to stop, one to start, one thing to automate.
- **What is the evidence?** Which claims in your portfolio are verified, documented, inferred or
  unknown.

Keep it evidence-based. The question is
what made a decision reasonable then and what would make a different decision reasonable now. The
same question applies to your review: what made "boundaries checked" feel sufficient, and what
evidence would make it a real test?

## Before you start

Read these in full:

- [`README.md`](README.md) — the lesson card, the instrument and the acceptance criterion.
- [`REVIEW-CHECKLIST.md`](REVIEW-CHECKLIST.md) — the template you will audit and extend.
- [archaeology-18](../archaeology-18-refactor-workshop/WORKSHOP-REPORT.md) — the prerequisite: the
  refactor workshop and its cost table.
- [`../../../docs/guide/archaeology-track.md`](../../../docs/guide/archaeology-track.md) — §3, the
  failure taxonomy; §5, the severity scale; §7, the failure catalog; §9, the Track A mapping.
- [`../../../docs/guide/lesson-catalog.md`](../../../docs/guide/lesson-catalog.md) — the Track B
  sequence and the acceptance criterion for this lesson.

You also need:

- your portfolio from `archaeology-00` through `archaeology-18`, especially the evidence records and
  failure cards;
- a local clone of the archive at tag `v1.0.0`;
- a C++20 compiler, `bash`, `grep`, `wc` and `diff`;
- a scratch directory outside `original/`, for example `build/review-sample/`.

> **Rule:** the archive is read-only. Copy any file you want to inspect closely into a scratch
> directory; never edit `original/`. A review that changes the evidence is not a review.

## The 90-minute path

| Min | Step | What you do | Evidence |
|---:|---|---|---|
| 0–15 | Inspect | Inventory the portfolio, audit the template, choose the sealed sample | Inventory table, vague-item list |
| 15–35 | Checklist | Rewrite items, add coverage, set stop rules | Reusable checklist, coverage matrix |
| 35–55 | Apply | Timed pass on the sealed variant, then compare with the catalog | Findings table, detection rate |
| 55–70 | Retrospect | Review the whole track and your own process | Keep/stop/start table, claim audit |
| 70–82 | Explain | Give the 60-second version and defend the families | Explanation note |
| 82–90 | Record | Fill `EVIDENCE.md`, confirm the archive is clean | Portfolio entry |

If the checklist audit takes twenty-five minutes because the template
contains more vague items than you expected, that investigation is the lesson. Do not skip the
timed pass to recover the time; shorten the explanation instead.

### The engineering check in this lesson

Track B uses the same habit as Track A, with a different verb: the behavior already exists, so you
**freeze** what it does before you move it. Here the artifact under review is your own process.

| Step | Where it happens | What it produces |
|---|---|---|
| **Inspect** | Step 1 | Portfolio inventory, template audit, sealed sample chosen. |
| **Checklist** | Step 2 | At least 15 observable items with evidence and classification. |
| **Apply** | Step 3 | A timed pass, a findings table and a detection rate. |
| **Retrospect** | Step 4 | Keep/stop/start and a claim audit for the whole track. |
| **Explain** | Step 5 | A 60-second pitch and the limits of the instrument. |
| **Record** | Step 6 | `EVIDENCE.md` with commands, findings and boundaries. |

## Two sittings

This lesson is long by design. Split it into two sittings:

- **Sitting 1 — Core (Steps 1–3, about 55 min).** Stop after Step 3, when the checklist exists and
  has been applied once.
- **Sitting 2 — Extension (Steps 4–6, about 35 min).** Start only after Sitting 1. You will
  run the retrospective and record the claim audit.

## Step 1 — Inspect (15 min)

### Inventory the portfolio

Start with what the track produced. Do not trust memory; count files and entries.

```bash
cd archaeology/lessons
find . -name 'EVIDENCE.md' -o -name 'EVIDENCE-es.md' | sort
find . -name '*.md' | wc -l
grep -rl 'failure-0' --include='*.md' . | sort
```

Record the lessons that produced a reusable artifact, not only a reading. A failure card, a test, a
trace, a refactor report and an evidence record are artifacts. A lesson you read and enjoyed is not.

| Lesson | Artifact I produced | Evidence state | Reusable? |
|---|---|---|---|
| archaeology-00 | Code map | | |
| archaeology-01 | Build or documented blocker | | |
| archaeology-02 | Code inventory and call graph | | |
| archaeology-03 | Spec and traces | | |
| archaeology-04 | Failure cards and tests | | |
| archaeology-05 | Global mutation map | | |
| archaeology-06 | One extraction, behavior preserved | | |
| archaeology-07 | Record introduced, callers updated | | |
| archaeology-08 | Header and implementation | | |
| archaeology-09 | Opaque type and representation swap | | |
| archaeology-10 | Dispatch replacing the switch | | |
| archaeology-11 | Core without console | | |
| archaeology-12 | Rename and comment rewrite | | |
| archaeology-13 | README, spec and build instructions | | |
| archaeology-14 | Golden traces and seed control | | |
| archaeology-15 | Static analysis and warnings | | |
| archaeology-16 | Portable build and adapter | | |
| archaeology-17 | Code review | | |
| archaeology-18 | Refactor report and cost table | | |

The empty cells are the point. If a lesson produced no artifact, the portfolio has a gap, and the
retrospective in Step 4 must name it.

### Audit the template

Open `REVIEW-CHECKLIST.md`. It has 25 checkboxes and five retrospective prompts. Read each
item and ask: **can two reviewers apply this and reach the same verdict from the same evidence?**

Some items already pass. "Randomness has a seed" is observable: find the seed call, find the retry
loop, check whether the seed is set once. "Every claim has a source" is observable: open the claim
and look for a file, line or command.

Others do not. "Boundaries checked" is a topic, not a test. Which boundaries? Checked how? Against
what? "Global state mapped" is better, but it still does not say what a map contains or when it is
complete. "Core runs without a console" is close; it needs a command and an expected result.

Write the vague items down before you rewrite them. The list is evidence of the template's quality,
and it is the input to Step 2.

| Template item | Why it is vague | What evidence would settle it |
|---|---|---|
| Boundaries checked. | Which boundaries, and against what? | |
| Global state mapped. | What does a complete map contain? | |
| Representation hidden. | Hidden from whom, and how do you prove it? | |
| Errors are values or documented blockers. | Which errors, and where is the blocker recorded? | |
| Tree is clean. | Clean by which command, and which paths are excluded? | |

Add your own rows. Five is a floor, not a target.

### Choose the sealed sample

Use `variant-02-records-2008`. It is not the baseline you have read most often, it contains the
record variant's known failures, and it is small enough for a 20-minute pass.

```bash
mkdir -p build/review-sample
cp original/variant-02-records-2008/source/guerra-de-caballos-records.cpp \
   build/review-sample/sample.cpp
wc -l build/review-sample/sample.cpp
```

Do not open the failure catalog during the timed pass. The pass tests the checklist, not your memory
of §7. You will compare with the catalog in Step 3.

Why this variant and not `variant-01`:

- `variant-01` is the baseline used in several lessons; your memory of its anchors is too strong.
- `variant-02` uses records, so the design findings are different from the pointer variant.
- The track guide records the off-by-one at line 72 of this variant, which gives you one verifiable
  anchor for calibration.

### Predict before the pass

Write five predictions. They are about the checklist, not about the code.

1. How many of the ten expected failures will the checklist find in 20 minutes?
2. Which family will produce the most findings?
3. Which family will produce the fewest?
4. Which finding will be the first high-severity one, and at what minute?
5. Which item will produce a false positive, and why?

A useful prediction is falsifiable. "The checklist will work" is not. "It will find seven of ten,
miss the comment failure because comments are not executable, and report one false positive on
`system("color")` because the item does not distinguish a call from a string" is.

## Step 2 — Checklist (20 min)

### The shape of an item

Use one row per item. The columns are the instrument.

| Column | What it contains | Example |
|---|---|---|
| **ID** | Stable name, family prefix. | `C-03` |
| **Family** | Correctness, design, robustness, practice or evidence. | Correctness |
| **Question** | One observable question, no project nouns. | "Does every loop that writes an array stop before the element count?" |
| **Evidence** | File, line, command or document that answers it. | "Array declaration and loop header; `grep -n 'for'`." |
| **Pass criterion** | What makes the answer yes. | "The bound is `< N`, and the last element is written exactly once." |
| **Failure class** | The class from the taxonomy. | Correctness |
| **Track A rung** | The rung that removes the class. | `construction-02` |
| **Severity if failed** | Consequence level and one sentence. | High: silent out-of-bounds write. |

If a row cannot fill all eight columns, it is not ready. A question without evidence is a wish; a
pass criterion without a class cannot be triaged; a class without a rung cannot be fixed
structurally.

### Convert vague to observable

Three worked conversions. Copy the pattern, not the words.

**Example 1 — correctness.**

- Before: "Boundaries checked."
- After: "Does every loop that writes an array use a bound strictly less than the element count, and
  does the last element get written exactly once?"
- Evidence: array declaration, loop header, one run with a sanitizer if available.
- Pass: the bound is `< N`; the last element is written once; no sanitizer report on the path.
- Class: correctness. Rung: `construction-02`. Severity: critical if the write is out of bounds.

**Example 2 — design.**

- Before: "Global state mapped."
- After: "Does every mutable namespace-scope variable have a table of writers and readers, and a
  test that sets it twice to expose order dependence?"
- Evidence: `grep` for the declaration, call sites, the test.
- Pass: each variable has at least one writer, one reader, and one test that fails when the order is
  reversed.
- Class: design. Rung: `construction-08`. Severity: high if an incident cannot be reproduced.

**Example 3 — robustness.**

- Before: "No platform headers in the core."
- After: "Does the core translation unit include only standard headers, and do platform calls appear
  only in adapter files?"
- Evidence: `grep -n '#include'`, `nm -C -u` on the core object.
- Pass: no `conio.h`, `windows.h` or `system("color")` in the core; the adapter owns them.
- Class: robustness. Rung: `construction-20`. Severity: high if the build fails on a second target.

Notice what changed. The question became a sentence someone can answer with a file and a command.
The class and the rung were added. The severity became a consequence, not an adjective.

### Build the checklist

Minimum coverage:

| Family | Minimum items | Must include |
|---|---:|---|
| Correctness | 3 | Bounds, initialization, randomness or input. |
| Design | 3 | Global state, duplication, representation or dispatch. |
| Robustness | 3 | Platform boundary, input validation, error path. |
| Professional practice | 3 | Build, tests or traces, documentation or tree hygiene. |
| Evidence | 3 | Source per claim, confidence label, archive integrity, label accuracy. |

Fifteen items is the floor. Twenty is comfortable. More than thirty is usually a sign that the
reviewer is writing a book instead of a checklist; merge related questions and keep the pass
criteria sharp.

All five failure classes must appear at least once. Correctness, design, robustness, professional
practice and evidence each have their own family.

Starter set. Replace the blanks with your own wording and evidence.

| ID | Family | Question | Evidence | Pass criterion | Class | Rung | Severity if failed |
|---|---|---|---|---|---|---|---|
| C-01 | Correctness | Does every array write stay below the element count? | | | Correctness | `construction-02` | Critical |
| C-02 | Correctness | Is every variable initialized before its first read? | | | Correctness | `construction-04` | High |
| C-03 | Correctness | Is randomness seeded once, outside the retry loop? | | | Correctness | `common-02` | High |
| C-04 | Correctness | Does the win condition match the written spec? | | | Correctness | `construction-00` | High |
| D-01 | Design | Does every mutable global have a writer/reader map? | | | Design | `construction-08` | High |
| D-02 | Design | Is duplicated logic named and extracted once? | | | Design | `construction-03` | Medium |
| D-03 | Design | Is the representation hidden behind an operation? | | | Design | `construction-09` | Medium |
| D-04 | Design | Does dispatch replace the type switch? | | | Design | `construction-12` | Medium |
| R-01 | Robustness | Is the core free of platform headers and calls? | | | Robustness | `construction-20` | High |
| R-02 | Robustness | Is every input length bounded before use? | | | Robustness | `construction-22` | Critical |
| R-03 | Robustness | Does every error path return a value or record a blocker? | | | Robustness | `construction-22` | High |
| P-01 | Practice | Does one command build the artifact from a clean tree? | | | Professional practice | `common-04` | High |
| P-02 | Practice | Do golden traces and a seed control exist? | | | Professional practice | `common-05` | High |
| P-03 | Practice | Are build, test and documentation evidence present? | | | Professional practice | `common-03` | High |
| E-01 | Evidence | Does every claim cite a file, line, command or document? | | | All | `common-05` | High |
| E-02 | Evidence | Is every claim labelled verified, documented, inferred or unknown? | | | All | `common-03` | Medium |
| E-03 | Evidence | Is the archive unmodified after the review? | | | All | `common-05` | High |
| E-04 | Evidence | Does every label or folder name match the mechanism actually present? | | | Evidence | `common-03` | Medium |

### Quality gate

Before you use the checklist, run these five tests on it.

1. **Two-reviewer test.** Pick one item. Could a second reviewer apply it without asking you what
   you meant? If not, rewrite the question.
2. **Evidence test.** Does the item name where the answer lives? "Read the code" is not an evidence
   source.
3. **Pass test.** Is the criterion binary? "Looks reasonable" is not a criterion.
4. **Class test.** Does the item name one failure class? If it names three, split it.
5. **Reuse test.** Does the question avoid `tablero`, `jugada`, `variant-02` and every other project
   noun? If not, move the noun to the evidence column.

An item that fails a test is not a moral failure. It is a draft. Rewrite it and keep the old version
in your evidence record; the diff is the learning.

### Stop rules and severity

Copy the five stop rules from the concept section into your checklist header. Add one line for
severity: **every finding gets a level and a context sentence, or it is not a finding.**

The stop rules are part of the instrument. A checklist without them produces a review that never
ends, or one that turns into an unrequested rewrite. Both are professional failures.

### The reuse test

Take two items and apply them to a different codebase you know: a personal project, an open-source
file, a work repository. If the item still makes sense, it is reusable. If it only makes sense in
`guerra-de-caballos`, it is archive-specific and belongs in the evidence column, not the question.

Write the result in one line per item. This is the evidence that the checklist is an instrument and
not a second diagnosis of the archive.

## Step 3 — Apply (20 min)

### The timed pass

Set a timer for 20 minutes. Open only `build/review-sample/sample.cpp` and your checklist. Do not
open the failure catalog, the track guide or your old notes.

Work in this order:

1. Skim the file for structure: includes, globals, functions, entry point. Two minutes.
2. Apply the correctness items. Five minutes.
3. Apply the design items. Five minutes.
4. Apply the robustness and practice items. Five minutes.
5. Apply the evidence items and write the findings table. Three minutes.

If an item does not apply, mark it `n/a` and say why. An `n/a` with a reason is data; an `n/a`
without one is a skipped item.

### Record findings

One row per finding, including the misses you notice after the timer.

| ID | Finding | Evidence (file:line or command) | Class | Severity | Checklist item | Verdict |
|---|---|---|---|---|---|---|
| F-01 | | | | | | confirmed / false positive / missed |
| F-02 | | | | | | |
| F-03 | | | | | | |

Rules for the table:

- A **confirmed** finding has a line, command or document.
- A **false positive** is a finding the checklist produced that the evidence does not support.
- A **missed** finding is an expected failure the checklist did not produce. Record it after the
  comparison, not during the timed pass.
- Do not edit the sample. The archive and the copy stay as they are.

### Compare with the catalog

After the timer, open the track guide §7 and the variant's known issues. The expected failures for
this sample are:

| Expected | Failure | Class | What the checklist should have produced |
|---|---|---|---|
| failure-001 | `i <= 64` over a 64-element array | Correctness | A bounds finding with the loop line. |
| failure-002 | `void main()` | Correctness | A portability finding with the entry-point line. |
| failure-003 | `srand(time(NULL))` inside a retry loop | Correctness | A determinism finding with both the seed and the loop. |
| failure-007 | Global mutable state | Design | A writer/reader map or a finding that the map is absent. |
| failure-008 | Eight hand-copied `case` bodies | Design | A duplication finding with a count or a range. |
| failure-009 | Parallel arrays or loose knight variables | Design | A representation finding with the declarations. |
| failure-012 | Input and output inside the movement logic | Design | A testability finding with the `cout` or `cin` line. |
| failure-015 | `conio.h`, `iostream.h`, `system("color")` | Robustness | A platform-boundary finding with the include lines. |
| failure-016 | No tests, no harness, no seed | Professional practice | A practice finding with the missing artifact named. |
| failure-019 | Comments that narrate or lie | Professional practice | A comment finding with the line and the contradiction. |

The list is a calibration set, not a complete diagnosis. A real review may find more; it may also
find a failure the catalog does not list. Both are valid, as long as the evidence is recorded.

### Measure

Compute the numbers below.

| Metric | Formula | My result |
|---|---|---|
| Detected | Confirmed expected failures | |
| Missed | Expected failures not detected | |
| False positives | Reported findings the evidence did not support | |
| Detection rate | Detected / 10 | |
| Time to first high-severity finding | Minutes from start | |

Then answer three questions in writing:

1. Which checklist item produced the most value per minute?
2. Which item produced the false positive, and how would you rewrite it?
3. Which missed failure was a checklist gap, and which was a reviewer gap?

The distinction matters. A checklist gap is fixed by adding or rewriting an item. A reviewer gap is
fixed by practice, or by a second reviewer. Do not confuse them.

### What the sample cannot prove

One variant, one reviewer and one timed pass is a small experiment. State the limits:

- It does not prove the checklist catches every failure class.
- It does not prove the detection rate generalizes to another codebase.
- It does not prove the archive is correct or incorrect; it proves what you found.
- It does not replace a test, a sanitizer or a trace.
- It does not measure false negatives outside the ten expected failures.

A reviewer who states these limits is more trustworthy than one who reports a perfect score.

## Step 4 — Retrospect (15 min)

### The track in one table

Group the twenty lessons by what they taught, not by their number.

| Phase | Lessons | What it taught | Artifact |
|---|---|---|---|
| Context | 00–03 | Provenance, build, lineage and spec before judgment. | Note, build, table, traces. |
| Correctness | 04 | The program does not do what it claims. | Failure cards and tests. |
| Design | 05–11 | Structure makes change expensive. | Maps, extractions, modules, types, dispatch, ports. |
| Practice | 12–15 | The artifact must be shareable and maintainable. | Names, docs, tests, clean tree. |
| Environment | 16 | The program works only in one environment. | Adapter and portable build. |
| Teaching | 17 | The label can lie about the artifact. | Label audit. |
| Synthesis | 18 | A refactor has a price and a preserved behavior. | Workshop report. |
| Review | 19 | The instrument and the retrospective. | Checklist and this record. |

### Four questions

Answer these before you write the keep/stop/start table.

1. **Strongest diagnosis.** Which finding in the whole track are you most confident in? Name the
   evidence and why it survives a challenge.
2. **Weakest diagnosis.** Which finding would you retract or weaken today? Name the missing
   evidence.
3. **Under-diagnosed class.** Which family did you consistently skip or treat as style? Correctness,
   design, robustness, practice or evidence?
4. **Over-diagnosed class.** Which family did you over-weight because it was easy to see? A reviewer
   who finds ten naming issues and no correctness bug has a calibration problem, not a clean file.

### Keep / stop / start

| Keep | Stop | Start |
|---|---|---|
| One habit that produced evidence. | One habit that produced noise or false confidence. | One habit you will use on the next review. |
| | | |

Examples, not prescriptions:

- Keep: writing the line number before writing the claim.
- Stop: calling a finding "high" without a context sentence.
- Start: applying the two-reviewer test to every new checklist item.

### Claim audit

Pick five claims from your portfolio. Label each one and, where possible, name the evidence.

| # | Claim from my portfolio | State | Evidence | What would change it |
|---:|---|---|---|---|
| 1 | | verified / documented / inferred / unknown | | |
| 2 | | | | |
| 3 | | | | |
| 4 | | | | |
| 5 | | | | |

If three or more claims are inferred or unknown, the portfolio is thinner than it looked. That is a
finding about the process, not about you. The retrospective's job is to make the next portfolio
stronger.

### What changed in how you read code

Write one paragraph. It should name a before and an after, not a virtue.

Weak: "I learned to write better code."

Strong: "Before, I read the entry point and followed the happy path. Now I read the declarations
first, count the writers of every global, and check the loop bounds before I trust the output. The
change is that I look for the class before I look for the instance."

### Blameless self-review

The archive's history is not the subject. You are not the subject either. The subject is the process
that made a decision look sufficient.

Ask: what made "boundaries checked" feel like a test? What made a confident claim survive without a
source? What would a second reviewer have caught in five minutes? The answers are process defects,
and process defects are fixable.

### The retrospective is about the process

Do not count lessons completed and call it progress. A learner can finish twenty lessons and still
write "boundaries checked." The retrospective measures the quality of the instrument and the honesty
of the evidence, not the number of pages read.

## Step 5 — Explain (12 min)

### The 60-second version

Write and say this out loud. If it takes more than 60 seconds, it is not ready.

> "This is a review checklist for legacy code. It has five families: correctness, design,
> robustness, professional practice and evidence. Each item names a question, the evidence that
> answers it, a pass criterion, a failure class and the Track A rung that prevents the class. I
> applied it to one archive variant in 20 minutes and found X of 10 expected failures, with Y false
> positives. The checklist does not prove runtime behavior and does not replace tests; it makes the
> review reproducible and the claims checkable."

Replace X and Y with your numbers. Do not round up.

### Defend the families

For each family, answer two questions: what does it catch, and what happens when it is missing?

| Family | What it catches | What a miss looks like |
|---|---|---|
| Correctness | Wrong behavior and undefined behavior. | A silent out-of-bounds write ships. |
| Design | Expensive change and hidden coupling. | Every rule change touches eight places. |
| Robustness | Environment and input assumptions. | The build fails on a second target. |
| Professional practice | Missing build, tests, documentation or clean tree. | Nobody can reproduce or trust the artifact. |
| Evidence | Unsupported claims and hidden unknowns. | Confidence replaces proof. |

A reviewer who cannot defend a family will drop it under time pressure. The defense is the reason
the family survives the next review under time pressure.

### What the checklist cannot do

Name at least three limits:

- It cannot prove runtime behavior; only a run, a test or a trace can.
- It cannot replace domain knowledge; a reviewer who does not know the rules will miss a wrong rule.
- It cannot remove the need for a second reviewer; one pass is one perspective.
- It cannot make a vague item precise by being on a list.
- It cannot fix the code; it can only produce evidence for a fix.

### Transfer to another codebase

The checklist transfers if the questions are generic. Test it on a codebase outside this project.
Keep the families and the shape; change the evidence column. A checklist that only works on
`guerra-de-caballos` is a diagnosis, not an instrument.

### Link to Track A

Track B says what hurts. Track A says what removes the hurt. The link is the rung, not the slogan.

| Checklist family | Failure it catches | Track A rung | Change test |
|---|---|---|---|
| Correctness | Off-by-one, uninitialized state, non-determinism. | `construction-02`, `construction-04`, `common-02` | Same change, localized; seeded trace twice. |
| Design | Global state, duplication, representation leak, switch on type. | `construction-08`, `construction-09`, `construction-12` | Board 8×8 → 10×10; only the board module changes. |
| Robustness | Platform lock-in, unbounded input, unhandled error. | `construction-20`, `construction-22` | Same core, console and test adapters. |
| Professional practice | No build, no tests, no documentation, dirty tree. | `common-04`, `common-05`, `construction-24` | One command builds; traces pass; cost defended. |
| Evidence | Claim without source, hidden unknown. | `common-03`, `common-05` | Every term defined; every trace reproducible. |

### The cost of a missed class

A missed correctness failure can corrupt memory. A missed design failure makes every future change
expensive. A missed robustness failure turns a build into a platform project. A missed practice
failure makes the artifact unusable or untrustworthy. A missed evidence failure makes the
review itself untrustworthy.

The costs are different, and that is why the families are not interchangeable. A reviewer who
optimizes for the easiest family will produce a clean-looking review with a predictable blind spot.

## Step 6 — Record (8 min)

Fill `EVIDENCE.md` (or `EVIDENCE-es.md`). At minimum:

- the portfolio inventory;
- the template audit and the rewrites;
- the checklist with all eight columns;
- the sealed sample, the timed pass and the findings table;
- the detection rate, the false positives and the misses;
- the retrospective, the claim audit and the keep/stop/start table;
- the archive integrity check.

Then confirm the archive is clean:

```bash
cd /path/to/archive
git status --short
```

The output must be empty. If it is not, you edited the evidence. Revert your change, copy the file
to a scratch directory, and redo the inspection there.

Keep the checklist in your portfolio. It is the artifact that outlives the lesson. The next review
starts from it, not from a blank page.

No commit and no push are required. This is a learning record, not a contribution to the archive.

## The archive in detail: 19 failures

This table is the link back to the failure classes and the Track A rungs. Use it to check coverage,
not to memorize the list.

| ID | Failure | Class | Diagnosed in | Track A rung |
|---|---|---|---|---|
| failure-001 | `i <= 64` over a 64-element array | Correctness | archaeology-04 | `construction-02` |
| failure-002 | `void main()` | Correctness | archaeology-04 | `construction-02` |
| failure-003 | `srand(time(NULL))` inside a retry loop | Correctness | archaeology-04, archaeology-14 | `common-02` |
| failure-004 | Broken pointer initialization in `_dev` | Correctness | archaeology-04 | `construction-07` |
| failure-005 | Inverted win condition in the precursor | Correctness | archaeology-03, archaeology-04 | `construction-00` |
| failure-006 | Unbounded name input | Correctness | archaeology-04, archaeology-16 | `construction-22` |
| failure-007 | Global mutable state | Design | archaeology-05 | `construction-08` |
| failure-008 | Eight hand-copied `case` bodies | Design | archaeology-06 | `construction-03` |
| failure-009 | Parallel arrays and loose knight variables | Design | archaeology-07 | `construction-04`, `construction-05` |
| failure-010 | `struct stablero` is not an abstract data type | Design | archaeology-07, archaeology-09 | `construction-09` |
| failure-011 | `switch` on move number and `if (cab == 1)` | Design | archaeology-10 | `construction-12` |
| failure-012 | Input and output inside the movement logic | Design | archaeology-11 | `construction-20` |
| failure-013 | Magic numbers | Design | archaeology-06, archaeology-07 | `construction-04` |
| failure-014 | Debug `cout` and live `enter` guard | Design | archaeology-06, archaeology-12 | `construction-03` |
| failure-015 | `conio.h` and `system("color")` | Robustness | archaeology-16 | `construction-08`, `construction-20` |
| failure-016 | No tests, no harness, no seed | Professional practice | archaeology-14 | `common-02`, `common-05` |
| failure-017 | No README, spec or build instructions | Professional practice | archaeology-13, archaeology-15 | `common-03`, `common-04` |
| failure-018 | Binaries and IDE files in the working folders | Professional practice | archaeology-15 | `common-04` |
| failure-019 | Comments that narrate or lie | Professional practice | archaeology-12 | `common-03` |

The mapping is bidirectional. If a review finds a failure with no rung, the taxonomy is
incomplete or the finding is not yet understood. If a rung has no failure, the lesson is
theoretical. Both are worth recording.

## What this checklist proves and does not prove

| Claim | Supported? | Why |
|---|---|---|
| The checklist is reusable outside this archive. | Only after the reuse test. | The questions are generic; the evidence is not. |
| The checklist found X of 10 expected failures. | Yes, for this sample and this reviewer. | The timed pass and the findings table record it. |
| The checklist catches every correctness failure. | No. | One variant and ten expected failures are a sample. |
| A missed finding is a checklist defect. | Not always. | It can be a reviewer gap or a bad timebox. |
| The archive is unmodified. | Yes, if `git status` is empty. | The command is the evidence. |
| The review replaces a test. | No. | A review cannot prove runtime behavior. |
| The retrospective is objective. | No. | It is a structured judgment with evidence, not a measurement. |

## Self-check (binary)

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

## Misconceptions this lesson refutes

**"A checklist is a list of best practices."** A best practice is a preference. A checklist item is
a test with evidence, a pass criterion and a class. The difference is whether two reviewers can
disagree productively.

**"More items means a better review."** More items means more time and more false positives. The
target is coverage of the classes, not volume. Fifteen sharp items beat fifty vague ones.

**"A missed finding means I am a bad reviewer."** A missed finding is data about the instrument or
the timebox. Fix the item, the evidence source or the stop rule. Do not turn a process defect into a
character judgment.

**"The retrospective is a summary of what I read."** A summary is not a retrospective. The
retrospective names what changed in your process, what you would retract and what you will do
differently.

**"The checklist replaces tests."** The checklist finds classes; tests prove behavior. A review that
claims runtime correctness without a run is overclaiming.

**"Blameless means no judgment."** Blameless means judge the decision and the system, not the
person. The review still has severity, evidence and a verdict.

## Terms used here

| Term | Meaning in this lesson |
|---|---|
| **Checklist item** | A question with evidence, a pass criterion, a failure class and a rung. |
| **Detection rate** | Detected expected failures divided by expected failures. |
| **Precision** | Real findings divided by reported findings. |
| **False positive** | A reported finding the evidence does not support. |
| **Miss** | An expected failure the checklist did not produce. |
| **Sealed sample** | A variant inspected without the failure catalog open. |
| **Timebox** | A fixed pass duration that ends the pass even if the file is unfinished. |
| **Claim audit** | Labelling portfolio claims as verified, documented, inferred or unknown. |
| **Keep/stop/start** | A retrospective format for one habit in each column. |
| **Blameless** | Judging the decision and the system, not the person. |

## What “done” means here

The **Self-check (binary)** above is the authoritative checklist. If every box is ticked, the
lesson is done for you. `meta.yml` mirrors the same criteria for tooling.

## Next step

Track B ends here. The next move is Track A, where the rungs remove the classes you diagnosed:

- `construction-02` for loops and bounds;
- `construction-08` and `construction-09` for modules and abstract data types;
- `construction-12` and `construction-14` for dispatch and generics;
- `construction-20` for ports and adapters;
- `construction-24` for the capstone cost report.

If you want to keep the archaeology habit, review a different codebase with the same checklist and
add one failure card the catalog does not have. If you want to improve the instrument, run a second
reviewer on the same sample and compare your findings. The disagreement is the next lesson.

## References

- [`README.md`](README.md) — the lesson card and the acceptance criterion.
- [`REVIEW-CHECKLIST.md`](REVIEW-CHECKLIST.md) — the template audited in Step 1.
- [`../../../docs/guide/archaeology-track.md`](../../../docs/guide/archaeology-track.md) — the
  taxonomy, the severity scale, the catalog and the Track A mapping.
- [`../../../docs/guide/lesson-catalog.md`](../../../docs/guide/lesson-catalog.md) — the Track B
  sequence and this lesson's catalog entry.
- [archaeology-04 — correctness failures][arch-04] — the failure-card format.
- [archaeology-14 — tests and determinism][arch-14] — the trace and seed discipline.
- [archaeology-18 — refactor report][arch-18] — the cost table and the preserved behavior.

[arch-04]: ../archaeology-04-correctness-failures/lesson.md
[arch-14]: ../archaeology-14-tests-determinism/lesson.md
[arch-18]: ../archaeology-18-refactor-workshop/WORKSHOP-REPORT.md
