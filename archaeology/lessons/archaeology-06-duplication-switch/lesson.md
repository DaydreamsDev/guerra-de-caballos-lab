# archaeology-06 — Duplication and the eight-case switch

> [English](lesson.md) | [Español](lesson-es.md)

> **Status:** drafted · **Concept:** duplication and extraction · **Prerequisite:** archaeology-04 ·
> **Estimated time:** 60 minutes
> **Reading:** ~19 minutes

## Why this lesson exists

`archaeology-04` diagnosed failures that produce a wrong result. `archaeology-05` diagnosed state
that no function owns. Both defects announce themselves: a corrupted cell, a function that reads six
globals it never received. This lesson is quieter. The program works. The move is legal, the knight
lands on the right square, the board records the move. Nothing crashes and nothing prints the wrong
winner.

The defect is that the same rule is written eight times. In `movcab`, the archived `variant-01` has
an eight-case `switch` whose bodies occupy 455 lines. Strip the comments, the whitespace and the
digits from those bodies, and the token sequence is the same. The only real difference between case
1 and case 8 is two integers: a row delta and a column delta. Four hundred and fifty-five lines
encode sixteen integers.

The original code shows it. At line 301, between case 1 and case 2, the source says:

```text
//los demas casos tienen la misma estructura que esta explicada en el caso 1
```

*The other cases have the same structure as the one explained in case 1.* That comment is the
diagnosis, written by the person who paid for it.

This is a **design** failure, not a correctness failure. The taxonomy in
[`archaeology-track.md`](../../../docs/guide/archaeology-track.md) defines the class: the program
works, but the structure makes change expensive. The price is not paid today. It is paid on the next
rule change, by the next person, eight times.

The product is one **extraction**: replace the repeated case bodies with a table, freeze the old
behavior first, and prove that the new code reproduces it. The proof is a run, not an opinion:
200 comparisons, zero mismatches. The record is the cost: lines removed, cases removed,
table entries kept, test result.

## What you will be able to do

By the end of this lesson you can:

- tell duplicated **text** from duplicated **knowledge**, and say which one this switch is;
- read one `case` body and separate the varying data from the repeated skeleton;
- count what a rule change costs in the switch and in the table, in touched lines;
- capture an oracle before deleting it, so the comparison is not circular;
- run a baseline that keeps both implementations and reports a mismatch count;
- remove the switch in a scratch copy and keep the table;
- explain what 200 comparisons with zero mismatches proves, and what it does not;
- state the isolation boundary: the baseline extracts the delta, not the original's bounds, visited,
  capture, turn or printing logic;
- link the extraction to a Track A rung without confusing it with the polymorphism lesson;
- keep the archive read-only while you work.

The extraction is done when the evidence is reproducible by
someone else.

## The concept: duplication and extraction

### Duplication is repeated knowledge

Two lines that look alike are not automatically duplication. A loop body and a function that happens
to share a `+ 1` are not the same defect. The useful definition is about **knowledge**: duplication
exists when one rule is stored in more than one place, so a change to the rule requires more
than one edit.

There are two common shapes:

| Shape | What it looks like | Why it hurts |
|---|---|---|
| **Textual clone** | The same lines pasted twice. | A fix in one copy leaves the other wrong. |
| **Knowledge clone** | The same rule expressed with different names, order or syntax. | A reader cannot see that the copies must move together. |

The eight-case switch is a knowledge clone. The bodies are not byte-identical: case 1 uses
`cf-1, cc+2`, case 6 uses `cf+2, cc-1`, and the comments and whitespace differ. But the rule is one
rule: *a knight moves by one of eight deltas, then the same skeleton checks the board, the turn, the
visited cell and the capture, applies the delta, records the move and prints the case number*. The
delta is data. The skeleton is behavior. The switch stores the delta eight times and the skeleton
sixteen times.

### The extraction move

**Extraction** means: name the part that varies, name the part that stays, keep one copy of the part
that stays, and move the variants into data.

In this lesson the varying part is a pair of integers:

| Case | Row delta | Column delta |
|---:|---:|---:|
| 1 | -1 | +2 |
| 2 | -2 | +1 |
| 3 | -2 | -1 |
| 4 | -1 | -2 |
| 5 | +1 | -2 |
| 6 | +2 | -1 |
| 7 | +2 | +1 |
| 8 | +1 | +2 |

