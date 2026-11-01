# Array of records (arreglo de estructuras)

> [English](lesson.md) | [Español](lesson-es.md)

**Concept:** collection and traversal (colección y recorrido) · **Time:** about 62 minutes ·
**Status:** drafted
**Reading:** ~22 minutes

## Why this lesson exists

`construction-04` gave the knight a record and left the board alone. The board is still
`bool visitedCells[9][9]`: 64 truth values, each reached by a pair of indices written at the site
that needs it. The record grouped what belonged to one knight. The board is a collection of things
that each need the same fields, and it never got the same treatment.

The mechanical step is small: declare a record for the cell and change the array's element type.
The lesson is what that changes about the unit. The board stops being a collection of facts and
becomes a collection of cells. Storage and traversal start at the same place.

The sentence to keep:

> An array of records makes the cell the unit of storage and the unit of traversal at once.

Both halves are measurable. The first is a count: one per-cell fact costs one line in the type and
one line at each write site, with zero signatures and zero new arrays. The second is a reading: the
board parameter names an element type and says nothing about which fields a body reads. The record
did not remove the index arithmetic; it removed the second array that had to agree with the first.

## What you will be able to do

By the end of this lesson you can:

- explain collection and traversal over an array of records;
- traverse the board without 64 separate variables.

## The concept: one element, every fact it owns

Five forms of name do the work in this comparison:

```text
fact array      bool visitedCells[9][9];              the element is a truth value
parallel array  bool visitedCells[9][9];
                int  visitedMoveNumbers[9][9];        two elements per cell, one convention
record element  struct BoardCell { bool visited; };  the element is a cell
collection      BoardCell boardCells[9][9];           the board is a collection of cells
traversal       for row ... for column ...            every cell, in one pair of loops
```

The two baselines differ in exactly one decision, and the diff is small: 15 lines added, 10
removed. Five kinds of change:

1. A new element type, five lines.
2. Three signatures change type, with the same arity: `tryMove`, `playTurn`, `playGame`.
3. Three call sites change one argument name, with the same argument count.
4. Three field accesses gain `.visited`: one read, two writes.
5. The declaration in `main` changes type, and so does the call it makes.

Nothing else moves. The random draw order is identical, the control flow is identical, the
transcript is identical, the file still has 19 function definitions and 63 parameters.

Side by side, the difference is one noun:

| Question | `bool visitedCells[9][9]` | `BoardCell boardCells[9][9]` |
|---|---|---|
| What one element is | a truth value | a cell with fields |
| How the visited fact is read | `visitedCells[r][c]` | `boardCells[r][c].visited` |
| How a second per-cell fact is read | a second array, same subscripts | another field, same element |
| What a new per-cell fact costs | a declaration, parameters, call arguments | one line in the type |
| What a traversal iterates over | truth values | cells |
| What keeps two facts on one cell | matching subscripts, by convention | the element itself |
| What the board knows about its size | nothing; the size is in constants | nothing; the size is in constants |

That is the collection version of the claim from `construction-04`. A field does not reach the
signature. A per-cell field does not reach it at all, because the board already crosses every
boundary the field needs to cross.

Three consequences follow:

1. **The fact and the cell cannot drift apart.** One element, one array. A second fact on the
   same cell is another field of the same element, selected by the same pair of coordinates.
2. **The traversal is written once.** Two loops cover 64 cells and name none of them. There is no
   list of 64 variables to keep in step with the board.
3. **A second per-cell fact is one line in the type.** It is not a second array, not a second
   parameter, and not a second call-site argument. A third fact is one more line. The parameter
   total stays at 63.

What the array of records is **not**, in this lesson:

- **Not a container.** `BoardCell boardCells[9][9]` is a raw C array. It does not know its size, it
  does not bounds-check, and it decays to a pointer when passed. The bounds live in two constants
  and in the caller's memory.
- **Not encapsulation.** Any function that holds the board can write any field of any cell.
  `boardCells[0][0].visited = true;` compiles in every function that receives the board. Nothing
  here prevents it.
