# Abstract data type (tipo abstracto de datos, TAD)

> [English](lesson.md) | [Español](lesson-es.md)

**Concept:** representation independence (independencia de representación) · **Time:** about 90
minutes · **Status:** drafted
**Reading:** ~21 minutes

## Why this lesson exists

`construction-08` moved the board behind five functions. That was a real boundary: `game.cpp`
could not name `BoardCell`, `boardCells`, or the two dimension constants. But the board was still
not a value. One array sat in static storage inside `board.cpp`, and the five functions took no
parameters. A caller could not hold a board, pass one to a function, or make a second one. The board
was a set of global functions over a global object.

This baseline changes that. `board.h` declares a name and stops:

```cpp
struct Board;
```

No fields, no size, no array. Seven operations take `const Board *` or `Board *`. The definition of
the type lives in `board.cpp`, next to the storage. The game is byte-identical: all 20 traces for
8×8 match before you change anything.

Then the change test. Replace the array inside `board.cpp` with a flat `std::vector` and an index
function. One file changes. `board.h`, `game.cpp` and `main.cpp` are untouched. All 20 traces still
pass, because the layout never left `board.cpp` in the first place.

The sentence to keep:

> A type is abstract when its declaration gives the type a name but not a size, and every
> operation takes that type as a parameter.

Three honest notes before the comparison.

1. The source grows a little. `construction-08` is 463 lines across seven source files; this
   baseline is 479. `board.h` goes from 7 to 11 lines, `board.cpp` from 57 to 65, `game.cpp` from
   284 to 288. Most of the growth is one parameter per function.
2. The swap is not free at the binary level. The object file for `board.cpp` carries 504 bytes of
   text with the array, 1311 with a flat vector, and 4172 with a vector of vectors. The exported
   names are identical in all three; the cost of the operations is not.
3. The 20 traces do not verify everything. Remove the initialization from `board_create` and the
   traces still pass, because `board_reset` runs first and allocates. A trace is a behavioral
   contract, not a memory-safety contract. `construction-10` and `construction-11` close that gap.

## What you will be able to do

By the end of this lesson you can explain representation independence and apply the lesson's change
test.

## The concept: the header names the type, the .cpp gives it a size

A declaration is not a definition. `struct Board;` is a declaration with a semicolon after the
name. It tells the compiler three things and withholds one:

```text
tells:      a type named Board exists, at namespace scope
tells:      the name can appear in declarations
tells:      Board is a struct, not a union, enum or class
withholds:  how many bytes a Board occupies, and what is inside
```

That is an **incomplete type**. The C++ standard calls it exactly that, and the restrictions follow
from the missing size. With an incomplete type you can:

- declare a pointer to it: `Board *board;`
- declare a reference to it: `Board &board = *pointer;`
- declare a function that takes or returns a pointer to it;
- compare two pointers to it, or test one against `nullptr`.

With an incomplete type you cannot:

- declare an object of it: `Board board;` needs the size to reserve stack space;
- take `sizeof(Board)`;
- access a member: `board->visited` needs the layout to compute an offset;
- index it or copy it by value;
- `delete` a pointer to it: the compiler cannot know whether a destructor exists.

Read that list again. Every forbidden operation is a way to learn the layout. Every allowed
operation is a way to hold the object without learning it. That is the entire mechanism of an
abstract data type in C++, and it is enforced by the compiler, not by a convention or a code
review.

The seven declarations are the whole public surface:

```cpp
struct Board;

Board *board_create();
void board_destroy(Board *board);
int board_row_count(const Board *board);
int board_column_count(const Board *board);
void board_reset(Board *board);
bool board_was_visited(const Board *board, int row, int column);
void board_mark_visited(Board *board, int row, int column);
```

| Operation | Parameter | Why that form |
|---|---|---|
| `board_create` | none | Returns `Board *`, never `Board`. A function cannot return an incomplete type by value: the caller would need the size to reserve space for the result. A pointer has a known size whatever it points at. |
| `board_destroy` | `Board *` | Frees the object. The definition is visible inside `board.cpp`, so `delete` is legal there and illegal in every caller. |
| `board_row_count` | `const Board *` | Reads. `const` says the operation does not modify the object. |
| `board_column_count` | `const Board *` | Reads. |
| `board_reset` | `Board *` | Writes to every cell. |
| `board_was_visited` | `const Board *` | Reads one cell. |
| `board_mark_visited` | `Board *` | Writes one cell. |

