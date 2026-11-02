# Structured programming (programación estructurada)

> [English](lesson.md) | [Español](lesson-es.md)

**Concept:** sequence, selection and iteration · **Time:** about 65 minutes ·
**Prerequisites:** construction-01 · **Status:** drafted
**Reading:** ~16 minutes

## Why this lesson exists

The difference is not the number of jumps. It is the number of *destinations*. Sequence says
the next statement is the one below. Selection says both branches rejoin after the block.
Iteration says the body repeats at the same place until the condition changes. Every transfer
names a region the reader can resolve from the inside out, without a map.

This lesson applies one rule to two files that already produce the same transcripts. You will count
two different things: the edits the rule needs, and the edges a reader must inspect to believe the
edits are on every path. The interesting result is which count moves and which does not.

## What you will be able to do

By the end of this lesson you can:

- explain sequence, selection and iteration as boundaries;
- apply the skip-turn change to both shapes and count the localized edits.

## The concept: a region is a name for the next statement

Three constructs do all the naming:

```text
sequence:    a; b; c;
selection:   if (p) { a; } else { b; }   both branches continue after the block
iteration:   while (p) { a; }            one condition, one body, one back-edge
```

The unit of reading is the region, not the line. A sequence names its successor by position in the
block. A selection names a join point. An iteration names a body and a condition, and the end of the
body names the condition again. In every case the reader can answer "where does control go next?"
without leaving the enclosing block.

The forcing syntax in this baseline is the absence of an **arbitrary target**. There is no `goto`
and no label. There is `continue`, which targets the nearest enclosing loop. That is not a
counterexample: `continue` names a boundary the reader can see from the block, and a future edit
cannot make it land on a different line. A strict single-entry/single-exit reading would rewrite
`continue` as a guarded block. This lesson uses the practical definition: a transfer is structured
when its target is an enclosing region, not an arbitrary position. The distinction matters later in
the lesson, so keep it.

Two consequences follow, and they are the whole lesson:

1. **A turn starts in one region.** The body of the game loop is the only place a turn begins. Its
   entry has one predecessor: the loop condition. To prove that every turn passes a check, read
   that one region.
2. **The next turn is the loop's back-edge.** A skip only has to reach the loop boundary. The loop
   then re-enters the body through the same condition it always uses. No new destination is named.

This is what "localized" will mean here: the control-flow change and the argument for its
completeness live in one region with one entry. A declaration outside the loop is state, not flow;
it does not add a path. Localized does not necessarily mean fewer keystrokes. You will measure both
and keep them apart.

## Before you start

- [`construction-01`](../construction-01-non-structured/lesson.md) — you have priced R-SKIP on the
  non-structured baseline and you have its count.
- A C++20 compiler (`g++` or `clang++`). `cmake` is optional.

You do not need to know C++ well. You need to read a loop, an `if`, and the line where a turn
starts. That is the whole skill this lesson exercises.

### What you need

- The structured baseline: [`code/structured-baseline.cpp`](code/structured-baseline.cpp).
- The non-structured baseline:
  [`non-structured-baseline.cpp`][non-structured-baseline].
- Both build scripts and notes: [`run.sh`](run.sh) and [`README.md`](README.md), plus the
  construction-01 equivalents.
- The rules: [`common/spec/game-rules.md`](../../../common/spec/game-rules.md).
- The transcript format:
  [`common/spec/transcript-format.md`](../../../common/spec/transcript-format.md).
- One golden trace: [`common/traces/seed-0001.txt`](../../../common/traces/seed-0001.txt).
- This learner template: [`EVIDENCE.md`](EVIDENCE.md).

Check that the artifacts are present before you begin, from this lesson directory:

```bash
test -f code/structured-baseline.cpp &&
test -f ../construction-01-non-structured/code/non-structured-baseline.cpp &&
test -f ../../../common/traces/seed-0001.txt
```

If either baseline is missing, the comparison has no second side. Do not invent a count and do not
describe a program you have not read. Record the missing file in `EVIDENCE.md` §10 and return when
it is available. A cost measured against an imaginary alternative is not evidence.

