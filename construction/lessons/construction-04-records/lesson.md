# Records (registros)

> [English](lesson.md) | [Español](lesson-es.md)

**Concept:** aggregation (agregación) · **Time:** about 55 minutes · **Status:** drafted ·
**Prerequisites:** construction-03
**Reading:** ~16 minutes
**Lesson role:** core

## Why this lesson exists

`construction-03` ended with a bill it did not pay. `tryMove` produces three results — the outcome,
the destination row, the destination column — and a function returns one. Two of them travel as
references. The obvious next move is to group those three into one value.

That is not the lesson. The lesson is one step earlier. The two knights were never three results.
They were two things — a player knight and a machine knight — each with a row and a column, and the
code kept four numbers in four separate names. Grouping what already belongs together is
**aggregation**.

The sentence to keep is shorter and less comfortable than the one from `construction-03`:

> A record makes a field addition free by making the parameter list less precise.

Both halves are measurable. The first half is a count: zero signatures change. The second half is a
reading: a parameter that says `const Knight &` no longer says which fields the body touches. You
will measure the first and argue the second, against the same 20 traces.

## What you will be able to do

By the end of this lesson you can:

- explain aggregation and what a record keeps together;
- add a captured field and list the signatures that changed.

## The concept: a record is a name for fields that move together

Five forms of name do the work in this file:

```text
separate    int playerRow; int playerColumn;    two names, two lifetimes to keep in step
record      struct Knight { int row; int column; };
member      playerKnight.row                    one object, one lifetime, fields inside it
reference   Knight &activeKnight                one name for the caller's object
const ref   const Knight &otherKnight           read-only access to the caller's object
```

In `construction-03`, a function that needs a knight needs both of its numbers separately.
`playTurn` receives `playerRow, playerColumn, machineRow, machineColumn` as four reference
parameters. In `construction-04` it receives `Knight &playerKnight, Knight &machineKnight`.

Across the 19 function definitions, the parameter totals are 75 and 63. The seven functions that
differ:

| Function | `construction-03` parameters | `construction-04` parameters |
|---|---|---|
| `initializePositions` | 4 | 2 |
| `printGameHeader` | 9 | 7 |
| `printTurnHeader` | 5 | 4 |
| `printMove` | 7 | 6 |
| `tryMove` | 8 | 6 |
| `playTurn` | 8 | 6 |
| `playGame` | 8 | 6 |
| **All 19 definitions** | **75** | **63** |

Twelve parameters disappear. Nothing about the game changed to make that possible: the same two
objects were always there, and the four names were a choice.

Now read the two `tryMove` lines together.

```cpp
MoveOutcome tryMove(int activeKnightRow, int activeKnightColumn, int otherKnightRow,
                    int otherKnightColumn, int moveNumber, bool visitedCells[...],
                    int &destinationRow, int &destinationColumn);
```

```cpp
MoveOutcome tryMove(const Knight &activeKnight, const Knight &otherKnight, int moveNumber,
                    bool visitedCells[...], int &destinationRow, int &destinationColumn);
```

The first names four integers and nothing else. A reader knows which numbers the body may read
without opening it. The second names two objects, and the body may read any field either one has now
or gains later. The second is shorter and it says less. That trade is the lesson.

Three consequences follow:

1. **A new field does not reach the signature.** Every function that already receives a `Knight`
   can use the field without being recompiled differently. Adding `captured` costs one line in the
   type and one assignment where the fact happens.
2. **A record has one default, and it covers fields added later.** `Knight playerKnight = {};`
   zero-initializes every member, including one that does not exist yet when the line is written.
   Four parallel integers have no shared default: a fifth one is a fifth declaration to remember.
3. **A new field's use still reaches every function that reads it, and no signature lists them.**
   That is the second half of the sentence. `grep` is now the only way to answer "who reads
   `captured`?", and the answer is a text search rather than a parameter list.

What the record is **not**, in this lesson:

- **Not encapsulation.** `.row`, `.column` and `.captured` are reachable from every function that
  holds a `Knight`. `playerKnight.row = 0;` compiles. Nothing here can prevent it.
