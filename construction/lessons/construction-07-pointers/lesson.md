# Pointers (punteros)

> [English](lesson.md) | [Español](lesson-es.md)

**Concept:** indirection and lifetime (indirección y ciclo de vida) · **Time:** about 88 minutes ·
**Status:** drafted · **Prerequisites:** construction-06
**Reading:** ~17 minutes

## Why this lesson exists

`construction-06` gave the selected knight a name that could not be null, could not be reseated, and
could not be seen at the call site. That name was enough to move a knight, but it hid two facts:
where the record lives, and how long it lives.

The pointers baseline replaces every knight reference with a pointer, adds one function that returns
the address of a board cell, and leaves the game untouched. All 20 traces stay byte-identical. The
diff is 40 lines added and 34 removed, net 6; one function appears (`getBoardCell`), 22 definitions
become 23, and 72 parameters become 75.

The sentence to keep:

> A value that names a location may be empty, may be redirected, and may outlive the thing it
> names; the type says what is there, not whether it is still there.

Two measurements follow. The first is structural: rewrite one board access as `rowStart + column`,
and the 20 traces stay identical because the address is the same. The second is arithmetic: cell
(2,3) sits 21 `BoardCell`s from (0,0) when the row width is 9, and 25 when it is 11. A stride
written as a literal `9` agrees with the first layout and silently misreads the second.

One honest note before the comparison. The previous lesson promised that the call site would show
the `&`. It does where the address is taken, in `main`:

```cpp
initializePositions(&playerKnight, &machineKnight);
```

It does not inside `playTurn`, where the address is already held:

```cpp
moveKnight(activeKnight, destinationRow, destinationColumn);
```

That line looks exactly like the reference version. A pointer makes the indirection visible where
the address is created, not at every use. The other honest note: pointers were already in this
file. `char **argumentValues`, `const char *activeKnightName` and
`BoardCell boardCells[][boardColumnCount + 1]` are pointer types, and the last one is a pointer to a
row. This lesson does not introduce the `*` token. It moves the knight aliases and one board access
to the pointer side, and makes the layout and the lifetime visible.

## What you will be able to do

By the end of this lesson you can:

- explain indirection, lifetime and the row stride;
- access a board cell through a pointer and keep the traces unchanged.

## The concept: a value that names a location

Three forms name a knight, and one new form names a board cell:

```text
value parameter      void moveKnight(Knight knight, ...)        the callee owns a copy
reference parameter  void moveKnight(Knight &knight, ...)       another name; must bind
pointer parameter    void moveKnight(Knight *knight, ...)       a location; may be null
pointer return       Knight *selectActiveKnight(...)            the caller holds a location
cell return          BoardCell *getBoardCell(...)               a location inside the board
```

Side by side, the sites that name the knight or the cell:

| Decision | `construction-06` | `construction-07` |
|---|---|---|
| Active knight | `Knight &` returned by `selectActiveKnight` | `Knight *` returned by `selectActiveKnight` |
| Other knight | `const Knight &` returned by `selectOtherKnight` | `const Knight *` returned by `selectOtherKnight` |
| Move | `moveKnight(Knight &, ...)` | `moveKnight(Knight *, ...)` |
| Board cell | `boardCells[row][column]` | `getBoardCell(...)` returning `BoardCell *` |

The two baselines differ in one decision: how the selected knight and the board cell are named. Ten
signatures change a knight parameter or return type, and none changes its parameter count. Three
call sites gain an `&`; three board expressions gain an accessor call. No existing argument is
reordered and no existing body statement disappears except the direct board index. The transcript
does not move.

Side by side, the difference is what the type permits:

| Question | `Knight &` | `Knight *` |
|---|---|---|
| What the parameter holds | another name | a location |
| A write inside the function | changes the caller's record | changes the pointee |
| What the caller sees after the call | the new field values | the new field values |
| What the call site says about mutation | nothing | `&` where the address is taken, nothing after |
| Can the parameter be null | no | yes |
| Can it be reseated later | no; assignment copies the record | yes; assignment redirects the name |
| What the call costs | a name passed | an address passed |
| What the compiler checks | binding and `const` | pointee compatibility and `const` |
| What the compiler cannot check | lifetime | null, bounds, lifetime across scopes |