Both baselines reproduce all 20 traces in `common/traces/` byte for byte. This lesson uses seed 1
for the detailed check and one extra diff to prove the two files are the same game.

## Step 1 — Inventory both shapes (~10 min)

Read both files once, top to bottom, without judging them. Then build two inventories.

Three commands give you a starting point, not the whole answer:

```bash
grep -nE '^[[:space:]]*[A-Za-z_][A-Za-z0-9_]*:' \
  ../construction-01-non-structured/code/non-structured-baseline.cpp
grep -n 'goto ' ../construction-01-non-structured/code/non-structured-baseline.cpp
grep -nE '\b(while|for|continue|break)\b' code/structured-baseline.cpp
```

The label pattern also matches `std:` inside indented calls, and a jump may share a line with a
condition. Read the files to confirm what the commands report. The inventory is the file, not the
grep.

Then answer in `EVIDENCE.md` §2:

1. Non-structured file: how many labels and how many jumps?
2. Non-structured file: which position begins a turn, and which edges enter it — list every line
   and say whether the edge is a jump or a fall-through.
3. Structured file: which loop begins a turn, what is its condition, and what is the line range of
   the region executed once per turn?
4. Structured file: which edges enter that region? Count the loop's condition-to-body edge once.
5. In each file: where is the active side assigned, and where is `successfulMoveCount` incremented?
6. Which file writes "a turn starts here" as a construct, and which writes it as a position?

Question 4 is the one that matters. Answer it by drawing the edges, not by trusting the indentation.

## Step 2 — Predict the cost before you measure (~10 min)

The change test is one requirement. It is the same requirement you applied in `construction-01`.
Read it twice, then write your prediction.

> **R-SKIP — skip turn.** Count turns as they start, whether or not they produce a move. This
> counter is separate from the transcript's `moves=` value. When the counter is a multiple of 5, the
> active knight makes no move: the turn is still announced with its `turn=` and `order=` fields, no
> `try=` line and no `move=` line follow, and play passes to the other knight. A skipped turn does
> not count toward the 200 successful moves.

R-SKIP is deterministic on purpose: it adds no random draw, so it does not disturb the call order in
§12 of the rules. The skipped turn still draws and prints its order; the skip happens after the
header. A rule that drew a number would be a different, larger change.

Before you count anything, fix the two units.

### 2.1 The counting rule

A count is only checkable if the rule for counting is fixed before the count. Use this one.

- A **turn entry** is where a turn begins. In the non-structured file it is the label
  `gameLoopStart`; in the structured file it is the body of the game loop.
- A **turn-start region** is the code executed once per turn, from the turn entry through the
  statement that prints the turn header. It ends before the first attempt. In the non-structured
  file the terminal check sits inside this region; in the structured file the terminal check is the
  loop condition, which is the entry edge.
- An **incoming edge** is a control-flow edge that enters the turn entry.
  - In a file with labels, count every jump whose target is the entry, plus any fall-through into
    it.
  - In a file with a loop, count the loop's condition-to-body edge once. The first turn and every
    later turn use that same edge; do not multiply by the number of turns.
- An **edit site** is a contiguous source change the requirement forces: a declaration, an
  increment, a format argument, or a guard block with its transition. Count a contiguous block
  once, even when it holds two statements.
- A **new control-flow edge** is an edge that does not exist before the change. A `continue` to an
  existing loop and a `goto` to an existing label each count once. A new label counts separately.
- Do **not** count comments, blank lines, formatting, generated traces, or the number of turns the
  program runs.

`construction-01` counted jumps whose target was the turn entry. This rule also counts a
fall-through into the entry, so the non-structured number can be one higher than the number you
brought. That is the unit, not an error; record both in §1 and §3.

Count edit sites and incoming edges separately. They answer different questions: how much text
changes, and how many paths a reader must inspect to know the text is on every path.