The `const` is not decoration. `board_was_visited` receives a `const Board *`, so inside
`board.cpp` it cannot assign through that pointer. If someone later writes an assignment there, the
compiler rejects it. `game.cpp` passes the same pointer to `board_was_visited` and to
`board_mark_visited`; the first call cannot change the object, the second can. That distinction is
checked at the call site and inside the implementation.

Now the honest part. What did this buy over `construction-08`?

| | `construction-08` | `construction-09` |
|---|---|---|
| Type name in the header | none | `Board`, incomplete |
| Operations | 5 | 7 |
| Operations that take a board | 0 | 7 |
| Boards alive at once | 1 | any number |
| Board storage | static array in `board.cpp` | heap object via `new` / `delete` |
| Board lifetime | whole program | from `board_create` to `board_destroy` |
| Caller can hold a board | no | yes, as `Board *` |
| Caller can see the layout | no | no |

The representation swap is **not** what separates the two. `construction-08` could also replace
its array with a vector inside `board.cpp` and no caller would notice, because the array was already
in an anonymous namespace. For a single board, the swap was already safe.

What the ADT adds is **multiplicity, lifetime, and a value to pass**. In `construction-08` the board
is a singleton in static storage; a second game in the same process would share it. Here each
`board_create` returns a separate object, and the caller decides when it dies. That is what lets a
test build a fresh board instead of resetting the global one, and it is what makes ownership
(`construction-16`) a question worth asking.

What the header does **not** do:

- **Does not enforce a null contract.** `board_row_count(nullptr)` returns 8 in this
  implementation, because the parameter is unnamed and ignored. `board_reset(nullptr)` dereferences
  and is undefined. Neither call is checked.
- **Does not prevent copying the handle.** `Board *second = board;` compiles. Two handles now own
  one object, and destroying both is a double free. Nothing in the header says "one owner".
- **Does not enforce the invariants.** `board_mark_visited(board, 0, 0)` still does nothing,
  silently. Bounds are `construction-10` and `construction-11`.
- **Does not hide the object's identity.** A caller can store the pointer in its own struct, compare
  two pointers, and pass the handle around. The layout is hidden; the address is not.
- **Does not make the operations cheap.** The vector versions below cost more text and more
  indirection than the array. Representation independence is a design property, not a performance
  property.

## Before you start

- [`construction-08`](../construction-08-modular/lesson.md) — the catalog prerequisite. You need
  its modular board in front of you, because the whole lesson is a diff against it.
- [`construction-03`](../construction-03-functions/lesson.md) — you can read a function
  declaration, a parameter list and a return type.
- A C++20 compiler (`g++` or `clang++`). `cmake` is optional; `run.sh` falls back to a direct
  compile when CMake is absent.

You do not need to know name mangling, the ABI, or how `new` finds memory. You need to read a
header, a `.cpp` file, and one compiler diagnostic.

### What you need

- The ADT baseline:
  [`code/main.cpp`](code/main.cpp), [`code/game.h`](code/game.h),
  [`code/game.cpp`](code/game.cpp), [`code/board.h`](code/board.h),
  [`code/board.cpp`](code/board.cpp), [`code/random_sequence.h`](code/random_sequence.h),
  [`code/random_sequence.cpp`](code/random_sequence.cpp).
- The previous baseline, for the comparison:
  [`../construction-08-modular/code/board.h`](../construction-08-modular/code/board.h) and
  [`../construction-08-modular/code/board.cpp`](../construction-08-modular/code/board.cpp).
- Build scripts and notes: [`run.sh`](run.sh), [`CMakeLists.txt`](CMakeLists.txt) and
  [`README.md`](README.md).
- The rules: [`common/spec/game-rules.md`](../../../common/spec/game-rules.md) — §12 fixes the
  order of random draws.
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
test -f ../construction-08-modular/code/board.cpp &&
test -f ../../../common/traces/seed-0001.txt
```

If a file is missing, the comparison has no second side. Record the missing file in `EVIDENCE.md`
§11 and return when it is available. A count against a file you have not read is not evidence.

Both baselines reproduce all 20 traces in `common/traces/` byte for byte, before you change
anything. Same game, five free functions against seven operations on a handle. That is what makes
the comparison fair.

## Step 1 — Inventory the incomplete type (~12 min)

Read `board.h` first, then let the compiler tell you where the definition is.

```bash
grep -n . code/board.h
grep -rn 'struct Board' code/
```

The first command prints 11 lines. The second prints two hits: the forward declaration in
`board.h` and the definition in `board.cpp`. Two hits, two different files, and that is the design.

Now the include graph, as the compiler sees it:

```bash
g++ -std=c++20 -Icode -MM code/main.cpp
g++ -std=c++20 -Icode -MM code/game.cpp
g++ -std=c++20 -Icode -MM code/board.cpp
g++ -std=c++20 -Icode -MM code/random_sequence.cpp
```

The output is short:

```text
main.o: code/main.cpp code/game.h
game.o: code/game.cpp code/game.h code/board.h code/random_sequence.h
board.o: code/board.cpp code/board.h
random_sequence.o: code/random_sequence.cpp code/random_sequence.h
```

`game.cpp` includes `board.h`, so it sees `Board` and the seven operations. It does not include
`board.cpp`, so it does not see the definition. That single missing `#include` is the abstraction.