What a pointer is **not**, in this lesson:

- **Not an integer.** It is a typed location. `rowStart + 1` moves by `sizeof(BoardCell)`, not by
  one byte.
- **Not always valid.** Null is representable, and so is a location that has stopped being valid.
- **Not an owner.** Nothing here allocates or deletes. Ownership is `construction-16`.
- **Not bounds-checked.** `rowStart + 9` compiles even though the row has indices `0..8`.
- **Not a reference.** It can be compared, copied, reseated and set to null.
- **Not visible at every call.** The `&` appears where the address is taken; after that, the pointer
  variable carries it silently.
- **Not a safety feature.** It gives the callee every field of the pointee and adds no invariant.
  That starts in `construction-10` and `construction-11`.

Three consequences follow:

1. **The selection has one home.** `selectActiveKnight` returns a location, so `playTurn` does not
   repeat the ternary that chooses between the two records.
2. **The write has one home.** `moveKnight` writes both fields through the pointer.
3. **The board access has one home.** `getBoardCell` computes the address once, and the rest of the
   game reads or writes `->visited`.

The cost is the fourth thing: null, reseating and lifetime become the programmer's job. The type
says what is at the location; it does not say whether the location is valid.

## Before you start

- [`construction-06`](../construction-06-references/lesson.md) — you have its reference aliases, its
  `const` boundary, and its by-value experiment.
- A C++20 compiler (`g++` or `clang++`). `cmake` is optional.

You do not need to know C++ well. You need to read a parameter list, a return type, and an
expression with `*` or `->`. The lesson is about what those marks permit, not about their spelling.

### What you need

- The pointers baseline: [`code/pointers-baseline.cpp`](code/pointers-baseline.cpp).
- The references baseline:
  [`references-baseline.cpp`](../construction-06-references/code/references-baseline.cpp).
- Build scripts and notes: [`run.sh`](run.sh) and [`README.md`](README.md).
- The rules: [`common/spec/game-rules.md`](../../../common/spec/game-rules.md) — §12 fixes the
  order of random draws.
- The transcript format:
  [`common/spec/transcript-format.md`](../../../common/spec/transcript-format.md).
- One golden trace: [`common/traces/seed-0001.txt`](../../../common/traces/seed-0001.txt).
- This learner template: [`EVIDENCE.md`](EVIDENCE.md).

Check that the artifacts are present before you begin, from this lesson directory:

```bash
test -f code/pointers-baseline.cpp &&
test -f ../construction-06-references/code/references-baseline.cpp &&
test -f ../../../common/traces/seed-0001.txt
```

If either baseline is missing, the comparison has no second side. Record the missing file in
`EVIDENCE.md` §11 and return when it is available. A count against a file you have not read is not
evidence.

Both baselines reproduce all 20 traces in `common/traces/` byte for byte, before you change
anything. Same game, two ways of naming the knight and the cell. That is what makes the comparison
fair.

## The layout: row-major, nine cells wide

The board is declared with a sentinel row and a sentinel column:

```cpp
BoardCell boardCells[boardRowCount + 1][boardColumnCount + 1] = {};
```

With `boardRowCount` and `boardColumnCount` at 8, that is 9 rows of 9 cells: 81 cells, of which 64
are used (rows `1..8`, columns `1..8`). Row 0 and column 0 exist so that the game can number cells
from 1 without subtracting one at every access.

```text
index:      0      1      2     ...     8
row 0:   [0,0]  [0,1]  [0,2]   ...   [0,8]    sentinel row
row 1:   [1,0]  [1,1]  [1,2]   ...   [1,8]
row 2:   [2,0]  [2,1]  [2,2]   ...   [2,8]
...
row 8:   [8,0]  [8,1]  [8,2]   ...   [8,8]
```