Why the loop edge counts once: the body of a loop has one predecessor, the condition. The condition
may be reached from before the loop and from the end of the body, but both routes enter the body
through the same edge. That is a fact about the control-flow graph, not a convenience.

The region has internal branches, and in the non-structured file the terminal check can leave the
game. A path that leaves there starts no turn. Every path that starts a turn reaches the header, and
a guard placed immediately after the header is on all of them. That is why counting the edges into
the entry is enough.

### 2.2 A worked example of the method

This is a counting demo, not either baseline. Its numbers are not your answer.

```text
structured                       non-structured
while (p) {                      start:
    a;                               a;
}                                    if (q) goto start;
                                     goto start;
```

Both versions put one guard before `a`. The guard is the same size. The structured body has one
incoming edge, the condition. The non-structured entry has three: the fall-through, the conditional
jump, and the unconditional jump. A reader who wants to know that no path misses the guard must
inspect one edge in the first file and three in the second. The edit is equal; the verification is
not.

The lesson of the demo: the cost of a change is the text you type plus the set of paths you must
inspect to know the text is on every path. The second part is what scatters.

Then answer in `EVIDENCE.md` §3:

1. How many edit sites does R-SKIP need in the non-structured file?
2. How many edit sites does it need in the structured file?
3. How many incoming edges does the turn entry have in each file before the change?
4. How many new control-flow edges does the change add in each file?
5. How many new labels does it need in each file?
6. How many files does it touch in each case?
7. How many new language concepts does it require in each case?
8. Which golden line do you expect to differ first, and why?
9. Confidence, 0–100 %, for question 3 in the structured file.
10. One sentence: where do you expect the difference to appear, and where do you expect a tie?

Commit to a number for each file. "The structured one is better" is not a prediction; it does not
say which count moves. The gap between your numbers and the counted numbers is the calibration
signal.

## Step 3 — Run both baselines (~10 min)

Build both and reproduce a golden trace. This step establishes that the two programs you are about
to compare are the game, not two cousins of the game.

From this lesson directory:

```bash
./run.sh --seed 1 --name Learner > /tmp/structured-0001.txt
diff -u ../../../common/traces/seed-0001.txt /tmp/structured-0001.txt
```

Then the non-structured baseline:

```bash
cd ../construction-01-non-structured
./run.sh --seed 1 --name Learner > /tmp/nonstructured-0001.txt
diff -u ../../../common/traces/seed-0001.txt /tmp/nonstructured-0001.txt
```

Finally, compare the two files with each other:

```bash
diff -u /tmp/nonstructured-0001.txt /tmp/structured-0001.txt
```

Three empty diffs. The first two say each file is the game. The third says the two files are the
same game. Without the third, you would be pricing a change against a program that might behave
differently; the comparison would measure the wrong thing.

If `run.sh` cannot build, compile once by hand and record the exact command. For the structured
file:

```bash
mkdir -p build
g++ -std=c++20 -Wall -Wextra -Werror -O2 \
  -o build/structured-baseline code/structured-baseline.cpp
```

For the non-structured file, run the same command from its directory with its own file name.

If a diff is not empty, stop. A baseline is not faithful, and every count you produce would
describe a different program. Record the diff in `EVIDENCE.md` §10 and report it; do not adjust the
trace to match the code.

## Step 4 — Apply R-SKIP to both and measure (~15 min)

Return to this lesson directory first; Step 3 ends in the construction-01 directory. Copy both files
to a scratch directory outside the repository. The tracked baselines are your control; editing them
destroys the comparison.

```bash
mkdir -p /tmp/rskip
cp code/structured-baseline.cpp /tmp/rskip/structured-rskip.cpp
cp ../construction-01-non-structured/code/non-structured-baseline.cpp \
  /tmp/rskip/nonstructured-rskip.cpp
```

Apply the same requirement to both copies. The requirement is the paragraph in Step 2; these bullets
fix the details so the two implementations are comparable:

1. Declare a started-turn counter, initialized to 0, next to the other turn state.
2. Increment it once per turn, after the terminal check and before the order draw.
3. Print `turn=` from that counter, not from `successfulMoveCount + 1`. If you leave the header on
   the move count, a skipped turn prints a number that will be printed again, and the transcript
   stops being a sequence of turns.
4. After the header and before the first attempt, if the counter is a positive multiple of 5: flip
   the active side, record no move, and go to the next turn.
5. Keep the 200-move cap and `moves=` on `successfulMoveCount`. A skipped turn does not count as a
   successful move.

Do not copy the transfer spelling from one file to the other. Each file already has a way to begin
another turn; use the one that file provides. Do not add a new label to the non-structured file
unless you can show the existing one cannot be reused. If you do add one, record why: that is a real
cost, not a mistake.

Build both scratch copies:

```bash
g++ -std=c++20 -Wall -Wextra -Werror -O2 \
  -o /tmp/rskip/structured /tmp/rskip/structured-rskip.cpp
g++ -std=c++20 -Wall -Wextra -Werror -O2 \
  -o /tmp/rskip/nonstructured /tmp/rskip/nonstructured-rskip.cpp
```

Run both and compare:

```bash
/tmp/rskip/structured --seed 1 --name Learner > /tmp/rskip/structured-0001.txt
/tmp/rskip/nonstructured --seed 1 --name Learner > /tmp/rskip/nonstructured-0001.txt
diff -u /tmp/rskip/nonstructured-0001.txt /tmp/rskip/structured-0001.txt
```

The first diff must be empty: two correct implementations of the same rule are the same new game.
If it is not empty, the implementations disagree; find the first differing line before counting
anything.

Now compare each modified transcript with the golden trace. Both must differ, and that is expected:
R-SKIP changes behavior, so it is a new game, not a regression. For seed 1, the first divergence
should be the fifth turn: the `turn=5` header is followed by the `turn=6` header instead of a
`try=` line. If your first divergence is elsewhere, check the counter and the guard placement
before trusting the rest of the run.

Record how many of the 20 golden traces the modified versions no longer match. The number should be
the same in both; it measures the contract change, not a regression. The README's loop over
`common/traces/seed-*.txt` works here too; point it at the scratch binary.

```bash
diff -u ../../../common/traces/seed-0001.txt /tmp/rskip/structured-0001.txt | head
diff -u ../../../common/traces/seed-0001.txt /tmp/rskip/nonstructured-0001.txt | head
```

Then count. Mark every edit site in each scratch copy and fill `EVIDENCE.md` §6. List every incoming
edge in the unmodified file with its line and its kind. A number without a list is an opinion; the
list is what makes the count reproducible.

Answer the comparison questions in §6.4:

- Did the structured version need fewer edit sites? If not, what did it localize?
- Which count moved between the two shapes, and which stayed equal?
- Which incoming edge in the non-structured file was easiest to miss?
- If a fifth `goto gameLoopStart` were added, would the guard cover it? What would you read to know?
- If a second loop that also starts a turn were added to the structured file, would the guard cover
  it? What would you read to know?
- What does the structured file make visible at the point of the change that the non-structured file
  leaves implicit?

If a number differs from your prediction, that is the result. Do not revise the prediction; write
the difference next to it.

## Step 5 — Explain the difference (~10 min)

Write the explanation in `EVIDENCE.md` §7. Two parts.

**First, one sentence.** Say why the change is localized in one file and scattered in the other
without using *bad*, *clean*, *readable*, *elegant*, *ugly*, *spaghetti*, *unreadable* or *goto*.
Those words name a verdict or a keyword, not a cause. If your sentence needs one of them, you have
described your reaction, not the control flow.

**Second, one paragraph.** Answer:

- Where does a turn start in each file, as a control-flow fact?
- What does the skip edge target in each file: a region boundary or a position?
- How many edges enter the turn entry in each file, and why does the loop edge count once?
- If the edit counts tie, why can the verification costs still differ? If they do not tie, what did
  the extra site buy?
- What must a reader hold in working memory in each case?
- What would make a future change incomplete in each file, and how would a reader notice?

