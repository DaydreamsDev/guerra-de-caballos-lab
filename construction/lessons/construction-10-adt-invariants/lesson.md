# Abstract data type invariants (invariantes del tipo abstracto de datos)

> [English](lesson.md) | [Español](lesson-es.md)

**Concept:** validity and lifecycle (validez y ciclo de vida) · **Time:** about 60 minutes ·
**Status:** drafted
**Reading:** ~16 minutes

## Why this lesson exists

`construction-09` made `Board` opaque. A caller can hold it, pass it and destroy it, and cannot see
its bytes. What a caller also could not do was ask whether the object is in a legal state. Opaque is
not the same as valid: the header hid the layout and promised nothing about the values inside it.

This baseline adds one declaration and one definition:

```cpp
bool board_is_valid(const Board *board);
```

Eight operations now, and a rule behind them: only cells inside the active 8×8 area may be marked
visited. Three places carry that rule:

| Check | Location | What it does |
|---|---|---|
| `board_mark_visited` guard | `board.cpp:81` | Ignores a row or column outside 1..8. |
| `board_was_visited` guard | `board.cpp:71` | Returns `false` outside 1..8. |
| Outside-active-area scan | `board.cpp:36–56` | Returns `false` if a cell outside the active area is marked. |

Then the part the README does not say. `board_is_valid` is **never called** by the game:

```bash
grep -rn 'board_is_valid' code/
```

Two hits: the declaration in `board.h` and the definition in `board.cpp`. Zero call sites. All 20
traces pass without evaluating the invariant once, so they cannot tell you whether it holds. The
only caller in this lesson is the probe you are about to write.

The gap is not in the checker. It is in the wiring: the rule has two enforcement points and one
observer, and no client. The change test closes the loop. Remove the guard in a scratch copy, mark
`(0, 0)`, and the checker catches the illegal state. Restore the guard, and the same call is
rejected before it reaches the array.

The sentence to keep:

> An invariant is a predicate, an enforcement point, and a caller that checks it. This baseline
> ships the first two; the third one is you.

Three honest notes before the comparison.

1. The source grows by 23 lines: 502 across seven files, against 479 in `construction-09`.
   `board.h` goes from 11 to 12 lines, `board.cpp` from 65 to 87. The rest is unchanged.
2. The object grows by 104 bytes of text: 608 for `board.o`, against 504 in `construction-09`.
   Eight exported symbols instead of seven. The new one is `_Z14board_is_validPK5Board`.
3. Twenty of twenty traces do not exercise the checker, because nothing calls it. The probe does,
   and the change test does. A trace is a behavioral contract; an invariant needs its own caller.

## What you will be able to do

By the end of this lesson you can explain validity and lifecycle and apply the lesson's change test.

## The concept: a check nobody calls is still only a seam

`construction-09` answered one question: can a caller see the representation? The answer was no.
This lesson asks a different question: can a caller trust the values inside the object? The answer
is not in the header. The header gives you a name and eight operations; it does not say which states
are legal.

An invariant is a property that must hold for every legal state of the object. For this board there
are two:

1. **Bounds.** No cell outside rows 1..8 and columns 1..8 is ever marked visited.
2. **Lifecycle.** A board is usable from `board_create` to `board_destroy`, and every state between
   them satisfies rule 1.

Rule 1 has two enforcement points and one observer:

| Role | Code | What it decides |
|---|---|---|
| Enforcement | `board_mark_visited` | Whether a write reaches the array. |
| Enforcement | `board_was_visited` | Whether a read reaches the array. |
| Observation | `board_is_valid` | Whether the array currently satisfies rule 1. |

Enforcement and observation are different jobs. The guards make an illegal write *not happen*. The
checker says whether the state is legal *right now*. A design can have either without the other, and
this baseline is the example: it has all three, and no caller for the third.

Why the observer matters at all, if the guards already stop illegal writes? Because a guard is a
local decision and an invariant is a global claim. A guard can be removed by an edit, bypassed by a
second function, or defeated by a bug in the array index. The checker is the only place that looks
at the whole object and answers one yes-or-no question. It is also the only place a test can call.

This checker is correct, and the lesson shows why in three parts:

- **The scan domain is the array.** `row` and `column` run from 0 to 15, with `<`, over
  `bool visited[16][16]`. Every cell is read exactly once, and none is read twice or missed.
