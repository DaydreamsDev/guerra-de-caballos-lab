# archaeology-18 — Refactor workshop

> [English](lesson.md) | [Español](lesson-es.md)

> **Status:** drafted · **Concept:** all failure classes · **Prerequisites:** archaeology-04,
> archaeology-05, archaeology-08, archaeology-09 · **Estimated time:** 180 minutes
> **Reading:** ~18 minutes

## Why this lesson exists

Every Track B lesson ends the same way: a structural fix, and a line that says "Track A link:
`construction-08`" or "`construction-09`". That line is a promise. Nobody has paid it yet.

A diagnosis is cheap. A proposed refactor is a sentence. An executed refactor is a diff, a build, a
transcript that either matches or does not, and a bill. This workshop is the only place in Track B
where you must hand in the bill.

The skill it builds is the one that separates a reviewer from a consultant: **you can move working
code from a bad shape to a good one without breaking it, and you can say what the move cost.** That
is the whole job. Everything else in this laboratory is preparation for it.

The workshop is deliberately open. You choose the failure and you choose the rung, because choosing
is part of the competence. The lesson fixes only four things: the method, the evidence, the
acceptance criterion, and the honesty rules.

## What you will be able to do

By the end of this workshop you can:

- turn a defect into a runnable bench whose output is a function of its inputs;
- record a baseline transcript that survives a change, and prove it byte for byte;
- choose a target rung and say which class of failure it removes, not which syntax it adds;
- plan a refactor as a list of changes and a list of things that must not change;
- refactor in steps that each compile and each keep the transcript identical;
- separate a behavior-preserving refactor from a behavior-changing repair, and show both diffs;
- measure the cost of a design move in files, lines, public names and tests;
- state what a passing trace set proves and what it does not;
- keep the archive read-only while you work.

The workshop is done when the report has measured
numbers and the transcript is unchanged.

## The concept: a refactor is a proof obligation

### Three statements before the first edit

A refactor is not "cleaning up". It is a claim, and it needs three statements before the first edit:

| Statement | The claim | How it is falsified |
|---|---|---|
| Baseline | This is what the program does now, reproducibly. | A second run differs. |
| Target | This rung removes this class of failure. | The rung's change test still needs the old edit. |
| Invariant | These observable behaviors must not change. | The transcript differs by one byte. |

If you cannot write the first statement, you have nothing to preserve. If you cannot write the
second, you are renaming. If you cannot write the third, you are rewriting and calling it a
refactor.

### The baseline problem

Here is the trap that catches almost everyone on the first attempt: **the archive is not a usable
baseline.**

Three facts, all from earlier lessons:

- It does not compile today. `#include<conio.h>`, `#include<iostream.h>` and `void main()` are
  Microsoft Visual C++ 6.0 artifacts (`archaeology-16`, failure-002).
- It is not deterministic. `srand(time(NULL))` sits inside the placement retry loop
  (`variant-01:76`, failure-003), so placement changes between runs.
- It waits for a key. `getch()` appears five times (`variant-01:781, 806, 835, 861`), so its output
  is not a function of its inputs.

A program you cannot run twice cannot have a baseline. So the baseline is not the archive. The
baseline is a **bench**: the smallest compilable program that still contains the defect and writes a
transcript that depends only on its inputs.

That is the first professional move of the workshop, and it is not a technicality:

> You cannot preserve behavior you cannot observe.

### The bench has three parts

| Part | What it is | What goes wrong if you skip it |
|---|---|---|
| The defective code | The smallest function from the archive that carries the failure, transcribed | You refactor a program that does not have the defect |
| A controlled input | A seed, a name, a scripted move list, a scripted clock | The transcript changes for reasons unrelated to your edit |
| A transcript | One line per observable event, on standard output | You have no invariant to check |

Three rules keep the bench honest:

1. **Transcribe, do not repair.** Every repair before trace A destroys the baseline. If you fix the
   off-by-one while copying, you no longer have the failure you chose.
2. **Control the input, do not delete it.** A bench that removes the defect is not a baseline. If
   the defect is `srand(time(NULL))`, do not delete the call; replace `time(NULL)` with a scripted
   value that advances on each call and print the seed used. The defect stays visible, and the run
   becomes reproducible.
3. **Run it twice before you trust it.** Two runs, same input, `cmp`. If they differ, the input is
   not controlled yet.

Rule 2 is the one that separates a bench from a demo. Deleting the non-determinism makes the
transcript stable and destroys the evidence. Controlling it makes the transcript stable and keeps
the evidence.

