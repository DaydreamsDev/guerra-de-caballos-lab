# Capstone: cost report (informe de costes)

> [English](lesson.md) | [Español](lesson-es.md)

**Concept:** tradeoff justification · **Time:** about 180 minutes ·
**Status:** drafted
**Reading:** ~20 minutes

## Why this lesson exists

Track A ends here. There is no new code baseline and no new syntax. This directory has no `code/`
on purpose: the two copies you make are the baselines. You take two rungs you have already built,
apply the same change to both, measure what it costs, and write one page that defends a verdict
with numbers.

The change is fixed:

- add a third piece type;
- add a 10×10 board;
- keep the transcript contract;
- keep all 20 golden traces passing on the unchanged 8×8 path.

The sentence to keep:

> A design is not cheap or expensive in the abstract. It is cheap or expensive for a named change,
> and the cost is the set of files, lines, concepts and tests that the change touches.

Three honest notes before you start.

1. The artifact is a report, not a program. The two scratch copies are disposable. The one-page
   report and the evidence record are what you keep. If the copies compile but the report has no
   prediction, the capstone is not done.
2. The comparison is only as fair as the change. Both copies get the same third piece, the same
   10×10 entry point, the same acceptance and the same compiler. The route may differ, because the
   rungs force different routes; when it does, the route is part of the cost and you record it.
3. The 20 traces are a control, not a proof. They cover the 8×8 game only. There is no golden trace
   for 10×10 and none for the third piece. The report must say what is proven and what is only
   smoke-tested.

## What you will be able to do

By the end of this lesson you can explain tradeoff justification and apply the lesson's change test.

## The concept: cost is a property of the next change

Every earlier lesson measured one design against another with one change. This capstone does the
same thing, but it does not tell you which change is "the" change. It fixes one and asks you to
apply it twice.

The unit of measurement is a **change test**: a small requirement applied to two variants. The
output is not a winner. The output is a bill. The check you are running is
**predict → measure → compare → explain → record**.

| Metric | What it measures | Why it is in the table |
|---|---|---|
| Files touched | Coupling | A change that reaches the build file is more coupled than one that does not. |
| Lines added and removed | Blast radius | Size is not quality, but it predicts review time. |
| New concepts required | Learning cost | A route that needs a new idea costs more than one that reuses an old one. |
| Tests broken | Safety net | A change that breaks the contract on the way is riskier than one that does not. |
| Compile time | Feedback loop | A slower build is a slower learner. |
| Binary text bytes | Code growth | It is the only size number the compiler gives you for free. |
| Golden traces passed | Contract | The control that says the 8×8 game did not move. |

Two of these are weak signals and you must say so. Compile time on a 600-line program is dominated
by headers and process startup, not by your design. Binary text moves with inlining and with the
optimizer. They belong in the table because the template asks for them, not because they decide the
verdict. The verdict is decided by the numbers you can defend.

### What "cheaper" means here

Cheaper means: for this change, on this machine, with these routes, one rung cost less. It does not
mean the rung is better in general. The next change can reverse the bill, and the last section of
the report asks you to name the change that would reverse it.

That is the whole discipline of Track A: a design is a bet on which change comes next. The capstone
is where you show your bet and its price.

### Why the pair matters more than the rungs

A comparison is only readable if most variables are held constant. The two recommended pairs are
chosen for that:

| Pair | What it holds roughly constant | What it varies | What it isolates |
|---|---|---|---|
| 12 and 13 | The concrete 8×8 `Board`; the turn loop's shape | The piece design: interface versus concrete member | The cost of a third piece |
| 14 and 15 | The move generator; the piece type, still `Knight` | The engine shape: free functions versus class templates | The cost of a 10×10 board |

If you compare 12 and 15, three things change at once: the piece design, the board design and the
engine shape. You can still write a report, but you cannot attribute the difference to one cause.
If you do it anyway, say so in the verdict and do not claim more than the numbers show.

## Before you start

- [`construction-12`](../construction-12-polymorphism/lesson.md) — the interface rung. It is the
  catalog prerequisite and one side of the recommended first pair.
- [`construction-15`](../construction-15-generic-classes/lesson.md) — the generic rung. It is the
  other catalog prerequisite. Its `Board<RowCount, ColumnCount>` and `GameEngine<8, 8>` are the
  cheapest 10×10 route in the ladder.
