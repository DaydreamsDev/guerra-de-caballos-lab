# Encapsulation (encapsulación)

> [English](lesson.md) | [Español](lesson-es.md)

**Concept:** class with invariant (clase con invariante) · **Time:** about 60 minutes ·
**Status:** drafted
**Reading:** ~18 minutes

## Why this lesson exists

`construction-10` made the invariant observable. It added `board_is_valid`, two guards and a scan,
and then named the gap: nothing called the checker, and the state was still reachable to any code
that could see the definition. The rule was real, but the language did not enforce who could write
the field.

This baseline moves the state behind a class boundary:

```cpp
class Board
{
public:
    Board();
    int row_count() const;
    int column_count() const;
    bool is_valid() const;
    void reset();
    bool was_visited(int row, int column) const;
    void mark_visited(int row, int column);

private:
    static constexpr int maximumRowCount = 16;
    static constexpr int maximumColumnCount = 16;
    static constexpr int activeRowCount = 8;
    static constexpr int activeColumnCount = 8;

    bool visited[maximumRowCount][maximumColumnCount];
};
```

Seven public operations. Five private data declarations. One private array. `game.cpp` owns a
`Board` by value and can only call the public operations. There is no `board_create`, no
`board_destroy`, and no `Board *` in the game path.

The sentence to keep:

> Encapsulation is not hiding the bytes. It is making the only legal writes the ones the object
> itself performs.

Three honest notes before the comparison.

1. The source shrinks: 488 lines across seven files, against 502 in `construction-10` and 479 in
   `construction-09`. `board.h` grows from 12 to 21 lines; `board.cpp` shrinks from 87 to 65.
2. The object file shrinks: 552 bytes of text for `board.o`, against 608 in `construction-10` and
   504 in `construction-09`. Eight exported symbols, the same count as `construction-10`, but the
   names are now `Board::` members and the two constructor variants.
3. Twenty of twenty traces still pass. They cannot see the boundary: a trace never writes
   `board.visited[0][0] = true`, and outside the class that line does not compile.

The comparison with `construction-10` is not "the class is better". It is more precise: the opaque
ADT hid the representation by keeping the definition out of the header. The class keeps the
definition in the header and hides the writers with access control. Those are different things, and
the change test shows exactly which one this baseline buys.

## What you will be able to do

By the end of this lesson you can:

- explain a class boundary that preserves an invariant;
- show that private state keeps the invariant and public state exposes it.

## The concept: a boundary the compiler enforces

`construction-09` asked: can a caller see the representation? No, because the header declared
`struct Board;` and the definition lived in `board.cpp`. `construction-10` asked: can a caller trust
the values? Only if someone calls the checker, and nobody did. This lesson asks a third question:
can a caller construct an invalid state at all?

The answer is now no, through the public interface. `visited` is private. The compiler rejects any
expression outside `Board` that names it. The only writers are the member functions, and the two
writers guard their writes. That is a different guarantee from the one in `construction-10`:

| Baseline | How the representation is hidden | Who can write `visited` |
|---|---|---|
| construction-10 | Incomplete type in the header; definition in `board.cpp`. | Any code in `board.cpp`; callers only through free functions. |
| construction-11 | Complete class in the header; `private:` access. | `Board` member functions only. |

The table is the whole lesson. The ADT hides the layout from code outside `board.cpp`. The class
hides the writer set from code outside `Board`. Neither one hides both. If you move the
`construction-10` struct into the header, `visited` becomes visible to every caller. If you move the
`construction-11` class into a header — it is already there — `visited` stays unreachable because of
`private:`. That is the upgrade: the boundary is a property of the type, not of file placement.

The invariant is unchanged: only cells inside rows 1..8 and columns 1..8 may be marked visited.
What changes is the set of code that can attempt the write.

| Role | Operation | Access | What it decides |
|---|---|---|---|
| Construction | `Board::Board()` | public | All 256 cells start `false`. |
| Observation | `is_valid()` | public, `const` | Whether a cell outside 1..8 is marked. |
| Enforcement | `mark_visited()` | public, non-`const` | Whether a write reaches the array. |
| Enforcement | `was_visited()` | public, `const` | Whether a read reaches the array. |
| Reset | `reset()` | public, non-`const` | Clears 1..8; leaves 0 and 9..15 alone. |
| Boundary | `private:` | — | Which code can name `visited`. |

