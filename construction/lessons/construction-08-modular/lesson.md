# Modular programming (programación modular)

> [English](lesson.md) | [Español](lesson-es.md)

**Concept:** information hiding (ocultamiento de información) · **Time:** about 92 minutes ·
**Status:** drafted · **Prerequisites:** construction-03, construction-07
**Reading:** ~18 minutes

## Why this lesson exists

`construction-07` left the board layout in the open. Four signatures carried
`BoardCell (*)[9]`, `getBoardCell` returned a pointer whose stride only the declaration knew, and
every caller could name the cell struct. The modular baseline moves that knowledge behind three
headers and four translation units. The game is the same: all 20 traces for 8×8 are byte-identical
before you change anything.

Then the change test. Set the active board to 10×10 inside `board.cpp`. One file and two lines
change. `game.cpp` and `main.cpp` are untouched. Only one object recompiles, and the game still
runs. The behavior changes, so the 8×8 traces stop applying. The lesson is not that modular code
preserves traces. It is that the board's size became a fact only the board module owns.

The sentence to keep:

> A public declaration says which names a caller may use and what types they carry; the
> implementation decides the state and the numbers behind those names.

Three honest notes before the comparison.

1. The source grows. The pointers baseline is 381 lines in one file. The modular baseline is 463
   lines across seven source files: four `.cpp` and three `.h`. Add `CMakeLists.txt` and `run.sh`
   and the artifact is larger. Modularity is a boundary, not a diet.
2. The header does not hide the board's existence, the 1-based coordinate convention, or the
   run-time counts. It hides the representation, the storage and the helper functions. A caller
   still knows that a board exists and that `board_row_count()` answers with an `int`.
3. `run.sh` without CMake compiles all four translation units in one command. A naive rebuild
   therefore recompiles everything. To count recompiles you need object files or a build system
   that tracks dependencies. This lesson measures with explicit object files.

## What you will be able to do

By the end of this lesson you can:

- explain information hiding across a header and an implementation;
- change the board from 8x8 to 10x10 and count the touched modules.

## The concept: a declaration is a promise, the .cpp keeps it

A C++ program is compiled one translation unit at a time. A translation unit is one `.cpp` file
after the preprocessor has pasted in every `#include`. The compiler sees declarations from the
headers and definitions from the `.cpp`. It does not see the other `.cpp` files.

```text
main.cpp            includes game.h only
game.cpp            includes game.h, board.h, random_sequence.h
board.cpp           includes board.h only
random_sequence.cpp includes random_sequence.h only
```

That graph is the whole design in four lines. `main.cpp` can call `run_game` because `game.h`
declares it. It cannot name `BoardCell`, `boardCells`, `Knight` or `randomState`, because no header
it includes declares them. The restriction is not a permission system. It is a name-visibility
rule: if the compiler has not seen the declaration, the name does not exist in that translation
unit.

The public surface is nine declarations across three headers:

| Module | Public header | Callers may name | The implementation hides |
|---|---|---|---|
| `random_sequence` | `random_sequence.h` | `random_set_seed`, `random_below`, `random_integer` | multiplier, increment, state |
| `board` | `board.h` | `board_row_count`, `board_column_count`, `board_reset`, `board_was_visited`, `board_mark_visited` | `BoardCell`, `boardCells`, maximum and active dimensions, bounds checks |
| `game` | `game.h` | `run_game` | `Knight`, `MoveOutcome`, move tables, 17 helpers, turn loop, transcript |

The `.cpp` files define 28 functions. Nine are declared in headers, eighteen are internal (17 in
`game.cpp` and `parseArguments` in `main.cpp`), and one is `main`. Those helpers sit in an anonymous
namespace:

```cpp
namespace
{
constexpr int maximumMoveCount = 200;
// ...
}
```

An anonymous namespace gives its names internal linkage. They exist in that translation unit and
nowhere else. At `-O2` many of them disappear entirely; the object file exports only the header's
names plus `main`. The nine declarations are the interface, and the object file shows it.

A declaration is not a definition. The compiler checks that each call matches a declaration it has
seen. The linker checks that every used external name has exactly one definition. If `board.cpp`
loses `board_column_count`, `game.cpp` still compiles, and the link fails with an undefined
reference. That split is what "separate compilation" means: the compiler checks each unit, the
linker joins them.

The board header is small enough to read in one screen:

```cpp
#pragma once

int board_row_count();
int board_column_count();
void board_reset();
bool board_was_visited(int row, int column);
void board_mark_visited(int row, int column);
```

Five names, no struct, no array, no constant. A caller cannot declare a `BoardCell`, cannot index
`boardCells`, cannot read the active dimensions without calling a function. The implementation file
holds all of that:

```cpp
namespace
{
constexpr int maximumBoardRowCount = 16;
constexpr int maximumBoardColumnCount = 16;
constexpr int activeBoardRowCount = 8;
constexpr int activeBoardColumnCount = 8;

struct BoardCell
{
    bool visited;
};

BoardCell boardCells[maximumBoardRowCount][maximumBoardColumnCount] = {};
}
```

Notice what the split did to the dimensions. They are values returned by functions, not types in a
signature. `game.cpp` writes `row <= board_row_count()` and `column <= board_column_count()`. No
signature mentions 8, and no caller has to be recompiled when the active count changes. In
`construction-07`, the width was part of the parameter type, `BoardCell (*)[9]`; every boundary
that received the board repeated it. Here the width is data inside one translation unit.

What the header does **not** do:

- **Does not enforce the contract.** `board_was_visited(0, 0)` returns `false` and
  `board_mark_visited(0, 0)` does nothing. The header cannot stop a caller from trying. Invariants
  are `construction-10` and `construction-11`.
- **Does not hide the coordinate convention.** Callers pass rows and columns from 1, and the board
  silently rejects anything outside the active range. The convention is a comment in the
  implementation, not a fact in the type.
- **Does not make a caller trustworthy.** A caller can include `board.h` and call anything public.
  The boundary is a design convention plus an include graph, not a language lock.
- **Does not prevent a second definition.** Two `.cpp` files defining `board_reset` produce a
  link error, not a design error.
- **Does not make the program smaller.** It makes the unit of change smaller.

## Before you start

- [`construction-03`](../construction-03-functions/lesson.md) — you can read a function
  declaration, a parameter list and a return type. This is the catalog prerequisite.
- [`construction-07`](../construction-07-pointers/lesson.md) — the comparison uses its
  pointers baseline. If you have not read that lesson, read the baseline as a single-file version
  of the same game; the contrast still works.
- A C++20 compiler (`g++` or `clang++`). `cmake` is optional; the measurements below do not need it.

You do not need to know how linkers work in detail. You need to read a header, a `.cpp` file, and
the list of objects a build produces.

### What you need

- The modular baseline, all four translation units and three headers:
  [`code/main.cpp`](code/main.cpp), [`code/game.h`](code/game.h),
  [`code/game.cpp`](code/game.cpp), [`code/board.h`](code/board.h),
  [`code/board.cpp`](code/board.cpp), [`code/random_sequence.h`](code/random_sequence.h),
  [`code/random_sequence.cpp`](code/random_sequence.cpp).