Fill this table into `EVIDENCE.md` §2.1 while you read.

| Header | Lines | Declarations | Incomplete type | Definition in |
|---|---|---|---|---|
| `board.h` | 1–11 | 7 | `Board` | `board.cpp` |
| `game.h` | 1–6 | 1 | none | `game.cpp` |
| `random_sequence.h` | 1–7 | 3 | none | `random_sequence.cpp` |
| `main` | no header | 0 | none | `main.cpp` |

Counts: 3 headers, 4 translation units, 11 public declarations, 1 incomplete type, 30 definitions,
18 internal helpers (17 in `game.cpp`, 1 in `main.cpp`), 1 program entry point.

The 30 definitions break down as 7 in `board.cpp`, 18 in `game.cpp`, 3 in `random_sequence.cpp`,
and 2 in `main.cpp`. Compare with `construction-08`: 28 definitions, 9 public declarations. The ADT
added two public declarations and two definitions, and changed the signature of five.

Then answer in `EVIDENCE.md` §2:

1. Which names does `main.cpp` know?
2. Which names does `game.cpp` get from `board.h`, and which board names can it still not see?
3. How many bytes does `Board` occupy, as far as `game.cpp` knows?
4. In which file is `struct Board` defined, and what is inside it?
5. What did `board_create` add that `construction-08` did not have at all?
6. Which two operations take a non-`const` `Board *`, and why?
7. `board_row_count` does not use its parameter. Is that a bug?
8. What is the difference between "the caller cannot see the layout" and "the caller cannot see the
   object"?

Question 3 has a precise answer: zero information. `game.cpp` can write `sizeof(Board *)`, which is
the size of a pointer, but not `sizeof(Board)`, which would need the size of the object.

Question 7 is the honesty check. It is not a bug for this game, because the counts are compile-time
constants in `board.cpp`. But it is a signal: a constant wearing a method's clothes. The moment the
board becomes configurable (`construction-15`), the parameter starts being used, and the signature
does not change.

Question 8 separates two different claims. The caller cannot read the bytes. The caller can copy the
address, compare it, store it, and pass it. Hiding the layout is not hiding the object.

## Step 2 — Predict before you run (~15 min)

### 2.1 The traces

Write these predictions before running anything. Both files are in front of you; the exercise is to
see how much of the behavior you can account for without measuring it.

| Question | My prediction |
|---|---|
| ADT baseline vs `seed-0001.txt` | |
| Modular baseline vs `seed-0001.txt` | |
| ADT vs modular, seed 1 | |
| Seeds that match, of 20, for each baseline | |
| First line that could differ, and why | |

The last row is a trap. There is no reason for any line to differ: the rules, the draw order and
the board dimensions are unchanged. The only thing that moved is where the array lives. If a line
differs, you have found a bug in one of the two baselines, not a property of the design.

### 2.2 The swap surface

You will replace the array inside `board.cpp` with a flat `std::vector` plus an index function.
Predict the cost before you touch the edit.

| Question | My prediction |
|---|---|
| Source files edited | |
| Lines edited | |
| Headers edited | |
| `.cpp` files edited outside `board.cpp` | |
| Objects recompiled (of 4) | |
| Does it compile without other edits? | |
| Seeds that match, of 20 | |
| Does the object file grow, shrink or stay the same size? | |

The size question is the one most people get wrong. A vector needs a pointer, a size and a
capacity, and every access goes through a call that the optimizer may or may not remove. Predict
the direction before you measure it.

### 2.3 What the header permits

Predict which of these expressions compiles in each translation unit, before checking with the
compiler. Write `yes`, `no`, or `link error`.

| Expression | `main.cpp` | `game.cpp` | `board.cpp` |
|---|---|---|---|
| `Board *board = board_create();` | | | |
| `sizeof(Board)` | | | |
| `board->visited` | | | |
| `Board board;` | | | |
| `delete board;` | | | |
| `board_was_visited(board, 1, 1)` | | | |
| `Board *copy = board;` | | | |