- **Not representation independence.** The field names appear in 14 places across six functions.
  Replacing `row` and `column` with one cell index would rewrite all 14. `construction-09` prices
  that change.
- **Not a value with behavior.** `Knight` has no functions of its own, no equality, no printing, no
  invariant. It is a shape for data, not a guardian of it. `construction-10` and `construction-11`
  add the guardian.

## Before you start

- [`construction-03`](../construction-03-functions/lesson.md) — you have its two counts: edit sites
  and callers changed by R-SKIP, and you know which function reads no global.
- A C++20 compiler (`g++` or `clang++`). `cmake` is optional.

You do not need to know C++ well. You need to read a type definition, a member access (`knight.row`)
and a reference parameter. That is the whole skill this lesson exercises.

### What you need

- The records baseline: [`code/records-baseline.cpp`](code/records-baseline.cpp).
- The functions baseline: [`functions-baseline.cpp`][functions-baseline].
- Build scripts and notes: [`run.sh`](run.sh) and [`README.md`](README.md).
- The rules: [`common/spec/game-rules.md`](../../../common/spec/game-rules.md) — §12 fixes the
  order of random draws.
- The transcript format:
  [`common/spec/transcript-format.md`](../../../common/spec/transcript-format.md).
- One golden trace: [`common/traces/seed-0001.txt`](../../../common/traces/seed-0001.txt).
- This learner template: [`EVIDENCE.md`](EVIDENCE.md).

Check that the artifacts are present before you begin, from this lesson directory:

```bash
test -f code/records-baseline.cpp &&
test -f ../construction-03-functions/code/functions-baseline.cpp &&
test -f ../../../common/traces/seed-0001.txt
```

If either baseline is missing, the comparison has no second side. Record the missing file in
`EVIDENCE.md` §10 and return when it is available. A count against a file you have not read is not
evidence.

Both baselines reproduce all 20 traces in `common/traces/` byte for byte, before you change
anything. Same game, two representations of one fact. That is what makes the comparison fair.

## Step 1 — Read the record and its callers (~10 min)

Read both files once. Then let the text tell you where the type appears:

```bash
grep -n 'Knight' code/records-baseline.cpp
grep -n '^\(void\|int\|bool\|MoveOutcome\|std::uint64_t\) [a-zA-Z]*(' \
  code/records-baseline.cpp
```

The first command prints the definition, every parameter list that names the type, and every field
access in a body. The second prints 19 definitions. Fill this table into `EVIDENCE.md` §2.1 while
you read.

| Function | Line | Receives | Fields it reads | Fields it writes |
|---|---|---|---|---|
| `initializePositions` | 120 | `Knight &` ×2 | — | `row`, `column` |
| `printGameHeader` | 144 | `const Knight &` ×2 | `row`, `column` | — |
| `printTurnHeader` | 166 | `const Knight &` | `row`, `column` | — |
| `printMove` | 201 | `const Knight &` | `row`, `column` | — |
| `tryMove` | 213 | `const Knight &` ×2 | `row`, `column` | — |
| `playTurn` | 236 | `Knight &` ×2 | through the calls it makes | `row`, `column` |
| `playGame` | 305 | `Knight &` ×2 | — | — |
| `main` | 323 | — | — | declares both knights |

Two rows are worth a minute.

- `playGame` receives two knights and reads no field of either. It is a pass-through. In
  `construction-03` the same pass-through is four integers wide; here it is two objects wide. The
  function's job did not change, and neither did its ignorance of the fields.
- `playTurn` writes `row` and `column` for the **active** knight, and it does so through
  `activeKnight`, a reference chosen from `playerIsActive` at line 246. In `construction-03` the
  same write needs an `if` with two branches, because there is no reference to choose. Keep that
  observation; Step 5 pays for it.

Then answer in `EVIDENCE.md` §2:

1. How many functions receive a `Knight`? How many receive it as `const Knight &`?
2. Which function receives knights and reads no field of either?
3. How many parameters would `tryMove` have today if each `Knight` were expanded into its fields?
4. What does `const Knight &` prevent? What does it not prevent?
5. Where is a knight's `row` first set? Where is it last set?
6. If `Knight` gained a third field, which of the 19 definitions would fail to compile? (Check
   before you answer; the number is the point.)
