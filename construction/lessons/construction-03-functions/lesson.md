# Functions and parameters (funciones y parámetros)

> [English](lesson.md) | [Español](lesson-es.md)

**Concept:** scope and decomposition · **Time:** about 45 minutes · **Status:** drafted
**Reading:** ~14 minutes

## Why this lesson exists

`construction-02` removed the arbitrary jump. The turn became a region, and the region had one
entry. This lesson asks a different question about the same region: what is it allowed to read?

In `construction-02` the answer is "anything at global scope". Nothing in a call says which globals
a function touches. In `construction-03` the answer is written in the parameter list, and the
compiler enforces it. The sentence to keep is short:

> A global is a parameter that no signature mentions.

You will check that sentence twice. Once statically: how many mutable names live at namespace scope
in each file. Once operationally: can you exercise one move decision without building the game
first? The first check is a number. The second is the one that changes how you test.

## What you will be able to do

By the end of this lesson you can:

- explain scope and decomposition through a function signature;
- extract a move decision without using globals.

## The concept: a signature is the list of names the body may read

Five forms of name do the work in this file:

```text
global      int successfulMoveCount;       visible in every function
local       int destinationRow = 0;        visible in one block
parameter   int activeKnightRow            local, initialized by the caller
reference   int &destinationRow            local name for the caller's object
return      MoveOutcome                    the one value the body hands back
```

A function body can reach three kinds of name: its parameters and locals, the functions it calls,
and the globals. The first is in the text around the call and the second is named at the call. The
third is nowhere. That is the asymmetry this lesson prices. Constants are the exception: visible
everywhere, but not state, because they cannot change between calls.

In `construction-02`, `successfulMoveCount` is a global. `main` reads it, the loop reads it, the
move branch increments it, the header prints it. Nothing in any of those statements says "this
function depends on a counter owned by `main`". You learn that by reading the file, or by changing
the counter and finding out which outputs move.

In `construction-03` the same counter is a local in `main` and a reference parameter of `playTurn`
and `playGame`. The dependency is in the call. The compiler refuses a call that omits it.

Three consequences follow, and they are the lesson:

1. **A function that reads no global can be called before the game exists.** `tryMove` decides a
   move from its parameters alone, so Step 4 can call it with a local board and three integers. That
   is the acceptance criterion.
2. **New turn state must be threaded.** A counter the turn needs must appear in the signature of
   every function between its owner and its user. That is a real cost. Step 5 measures it.
3. **A global can still hide.** `randomState` remains global, so `playTurn` is not testable the same
   way. The scope change moved game state, not process state.

The baseline is honest about what the signature does *not* say, and so is this lesson:

- `bool visitedCells[][boardColumnCount + 1]` decays to a pointer to an array of 9 `bool`. The row
  count is not in the type. A caller can pass an 8-row board or a 3-row board; the compiler cannot
  tell the difference.
- `tryMove` writes the destination before it decides. On `outsideBoard` the two out-parameters hold
  off-board coordinates. The signature does not say "the destination is meaningful only when the
  outcome is `legal` or `capture`".
- A function returns one value. `tryMove` produces three (outcome, row, column), so two travel by
  reference. `construction-04` replaces that with a record.

## Before you start

- [`construction-02`](../construction-02-structured/lesson.md) — you have its two counts: edit sites
  and incoming edges to the turn entry.
- A C++20 compiler (`g++` or `clang++`). `cmake` is optional.

You do not need to know C++ well. You need to read a parameter list, a reference (`&`) and a
`return`. That is the whole skill this lesson exercises.

### What you need

- The functions baseline: [`code/functions-baseline.cpp`](code/functions-baseline.cpp).
- The structured baseline:
  [`structured-baseline.cpp`][structured-baseline].
- Build scripts and notes: [`run.sh`](run.sh) and [`README.md`](README.md).
- The rules: [`common/spec/game-rules.md`](../../../common/spec/game-rules.md) — §12 fixes the
  order of random draws.
- The transcript format:
  [`common/spec/transcript-format.md`](../../../common/spec/transcript-format.md).
- One golden trace: [`common/traces/seed-0001.txt`](../../../common/traces/seed-0001.txt).
- This learner template: [`EVIDENCE.md`](EVIDENCE.md).