`main.cpp` includes only `game.h`, so it does not even see the name `Board`. That row is `no` for a
different reason than the others: not an incomplete type, but an unknown name.

Write the table in `EVIDENCE.md` §3.3 before compiling. The interesting rows are the ones that are
`no` in one unit and `yes` in another.

## Step 3 — Run both baselines (~10 min)

Create the scratch directory first; every command below writes into it.

```bash
mkdir -p /tmp/adt-test
./run.sh --seed 1 --name Learner > /tmp/adt-test/adt-0001.txt
diff -u ../../../common/traces/seed-0001.txt /tmp/adt-test/adt-0001.txt
```

Then the previous baseline:

```bash
cd ../construction-08-modular
./run.sh --seed 1 --name Learner > /tmp/adt-test/modular-0001.txt
diff -u ../../../common/traces/seed-0001.txt /tmp/adt-test/modular-0001.txt
```

Then the two against each other:

```bash
diff -u /tmp/adt-test/modular-0001.txt /tmp/adt-test/adt-0001.txt
```

Three empty diffs. Return to this lesson directory and confirm all 20 seeds on the ADT baseline:

```bash
cd ../construction-09-abstract-data-type
for trace in ../../../common/traces/seed-*.txt; do
  traceName="$(basename "${trace}" .txt)"
  seedText="${traceName#seed-}"
  seedNumber="$((10#${seedText}))"
  ./run.sh --seed "${seedNumber}" --name Learner | cmp - "${trace}"
done
```

A silent loop is 20 matches. Run the same loop in `../construction-08-modular` if you have not
already done it in that lesson.

If `run.sh` cannot build, compile once by hand:

```bash
mkdir -p build
g++ -std=c++20 -Wall -Wextra -Werror -O2 -Icode \
  -o build/adt-baseline code/main.cpp code/game.cpp code/board.cpp \
  code/random_sequence.cpp
```

If a diff is not empty, stop. Record it in `EVIDENCE.md` §11. Do not adjust a trace to match the
code.

## Step 4 — Prove the type is incomplete (~12 min)

The traces say the two versions behave the same. This step says the abstraction is real, not a
naming convention. Write the probe in the scratch directory; never in the tracked files.

```bash
mkdir -p /tmp/adt-test/probe
cat > /tmp/adt-test/probe/probe.cpp <<'EOF'
#include "board.h"

int main()
{
    Board *board = board_create();
    int size = static_cast<int>(sizeof(Board));
    bool visited = board->visited;
    Board local;
    delete board;
    (void)local;
    return size + (visited ? 1 : 0);
}
EOF
g++ -std=c++20 -Wall -Wextra -Werror -Icode \
  -c /tmp/adt-test/probe/probe.cpp -o /tmp/adt-test/probe/probe.o
```

At least four diagnostics, one per forbidden operation: `sizeof` on an incomplete type, member
access on an incomplete type, a local object of an incomplete type, and `delete` on a pointer to an
incomplete type. The `delete` case usually produces two, because the compiler also reports that the
pointee has no definition. A note points at the forward declaration in `board.h`. The exact wording
depends on the compiler and the locale; record the class of each diagnostic, not the sentence.

Now compile the real `board.cpp` to an object and ask what it exports:

```bash
mkdir -p /tmp/adt-test/objects
g++ -std=c++20 -Wall -Wextra -Werror -O2 -Icode \
  -c code/board.cpp -o /tmp/adt-test/objects/board.o
nm -C --defined-only /tmp/adt-test/objects/board.o | grep ' T '
```

Expected shape on this platform:

```text
board_create()
board_destroy(Board*)
board_row_count(Board const*)
board_column_count(Board const*)
board_reset(Board*)
board_was_visited(Board const*, int, int)
board_mark_visited(Board*, int, int)
```

Seven symbols, exactly the seven declarations. No `Board` layout appears anywhere in the object's
external surface. The type is incomplete in the header and complete in one object file, and the
object exports only the operations.

### 4.1 The mangled names, and why the swap is safe

`nm` without `-C` shows the mangled names:

```bash
nm --defined-only /tmp/adt-test/objects/board.o | grep ' T '
```

```text
_Z12board_createv
_Z13board_destroyP5Board
_Z15board_row_countPK5Board
_Z18board_column_countPK5Board
_Z11board_resetP5Board
_Z17board_was_visitedPK5Boardii
_Z18board_mark_visitedP5Boardii
```