The `const` qualifier is part of the boundary. `is_valid`, `was_visited`, `row_count` and
`column_count` cannot modify the array; the compiler rejects a write through a `const Board&`.
`mark_visited` and `reset` are the only non-`const` operations. In `game.cpp`, `tryMove` and
`initializePositions` take `const Board&`; `playTurn` and `playGame` take `Board&`. The signatures
say which functions observe and which mutate, and the compiler checks it.

Two limits, stated now so the acceptance is honest:

1. **The boundary is not a security boundary.** `#define private public`, a `friend` declaration, or
   a `reinterpret_cast` can defeat it. It is a design boundary enforced against ordinary code.
2. **The boundary does not make the member functions correct.** A bug inside `mark_visited` can
   still break the invariant. The class moves the trust boundary from every caller to the class
   implementation; it does not remove the need for the checker or the tests.

Access control is a compile-time rule. It adds no runtime check and no per-object storage:
`sizeof(Board)` is 256 bytes on this platform, the array itself. The boundary costs nothing at run
time; it costs a rule at compile time.

That is why `is_valid()` is still public and still has zero call sites in the game. The class
removes the outside writer; the checker remains the diagnostic for the inside writer.

## Before you start

- [`construction-10`](../construction-10-adt-invariants/lesson.md) — the catalog prerequisite. You
  need the invariant baseline in front of you, because this lesson is a diff against it.
- [`construction-09`](../construction-09-abstract-data-type/lesson.md) — the opaque `Board` and its
  seven operations, so you can see what the class changes and what it keeps.
- [`construction-03`](../construction-03-functions/lesson.md) — you can read a function
  declaration, a parameter list and a return type.
- A C++20 compiler (`g++` or `clang++`). `cmake` is optional; `run.sh` falls back to a direct
  compile when CMake is absent.

You do not need to know how access control is implemented in the compiler. You need to read a class
declaration, identify the `public:` and `private:` sections, and compile one small file twice.

### What you need

- The encapsulation baseline:
  [`code/main.cpp`](code/main.cpp), [`code/game.h`](code/game.h),
  [`code/game.cpp`](code/game.cpp), [`code/board.h`](code/board.h),
  [`code/board.cpp`](code/board.cpp), [`code/random_sequence.h`](code/random_sequence.h),
  [`code/random_sequence.cpp`](code/random_sequence.cpp).