- [`construction-13`](../construction-13-composition/lesson.md) and
  [`construction-14`](../construction-14-generic-functions/lesson.md) — the rungs between them. The
  recommended pairs use them. If you skipped one, either do it first or accept a comparison that
  changes two variables at once and say so in the report.
- [`construction-00`](../construction-00-read-the-rules/lesson.md) through
  [`construction-11`](../construction-11-encapsulation/lesson.md) — you can read a class, a header,
  a build script and a trace.
- The rules: [`common/spec/game-rules.md`](../../../common/spec/game-rules.md) — §1 fixes the board
  and its 1-based numbering, §4 fixes the knight move order, §8 fixes when a cell becomes visited.
- The transcript format:
  [`common/spec/transcript-format.md`](../../../common/spec/transcript-format.md).
- A C++20 compiler (`g++` or `clang++`). `cmake` is optional; each `run.sh` falls back to a direct
  compile when CMake is absent.

You do not need to know how to template a loop or how to lay out a vtable. You need to copy a
directory, edit it, build it, count what you changed, and write down which number decided your
answer.

### What you need

- Two of these four rungs, each with `code/`, `run.sh` and `CMakeLists.txt`:
  [`construction-12`](../construction-12-polymorphism/),
  [`construction-13`](../construction-13-composition/),
  [`construction-14`](../construction-14-generic-functions/),
  [`construction-15`](../construction-15-generic-classes/).
- The measuring helper: [`measure.sh`](measure.sh).
- The report template: [`REPORT-TEMPLATE.md`](REPORT-TEMPLATE.md).
- This learner record: [`EVIDENCE.md`](EVIDENCE.md).
- The rules and the transcript format, listed above.
- One golden trace, for the first diff:
  [`common/traces/seed-0001.txt`](../../../common/traces/seed-0001.txt).
- Scratch space outside the repository. This lesson uses `/tmp/opencode/capstone`; any writable
  path works, but do not create the copies inside the lesson directory.

Check that the artifacts are present before you begin, from this lesson directory:

```bash
# common artifacts
test -f ../../../common/traces/seed-0001.txt &&
test -f measure.sh &&
test -f REPORT-TEMPLATE.md

# the two rungs you chose, for example 12 and 13
test -f ../construction-12-polymorphism/code/piece.h &&
test -f ../construction-13-composition/code/knight.cpp
```

If a rung is missing, choose another pair. A cost report against a rung you have not read is not
evidence. Record the substitution in `EVIDENCE.md` §11.

## The change, defined once

Both copies receive the same change. Write these definitions into `EVIDENCE.md` §2.1 before you
edit anything, and use them as the acceptance for both.

### The third piece

A new type that:

- answers the same four operations the rung's loop already asks: `row()`, `column()`,
  `set_position(row, column)` and `role_name()`;
- can be constructed from a role name;
- moves with the knight's eight offsets, so the golden traces remain a valid control;
- travels through the same turn loop, not through a copy of it;
- leaves the 8×8 transcript byte-identical.

The cheapest proof that it travels through the loop is substitution: construct the new type where
the rung constructs its two pieces, rebuild, and compare all 20 traces. Because the answers are
identical, the transcript is identical, and the 20 traces prove the loop accepted a different type.
Keep the role names `player` and `machine`: the role name is part of the transcript, so renaming it
changes the control. You may keep the substitution or restore the original; say which you did.

A second proof is a scratch entry point that runs the loop with the new type. It costs more lines
and proves the same thing. If you choose it, count it.

Do not give the third piece a different movement rule in this capstone. That is a different change,
and it changes the interface, not just the type. `construction-12` §5.5 measures it; the capstone
mentions it in the verdict as the next change, not as this one.

### The 10×10 board

A second entry point in the scratch copy that:

- runs the same turn loop on a 10×10 active area;
- accepts the same `--seed` and `--name` arguments, or calls the same function with them;
- prints the same transcript format;
- does not change the default 8×8 entry point's output.

Acceptance for the 10×10 run: it starts, prints `initial_player` and `initial_machine` with both
coordinates in 1..10, and ends with `winner=` and `moves=`. That is a smoke test, not a golden
trace. There is no 10×10 contract in `common/traces/`.

Three honest routes to 10×10, each with a price:

| Route | What it changes | When it is the cheap one |
|---|---|---|
| Runtime dimensions | `Board` takes rows and columns; `run_game` passes them | The board is a concrete class (12, 13, 14) |
| Template dimensions | `Board<Rows, Columns>` and an engine over them | The rung already has class templates (15) |
| A second board type | A copy of the board with 10×10 constants | Never in this capstone; it duplicates the rules |

The third route is listed so you can reject it with a reason: it duplicates the bounds logic, and
the report would measure a copy, not a design.

### What must not move

- The 20 golden traces, byte for byte, on the default 8×8 path.
- The transcript format: the same line order, the same keys, the same values.
- The game rules: bounds, capture, visited, trap, draw.
- The tracked files in the repository. Every edit happens in a scratch copy.

## Step 1 — Choose the pair and copy (~15 min)

### 1.1 Choose the pair

Use 12 and 13 if you want to measure the price of a third piece. Use 14 and 15 if you want to
measure the price of a 10×10 board. Do not choose the pair you expect to win; choose the pair whose
difference you can explain.

Write one sentence in `EVIDENCE.md` §2.1: "I chose A and B because they hold ______ constant and
vary ______." If you cannot finish the sentence, choose the other pair.

### 1.2 Copy the whole lesson directory

`measure.sh` calls `run.sh` inside the copy, so the copy needs `code/`, `run.sh` and
`CMakeLists.txt`. Copy the directory, not only `code/`:

```bash
mkdir -p /tmp/opencode/capstone
rm -rf /tmp/opencode/capstone/rung-a /tmp/opencode/capstone/rung-b
cp -r ../construction-12-polymorphism /tmp/opencode/capstone/rung-a
cp -r ../construction-13-composition /tmp/opencode/capstone/rung-b
rm -rf /tmp/opencode/capstone/rung-a/build /tmp/opencode/capstone/rung-b/build
```

The last line matters. A copied `build/` contains a binary whose timestamp is newer than the
sources, so `run.sh` will not rebuild after your first edit and you will measure the old program.

Record the commit you copied from, or the date if this checkout has no commit yet, so the two sides
are the same baseline:

```bash
git rev-parse HEAD 2>/dev/null || date -u +%Y-%m-%dT%H:%M:%SZ
```

### 1.3 Measure both baselines before touching anything

```bash
./measure.sh /tmp/opencode/capstone/rung-a
./measure.sh /tmp/opencode/capstone/rung-b
```

The output on this repository, for the four candidate rungs:

```text
construction-12  files=10 lines=540 traces=20/20
construction-13  files=11 lines=556 traces=20/20
construction-14  files=12 lines=587 traces=20/20
construction-15  files=12 lines=598 traces=20/20
```

Record your two lines in `EVIDENCE.md` §2.2. These are the "before" numbers. `measure.sh` counts
every `.h` and `.cpp` directly under `code/`, so a new entry point or probe under `code/` will
increase the count. That is why the report distinguishes the size of the copy from the delta you
caused.

## Step 2 — Predict before you touch code (~20 min)

This is the step the capstone is really about. Write the prediction in `EVIDENCE.md` §3 and do not
edit it later. If the observation disagrees, the gap is the result.

### 2.1 The cost prediction

| Metric | Rung A | Rung B | Confidence |
|---|---:|---:|---|
| Files touched | | | |
| Lines added | | | |
| Lines removed | | | |
| New concepts required | | | |
| Traces broken on the first green build | | | |
| Compile time, median of 3 (seconds) | | | |
| Binary text bytes | | | |
| Golden traces passed at the end | | | |

Confidence is `high`, `medium` or `low`. A row you mark `low` and get wrong is worth more than a row
you mark `high` and get right by luck.

### 2.2 The route prediction

| Question | My prediction |
|---|---|
| Third piece route in A | |
| Third piece route in B | |
| 10×10 route in A | |
| 10×10 route in B | |
| Files I expect to touch in A | |
| Files I expect to touch in B | |
| Which half dominates in A: piece or board? | |
| Which half dominates in B: piece or board? | |

The route is part of the cost. If A needs a new translation unit and B does not, that is a file and
a build-file edit, and it belongs in the table.

### 2.3 The verdict prediction

| Question | My prediction |
|---|---|
| Which rung I expect to be cheaper | |
| The number that I expect to decide it | |
| The number that could reverse it | |
| The cost I expect the table not to show | |
| The prediction I am least sure about | |