The cells sit in one contiguous block, row after row. The address of `(row, column)` is:

```text
base + (row * 9 + column) * sizeof(BoardCell)
```

That `9` is the row width, not the column count. It is `boardColumnCount + 1`, and it is the stride
between one row and the next. This is what **row-major** means: the row index moves in whole rows,
the column index moves one cell inside the current row.

Two expressions expose the layout:

```cpp
BoardCell *rowStart = boardCells[row];      // a BoardCell[9] decays to BoardCell *
BoardCell *cell = rowStart + column;        // column cells forward, not column bytes
```

`boardCells[row]` is not a cell; it is a row, and it decays to a pointer to its first cell.
`rowStart + column` is the address of `boardCells[row][column]`, because pointer arithmetic counts
elements of the pointee type.

The parameter type keeps the width:

```cpp
BoardCell boardCells[][boardColumnCount + 1]
```

An array parameter decays, but only its outermost dimension. What the function receives is
`BoardCell (*)[9]`: a pointer to a row of 9 cells. Arithmetic on that type moves a whole row. That
is why a call with a differently sized array is rejected by the compiler.

`getBoardCell` returns `BoardCell *`. That return type drops the width. From the returned pointer,
`+1` is the next column, but `+9` is the next row only while the width is 9. The accessor hides the
two-dimensional indexing and, with it, the stride.

## Step 1 — Inventory the pointers (~12 min)

Read both files once. Then let the text tell you where a knight or a cell is named by a location:

```bash
grep -n 'Knight \*' code/pointers-baseline.cpp
grep -n 'const Knight \*' code/pointers-baseline.cpp
grep -n 'BoardCell \*\|getBoardCell' code/pointers-baseline.cpp
```

The first command also matches the `const Knight *` lines; the second prints the read-only subset.
The third prints the accessor and its call sites.

Then read the whole change in one view:

```bash
diff -u ../construction-06-references/code/references-baseline.cpp \
  code/pointers-baseline.cpp
```

The change has five kinds of hunk: the knight parameter types, the `.` to `->` rewrites, the new
accessor, the two board writes that became calls, and the three `&` added in `main`.

Fill this table into `EVIDENCE.md` §2.1 while you read. Line numbers are from the pointers baseline.

| Function | Line | Knight parameters | Form | Reads | Writes |
|---|---|---|---|---|---|
| `initializePositions` | 125 | 2 | `Knight *` | — | `row`, `column` |
| `printGameHeader` | 149 | 2 | `const Knight *` | `row`, `column` | — |
| `printTurnHeader` | 171 | 1 | `const Knight *` | `row`, `column` | — |
| `printMove` | 206 | 1 | `const Knight *` | `row`, `column` | — |
| `selectActiveKnight` | 218 | 2 | `Knight *`, returns `Knight *` | — | — (grants write access) |
| `selectOtherKnight` | 223 | 2 | `const Knight *`, returns `const Knight *` | — | — |
| `moveKnight` | 229 | 1 | `Knight *` | — | `row`, `column` |
| `getBoardCell` | 235 | 0 | returns `BoardCell *` | the cell address | — |
| `tryMove` | 240 | 2 | `const Knight *` | `row`, `column`; cell | — |
| `playTurn` | 265 | 2 | `Knight *` | through helpers | through `moveKnight` and `getBoardCell` |
| `playGame` | 334 | 2 | `Knight *` | — | forwards |
| `main` | 352 | 0 | declares two records | — | initializes |

Counts: 9 mutable knight parameters, 8 read-only ones, 0 by value, 2 knight pointer returns, 3
pointer returns in total (the third is `BoardCell *`), and 3 `getBoardCell` call sites.

Two rows are worth a minute.

- `playGame` receives both knights as mutable pointers and writes neither. It forwards them to
  `playTurn`. The mutable pointer is a pass-through, exactly as the mutable reference was in
  `construction-06`. The type does not say "I write"; it says "I may".
