# Non-structured programming (programación no estructurada)

> [English](lesson.md) | [Español](lesson-es.md)

**Concept:** arbitrary control flow · **Time:** about 60 minutes · **Status:** drafted
**Reading:** ~11 minutes

## Why this lesson exists

A program that can jump anywhere has no boundaries, only positions. That sounds like freedom. This
lesson shows what it costs.

You will not write a program. You will read one that already works, price a small rule change with
your hands off the keyboard, and discover that the price is paid in *places*, not in lines.

## What you will be able to do

By the end of this lesson you can explain arbitrary control flow and apply the lesson's change test.

## The concept: a jump names a position, not a boundary

A function names an entry and an exit. A loop names a body and a condition. Both give the reader a
boundary: something that starts here, ends there, and can be understood without the rest of the
file.

A jump names a **position**. It says "continue at line 184". It says nothing about what must be true
when you arrive, what you may assume, or who is allowed to leave. The position is real. The boundary
is a convention that lived only in the programmer's head.

Two consequences follow, and they are the whole lesson:

1. **A position can be entered from anywhere, so its readers are everywhere.** A turn may begin at a
   single label, yet the code that decides to go there sits inside every branch that finishes other
   work. The rule lives at one position; the ways to reach it live at many.
2. **The reader must simulate.** With boundaries you can read a unit in isolation. Without them, you
   trace control from the top, holding every possible destination in working memory at once.

This is why the cost of the next change is high. Not because the lines are ugly, but because the
places that must agree are not collected anywhere.

## Before you start

- `construction-00` — you have the contract and you have seen a transcript.
- A C++20 compiler (`g++` or `clang++`). `cmake` is optional.

You do not need to know C++ well. You need to be able to follow a jump from one label to another.
That is the whole skill this lesson exercises.

### What you need

- The baseline: [`code/non-structured-baseline.cpp`](code/non-structured-baseline.cpp).
- Its build script and build notes: [`run.sh`](run.sh) and [`README.md`](README.md).
- The rules: [`common/spec/game-rules.md`](../../../common/spec/game-rules.md).
- The transcript format:
  [`common/spec/transcript-format.md`](../../../common/spec/transcript-format.md).
- One golden trace: [`common/traces/seed-0001.txt`](../../../common/traces/seed-0001.txt).
- This learner template: [`EVIDENCE.md`](EVIDENCE.md).

Check that the artifacts are present before you begin, from this lesson directory:

```bash
test -f code/non-structured-baseline.cpp && test -f ../../../common/traces/seed-0001.txt
```

If the baseline is missing, the artifact is not in this checkout. Do not invent a jump count and do
not describe a program you have not read. Record the missing file in `EVIDENCE.md` §9 and return
when it is available. A cost measured on an imaginary program is not evidence.

If the baseline is present but its command-line flags differ from the ones below, use its flags and
record the exact invocation. Do not patch it to match this page.

## Step 1 — Inventory the jumps (~15 min)

Read the baseline once, top to bottom, without judging it. Then build an inventory.

List every label you find, with its line number. List every jump, with its line number and its
target. This is mechanical work; do it with the file open, not from memory.

Two commands give you a starting point, not the whole answer:

```bash
grep -nE '^[[:space:]]*[A-Za-z_][A-Za-z0-9_]*:' code/non-structured-baseline.cpp
grep -n 'goto ' code/non-structured-baseline.cpp
```

The pattern also matches `std::` inside declarations, and a jump may share a line with a condition.
Read the file to confirm what the commands report. The inventory is the file, not the grep.

Then answer in `EVIDENCE.md` §2:

1. How many labels exist?
2. How many jumps exist?
3. Which labels can be reached from more than one jump?
4. Which labels can be reached from both a player path and a machine path?
5. Which labels are turn entries — reached when the program is about to draw a move order?

Question 5 is the one that matters. Answer it by following the code to the place where the move
order is drawn, then walking backwards to every position that can arrive there.

## Step 2 — Predict the cost before you measure (~10 min)

The change test for this lesson is one requirement. Read it twice, then write your prediction.

> **R-SKIP — skip turn.** Count turns as they start, whether or not they produce a move. This
> counter is separate from the transcript's `moves=` value. When the counter is a multiple of 5, the
> active knight makes no move: the turn is still announced with its `turn=` and `order=` fields, no
> `try=` line and no `move=` line follow, and play passes to the other knight. A skipped turn does
> not count toward the 200 successful moves.