Check that the artifacts are present before you begin, from this lesson directory:

```bash
test -f code/functions-baseline.cpp &&
test -f ../construction-02-structured/code/structured-baseline.cpp &&
test -f ../../../common/traces/seed-0001.txt
```

If either baseline is missing, the comparison has no second side. Record the missing file in
`EVIDENCE.md` §10 and return when it is available. A probe against a function you have not read is
not evidence.

Both baselines reproduce all 20 traces in `common/traces/` byte for byte. The game is identical;
only the scope of the names changes. That is what makes the comparison worth doing.

## Step 1 — Inventory both scopes (~10 min)

Read both files once. Then ask the compiler what it sees. Namespace-scope data with external
linkage shows up in the object file; locals and parameters do not.

```bash
mkdir -p /tmp/scope-test
g++ -std=c++20 -c -o /tmp/scope-test/structured.o \
  ../construction-02-structured/code/structured-baseline.cpp
g++ -std=c++20 -c -o /tmp/scope-test/functions.o code/functions-baseline.cpp
nm -C --defined-only -g /tmp/scope-test/structured.o | grep ' [BbDd] '
nm -C --defined-only -g /tmp/scope-test/functions.o | grep ' [BbDd] '
```

The first listing prints 16 names. The second prints one:

```text
0000000000000000 B randomState
```

`B` is zero-initialized data, `D` is initialized data. Both are mutable and both are visible in
every function of that translation unit. The `constexpr` board size, move tables and generator
constants do not appear: constants are not state, and they have internal linkage.

Then answer in `EVIDENCE.md` §2:

1. How many mutable namespace-scope names does each file define? List them with line numbers.
2. Which of the 16 are still globals in `construction-03`, and why that one?
3. For each of the other 15: is it now a local of `main`, a local of another function, a value
   parameter, or a reference parameter? Name the function.
4. How many function definitions does each file have? (Count `main`.)
5. How many parameters does `tryMove` have, and which of them are outputs?
6. Which functions depend on `randomState` without naming it in their own parameter list?
7. In which file can you answer "what does this function depend on?" from its definition line
   alone?

Question 3 is the inventory. Question 6 is the counterexample that keeps the lesson honest.

## Step 2 — Predict before you measure (~8 min)

Two predictions, both written before any run: one about a call, one about a cost.

### 2.1 Predict one call

Step 4 calls `tryMove` with these arguments. Write the answer now, from the function body:

```text
active knight      row 2, column 1
other knight       row 8, column 8
move number        8
visited board      only cell (3,3) is marked
```

The move table is at the top of the file: move 8 changes the row by +1 and the column by +2. The
order of the checks inside `tryMove` is the part worth reading twice.

Record in `EVIDENCE.md` §3.1: the `MoveOutcome` you expect, the destination row and column, and your
confidence. If you predict `legal`, you have skipped a check; that is a calibration signal, not a
mistake to hide.

### 2.2 Predict the cost of R-SKIP

The change test is the same rule you applied in `construction-02`. Read it again, then predict.

> **R-SKIP — skip turn.** Count turns as they start, whether or not they produce a move. When the
> count is a positive multiple of 5, the active knight makes no move: the turn is still announced
> with its `turn=` and `order=` fields, no `try=` line and no `move=` line follow, and play passes
> to the other knight. A skipped turn does not count toward the 200 successful moves.

R-SKIP adds no random draw, so it does not disturb the call order in §12 of the rules. The turn
still draws and prints its order; the skip happens after the header.

Fix the units before you count.

- An **edit site** is a contiguous source change the requirement forces: a declaration, an
  assignment, a format argument, a signature line, or a guard block with its transition.
- A **signature change** is a function whose parameter list or return type changes. Count it once,
  separate from the call sites it forces.
- A **call site** is a place where a call expression must be edited because a signature changed.
- A **caller** is a function that contains a call site. One caller can hold several call sites.
- A **new global** is a mutable name added at namespace scope.
- Do **not** count comments, blank lines, formatting, generated traces, or the number of turns.

The question the README asks is the one to commit to: how many **callers** must change? Answer it
with `grep` before you predict with intuition:

```bash
grep -n 'playTurn(' code/functions-baseline.cpp
grep -n 'playGame(' code/functions-baseline.cpp
```