- **The predicate matches the guards.** A cell is outside the active area when
  `row < 1 || row > 8 || column < 1 || column > 8`. That is the same condition the two guards use,
  so the observer and the enforcement points agree on what "outside" means.
- **The null case is explicit.** `board_is_valid(nullptr)` returns `false` before it touches the
  array, instead of dereferencing.

What the checker cannot do is call itself. The header offers the check; the game does not take it.
That is the design gap this lesson exposes, and `construction-11` closes it from the other side: it
makes the field unreachable, so a caller cannot construct the invalid state in the first place.

The lifecycle half is thinner than it sounds. `board_create` returns `new Board{}`, which
value-initializes all 256 cells to `false`. `board_reset` clears rows and columns 1..8. Nothing
clears rows 9..15 after creation. That is sound while the invariant holds, because those cells start
at `false` and no legal write reaches them. It stops being sound the moment an illegal write does.

## Before you start

- [`construction-09`](../construction-09-abstract-data-type/lesson.md) — the catalog prerequisite.
  You need the opaque `Board` and its seven operations in front of you, because this lesson is a
  diff against that baseline.
- [`construction-03`](../construction-03-functions/lesson.md) — you can read a function
  declaration, a parameter list and a return type.
- A C++20 compiler (`g++` or `clang++`). `cmake` is optional; `run.sh` falls back to a direct
  compile when CMake is absent.

You do not need to know how the linker resolves symbols. You need to read two loop bounds, compare
them with an array size, and run one small program.

### What you need

- The invariant baseline:
  [`code/main.cpp`](code/main.cpp), [`code/game.h`](code/game.h),
  [`code/game.cpp`](code/game.cpp), [`code/board.h`](code/board.h),
  [`code/board.cpp`](code/board.cpp), [`code/random_sequence.h`](code/random_sequence.h),
  [`code/random_sequence.cpp`](code/random_sequence.cpp).
- The previous baseline, for the comparison: the
  [`board.h`](../construction-09-abstract-data-type/code/board.h) and
  [`board.cpp`](../construction-09-abstract-data-type/code/board.cpp) of `construction-09`.
- Build scripts and notes: [`run.sh`](run.sh), [`CMakeLists.txt`](CMakeLists.txt) and
  [`README.md`](README.md).
- The rules: [`common/spec/game-rules.md`](../../../common/spec/game-rules.md) — §1 fixes the board
  and its 1-based numbering, §8 fixes when a cell becomes visited.
- The transcript format:
  [`common/spec/transcript-format.md`](../../../common/spec/transcript-format.md).
- One golden trace: [`common/traces/seed-0001.txt`](../../../common/traces/seed-0001.txt).
- This learner template: [`EVIDENCE.md`](EVIDENCE.md).

Check that the artifacts are present before you begin, from this lesson directory:

```bash
test -f code/main.cpp &&
test -f code/game.h &&
test -f code/game.cpp &&
test -f code/board.h &&
test -f code/board.cpp &&
test -f code/random_sequence.h &&
test -f code/random_sequence.cpp &&
test -f ../construction-09-abstract-data-type/code/board.cpp &&
test -f ../../../common/traces/seed-0001.txt
```

If a file is missing, the comparison has no second side. Record the missing file in `EVIDENCE.md`
§11 and return when it is available. A count against a file you have not read is not evidence.

The baseline reproduces all 20 traces in `common/traces/` byte for byte, before you change
anything. That is the behavioral contract, and it is silent about validity.

## Step 1 — Inventory the three checks (~10 min)

Read the header first. It is 12 lines, and the new line is the fifth operation:

```bash
grep -n . code/board.h
```

```text
1:#pragma once
2:
3:struct Board;
4:
5:Board *board_create();
6:void board_destroy(Board *board);
7:int board_row_count(const Board *board);
8:int board_column_count(const Board *board);
9:bool board_is_valid(const Board *board);
10:void board_reset(Board *board);
11:bool board_was_visited(const Board *board, int row, int column);
12:void board_mark_visited(Board *board, int row, int column);
```

Now the line map of `board.cpp`. Fill it into `EVIDENCE.md` §2.2 from the source, not from this
table:

| Lines | Contents |
|---|---|
| 5–8 | Four constants: maximum 16, active 8. |
| 11–14 | `struct Board` with `bool visited[16][16]`. |
| 16–19 | `board_create` |
| 21–24 | `board_destroy` |
| 26–29 | `board_row_count` |
| 31–34 | `board_column_count` |
| 36–56 | `board_is_valid` |
| 58–67 | `board_reset` |
| 69–77 | `board_was_visited` |
| 79–87 | `board_mark_visited` |

Counts for this baseline: 3 headers, 4 translation units, 12 public declarations, 1 incomplete
type, 31 definitions, 18 internal helpers (17 in `game.cpp`, 1 in `main.cpp`), 1 entry point.

Compare with `construction-09`: 11 public declarations and 30 definitions. This lesson added one
declaration and one definition. The other seven declarations did not change, and neither did any
caller.

Then answer in `EVIDENCE.md` §2:

1. Which operation is new, and which file declares it?
2. How many call sites does `board_is_valid` have in `code/`?
3. Which two operations enforce the bounds rule, and what does each do on an out-of-range index?
4. What does `board_is_valid` do with a `nullptr`?
5. Which cells does `board_reset` clear, and which cells does it leave alone?
6. Which cells does `board_create` initialize, and with what value?
7. Does `game.cpp` depend on the checker in any way?
8. If you deleted `board_is_valid` from both files, what would stop compiling?

Question 2 has a precise answer: zero. Question 8 has the same answer: nothing. The function is
declared, defined, exported, and unused. That is not a contradiction; it is the seam where a test
would attach. But a seam with nothing attached is not a guarantee.

Question 3 has two halves. `board_mark_visited` returns without writing; `board_was_visited` returns
`false` without reading. Neither one reports an error. The checker is the operation that reports.

## Step 2 — Predict before you run (~10 min)

### 2.1 The guards

Write these predictions before running anything. The array is `bool visited[16][16]`; the active
area is rows 1..8 and columns 1..8.

| Call sequence | Predicted `board_was_visited` after | Predicted `board_is_valid` after |
|---|---|---|
| `board_mark_visited(board, 8, 8)` | | |
| `board_mark_visited(board, 0, 0)` | | |
| `board_mark_visited(board, 9, 9)` | | |
| `board_mark_visited(board, 4, 0)` | | |
| `board_mark_visited(board, 8, 9)` | | |

The first row is the only legal write. The other four are out of range in one way or another: below
the active area, above it, and on the reserved row or column 0. Predict what each one leaves behind.

### 2.2 The checker

| Question | My prediction |
|---|---|
| `board_is_valid` on a freshly created and reset board | |
| `board_is_valid` after one legal mark | |
| `board_is_valid` after one guarded out-of-range mark | |
| Seeds that match, of 20 | |
| Does the game call the checker? | |
| What happens to `board_is_valid` if the guard is removed and `(0, 0)` is marked? | |
| Does `board.o` grow, shrink or stay the same size? | |

The first row is the one to take seriously. A checker that cannot say "true" about a fresh object is
not a checker. Predict `true`, then measure. If you predict `false`, write down which line of the
scan would make it return false, and check that line in Step 3.

### 2.3 The demonstration

The change test for this lesson is: remove the bounds guard from `board_mark_visited`, mark
`(0, 0)`, and watch `board_is_valid` turn false. Predict which of the two guards you have to remove
for that to happen, and which line of the predicate classifies `(0, 0)` as outside. Write the
prediction in `EVIDENCE.md` §3.3.

## Step 3 — Run the baseline and a probe (~10 min)

First the traces, unmodified:

```bash
mkdir -p /tmp/invariant-test
./run.sh --seed 1 --name Learner > /tmp/invariant-test/baseline-0001.txt
diff -u ../../../common/traces/seed-0001.txt /tmp/invariant-test/baseline-0001.txt
for trace in ../../../common/traces/seed-*.txt; do
  traceName="$(basename "${trace}" .txt)"
  seedText="${traceName#seed-}"
  seedNumber="$((10#${seedText}))"
  ./run.sh --seed "${seedNumber}" --name Learner | cmp - "${trace}"
done
```

One empty diff and a silent loop: 20 of 20. If `run.sh` cannot build, compile once by hand:

```bash
mkdir -p build
g++ -std=c++20 -Wall -Wextra -Werror -O2 -Icode \
  -o build/invariant-baseline code/main.cpp code/game.cpp code/board.cpp \
  code/random_sequence.cpp
```