- The previous baseline, for the comparison: the
  [`board.h`](../construction-10-adt-invariants/code/board.h) and
  [`board.cpp`](../construction-10-adt-invariants/code/board.cpp) of `construction-10`.
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
test -f ../construction-10-adt-invariants/code/board.cpp &&
test -f ../../../common/traces/seed-0001.txt
```

If a file is missing, the comparison has no second side. Record the missing file in `EVIDENCE.md`
§11 and return when it is available. A count against a file you have not read is not evidence.

The baseline reproduces all 20 traces in `common/traces/` byte for byte, before you change
anything. That is the behavioral contract, and it is silent about the boundary.

## Step 1 — Inventory the class boundary (~10 min)

Read the header first. It is 21 lines, and the shape is the lesson:

```bash
grep -n '^' code/board.h
```

```text
1:#pragma once
2:
3:class Board
4:{
5:public:
6:    Board();
7:    int row_count() const;
8:    int column_count() const;
9:    bool is_valid() const;
10:    void reset();
11:    bool was_visited(int row, int column) const;
12:    void mark_visited(int row, int column);
13:
14:private:
15:    static constexpr int maximumRowCount = 16;
16:    static constexpr int maximumColumnCount = 16;
17:    static constexpr int activeRowCount = 8;
18:    static constexpr int activeColumnCount = 8;
19:
20:    bool visited[maximumRowCount][maximumColumnCount];
21:};
```

Now the line map of `board.cpp`. Fill it into `EVIDENCE.md` §2.2 from the source, not from this
table:

| Lines | Contents |
|---|---|
| 3–6 | `Board::Board()` — value-initializes `visited`. |
| 8–11 | `Board::row_count` |
| 13–16 | `Board::column_count` |
| 18–34 | `Board::is_valid` — the outside-active-area scan. |
| 36–45 | `Board::reset` — clears rows and columns 1..8. |
| 47–55 | `Board::was_visited` — the read guard. |
| 57–65 | `Board::mark_visited` — the write guard. |

Counts for this baseline: 3 headers, 4 translation units, 11 public declarations, 5 private data
declarations, 1 complete class definition, 0 incomplete types, 30 definitions, 17 internal helpers
in `game.cpp`, 1 internal helper in `main.cpp`, 1 entry point.

Compare with `construction-10`: 12 public declarations and 31 definitions. The board surface went
from eight free functions to seven public members: `board_create` and `board_destroy` are gone, and
the constructor replaces creation. The net is one fewer public declaration and one fewer definition.
The callers changed only in `game.cpp`: every `board_*(&board, ...)` became a `board.*(...)` member
call.

Then answer in `EVIDENCE.md` §2:

1. Which line opens `public:`, and which line opens `private:`?
2. How many public operations does `Board` declare? Name them.
3. How many private data declarations are there? Name them.
4. Which public operations are `const`, and what does that prevent?
5. Where is the array dimension defined, and why can a caller not use it?
6. Which operation constructs the board, and what value does it give each cell?
7. Which operation clears cells, and which cells does it leave alone?
8. How many call sites does `is_valid()` have in `code/`?
9. What happened to `board_create` and `board_destroy`?
10. What does `game.cpp` own, and how does it pass the board to helpers?

Question 8 has a precise answer: zero. The declaration is at `board.h:9`, the definition at
`board.cpp:18`, and no translation unit calls it. Encapsulation removes the outside writer; it does
not add a caller for the observer.

Question 3 has five answers: four `static constexpr int` dimensions and one
`bool visited[16][16]`. The dimensions are private, but they are still visible to a human reading
the header. That is a tradeoff, not an accident: the class hides the field from code, not the
layout from a reader.

Question 9 has the structural answer. `board_create` returned `new Board{}`; `board_destroy` called
`delete`. The class replaces both with a value: `Board board;` in `run_game` at `game.cpp:267`, and
an implicit destructor at the end of the function. Ownership is a side effect here;
`construction-16` takes it as its main subject.

## Step 2 — Predict before you run (~10 min)

### 2.1 The public operations

Write these predictions before running anything. The array is `bool visited[16][16]`; the active
area is rows 1..8 and columns 1..8.

| Call sequence | Predicted `is_valid()` after | Predicted `was_visited` after |
|---|---|---|
| `Board board;` | | — |
| `board.reset()` | | — |
| `board.mark_visited(1, 1)` | | |
| `board.mark_visited(8, 8)` | | |
| `board.mark_visited(0, 0)` | | |
| `board.mark_visited(9, 9)` | | |
| `board.mark_visited(4, 0)` | | |
| `board.mark_visited(8, 9)` | | |

The first row is the one to take seriously. A freshly constructed board must be valid. Predict
`true`, then measure. If you predict `false`, write down which line of the constructor or the scan
would make it false, and check that line in Step 3.

The next two rows are the legal writes. The last four are outside the active area in different
directions: below it, above it, on the reserved column 0, and past the active column 8. Predict
what each one leaves behind.

### 2.2 The change test

| Question | My prediction |
|---|---|
| What happens when `private:` is deleted and `board.visited[0][0] = true` compiles? | |
| `is_valid()` after that write | |
| What happens when `private:` is restored and the same probe is compiled? | |
| Seeds that match, of 20, with the class unchanged | |
| Seeds that match, of 20, with `visited` public | |
| Does the game call `is_valid()`? | |
| `board.o` text size: grows, shrinks or stays the same? | |

The first row is the demonstration. Deleting one line changes what the compiler accepts. Predict
whether the write compiles, and predict what `is_valid()` says after it.

### 2.3 The compiler

| Question | My prediction |
|---|---|
| The line the compiler points to when `visited` is private | |
| The exact error text I expect | |
| Why the compiler can reject the write even though `board.h` is included | |
| Why the compiler cannot reject a bad index inside `mark_visited` | |
| My reason | |

The last two rows are the boundary of the boundary. Access control is a compile-time rule about
names. Array bounds are not. `mark_visited(0, 0)` compiles even when `visited` is private; the guard
inside the function is what rejects it. Do not confuse the two checks.

## Step 3 — Run the baseline and a boundary probe (~10 min)

First the traces, unmodified:

```bash
mkdir -p /tmp/encapsulation-test
./run.sh --seed 1 --name Learner > /tmp/encapsulation-test/baseline-0001.txt
diff -u ../../../common/traces/seed-0001.txt /tmp/encapsulation-test/baseline-0001.txt
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
  -o build/encapsulation-baseline code/main.cpp code/game.cpp code/board.cpp \
  code/random_sequence.cpp
