# Generic classes and concepts (clases genéricas y conceptos)

> [English](lesson.md) | [Español](lesson-es.md)

**Concept:** constraints · **Time:** about 81 minutes · **Prerequisites:** construction-14 ·
**Status:** drafted
**Reading:** ~21 minutes
**Lesson role:** core

## Why this lesson exists

`construction-14` ended with one generic function. `generate_move_candidates` accepted any type with
`row()` and `column()`, and the game loop still named `Knight` in ten signatures. The board was a
concrete class with two constants, `activeRowCount` and `activeColumnCount`, fixed at 8.

This baseline moves the dimensions into the type. `Board` and `GameEngine` are class templates:

```cpp
template <int RowCount, int ColumnCount>
class Board
{
    ...
};

template <int RowCount, int ColumnCount>
class GameEngine
{
    ...
};
```

`game.cpp` instantiates `GameEngine<8, 8>`. A scratch program instantiates `GameEngine<10, 10>`
from the same source. The piece contract is named by a C++20 concept:

```cpp
template <typename PieceType>
concept PositionedPiece = requires(PieceType piece, const PieceType constPiece) {
    { constPiece.row() } -> std::convertible_to<int>;
    { constPiece.column() } -> std::convertible_to<int>;
    { piece.set_position(1, 1) } -> std::same_as<void>;
    { constPiece.role_name() } -> std::convertible_to<const char *>;
};

static_assert(PositionedPiece<Knight>);
```

The sentence to keep:

> A class template is a pattern for a class: the compiler writes one class for each set of
> arguments. A concept names the operations a type must provide, and the compiler checks them where
> the type is used.

Three honest notes before the comparison.

1. The source grows by 11 lines, from 587 across twelve files to 598 across twelve. The shape
   changes more than the count: `board.cpp` (65 lines) disappears, `game.cpp` falls from 281 to 10,
   `board.h` grows from 21 to 76, and `game_engine.h` is new at 292.
2. The linked text shrinks by 86 bytes, from 9,496 to 9,410, and data stays at 692. The object file
   for `game.cpp` grows by 479 bytes, from 3,913 to 4,392, because the engine's code now lives in a
   header and is emitted in the unit that instantiates it. The direction depends on which unit you
   measure.
3. The engine is generic over dimensions, not over the piece type. `GameEngine` still declares
   `Knight playerKnight;` and `Knight machineKnight;`, and six member signatures name `Knight`.
   The concept checks `Knight` once, with `static_assert`; it does not constrain a `PieceType`
   parameter, because there is no such parameter. That gap is the lesson's limit, and Step 6
   names it.

The comparison with `construction-14` is not "class templates are better". It is more precise: the
class template buys one board and one engine for any dimensions in 1..16, and it pays with one class
per argument list, a header-visible body, a fixed 16×16 storage, and a piece type that is still
concrete. This lesson measures both sides.

## What you will be able to do

By the end of this lesson you can:

- explain constraints on a class template;
- instantiate the same engine for 8x8 and 10x10 boards.

## The concept: a class with parameters

`construction-14` asked: can one function work for more than one piece type? The answer was yes, if
the piece type becomes a parameter of the function. The loop still named `Knight`, and the board was
still an 8×8 class.

This lesson asks a different question: can the board and the engine be written once for more than
one board size, without copying the turn loop?

Yes, if the dimensions become parameters of the class. `Board` does not name 8. It names `RowCount`
and `ColumnCount`, placeholders that the compiler replaces with values at each instantiation:

```cpp
template <int RowCount, int ColumnCount>
class Board;
```

A class template is not a class. It is a pattern. The compiler produces a class when it sees a use
with concrete arguments. `Board<8, 8>` and `Board<10, 10>` are two distinct types generated from the
same 76 lines. The same is true for `GameEngine<8, 8>` and `GameEngine<10, 10>`.

### Three kinds of template parameter

| Kind | Syntax | What varies | Where it appears here |
|---|---|---|---|
| Type parameter | `typename PieceType` | a type | `generate_move_candidates<PieceType>` |
| Non-type parameter | `int RowCount` | a compile-time value | `Board<RowCount, ColumnCount>` |
| Constrained type parameter | `PositionedPiece PieceType` | a type that satisfies a concept | not used by `GameEngine`; only a `static_assert` |

The first kind appeared in `construction-14`. The second kind is this lesson. The third kind is
available in C++20 and is the natural next step, but this baseline does not use it as a parameter.
That distinction matters: a concept can be a constraint on a template parameter, or it can be a
standalone compile-time check. Here it is the second.

### What is generic, what is constrained

| Question | Answer |
|---|---|
| What is generic? | `RowCount` and `ColumnCount`; the board and the engine source. |
| What is constrained? | The dimensions, to 1..16, and the piece type, by `PositionedPiece`. |
| What changes with dimensions? | Bounds, cell count, initial positions, legal moves, transcript. |
| What does not change with dimensions? | Storage, `sizeof`, the rules, the move geometry, the piece type. |
| What is still concrete? | `Knight`, the 16×16 maximum, the 200-move draw, the offset arrays. |

Two constraints, two mechanisms:

1. **Dimensions.** `board.h:72–73` contains two `static_assert`s:
   `RowCount >= 1 && RowCount <= maximumRowCount`, and the same for columns. This is a local range
   check on non-type parameters. It is not a concept.
2. **Piece operations.** `game_engine.h:14–20` defines `PositionedPiece`, and line 22 checks
   `static_assert(PositionedPiece<Knight>)`. This is a named, reusable predicate on a type. It is a
   concept, but it is used as a standalone check, not as a template parameter constraint.

What each one catches:

- `GameEngine<17, 8>` fails at `board.h:72` with `static assertion failed`.
- `GameEngine<0, 8>` fails at the same line.
- `static_assert(PositionedPiece<IncompletePawn>)` fails with `constraints not satisfied`.
- A type with `row()` and `column()` but no `set_position` fails the concept.

What neither one catches:

- A type that satisfies `PositionedPiece` but moves like a bishop. The concept names operations, not
  semantics.
- A legal dimension that is too small to play well, such as 1×1. The `static_assert` allows it; the
  game may trap immediately. That is a semantic limit, not a type error.

### What changes with dimensions

| Thing | 8×8 | 10×10 | Why |
|---|---|---|---|
| Active cells | 64 | 100 | `RowCount * ColumnCount` |
| Board storage | 256 bytes | 256 bytes | fixed `bool[16][16]` |
| `sizeof(Board)` | 256 | 256 | same storage |
| `sizeof(GameEngine)` | 336 | 336 | board plus two knights |
| Initial player (seed 1) | `8,5` | `2,3` | `random_below(64)` vs `random_below(100)` |
| Initial machine (seed 1) | `4,4` | `2,10` | same reason |
| Rolls (seed 1) | `11,20` | `11,20` | same number of draws, same LCG state |
| Turns (seed 1) | 22 | 27 | different legal moves |
| Attempts (seed 1) | 36 | 55 | different board |
| Winner (seed 1) | machine | machine | same seed, different game |
| Moves (seed 1) | 21 | 27 | different game |
| Transcript lines (seed 1) | 89 | 118 | different game |
| Golden traces | 20 of 20 | none | no 10×10 contract |

The rolls are identical because `run()` draws two cell numbers and then two rolls. The number of
draws does not depend on the board size, so the LCG state at the third draw is the same. Only the
bounds change, and with them the cell numbers.

### The piece contract is named, not implied

In `construction-14`, the generator's requirements lived only in its body:

```cpp
piece.row() + knightRowChangePerMove[index]
piece.column() + knightColumnChangePerMove[index]
```

A bad type produced a cascade of errors inside `move_generator.h`. In this baseline, the same
operations are written once, in a concept:

```cpp
template <typename PieceType>
concept PositionedPiece = requires(PieceType piece, const PieceType constPiece) {
    { constPiece.row() } -> std::convertible_to<int>;
    { constPiece.column() } -> std::convertible_to<int>;
    { piece.set_position(1, 1) } -> std::same_as<void>;
    { constPiece.role_name() } -> std::convertible_to<const char *>;
};
```

The concept adds two requirements that the generator did not need: `set_position` and `role_name`.
They are required by the engine, not by the generator. That is the first sign that the contract is
now the engine's contract, not the generator's.

`static_assert(PositionedPiece<Knight>)` makes the check part of the build. If `Knight` loses an
operation, the build stops at line 22 with one message. That is the improvement over the cascade.

The limit is the next paragraph. A concept describes the shape of a type. It does not describe what
the operations mean, and it does not make the engine accept a second piece type. `GameEngine` still
declares two `Knight` members. The concept is a check, not a parameter.

## Before you start

- [`construction-14`](../construction-14-generic-functions/lesson.md) — the baseline this lesson
  starts from. Its `game.cpp` is the left side of the structural diff in Step 6.
- [`construction-11`](../construction-11-encapsulation/lesson.md) — the class boundary. `Position`
  and `Knight` keep private state and public operations. `Board` keeps its storage private too.
- [`construction-09`](../construction-09-abstract-data-type/lesson.md) — representation
  independence. The board's public operations are the same; only their definitions moved.
- [`construction-04`](../construction-04-records/lesson.md) — records. `MoveCandidate` is still a
  record with three public fields.
- A C++20 compiler (`g++` or `clang++`) with concepts support. `cmake` is optional; `run.sh` falls
  back to a direct compile when CMake is absent.

You do not need to know how a compiler instantiates a class template. You need to read one template,
instantiate it with two argument lists, and compare the generated programs.

### What you need

- The generic-classes baseline:
  [`code/main.cpp`](code/main.cpp), [`code/game.h`](code/game.h),
  [`code/game.cpp`](code/game.cpp), [`code/game_engine.h`](code/game_engine.h),
  [`code/board.h`](code/board.h), [`code/move_generator.h`](code/move_generator.h),
  [`code/knight.h`](code/knight.h), [`code/knight.cpp`](code/knight.cpp),
  [`code/position.h`](code/position.h), [`code/position.cpp`](code/position.cpp),
  [`code/random_sequence.h`](code/random_sequence.h),
  [`code/random_sequence.cpp`](code/random_sequence.cpp).
- The previous baseline, for the diff:
  [`code/game.cpp`](../construction-14-generic-functions/code/game.cpp) and its eleven companions in
  [`../construction-14-generic-functions/code/`](../construction-14-generic-functions/code/).
- Build scripts and notes: [`run.sh`](run.sh), [`CMakeLists.txt`](CMakeLists.txt) and
  [`README.md`](README.md).
- The rules: [`common/spec/game-rules.md`](../../../common/spec/game-rules.md) — §1 fixes the board
  and its 1-based numbering, §4 fixes the knight move order, §8 fixes when a cell becomes visited.
- The transcript format:
  [`common/spec/transcript-format.md`](../../../common/spec/transcript-format.md).
- One golden trace: [`common/traces/seed-0001.txt`](../../../common/traces/seed-0001.txt).
- This learner template: [`EVIDENCE.md`](EVIDENCE.md).

Check that the artifacts are present before you begin, from this lesson directory:

```bash
test -f code/main.cpp &&
test -f code/game.h &&
test -f code/game.cpp &&
test -f code/game_engine.h &&
test -f code/board.h &&
test -f code/move_generator.h &&
test -f code/knight.h &&
test -f code/knight.cpp &&
test -f code/position.h &&
test -f code/position.cpp &&
test -f code/random_sequence.h &&
test -f code/random_sequence.cpp &&
test -f ../construction-14-generic-functions/code/game.cpp &&
test -f ../../../common/traces/seed-0001.txt
```