- The previous lesson's single file:
  [`pointers-baseline.cpp`](../construction-07-pointers/code/pointers-baseline.cpp).
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
test -f ../construction-07-pointers/code/pointers-baseline.cpp &&
test -f ../../../common/traces/seed-0001.txt
```

If a file is missing, the comparison has no second side. Record the missing file in `EVIDENCE.md`
§11 and return when it is available. A count against a file you have not read is not evidence.

Both baselines reproduce all 20 traces in `common/traces/` byte for byte, before you change
anything. Same game, one translation unit against four. That is what makes the comparison fair.

## Step 1 — Inventory the interface (~12 min)

Read the three headers first. Then let the text tell you where each definition lives:

```bash
grep -n . code/board.h code/game.h code/random_sequence.h
grep -n '^int \|^void \|^bool \|^std::' code/board.cpp code/random_sequence.cpp code/game.cpp
```

The first command prints every header line. The second prints the function definitions that start
at column zero; the helpers indented inside anonymous namespaces do not match, which is exactly the
point. `run_game` starts at column zero in `game.cpp` and appears; the 17 helpers do not.

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

Fill this table into `EVIDENCE.md` §2.1 while you read.

| Module | Header | Header lines | Declarations | Definitions in | Data hidden |
|---|---|---|---|---|---|
| `random_sequence` | `random_sequence.h` | 1–7 | 3 | `random_sequence.cpp` | multiplier, increment, state |
| `board` | `board.h` | 1–7 | 5 | `board.cpp` | `BoardCell`, array, 4 constants |
| `game` | `game.h` | 1–6 | 1 | `game.cpp` | `Knight`, `MoveOutcome`, 5 constants, 2 tables, 17 helpers |
| `main` | none | — | 0 | `main.cpp` | `parseArguments` |

Counts: 3 headers, 4 translation units, 9 public declarations, 28 definitions, 18 internal
helpers, 1 program entry point.

Then answer in `EVIDENCE.md` §2:

1. Which names does `main.cpp` know? Which names does it not know?
2. Which names does `game.cpp` get from `board.h`, and which board names can it not see?
3. Which four numbers moved from the game code into `board.cpp`?
4. Which functions in `game.cpp` are not declared in any header, and what makes them internal?
5. Could `main.cpp` call `board_reset`? What would it have to add, and why is that a design
   decision rather than a compiler restriction?
6. Does `board_row_count()` return a compile-time constant or a run-time value? What does that
   change about the signatures that use it?
7. `game.cpp` contains the literal `8` in `moveOrderMaximum = 8`. Is that a board dimension?
8. What is the difference between what the compiler checks and what the linker checks?

Question 5 is the honesty check. `main.cpp` *could* include `board.h`; nothing stops it. The
boundary is a convention, held by the include graph and by review, not by the language. Question 7
matters: the only `8` left in `game.cpp` is the number of knight moves, not the board width. Search
for it and prove it.

## Step 2 — Predict before you run (~15 min)

### 2.1 The traces

Write these predictions before running anything. The files are in front of you; the exercise is to
see how much of the behavior you can account for without measuring it.

| Question | My prediction |
|---|---|
| Modular baseline vs `seed-0001.txt` | |
| Pointers baseline vs `seed-0001.txt` | |
| Modular vs pointers, seed 1 | |
| Seeds that match, of 20 | |
| First line that could differ, and why | |

### 2.2 The change surface

You will change `activeBoardRowCount` and `activeBoardColumnCount` in `board.cpp` from 8 to 10.
Predict the cost before you touch the edit.

| Question | My prediction |
|---|---|
| Source files edited | |
| Lines edited | |
| Headers edited | |
| `.cpp` files edited outside `board.cpp` | |
| Objects recompiled (of 4) | |
| Does it compile without other edits? | |
| Seeds that match the 8×8 traces, of 20 | |
| Does the game still run to a winner or a draw? | |

### 2.3 What the headers hide

Predict which of these names a translation unit can use, before checking with the compiler. Write
`yes` or `no`.

| Name | `main.cpp` | `game.cpp` | `board.cpp` |
|---|---|---|---|
| `run_game` | | | |
| `board_column_count` | | | |
| `BoardCell` | | | |
| `boardCells` | | | |
| `Knight` | | | |
| `randomState` | | | |
| `moveOrderMaximum` | | | |

The interesting rows are the ones that are `no` in one unit and `yes` in another. That difference
is the boundary. Write the table in `EVIDENCE.md` §3.3 before compiling.

## Step 3 — Run both baselines (~10 min)

Create the scratch directory first; every command below writes into it.

```bash
mkdir -p /tmp/modular-test
./run.sh --seed 1 --name Learner > /tmp/modular-test/modular-0001.txt
diff -u ../../../common/traces/seed-0001.txt /tmp/modular-test/modular-0001.txt
```

Then the previous baseline:

```bash
cd ../construction-07-pointers
./run.sh --seed 1 --name Learner > /tmp/modular-test/pointers-0001.txt
diff -u ../../../common/traces/seed-0001.txt /tmp/modular-test/pointers-0001.txt
```

Then the two against each other:

```bash
diff -u /tmp/modular-test/pointers-0001.txt /tmp/modular-test/modular-0001.txt
```

Three empty diffs. Return to the modular lesson directory and confirm all 20 seeds, not only the
first:

```bash
cd ../construction-08-modular
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
g++ -std=c++20 -Wall -Wextra -Werror -O2 -Icode \
  -o build/modular-baseline code/main.cpp code/game.cpp code/board.cpp \
  code/random_sequence.cpp
```

If a diff is not empty, stop. Record it in `EVIDENCE.md` §11. Do not adjust a trace to match the
code.

## Step 4 — Map each header to its implementation (~12 min)

The trace comparison says the two versions behave the same. This step says *why* the split is
real, not cosmetic. Compile each translation unit to its own object and ask the object what it
exports.

```bash
mkdir -p /tmp/modular-test/objects
for source in main game board random_sequence; do
  g++ -std=c++20 -Wall -Wextra -Werror -O2 -Icode \
    -c "code/${source}.cpp" -o "/tmp/modular-test/objects/${source}.o"