```

Now the boundary probe. It goes in the scratch directory, never in the tracked files. It exercises
every public operation and asks the checker after each one:

```bash
mkdir -p /tmp/encapsulation-test/probe
cat > /tmp/encapsulation-test/probe/boundary.cpp <<'EOF'
#include "board.h"

#include <cstdio>

int main()
{
    Board board;
    std::printf("after construction: valid=%d rows=%d columns=%d\n",
                static_cast<int>(board.is_valid()), board.row_count(),
                board.column_count());

    board.reset();
    std::printf("after reset: valid=%d\n", static_cast<int>(board.is_valid()));

    board.mark_visited(1, 1);
    std::printf("mark(1,1): valid=%d was=%d\n",
                static_cast<int>(board.is_valid()),
                static_cast<int>(board.was_visited(1, 1)));

    board.mark_visited(8, 8);
    std::printf("mark(8,8): valid=%d was=%d\n",
                static_cast<int>(board.is_valid()),
                static_cast<int>(board.was_visited(8, 8)));

    board.mark_visited(0, 0);
    std::printf("mark(0,0): valid=%d was=%d\n",
                static_cast<int>(board.is_valid()),
                static_cast<int>(board.was_visited(0, 0)));

    board.mark_visited(9, 9);
    std::printf("mark(9,9): valid=%d was=%d\n",
                static_cast<int>(board.is_valid()),
                static_cast<int>(board.was_visited(9, 9)));

    board.mark_visited(4, 0);
    std::printf("mark(4,0): valid=%d was=%d\n",
                static_cast<int>(board.is_valid()),
                static_cast<int>(board.was_visited(4, 0)));

    board.mark_visited(8, 9);
    std::printf("mark(8,9): valid=%d was=%d\n",
                static_cast<int>(board.is_valid()),
                static_cast<int>(board.was_visited(8, 9)));

    return 0;
}
EOF
g++ -std=c++20 -Wall -Wextra -Werror -O2 -Icode \
  -o /tmp/encapsulation-test/probe/boundary \
  /tmp/encapsulation-test/probe/boundary.cpp code/board.cpp