If a file is missing, the comparison has no second side. Record the missing file in `EVIDENCE.md`
§11 and return when it is available. A count against a file you have not read is not evidence.

The 8×8 baseline reproduces all 20 traces in `common/traces/` byte for byte, before you change
anything. That is the behavioral contract for 8×8. There is no golden trace for 10×10; the second
instantiation is a smoke test, not a behavioral contract.

## Step 1 — Inventory the templates (~12 min)

Read the two headers first. The shape is the lesson:

```bash
grep -n 'template\|static_assert\|class Board\|class GameEngine\|GameEngine<' \
  code/board.h code/game_engine.h code/game.cpp
```

```text
code/board.h:3:template <int RowCount, int ColumnCount>
code/board.h:4:class Board
code/board.h:72:    static_assert(RowCount >= 1 && RowCount <= maximumRowCount);
code/board.h:73:    static_assert(ColumnCount >= 1 && ColumnCount <= maximumColumnCount);
code/game_engine.h:14:template <typename PieceType>
code/game_engine.h:22:static_assert(PositionedPiece<Knight>);
code/game_engine.h:34:template <int RowCount, int ColumnCount>
code/game_engine.h:35:class GameEngine
code/game.cpp:8:    GameEngine<8, 8> engine;
```

Now the line map. Fill it into `EVIDENCE.md` §2.2 from the source, not from this table.

`board.h`:

| Lines | Contents |
|---|---|
| 1 | `#pragma once` |
| 3–4 | The template header and `class Board`. |
| 7–10 | Default constructor; value-initializes `visited`. |
| 12–20 | `row_count()` and `column_count()` return the parameters. |
| 22–37 | `is_valid()` scans the maximum storage. |
| 39–48 | `reset()` clears the active area. |
| 50–57 | `was_visited()` bounds-checks against the parameters. |
| 59–66 | `mark_visited()` bounds-checks against the parameters. |
| 68–75 | Private maximum storage, the dimension checks, the `visited` array. |
| 72–73 | The two dimension constraints. |
| 75 | `bool visited[16][16]`. |

`game_engine.h`:

| Lines | Contents |
|---|---|
| 1–12 | Includes: board, knight, generator, random sequence, standard headers. |
| 14–20 | `PositionedPiece`, the named piece contract. |
| 22 | `static_assert(PositionedPiece<Knight>)`. |
| 24–30 | `MoveOutcome`, the four outcomes. |
| 32 | `maximumMoveCount = 200`. |
| 34–36 | The template header and `class GameEngine`. |
| 38–41 | Constructor builds the two knights. |
| 43–60 | `run()`: seed, reset, positions, rolls, header, game. |
| 62–65 | Private data: one board and two knights. |
| 67–75 | Cell-number conversion uses `column_count()`. |
| 77–90 | Initial positions use `row_count() * column_count()`. |
| 92–100 | Rolls. |
| 102–112 | Game header. |
| 114–121 | Move order. |
| 123–138 | Turn header. |
| 140–158 | Attempt printers. |
| 160–170 | Move printer. |
| 172–185 | Knight selection and movement. |
| 187–208 | `tryMove` bounds-checks against the parameters. |
| 210–276 | `playTurn`. |
| 278–291 | `playGame`. |

Counts for this baseline. The counting rule is stated so you can reproduce it:

- Headers: 7 (`board.h`, `game.h`, `game_engine.h`, `knight.h`, `move_generator.h`, `position.h`,
  `random_sequence.h`).
- Translation units: 5 (`main.cpp`, `game.cpp`, `knight.cpp`, `position.cpp`,
  `random_sequence.cpp`).
- Complete class definitions: 5 (`Board`, `GameEngine`, `Knight`, `Position`, `MoveCandidate`).
- Incomplete types: 0.
- Function definitions: 41.
- Internal helpers in `game_engine.h`: 17.
- Internal helper in `main.cpp`: 1.
- Program entry points: 1.
- Private data declarations in headers: 10.
- Public declarations, counted as public member functions (constructors included), public data
  fields, free functions, and namespace-scope concepts and enums: 27.

Compare with `construction-14`:

- Headers: 6 → 7.
- Translation units: 6 → 5.
- Complete class definitions: 4 → 5.
- Function definitions: 39 → 41.
- Private data declarations in headers: 9 → 10.
- Public declarations: 23 → 27.
- Source lines: 587 → 598.
- Files: 12 → 12.
- `board.cpp`: 65 lines → absent.
- `board.h`: 21 lines → 76.
- `game.cpp`: 281 lines → 10.
- `game_engine.h`: absent → 292.

Then answer in `EVIDENCE.md` §2:

1. Which file holds `Board`, and on which lines?
2. Which file holds `GameEngine`, and on which lines?
3. What kind of template parameters does each declare? Type or non-type?
4. Where is `PositionedPiece`, and what four operations does it require?
5. Where is `static_assert(PositionedPiece<Knight>)`?
6. Does `GameEngine` take a piece type as a parameter? What does it declare instead?
7. How many member signatures in `game_engine.h` name `Knight`?
8. Where is `GameEngine<8, 8>` instantiated?
9. Where are the dimension constraints, and what range do they allow?
10. What happened to `board.cpp`?

Question 3 has a precise answer: both declare two non-type `int` parameters, `RowCount` and
`ColumnCount`. Neither declares a type parameter. `PositionedPiece` is a type parameter only inside
its own concept definition.

Question 6 has a precise answer: no. `GameEngine` declares `Knight playerKnight;` and
`Knight machineKnight;` at lines 64–65.