Do not write "it depends". Write the number you expect and the condition under which it flips.

## Step 3 — Apply the change to rung A (~40 min)

Work in `/tmp/opencode/capstone/rung-a`. The order below is a suggestion, not a rule; what matters
is that you record the checkpoints.

### 3.1 Third piece first

It is the smaller unit, and it tells you whether the loop is open. The route depends on the rung:

- **construction-12:** the loop holds `Piece *`. Add the new type in its own header and source, add
  the source to `CMakeLists.txt` and to the fallback list in `run.sh`, then substitute it at the two
  constructions in `run_game`. That is the whole change; the loop is untouched.
- **construction-13:** the loop names `Knight` in ten helper signatures. You must either make those
  helpers templates over the piece type, or duplicate the loop. Record which route you took. The
  first reuses the idea from `construction-14`; the second duplicates the turn logic.
- **construction-14:** the generator already accepts any type with `row()` and `column()`, but the
  loop still names `Knight`. The same two routes as `construction-13` apply.
- **construction-15:** `GameEngine` declares two `Knight` members and six `Knight` signatures. Add a
  piece type parameter constrained by `PositionedPiece`, or duplicate the engine. The concept is
  already there; using it as a constraint is the work.

Checkpoint A1: the new type compiles and answers the four operations. A probe is enough:

```cpp
// /tmp/opencode/capstone/probe.cpp — scratch only
#include "piece.h"          // or knight.h, depending on the rung
#include "spare_knight_piece.h"

#include <cstdio>

int main()
{
    SpareKnightPiece piece("player");
    piece.set_position(3, 4);
    std::printf("role=%s at=%d,%d\n", piece.role_name(), piece.row(), piece.column());
    return 0;
}
```

Checkpoint A2: all 20 traces still pass. If they fail, stop and read the first diff. A third piece
with the same answers cannot change the transcript; a failure means the default path changed.

### 3.2 The 10×10 entry point

Add a second entry point under `code/`, for example `main_ten.cpp`, that calls the same loop with
10×10 dimensions. Keep the default entry point untouched.

Checkpoint A3: the 10×10 run prints the header and ends with `winner=` and `moves=`. Check that the
initial coordinates are in 1..10.

Checkpoint A4: all 20 traces still pass. The 10×10 entry point has a second `main`, so it cannot be
linked into the default binary; the build target must list the default sources only.

### 3.3 Three traps that distort the measurement

1. **The build file is a file.** `CMakeLists.txt` and the fallback source list in `run.sh` hardcode
   the translation units. A new `.cpp` is invisible until you edit both. Count both as files
   touched. This is not busywork; it is the coupling the table is measuring.
2. **`run.sh` in 12 and 13 does not check headers.** After editing `board.h` or `piece.h`, delete
   `build/` or touch a `.cpp`, or you will run the old binary. `run.sh` in 14 and 15 does check
   headers.
3. **Never glob `code/*.cpp` for the default build.** The 10×10 entry point has its own `main`, so
   the glob produces a duplicate symbol at link time. List the sources explicitly.

### 3.4 Record as you go

Keep a checkpoint log in `EVIDENCE.md` §4.1:

| Checkpoint | Command | Result | Files touched so far | Traces |
|---|---|---|---|---|
| Baseline | `./measure.sh` | | 0 | 20/20 |
| A1 third piece compiles | | | | |
| A2 traces after the piece | | | | |
| A3 10×10 runs | | | | |
| A4 traces after the board | | | | |

The log is how you answer "time to first green" later. The report's table does not have a row for
it, but §5 asks for the cost the table does not show.

## Step 4 — Apply the same change to rung B (~40 min)

Same definitions, same acceptance, same order if it fits. The rule is: do not improve B while you
are there. A rename, a cleanup or an extra test that A did not get makes the comparison
unreadable.

Checkpoints B1 to B4 mirror A1 to A4. Record them in `EVIDENCE.md` §4.2.

If B cannot be made to compile within your time budget, that is a measurement, not a failure. Record
the wall-clock time, the last error, and the route you tried. Do not invent numbers, and do not
quietly drop B from the report. "B did not reach a green build in 40 minutes; A did it in 12" is a
legitimate cost result.

## Step 5 — Measure (~20 min)

Run every command from the capstone directory. The scratch paths are variables so the commands are
the same for both rungs.