/tmp/encapsulation-test/probe/boundary
```

The output on this platform:

```text
after construction: valid=1 rows=8 columns=8
after reset: valid=1
mark(1,1): valid=1 was=1
mark(8,8): valid=1 was=1
mark(0,0): valid=1 was=0
mark(9,9): valid=1 was=0
mark(4,0): valid=1 was=0
mark(8,9): valid=1 was=0
```

Read the first line twice. Construction produces a valid board: the constructor value-initializes
all 256 cells, so no cell outside the active area starts `true`. The guards then do their job:
`was=1` for the two legal marks, `was=0` for the four illegal ones, and `is_valid()` stays `1` after
every call.

If your first line is `valid=0`, stop. That is a different baseline from the one this lesson
describes; record it in `EVIDENCE.md` §11 and do not adjust the prediction to match.

## Step 4 — Compare: what the boundary probe proves (~8 min)

Open `board.h` at the interface and `board.cpp` at the guards. Five facts, read against the class
definition:

1. **The public interface has no data member.** Line 5 opens `public:`; lines 6–12 declare only
   functions. The first data declaration is after `private:` at line 14. There is no public field
   to write.
2. **The only writers are members.** `reset()` and `mark_visited()` are the two non-`const`
   operations. `reset()` writes only rows and columns 1..8; `mark_visited()` returns before the
   write when the index is outside the same range, including negative indices.
3. **The observers are `const`.** `row_count`, `column_count`, `is_valid` and `was_visited` cannot
   modify `visited`. The compiler rejects a write through a `const Board&`, which is why `tryMove`
   can take `const Board&` and still call `was_visited`.
4. **The constructor is the only full initializer.** `Board::Board()` at `board.cpp:3–6` uses
   `visited{}`, which value-initializes all 256 cells to `false`. No public operation can mark a
   cell in row 0 or rows 9..15, so those cells stay `false` for the whole life of the object.
5. **The checker still sees the whole array.** `is_valid()` scans `0..15` over the 16×16 array, so
   it can report an illegal write if one ever happens. It is the only operation that looks at the
   cells the rest of the interface refuses to name.

The probe output, one row per call:

| Call | Guard decision | Array written? | `was_visited` | `is_valid` |
|---|---|---|---|---|
| none, constructed | — | no | — | `1` |
| `reset()` | — | clears 1..8 | — | `1` |
| `mark(1, 1)` | inside | yes | `1` | `1` |
| `mark(8, 8)` | inside | yes | `1` | `1` |
| `mark(0, 0)` | outside | no | `0` | `1` |
| `mark(9, 9)` | outside | no | `0` | `1` |
| `mark(4, 0)` | outside | no | `0` | `1` |
| `mark(8, 9)` | outside | no | `0` | `1` |

Now the limit of the evidence. The probe proves that the public operations preserve the invariant.
It does not prove that no caller can write the field, because the probe only uses the public
interface. The missing half is the change test: remove the access specifier and write the field
directly. That is the only way to see what `private:` is doing.

Record in `EVIDENCE.md` §5 the five facts, the eight probe rows, and the call-site count. The count
is the number that separates "the class is used" from "the checker is called". It is still zero.

## Step 5 — Run the change test (~12 min)

Copy the baseline. Tracked files stay untouched.

```bash
lessonDirectory="$(pwd)"
cp -r code /tmp/encapsulation-test/public
cd /tmp/encapsulation-test/public
```

### 5.1 Delete `private:`

In the scratch copy only, remove the access specifier from `board.h`. The data block then falls
under `public:`, because the `public:` label is still in effect:

```cpp
class Board
{
public:
    Board();
    int row_count() const;
    int column_count() const;
    bool is_valid() const;
    void reset();
    bool was_visited(int row, int column) const;
    void mark_visited(int row, int column);

    static constexpr int maximumRowCount = 16;
    static constexpr int maximumColumnCount = 16;
    static constexpr int activeRowCount = 8;
    static constexpr int activeColumnCount = 8;

    bool visited[maximumRowCount][maximumColumnCount];
};
```

That is a one-line edit. The array, the dimensions and the representation are now reachable from
any code that includes the header.

Deleting the label is the one-line version of the move. If you move only the array declaration, keep
the four constants above it: the array's type names them, and a member declaration cannot use a name
that is not yet declared. Moving the array line alone, above the constants, does not compile.

### 5.2 Compile and run the change probe

Write the probe first, while the state is public:

```bash
cat > /tmp/encapsulation-test/probe/change.cpp <<'EOF'
#include "board.h"

#include <cstdio>

int main()
{
    Board board;
    board.reset();
    std::printf("before write: valid=%d\n", static_cast<int>(board.is_valid()));

    board.visited[0][0] = true;
    std::printf("after visited[0][0]=true: valid=%d\n",
                static_cast<int>(board.is_valid()));

    return 0;
}
EOF
g++ -std=c++20 -Wall -Wextra -Werror -O2 -I. \
  -o /tmp/encapsulation-test/probe/change \
  /tmp/encapsulation-test/probe/change.cpp board.cpp