- **Not representation independence.** The 9×9 shape, the unused row 0 and column 0, and the field
  names are the representation. Flattening the array rewrites every access site.
  `construction-09` prices that change.
- **Not a traversal abstraction.** You still write both loops and both bounds. `construction-17`
  replaces the loops with an algorithm; this lesson only makes the loop possible once.

## Before you start

- [`construction-04`](../construction-04-records/lesson.md) — you have its record, its two counts
  (signature changes and call sites for `captured`), and its distinction between what a record
  localizes and what it does not.
- A C++20 compiler (`g++` or `clang++`). `cmake` is optional.

You do not need to know C++ well. You need to read an array declaration, an element type, a
subscript pair (`boardCells[row][column]`) and a nested `for`. That is the whole skill.

### What you need

- The array-of-records baseline: [`code/board-cells-baseline.cpp`](code/board-cells-baseline.cpp).
- The records baseline: [`records-baseline.cpp`][records-baseline].
- Build scripts and notes: [`run.sh`](run.sh) and [`README.md`](README.md).
- The rules: [`common/spec/game-rules.md`](../../../common/spec/game-rules.md) — §12 fixes the
  order of random draws.
- The transcript format:
  [`common/spec/transcript-format.md`](../../../common/spec/transcript-format.md).
- One golden trace: [`common/traces/seed-0001.txt`](../../../common/traces/seed-0001.txt).
- This learner template: [`EVIDENCE.md`](EVIDENCE.md).

Check that the artifacts are present before you begin, from this lesson directory:

```bash
test -f code/board-cells-baseline.cpp &&
test -f ../construction-04-records/code/records-baseline.cpp &&
test -f ../../../common/traces/seed-0001.txt
```

If either baseline is missing, the comparison has no second side. Record the missing file in
`EVIDENCE.md` §11 and return when it is available. A count against a file you have not read is not
evidence.

Both baselines reproduce all 20 traces in `common/traces/` byte for byte, before you change
anything. Same game, two element types. That is what makes the comparison fair.

## Step 1 — Read the board and its clients (~10 min)

Read both files once. Then let the text tell you where the element type appears:

```bash
grep -n 'BoardCell' code/board-cells-baseline.cpp
grep -n '\.visited' code/board-cells-baseline.cpp
grep -n 'boardCells' code/board-cells-baseline.cpp
```

The first command prints five lines: the type definition, three parameters, one declaration. The
second prints three field accesses. Fill this table into `EVIDENCE.md` §2.1 while you read.

| Function | Line | Receives | Fields it reads | Fields it writes |
|---|---|---|---|---|
| `tryMove` | 218 | `BoardCell[][9]` | `visited` | — |
| `playTurn` | 241 | `BoardCell[][9]` | through `tryMove` | `visited` |
| `playGame` | 310 | `BoardCell[][9]` | — | — |
| `main` | 328 | — | — | declares and passes |

Two rows are worth a minute.

- `playGame` receives the board and reads no field. It is a pass-through, exactly as it was for the
  two knights in `construction-04`. The element type change did not give it an opinion about
  cells. Its signature line changed type and nothing else.
- `main` declares the board and never touches a field. It only hands the board over. The fields are
  known in exactly two functions, while four names mention the type.

Then answer in `EVIDENCE.md` §2:

1. How many functions receive the board? How many read a field? How many write one?
2. Which function receives the board and reads no field of any cell?
3. Which call sites pass the board? Name the caller and the callee of each.
4. What does `bool visitedCells[9][9]` say that `BoardCell boardCells[9][9]` does not, and the
   other way around?
5. Why is the first dimension 9 when the board has 8 rows? How many elements does the array have,
   and how many of them are board cells?
6. If `BoardCell` gained a third field, which of the 19 definitions would fail to compile? Check
   before you answer; the number is the point.

Question 6 has the answer you will measure in Step 4. Question 4 has the answer you will argue in
Step 7.

## Step 2 — Predict before you measure (~8 min)

Three predictions, all written before any run: one about the field, one about the traversal, one
about the alternatives.

### 2.1 The change under test

The requirement, fixed for both baselines:

> **R-LASTVISIT — record when each cell was visited.** Every cell that becomes visited stores the
> number of the successful move that visited it. No function reads the new field yet, and the game's
> output does not change. A traversal over the board returns the highest stored move number.

The clause that says nothing reads the field is the one that keeps the change honest. The trace
check can prove the game did not change; it cannot prove the field holds the right value, because
the game does not print it. Step 5 shows what can.

For the array-of-records file, predict before editing:

- Field declaration sites:
- Assignment sites, and which branches of `playTurn`:
- Signatures that change:
- Call sites that change:
- New arrays:
- New globals:
- Lines added:
- Traces that differ, of 20:
- Lines in the traversal function:
- Does the traversal's number equal `moves=` on all 20 seeds? (yes / no)
- Confidence, 0–100 %:

The argument you are testing: `playTurn` already holds `boardCells` and `successfulMoveCount` at
the moment it marks a cell. The fact you need to record is one line away from the mark. Nothing
has to be passed that is not already there.

### 2.2 The same fact without the record

Both alternatives start from the **records baseline** of `construction-04`, whose board is still
`bool visitedCells[9][9]`. Fix the units first; they are the same five as in `construction-04`,
plus one.

- An **edit site** is a contiguous source change the requirement forces: a declaration, an
  assignment, a signature line, a call-site line, or a guard block with its transition.
- A **signature change** is a function whose parameter list or return type changes. Count it once,
  separate from the call sites it forces.
- A **call site** is a place where a call expression must be edited because a signature changed.
- A **caller** is a function that contains a call site. One caller can hold several call sites.
- A **new global** is a mutable name added at namespace scope.
- A **new array** is a second collection added to hold one fact per cell.
- Do **not** count comments, blank lines, formatting, generated traces, or the number of turns.

| Metric | Field in `BoardCell` | Parallel `int` array | 64 `int` names |
|---|---|---|---|
| Edit sites | | | |
| Signature changes | | | |
| Call sites changed | | | |
| New arrays | | | |
| New globals | | | |
| Lines added | | | |
| Golden traces broken (of 20) | | | |
| How a cell is identified | | | |

Commit to a number in every cell. "The record version is cleaner" is not a prediction; it does not
say which row moves.

### 2.3 Which of the three comparisons is fair

The 64-name version is not a serious contender. It is the rung below the array, included so you can
say why the array exists at all. The real comparison is the field against the parallel array.
Answer now, before measuring:

- In the record version, how many subscript pairs identify both facts of one cell?
- In the parallel version, how many subscript pairs identify both facts of one cell?
- What makes the two parallel subscript pairs refer to the same cell: the language, or a
  convention?
- If a write site forgets the second array, where does the bug appear first: the transcript, or the
  traversal?
- If a write site swaps the two subscripts in the second array, what does the compiler say?

Write the answers in `EVIDENCE.md` §3.4. Step 5 checks the last two.

## Step 3 — Run both baselines (~5 min)

Create the scratch directory first; every command below writes into it.

```bash
mkdir -p /tmp/array-test
./run.sh --seed 1 --name Learner > /tmp/array-test/board-cells-0001.txt
diff -u ../../../common/traces/seed-0001.txt /tmp/array-test/board-cells-0001.txt
```

Then the records baseline:

```bash
cd ../construction-04-records
./run.sh --seed 1 --name Learner > /tmp/array-test/records-0001.txt
diff -u ../../../common/traces/seed-0001.txt /tmp/array-test/records-0001.txt
```

Then the two against each other:

```bash
diff -u /tmp/array-test/records-0001.txt /tmp/array-test/board-cells-0001.txt
```

Three empty diffs. Return to this lesson directory and confirm all 20 seeds, not only the first:

```bash
mkdir -p /tmp/array-test
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
  -o build/board-cells-baseline code/board-cells-baseline.cpp
```

If a diff is not empty, stop. Record it in `EVIDENCE.md` §11. Do not adjust a trace to match the
code.

## Step 4 — Add the field and record the move (~10 min)

Copy the baseline. The tracked file stays untouched.