Sixteen integers. That is the entire information content that changes between the eight cases.

The part that stays is the skeleton: bounds, conversion, turn, visited, capture, apply, record,
print. In the original it is copied twice per case, once for `cab==1` and once for the other knight,
so the apply block alone exists sixteen times.

The extraction does not make the program shorter for its own sake. It makes the **change test**
cheaper. A design lesson is judged by a change, not by a shape.

### The change test

Imagine the rules change: the knight also moves like a camel, or one delta is corrected. Count the
places you must touch.

| Change | Switch | Table |
|---|---|---|
| Add a ninth move. | One new `case` body, about 56–61 lines, plus the loop bound. | One new table entry, plus the loop bound. |
| Correct one delta. | Find the case, edit the bounds check, the first conversion, the two assignments and the second conversion. Miss one and the move half-applies. | Edit two integers in one row. |
| Rename the printed case label. | Eight `cout` sites (sixteen with both turn branches). | One print, driven by the index. |

The table wins on every row. That is the argument. It is not "the switch is ugly"; it is "the switch
multiplies every edit by eight".

### Why this is a design failure

The program is correct for the inputs it receives. The cost is structural:

- **Change amplification.** A rule change touches eight bodies.
- **Silent divergence.** A fix applied to seven bodies leaves one case behaving differently. The
  compiler cannot see the omission.
- **Review cost.** A two-integer change arrives as a 60-line diff, or as an eight-file-shaped diff
  inside one function. A reviewer cannot see the rule for the copies.
- **Test cost.** To test the rule you either exercise eight cases or compare two implementations.
  The second option is what this lesson builds.
- **Merge cost.** Two people editing different cases conflict in the same function even when their
  changes are independent.

Severity is medium in the track's scale: expensive change, fragile structure, fixed on a schedule.
It is not critical, because nothing here is undefined behavior by itself. The `i <= 64` loop from
`archaeology-04` is critical; this is not that. The catalog names this defect `failure-008`: eight
hand-copied `case` bodies.

### What this switch is not

Three distinctions keep the lesson honest.

1. **It is not the dispatch problem.** A `switch` on a move number is also a missing-polymorphism
   smell (`failure-011`), but this lesson does not replace dispatch with objects. The table still
   dispatches by index. It removes copied arithmetic, not the decision to dispatch. Polymorphism is
   `archaeology-10`.
2. **It is not a performance lesson.** A modern compiler may already turn the switch into a jump
   table. The extraction buys change cost, not cycles. Do not claim a speedup you did not measure.
3. **It is not the whole rule.** The original `movcab` also checks bounds, turn, visited cells and
   capture, prints, and can end the game. The baseline in this lesson isolates the delta
   calculation. It is a bench for one decision, not a reimplementation of `movcab`.

### The legacy constraint

Copying a case eight times was reasonable under the constraints: one translation unit, no tests,
no version control, an old compiler, a deadline. A hand-copied case is visually auditable: you can
compare two blocks side by side. A table requires trusting an index convention (`moveNumber - 1`)
and a data layout. That trust is what a test provides, and the legacy project had no test.

The lesson is not "the code was careless". It is "the structure made the next change expensive,
and a table plus a test removes that cost".

## Before you start

Read these in full:

- [`README.md`](README.md) — the lesson card, the build command and the acceptance criterion.
- [archaeology-04](../archaeology-04-correctness-failures/lesson.md) — the prerequisite: symptom,
  root cause, minimal fix, structural fix.
- [archaeology-05](../archaeology-05-global-state/lesson.md) — recommended: the `movcab` body and
  its globals, which is where this switch lives.
- [`../../../docs/guide/archaeology-track.md`](../../../docs/guide/archaeology-track.md) — §3, the
  failure taxonomy; §5, the severity scale; §9, the Track A mapping.
- [`../../../docs/archive.md`](../../../docs/archive.md) — the pinned archive, tag and commit.

You also need:

- a local clone of the archive at tag `v1.0.0`;
- a C++20 compiler (`c++`, `g++` or `clang++`);
- `bash`;
- a scratch directory outside `original/`.

> **Rule:** the archive is read-only. Copy the file you want to change into a scratch directory;
> never edit `original/`. The baseline in this lesson is also evidence: copy it before you delete
> anything.

## The 60-minute path