/tmp/encapsulation-test/probe/change
```

```text
before write: valid=1
after visited[0][0]=true: valid=0
```

There it is. The write reached `visited[0][0]`; row 0 is outside the active area; the scan saw the
cell and returned `false`. The invalid state is constructible again, and `is_valid()` is the
operation that makes it visible. This is the same invalid state `construction-10` could produce by
removing the guard, reached by a different route: not by changing the writer, but by changing who is
allowed to be a writer.

### 5.3 Restore `private:`

Put the access specifier back and rebuild the same probe:

```bash
g++ -std=c++20 -Wall -Wextra -Werror -O2 -I. \
  -o /tmp/encapsulation-test/probe/change \
  /tmp/encapsulation-test/probe/change.cpp board.cpp
```

```text
change.cpp: In function ‘int main()’:
change.cpp:11:11: error: ‘bool Board::visited [16][16]’ is private within this context
   11 |     board.visited[0][0] = true;
      |           ^~~~~~~
board.h:20:10: note: declared private here
   20 |     bool visited[maximumRowCount][maximumColumnCount];
      |          ^~~~~~~
```

Exit status 1. The same line that compiled one edit ago no longer compiles. That is the acceptance
demonstration: the access specifier is what prevents an ordinary caller from constructing the
invalid state.

The wording of the error depends on the compiler and the locale. The important part is the reason:
`visited` is private within this context. If your compiler says `is inaccessible` or `is private`,
the lesson is the same.

### 5.4 The traces, with and without `private:`

Build the whole program from the public scratch copy and run the 20 traces:

```bash
g++ -std=c++20 -Wall -Wextra -Werror -O2 -I. \
  -o encapsulation-public main.cpp game.cpp board.cpp random_sequence.cpp
cd "${lessonDirectory}"
for trace in ../../../common/traces/seed-*.txt; do
  traceName="$(basename "${trace}" .txt)"
  seedText="${traceName#seed-}"
  seedNumber="$((10#${seedText}))"
  /tmp/encapsulation-test/public/encapsulation-public --seed "${seedNumber}" \
    --name Learner | cmp - "${trace}"