Each name appears twice: one definition, one call. That is the caller graph, and it is two lines
wider than you probably expect.

Then fill the cost table in `EVIDENCE.md` §3.2: edit sites, signature changes, call sites, callers
of `playTurn` changed, callers of `playGame` changed, new globals, new control-flow edges, files
touched, and golden traces broken. Use your `construction-02` numbers for the second column. If you
did not keep them, re-derive them from that lesson's §6.

Commit to a number for each row. "The functions version is cleaner" is not a prediction; it does not
say which count moves.

## Step 3 — Run both baselines (~5 min)

From this lesson directory:

```bash
./run.sh --seed 1 --name Learner > /tmp/scope-test/functions-0001.txt
diff -u ../../../common/traces/seed-0001.txt /tmp/scope-test/functions-0001.txt
```

Then the structured baseline:

```bash
cd ../construction-02-structured
./run.sh --seed 1 --name Learner > /tmp/scope-test/structured-0001.txt
diff -u ../../../common/traces/seed-0001.txt /tmp/scope-test/structured-0001.txt
```

Then the two against each other:

```bash
diff -u /tmp/scope-test/structured-0001.txt /tmp/scope-test/functions-0001.txt
```

Three empty diffs. The first two say each file is the game. The third says the two files are the
same game, which is the only reason the scope comparison is fair.

Return to this lesson directory. If `run.sh` cannot build, compile once by hand:

```bash
mkdir -p build
g++ -std=c++20 -Wall -Wextra -Werror -O2 \
  -o build/functions-baseline code/functions-baseline.cpp
```

If a diff is not empty, stop. Record it in `EVIDENCE.md` §10. Do not adjust the trace to match the
code.

## Step 4 — Call one move decision with local state (~7 min)

This is the acceptance criterion of the lesson: one move decision, exercised through a function
with parameters, using nothing but local state.

Copy the baseline. The tracked file stays untouched.

```bash
mkdir -p /tmp/scope-test
cp code/functions-baseline.cpp /tmp/scope-test/functions-probe.cpp
```

In the copy, make one mechanical edit: rename `int main(` to `int baselineMain(`. Then append this:

```cpp
const char *outcomeName(MoveOutcome outcome)
{
    switch (outcome)
    {
    case MoveOutcome::outsideBoard:
        return "outside";
    case MoveOutcome::visited:
        return "visited";
    case MoveOutcome::capture:
        return "capture";
    case MoveOutcome::legal:
        return "legal";
    }
    return "unknown";
}

int main()
{
    // Local state only: one board, one active knight, one other knight.
    bool visitedCells[boardRowCount + 1][boardColumnCount + 1] = {};
    visitedCells[3][3] = true;

    int destinationRow = 0;
    int destinationColumn = 0;
    MoveOutcome outcome = tryMove(2, 1, 8, 8, 8, visitedCells, destinationRow,
                                  destinationColumn);
    std::printf("outcome=%s to=%d,%d\n", outcomeName(outcome), destinationRow,
                destinationColumn);
    return 0;
}
```

Build and run:

```bash
g++ -std=c++20 -Wall -Wextra -Werror -O2 \
  -o /tmp/scope-test/functions-probe /tmp/scope-test/functions-probe.cpp
/tmp/scope-test/functions-probe
```

The output is:

```text
outcome=visited to=3,3
```

Compare it with your §3.1 prediction. Then change one input at a time and confirm the other three
outcomes. The board and the coordinates are yours; no game was initialized.

| Active | Move | Other | Board | Expected output |
|---|---|---|---|---|
| 2,1 | 8 | 8,8 | (3,3) visited | `outcome=visited to=3,3` |
| 2,1 | 1 | 8,8 | (3,3) visited | `outcome=legal to=1,3` |
| 2,1 | 8 | 3,3 | (3,3) visited | `outcome=capture to=3,3` |
| 1,1 | 2 | 8,8 | (3,3) visited | `outcome=outside to=-1,2` |

Two rows deserve a comment.

- The capture case wins over the visited case even though the cell is marked. `tryMove` tests
  capture before visited. The function's order of checks is part of its contract, and the probe is
  how you see it.
- The outside case prints `-1,2`, an off-board destination, because the out-parameters were written
  before the bounds check. The outcome says the destination is meaningless. The type does not.