### The correctness fork

Some failures are correctness defects. For those, two goals pull in opposite directions:

- a refactor must preserve behavior;
- a repair must change it.

You cannot do both in one step, and you should not try. The professional sequence is three steps,
each with its own diff:

| Step | What it does | Evidence | Commit message |
|---|---|---|---|
| 1 | Bench the defect | Trace A | `test: record trace A` |
| 2 | Refactor to the rung, bug included | Trace A still byte-identical | `refactor: ...` |
| 3 | Repair the defect | Trace B; the A→B diff is the justification | `fix: ...` |

Step 2 is the workshop's acceptance criterion. Step 3 is optional, and it is the part where you earn
the right to say what the bug cost.

Most people merge 2 and 3. The result is a diff where a moved line and a changed operator sit
together, and nobody, including you next month, can tell which change removed the defect and which
change moved the code. Keep them apart. The diff is the argument.

### Two proofs, not one

A refactor owes two independent proofs:

| Proof | Question | Fails when |
|---|---|---|
| Behavior preservation | Does the program still do exactly what it did? | The transcript differs |
| Capability gain | Can I now make the change this rung promises, cheaply? | The rung's change test still touches the same files |

The first proof is a trace comparison. The second is the change test printed in
[`lesson-catalog.md`](../../../docs/guide/lesson-catalog.md). A move that passes the first and fails
the second is a rename with extra files. A move that passes the second and fails the first is a
rewrite. The workshop accepts only the intersection.

## Before you start

Read these in full:

- [`README.md`](README.md) — the workshop card and the acceptance criterion.
- [`WORKSHOP-REPORT.md`](WORKSHOP-REPORT.md) — the deliverable you will fill.
- [archaeology-04](../archaeology-04-correctness-failures/lesson.md) — correctness failures and the
  six failure cards.
- [archaeology-05](../archaeology-05-global-state/lesson.md) — global state and hidden coupling.
- [archaeology-08](../archaeology-08-module-boundary/lesson.md) — the module boundary.
- [archaeology-09](../archaeology-09-abstract-data-type/lesson.md) — the abstract data type.
- [`../../../docs/guide/archaeology-track.md`](../../../docs/guide/archaeology-track.md) — §2, the
  autopsy protocol; §9, the failure-to-rung mapping.
- [`../../../docs/guide/lesson-catalog.md`](../../../docs/guide/lesson-catalog.md) — the Track A
  rows and their change tests.
- [`../../../docs/archive.md`](../../../docs/archive.md) — the pinned archive, tag and commit.

You also need:

- a local clone of the archive at tag `v1.0.0`;
- a C++20 compiler (`c++`, `g++` or `clang++`);
- `git`, so your refactor has a diff and your cost table has a source of truth;
- optionally `cmake` and a sanitizer-capable compiler;
- a scratch directory outside the archive and outside this repository's tracked files.

> **Rule:** the archive is read-only. Copy what you need into scratch; never edit `original/`.

## The 180-minute path

| Min | Step | What you do | Evidence |
| ---: | --- | --- | --- |
| 0–45 | **Inspect** | Choose the failure and the rung; build the bench; record trace A; count the coupling | `trace-a.txt`, four counts |
| 45–75 | **Plan** | Write the change list, the untouched list and the predicted cost table | Report §3 |
| 75–140 | **Refactor** | Reach the rung in steps, each compiling and each trace-equal | Step log, `git log` |
| 140–165 | **Verify** | Build with `-Werror`; traces; determinism; sanitizer | Report §5 |
| 165–172 | **Explain** | Fill the measured cost table and the verdict | Report §6–7 |
| 172–180 | **Record** | Fill `EVIDENCE.md`; confirm the archive is clean | Portfolio entry |

If the refactor takes 120 minutes instead of 65, that is a finding about
your plan, and it belongs in the report under §4, "What broke".

### The engineering check in this lesson

Track B uses the same habit as Track A, with the strongest verb in the track:

| Step | Where it happens | What it produces |
| --- | --- | --- |
| **Inspect** | Step 1 | A bench, trace A and four coupling counts |
| **Plan** | Step 2 | A change list and a predicted cost table |
| **Refactor** | Step 3 | The rung, one green step at a time |
| **Verify** | Step 4 | Traces, determinism and warnings |
| **Explain** | Step 5 | The measured cost and the verdict |
| **Record** | Step 6 | The report and the evidence log |