done
```

Now list the external definitions in each object:

```bash
for object in /tmp/modular-test/objects/*.o; do
  echo "== ${object} =="
  nm -C --defined-only "${object}" | grep ' T '
done
```

Expected shape on this platform:

```text
main.o              main
game.o              run_game(long, std::string const&)
board.o             board_reset, board_row_count, board_was_visited,
                    board_column_count, board_mark_visited
random_sequence.o   random_below, random_integer, random_set_seed
```

`T` means an external definition in the text segment. The board object exports exactly the five
names in `board.h`. The game object exports exactly one name: `run_game`. The 17 helpers are
internal and, at `-O2`, mostly inlined away. This is what "the header is the interface" looks like
in a binary. The `std::string` type is abbreviated above; `nm -C` prints its full template form.

Fill this table in `EVIDENCE.md` §5.1:

| Header | Declarations | Object | External symbols | Hidden names |
|---|---|---|---|---|
| `board.h` | 5 | `board.o` | | |
| `game.h` | 1 | `game.o` | | |
| `random_sequence.h` | 3 | `random_sequence.o` | | |

### 4.1 The linker check

The compiler trusts the declarations. The linker is the check that each used declaration has a
definition. Prove it in a scratch copy; never in the tracked file.

```bash
cp -r code /tmp/modular-test/link-check
cd /tmp/modular-test/link-check
```

Delete the body of `board_column_count` from `board.cpp`, keep the declaration in `board.h`, then
compile all four translation units and link:

```bash
for source in main game board random_sequence; do
  g++ -std=c++20 -Wall -Wextra -Werror -O2 -I. \
    -c "${source}.cpp" -o "${source}.o"
done
g++ -o modular-baseline main.o game.o board.o random_sequence.o
```

The four compilations succeed. The link fails with an undefined reference to
`board_column_count()`. The exact wording depends on the linker and the locale, but the class of
failure is stable: compile-time success, link-time failure. Record the diagnostic in `EVIDENCE.md`
§5.3.

That is the division of labour:

- **Compiler:** each translation unit sees declarations and checks that calls and definitions
  agree with them.
- **Linker:** every used external name has exactly one definition.
- **Neither:** that the call is meaningful, that the row is in range, or that the board was reset.

## Step 5 — Change the board to 10×10 (~20 min)

Copy the baseline. The tracked file stays untouched.

```bash
lessonDirectory="$(pwd)"
cp -r code /tmp/modular-test/board-10
cd /tmp/modular-test/board-10
```

In `board.cpp`, change two lines:

```cpp
constexpr int activeBoardRowCount = 8;      // before
constexpr int activeBoardColumnCount = 8;   // before
```

to:

```cpp
constexpr int activeBoardRowCount = 10;     // after
constexpr int activeBoardColumnCount = 10;  // after
```

Do not change `maximumBoardRowCount` or `maximumBoardColumnCount`. They are 16, and 10 fits. Do not
touch any header, `game.cpp` or `main.cpp`.

### 5.1 Build and count what recompiled

Build four objects from scratch, then edit, then rebuild only the stale ones:

```bash
mkdir -p objects
for source in main game board random_sequence; do
  g++ -std=c++20 -Wall -Wextra -Werror -O2 -I. \
    -c "${source}.cpp" -o "objects/${source}.o"
done
# now apply the two-line edit in board.cpp
for source in main game board random_sequence; do
  if [[ "${source}.cpp" -nt "objects/${source}.o" ]]; then
    echo "compiling ${source}.cpp"
    g++ -std=c++20 -Wall -Wextra -Werror -O2 -I. \
      -c "${source}.cpp" -o "objects/${source}.o"
  fi
done
g++ -o modular-baseline objects/main.o objects/game.o objects/board.o \
  objects/random_sequence.o
```

The only `compiling` line is `compiling board.cpp`. The other three objects keep their timestamps.
The link runs again because the binary depends on all four objects. That is the change surface:
one translation unit, then the link.

This loop is deliberately naive: it watches only `.cpp` timestamps. It is enough here because no
header changes. A real build system also tracks headers; §5.4 shows why that matters.

### 5.2 Run the new game

Run seed 1 and compare with the 8×8 trace. Return to the lesson directory first; the trace paths
are relative to it.

```bash
cd "${lessonDirectory}"
/tmp/modular-test/board-10/modular-baseline --seed 1 --name Learner | head -8
/tmp/modular-test/board-10/modular-baseline --seed 1 --name Learner |
  diff -u ../../../common/traces/seed-0001.txt -
```

For seed 1, the first difference is in the `initial_*` lines. The starting cell is drawn from
`board_row_count() * board_column_count()`, now 100 instead of 64, and the conversion uses the new
column count. The rolls are drawn after the positions, so they and every later draw come from a
different state. The rules did not change; the board did.

Run all 20 seeds against the 8×8 traces:

```bash
for trace in ../../../common/traces/seed-*.txt; do
  traceName="$(basename "${trace}" .txt)"
  seedText="${traceName#seed-}"
  seedNumber="$((10#${seedText}))"
  /tmp/modular-test/board-10/modular-baseline --seed "${seedNumber}" \
    --name Learner | cmp - "${trace}"
done
```

Expect `0` matches out of 20. That is not a regression. The golden traces encode an 8×8 board, and
this probe changes the game on purpose. Record it as a structural observation: the unmodified
baseline matched 20 of 20; the 10×10 probe matches 0 of 20; both are correct for their own rules.

Does the new game finish? Run seed 1 to the end and check the last two lines. It should print a
`winner=` line and a `moves=` line, because the turn loop still ends on capture, trap or the
200-move draw. The behavior changed; the program did not break.

### 5.3 The file-by-file answer

Fill this table in `EVIDENCE.md` §6.3. It is the direct answer to the acceptance criterion.

| File | Edited? | Recompiled? |
|---|---|---|
| `code/board.cpp` | yes, 2 lines | yes |
| `code/board.h` | no | no |
| `code/game.cpp` | no | no |
| `code/game.h` | no | no |
| `code/main.cpp` | no | no |
| `code/random_sequence.cpp` | no | no |
| `code/random_sequence.h` | no | no |

Seven source files, one edited, one recompiled, then the link. `game.cpp` and `main.cpp` need no
edits because they never name the size. They ask `board_row_count()` and `board_column_count()` at
run time.

### 5.4 Change the header instead (the contrast)

The previous measurement changed an implementation file. Now change an interface file in a second
scratch copy and watch the direction reverse.

```bash
cp -r code /tmp/modular-test/header-10
cd /tmp/modular-test/header-10
# build four objects, then add one comment line to board.h
```

After touching `board.h`, rebuild with dependency rules: an object is stale if its `.cpp` is newer
than the object **or** any header it includes is newer. For this project:

| Object | Depends on |
|---|---|
| `main.o` | `main.cpp`, `game.h` |
| `game.o` | `game.cpp`, `game.h`, `board.h`, `random_sequence.h` |
| `board.o` | `board.cpp`, `board.h` |
| `random_sequence.o` | `random_sequence.cpp`, `random_sequence.h` |

Run this loop once to build all four objects, add one comment line to `board.h`, then run it again:

```bash
mkdir -p objects
for source in main game board random_sequence; do
  object="objects/${source}.o"
  stale=0
  [[ "${source}.cpp" -nt "${object}" ]] && stale=1
  case "${source}" in
    main) headers="game.h" ;;
    game) headers="game.h board.h random_sequence.h" ;;
    board) headers="board.h" ;;
    random_sequence) headers="random_sequence.h" ;;
  esac
  for header in ${headers}; do
    [[ "${header}" -nt "${object}" ]] && stale=1
  done
  if [[ "${stale}" -eq 1 ]]; then
    echo "compiling ${source}.cpp"
    g++ -std=c++20 -Wall -Wextra -Werror -O2 -I. \
      -c "${source}.cpp" -o "${object}"
  fi
done
```

The second run compiles `board.cpp` and `game.cpp`; `main.o` and `random_sequence.o` stay. A comment
in `board.h` is enough to force two compilations because the build system is conservative: it does
not read the comment, it reads the timestamp.

This is the rule to carry:

- A change in a `.cpp` recompiles that object.
- A change in a header recompiles every object that includes it, directly or transitively.
- A change in an interface has a wider blast radius than a change in an implementation.

## Step 6 — Compare the change surface (~10 min)

The track's change test asks for four numbers; this table adds two that matter for this change.
Measure them on both baselines.

| Metric | Pointers baseline (one file) | Modular baseline |
|---|---|---|
| Files touched | 1 (`pointers-baseline.cpp`) | 1 (`board.cpp`) |
| Lines changed | 2 | 2 |
| Signatures whose parameter type changes | 4 | 0 |
| New concepts required | 0 | 0 |
| Translation units recompiled | 1 of 1: board, game and `main` together | 1 of 4: `board.o`, then link |
| 8×8 traces broken | 20 | 20 |

Read the translation-units row carefully. Both changes touch one file and two lines, so the line
count does not separate them. The difference is **what is inside the recompiled unit**. In the
single file, the board constants, the game loop and `main` are one translation unit; changing the
board recompiles all of that code. In the modular baseline, the recompiled unit contains the board
and nothing else. `game.cpp` and `main.cpp` are not read by the compiler for this change.

The signature row is the type-level version of the same fact. In the single file, the width is part
of four parameter types, so changing it changes four signatures. In the modular baseline, no
signature mentions the width, so no type changes.

What the change bought:

- A smaller unit of knowledge. The board's representation, storage and dimensions are one file.
- A smaller unit of recompilation. One object instead of the whole game.
- A run-time answer instead of a compile-time fact. Callers ask for the count; no signature carries
  the width.
- A boundary for the next change. If the board's representation changes from an array to a
  bitboard inside `board.cpp`, these callers need no edit. That is the next lesson.

What the change cost:

- 463 lines instead of 381 across seven source files instead of one.
- A build file and a dependency graph to maintain.
- A function call where a constant used to be; the compiler can inline it, but the source no
  longer shows the number.
- A convention that callers must respect: rows and columns start at 1, and out-of-range calls are
  ignored rather than rejected.
- 20 traces that no longer describe this build. A 10×10 game needs its own traces; the old ones are
  still the contract for 8×8.

One more honest measurement. If the change were "replace the array with a linked list or a
bitboard", the single file would touch the board access in `tryMove`, the two writes in `playTurn`,
the declaration in `main` and the parameter types in three signatures. The modular baseline would
touch the five function bodies in `board.cpp`. The header would not change, because it never named
`BoardCell`. That is `construction-09`'s test, and it is the stronger reason for this boundary.

## Step 7 — Compare and explain (~10 min)

Write `EVIDENCE.md` §8. Two parts.

**First, one sentence.** Say what a public declaration decides about a caller, without using the
words *header*, *file*, *module*, *compile*, *better*, *worse*, *clean*, *cleaner*, *nice*,
*elegant*, *ugly* or *modular*. If your sentence needs one of those, you have described your
reaction to the layout rather than the boundary.

**Second, one paragraph.** Answer:

- What a public declaration decides: which names a caller may use and what types they carry.
- What the implementation file hides: representation, storage, constants, helper functions, state.
- What the compiler checks: each translation unit against the declarations it has seen.
- What the linker checks: one definition for every used external name.
- What the build graph shows: which translation units must be recompiled after a change.
- What the interface still exposes: the existence of a board, the coordinate convention, the
  run-time counts, and the names of the five board operations.
- What it does **not** enforce: bounds, invariants, a caller's discipline, or a single definition.
- What only a run proves: the transcript is unchanged before the change, 0 of 20 after, and the
  10×10 game still reaches an ending.
- What modularity cost: 82 more source lines, six more source files, a build graph, and a
  convention that is documented rather than typed.
- Which later lesson makes the representation swap explicit: `construction-09`; invariants:
  `construction-10` and `construction-11`; ownership: `construction-16`; ports and adapters:
  `construction-20`.

A strong answer keeps four things apart: **what the interface permits**, **what the
implementation hides**, **what the toolchain checks**, and **what a build log or a run shows**. A
weak answer says modular code is cleaner, which hides all four.

## Step 8 — Acceptance check and record (~3 min)

The acceptance criterion for this lesson is: **change the board size in the board module and show
that `game.cpp` and `main.cpp` require no edits; explain what the headers hide.**

Check your record against it:

- [ ] I ran both unmodified baselines and got three empty diffs, plus 20 matching seeds.
- [ ] I inventoried the 9 public declarations, 28 definitions and 18 internal helpers.
- [ ] I mapped each header to its object and listed the external symbols with `nm`.
- [ ] I showed that the compiler accepts a missing definition and the linker rejects it.
- [ ] I predicted the traces and the change surface before measuring.
- [ ] I changed two constants in `board.cpp` to 10 and 10.
- [ ] `board.h`, `game.h`, `game.cpp`, `main.cpp` and both random files were not edited.
- [ ] Only `board.o` recompiled, then the link.
- [ ] The 10×10 build runs and reaches a winner or a draw.
- [ ] I got 20 of 20 matches before the change and 0 of 20 after it.
- [ ] I explained why 0 of 20 is expected and not a regression.
- [ ] I showed that a header change recompiles its includers, while a `.cpp` change does not.
- [ ] I measured the change-surface cost on both baselines and said what modularity bought and cost.
- [ ] I explained what the headers hide, what they expose, and what neither tool enforces.
- [ ] My one-sentence explanation avoids the four layout words and the eight verdict words.
- [ ] I did not edit any tracked code file.
- [ ] I completed `EVIDENCE.md` §1–§10 and the self-check in §12.

The criterion is not "more files is better". It is "you can say which translation unit owns the
board's size, which units must be rebuilt when it changes, and which check would notice if a
declaration and its definition disagreed".

## Self-check (binary)

- [ ] I read all seven source files and inventoried the three headers.
- [ ] I predicted the trace results, the edit surface and the recompilation surface before
      measuring.
- [ ] I built and ran both baselines and reproduced all 20 traces.
- [ ] I compiled four objects and inspected the external symbols of each.
- [ ] I showed the compile-versus-link split with a missing definition.
- [ ] I changed the active board to 10×10 in `board.cpp` without editing `game.cpp` or `main.cpp`.
- [ ] I counted one edited file, two edited lines, one recompiled object and a relink.
- [ ] I saw 0 of 20 traces match and explained why that is expected.
- [ ] I contrasted a `.cpp` change with a header change and their different blast radii.
- [ ] I compared the change-surface cost and named what modularity bought and cost.
- [ ] I explained what the headers hide and what they do not enforce.
- [ ] I completed `EVIDENCE.md`.

These are acceptance criteria. If a box is
unchecked, write the next smallest action in `EVIDENCE.md` §10.

## Misconceptions this lesson refutes

| Misconception | Why it fails | What to do instead |
|---|---|---|
| "A header is a copy of the `.cpp`." | A header carries declarations; the bodies stay in the `.cpp`. | Read the header as the list of names a caller may use. |
| "Splitting the file changes behavior." | The unmodified modular baseline reproduces all 20 traces. | Compare the traces before judging the layout. |
| "Headers hide everything." | They expose names, types, the coordinate convention and the existence of a board. | List what a caller can still name and must still know. |
| "Information hiding is security." | A caller can include any header and call any public function. | Treat it as a dependency boundary, not access control. |
| "The compiler enforces the interface." | It checks declarations per translation unit; the linker resolves definitions. | Test both: a type mismatch fails the compiler, a missing body fails the linker. |
| "Changing `board.cpp` forces a full rebuild." | Only its object recompiles, then the link, if dependencies are tracked. | Measure with objects, not with a single-command `run.sh`. |
| "A header change costs the same as a `.cpp` change." | Every includer recompiles; a comment in `board.h` rebuilds `game.o` too. | Use `g++ -MM` to see the dependency edges before editing. |
| "The 10×10 change should keep the traces." | The traces encode 8×8 behavior; the starting draws and bounds changed. | Keep 20/20 before, expect 0/20 after, and say why. |
| "Modular code is smaller." | 463 source lines against 381, plus a build file. | Count the lines and files, not only the concepts. |
| "The board size is hidden in the header." | The header exposes `board_row_count()` and `board_column_count()` as run-time values. | Say that the number is hidden, not the fact that a count exists. |
| "One definition is enough because it compiles." | A missing definition compiles and fails at link. | Build and link the whole program before calling the interface complete. |
| "The anonymous namespace is decoration." | It gives internal linkage; without it the helpers would be external names. | Read it as "this name belongs to this translation unit". |

## Next step

`construction-09` — abstract data type. The board already hides its representation behind five
functions, but the header still speaks in rows and columns and the caller still owns the coordinate
convention. The next lesson makes the type opaque and gives the board its own bounds, so the `1..N`
stops being a fact the caller must remember.

See the [lesson catalog](../../../docs/guide/lesson-catalog.md) for the full path.

If either baseline was missing, do not start `construction-09`. The next contrast needs both sides.