### 5.1 Files and lines

`measure.sh` gives the size of the copy. The delta comes from `git diff --no-index`:

```bash
original=../construction-12-polymorphism
scratch=/tmp/opencode/capstone/rung-a

git diff --no-index --numstat "$original/code" "$scratch/code" || true
git diff --no-index --numstat "$original/CMakeLists.txt" "$scratch/CMakeLists.txt" || true
git diff --no-index --numstat "$original/run.sh" "$scratch/run.sh" || true
```

Each line is `added<TAB>removed<TAB>path`. Sum the columns:

```bash
{
  git diff --no-index --numstat "$original/code" "$scratch/code" || true
  git diff --no-index --numstat "$original/CMakeLists.txt" "$scratch/CMakeLists.txt" || true
  git diff --no-index --numstat "$original/run.sh" "$scratch/run.sh" || true
} | awk '{added += $1; removed += $2; files += 1}
       END {printf "files=%d added=%d removed=%d\n", files, added, removed}'
```

Count a file once, even if it appears in two diffs. `--numstat` reports added and removed lines
including comments and blank lines; say so in the report. `git diff --no-index` exits with status 1
when there are differences, which is why every call has `|| true`.

### 5.2 New concepts

A concept is new if the rung's own lesson did not already demonstrate it. Name each one and point
at the line that uses it. Examples you may legitimately count:

- a runtime dimension: a constructor parameter and members replacing `static constexpr`;
- a second entry point and a second `main`;
- a new translation unit added to the build;
- a template parameter on the turn loop;
- a concept used as a constraint rather than a `static_assert`;
- a duplicated loop, if you chose that route.

Do not count "reading the code", "using the compiler" or "adding a file" twice. The number is a
judgement; the list is the evidence. A count without a list is not accepted.

### 5.3 Tests broken and traces passed

These are two numbers, not one.

- **Tests broken:** the number of the 20 traces that fail on the first build that compiles after
  your change. This is the moment the contract was at risk.
- **Golden traces passed:** the number that pass at the end. The acceptance is 20.

Count failures without aborting on the first one:

```bash
passed=0
for trace in ../../../common/traces/seed-*.txt; do
  traceName="$(basename "${trace}" .txt)"
  seedText="${traceName#seed-}"
  seedNumber="$((10#${seedText}))"
  if "$scratch/run.sh" --seed "${seedNumber}" --name Learner | cmp -s - "${trace}"; then
    passed=$((passed + 1))
  else
    printf 'failed seed %s\n' "${seedNumber}"
  fi
done
printf 'passed=%d/20\n' "${passed}"
```

Then run the helper for the final record:

```bash
./measure.sh "$scratch"
```

If a trace failed, read the first differing line with `diff -u` and write down what it told you. A
failure in `initial_player` means the board dimensions reached the default path. A failure in
`who=` or `move=` means the third piece changed the roles or the order. A failure in `winner=` or
`moves=` means a rule moved.

### 5.4 Compile time

Use the same compiler, the same flags and the same source list for both rungs. Build the default
program, not the 10×10 entry point:

```bash
cd "$scratch"
sources="code/main.cpp code/game.cpp code/board.cpp code/knight_piece.cpp \
code/spare_knight_piece.cpp code/random_sequence.cpp"
rm -rf build
for run in 1 2 3; do
  /usr/bin/time -f "run=${run} seconds=%e" g++ -std=c++20 -Wall -Wextra -Werror -O2 \
    -Icode -o "/tmp/opencode/capstone/default-${run}" ${sources}
done
```

Record the median of the three, not the best. Record `g++ --version` once. If one rung uses CMake
and the other does not, the comparison includes the configure step and you must say so.

The source list above is an example for `construction-12`. Replace it with the list for your rung,
and keep it identical across the three runs. The variable is expanded unquoted on purpose: the shell
must split it into separate file arguments. If `/usr/bin/time` is not GNU time on your platform, use
the shell's `time` keyword and take the `real` line instead.

### 5.5 Binary text bytes

```bash
size /tmp/opencode/capstone/default-1 | awk 'NR==2 {print "text=" $1 " data=" $2}'
```

Text is code and read-only data; data is writable initialized data. Report text, as the template
asks. If the number moves in the opposite direction from the source, say why: inlining, a vtable,
or a template instantiation can do that.