Now the point of the exercise. This `main` never calls `parseArguments`, never seeds the generator,
never initializes positions, never creates the game. It cannot: `tryMove` needs none of that. Its
inputs are its parameters, and its parameters are visible at the call.

Write in `EVIDENCE.md` §5:

- The exact command that built the probe and the exact output.
- Which of the four rows you predicted, and which surprised you.
- One sentence: why this call is possible in `construction-03` and not in `construction-02`.

If you want the contrast, call `playTurn` from the same probe. It compiles, but you must set
`randomState` first, because its first action draws from the generator. Record what that costs you:
one line of setup, and the knowledge that the function has an input the signature does not mention.

## Step 5 — Apply R-SKIP and count the boundary (~10 min)

Return to this lesson directory. Copy the baseline again to a second scratch file:

```bash
cp code/functions-baseline.cpp /tmp/scope-test/functions-rskip.cpp
```

Apply R-SKIP to the copy. The rule is the one from `construction-02`; these are the details that
interact with scope:

1. The started-turn counter is declared **inside `playGame`**, next to its loop, not in `main`.
2. `playTurn` receives it by reference and increments it before the order draw.
3. `printTurnHeader` receives the counter instead of `successfulMoveCount + 1`.
4. After the header and before the first attempt, if the counter is a multiple of 5, flip
   `playerIsActive` and return from `playTurn`.
5. The 200-move cap and `moves=` stay on `successfulMoveCount`.

Why not the other two placements? This table is the scope decision, and it is worth a minute.

| Where the counter lives | Survives a call? | Visible without a parameter? | Verdict |
|---|---|---|---|
| local in `playTurn` | no | no | resets every turn; cannot count turns |
| `static` in `playTurn` | yes | yes | a global with a smaller name |
| local in `playGame`, by reference | yes | no | the counter belongs to the loop |

The rule is the narrowest scope that covers every use. Every use is inside the turn; the lifetime
must span turns; therefore the loop owns it.

Count first, then compare with this table. It is the expected shape in the revision shipped with
this lesson.

| Site | Line | What changes |
|---|---|---|
| `playTurn` signature | 232–234 | one more reference parameter |
| first statement of `playTurn` | after 235 | increment the counter |
| `printTurnHeader` call | 247 | `startedTurnCount` replaces `successfulMoveCount + 1` |
| after the header | after 248 | the guard block and its `return` |
| `playGame` body | before 317 | local counter, initialized to 0 |
| `playTurn` call | 319–320 | pass the counter |

Six edit sites, one signature, one call site, one new control-flow edge (the `return` to the
function boundary), zero new globals. `playGame`'s caller — `main` — does not change, because the
counter is owned below it.

Build and run:

```bash
g++ -std=c++20 -Wall -Wextra -Werror -O2 \
  -o /tmp/scope-test/functions-rskip /tmp/scope-test/functions-rskip.cpp
/tmp/scope-test/functions-rskip --seed 1 --name Learner > /tmp/scope-test/rskip-0001.txt
diff -u ../../../common/traces/seed-0001.txt /tmp/scope-test/rskip-0001.txt | head
```

R-SKIP changes the game, so the diff must not be empty. For seed 1, the first divergence is the
fifth turn: the `turn=5` header is followed by the `turn=6` header instead of a `try=` line.

If you still have your `construction-02` scratch binary, diff the two modified transcripts. They
must be identical: one rule, two shapes, same game.

```bash
diff -u /tmp/rskip/structured-0001.txt /tmp/scope-test/rskip-0001.txt
```

Then run all 20 seeds against the golden traces. Every one will differ, because the skip happens at
turn 5 in every game that reaches it. The number measures the contract change, not a regression.

Now answer §6.6:

- How many callers does `playTurn` have? How many change?
- How many callers does `playGame` have? How many change?
- Which count ties with `construction-02`, and which does not?
- What did the extra signature buy? Name the check the compiler now performs.
- Where could new turn state leak in each file, and how would a reader notice?

The honest answer to the third question comes from your own totals. The functions version adds a
signature and a call site; it does not make this change cheaper. It makes the new dependency
impossible to add silently. If your `construction-02` total was five, this one is six.

## Step 6 — Explain the difference (~3 min)

Write §7. Two parts.