## Step 1 — Inspect (45 min)

### 1.1 Choose the failure and the rung

Pick one failure from a prerequisite lesson and one rung. These pairings are starting points, not
assignments; you must justify yours in one sentence each.

| Failure from | What the bench reproduces | Rung | What the rung buys |
| --- | --- | --- | --- |
| `archaeology-04`, failure-001 | `i <= 64` writes one element past the array | `construction-02` | The bound lives in one place, so a second loop cannot repeat it |
| `archaeology-04`, failure-003 | `srand(time(NULL))` inside the retry loop | `construction-08` | The seed is an input of a module, not a call buried in the loop |
| `archaeology-04`, failure-004 | `pvectab` reset to `&vectab[0]` every iteration | `construction-09` | The board owns its reset; no caller can reset it wrong |
| `archaeology-04`, failure-006 | `cin >> nombre.vectorNombre` into `char[250]` | `construction-11` | The name is a type with a bound the compiler and the class enforce |
| `archaeology-05`, failure-007 | Globals mutated by many functions | `construction-08` | State lives behind a module; the caller cannot name it |
| `archaeology-08` | One translation unit, no header | `construction-09` | The representation is out of the caller's build |
| `archaeology-09`, failure-010 | `struct stablero { int mvectab; }` | `construction-11` | The class holds the invariant; the field is private |

Two notes on the table:

- `construction-12` applies when the bench contains the piece dispatch and the eight hand-copied
  `case` bodies. `construction-13` applies when you reached a rung that uses inheritance and you now
  want to replace it with delegation. Both are legitimate targets; both are more expensive than the
  table suggests.
- The archive ships no class version (`variant-05` is empty by design), so `construction-11`,
  `construction-12` and `construction-13` are curriculum extensions, not historical artifacts. Say
  so in your report. It is the same scope note as
  [`archaeology-track.md`](../../../docs/guide/archaeology-track.md) §3.

Write two sentences in report §1 and §2:

> "I chose failure-NNN because it is the defect my bench can reproduce in under 50 lines."
> "I chose `construction-NN` because its change test is exactly the edit the legacy code makes
> expensive."

If either sentence is vague, pick again. A vague pairing produces a vague workshop.

### 1.2 Build the bench

The archive file will not compile. Do not fight it. Transcribe the smallest region that carries the
defect.

```bash
mkdir -p /tmp/gdc-workshop/bench
cd /tmp/gdc-workshop/bench
git init
```

Open the archive at the line your failure card cites. For `variant-01`:

```bash
sed -n '55,90p' \
  <archive>/original/variant-01-baseline-2008/source/guerra-de-caballos-baseline.cpp
```

That window holds the initialization loop, the seeding and the placement retry. Copy the function,
not the file. Then:

1. replace `iostream.h` / `conio.h` with `<cstdio>`;
2. replace `void main()` with `int main(int argc, char **argv)`;
3. replace `getch()` waits with a scripted input, or remove the pause entirely;
4. replace `time(NULL)` with a scripted clock value, if and only if the defect is not the seeding;
5. print the transcript with `std::printf`, one line per event.

A bench skeleton that works for most of the table:

```cpp
#include <cstdio>
#include <cstdlib>

int main(int argc, char **argv)
{
    const int seed = (argc > 1) ? std::atoi(argv[1]) : 17;
    std::printf("seed=%d\n", seed);
    // ... the transcribed legacy function, with its own trace lines ...
    return 0;
}
```

Then run it twice:

```bash
c++ -std=c++20 -Wall -Wextra -O2 bench.cpp -o bench
./bench 17 > run-1.txt
./bench 17 > run-2.txt
cmp run-1.txt run-2.txt && echo "bench is deterministic"
```

If `cmp` fails, stop. Nothing after this point means anything until the bench is a function of its
inputs.

### 1.3 Record trace A

```bash
./bench 17 > trace-a.txt
sha256sum trace-a.txt
cp trace-a.txt trace-a.expected.txt
```

Keep `trace-a.expected.txt` read-only for the rest of the workshop. It is the invariant. Do not edit
it, do not regenerate it, and do not "fix" it when a step goes red. If it changes, the refactor
changed behavior, and that is the finding.

Add a small runner so the check is one command, not four:

```bash
#!/bin/sh
set -eu
./bench 17 > /tmp/out.txt
cmp /tmp/out.txt trace-a.expected.txt
echo "trace-a: equal"
```

Commit the bench and trace A:

```bash
git add -A
git commit -m "test: record trace A for the legacy bench"
```

### 1.4 Count the coupling

These four numbers become the `before` column of the cost table. Take them now, from the bench and
from the archive, and write down which is which.

```bash
# 1. files in the artifact
ls *.cpp *.h 2>/dev/null | wc -l

# 2. lines in the artifact
cat *.cpp *.h 2>/dev/null | wc -l

# 3. public names a caller must know
grep -cE '^[A-Za-z_].*\);' header.h 2>/dev/null || echo "no header"

# 4. files you must read to answer: how is a cell stored?
grep -rln 'vectab\|tablero\|mvectab' . | wc -l
```

| Metric | Where it comes from | Why it matters |
| --- | --- | --- |
| Files | `ls` over the artifact | The unit of review and of rebuild |
| Lines | `wc -l` over sources | A cost, never a verdict |
| Public names | Declarations in headers | What a caller must learn to use the module |
| Representation files | `grep -rln` for the storage name | How far a representation change reaches |

The fourth number is the one that matters most and the one nobody measures. In `variant-03`, the
answer is one file, because everything is one file. In a healthy module, the answer is also one
file, but it is the implementation, not the caller. Same number, opposite meaning. Say which one you
have.

### 1.5 Write the defect sentence

One sentence, in this shape:

> The program does **X** because **Y** is not in **Z**.

Two examples:

> "The program clears only the first cell because the reset pointer is not part of the board's
> type."
> "The program places knights differently on every run because the seed is not an input of the
> placement function."

This sentence is what the target rung must make impossible. If the rung does not touch `Y` or `Z`,
you chose the wrong rung. Rewrite the sentence in report §1 and check it against §2.

## Step 2 — Plan (30 min)

### The change list

Fill report §3 before you edit anything. Four lists:

| List | What goes in it | Why it is separate |
| --- | --- | --- |
| Files to copy | What you take from the archive or from a Track A artifact | Copying is not changing |
| Files to change | What you will edit, by name | A list you can check off |
| Files to create | Headers, implementations, tests | New files are the rung's footprint |
| Untouched | Trace A, the bench's input, the archive | The invariant, stated as files |

The fourth list is the one people skip. Write it anyway. "Untouched: `trace-a.expected.txt`,
`bench.cpp`'s input handling, `original/`" is a claim you can verify at the end.

### The prediction

Fill the cost table in report §6 with predicted values **before** the refactor. Do not edit the
prediction afterwards. The gap between prediction and measurement is the learning signal; repairing
the prediction destroys it.

A prediction is a sentence plus a number, not a feeling:

| Metric | Predicted | Why that number |
| --- | ---: | --- |
| Files | | |
| Lines | | |
| Public names | | |
| Tests | | |
| Build time | | |

Most learners predict too few files and too many lines. The file count grows because a boundary
needs a header; the line count often shrinks because duplication disappears. If your prediction is
off by a factor of two, that is the most useful paragraph in your report.

### The rung's own change test

Copy the change test from the catalog into your plan. These are the acceptance criteria of the
Track A rungs this workshop accepts:

| Rung | Change test |
| --- | --- |
| `construction-08` | Board 8×8 → 10×10; only the board module changes |
| `construction-09` | Swap the board representation; callers compile; traces pass |
| `construction-11` | Private state and operations preserve the invariant; public state exposes an invalid board |
| `construction-12` | Add a `Piece` subclass; zero loop edits; original traces unchanged |
| `construction-13` | Replace inheritance with delegation; behavior and 20 traces unchanged |

You will run this test in Step 4. It is the second proof, and it is the reason the workshop is not
just "make it compile".

## Step 3 — Refactor (65 min)

### The five rules

1. **One mechanical step at a time.** A step is one idea: extract the header, move the definitions,
   split the translation unit, hide the representation. Each step compiles and each step keeps
   trace A equal.
2. **Never edit the trace.** If a step goes red, the code is wrong, not the expectation.
3. **Never mix a behavior change into a structural step.** That is the fork from §"The correctness
   fork", and it is not negotiable.
4. **Revert the step, not the refactor.** `git checkout -- .` takes you back to the last green
   commit, which is a known-good state. Reverting the whole refactor throws away information.
5. **Commit every green step.** The commit history is the cost table's source of truth, and it is
   the only honest answer to "how big was the change".

The loop, in full:

```bash
# 1. make one change
# 2. build and check
c++ -std=c++20 -Wall -Wextra -Werror -O2 *.cpp -o bench-new
./bench-new 17 | cmp - trace-a.expected.txt
# 3. if green, commit
git add -A && git commit -m "refactor: extract the board header"
```

Four lines, sixty-five minutes, repeated until the rung is reached. The discipline is the lesson.

### A modular split, step by step

The most common path is `archaeology-05` or `archaeology-08` to `construction-08`. Five steps, each
green:

| Step | Change | What it proves | What can break |
| ---: | --- | --- | --- |
| 1 | Extract declarations into `board.h`; keep the definitions where they are | The names are separable from the code | A declaration that does not match the definition |
| 2 | Move the definitions into `board.cpp`; still one translation unit | The split is textual before it is physical | A `static` helper that the caller used |
| 3 | Compile two translation units and link | The boundary is real | A missing include, a duplicate symbol |
| 4 | Make the representation private: no public field, no public array | The caller cannot name the storage | A caller that indexes the array directly |
| 5 | Delete the globals the module replaced; pass state as parameters | The hidden coupling is gone | A function that read a global through a side channel |

After step 5, run the change test: change the board dimensions in the board module and count the
files you touched. If the count is one, you bought `construction-08`. If it is five, you moved the
code and kept the coupling.

### When a step goes red

| Symptom | Most likely cause | Move |
| --- | --- | --- |
| Transcript differs by one line | You changed an observable behavior | Revert; find the line; ask whether it was ever in the invariant |
| Transcript differs in timing or ordering | You introduced a second output path | Revert; keep one writer |
| Link error: multiple definition | A header defines instead of declaring | Mark the definition `inline` or move it |
| Link error: undefined reference | A helper stayed `static` and the caller needs it | Put it in the header, or make the caller stop needing it |
| Build passes, change test fails | You moved code without hiding it | The rung is not reached; keep going |
| Build passes, sanitizer now reports | The refactor exposed the defect, or moved it | Read the report; it is a finding about the baseline |

The last row is worth its own paragraph.

### The sanitizer trap

A behavior-preserving refactor keeps the bug. So if you run AddressSanitizer before and after, and
it reports the same out-of-bounds write both times, the refactor is correct: it moved the code and
left the behavior alone.

If the sanitizer **stops** reporting after a refactor that was supposed to preserve behavior, one of
two things happened:

- you fixed the defect inside the refactor, which means your diff contains two changes and your cost
  table is wrong;
- you moved the write somewhere the sanitizer cannot see, which is worse than the original defect.

Both are findings. Neither is a success. Write down which one you have.

## Step 4 — Verify (25 min)

### Build

```bash
c++ -std=c++20 -Wall -Wextra -Werror -O2 *.cpp -o bench-new
```

`-Werror` is not decoration. A warning that compiles is a bug with a receipt, and the workshop's
whole argument is that receipts matter.

### Traces

```bash
./bench-new 17 | cmp - trace-a.expected.txt && echo "traces equal"
```

Then the byte-level diff, which you will need for the report:

```bash
diff -u trace-a.expected.txt <(./bench-new 17) || true
```

Use `cmp` to decide and `diff` to explain. `cmp` answers "equal or not"; `diff` answers "where and
why". Do not use `diff` alone: an empty diff and a diff you did not read look the same in a
screenshot.

If the artifact is the whole game rather than a local bench, run the full golden set:

```bash
TRACES=/path/to/lab/common/traces
for trace in "$TRACES"/seed-*.txt; do
    name="$(basename "${trace}" .txt)"
    seed="${name#seed-}"
    ./bench-new "$((10#${seed}))" | cmp - "${trace}"
done
```

Twenty traces, twenty silent `cmp` calls. Any output is a failure.

### Determinism

```bash
./bench-new 17 > run-a.txt
./bench-new 17 > run-b.txt
cmp run-a.txt run-b.txt && echo "deterministic"
```

Trace equality between two builds proves nothing if one build is not stable with itself. This check
takes two seconds and it catches every accidental `rand()` you left in.

### Sanitizers

```bash
c++ -std=c++20 -g -O1 -fsanitize=address,undefined *.cpp -o bench-asan
./bench-asan 17 > asan-out.txt 2> asan-err.txt
```

Record three things: whether `asan-out.txt` matches trace A, what `asan-err.txt` says, and whether
the report is the same as it was before the refactor. The third is the one that catches a hidden
fix.

### The correctness fork, verified

If you took the fork, you now have two transcripts:

| Transcript | Produced by | What it proves |
| --- | --- | --- |
| Trace A | The bench, and the refactored bench | Behavior preserved through a structural change |
| Trace B | The refactored bench plus the repair | The repair changed exactly what it claimed |

```bash
diff -u trace-a.expected.txt trace-b.txt
```

Every line in that diff must be named in report §5 and traced to the repair, not to the refactor. If
a line changes and you cannot say which change caused it, the fork failed and the evidence is gone.

## Step 5 — Explain (7 min)

### The cost table

Fill report §6 with measured values. The definitions matter more than the numbers:

| Metric | How to measure it | What it is a proxy for |
| --- | --- | --- |
| Files | `git diff --stat` for touched; `ls` for total | Review and rebuild surface |
| Lines | `git diff --numstat` for added/removed; `wc -l` for total | Reading cost, not quality |
| Public names | Count the declarations in the headers a caller includes | What a newcomer must learn |
| Tests | Count the automated checks and the failure classes they cover | Regression safety |
| Build time | Median of five clean builds, `time` each | Machine speed at this size |

Two honest notes that belong in your report:

- **Line count is a cost, not a verdict.** In this repository the ladder runs 507, 523, 546, 532,
  587 and 606 lines for `construction-08` through `construction-13`. `construction-11` is *smaller*
  than `construction-10` because encapsulation removed a layer of free functions. A rung that adds
  lines can still remove coupling.
- **Build time on a 500-line program measures your laptop.** Record it, then ignore it. It becomes a
  real metric at `construction-19`, where 10,000 games are the unit of work.

### The verdict paragraph

Three sentences, in report §7, in this order:

1. Did the rung preserve behavior, and what is the evidence?
2. What did it buy, in the rung's own change test?
3. What did it cost, and what did it not fix?

The third sentence is the one that separates a report from a sales pitch. Every rung leaves
something broken. `construction-08` gives you a boundary and no representation independence.
`construction-09` gives you the swap and a manual lifetime. `construction-11` gives you an invariant
and no polymorphism. Name the next defect.

## Step 6 — Record (8 min)

### Report versus evidence

Two documents, two jobs. Do not duplicate them.

| Document | Job | Shape |
| --- | --- | --- |
| `WORKSHOP-REPORT.md` | Argues the case | Seven sections, prose and a cost table |
| `EVIDENCE.md` | Proves the case | Commands, raw output, counts, digests, diffs |

A sentence such as "the refactor preserved behavior" belongs in the report. The command that shows
it belongs in the evidence. A claim with no command is an opinion, and an opinion is not a workshop
deliverable.

Fill in this order: §1 baseline, §2 target, §3 plan (already written), §4 refactor, §5 verification,
§6 cost, §7 verdict. Then the evidence log: setup, prediction, counts, trace A digest, the step log,
the final diff, the evidence boundary, the archive integrity check.

## A worked example: construction-08 → construction-09

The cheapest useful rung move in this repository, measured. It is here so you can calibrate your own
prediction.

Both artifacts are the same game, same 8×8 board, same rules, same transcript format. The move
changes who owns the board's layout.

| Measurement | `construction-08` | `construction-09` | Delta |
| --- | ---: | ---: | ---: |
| Source files | 7 | 7 | 0 |
| Total lines (code + build) | 507 | 523 | +16 |
| `board.h` lines | 7 | 11 | +4 |
| `board.cpp` lines | 57 | 65 | +8 |
| `game.cpp` lines | 284 | 288 | +4 |
| Golden traces matching | 20 of 20 | 20 of 20 | 0 |

The header before and after:

```cpp
// construction-08/board.h — the representation is in the caller's build
int board_row_count();
int board_column_count();
void board_reset();
bool board_was_visited(int row, int column);
void board_mark_visited(int row, int column);
```

```cpp
// construction-09/board.h — the representation is in board.cpp
struct Board;
Board *board_create();
void board_destroy(Board *board);
int board_row_count(const Board *board);
int board_column_count(const Board *board);
void board_reset(Board *board);
bool board_was_visited(const Board *board, int row, int column);
void board_mark_visited(Board *board, int row, int column);
```

Sixteen lines, four of them in the header, and the caller can no longer size, allocate or index a
board. That is what a rung move costs **when the boundary already exists**.

### Why it was cheap

`construction-08` already had a header and a separate `board.cpp`. The ADT move only changed what
the header revealed. The expensive part of `construction-09` had been paid one rung earlier.