| Min | Step | What you do | Evidence |
|---|---|---|---|
| 0–10 | Predict | Estimate the case size, the changing lines and the cost of one edit | Prediction table |
| 10–25 | Inspect | Read the original switch and run the baseline | Anchor notes and `comparisons=200 mismatches=0` |
| 25–40 | Extract | Freeze the oracle, remove the switch, keep the table | Scratch copy and `golden.txt` |
| 40–45 | Compare | Run the extracted version against the frozen trace | 200 comparisons, 0 mismatches |
| 45–55 | Explain | Price the duplication and name the class | Cost table and change test |
| 55–60 | Record | Fill `EVIDENCE.md`; confirm the archive is clean | Portfolio entry |

If counting one case body takes ten minutes, that count is the lesson.

### The engineering check in this lesson

Track B uses the same habit as Track A, with a different verb: the behavior already exists, so you
**freeze** it instead of specifying it from scratch.

| Step | Where it happens | What it produces |
|---|---|---|
| **Predict** | Step 1 | A falsifiable estimate of case size and edit cost. |
| **Inspect** | Step 2 | The original anchors, the delta table and the baseline run. |
| **Extract** | Step 3 | A scratch copy with one table and no switch. |
| **Compare** | Step 4 | 200 comparisons against the frozen oracle, 0 mismatches. |
| **Explain** | Step 5 | The change test and the cost of duplication. |
| **Record** | Step 6 | `EVIDENCE.md` with counts, diff and result. |

## Step 1 — Predict (10 min)

Before you open the source, answer these four questions in writing. The point is to commit to a
number you can be wrong about.

1. How many lines does one `case` body occupy, including its comment and braces?
2. How many lines inside that body actually change from case to case?
3. If you correct one delta, how many lines must move together in one case?
4. What will the table buy, and what will it cost?

A useful prediction is falsifiable. "The switch is repetitive" is not. "Each body is about 50 lines,
two lines carry the delta, and one edit touches four lines in one body" is.

### A worked prediction

Before reading, I would write:

- case size: about 50 lines, because the body has two turn branches and each branch repeats the
  apply block;
- changing lines: two integers, but they appear in four places per branch (bounds, first
  conversion, assignment, second conversion);
- edit cost: one case, about six lines, if I find every occurrence;
- table benefit: one row per move, one lookup, one place to change;
- table cost: an index convention and a missing `default` guard.

Every one of those claims can be checked. That is the standard. If your prediction says "eight
cases", you have not predicted anything yet; the source already told you there are eight.

## Step 2 — Inspect (15 min)

### Read the original switch

Open `original/variant-01-baseline-2008/source/guerra-de-caballos-baseline.cpp` at tag `v1.0.0` and
find `movcab`. The switch starts at line 239. Use this anchor table to navigate; the line numbers
are from the pinned archive.

| Line | What is there |
|---:|---|
| 239 | `switch(vecjugadas[i])` — dispatch on the move number. |
| 241 | `case 1` — delta `(-1, +2)`. |
| 301 | The comment: the other cases have the same structure as case 1. |
| 302 | `case 2` — delta `(-2, +1)`. |
| 359 | `case 3` — delta `(-2, -1)`. |
| 415 | `case 4` — delta `(-1, -2)`. |
| 471 | `case 5` — delta `(+1, -2)`. |
| 527 | `case 6` — delta `(+2, -1)`. |
| 584 | `case 7` — delta `(+2, +1)`. |
| 640 | `case 8` — delta `(+1, +2)`. |
| 696 | `default` — the out-of-range branch. |
| 241–695 | The eight case bodies: 455 lines. |

The other 2008 variants carry the same switch with their own naming:

| Variant | `switch` line | First `case` |
|---|---:|---:|
| `variant-01-baseline-2008` | 239 | 241 |
| `variant-02-records-2008` | 290 | 292 |
| `variant-03-pointers-dev-2008` | 316 | 318 |
| `variant-04-pointers-final-2008` | 330 | 332 |

The pointer variants changed the container, not the duplication. That is worth noticing: a syntax
change did not remove the design problem.

### The skeleton, once

Read case 1 from line 241 to line 295. It contains two nearly identical branches: one for `cab==1`
and one for the other knight. Here is the branch for `cab==1`, with the repeated steps marked:

| Line | Step | Varies? |
|---:|---|---|
| 243 | Guard: this case is the current move and the game is not over. | Only the case number. |
| 245 | Bounds check with the delta. | The delta. |
| 247 | `ccab = conversionmv(cf-1, cc+2)` — target cell. | The delta. |
| 249 | Turn branch: `if (cab==1)`. | No. |
| 252 | Visited check against the board. | No. |
| 254 | Capture check against the other knight. | No. |
| 257 | `ganador=0` on capture. | No. |
| 262 | `cout << " 1."` — print the case number. | The case number. |
| 263 | `i=-1` — stop scanning cases. | No. |
| 265–266 | Apply the delta to `cf` and `cc`. | The delta. |
| 267 | Recompute the target cell. | No. |
| 269 | `vectab[ccab]=jugada` — record the move. | No. |
| 270 | `break`. | No. |

Then lines 274–295 repeat the same block for the other knight. One case body contains the apply
block twice; eight cases contain it sixteen times. The switch is not eight copies of a rule. It is
sixteen copies of a skeleton wrapped around eight copies of a two-integer payload.

### The varying data

The only values that change across cases are the two deltas. Everything else is the same token
sequence after comments, whitespace and digits are stripped. You can check this yourself: copy the
eight bodies, remove comments and blank lines, replace every digit with `N`, and diff them. Cases 1
and 2 become identical; the rest differ only in the signs of the two deltas.

That is the strongest evidence in the lesson. It is also the reason the table is possible: if the
structure is the same, the structure can be written once.

### Run the baseline

From this lesson's directory:

```bash
cd archaeology/lessons/archaeology-06-duplication-switch
./run.sh
```

Expected output:

```text
comparisons=200 mismatches=0
```

The script compiles `code/movement.cpp` with:

```text
-std=c++20 -Wall -Wextra -Werror -O2
```

`-Werror` matters: the baseline is warning-clean under the flags it ships with. If your compiler
rejects it, record the compiler and the diagnostic; do not silence the warning to make the run pass.

### Read the baseline honestly

`code/movement.cpp` is 83 lines. It contains:

| Part | Lines | What it is |
|---|---:|---|
| `Position` | 4–10 | A row and a column, with `operator==`. |
| `Delta` | 12–16 | The two integers that vary. |
| `moveDeltas` | 18–27 | Eight entries: the extracted data. |
| `apply_switch` | 29–52 | The oracle: eight explicit cases. |
| `apply_table` | 54–58 | The extraction: one indexed lookup. |
| `main` | 60–83 | 8 moves × 25 positions = 200 comparisons. |

The 25 positions are the grid from `-2` to `+2` in both coordinates. That grid exercises positive
and negative deltas without needing a board. It does not test board bounds, because the baseline has
no board.

What the baseline does **not** contain: bounds checks, visited checks, capture, turn selection,
printing, termination. It isolates the delta calculation. Do not describe a passing run as proof
that `movcab` was preserved; it is proof that the delta was preserved.

### Check your prediction

Compare the four numbers you wrote in Step 1 with the anchors. The measured case size is 56–61
lines (42–43 non-blank). The track summary calls them "~48-line" bodies; the exact count depends on
whether you count comments and blank lines. Either way, the order of magnitude is the point: one
rule, dozens of lines per copy.

## Step 3 — Extract (15 min)

### Make the scratch copy

Create a scratch directory outside `original/`. `build/` is ignored by this repository and is a good
place:

```bash
mkdir -p archaeology/lessons/archaeology-06-duplication-switch/build/scratch
cd archaeology/lessons/archaeology-06-duplication-switch/build/scratch
cp ../../code/movement.cpp movement.cpp
```

From here on, edit `build/scratch/movement.cpp`. Do not edit `code/movement.cpp`; it is the baseline
that the next learner will run.

### Freeze the oracle before deleting it

This is the step that makes the comparison honest. If you delete `apply_switch` and then compare
`apply_table` with a table you wrote from the same deltas, you have compared the table with itself.
Zero mismatches would prove nothing.

Capture the switch's output first. Replace the body of `main` with this temporary loop, before
removing anything:

```cpp
for (int moveNumber = 1; moveNumber <= 8; moveNumber += 1)
{
    for (int row = -2; row <= 2; row += 1)
    {
        for (int column = -2; column <= 2; column += 1)
        {
            Position result = apply_switch(moveNumber, Position{row, column});
            std::printf("%d %d %d %d %d\n", moveNumber, row, column,
                        result.row, result.column);
        }
    }
}
```