A strong answer separates *edit cost* (the places you type) from *verification cost* (the edges you
read). It names the predecessor count and the kind of target. A weak answer says one file is
cleaner. Clean is what you see; a body with one predecessor is what causes it.

## Step 6 — Acceptance check (~5 min)

The acceptance criterion for this lesson is: **apply one R-SKIP requirement to both baselines; show
that the structured version localizes the change; explain the difference by control flow, not by
style.**

Check your record against it:

- [ ] I ran both unmodified baselines and got three empty diffs.
- [ ] I applied the same R-SKIP specification to both on scratch copies.
- [ ] The two modified versions produce identical transcripts for seed 1.
- [ ] I listed every edit site in both files with its line.
- [ ] I counted incoming edges to the turn entry with the stated rule.
- [ ] I can name which count ties and which does not.
- [ ] I can state what the structured version localizes and what it does not.
- [ ] My one-sentence explanation avoids the eight forbidden words.
- [ ] I did not edit the tracked baselines.
- [ ] I completed `EVIDENCE.md` §1–§9 and the self-check in §11.

The criterion is not "the structured version wins". It is "you can say exactly what it localizes,
by how much, and at what cost". A tie in edit sites is a valid result; it is not a failure to find a
difference.

## Step 7 — Record (~5 min)

Complete `EVIDENCE.md` §1–§9. Keep the inventory, the prediction, the three baseline diffs, the
scratch implementation, the cost table, the edge list and the explanation. This file is your record.

If either baseline was missing or unfaithful, fill §10 instead and stop. There is nothing to
compare yet.

## Self-check (binary)

- [ ] I read both baselines and inventoried their control-flow shapes.
- [ ] I wrote the predicted edit sites and incoming edges before counting.
- [ ] I built both baselines and got three empty diffs.
- [ ] I applied the same R-SKIP requirement to both scratch copies.
- [ ] The two modified versions agreed with each other, and both differed from the golden trace.
- [ ] I listed every edit site and every incoming edge.
- [ ] I compared the prediction with the counts and named the misses.
- [ ] I explained the difference in one sentence plus one paragraph, by control flow.
- [ ] I completed `EVIDENCE.md`.

These are acceptance criteria. If a box is
unchecked, write the next smallest action in `EVIDENCE.md` §9.

## Misconceptions this lesson refutes

| Misconception | Why it fails | What to do instead |
|---|---|---|
| "Structured means no jumps at all." | `continue` and `break` transfer to an enclosing boundary. The concept forbids arbitrary targets, not transfers. | Ask what the transfer names: a region or a position. |
| "Fewer lines means more structured." | The R-SKIP edit can be the same size in both files. | Count incoming edges, not lines. |
| "The structured version has one entry, so every future turn is covered." | A second loop is a second turn-start region; the guard must be repeated or extracted. | Count regions, not files. |
| "The non-structured version is wrong because it uses `goto`." | It produces the same 20 traces. The cost is in the next change, not in current correctness. | Compare the change, not the keyword. |
| "The fourth incoming edge is obvious." | Three jumps and a fall-through are easy to collapse into "the loop". | List every edge with its line. |
| "A tie in edit sites means the designs are equivalent." | Edit count measures typing; audit count measures what a reader must prove. | Report both counts separately. |
| "I can predict after I measure." | The prediction is the calibration signal; it must come first. | Write the numbers, then measure. |
| "If the modified trace differs from golden, I broke the baseline." | R-SKIP changes behavior by design. | Check modified against modified; keep golden for the unmodified baselines. |

## Next step

`construction-03` — functions and parameters. The next rung gives the region a name and a parameter
list. You will extract one turn into a function and count what the signature must carry. Bring the
two counts from this lesson: the edit count and the incoming-edge count.

See the [lesson catalog](../../../docs/guide/lesson-catalog.md) for the full path.

If either baseline was missing, do not start `construction-03`. The next contrast needs both sides.

[non-structured-baseline]: ../construction-01-non-structured/code/non-structured-baseline.cpp