### 5.6 Fill the comparison

Put the numbers in `EVIDENCE.md` §6 and in `REPORT-TEMPLATE.md` §3. Do not copy the prediction into
the observation column. The gap is the point.

| Metric | Rung A | Rung B |
|---|---:|---:|
| Files touched | | |
| Lines added | | |
| Lines removed | | |
| New concepts required | | |
| Tests broken | | |
| Compile time (seconds) | | |
| Binary text bytes | | |
| Golden traces passed | | |

## Step 6 — Write the one-page report (~20 min)

Open [`REPORT-TEMPLATE.md`](REPORT-TEMPLATE.md) and fill all six sections. The page limit is real:
raw command output belongs in `EVIDENCE.md`, not in the report.

### 6.1 The verdict rule

The verdict is one sentence with three parts:

1. which rung was cheaper **for this change**;
2. which measured number decided it;
3. which number came closest to reversing it.

Example shape, not an answer: "For the third piece and the 10×10 board, rung A was cheaper; the
number that decided it was files touched, and compile time came closest to reversing it."

If your sentence has no number, it is an opinion. If it has no scope, it is a slogan.

### 6.2 The cost the table does not show

Answer this in §5 of the report. Candidates, in order of usefulness:

- time to first green build, from your checkpoint log;
- number of build attempts before the first green;
- route changes: did you start with one route and abandon it?
- conceptual load: which new idea took the longest to understand?
- risk: what would a reviewer have to re-check by hand?
- what you did not measure: runtime, memory, review time, maintenance.

Pick the one that actually changed your verdict or your confidence. A list of six is not an answer.

### 6.3 The next change

Name one change you would make next and predict its cost in the same table. This is the forward
test: if you cannot predict it, you have not understood the coupling you just measured.

Good candidates:

- a third piece with a different movement rule;
- a board size chosen at runtime from `argv`;
- a second piece type in the loop without a template;
- a transcript format change.

### 6.4 Calibration

Compare the prediction with the observation, row by row. Name the row with the largest gap and
explain it mechanically, not morally. "I predicted two files and touched nine because the build
file lists every source" is calibration. "I underestimated the complexity" is not.

## Step 7 — Defend the verdict (~15 min)

A reviewer who has not seen your copies asks three questions. Fold the answers into §5 of the
report: fairness into the deciding-number bullet, reversal into the next-change bullet, and blind
spots into the cost-the-table-does-not-show bullet. One sentence each.

1. **Why is the comparison fair?** Same change, same acceptance, same compiler and flags, same
   machine, same traces. If the routes differ, say how and why the difference is still attributable.
2. **What would reverse the verdict?** Name a concrete change and the metric it would move. "If the
   next change were a different movement rule, the interface rung would pay for a new operation in
   the interface and the generic rung would pay for a new constraint."
3. **What did you not measure?** Runtime, memory, review time, team cost. Say which one matters for
   your next change.

Then write the one-sentence explanation without the words *better*, *worse*, *cleaner*, *more
flexible*, *more reusable*, *more elegant*, *modern* or *proper*. Mechanism words are allowed here,
because the subject is a design; verdict words are not, because they hide the number.

## Step 8 — Acceptance check and record (~5 min)

The acceptance criterion is: **a one-page report with a prediction, measured numbers for two rungs,
a verdict and a next change; both scratch copies pass all 20 traces on the unchanged 8×8 path.**

- [ ] I chose a pair and can finish the sentence "they hold ______ constant and vary ______".
- [ ] I copied two rungs to scratch directories outside the repository and removed `build/`.
- [ ] I recorded the commit and both baseline `measure.sh` outputs before editing.
- [ ] I wrote the prediction, with confidence, before the first edit.
- [ ] I applied the same third piece and the same 10×10 entry point to both copies.
- [ ] The third piece travels through the same turn loop, and I said how I proved it.
- [ ] The 10×10 entry point runs and prints the transcript format.
- [ ] Both copies pass 20/20 on the 8×8 path.
- [ ] I measured files and lines with `--numstat`, including the build files.
- [ ] I measured compile time as the median of three clean builds with the same flags.
- [ ] I recorded binary text bytes for the default program.
- [ ] I recorded tests broken on the first green build separately from the final 20/20.
- [ ] The report has all six sections and fits on one page.
- [ ] The verdict names the deciding number and the near-reversal.
- [ ] The next change is named and predicted in the same table.
- [ ] I did not edit any tracked file.