Keep `return 0;` after the loop. Compile and run it, and save stdout as `golden.txt`:

```bash
c++ -std=c++20 -Wall -Wextra -Werror -O2 movement.cpp -o movement-golden
./movement-golden > golden.txt
wc -l golden.txt
```

`wc -l` must print `200`. Each line is one observation: move number, starting row, starting column,
resulting row, resulting column. That file is now the oracle. The switch is no longer needed to
produce it.

### Remove the switch

In the scratch copy:

1. delete the `apply_switch` function (24 lines, lines 29–52 of the baseline);
2. keep `moveDeltas` and `apply_table`;
3. rewrite `main` to read `golden.txt` and compare each line with `apply_table`;
4. keep the output line `comparisons=200 mismatches=0`;
5. keep the exit code: zero only when there are no mismatches.

A compact reader looks like this:

```cpp
#include <fstream>

// inside main
int comparisonCount = 0;
int mismatchCount = 0;
std::ifstream golden{"golden.txt"};
int moveNumber = 0;
int row = 0;
int column = 0;
int expectedRow = 0;
int expectedColumn = 0;
while (golden >> moveNumber >> row >> column >> expectedRow >> expectedColumn)
{
    Position result = apply_table(moveNumber, Position{row, column});
    comparisonCount += 1;
    if (result.row != expectedRow || result.column != expectedColumn)
    {
        mismatchCount += 1;
    }
}
```

After the loop, print `comparisons=%d mismatches=%d` and return zero only when `mismatchCount` is
zero. Compile with the same flags and run from the directory that contains `golden.txt`:

```bash
c++ -std=c++20 -Wall -Wextra -Werror -O2 movement.cpp -o movement
./movement
```

Expected output, unchanged:

```text
comparisons=200 mismatches=0
```

The switch is gone from the scratch copy. The table remains. The test still passes.

### The extraction cost

Record these numbers, not an impression:

| Measure | Value |
|---|---:|
| Lines removed with `apply_switch` | 24 |
| `case` bodies removed | 8 |
| Table entries kept | 8 |
| Lines added for the golden reader | about 20 |
| Comparisons after extraction | 200 |
| Mismatches after extraction | 0 |

In the original, the same extraction is larger: 455 lines of case bodies become one 10-line table
and one lookup. The skeleton — bounds, visited, capture, apply, record — still has to be written
once, outside the table. That is the next size of the fix, and it belongs in Step 5.

### Optional change test

Append a ninth delta to `moveDeltas` and change the loop bound to 9. In the table version that is
one entry and one bound. In the switch version it is a new `case` body of about 56–61 lines plus the
bound. Count both. The ratio is the design argument in one number.

If you do this, regenerate `golden.txt` from the switch first, or keep the ninth move out of the
comparison. Do not compare a nine-move table against an eight-move trace and call the missing line a
mismatch.

## Step 4 — Compare (5 min)

### What the run says

```text
comparisons=200 mismatches=0
```

Two hundred observations, zero differences. For every move 1–8 and every starting position in the
5×5 grid, `apply_table` returns the same `Position` as the frozen `apply_switch`.

### What equality proves

- The eight deltas in `moveDeltas` match the eight deltas hard-coded in `apply_switch`.
- The index convention `moveNumber - 1` maps 1–8 to entries 0–7.
- The table applies both coordinates in the same direction as the switch.
- For these 200 inputs, the extraction is behavior-preserving.

### What equality does not prove

| Claim | Proven? | Why |
|---|---|---|
| The table is equivalent for every `int` move number. | No. | Only 1–8 were tested. |
| The original `movcab` still behaves the same. | No. | The baseline isolates the delta. |
| Bounds, visited, capture and turn logic are preserved. | No. | They are not in the baseline. |
| The extraction handles out-of-range input like the switch. | No. | The switch has `default`; the table does not. |
| The table is faster. | No. | No measurement was made. |
| The table is easier to change. | By the change test, yes. | Count the touched lines; do not claim a benchmark. |

### The oracle direction

The order matters. The switch is the oracle because it is the code that existed. The table is the
candidate because it is the code you wrote. If you generate both from the same table, the comparison
is circular: you are checking that a value equals itself.

This is the transferable rule: **freeze the old behavior before you change the structure**. It is
the same rule as a golden trace in `common-04`, applied to a refactor instead of a whole program.