Read the encoding: `P5Board` is "pointer to a type whose name is `Board` and whose length is 5".
`K` marks `const`. `ii` marks two `int` parameters. Nothing in the name records a size, a field, or
an array dimension. That is why the linker has nothing to renegotiate when the representation
changes: the symbol a caller references is the same string before and after the swap.

Compare with `construction-08`:

```text
_Z11board_resetv
_Z15board_row_countv
_Z17board_was_visitedii
_Z18board_column_countv
_Z18board_mark_visitedii
```

Five symbols, and the `v` means "no parameters". The board was not in the signature at all. After
this lesson, it is in every signature, as a name without a size.

Fill this table in `EVIDENCE.md` §5.2:

| Header | Declarations | Object | External symbols | Layout in the symbol name? |
|---|---|---|---|---|
| `board.h` | 7 | `board.o` | | |
| `game.h` | 1 | `game.o` | | |
| `random_sequence.h` | 3 | `random_sequence.o` | | |

The last column is the point of the step. If any exported symbol encoded the layout, the swap would
change the ABI and every caller would have to be recompiled and relinked. It does not.

## Step 5 — Swap the representation (~25 min)

Copy the baseline. The tracked files stay untouched.

```bash
lessonDirectory="$(pwd)"
cp -r code /tmp/adt-test/flat
cd /tmp/adt-test/flat
```

Replace `board.cpp` with this file. `board.h`, `game.cpp`, `main.cpp` and both random files are not
touched.

```cpp
#include "board.h"

#include <vector>

namespace
{
constexpr int activeBoardRowCount = 8;
constexpr int activeBoardColumnCount = 8;

int cellIndex(int row, int column)
{
    return row * (activeBoardColumnCount + 1) + column;
}
}

struct Board
{
    std::vector<unsigned char> visited;
};

Board *board_create()
{
    Board *board = new Board{};
    board->visited.assign((activeBoardRowCount + 1) * (activeBoardColumnCount + 1), 0);
    return board;
}

void board_destroy(Board *board)
{
    delete board;
}

int board_row_count(const Board *)
{
    return activeBoardRowCount;
}

int board_column_count(const Board *)
{
    return activeBoardColumnCount;
}

void board_reset(Board *board)
{
    board->visited.assign((activeBoardRowCount + 1) * (activeBoardColumnCount + 1), 0);
}

bool board_was_visited(const Board *board, int row, int column)
{
    if (row < 1 || row > activeBoardRowCount || column < 1 ||
        column > activeBoardColumnCount)
    {
        return false;
    }
    return board->visited[cellIndex(row, column)] != 0;
}

void board_mark_visited(Board *board, int row, int column)
{
    if (row < 1 || row > activeBoardRowCount || column < 1 ||
        column > activeBoardColumnCount)
    {
        return;
    }
    board->visited[cellIndex(row, column)] = 1;
}
```

Three things changed inside the file, and nothing else:

1. The storage: `bool visited[16][16]` became `std::vector<unsigned char>`. The maximum dimension
   constants disappeared entirely, because a vector grows. The active counts stayed.
2. A new internal helper `cellIndex` maps a row and a column to a flat index. The stride is
   `activeBoardColumnCount + 1`, that is 9, so row 1 column 1 is index 10 and row 8 column 8 is
   index 80. The `+ 1` keeps row 0 and column 0 reserved and unused, exactly like the array.
3. `board_create` and `board_reset` now allocate. A default-constructed vector is empty, so
   `assign` is what makes the object valid.

Why `unsigned char` and not `bool`: `std::vector<bool>` is a bit-packed specialization. It works,
but `board->visited[index]` returns a proxy object instead of a `bool &`, which breaks the moment
you try to bind a reference. `unsigned char` keeps each element a real byte. The nested variant
below uses `std::vector<bool>` deliberately, to show the difference is invisible to callers.

### 5.1 Build and run the flat variant

```bash
g++ -std=c++20 -Wall -Wextra -Werror -O2 -I. \
  -o flat-baseline main.cpp game.cpp board.cpp random_sequence.cpp
cd "${lessonDirectory}"
for trace in ../../../common/traces/seed-*.txt; do
  traceName="$(basename "${trace}" .txt)"
  seedText="${traceName#seed-}"
  seedNumber="$((10#${seedText}))"
  /tmp/adt-test/flat/flat-baseline --seed "${seedNumber}" --name Learner | cmp - "${trace}"
done
```

A silent loop is 20 of 20. Same transcript, different storage.

### 5.2 A second representation: vector of vectors

Copy the baseline again and use a different layout. This one is closer to the original, because it
keeps two subscripts.