done
```

A silent loop is 20 of 20. Record that count, restore `private:` in the scratch copy, rebuild with
the same command, and repeat: also 20 of 20. The game never writes outside the active area, so
changing the visibility of the field changes nothing the game can do. The trace cannot see the
boundary; only the change test can.

Restore `private:` in the scratch copy before the next step.

Record in `EVIDENCE.md` §6: the one-line diff, the two change-probe runs, the compiler error, and
both trace counts.

## Step 6 — The boundary in the rest of the program (~5 min)

The class boundary is not only `private:`. Two other mechanisms carry part of the invariant.

**`const` separates observers from mutators.** `tryMove` takes `const Board&` and calls
`row_count`, `column_count` and `was_visited`. All three are `const`. A write through that reference
does not compile:

```cpp
void observe(const Board &board)
{
    board.mark_visited(1, 1);
}
```

```text
error: passing ‘const Board’ as ‘this’ argument discards qualifiers
```

The exact wording varies, but the rule is stable: an observer cannot mutate the object it observes.
That is why the signatures in `game.cpp` are informative:

| Function | Board parameter | What it may do |
|---|---|---|
| `convertCellNumberToRow` | `const Board &` | Read `column_count`. |
| `convertCellNumberToColumn` | `const Board &` | Read `column_count`. |
| `initializePositions` | `const Board &` | Read `row_count` and `column_count`. |
| `tryMove` | `const Board &` | Read `row_count`, `column_count`, `was_visited`. |
| `playTurn` | `Board &` | Call `mark_visited`. |
| `playGame` | `Board &` | Pass the board to `playTurn`. |

**Value ownership removes the null case from the game path.** `run_game` declares `Board board;` at
`game.cpp:267`. There is no `new`, no `delete`, and no `Board *` to pass around. The destructor runs
automatically at the end of the function. That is a real improvement for this program, but it is not
the subject of this lesson; `construction-16` owns lifetime and will examine copies, moves and
resource rules in detail.

The class is copyable by default. A copy duplicates the 256 cells, so a copy of a valid board is
valid; copying reproduces the source rather than introducing a new mark. `construction-16` examines
copy and move semantics in detail.

Two things the class does not do, restated because the acceptance depends on them:

- A `Board *` can still be null. The class removes the null case from the value-and-reference path
  used by `game.cpp`, not from the language.
- A member function can still be wrong. The class moves the trust boundary; it does not make the
  implementation correct.

Fill the `EVIDENCE.md` §7 table with these answers. The last row is the one that keeps the lesson
honest: the class is not a proof, it is a narrowing of who can write.

## Step 7 — Compare and explain (~3 min)

The comparison in one table:

| Question | construction-10 | construction-11 |
|---|---|---|
| How is the representation hidden? | Incomplete type in the header; definition in `board.cpp`. | Complete class in the header; `private:` access. |
| Who can name `visited`? | Code in `board.cpp`. | `Board` member functions. |
| What stops an out-of-range write? | The guard in `board_mark_visited`. | The compiler rejects caller writes; `mark_visited` guards member writes. |
| What observes the state? | `board_is_valid`, a free function with a null check. | `is_valid`, a `const` member with no null check. |
| How is the board created? | `board_create()` returns `new Board{}`. | `Board board;` constructs a value. |
| How is it destroyed? | `board_destroy()` calls `delete`. | The implicit destructor runs at the end of scope. |
| Call sites of the checker | 0 | 0 |
| What the change test edits | Remove the guard from `board_mark_visited`. | Delete `private:` from `board.h`. |
| What the change test proves | The guard is what rejects the write. | Access control is what rejects the write. |

Write `EVIDENCE.md` §8. Two parts.

**First, one sentence.** Say what the class decides about the state, without using the words
*class*, *private*, *public*, *access*, *compiler*, *keyword*, *header*, *file*, *module*,
*encapsulation*, *better*, *worse*, *clean*, *cleaner*, *nice*, *elegant*, *ugly* or *modular*. If
your sentence needs one of those, you have described the syntax rather than the decision.

**Second, one paragraph.** Answer:

- What the class fixes: which code may write the state, and which operations preserve the
  invariant.
- What enforces it: access control for the field, guards inside the two writers, and `const` on the
  observers.
- What makes the boundary different from `construction-10`: the definition is visible in the header,
  but the field is still unreachable; the boundary is a property of the type, not of file
  placement.
- What the public operations do: the constructor initializes all cells, `reset` clears 1..8,
  `mark_visited` guards the write, `was_visited` guards the read, `is_valid` observes, and the two
  count operations report the dimensions.
- What the 20 traces prove: the game's behavior is unchanged, with `visited` public and with it
  private.
- What the traces do not prove: that the boundary holds, because no trace attempts an illegal
  write.
- What the change test proves: with `visited` public, the invalid state is constructible and
  `is_valid()` reports it; with `private:`, the same write does not compile.
- What the change test costs: one line in `board.h`, zero caller edits, zero broken traces.
- What the class does not fix: a bug inside a member function, a `friend`, or a cast that defeats
  access control. The class narrows the writer set; it does not prove the writers correct.
- Which later lesson builds on the class: `construction-12`, polymorphism.
- Which later lesson owns lifetime: `construction-16`.

A strong answer keeps four things apart: **what the rule says**, **who is allowed to write**, **what
the compiler checks**, and **what a run can see**. A weak answer says the data is private, which
collapses the first three into a keyword.

## Step 8 — Acceptance check and record (~2 min)

The acceptance criterion for this lesson is: **show that no public field or operation can break the
board invariant, and that making the state public makes the invalid state constructible.**

Check your record against it:

- [ ] I identified the private state, the seven public operations and the `private:` line.
- [ ] I found that `board.h` has no public data member.
- [ ] I predicted `valid=1` after construction and observed `valid=1`.
- [ ] I showed that every public operation leaves `is_valid()` true.
- [ ] I found that `is_valid()` still has zero call sites in `code/`.
- [ ] I deleted `private:` in a scratch copy and `board.visited[0][0] = true` compiled.
- [ ] I observed `is_valid()` become false after that write.
- [ ] I restored `private:` and the same write no longer compiled.
- [ ] I read the compiler error and explained which name it rejects.
- [ ] All 20 traces pass with `visited` public and with `visited` private.
- [ ] I recorded that the traces cannot see the boundary.
- [ ] I explained what access control prevents and what it does not.
- [ ] I explained why `mark_visited(0, 0)` still needs a runtime guard.
- [ ] I did not edit any tracked code file.
- [ ] I completed `EVIDENCE.md` §1–§10 and the self-check in §12.

The criterion is not "the field is private". It is "you can say which code may write the state,
which compiler rule rejects the outside write, which runtime guard rejects the inside write, and
what a passing trace does not cover".

## Self-check (binary)

- [ ] I read `board.h` and `board.cpp` and located the class boundary by line.
- [ ] I compared the class with `construction-10` and named what each one hides.
- [ ] I predicted the public-operation behavior and the change test before running.
- [ ] I reproduced all 20 traces on the unmodified baseline.
- [ ] I built a boundary probe and saw a fresh board report valid.
- [ ] I showed that every public operation preserves the invariant.
- [ ] I deleted `private:` in a scratch copy and saw the invalid state become constructible.
- [ ] I restored `private:` and saw the same write rejected at compile time.
- [ ] I reproduced 20 of 20 traces with `visited` public and with it private.
- [ ] I explained the difference between access control and array bounds.
- [ ] I explained what the class does not fix.
- [ ] I completed `EVIDENCE.md`.

These are acceptance criteria. If a box is
unchecked, write the next smallest action in `EVIDENCE.md` §10.

## Misconceptions this lesson refutes

| Misconception | Why it fails | What to do instead |
|---|---|---|
| "Encapsulation means the data is invisible." | The header still shows the array and its dimensions. `private:` stops code, not readers. | Separate access control from secrecy. |
| "A class is encapsulation." | A class with a public data member is not encapsulated. The keyword alone proves nothing. | Read the access specifiers, not the keyword. |
| "Private is a security boundary." | `#define private public`, `friend`, and casts can defeat it. | Treat it as a design boundary enforced against ordinary code. |
| "The compiler checks the array bounds." | It checks access, not indices. `mark_visited(0, 0)` compiles; the guard rejects it at runtime. | Keep the guard and the checker. |
| "The class makes `is_valid` unnecessary." | The class removes the outside writer; it does not prove the member functions correct. | Keep the checker for tests and member bugs. |
| "If the field is private, no caller can break the invariant." | A member function can still have a bug; a `friend` or a cast can still write. | State the trust boundary precisely: caller code, not all code. |
| "The 20 traces prove the boundary." | The traces never attempt an illegal write, and they pass with the field public. | Use the change test. |
| "The class is strictly better than the opaque ADT." | The class hides the writers but exposes the layout; the ADT hides the layout but not the writers. | Choose by what you need to hide, and combine them when you need both. |
| "`const` is decoration." | It prevents observers from writing through a `const` reference. | Read the qualifier as part of the contract. |
| "Moving `visited` public only changes visibility." | It changes which states are constructible. The change test shows the behavioral difference. | Treat access specifiers as behavior, not formatting. |
| "A constructor cannot produce an invalid object." | This one value-initializes the array, but a different constructor could forget a field or throw. | Separate construction from validation, and test the constructor. |
| "The checker should repair the board it finds invalid." | A checker that mutates is a second writer, and its caller cannot tell what changed. | Observe in the checker; repair in the operation that broke the rule. |

## Next step

`construction-12` — polymorphism. This lesson made the board a type with a boundary: the state is
private, the operations are public, and the compiler keeps ordinary callers outside. The next
lesson uses that type as the substrate for late binding: a piece interface, two implementations,
and a game loop that depends on the interface rather than on a `switch`.

See the [lesson catalog](../../../docs/guide/lesson-catalog.md) for the full path.

If either baseline was missing, do not start `construction-12`. The next contrast needs both sides.