7. Which two names at namespace scope are data, and which is a type? Confirm with `nm` if you
   want: `g++ -std=c++20 -c -o /tmp/rec.o code/records-baseline.cpp` then
   `nm -C --defined-only -g /tmp/rec.o | grep ' [BbDd] '`.

Question 6 has the answer you will measure in Step 4, and question 4 has the answer you will argue
in Step 6.

## Step 2 — Predict before you measure (~8 min)

Three predictions, all written before any run: one about the record, one about the replacement, one
about the placement of the replacement.

### 2.1 The change under test

The requirement, fixed for both files:

> **R-CAPTURED — record the capture.** A knight that is captured is marked. The mark is set when the
> capture happens, it starts false for both knights, and no function reads it. The game's output
> does not change.

The clause that says nothing reads the mark is the one that keeps the change honest. The trace check
can prove the game did not change; it cannot prove the field holds the right value, because nothing
reads it. Step 4 shows why that distinction matters.

For the records file, predict before editing:

- Signatures that change:
- Call sites that change:
- Functions whose body changes:
- Lines added:
- Traces that differ, of 20:
- Confidence, 0–100 %:

The argument you are testing: `playTurn` already holds `Knight &otherKnight` at line 247. The fact
you need to record is about `otherKnight`. Nothing has to be passed that is not already there.

### 2.2 The same fact without a record

`construction-03` has no `Knight`. To record the same fact you need two new boolean names, one per
knight, and they must be reachable where the capture happens — inside `playTurn`. Predict the cost
for **both** placements. Fix the units first; they are the same five as in `construction-03`.

- An **edit site** is a contiguous source change the requirement forces: a declaration, an
  assignment, a format argument, a signature line, or a guard block with its transition.
- A **signature change** is a function whose parameter list or return type changes. Count it once,
  separate from the call sites it forces.
- A **call site** is a place where a call expression must be edited because a signature changed.
- A **caller** is a function that contains a call site. One caller can hold several call sites.
- A **new global** is a mutable name added at namespace scope.
- Do **not** count comments, blank lines, formatting, generated traces, or the number of turns.

| Metric | Record (`captured`) | Flags local to `playGame` | Flags local to `main` |
|---|---|---|---|
| Edit sites | | | |
| Signature changes | | | |
| Call sites changed | | | |
| Callers of `playTurn` changed | | | |
| Callers of `playGame` changed | | | |
| New globals | | | |
| Lines added | | | |
| Golden traces broken (of 20) | | | |

Commit to a number in every cell. "The record version is cleaner" is not a prediction; it does not
say which row moves.

### 2.3 Which placement is the fair comparison

The two flag placements are not equally fair, and the difference is the lesson. Answer now, before
measuring:

- Which scope holds the fact in the record version? Name the variable.
- In the flag version, which scope owns the fact? `playGame`'s body, or `main`'s?
- If the end of `playGame` printed which knight was captured, which placement would still compile?

Write the answer in `EVIDENCE.md` §3.3. You will check it against the counts in Step 5.

## Step 3 — Run both baselines (~5 min)

Create the scratch directory first; every command below writes into it.

```bash
mkdir -p /tmp/records-test
./run.sh --seed 1 --name Learner > /tmp/records-test/records-0001.txt
diff -u ../../../common/traces/seed-0001.txt /tmp/records-test/records-0001.txt
```

Then the functions baseline:

```bash
cd ../construction-03-functions
./run.sh --seed 1 --name Learner > /tmp/records-test/functions-0001.txt
diff -u ../../../common/traces/seed-0001.txt /tmp/records-test/functions-0001.txt
```

Then the two against each other:

```bash
diff -u /tmp/records-test/functions-0001.txt /tmp/records-test/records-0001.txt
```

Three empty diffs. Return to this lesson directory and confirm all 20 seeds, not only the first:

```bash
mkdir -p /tmp/records-test
for trace in ../../../common/traces/seed-*.txt; do
  traceName="$(basename "${trace}" .txt)"
  seedText="${traceName#seed-}"
  seedNumber="$((10#${seedText}))"
  ./run.sh --seed "${seedNumber}" --name Learner | cmp - "${trace}"
done
```