Question 7 has a precise answer: six member signatures name `Knight` — `printTurnHeader`,
`printMove`, `selectActiveKnight`, `selectOtherKnight`, `moveKnight` and `tryMove`. `playTurn` uses
`Knight` pointers as locals, but its signature does not name the type. The two data members are
separate.

Question 10 has a precise answer: it was removed. A class template needs its definitions visible at
the point of instantiation, so `Board`'s definitions moved from `board.cpp` into `board.h`.

## Step 2 — Predict before you run (~12 min)

### 2.1 The dimension table

Write these predictions before running anything. Seed 1, name `Learner`.

| Question | My prediction for 8×8 | My prediction for 10×10 | Confidence |
|---|---|---|---|
| Initial player position | | | |
| Initial machine position | | | |
| Rolls | | | |
| Turns | | | |
| Attempts | | | |
| Winner | | | |
| Moves | | | |
| Transcript lines | | | |
| `sizeof(Board)` | | | |
| `sizeof(GameEngine)` | | | |
| Compiles? | | | |
| Same output as 8×8? | | | |

The rows to take seriously are the positions, the turns and the sizes. A board with 36 more cells
changes the random cell numbers, so the initial positions change. It does not change the storage,
because the array is still 16×16. Predict both, and say why.

### 2.2 The change test

The change is: instantiate the same engine for 10×10, from a scratch file, without editing the
baseline.

| Question | My prediction |
|---|---|
| Files I will add | |
| Files I will edit | |
| Lines in the new file | |
| New concepts the change requires | |
| Seeds that match, of 20, after the change | |
| 10×10 compiles? | |
| 10×10 output identical to 8×8? | |
| Tracked files I will touch | |

The sixth and seventh rows are the acceptance. Predict "compiles" and "different" before you
measure. If the 10×10 output were identical, the dimensions would not be doing anything.

### 2.3 Instantiation and sizes

| Question | My prediction |
|---|---|
| Instantiations of `GameEngine` in the 8×8 game | |
| Instantiations of `GameEngine` in a unit with both sizes | |
| `nm -C both.o` symbols for `GameEngine<8, 8>`, at `-O0` | |
| The same count for `GameEngine<10, 10>` | |
| `GameEngine<8, 8>` symbols at `-O2` | |
| `sizeof(Board<8, 8>)` | |
| `sizeof(Board<10, 10>)` | |
| `sizeof(GameEngine<8, 8>)` | |
| `sizeof(GameEngine<10, 10>)` | |
| Indirect calls in `both.o` | |

The symbol counts are the mechanism test. A class template that is inlined has fewer symbols; a
class template that is not inlined has one set per argument list. Predict both counts before you
compile anything.

## Step 3 — Run the baseline and the second instantiation (~12 min)

First the traces, unmodified:

```bash
mkdir -p /tmp/generic-classes-test
./run.sh --seed 1 --name Learner > /tmp/generic-classes-test/baseline-0001.txt
diff -u ../../../common/traces/seed-0001.txt /tmp/generic-classes-test/baseline-0001.txt
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
  -o build/generic-classes-baseline code/main.cpp code/game.cpp code/knight.cpp \
  code/position.cpp code/random_sequence.cpp
```

Now the 10×10 probe. It goes in the scratch directory, never in the tracked files:

```bash
cat > /tmp/generic-classes-test/ten.cpp <<'EOF'
#include "game_engine.h"

int main()
{
    GameEngine<10, 10> engine;
    return engine.run(1, "Learner");
}
EOF
g++ -std=c++20 -Wall -Wextra -Werror -O2 -Icode \
  -o /tmp/generic-classes-test/ten \
  /tmp/generic-classes-test/ten.cpp code/knight.cpp code/position.cpp \
  code/random_sequence.cpp
/tmp/generic-classes-test/ten > /tmp/generic-classes-test/ten-0001.txt
```

The output on this platform:

```text
$ wc -l /tmp/generic-classes-test/baseline-0001.txt \
        /tmp/generic-classes-test/ten-0001.txt
  89 /tmp/generic-classes-test/baseline-0001.txt
 118 /tmp/generic-classes-test/ten-0001.txt
$ head -n 8 /tmp/generic-classes-test/ten-0001.txt
seed=1
player=Learner
roll_player=11
roll_machine=20
start=machine
initial_player=2,3
initial_machine=2,10
turn=1 who=machine at=2,10 order=1,2,3,4,5,6,7,8
$ tail -n 2 /tmp/generic-classes-test/ten-0001.txt
winner=machine
moves=27
```

The 8×8 output starts with `initial_player=8,5` and `initial_machine=4,4`, and ends with `moves=21`.
The rolls are the same. The positions are not. The board changed, so the game changed.

Now the sizes:

```bash
cat > /tmp/generic-classes-test/sizes.cpp <<'EOF'
#include "game_engine.h"

#include <cstdio>

int main()
{
    std::printf("sizeof(Board<8,8>)=%zu\n", sizeof(Board<8, 8>));
    std::printf("sizeof(Board<10,10>)=%zu\n", sizeof(Board<10, 10>));
    std::printf("sizeof(GameEngine<8,8>)=%zu\n", sizeof(GameEngine<8, 8>));
    std::printf("sizeof(GameEngine<10,10>)=%zu\n", sizeof(GameEngine<10, 10>));
    std::printf("sizeof(Knight)=%zu\n", sizeof(Knight));
    std::printf("sizeof(MoveCandidate)=%zu\n", sizeof(MoveCandidate));
    return 0;
}
EOF
g++ -std=c++20 -Wall -Wextra -Werror -O2 -Icode \
  -o /tmp/generic-classes-test/sizes \
  /tmp/generic-classes-test/sizes.cpp code/knight.cpp code/position.cpp
/tmp/generic-classes-test/sizes
```