Now the probe. It goes in the scratch directory, never in the tracked files:

```bash
mkdir -p /tmp/invariant-test/probe
cat > /tmp/invariant-test/probe/probe.cpp <<'EOF'
#include "board.h"

#include <cstdio>

int main()
{
    Board *board = board_create();
    board_reset(board);
    std::printf("fresh: valid=%d\n", static_cast<int>(board_is_valid(board)));

    board_mark_visited(board, 8, 8);
    std::printf("mark(8,8): valid=%d was=%d\n",
                static_cast<int>(board_is_valid(board)),
                static_cast<int>(board_was_visited(board, 8, 8)));

    board_mark_visited(board, 0, 0);
    std::printf("mark(0,0): valid=%d was=%d\n",
                static_cast<int>(board_is_valid(board)),
                static_cast<int>(board_was_visited(board, 0, 0)));

    board_mark_visited(board, 9, 9);
    std::printf("mark(9,9): valid=%d was=%d\n",
                static_cast<int>(board_is_valid(board)),
                static_cast<int>(board_was_visited(board, 9, 9)));

    board_destroy(board);
    return 0;
}
EOF
g++ -std=c++20 -Wall -Wextra -Werror -O2 -Icode \
  -o /tmp/invariant-test/probe/probe /tmp/invariant-test/probe/probe.cpp code/board.cpp
/tmp/invariant-test/probe/probe
```

The output on this platform:

```text
fresh: valid=1
mark(8,8): valid=1 was=1
mark(0,0): valid=1 was=0
mark(9,9): valid=1 was=0
```

Read the first line twice. A fresh board, created and reset, is valid. The guards did their job:
`was=0` for both out-of-range marks, and `was=1` for the legal one. And the checker agreed with
them after every call, which is the part `construction-09` could not offer.

If your first line is `0`, stop. That is a different baseline from the one this lesson describes;
record it in `EVIDENCE.md` §11 and do not adjust the prediction to match.

## Step 4 — Compare: what the probe proves (~8 min)

Open `board.cpp` at the scan, lines 42–54:

```cpp
for (int row = 0; row < maximumBoardRowCount; row += 1)
{
    for (int column = 0; column < maximumBoardColumnCount; column += 1)
    {
        bool outsideActiveArea =
            row < 1 || row > activeBoardRowCount || column < 1 ||
            column > activeBoardColumnCount;
        if (outsideActiveArea && board->visited[row][column])
        {
            return false;
        }
    }
}
```

Four facts, read against `bool visited[16][16]`:

1. **The scan domain is exactly the array.** `maximumBoardRowCount` is 16, the array dimension is
   16, and the condition is `<`. The last iteration reads `visited[15][15]`, the last element. No
   cell is skipped, and none is read out of bounds.
2. **Storage and validity are different spaces.** `(0, 0)` is a valid array index and an invalid
   board cell. The scan visits it because it is inside the array; the predicate rejects it because
   `row < 1`. `(16, 16)` is outside both, and the scan never reaches it: the guards stop the write
   and the loop stops at 15.
3. **The predicate is the same rule as the guards.** Compare lines 47–48 with lines 71–72 and
   81–82. All three say: below 1, above 8, left of 1, right of 8. When the two halves agree, a
   guarded write and the checker cannot disagree.
4. **The null check comes first.** `board_is_valid(nullptr)` returns `false` at line 40, before any
   dereference. That is one input contract the header does not state, but the implementation
   enforces.

The probe output, one row per call:

| Call | Guard decision | Array written? | `board_was_visited` | `board_is_valid` |
|---|---|---|---|---|
| none, fresh board | — | no | — | `1` |
| `mark(8, 8)` | inside | yes, `visited[8][8]` | `1` | `1` |
| `mark(0, 0)` | outside | no | `0` | `1` |
| `mark(9, 9)` | outside | no | `0` | `1` |

Now the limit of the evidence. The probe proves the checker works. The 20 traces prove the game
still behaves. Neither proves that the program uses the checker, and it does not:

- No translation unit calls `board_is_valid`. `grep -rn 'board_is_valid' code/` returns two hits:
  one declaration and one definition.
- A trace exercises `run_game`; the checker is not on that path.
- The probe is the only caller in this lesson, and the change test in Step 5 is the second.