A silent loop is 20 matches. Return to this lesson directory if you changed it. If `run.sh` cannot
build, compile once by hand:

```bash
mkdir -p build
g++ -std=c++20 -Wall -Wextra -Werror -O2 \
  -o build/records-baseline code/records-baseline.cpp
```

If a diff is not empty, stop. Record it in `EVIDENCE.md` §10. Do not adjust a trace to match the
code.

## Step 4 — Add the field to the record (~10 min)

Copy the baseline. The tracked file stays untouched.

```bash
mkdir -p /tmp/records-test
cp code/records-baseline.cpp /tmp/records-test/records-captured.cpp
```

Three edits. The first is the type:

```cpp
struct Knight
{
    int row;
    int column;
    bool captured;
};
```

The second is the initial state, after the two declarations in `main`:

```cpp
    Knight playerKnight = {};
    Knight machineKnight = {};
    playerKnight.captured = false;
    machineKnight.captured = false;
```

The third is where the fact happens — the capture branch of `playTurn`, before the move is counted:

```cpp
            otherKnight.captured = true;
```

Build and run all 20 seeds:

```bash
g++ -std=c++20 -Wall -Wextra -Werror -O2 \
  -o /tmp/records-test/records-captured /tmp/records-test/records-captured.cpp
for trace in ../../../common/traces/seed-*.txt; do
  traceName="$(basename "${trace}" .txt)"
  seedText="${traceName#seed-}"
  seedNumber="$((10#${seedText}))"
  /tmp/records-test/records-captured --seed "${seedNumber}" --name Learner | cmp - "${trace}"
done
```

20 matches, no output. The diff against the tracked file is four added lines and zero removed:

```bash
diff -u code/records-baseline.cpp /tmp/records-test/records-captured.cpp
```

Four observations, each one part of the answer.

**One — no signature moved.** Compare the two `tryMove` and `playTurn` parameter lists; they are
identical. The compiler had nothing to check because nothing was restated. Confirm with the edit
list, not with the absence of errors:

```bash
grep -n 'captured' /tmp/records-test/records-captured.cpp
```

Three sites: line 25 in the type, line 279 in `playTurn`, lines 338–339 in `main`.

**Two — the trace cannot see the field.** Change the two initializers to `true` and rebuild. All 20
traces still pass, because nothing reads the field. The trace checks the game; it does not check the
fact. Only the assignment site can be checked, and only by reading it. Write that down: a golden
trace is a behavior contract, not a data contract.

**Three — the default was already correct.** Set the two lines back to `false`, then delete them and
rebuild. All 20 traces still pass. `Knight playerKnight = {};` zero-initializes every member,
including the one you just added. The two explicit lines are documentation for a reader, not a
requirement of the program. That is worth knowing and worth saying out loud: with four parallel
integers, forgetting the fourth initializer is a real bug; with a record, the aggregate initializer
covers it.

**Four — no new name at namespace scope.** The type is at namespace scope; it is not data.

```bash
g++ -std=c++20 -c -o /tmp/records-test/rec.o /tmp/records-test/records-captured.cpp
nm -C --defined-only -g /tmp/records-test/rec.o | grep ' [BbDd] '
```

Still one line, `randomState`. The `Knight` type added a name the compiler tracks and the linker
does not export as data. Two integers became one object and the count of mutable globals did not
move.

Optional, if layout interests you: `sizeof(Knight)` goes from 8 to 12, with `captured` at offset 8.
The compiler pads the record to a multiple of its alignment. Nothing in the transcript depends on
that, and nothing in the record depends on the padding.

Write `EVIDENCE.md` §5: the exact commands, the diff, which of your §2.1 predictions held, and which
one missed. The usual miss is "the initializers": learners predict zero edit sites and forget that
a field with a meaningful initial value usually wants a line that shows it.

## Step 5 — Add the same fact without a record (~12 min)

Now the other side. Copy the functions baseline to a second scratch file:

```bash
cp ../construction-03-functions/code/functions-baseline.cpp \
  /tmp/records-test/functions-captured.cpp
```

The fact still has to be reachable inside `playTurn`'s capture branch. There are two places to put
it, and the difference between them is the difference the record removes.