```bash
cp -r "${lessonDirectory}/code" /tmp/adt-test/nested
cd /tmp/adt-test/nested
```

Only three regions of `board.cpp` change:

```cpp
struct Board
{
    std::vector<std::vector<bool>> visited;
};

Board *board_create()
{
    Board *board = new Board{};
    board->visited.assign(activeBoardRowCount + 1,
                          std::vector<bool>(activeBoardColumnCount + 1, false));
    return board;
}

void board_reset(Board *board)
{
    for (std::vector<bool> &rowCells : board->visited)
    {
        rowCells.assign(rowCells.size(), false);
    }
}
```

`board_was_visited` and `board_mark_visited` keep their bounds checks and read
`board->visited[row][column]` instead of `board->visited[cellIndex(row, column)]`. The `maximum*`
constants disappear here too.

```bash
g++ -std=c++20 -Wall -Wextra -Werror -O2 -I. \
  -o nested-baseline main.cpp game.cpp board.cpp random_sequence.cpp
cd "${lessonDirectory}"
for trace in ../../../common/traces/seed-*.txt; do
  traceName="$(basename "${trace}" .txt)"
  seedText="${traceName#seed-}"
  seedNumber="$((10#${seedText}))"
  /tmp/adt-test/nested/nested-baseline --seed "${seedNumber}" --name Learner | cmp - "${trace}"
done
```

Silent again: 20 of 20. Two different layouts, one unchanged transcript.

### 5.3 What the object shows

Build all three `board.cpp` files to objects and compare:

```bash
for variant in code /tmp/adt-test/flat /tmp/adt-test/nested; do
  name="$(basename "${variant}")"
  g++ -std=c++20 -O2 -I"${variant}" -c "${variant}/board.cpp" \
    -o "/tmp/adt-test/objects/${name}-board.o"
done
size /tmp/adt-test/objects/*-board.o
for name in code flat nested; do
  nm --defined-only "/tmp/adt-test/objects/${name}-board.o" | grep ' T ' |
    awk '{print $3}' > "/tmp/adt-test/${name}.sym"
done
diff /tmp/adt-test/code.sym /tmp/adt-test/flat.sym &&
  diff /tmp/adt-test/code.sym /tmp/adt-test/nested.sym &&
  echo "identical symbols"
```

The `awk` drops the address column, which differs because each object lays out its own text
section. Compare names, not addresses.

Two results, both worth recording:

- The exported symbols are byte-identical across the three objects. The swap is invisible to the
  linker.
- The text size is not. On this platform: 504 bytes for the array, 1311 for the flat vector, 4172
  for the vector of vectors. The abstraction costs allocation and indirection, and it does not hide
  that cost from the program's running time.

Record both numbers in `EVIDENCE.md` §6.3. A design that hides a cost from the source still pays it
at run time.

### 5.4 The trap the traces cannot catch

Delete the `assign` from `board_create` in the flat variant, keeping it in `board_reset`:

```cpp
Board *board_create()
{
    return new Board{};
}
```

Rebuild and rerun the 20 traces. They still pass, all 20. The reason is the call order in
`run_game`: `board_create()`, then `board_reset(board)`, then the first read. `board_reset`
allocates, so the empty vector never reaches an accessor.

The trace is a behavioral contract. It says the transcript matches. It does not say the object was
ever valid between `create` and `reset`. In the array version, `new Board{}` zero-initializes the
array, so a read before `reset` would be defined and return `false`. In the flat version it is an
out-of-bounds access on an empty vector, which is undefined behavior that happens not to crash.
Same traces, different contract.

Record this in `EVIDENCE.md` §6.5. It is the reason `construction-10` exists: an abstract data type
without an invariant is a layout you cannot see, not a guarantee you can rely on.

### 5.5 The file-by-file answer

Fill this table in `EVIDENCE.md` §6.4. It is the direct answer to the acceptance criterion.

| File | Edited? | Recompiled? |
|---|---|---|
| `code/board.cpp` | yes, the representation | yes |
| `code/board.h` | no | no |
| `code/game.cpp` | no | no |
| `code/game.h` | no | no |
| `code/main.cpp` | no | no |
| `code/random_sequence.cpp` | no | no |
| `code/random_sequence.h` | no | no |

Seven source files, one edited, one recompiled, then the link. `game.cpp` needs no edit because it
never wrote `board->visited`; it only called the operations. That is the acceptance criterion, and
it is a claim about a translation unit, not about good taste.

## Step 6 — Compare the change surface (~10 min)

The track's change test asks for four numbers. This table adds the ones that matter for this change.