Record in `EVIDENCE.md` §5 the four facts, the four probe rows, and the call-site count. The count
is the number that separates "the invariant is implemented" from "the invariant is used".

## Step 5 — Run the change test (~12 min)

Copy the baseline. Tracked files stay untouched.

```bash
lessonDirectory="$(pwd)"
cp -r code /tmp/invariant-test/fixed
cd /tmp/invariant-test/fixed
```

Write the change probe first, while the guard is still in place. It prints validity before and after
one illegal call:

```bash
mkdir -p /tmp/invariant-test/probe
cat > /tmp/invariant-test/probe/change.cpp <<'EOF'
#include "board.h"

#include <cstdio>

int main()
{
    Board *board = board_create();
    board_reset(board);
    std::printf("before mark: valid=%d\n", static_cast<int>(board_is_valid(board)));

    board_mark_visited(board, 0, 0);
    std::printf("after mark(0,0): valid=%d\n",
                static_cast<int>(board_is_valid(board)));

    board_destroy(board);
    return 0;
}
EOF
g++ -std=c++20 -Wall -Wextra -Werror -O2 -I. \
  -o /tmp/invariant-test/probe/change /tmp/invariant-test/probe/change.cpp board.cpp
/tmp/invariant-test/probe/change
```

```text
before mark: valid=1
after mark(0,0): valid=1
```

That is the enforcement half working: the call is rejected, the array is untouched, the state is
still legal.

### 5.1 Remove the guard

In the scratch copy only, delete the guard from `board_mark_visited`:

```cpp
void board_mark_visited(Board *board, int row, int column)
{
    board->visited[row][column] = true;
}
```

Rebuild and run the same probe:

```bash
g++ -std=c++20 -Wall -Wextra -Werror -O2 -I. \
  -o /tmp/invariant-test/probe/change /tmp/invariant-test/probe/change.cpp board.cpp
/tmp/invariant-test/probe/change
```

```text
before mark: valid=1
after mark(0,0): valid=0
```

There it is. The write reached `visited[0][0]`, and the scan saw it: row 0 is outside the active
area, the cell is `true`, and the function returned `false`. The invalid state is now constructible,
and the checker is what makes it visible.

### 5.2 Restore the guard

Put the guard back and rebuild:

```bash
g++ -std=c++20 -Wall -Wextra -Werror -O2 -I. \
  -o /tmp/invariant-test/probe/change /tmp/invariant-test/probe/change.cpp board.cpp
/tmp/invariant-test/probe/change
```

```text
before mark: valid=1
after mark(0,0): valid=1
```

Same call, different outcome, and the only difference is five lines of guard. That is the acceptance
demonstration: the rule is enforced at the write, and the checker confirms the state either way.

### 5.3 The traces, with and without the guard

Build the whole program from the scratch copy and run the 20 traces:

```bash
g++ -std=c++20 -Wall -Wextra -Werror -O2 -I. \
  -o invariant-fixed main.cpp game.cpp board.cpp random_sequence.cpp
cd "${lessonDirectory}"
for trace in ../../../common/traces/seed-*.txt; do
  traceName="$(basename "${trace}" .txt)"
  seedText="${traceName#seed-}"
  seedNumber="$((10#${seedText}))"
  /tmp/invariant-test/fixed/invariant-fixed --seed "${seedNumber}" --name Learner |
    cmp - "${trace}"
done
```

A silent loop is 20 of 20. Now remove the guard again, rebuild with the same command, and repeat:
also 20 of 20. The game never marks a cell outside the active area, so removing the guard changes
nothing it can do. The trace cannot see the difference; only the probe can. That is the whole
argument for a caller: the invariant is not protected by the tests, it is protected by the checks,
and the checks need someone to run them.

Restore the guard in the scratch copy before the next step.

Record in `EVIDENCE.md` §6: the diff of your one change, the three probe runs, and both trace
counts.

## Step 6 — Lifecycle: what `reset` does not repair (~5 min)

The lifecycle rule says a board is usable from `board_create` to `board_destroy`. Two functions
define the edges, and one of them is weaker than it looks.

```cpp
Board *board_create()
{
    return new Board{};
}
```

`new Board{}` value-initializes the object, so all 256 cells start at `false`. That is the only
moment the whole array is known to be clean.