| Where the flags live | Who owns the fact | Signatures forced to change |
|---|---|---|
| local in `playGame`, passed to `playTurn` | the loop | `playTurn` only |
| local in `main`, passed through `playGame` | the game | `playTurn` and `playGame` |

Apply the second one first, because it is the honest comparison: the knight's lifetime is the game's
lifetime, and `main` is where the knights live.

1. In `main`, next to the four coordinate declarations:
   `bool playerCaptured = false; bool machineCaptured = false;`
2. `playTurn` gains `bool &playerCaptured, bool &machineCaptured`.
3. `playGame` gains the same two parameters and forwards them.
4. The capture branch sets the right one. `playTurn` holds `otherKnightRow` and `otherKnightColumn`
   as **values**, not as a reference to a knight, so the branch must choose:

   ```cpp
   if (playerIsActive)
   {
       machineCaptured = true;
   }
   else
   {
       playerCaptured = true;
   }
   ```

5. The call in `main` passes them.

Build and run all 20 seeds as in Step 4. They pass. The game did not change; only the shape of the
fact changed. Then count what the change reached:

| Metric | Record | Flags in `main` |
|---|---|---|
| Edit sites | 3 | 6 |
| Signature changes | 0 | 2 |
| Call sites changed | 0 | 2 |
| Callers changed | 0 | 2 (`playTurn`, `playGame`) |
| New globals | 0 | 0 |
| Lines added / removed | 4 / 0 | 20 / 7 |
| Golden traces broken (of 20) | 0 | 0 |
| Functions that can read the fact | any that holds a knight | only those that list it |

The record's edit-site count is 3 with the two explicit initializer lines and 2 without them. Keep
the number that matches the file you built, and say which one it is.

Now the cheaper placement. Move the two declarations from `main` into `playGame`, drop the two
parameters from `playGame`'s signature and from `main`'s call, keep them on `playTurn`. Rebuild, run
20 seeds. They pass. The count drops to 4 edit sites, 1 signature change, 1 call site, 1 caller,
15 lines added and 3 removed.

That is cheaper than the record on one row — lines changed — and it is not the same change. Answer
these in `EVIDENCE.md` §6.6:

- Why does the narrow placement compile? Name the exact condition it relies on.
- What happens to the narrow placement if the end of `playGame` prints which knight was captured?
- What does the record version do in that case? (Try it: add the print to the captured copy.)
- In `construction-03`, what is the smallest edit that makes the two flags belong to the knights
  rather than to the loop?

The last question has no cheap answer, and that is the result. With separate variables there is no
way to say "this fact belongs to that knight". You can choose a scope and thread names through it;
the attachment is a convention held in a comment or in a reader's head. With a record the
attachment is in the type: `otherKnight.captured` is a field of the object the capture is about.

Note also what the record did to the conditional. Four lines of `if`/`else` became one assignment,
because `otherKnight` is already a reference to the right knight. The functions version had to
re-select the loser from `playerIsActive`, a fact it already knew and had already used at line 241
to pick `otherKnightName`. The record let it reuse the choice instead of repeating it.

## Step 6 — Compare and explain (~8 min)

Write `EVIDENCE.md` §7. Two parts.

**First, one sentence.** Say what the record decides about a field addition, without using the word
*struct* and without a verdict word: *better*, *worse*, *clean*, *cleaner*, *nice*, *elegant*,
*ugly*, *modular*. If your sentence needs one of those, you have described your reaction to the file
rather than the count.

**Second, one paragraph.** Answer:

- Which names the new field had to reach in each version: the type, the initializer, the assignment,
  and whatever parameters carry it.
- What the shorter parameter list stopped saying. Give one function where you lost information and
  one where you lost nothing.
- What the record localizes: the definition of a knight's fields, the default for all of them, and
  the attachment of a fact to the object it describes.
- What the record does **not** localize: the reads, the invariants, and the representation.
- What a reader must hold in working memory to know which fields a function uses, in each file.
- What a golden trace can and cannot prove about this change.
- Which later lesson fixes each remaining gap. There are three: representation (`construction-09`),
  invariants (`construction-10`, `construction-11`), and access (`construction-11`).