```text
sizeof(Board<8,8>)=256
sizeof(Board<10,10>)=256
sizeof(GameEngine<8,8>)=336
sizeof(GameEngine<10,10>)=336
sizeof(Knight)=40
sizeof(MoveCandidate)=12
```

The two board types have the same size because the storage is fixed at 16×16. The parameters change
the active area, not the array. That is the first surprise for most learners: "10×10" does not mean
"100 bytes".

Now put both instantiations in one translation unit and count the symbols:

```bash
cat > /tmp/generic-classes-test/both.cpp <<'EOF'
#include "game_engine.h"

int main()
{
    GameEngine<8, 8> eight;
    GameEngine<10, 10> ten;
    return eight.run(1, "Learner") + ten.run(1, "Learner");
}
EOF
g++ -std=c++20 -Wall -Wextra -Werror -O0 -Icode -c \
  /tmp/generic-classes-test/both.cpp -o /tmp/generic-classes-test/both.o
nm -C /tmp/generic-classes-test/both.o | grep -c 'GameEngine<8, 8>'
nm -C /tmp/generic-classes-test/both.o | grep -c 'GameEngine<10, 10>'
```

```text
24
24
```

At `-O2`, the same unit has one symbol for each engine, `playTurn`, and no `Board` symbols, because
the rest was inlined:

```bash
g++ -std=c++20 -Wall -Wextra -Werror -O2 -Icode -c \
  /tmp/generic-classes-test/both.cpp -o /tmp/generic-classes-test/both-O2.o
nm -C /tmp/generic-classes-test/both-O2.o | grep -c 'GameEngine<8, 8>'
nm -C /tmp/generic-classes-test/both-O2.o | grep -c 'GameEngine<10, 10>'
nm -C /tmp/generic-classes-test/both-O2.o | grep -c 'Board<8, 8>'
```

```text
1
1
0
```

If the first diff is not empty, stop. That is a different baseline from the one this lesson
describes; record it in `EVIDENCE.md` §11 and do not adjust the prediction to match.

## Step 4 — Compare: what the class templates prove (~10 min)

Open `board.h`, `game_engine.h` and `game.cpp`. Five facts, read against the two instantiations.

1. **One source, two classes.** At `-O0`, `nm` shows 24 symbols for `GameEngine<8, 8>` (22 weak,
   2 local) and 24 for `GameEngine<10, 10>`. At `-O2`, one each. The source has one `GameEngine`;
   the object file has one set of members per argument list, or fewer after inlining.
2. **The dimensions are compile-time.** There is no constructor argument for rows or columns, no
   `resize`, and no branch on dimensions. `row_count()` returns `RowCount`; `column_count()` returns
   `ColumnCount`.
3. **Storage is fixed.** `Board<8, 8>` and `Board<10, 10>` are both 256 bytes, because `visited` is
   `bool[16][16]`. The parameters change the active area, not the storage. `GameEngine` is 336 bytes
   for both sizes: 256 for the board plus 80 for two knights.
4. **The 8×8 transcript is identical to `construction-14`.** The 10×10 transcript differs because
   the board changed: 27 turns instead of 22, 55 attempts instead of 36, 27 moves instead of 21,
   118 lines instead of 89.
5. **The total binary text shrinks by 86 bytes**, from 9,496 to 9,410, and data stays at 692.
   `game.o` grows by 479 bytes, from 3,913 to 4,392, because the engine is emitted in the unit that
   instantiates it. Indirect calls stay at 0 on both sides.

The direction of the size change depends on the unit. The total binary is the honest measure of the
program; `game.o` is the honest measure of the instantiation cost. Say which one you mean.

### 4.1 What the compiler emits

Compile `game.cpp` alone on both sides:

```bash
g++ -std=c++20 -O2 -Icode -c code/game.cpp -o /tmp/generic-classes-test/game15.o
g++ -std=c++20 -O2 -I../construction-14-generic-functions/code \
  -c ../construction-14-generic-functions/code/game.cpp \
  -o /tmp/generic-classes-test/game14.o
size /tmp/generic-classes-test/game14.o /tmp/generic-classes-test/game15.o
nm -C /tmp/generic-classes-test/game15.o | grep -c 'GameEngine<8, 8>'
objdump -d /tmp/generic-classes-test/game15.o | grep -c 'call.*\*'
```

With this compiler and `-O2`, the `nm` count is 1 (the `playTurn` symbol) and the indirect-call
count is 0. At `-O0`, the same `game.cpp` emits 24 symbols for `GameEngine<8, 8>` (22 weak, 2
local) and 8 for `Board<8, 8>` (7 weak, 1 local). Record both in `EVIDENCE.md` §5.2.

The presence of one symbol at `-O2` is not a property of class templates. It is an optimization
decision. A larger body, a debug build or a different compiler can leave more members out of line.
The structural claim is narrower and true in both cases: the concrete arguments are fixed at compile
time, and there is no runtime dispatch to resolve.

One build note: `run.sh` compares every `.h` file in `code/` against the binary and rebuilds when
one is newer. Editing `game_engine.h` or `board.h` triggers a rebuild; you do not need to delete
`build/` by hand.

### 4.2 The concept: one clear message, no runtime cost

The concept's contract is checked where the type is used. See it pass and fail in the scratch
directory:

```bash
cat > /tmp/generic-classes-test/probe/concept.cpp <<'EOF'
#include "game_engine.h"

#include <cstdio>

struct GoodPawn
{
    int rowValue = 1;
    int columnValue = 1;

    int row() const { return rowValue; }
    int column() const { return columnValue; }
    void set_position(int row, int column) { rowValue = row; columnValue = column; }
    const char *role_name() const { return "pawn"; }
};

struct MissingSetter
{
    int row() const { return 1; }
    int column() const { return 1; }
    const char *role_name() const { return "pawn"; }
};

static_assert(PositionedPiece<GoodPawn>);
static_assert(!PositionedPiece<MissingSetter>);

int main()
{
    std::printf("good=%d missing=%d knight=%d\n", PositionedPiece<GoodPawn>,
                PositionedPiece<MissingSetter>, PositionedPiece<Knight>);
    return 0;
}
EOF
g++ -std=c++20 -Wall -Wextra -Werror -O2 -Icode \
  -o /tmp/generic-classes-test/probe/concept \
  /tmp/generic-classes-test/probe/concept.cpp code/knight.cpp code/position.cpp
/tmp/generic-classes-test/probe/concept
```

```text
good=1 missing=0 knight=1
```

Now the failure. A type that does not satisfy the concept:

```bash
cat > /tmp/generic-classes-test/probe/missing_operations.cpp <<'EOF'
#include "game_engine.h"

struct IncompletePawn
{
};

static_assert(PositionedPiece<IncompletePawn>);

int main()
{
    return 0;
}
EOF
g++ -std=c++20 -Wall -Wextra -Werror -O2 -Icode -c \
  /tmp/generic-classes-test/probe/missing_operations.cpp \
  -o /tmp/generic-classes-test/probe/missing_operations.o
```

The compiler reports, at the `static_assert`:

```text
error: static assertion failed
  • constraints not satisfied
  • required by the constraints of 'template<class PieceType> concept PositionedPiece'
```

One message at the check, plus the requirements. Compare it with `construction-14`, where the same
kind of type produced a cascade inside `move_generator.h`. The concept moves the diagnostic to the
declaration. That is the gain.

There is no runtime cost. A `requires` expression is unevaluated, and a `static_assert` is evaluated
at compile time. The concept adds no object, no vtable and no branch. `objdump` shows no indirect
calls.

### 4.3 What the concept does not check

`PositionedPiece` checks that the operations exist and return compatible types. It does not check:

- that `row()` and `column()` return the piece's current square;
- that `set_position` updates the same state that `row()` and `column()` read;
- that `role_name()` returns a stable string;
- that the piece moves like a knight;
- that the type can be constructed from a role name.

The last point matters for the next change. `GameEngine` constructs `Knight("player")` and
`Knight("machine")` at lines 39. `PositionedPiece` does not require a constructor from
`std::string`. A fully generic engine must either add that requirement to the concept, or accept
the pieces from the caller. This baseline avoids the question by hardcoding `Knight`.

## Step 5 — Run the change test (~15 min)

The change test is the one the acceptance names: instantiate the same engine for 10×10. The baseline
is not edited. The new file is a scratch file.

```bash
lessonDirectory="$(pwd)"
rm -rf /tmp/generic-classes-test/change
mkdir -p /tmp/generic-classes-test/change
cp -r code /tmp/generic-classes-test/change/code
cd /tmp/generic-classes-test/change
```

### 5.1 Add the second instantiation

```bash
cat > ten.cpp <<'EOF'
#include "game_engine.h"

int main()
{
    GameEngine<10, 10> engine;
    return engine.run(1, "Learner");
}
EOF
```

That is 9 lines in one new file. It includes the same header the 8×8 game includes. It names no
board type directly; the engine owns the board.

### 5.2 Build and run

```bash
g++ -std=c++20 -Wall -Wextra -Werror -O2 -Icode \
  -o ten ten.cpp code/knight.cpp code/position.cpp code/random_sequence.cpp
./ten > ten-0001.txt
cd "${lessonDirectory}"
diff -u /tmp/generic-classes-test/baseline-0001.txt \
        /tmp/generic-classes-test/change/ten-0001.txt | head -n 20
```

The diff is not empty. That is the point. The 8×8 output and the 10×10 output share the seed, the
player name and the rolls, and differ in the positions, the turns and the moves.

### 5.3 Count what changed

| | 8×8 | 10×10 | Difference |
|---|---|---|---|
| Scratch source lines | 0 | 9 | +9 |
| Tracked files edited | 0 | 0 | 0 |
| Instantiations of `GameEngine` | 1 | 1 | 0 |
| Board active cells | 64 | 100 | +36 |
| Board storage bytes | 256 | 256 | 0 |
| Turns (seed 1) | 22 | 27 | +5 |
| Attempts (seed 1) | 36 | 55 | +19 |
| Moves (seed 1) | 21 | 27 | +6 |
| Transcript lines (seed 1) | 89 | 118 | +29 |
| Traces matched | 20 of 20 | not compared | — |

The change costs one new file and zero edits to the baseline. It buys a second board size from the
same engine source. It does not buy a second piece type.

### 5.4 The limit of the acceptance

The acceptance says: instantiate the same engine for 8×8 and 10×10. It does not say the engine is
generic over pieces, and it is not. `GameEngine` still has `Knight` members and six `Knight`
signatures. The 10×10 instantiation proves the dimensions are parameters. It does not prove the
piece type is.

Measure that change honestly in `EVIDENCE.md` §6.3: making the piece type a parameter would touch
the template header, the two data members, the constructor, six signatures and the places that call
`Knight` operations. The concept is ready for that change; the engine is not using it as a parameter
yet.

## Step 6 — Compare with construction-14 (~10 min)

The structural diff is where the class template shows its cost.