R-SKIP is deterministic on purpose: it adds no random draw, so it does not disturb the call order in
§12 of the rules. A rule that drew a number would be a different, larger change.

Before you count anything, write in `EVIDENCE.md` §3:

1. How many labels begin a turn?
2. How many jumps must be rewired for R-SKIP to hold everywhere?
3. How many new labels are needed?
4. How many new jumps are needed?
5. How many places decide who plays next?
6. Where is the cheapest single place to put the check — and what can it still not do from there?
7. The turn header prints a number derived from the move count. What number does a skipped turn
   print, and is that acceptable?
8. Confidence, 0–100 %, for question 2.

A prediction of "many" is not a prediction. Commit to a number, even if you expect it to be wrong.
The gap between your number and the counted number is the calibration signal.

### 2.1 The counting rule

A count is only checkable if the rule for counting is fixed before the count. Use this one.

- A **turn entry** is any position the program reaches when it is about to draw a move order.
- An **arrival** is a jump whose target is a turn entry.
- A **rewired jump** is an arrival whose target now performs the R-SKIP check. Count every arrival,
  including the ones that need no text edit: you cannot know that one needs none without reading it.
- A **new label** is a label that does not exist before the change and must exist after it.
- A **new jump** is a jump that does not exist before the change and must exist after it — for
  example the one that leaves the skip path.
- Do **not** count comments, formatting, blank lines, or a plain `return`.

Count jumps and labels separately. They are different units, and merging them hides which one is
expensive.

### 2.2 A worked example of the method

This is a counting demo, not the baseline. Its numbers are not your answer.

```text
turn_player:              <- reached by 2 jumps
    ...
    jump turn_machine
turn_machine:             <- reached by 2 jumps
    ...
    jump turn_player
```

Applying the rule: two turn entries, four arrivals, four rewired jumps, zero new labels — provided
the check is written at both entries. Writing it at only one leaves two arrivals landing on an entry
that never checks, and those two are silently wrong. The count does not change, because you still
had to read all four arrivals to know which entry each one lands on.

The lesson of the demo: the cost is not the size of the check. It is the number of arrivals. The
demo has two entries; a real program may have one, and an entry reached from several places still
counts every arrival.

## Step 3 — Run the baseline (~10 min)

Build it and reproduce a golden trace. This step establishes that the program you are reading is the
game, not a cousin of the game.

```bash
./run.sh --seed 1 --name Learner > /tmp/baseline-0001.txt
diff -u ../../../common/traces/seed-0001.txt /tmp/baseline-0001.txt
```

`run.sh` builds the binary with CMake when it is available, and falls back to `c++ -std=c++20` when
it is not. If both paths fail, compile once by hand and record the exact command:

```bash
mkdir -p build
g++ -std=c++20 -Wall -Wextra -Werror -O2 \
  -o build/non-structured-baseline code/non-structured-baseline.cpp
```

An empty `diff` is the point. The parser, the generator, the move table and the turn loop all agree
with the reference for this seed. The design changed nothing observable. That is the precondition
for measuring a change: you cannot price a change to a program whose current behavior you have not
confirmed.

If `diff` prints differences, stop. The baseline is not a faithful non-structured edition, and every
count you produce would describe a different program. Record the diff in `EVIDENCE.md` §9 and report
it; do not adjust the trace to match the code.

If the build fails, record the compiler output. A baseline that does not build has no behavior to
preserve.

## Step 4 — Compare: measure the cost (~10 min)

Now count. Apply the rule from §2.1 to the file you have, and fill the table in `EVIDENCE.md` §5.

| Item | Predicted | Counted | Difference |
|---|---|---|---|
| Labels that begin a turn | | | |
| Jumps rewired (arrivals at a turn entry) | | | |
| New labels required | | | |
| New jumps required | | | |
| Places that decide who plays next | | | |
| Files touched | 1 | 1 | 0 |

For every counted jump, write the label it targets and the line it sits on. A number without a list
is an opinion. The list is what makes the count reproducible by someone else.

Then answer:

- Which predicted number was closest, and why?
- Which predicted number was furthest, and what did you not see when you wrote it?
- What could the cheapest insertion point not do, and where did that part have to go instead?
- Would the transcript show the difference if the second part went missing? How?

If a number differs, that is the result. Do not revise the prediction; write the difference next to
it.

## Step 5 — Explain the cost (~10 min)

Write the explanation in `EVIDENCE.md` §6. Two parts.

**First, one sentence.** Say why this change is expensive without using the words *bad*, *goto*,
*spaghetti*, or *unreadable*. Those words name a verdict, not a cause. If your sentence needs one of
them, you have described your reaction, not the structure.

**Second, one paragraph.** Answer:

- Which part of R-SKIP has an obvious home, and which part has none?
- The program already contains the behavior "hand the turn to the other knight". Where is it, and
  why can a new path not reuse it as a unit?
- What must a reader hold in working memory to know whether a path reaches the check?
- What does the reader have to do to be sure a change is complete?

A good answer names the mechanism: the requirement splits into a check that has a home and a
transition that does not, so the second half must be copied out of a branch. A weak answer says the
code is messy. Messy is what you see; a one-off transition that now needs a twin is what causes it.

## Step 6 — Acceptance check (~10 min)

The acceptance criterion for this lesson is: **add one skip-turn requirement; count the rewired
jumps and explain the cost. No code fix is required.**

Check your record against it:

- [ ] R-SKIP is restated in my own words and adds no random draw.
- [ ] The counting rule is stated once and applied the same way to every jump.
- [ ] The count is a number, and every counted jump is listed with its target and line.
- [ ] I compared my predicted count with the counted one, including the misses.
- [ ] My one-sentence explanation avoids *bad*, *goto*, *spaghetti* and *unreadable*.
- [ ] My paragraph names the structural cause, not the symptom.
- [ ] I did not need to change the code to reach this conclusion.

Implementing R-SKIP is optional and outside the criterion. If you implement it, say so, and expect
the transcript to change: R-SKIP changes behavior, so it is a new game, not a new design. Measure it
separately and do not compare it against the golden traces.

## Step 7 — Record (~5 min)

Complete `EVIDENCE.md` §1–§8. Keep the inventory, the prediction, the command output, the count
table and the explanation. This file is your record.

If the baseline was missing, fill §9 instead and stop. There is nothing to count yet.

## Self-check (binary)

- [ ] I read the baseline and inventoried its labels and jumps.
- [ ] I wrote the predicted count before counting.
- [ ] I built the baseline and reproduced `seed-0001.txt` with an empty diff.
- [ ] I counted the rewired jumps using the stated rule and listed each one.
- [ ] I compared the prediction with the count and named the misses.
- [ ] I explained the cost in one sentence without the four forbidden words, plus one paragraph.
- [ ] I completed `EVIDENCE.md`.

These are acceptance criteria. If a box is
unchecked, write the next smallest action in `EVIDENCE.md` §8.

## Misconceptions this lesson refutes

| Misconception | Why it fails | What to do instead |
|---|---|---|
| "It works, so the design is fine." | Working behavior says nothing about the cost of the next rule. | Price one change and compare it with the alternative design. |
| "The cost is the number of lines I add." | The check is short; the arrivals are many. | Count places that must agree, not lines that must be typed. |
| "One check at the top covers every path." | Paths that never pass the top are exactly the ones that break. | Enumerate every arrival at a turn entry before editing. |
| "Non-structured means no structure at all." | It has structure: a graph of positions. It lacks boundaries. | Draw the graph and read the boundaries that are missing. |
| "Counting is objective, so no rule is needed." | Two people count different things and both are "right". | Fix the counting rule before counting. |
| "I can predict after I count." | The prediction is the calibration signal; it must come first. | Write the number, then measure. |
| "If the diff is non-empty, the trace is wrong." | The traces are the contract; the implementation is corrected. | Record the diff and stop until the baseline is faithful. |

## Next step

`construction-02` — structured programming. You will apply the same R-SKIP requirement to a version
built from sequence, selection and iteration, and count the same numbers. Bring this count with you.
The comparison between the two counts is the lesson; neither number means anything alone.

See the [lesson catalog](../../../docs/guide/lesson-catalog.md) for the full path.

If the baseline was missing, do not start `construction-02`. The contrast needs both sides.