```bash
mkdir -p /tmp/array-test
cp code/board-cells-baseline.cpp /tmp/array-test/board-cells-move-number.cpp
```

Three edits. The first is the type:

```cpp
struct BoardCell
{
    bool visited;
    int moveNumber;
};
```

The second is the capture branch of `playTurn`, after the cell is marked visited and after the
successful count is incremented:

```cpp
            boardCells[destinationRow][destinationColumn].visited = true;
            boardCells[destinationRow][destinationColumn].moveNumber = successfulMoveCount;
```

The third is the legal branch, in the same order:

```cpp
        boardCells[destinationRow][destinationColumn].visited = true;
        boardCells[destinationRow][destinationColumn].moveNumber = successfulMoveCount;
```

Build and run all 20 seeds:

```bash
g++ -std=c++20 -Wall -Wextra -Werror -O2 \
  -o /tmp/array-test/board-cells-move-number /tmp/array-test/board-cells-move-number.cpp
for trace in ../../../common/traces/seed-*.txt; do
  traceName="$(basename "${trace}" .txt)"
  seedText="${traceName#seed-}"
  seedNumber="$((10#${seedText}))"
  /tmp/array-test/board-cells-move-number --seed "${seedNumber}" --name Learner | cmp - "${trace}"
done
```

20 matches, no output. The diff against the tracked file is three added lines and zero removed:

```bash
diff -u code/board-cells-baseline.cpp /tmp/array-test/board-cells-move-number.cpp
```

Four observations, each one part of the answer.

**One — no signature moved.** Compare the three board parameters; their arity is unchanged. The
parameter total is still 63 across 19 definitions. The compiler had nothing new to check because
nothing was restated. Confirm with the edit list, not with the absence of errors:

```bash
grep -n 'moveNumber' /tmp/array-test/board-cells-move-number.cpp
```

The field is declared once and assigned twice. The other hits are the existing parameter name
`moveNumber`, which already means something else in this file. See the naming note below.

**Two — the trace cannot see the field.** Change both assignments to a constant, rebuild, run 20
seeds. All pass, because nothing reads the field. The trace checks the game; it does not check the
fact. Write that down: a golden trace is a behavior contract, not a data contract.

**Three — the field has two writers, not one.** A cell becomes visited in the capture branch and in
the legal branch, and both branches increment the successful count. If you edited only one branch,
the traces still pass and the field is wrong on some cells. Step 5 turns "some cells" into a number.

**Four — no new name at namespace scope.** The type is at namespace scope; it is not data.

```bash
g++ -std=c++20 -c -o /tmp/array-test/board-cells.o \
  /tmp/array-test/board-cells-move-number.cpp
nm -C --defined-only -g /tmp/array-test/board-cells.o | grep ' [BbDd] '
```

Still one line, `randomState`. `sizeof(BoardCell)` goes from 1 to 8: four bytes for the `int` on
this platform, one for the `bool`, and three bytes of padding so the next element starts aligned.
The 9×9 board goes from 81 to 648 bytes. Nothing in the transcript depends on that, and nothing in
this lesson depends on the padding. `construction-19` is where the layout becomes the subject.

**Naming note.** The field is called `moveNumber` because the requirement says so. In this file that
name already carries two meanings: `tryMove`'s parameter is the attempted move order, 1 to 8, and
`printMove`'s parameter is the successful move count, 1 to 200. The field joins the second meaning.
The member access makes the code legal and the reader's job harder. If you were naming it yourself,
`visitedOnMove` would say which of the two it is.

Write `EVIDENCE.md` §5: the exact commands, the diff, which of your §2.1 predictions held, and
which one missed. The usual miss is the writer count: learners predict one assignment, because a
move looks like one event, and forget that a capture also marks a cell.

## Step 5 — Traverse the board (~10 min)

Now the question the field was added for. One function, two loops:

```cpp
int lastRecordedMoveNumber(const BoardCell boardCells[][boardColumnCount + 1])
{
    int highestMoveNumber = 0;
    for (int rowIndex = 1; rowIndex <= boardRowCount; rowIndex += 1)
    {
        for (int columnIndex = 1; columnIndex <= boardColumnCount; columnIndex += 1)
        {
            if (boardCells[rowIndex][columnIndex].moveNumber > highestMoveNumber)
            {
                highestMoveNumber = boardCells[rowIndex][columnIndex].moveNumber;
            }
        }
    }
    return highestMoveNumber;
}
```