**First, one sentence.** Say what the signature decides about the body, without using *better*,
*worse*, *clean*, *cleaner*, *nice*, *elegant*, *ugly* or *modular*. Those words name a verdict, not
a scope fact. If your sentence needs one, you have described your reaction.

**Second, one paragraph.** Answer:

- Which names can `tryMove` read that its caller did not pass?
- Which names can `playTurn` read that its caller did not pass?
- What does that difference do to the probe in Step 4?
- What must a reader hold in working memory to know a function's inputs in each file?
- If a new piece of turn state is added, where must it be declared, and what forces the threading?
- What does the signature still fail to say, and which later lesson fixes it?

A strong answer separates **the set of names a body can reach** from **the number of edits the
change costs**. A weak answer says the functions file is cleaner. Clean is what you see; an empty
global list and a filled parameter list are what cause it.

## Step 7 — Acceptance check (~2 min)

The acceptance criterion for this lesson is: **one move decision is exercised through a function
with parameters; the test uses local state; the explanation names the scope change.**

Check your record against it:

- [ ] I ran both unmodified baselines and got three empty diffs.
- [ ] I inventoried the globals in both files from the object files, not from memory.
- [ ] I predicted the `tryMove` outcome before running the probe.
- [ ] The probe calls the tracked `tryMove`, not a copy of the rule.
- [ ] The probe initializes no game state: no seed, no positions, no `playGame`.
- [ ] I recorded the exact probe command and output.
- [ ] I applied R-SKIP with the counter owned by `playGame`.
- [ ] I counted callers with `grep`, not by assumption.
- [ ] I can state which count ties with `construction-02` and which does not.
- [ ] My one-sentence explanation avoids the eight forbidden words.
- [ ] I did not edit the tracked baselines.
- [ ] I completed `EVIDENCE.md` §1–§9 and the self-check in §11.

The criterion is not "the functions version wins". It is "you can say exactly what the parameter
list made visible, and what it charged for it".

## Self-check (binary)

- [ ] I read both baselines and inventoried their scopes.
- [ ] I predicted one `tryMove` outcome and the R-SKIP cost before measuring.
- [ ] I built both baselines and got three empty diffs.
- [ ] I called one move decision with a local board and no game setup.
- [ ] I applied R-SKIP to a scratch copy and counted the boundary.
- [ ] I compared the prediction with the counts and named the misses.
- [ ] I explained the difference in one sentence plus one paragraph, by scope.
- [ ] I completed `EVIDENCE.md`.

These are acceptance criteria. If a box is
unchecked, write the next smallest action in `EVIDENCE.md` §9.

## Misconceptions this lesson refutes

| Misconception | Why it fails | What to do instead |
|---|---|---|
| "A parameter is a global with a longer name." | A parameter is initialized per call and named at the call site; a global survives calls and is visible without being named. | Ask whether the caller can see the dependency. |
| "A reference parameter is the same as a global." | The caller chooses the object and the choice is in the signature. A global chooses itself. | Read the parameter list, not the body. |
| "A `static` local is a local." | It has static storage duration and survives calls. It is a global with a smaller name and no parameter. | Put the state in the owner and pass it. |
| "Returning a value is always better than an out-parameter." | `tryMove` produces three results; a function returns one. | Count the results before choosing the mechanism. |
| "The signature lists everything the function does." | The array decays, so the row count is unstated; the destination is written on early return. | Read the body for the order of checks and the write points. |
| "No game globals means no hidden state." | `randomState` is still global, and `playTurn` depends on it. | List the names the body can reach that no parameter supplies. |
| "Fewer globals makes the change test cheaper." | Here the edit costs a signature and a call site, not fewer keystrokes. | Measure the edit, then measure the test. |
| "The probe duplicates the game." | It calls the tracked `tryMove`; it does not reimplement the move rule. | Link the real function; never copy the rule. |

## Next step

`construction-04` — records. `tryMove` already returns three results through one return value and
two references. The next rung puts them in one name, so the signature stops growing a parameter per
field.

See the [lesson catalog](../../../docs/guide/lesson-catalog.md) for the full path.

If either baseline was missing, do not start `construction-04`. The next contrast needs both sides.

[structured-baseline]: ../construction-02-structured/code/structured-baseline.cpp