### When a mismatch appears

Do not "fix" the table until the mismatch is understood. Print the failing observation:

```text
move=6 row=-1 column=2 expected=(1,1) actual=(1,-1)
```

The tuple tells you whether the delta is wrong, the sign is wrong, or the index is off by one. A
mismatch is information about the extraction, not a failure of the lesson.

## Step 5 — Explain (10 min)

### Price the duplication

| Cost | Evidence in the original |
|---|---|
| One rule change touches eight bodies. | Cases at 241, 302, 359, 415, 471, 527, 584, 640. |
| One missed copy diverges silently. | The compiler cannot compare case 3 with case 6. |
| Review sees about 60 lines for a 2-integer change. | A delta appears in the bounds check, the first conversion, the assignments and the second conversion. |
| The same skeleton is copied twice per case. | Lines 249–270 and 274–295 are the two turn branches of case 1. |
| The duplication survives a syntax change. | Variants 02, 03 and 04 keep the switch under different containers. |
| Tests must either enumerate cases or compare implementations. | The baseline chooses comparison, which needs an oracle. |

### The class, not the instance

The instance is "eight cases". The class is "one rule stored as code copies instead of data". Look
for the class elsewhere in the archive:

- the two turn branches inside every case — sixteen copies of the apply block;
- the same switch repeated in four variants — a copy at the file level;
- the parallel arrays and loose knight variables — the next lesson's class (`archaeology-07`);
- the `switch` on move type itself — the dispatch class (`archaeology-10`).

The systemic fix is not "delete these eight cases". It is "store the varying data once, keep one
skeleton, and test the boundary between them". That fix removes the class.

### Two fixes, two sizes

**Minimal fix — the table.** Replace the eight case bodies with one table and one lookup. In the
baseline, the table already exists; your extraction removes the switch. In the original, the same
move would keep `movcab`'s skeleton and replace only the per-case arithmetic with a `Delta` lookup.
This is the smallest change that removes the copied delta. It does not remove the duplicated
skeleton, the globals, the printing or the missing tests.

**Structural fix — one apply function.** Extract the whole move application into one function that
takes a `Delta` and the state it needs:

```cpp
MoveResult applyMove(Position from, Delta delta, const Board &board, int turn);
```

The function checks bounds, checks the visited cell, detects capture, applies the delta, records the
move and returns the outcome. The eight cases disappear; the table stays as data. The two turn
branches collapse into one call. Track A links: `construction-03` (functions and parameters) for the
skeleton, `construction-05` (array of records) for the table, and `construction-19` (data-oriented
design) for the idea that the deltas are data, not code.

`construction-12` (polymorphism) is the fix for dispatch, not for duplication. Do not cite it here
as if it were the same repair; `archaeology-10` owns that argument.

### What the extraction does not fix

- The table trusts `moveNumber - 1`. `apply_table(0, ...)` and `apply_table(9, ...)` are undefined
  behavior. The baseline switch's `default` returned the position unchanged. If you keep the table,
  keep a guard or state the precondition and enforce it at the call site. This is a real regression
  that the 200-comparison test cannot see, because the test never calls out of range.
- The table does not remove the turn branch duplication inside the original `movcab`.
- The table does not remove the globals or the printing.
- The table does not make the game testable end to end. It makes one calculation testable.

### The tradeoff, stated fairly

The table costs an index convention, a range guard and a separation between data and code. The
switch costs eight copies and a change amplifier. In a one-shot program with no tests, the switch is
easier to verify by eye. In a maintained program with a test, the table is cheaper. The lesson asks
you to choose with the change test, not with taste.

## Step 6 — Record (5 min)

Fill [`EVIDENCE.md`](EVIDENCE.md):

- setup and archive pin;
- the prediction table, written before inspection;
- the original anchors and the measured case count;
- the baseline run and its exact output;
- the oracle capture: command, `golden.txt`, line count;
- the extraction diff: what was removed, what was kept, what was added;
- the cost table: lines, cases, entries, comparisons, mismatches;
- the comparison result and the oracle direction;
- the evidence boundary: what the run proves and what it does not;
- the archive integrity check.

A sentence such as "the switch is duplicated" is an opinion. "Eight bodies, 455 lines, 16 integers,
24 lines removed, 200 comparisons, 0 mismatches" is a diagnosis.