- The print helpers receive `const Knight *` and only read. They could take the knight by value and
  still be correct; the copy would cost eight bytes per call for no benefit, and the pointer marks
  the read-only intent at the declaration. `const` is on the pointee, not on the pointer.

Then answer in `EVIDENCE.md` §2:

1. How many functions receive a knight by mutable pointer? By `const` pointer? By value?
2. Which two functions return a knight pointer, and what does each return type permit?
3. Which function is the only one that writes a knight's fields through a parameter?
4. Which function returns a board cell, and what does its return type know and not know?
5. What did each `&` become at the declaration, and where did a `.` become `->`?
6. Which call sites now show an `&`, and which do not?
7. Twelve reference parameters remain in the file. Which ones, and why were they not converted?
8. Which pointer syntax existed in `construction-06` before this lesson?

Question 4 is the layout question. Question 7 is the honesty check: not every alias should become a
pointer. `int &destinationRow` and `std::string &winnerName` are out-parameters; they cannot be
absent, and making them pointers would add null as a state for no gain.

## Step 2 — Predict before you run (~15 min)

### 2.1 The diff

Write these predictions before running `diff`. The file is in front of you; the exercise is to see
how much of the change you can account for without measuring it.

| Question | My prediction |
|---|---|
| Lines added / removed / net | |
| Function definitions before / after | |
| Parameters before / after | |
| Signatures that change a knight type | |
| Call sites that gain an `&` | |
| Board expressions that become accessor calls | |
| Reference parameters that remain | |
| Traces that differ, of 20 | |

### 2.2 The board addresses

Answer without compiling in `EVIDENCE.md` §3.2.

| Question | My prediction |
|---|---|
| Element offset of (2,3) from (0,0), width 9 | |
| Element offset of (2,3) from (0,0), width 11 | |
| What `boardCells[2]` is, and what it decays to | |
| Does `boardCells[2] + 3` equal `&boardCells[2][3]`? | |
| What `+ 3` adds: bytes or elements | |
| What `getBoardCell(...) + 9` points to, width 9 | |
| What `getBoardCell(...) + 9` points to, width 11 | |
| Which of these the compiler checks | |

The unit matters. `rowStart + column` adds `column` elements of type `BoardCell`. On this platform
`sizeof(BoardCell)` is 1 because the only field is a `bool`, so elements and bytes happen to agree.
Do not build a rule on that coincidence; the compiler is free to pad the struct later.

### 2.3 The limits

Answer these now, without testing:

- Can a `Knight *` be null, so that "no knight" is representable?
- Can `activeKnight` be made to name the other record after it is initialized?
- What does `activeKnight = machineKnight;` do now, and what did it do in `construction-06`?
- What happens if a pointer outlives the record it points to?
- Does `moveKnight(activeKnight, ...)` tell a reader that the pointee may change?
- Which of these does the compiler catch?

Write the answers in `EVIDENCE.md` §3.3. Step 6 checks them.

## Step 3 — Run both baselines (~8 min)

Create the scratch directory first; every command below writes into it.

```bash
mkdir -p /tmp/pointer-test
./run.sh --seed 1 --name Learner > /tmp/pointer-test/pointers-0001.txt
diff -u ../../../common/traces/seed-0001.txt /tmp/pointer-test/pointers-0001.txt
```

Then the previous baseline:

```bash
cd ../construction-06-references
./run.sh --seed 1 --name Learner > /tmp/pointer-test/references-0001.txt
diff -u ../../../common/traces/seed-0001.txt /tmp/pointer-test/references-0001.txt
```

Then the two against each other:

```bash
diff -u /tmp/pointer-test/references-0001.txt /tmp/pointer-test/pointers-0001.txt
```

Three empty diffs. Return to this lesson directory and confirm all 20 seeds, not only the first:

```bash
cd ../construction-07-pointers
for trace in ../../../common/traces/seed-*.txt; do
  traceName="$(basename "${trace}" .txt)"
  seedText="${traceName#seed-}"
  seedNumber="$((10#${seedText}))"
  ./run.sh --seed "${seedNumber}" --name Learner | cmp - "${trace}"
done
```

A silent loop is 20 matches. If `run.sh` cannot build, compile once by hand:

```bash
mkdir -p build
g++ -std=c++20 -Wall -Wextra -Werror -O2 \
  -o build/pointers-baseline code/pointers-baseline.cpp
```

If a diff is not empty, stop. Record it in `EVIDENCE.md` §11. Do not adjust a trace to match the
code.

## Step 4 — Rewrite one board access (~15 min)

Copy the baseline. The tracked file stays untouched.

```bash
cp code/pointers-baseline.cpp /tmp/pointer-test/row-start.cpp
```

In `tryMove`, replace the accessor call with explicit pointer arithmetic:

```cpp
BoardCell *rowStart = boardCells[destinationRow];
BoardCell *destinationCell = rowStart + destinationColumn;
if (destinationCell->visited)
```

Build and run the 20 seeds:

```bash
g++ -std=c++20 -Wall -Wextra -Werror -O2 \
  -o /tmp/pointer-test/row-start /tmp/pointer-test/row-start.cpp
for trace in ../../../common/traces/seed-*.txt; do
  traceName="$(basename "${trace}" .txt)"
  seedText="${traceName#seed-}"
  seedNumber="$((10#${seedText}))"
  /tmp/pointer-test/row-start --seed "${seedNumber}" --name Learner | cmp - "${trace}"
done
```

All 20 match, because the address is identical: `boardCells[row]` is the first cell of the row, and
`+ column` walks `column` cells into it.

### 4.1 Measure the stride

Write a scratch probe with the same declarations and print the offsets:

```cpp
#include <cstdio>

struct BoardCell
{
    bool visited;
};

constexpr int boardRowCount = 8;
constexpr int boardColumnCount = 8;

int main()
{
    BoardCell board[boardRowCount + 1][boardColumnCount + 1] = {};
    BoardCell *rowStart = board[2];
    BoardCell *cell = rowStart + 3;
    BoardCell *flat = &board[0][0] + 2 * (boardColumnCount + 1) + 3;
    std::printf("rowStart + 3 == &board[2][3]: %d\n", cell == &board[2][3]);
    std::printf("flat == &board[2][3]: %d\n", flat == &board[2][3]);
    std::printf("element offset: %td\n", &board[2][3] - &board[0][0]);
    std::printf("byte offset: %td\n",
                reinterpret_cast<const char *>(&board[2][3]) -
                reinterpret_cast<const char *>(&board[0][0]));
    std::printf("sizeof(BoardCell): %zu\n", sizeof(BoardCell));
    return 0;
}
```

Compile it with the project flags and run it:

```bash
g++ -std=c++20 -Wall -Wextra -Werror -O2 \
  -o /tmp/pointer-test/stride /tmp/pointer-test/stride.cpp
/tmp/pointer-test/stride
```

Expected numbers on this platform: both comparisons print `1`, the element offset is `21`, the byte
offset is `21`, and `sizeof(BoardCell)` is `1`. The element offset is `2 * 9 + 3`; the byte offset
is that multiplied by the size of one cell.

Change `boardColumnCount` to 10 in the probe and run it again. The element offset becomes `25`
(`2 * 11 + 3`), and the `flat` comparison still prints `1` only because the probe recomputes the
stride from the constant. Replace `(boardColumnCount + 1)` with a literal `9` and the comparison
prints `0`: the flattened pointer now points at the wrong cell.

Write `EVIDENCE.md` §5: the rewrite, the build result, the 20 matches, the four numbers, and the
sentence that separates an element offset from a byte offset.

## Step 5 — Change the row width (~15 min)

The rewrite in Step 4 still names the row through `boardCells[row]`, so the compiler knows the
width. Now separate the two ways of writing the stride.

Copy the baseline again and change one constant:

```bash
cp code/pointers-baseline.cpp /tmp/pointer-test/width-10.cpp
```

In that copy, set `boardColumnCount` to `10`. Build with the project flags:

```bash
g++ -std=c++20 -Wall -Wextra -Werror -O2 \
  -o /tmp/pointer-test/width-10 /tmp/pointer-test/width-10.cpp
```

It compiles clean. The game is now 8 rows by 10 columns, so the starting cells, the legal moves and
the transcript all change; all 20 traces differ. That is not a regression. The golden traces encode
an 8×8 board, and this probe changes the game on purpose. Record it as a structural observation, not
as a failed trace.

What the probe shows:

- `boardCells[row]` and `rowStart + column` still work. The parameter type is now
  `BoardCell (*)[11]`, so the compiler recomputed the stride from the new declaration.
- A flattened access with a literal `9` still compiles and reads the wrong cell:

  ```cpp
  BoardCell *flatStart = &boardCells[0][0];
  BoardCell *wrongCell = flatStart + destinationRow * 9 + destinationColumn;
  ```

  At `(2,3)` with width 11, `wrongCell` is element 21 of the block, which is `(1,10)`, not `(2,3)`
  at element 25. No diagnostic, no crash; just the wrong cell.
- A flattened access that uses `boardColumnCount + 1` gives the right address, but now the function
  needs the constant, or an extra `rowWidth` parameter. A function that receives only
  `BoardCell *` cannot recover the stride from the pointer.

### 5.1 Count where the layout lives

Count the places that name the row width, in both baselines:

| Baseline | Two-index expressions (each relies on the stride) | Parameter declarations with `[][boardColumnCount + 1]` |
|---|---|---|
| `construction-06` | 3 (`tryMove`, two in `playTurn`) | 3 (`tryMove`, `playTurn`, `playGame`) |
| `construction-07` | 1 (`getBoardCell`) | 4 (`getBoardCell`, `tryMove`, `playTurn`, `playGame`) |

The totals are close; the location changed. In `construction-06`, every write names both indices. In
`construction-07`, the writes name a cell pointer, and the address computation is in one function.
The width still appears at every boundary that receives the board, because the second dimension is
part of the parameter type. What the accessor removes from the call sites, it does not remove from
the signatures.

Two failures are possible when the width changes:

- **Loud:** a declaration written as `BoardCell boardCells[][9]` with a literal second dimension.
  When the actual array becomes `[11]`, the call does not match the parameter type and the compiler
  rejects it.
- **Silent:** a flattened `BoardCell *` plus a literal `9` in the arithmetic. It compiles and points
  at the wrong cell. Only the run, or a test that checks the address, notices.

Write `EVIDENCE.md` §6: the width you chose, the build result, which traces changed and why that is
expected, the two arithmetic forms, and the counts in the table.

## Step 6 — Null, reseating and lifetime (~12 min)

The previous steps changed where the layout lives. This step measures what the pointer adds.

### 6.1 Null

Copy the baseline to a third scratch file:

```bash
cp code/pointers-baseline.cpp /tmp/pointer-test/null-check.cpp
```

In `playTurn`, before the loop, add:

```cpp
Knight *noKnight = nullptr;
moveKnight(noKnight, 1, 1);
```

Build with the project flags:

```bash
g++ -std=c++20 -Wall -Wextra -Werror -O2 \
  -o /tmp/pointer-test/null-check /tmp/pointer-test/null-check.cpp
```

It compiles clean. Run seed 1:

```bash
/tmp/pointer-test/null-check --seed 1 --name Learner
```

The process crashes with a segmentation fault (exit status 139 on this platform). The compiler could
not reject it because the type permits null and the value is only known at run time. This is the
first part of the bill: `construction-06` could not represent "no knight"; `construction-07` can,
and every dereference is now a promise that the location is valid.

Restore the file, or copy the baseline again, before continuing.

### 6.2 Reseating

A pointer can be redirected. In a scratch probe, or in a copy you do not run as a game:

```cpp
Knight *activeKnight = selectActiveKnight(playerIsActive, playerKnight, machineKnight);
Knight *before = activeKnight;
activeKnight = machineKnight;
```

`activeKnight` now points at the machine record. `before` still points at the original. This
compiles because assignment on a pointer changes the location, not the pointee. In
`construction-06`, `activeKnight = machineKnight;` copied the machine values into the player record
and left the name bound to the player.

Do not wire this into the game: the next `moveKnight` would move the wrong knight. The probe is
enough to see the difference. A pointer is a value you can reassign; a reference is a name you
cannot.

### 6.3 Lifetime

A pointer can outlive its pointee. Write a scratch probe:

```cpp
struct Knight
{
    int row;
    int column;
};

Knight *makeKnight()
{
    Knight local = {1, 1};
    return &local;
}

int main()
{
    Knight *knight = makeKnight();
    return knight->row;
}
```

Build with `-Wall -Wextra -Werror`. GCC rejects it:

```text
error: address of local variable 'local' returned [-Werror=return-local-addr]
```

The compiler catches this case because the local is visible in the same function. Without `-Werror`
the program builds, and the returned pointer dangles the moment `makeKnight` returns. The compiler
does not catch a pointer that outlives a record in another scope, a heap object, or a container that
reallocates. Ownership and lifetime are `construction-16`.

In the baseline, `getBoardCell` returns a pointer into `boardCells`, which is a local of `main` and
stays alive until `main` returns, after `playGame`. That is why the pointer is valid for the whole
game. The guarantee comes from the call graph, not from the type.

Write `EVIDENCE.md` §7: the null build and run, the reseating observation, the lifetime diagnostic,
and the sentence that separates "the type says what is there" from "the type says it is still
there".

## Step 7 — Compare and explain (~8 min)

Write `EVIDENCE.md` §8. Two parts.

**First, one sentence.** Say what a pointer decides about a record or a cell, without using the
words *pointer*, *address*, *dereference*, *better*, *worse*, *clean*, *cleaner*, *nice*, *elegant*,
*ugly* or *modular*. If your sentence needs one of those, you have described your reaction to the
file rather than the type.

**Second, one paragraph.** Answer:

- What a pointer type decides: the callee receives a location, not a copy and not a binding name.
- What a pointer return decides: which record or cell the caller can reach, chosen at run time.
- What `const` on the pointee decides, and where the compiler checks it.
- What the call site shows: the `&` where the address is taken, and nothing after that.
- What the board accessor localizes: the address computation, not the stride.
- What it does **not** localize: the row width, bounds, null, lifetime, ownership and call-site
  visibility after the address is held.
- What the compiler proves: pointee compatibility, `const` correctness, a returned local address.
- What only a run proves: that the arithmetic lands on the right cell, that the trace is unchanged,
  that a null dereference crashes.
- Which later lesson fixes bounds and representation: `construction-09`; invariants and access:
  `construction-10` and `construction-11`; ownership and lifetime: `construction-16`.

A strong answer keeps four things apart: **what the type permits**, **what the call site shows**,
**what the compiler enforces**, and **what only a run proves**. A weak answer says pointers are
powerful, which hides all four.

## Step 8 — Acceptance check (~3 min)

The acceptance criterion for this lesson is: **access a board cell through a pointer, explain the
row-major stride, and show that the transcript stays unchanged while the source exposes the
layout.**

Check your record against it:

- [ ] I ran both unmodified baselines and got three empty diffs, plus 20 matching seeds.
- [ ] I inventoried the 17 knight parameters: 9 mutable, 8 read-only, 0 by value.
- [ ] I identified `Knight *`, `const Knight *`, and `moveKnight(Knight *, ...)`.
- [ ] I identified `getBoardCell` returning `BoardCell *` and its three call sites.
- [ ] I predicted the diff counts and the board offsets before measuring.
- [ ] I rewrote one board access as `rowStart + column` and still got 20 matches.
- [ ] I measured the element offset at 21 for width 9 and 25 for width 11.
- [ ] I explained that `+ column` adds elements, not bytes.
- [ ] I changed the width to 10, saw the two-dimensional form adapt, and saw a literal `9` misread.
- [ ] I counted where the row width lives in both baselines.
- [ ] I showed that `moveKnight(nullptr, ...)` compiles and crashes.
- [ ] I showed that returning a local address fails the build with `-Wreturn-local-addr`.
- [ ] I showed that a pointer can be reseated and a reference cannot.
- [ ] I explained what the pointer makes visible and what it still hides.
- [ ] My one-sentence explanation avoids the three syntax words and the eight verdict words.
- [ ] I did not edit either tracked baseline.
- [ ] I completed `EVIDENCE.md` §1–§10 and the self-check in §12.

The criterion is not "pointers are more powerful than references". It is "you can say what a
location permits, where the layout is encoded, and which check would notice if the address were
wrong".

## Self-check (binary)

- [ ] I read both baselines and inventoried every knight parameter, pointer return and board access.
- [ ] I predicted the diff, the offsets and the null/lifetime behavior before measuring.
- [ ] I built both baselines and reproduced all 20 traces.
- [ ] I rewrote a board access as `rowStart + column` and kept 20 matches.
- [ ] I measured the row-major stride and changed the row width.
- [ ] I showed the silent failure of a literal stride and the loud failure of a wrong type.
- [ ] I showed null compiling and crashing, and a dangling local failing the build.
- [ ] I compared the prediction with the observation and named the misses.
- [ ] I explained what a pointer localizes and what it does not.
- [ ] I completed `EVIDENCE.md`.

These are acceptance criteria. If a box is
unchecked, write the next smallest action in `EVIDENCE.md` §10.

## Misconceptions this lesson refutes

| Misconception | Why it fails | What to do instead |
|---|---|---|
| "A pointer is just an integer address." | It is typed. `+1` moves by one element of the pointee type, not one byte. | Say what the pointee type is before doing arithmetic. |
| "A pointer and a reference are the same thing with different spelling." | Null, reseating, the call-site `&` and lifetime are different. | Compare the four properties, not the syntax. |
| "`rowStart + column` adds bytes." | It adds `column` elements of type `BoardCell`. | Multiply by `sizeof(BoardCell)` only when you need bytes. |
| "The two-dimensional parameter is an array." | It decays to `BoardCell (*)[9]`, a pointer to a row. | Read the parameter as "pointer to a row of 9". |
| "Changing the width only touches the declaration." | A literal `9` in flattened arithmetic compiles and misreads; the constant form adapts. | Count literals, not only declarations. |
| "`getBoardCell` hides the layout completely." | It hides the two-dimensional indexing; the returned `BoardCell *` no longer knows the width. | Keep the width in the type or pass it explicitly. |
| "The compiler catches null." | `moveKnight(nullptr, ...)` compiles clean and crashes. | Check for null, or use a form that cannot be null. |
| "The compiler catches all lifetime errors." | It catches a returned local; not a pointer that outlives a heap object or another scope. | Treat lifetime as a design decision, not a warning. |
| "Pointers are faster, so use them." | A two-int record is cheap to copy. The reason here is access and optionality. | Ask what the callee must be able to do. |
| "`const Knight *` means the pointer cannot change." | The pointee is read-only; the pointer itself can be reseated. | Read `const` left to right: `const Knight` is the pointee. |
| "Pointer arithmetic is safe because it compiles." | There is no bounds check. `rowStart + 9` compiles. | Keep the bounds in one place; `construction-09` starts that. |

## Next step

`construction-08` — modular programming. The board and the game move behind headers and separate
compilation, so the layout stops being visible to every caller. After that, `construction-09` gives
the board its own bounds, and the `1..8` stops being a fact the caller must remember.

See the [lesson catalog](../../../docs/guide/lesson-catalog.md) for the full path.

If either baseline was missing, do not start `construction-08`. The next contrast needs both sides.