A strong answer keeps two things apart: **the number of edits a field addition costs** and **the
precision of the parameter list**. The record wins the first and loses the second. A weak answer
says the record version is cleaner, which is a verdict that hides both numbers.

## Step 7 — Acceptance check (~2 min)

The acceptance criterion for this lesson is: **`captured` is added to the record; you can show which
call sites changed and which did not; and you can explain what the record localizes.**

Check your record against it:

- [ ] I ran both unmodified baselines and got three empty diffs, plus 20 matching seeds.
- [ ] I inventoried the seven functions that receive a `Knight` and the fields each one touches.
- [ ] I predicted the signature count for the record before editing.
- [ ] I added `captured` to the record and all 20 traces still passed.
- [ ] I showed that the two `= false` lines are optional because `= {}` covers them.
- [ ] I showed that the trace cannot detect a wrong default in an unread field.
- [ ] I applied the same fact to the functions baseline with two separate flags.
- [ ] I counted signature changes, call sites and callers for both placements.
- [ ] I can name the one row where the narrow placement beats the record, and why it is not the same
      change.
- [ ] My one-sentence explanation avoids *struct* and the eight verdict words.
- [ ] I did not edit either tracked baseline.
- [ ] I completed `EVIDENCE.md` §1–§9 and the self-check in §11.

The criterion is not "records win". It is "you can say exactly what a field addition reached, and
what the shorter parameter list stopped telling you".

## Self-check (binary)

- [ ] I read both baselines and inventoried which function touches which field.
- [ ] I predicted the record's cost and both flag placements before measuring.
- [ ] I built both baselines and reproduced all 20 traces.
- [ ] I added `captured` to a scratch copy of the record and kept the traces passing.
- [ ] I applied the same fact to the functions baseline and counted both placements.
- [ ] I compared the prediction with the counts and named the misses.
- [ ] I explained what the record localizes and what it does not.
- [ ] I completed `EVIDENCE.md`.

These are acceptance criteria. If a box is
unchecked, write the next smallest action in `EVIDENCE.md` §9.

## Misconceptions this lesson refutes

| Misconception | Why it fails | What to do instead |
|---|---|---|
| "A record is just a bag of variables." | It is a bag with one lifetime, one declaration and one default. The bag is the point: fields arrive together and are zeroed together. | Compare the initializers, not the field list. |
| "A record makes the code testable." | The record changes signatures. `tryMove` still needs a board, a move number and two out-parameters, and `playTurn` still draws from a global. | Test what the signature allows; the record did not add a seam. |
| "Adding a field never touches callers." | True for the signature, false for the use. Every function that reads the field must be found by `grep`, because no signature lists it. | Count signatures and count reads. They are different numbers. |
| "`const Knight &` means the function cannot change the knight." | It means the function cannot write through **that** reference. `playTurn` holds a non-const reference to the same object. | Check who else holds a mutable reference before trusting the promise. |
| "The record hides the representation." | `row` and `column` appear in 14 places across six functions. Changing them to one cell index rewrites all 14. | That is `construction-09`'s job, not this one's. |
| "If the traces pass, the new field is correct." | An unread field cannot change the output. A default of `true` also passes all 20 traces. | Read the assignment site; the trace only proves the game did not change. |
| "The record version is cheaper, so it is the better design." | It is cheaper for *this* change. It is more expensive for a change that must thread one field differently for two knights, and for a reader who wants to know which fields a function uses. | Price the change you are making, then the next one. |
| "Fewer parameters means a weaker contract." | It means a **coarser** contract: the function may read any field of the object. Coarser can be safer under field addition and weaker under field inspection. | Name which direction the change you are about to make cares about. |

## Next step

`construction-05` — array of records. A knight is now one object; the board is still
`bool visitedCells[9][9]`, and every cell is reached by a hand-written pair of indices. The next
rung gives the cell a name and the board a traversal, so the index arithmetic appears once.

See the [lesson catalog](../../../docs/guide/lesson-catalog.md) for the full path.

If either baseline was missing, do not start `construction-05`. The next contrast needs both sides.

[functions-baseline]: ../construction-03-functions/code/functions-baseline.cpp