## The original in detail

Facts you can cite, with line numbers from `variant-01` at tag `v1.0.0`:

| Location | What is there |
|---:|---|
| 239 | `switch(vecjugadas[i])` — the eight-way dispatch. |
| 241–301 | Case 1: 61 lines, delta `(-1, +2)`. |
| 301 | Comment: the other cases have the same structure as case 1. |
| 302–358 | Case 2: 57 lines, delta `(-2, +1)`. |
| 359–414 | Case 3: 56 lines, delta `(-2, -1)`. |
| 415–470 | Case 4: 56 lines, delta `(-1, -2)`. |
| 471–526 | Case 5: 56 lines, delta `(+1, -2)`. |
| 527–583 | Case 6: 57 lines, delta `(+2, -1)`. |
| 584–639 | Case 7: 56 lines, delta `(+2, +1)`. |
| 640–695 | Case 8: 56 lines, delta `(+1, +2)`. |
| 696–718 | `default`: print the case, advance `i`, detect a trapped knight, end the game. |
| 245 | The bounds check that the baseline omits. |
| 252 | The visited check that the baseline omits. |
| 254 | The capture check that the baseline omits. |
| 249, 274 | The two turn branches that duplicate the apply block. |

The case sizes are total lines including comments, blank lines and braces. Non-blank counts are
42–43. They are measured from one `case` label to the line before the next, so case 1 includes the
comment at line 301. The "~48-line" figure in the track summary is an approximation; cite your own
measurement.

## What the extraction proves and does not prove

| Claim | Supported? | Why |
|---|---|---|
| The table reproduces the switch for 200 inputs. | Yes | The run printed 0 mismatches. |
| The table is equivalent for all inputs. | No | The test covers 8 moves × 25 positions. |
| The original `movcab` behavior is preserved. | No | The baseline isolates the delta. |
| The extraction removes 24 lines and 8 cases from the baseline. | Yes | Count the diff. |
| The extraction reduces the original's 455 case lines to a table. | Yes, by construction | The table has 8 entries and one lookup; the skeleton is separate. |
| The extraction removes dispatch. | No | The table is still indexed dispatch. |
| The extraction removes the skeleton duplication. | No | Only the structural fix does that. |
| The table preserves the `default` behavior. | No | The table has no out-of-range guard. |
| The change test is cheaper with the table. | Yes, by line count | One entry versus one ~60-line case. |
| The table is faster at runtime. | Not measured | Do not claim it. |

This table is the honesty check. A record that says "the refactor is correct" without naming the
sample is overreaching. Say "for these 200 inputs" when that is what you ran.

## Self-check (binary)

- [ ] I wrote the four predictions before opening the source.
- [ ] I found the switch at line 239 and the eight cases at their anchors.
- [ ] I read the comment at line 301 and quoted it.
- [ ] I counted the case bodies: eight, 56–61 lines each, 455 lines total.
- [ ] I named the 16 integers that actually vary.
- [ ] I identified the skeleton steps that do not vary.
- [ ] I ran `./run.sh` and saw `comparisons=200 mismatches=0`.
- [ ] I recorded the compiler and flags.
- [ ] I copied the baseline into a scratch directory before editing.
- [ ] I captured `golden.txt` from `apply_switch` before deleting it.
- [ ] `golden.txt` has 200 lines.
- [ ] I removed `apply_switch` and kept `moveDeltas` and `apply_table`.
- [ ] The extracted version prints `comparisons=200 mismatches=0`.
- [ ] I recorded the extraction cost: 24 lines, 8 cases, 8 entries, 200/0.
- [ ] I explained why comparing the table with itself would be circular.
- [ ] I stated that the baseline does not include bounds, visited, capture or turn logic.
- [ ] I noted that the table loses the switch's `default` guard.
- [ ] I linked the structural fix to `construction-03`, `construction-05` and `construction-19`.
- [ ] I did not claim a performance improvement.
- [ ] The archive is unmodified (`git status` in the archive is clean).

If a box is not ticked, the unticked box names the next step. There is no penalty for repeating one.

## Misconceptions this lesson refutes