Four decisions in fifteen lines.

- **The parameter is `const` and names the second dimension.** A two-dimensional array parameter
  must state the row width so the compiler can compute the stride. `const` says the traversal reads
  and never writes. The first dimension is free to be unsized.
- **The loops run from 1 to 8.** The array has nine rows and nine columns; only 1 through 8 are
  board cells. Row 0 and column 0 exist so that a 1-based coordinate can be used as an index
  directly. Starting at 0 would read the unused border, and starting at 0 and ending at 7 would
  miss the last row and column. The bound is the board size, not the array size.
- **The default is 0.** An untouched board returns 0. That is not a move number; it is the only
  value available when no cell was visited. The return type has no way to say "no move". Record
  that gap; `construction-22` is about contracts like this one.
- **The scan does not stop early.** You cannot know the maximum until you have seen every cell, and
  the board does not store the move order anywhere else. 64 comparisons is the honest cost.

To check the traversal without changing the transcript, add that function above `playGame` in the
same scratch file and print to standard error:

```cpp
    std::fprintf(stderr, "last_recorded=%d moves=%d\n",
                 lastRecordedMoveNumber(boardCells), successfulMoveCount);
```

Call it at the end of `playGame`, after the two existing `printf` calls. Build and run seed 1:

```bash
g++ -std=c++20 -Wall -Wextra -Werror -O2 \
  -o /tmp/array-test/board-cells-traversal /tmp/array-test/board-cells-move-number.cpp
/tmp/array-test/board-cells-traversal --seed 1 --name Learner > /dev/null
```

```text
last_recorded=21 moves=21
```

Then all 20 seeds, stdout against the golden traces and stderr against the transcript's `moves=`
line:

```bash
for trace in ../../../common/traces/seed-*.txt; do
  traceName="$(basename "${trace}" .txt)"
  seedText="${traceName#seed-}"
  seedNumber="$((10#${seedText}))"
  /tmp/array-test/board-cells-traversal --seed "${seedNumber}" --name Learner \
    2>&1 >/dev/null
  /tmp/array-test/board-cells-traversal --seed "${seedNumber}" --name Learner \
    2>/dev/null | cmp - "${trace}"
done
```

The traversal returns the same number the transcript already prints as `moves=`, on all 20 seeds.
That is not a coincidence. Each accepted move increments `successfulMoveCount` exactly once and
writes that value to exactly one cell, so the maximum stored value is the last successful move
number. The maximum is also unique: two cells cannot hold the same move number.

Which raises the honest question: if the number was already in the transcript, what did the
traversal add? The **cell**. The number is redundant; the coordinates of the cell with the highest
number are not printed anywhere. Add two out-parameters and the same traversal returns both:

```cpp
int lastRecordedMoveNumber(const BoardCell boardCells[][boardColumnCount + 1],
                           int &lastRow, int &lastColumn)
{
    int highestMoveNumber = 0;
    lastRow = 0;
    lastColumn = 0;
    for (int rowIndex = 1; rowIndex <= boardRowCount; rowIndex += 1)
    {
        for (int columnIndex = 1; columnIndex <= boardColumnCount; columnIndex += 1)
        {
            if (boardCells[rowIndex][columnIndex].moveNumber > highestMoveNumber)
            {
                highestMoveNumber = boardCells[rowIndex][columnIndex].moveNumber;
                lastRow = rowIndex;
                lastColumn = columnIndex;
            }
        }
    }
    return highestMoveNumber;
}
```

For seed 1 it returns `21` at `4,1`, and `4,1` is the destination of the last `move=` line in
`seed-0001.txt`. That holds for all 20 seeds. The full table belongs in `EVIDENCE.md` §6.1.

### 5.1 What the traversal can prove

Two experiments, both on scratch copies.