This is the single most useful fact in the workshop: **rungs are cheap in the order they are taught
and expensive out of order.** Jumping from `variant-03` to `construction-09` is not the same move as
going from `construction-08` to `construction-09`, even though the target is identical.

### The expensive case

The same target, from the archive:

| Fact | Value | Source |
| --- | --- | --- |
| Lines in `variant-03` | 1058 | `wc -l` on the pinned file |
| Lines that mention `mvectab` | 102 | `grep -c` |
| Headers in the archive | 0 | `find` for `*.h` and `*.hpp` |
| Deterministic runs available | 0 | `srand(time(NULL))`, `getch()` |

Moving that file to `construction-09` in one step is not a refactor; it is a rewrite with a refactor
as its alibi. The five-step split in Step 3 is what makes it a refactor, and the reason the split
exists is that each step has its own trace comparison.

## What "traces equal" proves and does not prove

| Claim | Supported? | Why |
| --- | --- | --- |
| For input 17, the output is byte-identical. | Yes | `cmp` returned nothing; you recorded the digest. |
| The refactor changed no observable behavior for the inputs you ran. | Yes | The transcript is the observation. |
| The two programs are equivalent for every input. | No | You ran a finite set. |
| The refactor introduced no undefined behavior. | No | Only a sanitizer on the executed paths can speak to that. |
| The refactor removed the defect. | No | It preserved the defect on purpose. That is the fork. |
| The rung's capability was gained. | Only with its change test | Traces prove preservation, not capability. |
| The design is better. | No | "Better" needs the cost table and a context. |
| The archive is fixed. | No | You never edited it. |

The strongest claim in the workshop is a `cmp` that printed nothing. The weakest is "the design is
better". Keep them apart in every sentence you write.

## Self-check (binary)

- [ ] I chose one failure and one rung, and justified both in one sentence each.
- [ ] My bench transcribes the defect and does not repair it.
- [ ] My bench controls its input instead of deleting the defect.
- [ ] Two runs of the bench with the same input produced the same bytes.
- [ ] I recorded trace A, its command and its SHA-256 digest before the first edit.
- [ ] I recorded the four coupling counts, and I said which of them is the representation count.
- [ ] I wrote the change list, including the untouched list.
- [ ] I wrote the predicted cost table before the refactor and did not edit it afterwards.
- [ ] I copied the rung's change test from the catalog into my plan.
- [ ] Every committed step built with `-Wall -Wextra -Werror`.
- [ ] Every committed step kept trace A byte-identical.
- [ ] I reverted failed steps individually, not the whole refactor.
- [ ] I ran the rung's change test and recorded the number of files it touched.
- [ ] I ran the determinism check on the refactored artifact.
- [ ] I ran a sanitizer and explained whether its report changed.
- [ ] If I took the correctness fork, trace B exists and every changed line is attributed.
- [ ] The cost table contains measured values, not adjectives.
- [ ] The verdict names what the rung bought, what it cost and what it did not fix.
- [ ] `WORKSHOP-REPORT.md` or `WORKSHOP-REPORT-es.md` is complete.
- [ ] `EVIDENCE.md` or `EVIDENCE-es.md` is complete.
- [ ] The archive is unmodified (`git status` in the archive is clean).

If a box is not ticked, the unticked box names the next step. There is no penalty for repeating one.

## Misconceptions this lesson refutes

| Misconception | Why it is wrong | What to do instead |
| --- | --- | --- |
| "A refactor is a rewrite with better names." | A rewrite changes behavior; a refactor does not. | Keep trace A green at every step. |
| "If the tests pass, behavior is preserved." | Tests cover the inputs they run, and nothing else. | State the input set, then state the limit. |
| "The archive is the baseline." | It does not build, it is not deterministic and it waits for a key. | Build a bench with a controlled input. |
| "Preserving behavior means keeping the bug forever." | The refactor preserves it; a separate repair removes it. | Take the three-step fork. |
| "Deleting `srand(time(NULL))` makes the bench deterministic." | It also deletes the defect you chose. | Replace the clock, keep the call. |
| "More lines is a worse design." | `construction-11` is smaller than `construction-10`. | Price coupling, not line count. |
| "The change test and the traces prove the same thing." | One proves capability, the other proves preservation. | Run both, report both. |
| "A clean sanitizer run means no undefined behavior." | It covers only the paths that ran. | Say "no report on these paths". |
| "Build time tells you about the design." | At 500 lines it measures the machine. | Record it, then ignore it. |
| "One big commit is the same as five small ones." | The diff is the evidence, and one big diff has none. | Commit every green step. |
| "If trace A changed, the refactor was wrong." | Maybe trace A was never a function of the input. | Check determinism before blaming the code. |
| "Copying `construction-09` and renaming it is a refactor." | The bench is legacy-shaped; the rung must be applied to it. | Apply the rung to your bench, step by step. |
| "A rung move is always an improvement." | A rung you cannot afford is a debt. | Price it, then decide. |