| Metric | construction-14 | construction-15 | Difference |
|---|---|---|---|
| Files | 12 | 12 | 0 |
| Headers | 6 | 7 | +1 |
| Translation units | 6 | 5 | -1 |
| Source lines | 587 | 598 | +11 |
| `board.cpp` | 65 | absent | -65 |
| `board.h` | 21 | 76 | +55 |
| `game.cpp` | 281 | 10 | -271 |
| `game_engine.h` | absent | 292 | +292 |
| Complete classes | 4 | 5 | +1 |
| Function definitions | 39 | 41 | +2 |
| Private data in headers | 9 | 10 | +1 |
| Public declarations | 23 | 27 | +4 |
| `game.o` text | 3,913 | 4,392 | +479 |
| Binary text / data | 9,496 / 692 | 9,410 / 692 | -86 / 0 |
| Indirect calls | 0 | 0 | 0 |
| Traces | 20 of 20 | 20 of 20 (8×8) | 0 |

The shape is the story: `board.cpp` disappears because a class template must keep its definitions
visible; `game.cpp` becomes a ten-line instantiation; `game_engine.h` absorbs the turn loop. The
source grows by 11 lines net, but the distribution changes by hundreds.

### 6.1 What moved where

| Code | construction-14 | construction-15 |
|---|---|---|
| Board declaration | `board.h` | `board.h` |
| Board definitions | `board.cpp` | `board.h`, inline |
| Active dimensions | `activeRowCount`, `activeColumnCount` | `RowCount`, `ColumnCount` |
| Turn loop | `game.cpp`, free functions | `game_engine.h`, member functions |
| Engine state | locals in `run_game` and helpers | member data: board, two knights |
| Entry point | `run_game` | `run_game` calls `GameEngine<8, 8>::run` |
| Piece contract | implicit in the generator | `PositionedPiece` concept |

### 6.2 What the class template buys

- One board for any dimensions in 1..16.
- One engine for any dimensions in 1..16.
- A compile-time dimension check, not a runtime one.
- A named piece contract with a clear diagnostic.
- No duplication of the turn loop for 10×10.

### 6.3 What it costs

- One class per argument list in the object file, or one symbol set at `-O0`.
- The body must be visible in a header; `board.cpp` disappears and `game_engine.h` is 292 lines.
- The maximum storage is fixed at 16×16; a larger board needs a new maximum and a rebuild.
- The piece type is still concrete; the concept is checked once, not used as a constraint.
- The dimensions are compile-time; you cannot choose 8×8 or 10×10 at runtime without instantiating
  both and selecting one.

### 6.4 The gap: generic over dimensions, concrete over pieces

`grep -n 'Knight' code/game_engine.h` returns 38 lines. Six member signatures name `Knight`; the two
data members are `Knight`; the constructor builds two `Knight` objects. `game.cpp` has zero `Knight`
type mentions; its only reference is the lowercase `#include "knight.h"`, which is redundant because
`game_engine.h` already includes it. The class template did not remove the piece type; it moved it.

The full rung would parameterize the piece too:

```cpp
template <int RowCount, int ColumnCount, PositionedPiece PieceType>
class GameEngine
{
    ...
    PieceType playerKnight;
    PieceType machineKnight;
};
```

Then `GameEngine<8, 8, Knight>` and `GameEngine<8, 8, ScoutingPawn>` would be two engines from one
source. That change would also require the six signatures to use `PieceType`, a way to construct
the pieces, and the concept as a constraint on the template parameter rather than a `static_assert`
on `Knight`. This baseline does not do that. Say so before claiming the engine is fully generic.

### 6.5 What the compiler emits, again

`nm -C game15.o` at `-O2` shows one `GameEngine<8, 8>` symbol, `playTurn`. At `-O0` it shows 24. In
the probe with both sizes, `nm` shows 24 for each at `-O0` and one for each at `-O2`.

`objdump -d game15.o | grep -c 'call.*\*'` prints 0, as it did for `construction-14`. The class
template adds no indirect call and no table. The call is resolved at compile time, like any other
member function.

The honest claim is structural: the concrete arguments are fixed when the program is compiled, and
the same source produces a separate class for each argument list. Whether the members are inlined,
and whether the board stays in memory, depends on the optimizer. Measure before claiming a speedup;
this version does the same work per turn and changes no rule.

## Step 7 — Explain (~8 min)

The comparison in one table:

| Question | construction-14 | construction-15 |
|---|---|---|
| What is parameterized | piece type in one function | rows and columns in two classes |
| What is generic | the generator | the board and the engine |
| What is constrained | implicit: `row()`, `column()`, `const` | named: `PositionedPiece`; dimensions 1..16 |
| What the compiler emits | one function per type | one class per argument list |
| Runtime dispatch | none | none |
| The loop's piece type | `Knight` | `Knight` |
| Storage | fixed 8×8 | fixed 16×16, active area varies |
| Traces | 20 of 20 | 20 of 20 for 8×8 |

Write `EVIDENCE.md` §8. Two parts.

**First, one sentence.** Say what the two engines share and what changes, without using the words
*template*, *generic*, *type parameter*, *instantiation*, *compiler*, *better*, *worse*, *clean*,
*flexible*, *reusable* or *elegant*. If your sentence needs one of those, you have described the
mechanism rather than the decision.

**Second, one paragraph.** Answer:

- What the class template fixes: the board and the engine are written once; dimensions are
  arguments.
- What enforces the dimensions: the two `static_assert`s at `board.h:72–73`.
- What enforces the piece contract: `PositionedPiece` at `game_engine.h:14–20` and the
  `static_assert` at line 22.
- What changes with dimensions: bounds, cell count, initial positions, legality, transcript.
- What does not change with dimensions: storage (256 bytes), `sizeof(GameEngine)` (336 bytes), the
  rules, the move geometry, the piece type.
- What the 20 traces prove: the 8×8 behavior is unchanged.
- What the traces do not prove: that 10×10 is correct against a golden trace; there is none.
- What the 10×10 run proves: the same source compiles and runs at a second size, with different
  output.