**Forget one writer.** Remove the assignment from the capture branch, rebuild, run 20 seeds. All 20
traces still pass. The traversal now disagrees with `moves=` on 11 seeds and agrees on 9. The split
is exact: 11 games end in a capture, 9 end in a trap. When the last move is a capture, the last cell
never received its number, and the maximum falls back to the previous move.

**Write at the wrong cell.** Swap the two subscripts in both assignments, so the number is stored at
`[column][row]` instead of `[row][column]`. Rebuild, run 20 seeds. All 20 traces still pass. The
traversal that returns only the maximum also reports the right number on all 20, because permuting
the cells does not change the set of values. The traversal that returns the coordinates reports the
wrong cell on 14 of 20; the 6 that survive have their last cell on the diagonal, where the transpose
is the same cell.

The second result is the one to keep. **The maximum is invariant under any permutation of the
cells.** A traversal that returns only a value cannot tell you where the value was stored. To test
the storage, return the coordinates and compare them with the last `move=` line.

So the three checks prove different things:

| Check | What it proves | What it cannot see |
|---|---|---|
| Golden trace | The game's output did not change | A wrong or missing field |
| Value traversal | The maximum value was written somewhere | A value written on the wrong cell |
| Coordinate traversal | The maximum is on the right cell | A field written twice, or a wrong source value |

Nothing in this lesson proves that each cell was written exactly once. That needs an invariant, and
invariants arrive in `construction-10` and `construction-11`. What you can say today is smaller and
true: the fact lives on the cell, the traversal reads the cell, and the coordinates are checkable.

Write `EVIDENCE.md` §6: the function, the 20 results, the two experiments, and the answers to the
questions below.

1. Why do the loops start at 1 and not at 0?
2. Why does the maximum equal `moves=` on every seed?
3. Why is the maximum unique?
4. What does an untouched board return, and what does that value mean?
5. Why can the traversal not stop at the first cell it finds?
6. What does the traversal add that the transcript did not already print?
7. Which write site is easy to forget, and why does the trace not catch it?
8. Why does a transposed write keep the value correct and the coordinates wrong?
9. Which of the three checks would catch a write that stored the attempted move number (1 to 8)
   instead of the successful move number?

## Step 6 — The same fact without the record (~10 min)

Now the other side. Copy the records baseline to a second scratch file:

```bash
cp ../construction-04-records/code/records-baseline.cpp \
  /tmp/array-test/records-parallel.cpp
```

The fact still has to be reachable where a cell becomes visited: inside `playTurn`, at its two
write sites. Add one parallel array and thread it to those sites.

1. In `main`, next to the boolean board:
   `int visitedMoveNumbers[boardRowCount + 1][boardColumnCount + 1] = {};`
2. `playTurn` gains `int visitedMoveNumbers[][boardColumnCount + 1]`.
3. `playGame` gains the same parameter and forwards it.
4. Each write site gains one line:
   `visitedMoveNumbers[destinationRow][destinationColumn] = successfulMoveCount;`
5. The call in `main` passes the new array.

`tryMove` does not change. It only reads `visitedCells`, so the new fact never has to reach it.
That is the smallest reach the parallel version can have, and it is still two signatures and two
call sites.

Build and run all 20 seeds as in Step 4. They pass. The game did not change; only the shape of the
fact changed. Then count what the change reached:

| Metric | Field in `BoardCell` | Parallel `int` array |
|---|---|---|
| Edit sites | 3 | 7 |
| Signature changes | 0 | 2 (`playTurn`, `playGame`) |
| Call sites changed | 0 | 2 |
| New arrays | 0 | 1 |
| New globals | 0 | 0 |
| Lines added / removed | 3 / 0 | 10 / 5 |
| Golden traces broken (of 20) | 0 | 0 |
| Which element the subscripts select | one element, both facts | two arrays, one convention |

The record's three edit sites are the field and the two assignments. The parallel version's seven
are the declaration, the two signature lines, the two call-site lines, and the two assignments.
The traversal in the parallel version needs the second array as a parameter, which is one more
signature and one more call site if you call it from anywhere but `main`.