```cpp
void board_reset(Board *board)
{
    for (int row = 1; row <= activeBoardRowCount; row += 1)
    {
        for (int column = 1; column <= activeBoardColumnCount; column += 1)
        {
            board->visited[row][column] = false;
        }
    }
}
```

`board_reset` clears rows and columns 1..8. It does not clear 0 or 9..15, because a correct board
never has anything there. That is a valid optimization and an invalid repair. Remove the guard from
`board_mark_visited` again in the scratch copy (the same edit as Step 5.1), then break the invariant
and reset:

```bash
cat > /tmp/invariant-test/probe/reset.cpp <<'EOF'
#include "board.h"

#include <cstdio>

int main()
{
    Board *board = board_create();
    board_reset(board);
    board_mark_visited(board, 9, 9);
    std::printf("after unguarded mark(9,9): valid=%d\n",
                static_cast<int>(board_is_valid(board)));
    board_reset(board);
    std::printf("after reset: valid=%d was(9,9)=%d\n",
                static_cast<int>(board_is_valid(board)),
                static_cast<int>(board_was_visited(board, 9, 9)));
    board_destroy(board);
    return 0;
}
EOF
g++ -std=c++20 -Wall -Wextra -Werror -O2 -I. \
  -o /tmp/invariant-test/probe/reset /tmp/invariant-test/probe/reset.cpp board.cpp
/tmp/invariant-test/probe/reset
```

```text
after unguarded mark(9,9): valid=0
after reset: valid=0 was(9,9)=0
```

And `board_was_visited(9, 9)` returns `false` anyway, because its guard rejects the index. The only
observer that can see the damage is `board_is_valid`. That is why the checker is not optional
decoration: it is the sole reader of the cells the rest of the interface refuses to name.

Fill this table in `EVIDENCE.md` §7:

| Question | Answer |
|---|---|
| Who can break the invariant? | |
| Which function can detect it? | |
| Which function repairs it? | |
| Does `board_reset` repair a broken board? | |
| What happens to the invariant at `board_destroy`? | |
| Who is responsible for calling the checker? | |

The last row is the design gap this lesson exposes. Nobody is. The header offers the check; no
caller takes it. `construction-11` closes the gap from the other side: it makes the field
unreachable, so a caller cannot construct the invalid state in the first place. `construction-16`
adds the other half of the lifecycle, an owner that guarantees `board_destroy` runs.

## Step 7 — Compare and explain (~3 min)

Write `EVIDENCE.md` §8. Two parts.

**First, one sentence.** Say what an invariant decides about an object, without using the words
*header*, *file*, *module*, *compile*, *pointer*, *struct*, *class*, *opaque*, *encapsulation*,
*better*, *worse*, *clean*, *cleaner*, *nice*, *elegant*, *ugly* or *modular*. If your sentence
needs one of those, you have described the syntax rather than the decision.

**Second, one paragraph.** Answer:

- What the invariant fixes: which states are legal, and which operations may move between them.
- What enforces it: two guards that stop illegal accesses, and one scan that reports the state.
- What makes the scan correct: a domain of 0..15 over a 16×16 array, and a predicate identical to
  the guards' condition.
- What the scan cannot do: it observes. It cannot prevent a write, and nobody calls it.
- What the 20 traces prove: the game's behavior is unchanged, with and without the guard.
- What the traces do not prove: that the checker is correct, or that it ever runs.
- What the change test proves: with the guard removed, the illegal state exists and the checker
  reports it; with the guard restored, the same call leaves the state legal.
- What the change test costs: five lines in one file, zero caller edits, zero broken traces.
- What `board_reset` does not do: repair a board that is already invalid.
- Which later lesson removes the possibility instead of checking it: `construction-11`.
- Which later lesson owns the other end of the lifecycle: `construction-16`.

A strong answer keeps four things apart: **what the rule says**, **what the code enforces**, **what
the checker observes**, and **what a run can see**. A weak answer says the board is validated, which
collapses all four.

## Step 8 — Acceptance check and record (~2 min)

The acceptance criterion for this lesson is: **show that an out-of-range write is rejected, that
`board_is_valid` stays true, and that removing the guard makes the invalid state visible.**

Check your record against it:

- [ ] I identified `board_is_valid`, the two bounds guards and the outside-active-area rule.
- [ ] I found that `board_is_valid` has zero call sites in `code/`.
- [ ] I predicted `true` for a fresh board and observed `true`.
- [ ] I explained the scan domain as `0..15` over a 16×16 array.
- [ ] I explained why `(0, 0)` is a valid index and an invalid cell.
- [ ] I showed that the predicate and the two guards use the same four comparisons.
- [ ] With the guard intact, `board_mark_visited(0, 0)` and `(9, 9)` are rejected.
- [ ] With the guard removed, `board_mark_visited(0, 0)` makes `board_is_valid` false.
- [ ] I restored the guard and confirmed the invalid state is rejected again.
- [ ] All 20 traces pass with the guard intact and with it removed.
- [ ] I recorded that the traces cannot see the checker, because nothing calls it.
- [ ] I showed that `board_reset` does not repair an already invalid board.
- [ ] I did not edit any tracked code file.
- [ ] I completed `EVIDENCE.md` §1–§10 and the self-check in §12.

The criterion is not "the board is valid". It is "you can say which rule is being enforced, which
function observes it, who calls that function, and what a passing trace does not cover".

## Self-check (binary)

- [ ] I read `board.h` and `board.cpp` and located the three checks by line.
- [ ] I compared the header with `construction-09` and found the one new declaration.
- [ ] I predicted the guard behavior and the checker result before running.
- [ ] I reproduced all 20 traces on the unmodified baseline.
- [ ] I built a probe and saw a fresh board report valid.
- [ ] I explained the scan domain and why `(0, 0)` is outside the active area.
- [ ] I removed the guard in a scratch copy and saw `(0, 0)` make the board invalid.
- [ ] I restored the guard and saw the invalid state rejected.
- [ ] I reproduced 20 of 20 traces with the guard and without it.
- [ ] I showed that `board_reset` does not repair a broken board.
- [ ] I explained what the trace proves and what the checker proves.
- [ ] I completed `EVIDENCE.md`.

These are acceptance criteria. If a box is
unchecked, write the next smallest action in `EVIDENCE.md` §10.

## Misconceptions this lesson refutes

| Misconception | Why it fails | What to do instead |
|---|---|---|
| "The type is opaque, so it must be valid." | Opacity hides the layout. It says nothing about the values. | Separate "cannot see" from "cannot be wrong". |
| "The guards make the checker unnecessary." | Guards stop the writes you know about. The checker reports the state you did not anticipate. | Keep both, and call the checker from a test. |
| "If `board_is_valid` exists, the game uses it." | `grep` finds zero call sites. The game never evaluates the invariant. | Count call sites before trusting a check. |
| "20 of 20 traces prove the invariant." | The traces never call the checker, and they pass with the guard removed. | A trace tests the path it runs, nothing else. |
| "A check that returns true has run." | A function nobody calls returns nothing. | Count call sites, then run the probe. |
| "Row 0 is inside the board because it is inside the array." | The array is 16×16; the active area is 1..8. Storage and validity are different. | Write the predicate as `row < 1 || row > 8`, the same way the guards do. |
| "`board_reset` restores a valid board." | It clears 1..8 only. A mark at 9,9 survives the reset. | Repair at the source, or clear the whole array, and say which you chose. |
| "The checker should fix the board it finds invalid." | A checker that mutates is a second writer, and its caller cannot tell what changed. | Observe in the checker; repair in the operation that broke the rule. |
| "Returning false on `nullptr` makes the function total." | It handles one bad input. It says nothing about a non-null object in a bad state. | State the contract for both cases separately. |
| "If the compiler accepts the loop, the loop is in range." | The compiler does not check array bounds. Only a sanitizer or a reviewer does. | Read the bound, or build once with a sanitizer. |
| "Deleting the checker would break the build." | Nothing calls it. The build stays green. | Treat an unused check as a claim nobody verified. |
| "An invariant is a comment at the top of the file." | A comment cannot fail. A function can. | Make the invariant callable, and call it. |

## Next step

`construction-11` — encapsulation. This lesson made the invariant observable and showed that
observation without a caller is a promise nobody keeps. The next lesson removes the possibility
instead of checking it: the field moves out of reach, every write goes through an operation that
enforces the rule, and the invalid state stops being constructible rather than merely detectable.

See the [lesson catalog](../../../docs/guide/lesson-catalog.md) for the full path.

If either baseline was missing, do not start `construction-11`. The next contrast needs both sides.