## Terms used here

| Term | One-line meaning |
| --- | --- |
| Baseline | What the program does now, reproducibly. |
| Bench | The smallest runnable program that contains the defect and writes a transcript. |
| Trace | The recorded output of one run, byte for byte. |
| Trace A | The baseline transcript the refactor must not change. |
| Trace B | The transcript after a labeled repair, with its diff explained. |
| Characterization test | A test that pins current behavior, correct or not. |
| Behavior preservation | No observable output changes for the recorded inputs. |
| Refactor | A change to structure that preserves behavior. |
| Repair | A change to behavior, labeled and justified separately. |
| Rung | One Track A lesson, defined by its change test. |
| Change test | The edit a rung promises to make cheap. |
| Coupling count | How many files must change when one concept changes. |
| Byte identity | Two outputs equal at the byte level, decided by `cmp`. |
| Golden trace | A transcript recorded as the behavioral contract. |
| Sanitizer | A runtime checker for memory and undefined-behavior errors. |
| Scratch copy | A working copy outside the archive and outside the tracked tree. |
| Revert | Return to the last green commit, not to the beginning. |

## What “done” means here

The **Self-check (binary)** above is the authoritative checklist. If every box is ticked, the
lesson is done for you. `meta.yml` mirrors the same criteria for tooling.

## Next step

- Continue to `archaeology-19` — the review checklist and the final retrospective. Bring the report:
  the checklist is what you would have wanted a reviewer to ask you during this workshop.
- If your prediction missed by a factor of two, write the reason. That paragraph is worth more than
  the cost table.
- If the refactor took twice the planned time, re-plan it as if you were starting again, and compare
  the two plans. Planning is the skill this workshop is really about.
- If you want a harder target, take the same bench to `construction-12` and count the loop edits.
  Then decide whether the extra rung was worth its price.
- If you want the opposite exercise, take a rung you already built and move it *down*: reintroduce a
  global and measure what the traces still prove. That is what the rung was buying.

## References

- [`README.md`](README.md) — the workshop card and the acceptance criterion.
- [`WORKSHOP-REPORT.md`](WORKSHOP-REPORT.md) — the report you fill in.
- [`EVIDENCE.md`](EVIDENCE.md) — the evidence log you fill in.
- [archaeology-04](../archaeology-04-correctness-failures/lesson.md) — correctness failures.
- [archaeology-05](../archaeology-05-global-state/lesson.md) — global state and hidden coupling.
- [archaeology-08](../archaeology-08-module-boundary/lesson.md) — the module boundary.
- [archaeology-09](../archaeology-09-abstract-data-type/lesson.md) — the abstract data type.
- [construction-08](../../../construction/lessons/construction-08-modular/lesson.md) — modular
  programming.
- [construction-09](../../../construction/lessons/construction-09-abstract-data-type/lesson.md) —
  abstract data type.
- [construction-11](../../../construction/lessons/construction-11-encapsulation/lesson.md) —
  encapsulation.
- [construction-12](../../../construction/lessons/construction-12-polymorphism/lesson.md) —
  polymorphism.
- [construction-13](../../../construction/lessons/construction-13-composition/lesson.md) —
  composition.
- [`../../../common/traces/README.md`](../../../common/traces/README.md) — the golden trace set and
  how to verify it.
- [`../../../docs/guide/archaeology-track.md`](../../../docs/guide/archaeology-track.md) — §2, the
  autopsy protocol; §9, the failure-to-rung mapping.
- [`../../../docs/guide/lesson-catalog.md`](../../../docs/guide/lesson-catalog.md) — the Track A
  change tests.
- [`../../../docs/archive.md`](../../../docs/archive.md) — the pinned archive, tag and commit.
- Archive: <https://github.com/DaydreamsDev/DaydreamsDev-guerra-de-caballos-archive>, tag `v1.0.0`,
  commit `284e144e41403afbf0cc44422876425cacfb2bd5`.