These are acceptance criteria. If a box is
unchecked, write the next smallest action in `EVIDENCE.md` §10.

## Self-check (binary)

- [ ] I can say what the change cost in each rung without opening the report.
- [ ] I can name the metric that decided the verdict and the one that nearly reversed it.
- [ ] I can explain why the pair I chose isolates the variable I claimed.
- [ ] I can say what the 20 traces prove and what they do not.
- [ ] I can name the next change and predict its cost.
- [ ] The prediction and the observation are both in `EVIDENCE.md`, unedited.
- [ ] The report is one page and a stranger could check every number from §5.
- [ ] I completed `EVIDENCE.md` §1–§12.

## Misconceptions this lesson refutes

| Misconception | Why it fails | What to do instead |
|---|---|---|
| "The cheaper rung is the better design." | Cheaper is scoped to one change on one machine. | Say "cheaper for this change" and name the next change. |
| "Lines of code is the cost." | It is one of seven metrics, and the weakest for coupling. | Count files and concepts too. |
| "20 of 20 proves the change is correct." | The traces cover the 8×8 game only. | Call 10×10 and the third piece smoke tests. |
| "The 10×10 board is two constants." | Changing the constants moves the default path and breaks all 20 traces. | Keep the 8×8 path and add a second entry point. |
| "The third piece is free in the interface rung." | It costs two files, a build-file edit and a vtable. | Measure files and text bytes. |
| "Compile time measures design quality." | On 600 lines it is dominated by headers and startup. | Report it, but do not let it decide alone. |
| "If both pass, the change cost the same." | Files, lines and concepts can differ by an order of magnitude. | Fill every row of the table. |
| "I can compare 12 and 15 directly." | Three variables change at once. | Use 12/13 or 14/15, or scope the claim. |
| "A template route is always shorter." | It can add a header, an instantiation and a concept. | Count concepts, not just lines. |
| "A trace failed, so the design is wrong." | It failed because the default path moved. | That is a cost signal; restore the control. |
| "Prediction is a formality." | Calibration is half the deliverable. | Keep the original prediction and report the gap. |
| "I should clean up the rung while I am there." | Extra edits make the two sides incomparable. | Apply the same change only. |
| "A duplicate board is a cheap 10×10 route." | It duplicates the bounds and visited rules. | Change the dimension, not the rules. |
| "The report can be two pages if the table is long." | The constraint is the argument, not the paper. | Move raw output to `EVIDENCE.md`. |

## A verified route, for calibration only

Read this after you have written your prediction. It is one route for `construction-12`, measured
on this repository with `g++`, `-O2`, median of three builds. It is a floor, not the expected
answer; a template route or a different third piece will land elsewhere.

```text
files touched        9  (4 edited code files, 3 new code files, 2 build files)
lines added         77
lines removed        5
new concepts         2  (runtime dimension, second entry point)
traces broken        0 on the first green build
compile time     1.03 s -> 1.28 s
binary text     9,583 -> 10,609 bytes
golden traces       20/20 before and after
```

The route: give `Board` a constructor that takes rows and columns, keep 8×8 as the default, add
`SpareKnightPiece` as the third type, substitute it at the two constructions, and add
`main_ten.cpp` as the second entry point. The 10×10 run starts at `2,3` and `2,10` and ends with
`winner=machine` and `moves=27` for seed 1, which is the same 10×10 game `construction-15`
documents.

The numbers are not the lesson. The gap between your prediction and your observation is.

## Next step

There is no `construction-25`. Track A ends with this report.

What comes next depends on your verdict:

- if the board was the expensive half, the next change is a piece with a different movement rule,
  and the report's next-change table is where you predict it;
- if the piece was the expensive half, the next change is a board size chosen at runtime, and the
  same table applies;
- if the two halves disagreed, you have found the tradeoff the capstone exists to show: the rung
  that is cheap for one change is expensive for the other.

After the capstone, the lenses are `construction-16` (ownership), `construction-18` (functional) and
`construction-20` (hexagonal). Track B, archaeology, starts at `archaeology-00` in the catalog.

See the [lesson catalog](../../../docs/guide/lesson-catalog.md) for the full path.

Keep both scratch copies and the report. The report is the artifact; the copies are the evidence.