| Metric | `construction-08` (modular) | `construction-09` (ADT) |
|---|---|---|
| Files touched for the swap | 1 (`board.cpp`) | 1 (`board.cpp`) |
| Lines changed | 25 | 25 flat, 17 nested |
| Signatures changed | 0 | 0 |
| New concepts required | 0 | 0 |
| Translation units recompiled | 1 of 4 | 1 of 4 |
| 8×8 traces broken | 0 | 0 |
| Boards alive at once | 1 | any number |
| Board lifetime | static, whole program | explicit `create` / `destroy` |
| Operations that take a board | 0 of 5 | 7 of 7 |
| A test can build a fresh board | no, only reset the global | yes |

Read the first six rows carefully. They are **ties**. For this particular change, the modular board
and the ADT board cost the same: one file, about the same number of lines, no signature change, no
caller recompilation, no broken trace. If the lesson ended here, the ADT would look like a
refactor with extra ceremony.

The last four rows are where the difference lives. They are not about the swap; they are about what
the swap is safe for. In `construction-08` the array was private to `board.cpp`, so swapping it was
already safe for the one board that existed. In `construction-09` the swap is safe for every board
the program can create, because no caller ever had a way to see one.

What the ADT bought:

- A value. The board can be passed, stored, compared and created on demand.
- Multiplicity. Two boards in one process no longer collide.
- An explicit lifetime. The object begins at `board_create` and ends at `board_destroy`, and the
  gap between those two calls is now a design question instead of a static fact.
- A test seam. A test can build a fresh 8×8 board instead of resetting a global.

What the ADT cost:

- One parameter on seven operations, and 56 changed lines in `game.cpp` to thread the handle
  through six signatures and nineteen call sites. Measure it yourself with
  `diff -u ../construction-08-modular/code/game.cpp code/game.cpp`.
- Two new operations and one incomplete type in the header: 4 more header lines.
- An ownership obligation with no enforcement. The caller must call `board_destroy`, and nothing
  checks that it does. A thrown exception between `create` and `destroy` leaks the board.
- A null-pointer surface. Some operations survive `nullptr`, some do not, and the header does not
  say which.
- More text per operation. 504 bytes became 1311, or 4172.

The honest conclusion: for a single board in a single-threaded program, the ADT is a small cost
with no measurable benefit. Its benefit appears the moment a second board exists, or a test needs
one, or a caller needs to own one. That is why the next lesson is about invariants and the one after
that about ownership: an opaque type without those is a promise with nothing behind it.

## Step 7 — Compare and explain (~10 min)

Write `EVIDENCE.md` §8. Two parts.

**First, one sentence.** Say what the declaration decides about a caller, without using the words
*header*, *file*, *module*, *compile*, *pointer*, *struct*, *class*, *opaque*, *encapsulation*,
*better*, *worse*, *clean*, *cleaner*, *nice*, *elegant*, *ugly* or *modular*. If your sentence
needs one of those, you have described the syntax rather than the decision.

**Second, one paragraph.** Answer:

- What the declaration fixes: the name of the type, the seven operations, and which of them may
  write.
- What it withholds: the size, the fields, the storage, the maximum dimensions, the index function.
- What the compiler enforces: no object, no `sizeof`, no member access, no `delete` outside
  `board.cpp`.
- What the linker sees: the same seven symbol names before and after the swap.
- What a run proves: 20 of 20 before the swap, 20 of 20 after it, with two different layouts.
- What a run does **not** prove: that the object was valid between `create` and `reset`, that the
  handle has one owner, or that a null handle is rejected.
- What multiplicity adds: any number of boards, each with its own lifetime.
- What it costs: one parameter per operation, 56 changed lines in `game.cpp`, an ownership
  obligation, a null surface, and a larger object file.
- Which later lesson adds invariants: `construction-10`; encapsulation: `construction-11`; ownership
  and lifetime: `construction-16`; ports and adapters: `construction-20`.

A strong answer keeps four things apart: **what the declaration permits**, **what the definition
hides**, **what the toolchain checks**, and **what a run or a size measurement shows**. A weak
answer says the type is encapsulated, which hides all four.

## Step 8 — Acceptance check and record (~3 min)

The acceptance criterion for this lesson is: **swap the board representation inside `board.cpp` and
show that every caller still compiles and all 20 traces still pass.**

Check your record against it:

- [ ] I ran both unmodified baselines and got three empty diffs, plus 20 matching seeds each.
- [ ] I inventoried the 11 public declarations and located the one incomplete type.
- [ ] I showed at least one compiler diagnostic for each forbidden operation on `Board`.
- [ ] I listed the seven exported symbols of `board.o` and read the mangled names.
- [ ] I explained why no symbol name encodes the layout.
- [ ] I predicted the traces and the swap surface before measuring.
- [ ] I replaced the array with a flat vector and an index function.
- [ ] I replaced it again with a vector of vectors, to show the choice is internal.
- [ ] `board.h`, `game.h`, `game.cpp`, `main.cpp` and both random files were not edited.
- [ ] Only `board.o` recompiled, then the link.
- [ ] I got 20 of 20 matches before the swap and 20 of 20 after it.
- [ ] I recorded the object size for all three representations and did not call the swap free.
- [ ] I removed the initialization from `board_create` and explained why the traces still pass.
- [ ] I measured the change surface on both baselines and said where they tie and where they do not.
- [ ] I explained what the declaration permits, what it hides, and what neither tool enforces.
- [ ] My one-sentence explanation avoids the six syntax words and the eight verdict words.
- [ ] I did not edit any tracked code file.
- [ ] I completed `EVIDENCE.md` §1–§10 and the self-check in §12.

The criterion is not "an opaque type is better". It is "you can say which translation unit owns the
layout, which check would notice if a caller tried to read it, and what the swap does not prove".

## Self-check (binary)

- [ ] I read all seven source files and compared `board.h` with `construction-08`'s version.
- [ ] I predicted the traces, the swap surface and the visibility table before measuring.
- [ ] I built and ran both baselines and reproduced all 20 traces on each.
- [ ] I compiled a probe that fails on `sizeof`, member access and `delete`.
- [ ] I inspected the seven exported symbols and their mangled names.
- [ ] I swapped the representation twice, inside `board.cpp` only.
- [ ] I counted one edited file, one recompiled object and a relink.
- [ ] I saw 20 of 20 traces match after both swaps.
- [ ] I recorded the object size for the array and both vectors.
- [ ] I found the initialization that the traces cannot check.
- [ ] I compared the change surface and named the rows that tie.
- [ ] I completed `EVIDENCE.md`.

These are acceptance criteria. If a box is
unchecked, write the next smallest action in `EVIDENCE.md` §10.

## Misconceptions this lesson refutes

| Misconception | Why it fails | What to do instead |
|---|---|---|
| "A forward declaration is enough to use the type." | It is enough to hold a pointer or a reference, not to create, size, copy or delete the object. | List the five forbidden operations and check each one with the compiler. |
| "The header hides the type." | It hides the layout. The name, the address and the seven operations are public. | Say which facts are hidden and which are not. |
| "The ADT is what makes the representation swap possible." | `construction-08` could already swap its private array with no caller edit. | Compare the two baselines and find the rows that tie. |
| "An opaque type prevents misuse." | `board_reset(nullptr)` and a double `board_destroy` are still reachable. | Treat the invariant as `construction-10`'s job and ownership as `construction-16`'s. |
| "`const Board *` makes the object immutable." | It makes this call unable to write through this pointer. Another handle can still write. | Read `const` as a promise about the parameter, not about the object's lifetime. |
| "`board_row_count` needs the board." | The parameter is unnamed and ignored; the count is a file-scope constant. | Notice the smell, and expect it to disappear when the board becomes configurable. |
| "The swap is free because the source did not change." | The object file grew from 504 to 1311 or 4172 bytes of text. | Measure with `size`, not with the diff. |
| "20 of 20 traces means the swap is correct." | The traces passed with an uninitialized vector in `board_create`. | Add the invariant check; a trace is not a memory-safety test. |
| "The mangled name changes when the layout changes." | `P5Board` encodes the name and the pointer, not the size. | Read one mangled name and point at the missing layout. |
| "Copying the handle is copying the board." | `Board *second = board;` copies the address. Both handles now own one object. | Say "handle" and "object" separately, and let `construction-16` fix it. |
| "The ADT makes the code faster or smaller." | The vector versions are larger and allocate. | Separate the design property from the performance property. |
| "A struct with hidden fields is an ADT." | If the caller can `sizeof` it, put it on the stack, or copy it, the layout is public. | Test the five forbidden operations; that is the definition. |

## Next step

`construction-10` — abstract data type invariants. The board is now opaque, but nothing stops
`board_create` from returning an object that was never initialized, and nothing stops a caller from
destroying a handle twice. The next lesson gives the type a validity rule and a lifecycle, so the
promise the header makes has something behind it.

See the [lesson catalog](../../../docs/guide/lesson-catalog.md) for the full path.

If either baseline was missing, do not start `construction-10`. The next contrast needs both sides.