Now the cheaper-looking placement. Make `visitedMoveNumbers` a namespace-scope global. Drop the
two parameters and the two call-site arguments. Rebuild, run 20 seeds. They pass. The count drops
to three edit sites, zero signature changes, zero call sites, and one new mutable global. The cost
did not disappear; it moved out of the signatures and into a name that every function can reach
without saying so. That is the same trade `construction-04` priced for the capture flags.

### 6.1 The 64-name version

If you refuse both collections, the fact needs 64 `int` variables beside the 64 truth values. A name
cannot be computed from `destinationRow` and `destinationColumn`, so each write site becomes a
decision over 64 names: a `switch` with 64 cases, or an `if` chain of the same length. The
traversal becomes 63 comparisons written by name. Passing the set to a function needs 64 parameters,
or the global you were trying to avoid.

The counts are not the point; the shape is. The moment the write can be selected by a computed
coordinate, you have invented the array. The moment the second fact gets its own array, you have
invented the parallel array. The record is the next rung, not a different ladder.

Answer these in `EVIDENCE.md` §7.4:

- Why does the parallel version need only two signature changes and not three?
- What does the compiler check about the two parallel subscript pairs? What does it not check?
- Why can a 64-name version not be selected by `destinationRow` and `destinationColumn`?
- Which version can cross a call boundary as one argument?
- On which row does the parallel version win, if any, and why is that not the whole comparison?
- What would a third per-cell fact cost in each version?

The last question is the one that decides the design. For the record it is one line in the type and
one line at each write site. For the parallel array it is another declaration, two more signatures,
two more call sites, and another array that must agree with the first two. For 64 names it is
another 64 declarations and another 64-way dispatch.

### 6.2 The honest comparison

Do not claim the array of records is cheaper overall. Converting the `construction-04` board to
`BoardCell` cost 15 added and 10 removed lines and bought nothing observable. If you only ever
needed the last-visit number once, the parallel array is cheaper from the `construction-04`
baseline: 10 added and 5 removed lines against 25 changed lines plus the field. The record wins
**per fact added after the board is already a collection of cells**, and it wins the attachment:
one element, one array, one place where the fact can be read.

That is the same structure as `construction-04`. The record's advantage is marginal cost and
attachment, not the first conversion. Price the change you are making, then the next one.

## Step 7 — Compare and explain (~7 min)

Write `EVIDENCE.md` §8. Two parts.

**First, one sentence.** Say what the array of records decides about a per-cell fact, without using
the word *struct* and without a verdict word: *better*, *worse*, *clean*, *cleaner*, *nice*,
*elegant*, *ugly*, *modular*. If your sentence needs one of those, you have described your reaction
to the file rather than the count.

**Second, one paragraph.** Answer:

- What the element type decides: what one cell is, what fields it owns, and what its default is.
- What the traversal decides: which cells are visited, in what order, and where it stops.
- What the array of records localizes: the field set, the zero default, the one array, and the
  attachment of a fact to the cell it describes.
- What it does **not** localize: the writers, the bounds, the representation, and the invariant that
  each cell is written once.
- What a reader must hold in working memory to know which fields a function uses, in each file.
- What a golden trace, a value traversal and a coordinate traversal can each prove.
- Which later lesson fixes each remaining gap. There are four: naming the cell once
  (`construction-06`), representation and bounds (`construction-09`), invariants and access
  (`construction-10`, `construction-11`), and the loop itself (`construction-17`).

A strong answer keeps two things apart: **the cost of adding a fact** and **what the fact is
attached to**. The record wins the first at the margin and the second outright. It loses the
conversion cost and it does not remove a single subscript. A weak answer says the record version is
cleaner, which hides both numbers.

## Step 8 — Acceptance check (~2 min)

The acceptance criterion for this lesson is: **traverse the board with two loops, find the last move
number, explain why no parallel array or 64 variables are needed, and keep all 20 traces passing.**

Check your record against it:

- [ ] I ran both unmodified baselines and got three empty diffs, plus 20 matching seeds.
- [ ] I inventoried the three functions that receive the board and the two that touch a field.
- [ ] I predicted the field cost and the writer count before editing.
- [ ] I added `moveNumber` to `BoardCell` and recorded it in both branches of `playTurn`.
- [ ] All 20 traces still passed after the field was added.
- [ ] I wrote one two-loop traversal and it returned the same number as `moves=` on all 20 seeds.
- [ ] I showed that a forgotten capture write passes the trace and fails the traversal on 11 seeds.
- [ ] I showed that a transposed write keeps the value and moves the coordinates on 14 seeds.
- [ ] I built the parallel-array variant and counted its signatures and call sites.
- [ ] I explained why 64 names cannot be selected by a computed coordinate.
- [ ] I explained what the record localizes and what it does not.
- [ ] My one-sentence explanation avoids *struct* and the eight verdict words.
- [ ] I did not edit either tracked baseline.
- [ ] I completed `EVIDENCE.md` §1–§10 and the self-check in §12.

The criterion is not "the array of records wins". It is "you can say exactly what a per-cell fact
costs, what it is attached to, and which check would notice if it were wrong".

## Self-check (binary)

- [ ] I read both baselines and inventoried which function touches the board and which field.
- [ ] I predicted the field cost, the traversal result and both alternatives before measuring.
- [ ] I built both baselines and reproduced all 20 traces.
- [ ] I added `moveNumber` to a scratch copy and kept the traces passing.
- [ ] I wrote the traversal and checked its value and its cell against the transcript.
- [ ] I applied the same fact to the records baseline with a parallel array and counted it.
- [ ] I compared the prediction with the counts and named the misses.
- [ ] I explained what the array of records localizes and what it does not.
- [ ] I completed `EVIDENCE.md`.

These are acceptance criteria. If a box is
unchecked, write the next smallest action in `EVIDENCE.md` §10.

## Misconceptions this lesson refutes

| Misconception | Why it fails | What to do instead |
|---|---|---|
| "An array of records is a record with an index." | The index selects a cell; the record says what a cell is. Those are two decisions, and only the second one changed here. | Compare one array with two. |
| "The record removes the index arithmetic." | Every access still spells `boardCells[row][column]`. The record removes the *second* array, not the index. | Count how many arrays a write site touches: one here, two there. |
| "If the traces pass, the new field is correct." | An unread field cannot change the output. A forgotten writer also passes all 20 traces. | Compare the traversal with `moves=`; 11 of 20 seeds catch it. |
| "The maximum proves the cells are right." | The maximum is invariant under any permutation of the cells. A transposed write keeps it. | Return the coordinates; 14 of 20 seeds catch the transpose. |
| "A parallel array is wrong." | It passes all 20 traces and is a legitimate second collection. It is a convention, not a bug. | Price the next per-cell fact. |
| "64 variables are just an array with names." | A name cannot be computed from a row and a column. The write becomes a 64-way decision. | Try selecting one by coordinate without an array. |
| "The board knows its own size." | A raw array does not. The size lives in two constants and in every caller that passes it. | Count elements against board cells; then read `construction-09`. |
| "The record makes the board safe." | Any holder can write any field of any cell. `boardCells[0][0].visited = true;` compiles. | That is `construction-11`'s job. |
| "The traversal is generic." | The bounds are hard-coded to 1..8 and the field names are fixed. | `construction-17` replaces the loops. |
| "The field name is free." | `moveNumber` already means the attempted move order in `tryMove` and the successful count in `printMove`. | Name the meaning, not the number. |

## Next step

`construction-06` — references. The write site still spells
`boardCells[destinationRow][destinationColumn]` twice, once per field. A reference names that cell
once and both writes go through one name. That is the next rung: the cell is already the unit of
storage and traversal; the reference makes it a unit of *access*.

After that, `construction-07` moves the same traversal to a different layout, and `construction-09`
gives the board its own bounds so the `1..8` stops being a fact the caller must remember.

See the [lesson catalog](../../../docs/guide/lesson-catalog.md) for the full path.

If either baseline was missing, do not start `construction-06`. The next contrast needs both sides.

[records-baseline]: ../construction-04-records/code/records-baseline.cpp