| Misconception | Why it is wrong | What to do instead |
|---|---|---|
| "Duplication means identical text." | The cases differ in comments, whitespace and signs, but share one rule. | Compare the knowledge, not the bytes. |
| "The switch is eight rules." | It is one delta rule plus one skeleton, copied. | Separate the varying data from the repeated structure. |
| "A table is always better than a switch." | A table adds an index convention and loses the `default` guard. | Price both with the change test. |
| "Zero mismatches proves the refactor is correct." | It proves equality on 200 sampled inputs, with a frozen oracle. | State the sample and the oracle direction. |
| "The baseline is the original rule." | It isolates the delta; bounds, visited, capture and turn are absent. | Write the isolation boundary in the record. |
| "Delete the old code, then test the new one." | Without a frozen oracle, the comparison is circular. | Capture the golden trace first. |
| "The compiler already optimizes the switch, so the lesson is moot." | The lesson is about change cost, not cycles. | Argue with touched lines, not nanoseconds. |
| "Extraction means fewer lines." | The point is fewer places to change; the skeleton may stay. | Count touched places, not only lines. |
| "The switch and polymorphism are the same problem." | Dispatch and copied arithmetic are different classes. | Fix duplication here; dispatch in `archaeology-10`. |
| "The `default` branch is dead code." | It is the out-of-range contract; the table drops it. | Keep a guard or enforce the precondition. |
| "A passing test means the original is safe to change." | The original has no test and no build in this lab. | Keep the archive read-only; repair in a scratch copy. |

## Terms used here

| Term | One-line meaning |
|---|---|
| Duplication | One rule stored in more than one place. |
| Knowledge clone | The same rule written with different names, order or syntax. |
| Extraction | Moving the varying part into data and keeping one copy of the rest. |
| Delta | The pair of integers added to a position to produce the next position. |
| Table-driven | Selecting behavior by indexing data instead of writing a branch per case. |
| Oracle | The implementation whose behavior you trust while you replace it. |
| Golden trace | A frozen record of outputs used to compare a new implementation. |
| Change test | A concrete change whose touched places measure the design. |
| Dispatch | Choosing which code runs based on a value. |
| Default branch | The `switch` path for values no case matches. |
| Index convention | The rule that maps a move number to a table position, here `n - 1`. |
| Design failure | The program works, but the structure makes change expensive. |
| Evidence boundary | The line between what the run proves and what you infer. |

## What “done” means here

The **Self-check (binary)** above is the authoritative checklist. If every box is ticked, the
lesson is done for you. `meta.yml` mirrors the same criteria for tooling.

## Next step

- Continue to `archaeology-07` — representation leakage and parallel arrays. The board is still an
  `int[64]`; the same "one rule, many copies" habit appears there as parallel data.
- If the switch was easy to remove but the skeleton was not, you have found the boundary between the
  minimal fix and the structural fix. Write one sentence about which one you would ship.
- If you want a harder extraction, do it on `variant-02` or `variant-04` and compare the line
  counts. The container changed; the duplication did not.
- If you want the dispatch lesson, go to `archaeology-10`. Do not conflate the two repairs in your
  record.
- If you disagree with the severity, write the context that changes it. A one-shot demo and a
  maintained game do not price the same duplication.

## References

- [`README.md`](README.md) — the lesson card, the build command and the acceptance criterion.
- [`code/movement.cpp`](code/movement.cpp) — the baseline: switch, table and comparison test.
- [`run.sh`](run.sh) — compiles with `-std=c++20 -Wall -Wextra -Werror -O2` and runs the baseline.
- [`EVIDENCE.md`](EVIDENCE.md) — the portfolio record you fill in.
- [`../../../docs/guide/archaeology-track.md`](../../../docs/guide/archaeology-track.md) — §3, the
  failure taxonomy; §5, the severity scale; §9, the Track A mapping.
- [`../../../docs/guide/lesson-catalog.md`](../../../docs/guide/lesson-catalog.md) — the lesson
  sequence and the `archaeology-06` acceptance criterion.
- [`../../../docs/archive.md`](../../../docs/archive.md) — the pinned archive, tag and commit.
- [archaeology-04](../archaeology-04-correctness-failures/lesson.md) — the prerequisite: symptom,
  root cause, minimal fix, structural fix.
- [archaeology-05](../archaeology-05-global-state/lesson.md) — the `movcab` body and its globals.
- Archive: <https://github.com/DaydreamsDev/DaydreamsDev-guerra-de-caballos-archive>, tag `v1.0.0`,
  commit `284e144e41403afbf0cc44422876425cacfb2bd5`.