- What the concept does not cover: semantics; a type can satisfy `PositionedPiece` and still move
  wrongly.
- What the engine does not cover: the piece type is still `Knight`; the concept is a check, not a
  parameter constraint.
- What the change test costs: one new file, zero edits, no new concepts.
- Which later lesson owns lifetime: `construction-16`.
- Where a change like that is defended with numbers: the capstone, `construction-24`.

A strong answer keeps four things apart: **what is generic** (dimensions), **what is constrained**
(piece operations and dimension range), **what the compiler emits** (one class per argument list),
and **what a run can see** (the transcript). A weak answer says "the class template makes the code
reusable", which collapses all four into a slogan.

## Step 8 — Acceptance check and record (~2 min)

The acceptance criterion for this lesson is: **instantiate the same engine template for 8×8 and
10×10; explain what is generic, what is constrained, and what changes with dimensions.**

Check your record against it:

- [ ] I located `Board<RowCount, ColumnCount>` and `GameEngine<RowCount, ColumnCount>` by line.
- [ ] I named the two non-type parameters and the concept's four operations.
- [ ] I found `static_assert(PositionedPiece<Knight>)` and the dimension `static_assert`s.
- [ ] I confirmed that `GameEngine` does not take a piece type parameter.
- [ ] I reproduced all 20 traces on the unmodified 8×8 baseline.
- [ ] I built `GameEngine<10, 10>` from the same header and ran it.
- [ ] I saw that the 10×10 output differs from the 8×8 output.
- [ ] I measured the same `sizeof(Board)` and `sizeof(GameEngine)` for both sizes.
- [ ] I saw the two engine instantiations by name at `-O0`.
- [ ] I reproduced the dimension `static_assert` failure for 17×8.
- [ ] I reproduced the concept failure for a type without the operations.
- [ ] I named what the concept does not check.
- [ ] I named the limit: the piece type is still `Knight`.
- [ ] I did not edit any tracked code file.
- [ ] I completed `EVIDENCE.md` §1–§10 and the self-check in §12.

The criterion is not "the code uses templates". It is "you can say what varies, what is checked,
what the compiler produced, and what the second run changed".

## Self-check (binary)

- [ ] I read `board.h`, `game_engine.h` and `game.cpp` and located both templates by line.
- [ ] I compared the generic function with the class templates and named what each one owns.
- [ ] I predicted the dimension table, the change test and the instantiation counts before running.
- [ ] I reproduced all 20 traces on the unmodified 8×8 baseline.
- [ ] I built and ran `GameEngine<10, 10>` from the same header.
- [ ] I confirmed that the 10×10 output differs from the 8×8 output.
- [ ] I measured that both board types are 256 bytes and both engines are 336 bytes.
- [ ] I saw the two instantiations by name at `-O0`.
- [ ] I reproduced the dimension and concept failures.
- [ ] I named what the concept does not check.
- [ ] I named the limit: the engine is generic over dimensions, not pieces.
- [ ] I completed `EVIDENCE.md`.

These are acceptance criteria. If a box is
unchecked, write the next smallest action in `EVIDENCE.md` §10.

## Misconceptions this lesson refutes

| Misconception | Why it fails | What to do instead |
|---|---|---|
| "A class template is a class." | It is a pattern; the compiler writes a class per argument list. | Count the instantiations with `nm` at `-O0`. |
| "`GameEngine<8,8>` and `GameEngine<10,10>` share a runtime object." | They are two distinct types, generated at compile time. | Look for two symbol sets; there is no shared object. |
| "The 10×10 board uses more storage." | Storage is fixed at 16×16; only the active area changes. | Measure `sizeof` for both. |
| "The dimensions can be chosen at runtime." | They are non-type template parameters, fixed at compile time. | Try to read them from `argv`; you need a branch or two instantiations. |
| "A concept is an interface." | It is a compile-time predicate; no vtable, no object. | Look for indirect calls; there are none. |
| "A concept checks semantics." | It checks operations and return types, not what they mean. | A type can satisfy `PositionedPiece` and move wrongly. |
| "The engine is generic over pieces." | It still declares `Knight` members and six `Knight` signatures. | `grep -n 'Knight' game_engine.h`. |
| "`static_assert` is a runtime check." | It is evaluated at compile time. | Remove it and the build fails; there is no runtime branch. |
| "The 20 traces prove 10×10 works." | They only cover 8×8. There are no 10×10 golden traces. | Compare 8×8 only; call the 10×10 run a smoke test. |
| "The class template makes the binary smaller." | The linked text shrinks by 86 bytes, but `game.o` grows by 479. | Measure both units and say which one you mean. |
| "The body can live in a `.cpp`." | The compiler needs it at instantiation; the link fails otherwise. | Move it and reproduce the undefined reference. |
| "`Board<8,8>` and `Board<10,10>` have the same layout, so they are the same type." | They have different types; the layout happens to be the same size here. | Measure `sizeof`; do not confuse size with type identity. |
| "A concept removes the need for tests." | It checks shape, not behavior. | Keep the traces and add a semantic test. |
| "The engine is fully generic because it has a concept." | The concept is checked once, not used as a parameter constraint. | Read the template parameter list at `game_engine.h:34`. |

## Next step

`construction-16` owns lifetime. The class templates here hold value members: a board and two
knights. They do not allocate, so they do not answer who destroys what. `construction-16` asks that
question.

The full piece parameterization is the next change to this design: add a `PieceType` parameter
constrained by `PositionedPiece`, make the six signatures use it, and decide how the pieces are
constructed. `construction-24` is where you defend that change with numbers. This lesson stops at
dimensions, and says so.

See the [lesson catalog](../../../docs/guide/lesson-catalog.md) for the full path.

If either baseline was missing, do not start `construction-15`. The next contrast needs both sides.
