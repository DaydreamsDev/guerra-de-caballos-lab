# Generic functions (funciones genéricas)

> [English](lesson.md) | [Español](lesson-es.md)

**Concept:** type parameters · **Time:** about 68 minutes · **Status:** drafted
**Reading:** ~23 minutes

## Why this lesson exists

`construction-13` ended with a loop that named `Knight` in ten helper signatures. One of those
helpers, `tryMove`, also owned the movement geometry: two constant arrays and two lines that added
an offset to the active knight's square. The loop could move one piece type, and the rule for
producing destinations lived inside the turn logic.

This baseline moves the geometry into a function template. `move_generator.h` holds the offsets and
one generator:

```cpp
template <typename PieceType>
std::array<MoveCandidate, moveCandidateCount> generate_move_candidates(const PieceType &piece)
{
    std::array<MoveCandidate, moveCandidateCount> candidates = {};
    for (int index = 0; index < moveCandidateCount; index += 1)
    {
        candidates[index] = {
            index + 1,
            piece.row() + knightRowChangePerMove[index],
            piece.column() + knightColumnChangePerMove[index],
        };
    }
    return candidates;
}
```

`game.cpp` calls it once per turn with a `Knight`. `tryMove` no longer receives a move number or the
active knight; it receives one candidate that already carries a destination.

The sentence to keep:

> A function template is a pattern for a function: the compiler writes one function for each type
> that uses it.

Three honest notes before the comparison.

1. The source grows by 31 lines, from 556 across eleven files to 587 across twelve. `game.cpp` grows
   by one line, from 280 to 281, and the new header is 30 lines. The turn logic loses the offset
   arithmetic but gains a candidate lookup, so the file does not shrink.
2. The template does more arithmetic per turn, not less. `construction-13` computed one destination
   pair per attempted move; this version computes eight candidates at the start of every turn.
   Seed 1 has 22 turns and 36 attempts: 176 candidates against 36 pairs. Across the twenty
   traces it is 4,168 candidates against 1,017 pairs. The gain is separation and reuse, not speed.
3. All 20 traces still pass. The transcript records the game, not how a destination was produced.

The comparison with `construction-13` is not "templates are better". It is more precise: the
template buys one generator for any type with `row()` and `column()`, and it pays with one
instantiation per type, a body that must stay visible in a header, and error messages that arrive
only when a type is used. The loop still names `Knight` everywhere else. This lesson measures both
sides.

## What you will be able to do

By the end of this lesson you can:

- explain type parameters in a function;
- use one move generator for two piece types.

## The concept: a function with a type parameter

`construction-13` asked: can a piece work without a base class? The answer was yes, if the position
becomes a member. The loop then named `Knight` in every helper signature, and the movement geometry
stayed in `tryMove`.

This lesson asks a different question: can the geometry be written once for more than one piece
type, without a base class and without copying the loop?

Yes, if the concrete type becomes a parameter of the function. `generate_move_candidates` does not
name `Knight`. It names `PieceType`, a placeholder that the compiler replaces with the type at each
call:

```cpp
template <typename PieceType>
std::array<MoveCandidate, moveCandidateCount> generate_move_candidates(const PieceType &piece);
```

A function template is not a function. It is a pattern. The compiler produces a function when it
sees a call with a concrete type. That act is called **instantiation**. One call with `Knight`
produces `generate_move_candidates<Knight>`; one call with `ScoutingPawn` produces
`generate_move_candidates<ScoutingPawn>`. Both come from the same 14 lines.

| | construction-13 | construction-14 |
|---|---|---|
| Where the offsets live | `game.cpp:18–19` | `move_generator.h:7–8` |
| Who computes a destination | `tryMove`, once per attempt | `generate_move_candidates`, once per turn |
| What `tryMove` receives | active knight and move number | one `MoveCandidate` |
| Types the geometry accepts | `Knight` only | any type with `row()` and `column()` |
| When the concrete type is fixed | when the loop is compiled | at each instantiation |
| Runtime dispatch | none | none |
| Copies of the geometry | one, in `game.cpp` | one per instantiated type |
| The loop's type | `Knight` | `Knight` |

### The contract is structural, not declared

The template does not include `knight.h`. It does not inherit from an interface. It only uses two
expressions:

```cpp
piece.row() + knightRowChangePerMove[index]
piece.column() + knightColumnChangePerMove[index]
```

Any type whose `row()` and `column()` can be called on a `const` object and whose results can be
added to an `int` satisfies the body. C++ accepts the type by looking at the operations it uses.
This is sometimes called **duck typing**, and it is the reason the same generator serves a knight
and a test type with no shared base.

The contract has two parts that the source does not spell out as a declaration:

- the operations must exist: `row()` and `column()`;
- they must be callable through `const PieceType &`, so both accessors must be `const` member
  functions (or free functions that accept a const reference).

If either part is missing, the error appears at the instantiation, not at the template definition.
That is a cost, and `construction-15` addresses it with concepts.

### The generator produces candidates, not legal moves

`generate_move_candidates` adds eight fixed offsets to the piece's square. It does not know the
board, the other knight or the visited cells. A piece at `(1, 1)` produces destinations at row `0`
and column `-1`; those are candidates, not moves. `tryMove` still decides:

```cpp
if (destinationRow < 1 || destinationRow > board.row_count() ||
    destinationColumn < 1 || destinationColumn > board.column_count())
{
    return MoveOutcome::outsideBoard;
}
```

The separation is the point. The generator owns the geometry; the loop owns the rules. Neither
knows more than it needs.

### `MoveCandidate` is the new record

The generator returns a fixed-size array of records:

```cpp
struct MoveCandidate
{
    int moveNumber;
    int destinationRow;
    int destinationColumn;
};
```

The record is 12 bytes on this platform, and the array is 96 bytes. `playTurn` builds it once and
indexes it by move number:

```cpp
auto moveCandidates = generate_move_candidates(*activeKnight);
...
const MoveCandidate &candidate = moveCandidates[attemptedMoveNumber - 1];
```

The previous version passed `attemptedMoveNumber` into `tryMove` and indexed the offset arrays
there. The new version moves the index arithmetic into the generator and passes the finished
candidate.

## Before you start

- [`construction-13`](../construction-13-composition/lesson.md) — the baseline this lesson starts
  from. Its `game.cpp` is the left side of the diff in Step 5.
- [`construction-12`](../construction-12-polymorphism/lesson.md) — the catalog prerequisite and the
  other earlier design. If you did `construction-12` but skipped `construction-13`, read the
  composition lesson's "What composition costs" section before the diff; the change test assumes the
  composition baseline.
- [`construction-11`](../construction-11-encapsulation/lesson.md) — the class boundary. `Position`
  and `Knight` keep private state and public operations.
- [`construction-04`](../construction-04-records/lesson.md) — records. `MoveCandidate` is a record
  with three public fields.
- [`construction-03`](../construction-03-functions/lesson.md) — declarations, parameters and return
  types. A template declaration is read the same way, with one extra parameter.
- A C++20 compiler (`g++` or `clang++`). `cmake` is optional; `run.sh` falls back to a direct
  compile when CMake is absent.

You do not need to know how a compiler instantiates a template. You need to read one template, call
it with two types, and compare the generated program with the previous baseline.

### What you need

- The generic-functions baseline:
  [`code/main.cpp`](code/main.cpp), [`code/game.h`](code/game.h),
  [`code/game.cpp`](code/game.cpp), [`code/move_generator.h`](code/move_generator.h),
  [`code/knight.h`](code/knight.h), [`code/knight.cpp`](code/knight.cpp),
  [`code/position.h`](code/position.h), [`code/position.cpp`](code/position.cpp),
  [`code/board.h`](code/board.h), [`code/board.cpp`](code/board.cpp),
  [`code/random_sequence.h`](code/random_sequence.h),
  [`code/random_sequence.cpp`](code/random_sequence.cpp).
- The previous baseline, for the diff:
  [`code/game.cpp`](../construction-13-composition/code/game.cpp) and its ten companions in
  [`../construction-13-composition/code/`](../construction-13-composition/code/).
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
test -f code/move_generator.h &&
test -f code/knight.h &&
test -f code/knight.cpp &&
test -f code/position.h &&
test -f code/position.cpp &&
test -f code/board.h &&
test -f code/board.cpp &&
test -f code/random_sequence.h &&
test -f code/random_sequence.cpp &&
test -f ../construction-13-composition/code/game.cpp &&
test -f ../../../common/traces/seed-0001.txt
```

If a file is missing, the comparison has no second side. Record the missing file in `EVIDENCE.md`
§11 and return when it is available. A count against a file you have not read is not evidence.

The baseline reproduces all 20 traces in `common/traces/` byte for byte, before you change
anything. That is the behavioral contract, and it says nothing about how a destination is computed.

## Step 1 — Inventory the generator (~10 min)

Read the new header first. The shape is the lesson:

```bash
grep -n '^' code/move_generator.h
```

```text
1:#pragma once
2:
3:#include <array>
4:
5:constexpr int moveCandidateCount = 8;
6:
7:constexpr int knightRowChangePerMove[moveCandidateCount] = {-1, -2, -2, -1, 1, 2, 2, 1};
8:constexpr int knightColumnChangePerMove[moveCandidateCount] = {2, 1, -1, -2, -2, -1, 1, 2};
9:
10:struct MoveCandidate
11:{
12:    int moveNumber;
13:    int destinationRow;
14:    int destinationColumn;
15:};
16:
17:template <typename PieceType>
18:std::array<MoveCandidate, moveCandidateCount> generate_move_candidates(const PieceType &piece)
19:{
20:    std::array<MoveCandidate, moveCandidateCount> candidates = {};
21:    for (int index = 0; index < moveCandidateCount; index += 1)
22:    {
23:        candidates[index] = {
24:            index + 1,
25:            piece.row() + knightRowChangePerMove[index],
26:            piece.column() + knightColumnChangePerMove[index],
27:        };
28:    }
29:    return candidates;
30:}
```

Now the line map. Fill it into `EVIDENCE.md` §2.2 from the source, not from this table:

| Lines | Contents |
|---|---|
| 5 | `moveCandidateCount`, the number of candidates and the array bound. |
| 7–8 | The eight row and column offsets, in move order. |
| 10–15 | `MoveCandidate`, a record with three public fields. |
| 17–18 | The template header and the function signature. |
| 20 | The array of candidates, value-initialized. |
| 21–28 | The loop that fills all eight entries. |
| 23–27 | One candidate: move number, destination row, destination column. |
| 25–26 | The only two operations the template requires of `PieceType`. |
| 29 | The array is returned by value. |

Counts for this baseline: 6 headers, 6 translation units, 22 public declarations, 9 private data
declarations, 4 complete class definitions, 0 incomplete types, 39 function definitions, 17 internal
helpers in `game.cpp`, 1 internal helper in `main.cpp`, 1 entry point.

Compare with `construction-13`: 5 headers, 6 translation units, 19 public declarations, 9 private
data declarations, 3 complete classes, 38 definitions. The new header adds three public data
declarations (`moveNumber`, `destinationRow`, `destinationColumn`), one complete class definition
(`MoveCandidate`) and one function definition (the template). It adds no translation unit, because
the template is header-only.

Then answer in `EVIDENCE.md` §2:

1. Which file holds the template, and on which lines?
2. How many type parameters does it declare, and what is the parameter called?
3. Which operations does the body require of `PieceType`, and on which lines?
4. Does `move_generator.h` include `knight.h` or mention `Knight` anywhere?
5. How many times does `game.cpp` call `generate_move_candidates`?
6. What does `tryMove` receive now, and what did it receive in `construction-13`?
7. Where are the offset arrays, and how many entries does each have?
8. How many public data members does `MoveCandidate` have? Are they private?
9. How many instantiations does the game create? Which type is used?
10. What would happen if `Knight::row()` were not `const`?

Question 4 has a precise answer: no. `grep -n 'Knight' code/move_generator.h` prints
nothing. That is the structural claim of this baseline: the generator does not know the piece type.

Question 5 has a precise answer: once, at `game.cpp:184`. The eight reads of `row()` and `column()`
happen inside the instantiated function, not in the loop source.

Question 9 has a precise answer: one, for `Knight`. `game.cpp` calls the generator once and only
with that type, so the game contains one instantiation.

Question 10 has a precise answer: the program stops compiling at the instantiation with an error
such as `passing 'const Knight' as 'this' argument discards qualifiers`. The template takes
`const PieceType &`, so a non-const accessor cannot be called through it.

## Step 2 — Predict before you run (~10 min)

### 2.1 The candidate table

Write these predictions before running anything. The knight is at `(4, 5)`. The test type,
`ScoutingPawn`, is at `(1, 1)` on purpose: its candidates include squares outside the board, which
shows what the generator does and does not decide.

| Move | `Knight` at (4, 5) | `ScoutingPawn` at (1, 1) | Confidence |
|---|---|---|---|
| 1 | | | |
| 2 | | | |
| 3 | | | |
| 4 | | | |
| 5 | | | |
| 6 | | | |
| 7 | | | |
| 8 | | | |

The last four rows are the ones to take seriously. A pawn at the corner produces negative
coordinates. Predict whether the generator rejects them, and where the rejection happens if it does
not.

### 2.2 The change test

The change is the one that produced this baseline: start from `construction-13` and move the
geometry into a template.

| Question | My prediction |
|---|---|
| Files I will add | |
| Files I will edit | |
| Lines in the new header | |
| Lines I will add in `game.cpp` | |
| Lines I will remove in `game.cpp` | |
| Hunks in the `game.cpp` diff | |
| Functions in `game.cpp` whose signature changes | |
| Logic lines I will change (not names) | |
| Seeds that match, of 20, after the change | |
| New concepts the change requires | |

The eighth row is the acceptance. Predict zero before you measure it. A refactor that changes a rule
is a different lesson.

### 2.3 Instantiation and code generation

| Question | My prediction |
|---|---|
| Instantiations of the generator in the game | |
| `nm -C game14.o` symbols with `generate_move_candidates`, at `-O2` | |
| The same count at `-O0` | |
| `game.o` text: grows, shrinks or stays the same? | |
| Indirect calls in `game14.o` | |
| `sizeof(MoveCandidate)` | |
| `sizeof(std::array<MoveCandidate, 8>)` | |
| Candidates computed on seed 1, against destination pairs in `construction-13` | |

The second and third rows are the mechanism test. A template that is inlined has no symbol; a
template that is not inlined has one symbol per type. Predict both counts before you compile
anything.

## Step 3 — Run the baseline and the instantiation probe (~10 min)

First the traces, unmodified:

```bash
mkdir -p /tmp/generic-test
./run.sh --seed 1 --name Learner > /tmp/generic-test/baseline-0001.txt
diff -u ../../../common/traces/seed-0001.txt /tmp/generic-test/baseline-0001.txt
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
  -o build/generic-baseline code/main.cpp code/game.cpp code/board.cpp \
  code/knight.cpp code/position.cpp code/random_sequence.cpp
```

Now the instantiation probe. It goes in the scratch directory, never in the tracked files:

```bash
mkdir -p /tmp/generic-test/probe
cat > /tmp/generic-test/probe/instantiation.cpp <<'EOF'
#include "knight.h"
#include "move_generator.h"

#include <cstdio>

struct ScoutingPawn
{
    int rowValue;
    int columnValue;

    int row() const { return rowValue; }
    int column() const { return columnValue; }
};

int main()
{
    Knight knight("player");
    knight.set_position(4, 5);
    ScoutingPawn pawn{1, 1};

    auto knightCandidates = generate_move_candidates(knight);
    auto pawnCandidates = generate_move_candidates(pawn);

    for (int index = 0; index < moveCandidateCount; index += 1)
    {
        std::printf("%d knight=%d,%d pawn=%d,%d\n", knightCandidates[index].moveNumber,
                    knightCandidates[index].destinationRow,
                    knightCandidates[index].destinationColumn,
                    pawnCandidates[index].destinationRow,
                    pawnCandidates[index].destinationColumn);
    }

    std::printf("sizeof(MoveCandidate)=%zu sizeof(array)=%zu\n",
                sizeof(MoveCandidate), sizeof(knightCandidates));
    return 0;
}
EOF
g++ -std=c++20 -Wall -Wextra -Werror -O2 -Icode \
  -o /tmp/generic-test/probe/instantiation \
  /tmp/generic-test/probe/instantiation.cpp code/knight.cpp code/position.cpp
/tmp/generic-test/probe/instantiation
```

The output on this platform:

```text
1 knight=3,7 pawn=0,3
2 knight=2,6 pawn=-1,2
3 knight=2,4 pawn=-1,0
4 knight=3,3 pawn=0,-1
5 knight=5,3 pawn=2,-1
6 knight=6,4 pawn=3,0
7 knight=6,6 pawn=3,2
8 knight=5,7 pawn=2,3
sizeof(MoveCandidate)=12 sizeof(array)=96
```

Read the pawn column against the board. The pawn at `(1, 1)` produced `(0, 3)`, `(-1, 2)`,
`(-1, 0)` and `(0, -1)`. The generator did not reject them. It added the offsets and returned. That
is correct: validation belongs to `tryMove`, which knows the board.

Both columns came from the same template. One call was instantiated for `Knight`, one for
`ScoutingPawn`. To see the two instantiations by name, compile the probe without optimization:

```bash
g++ -std=c++20 -Wall -Wextra -Werror -O0 -Icode \
  -c /tmp/generic-test/probe/instantiation.cpp \
  -o /tmp/generic-test/probe/instantiation-O0.o
nm -C /tmp/generic-test/probe/instantiation-O0.o | grep 'generate_move_candidates'
```

```text
0000000000000000 W std::array<MoveCandidate, 8ul> generate_move_candidates<ScoutingPawn>(
    ScoutingPawn const&)
0000000000000000 W std::array<MoveCandidate, 8ul> generate_move_candidates<Knight>(
    Knight const&)
```

Two symbols, both weak, both from the same 14-line body. The `W` means the linker may merge
duplicate copies of each instantiation across translation units. There is no table, no base class
and no runtime type.

If the first diff is not empty, stop. That is a different baseline from the one this lesson
describes; record it in `EVIDENCE.md` §11 and do not adjust the prediction to match.

## Step 4 — Compare: what the template proves (~8 min)

Open `move_generator.h`, `tryMove` and `playTurn`. Five facts, read against the two baselines.

1. **One definition, two instantiations.** At `-O0`, `nm` shows
   `generate_move_candidates<Knight>` and `generate_move_candidates<ScoutingPawn>`. At `-O2`, the
   same probe has no such symbol, because the compiler inlined both instantiations. The source has
   one generator; the object file has one symbol per type, or none after inlining.
2. **The loop no longer computes offsets.** `tryMove` at `game.cpp:147–168` reads
   `candidate.destinationRow` and `candidate.destinationColumn`. The two offset arrays are no longer
   in `game.cpp`; they are at `move_generator.h:7–8`.
3. **`tryMove` changed its input.** It receives `const MoveCandidate &candidate` instead of
   `const Knight *activeKnight` and `int moveNumber`. `playTurn` builds the array once at line 184
   and indexes it at line 192.
4. **All 20 traces are identical.** The generator produces the same eight destinations the old
   formula produced, in the same order. The seed 1 transcript is 89 lines, with 22 turns and 36
   attempts, and it matches `construction-13` byte for byte.
5. **The object code grows, and the data does not.** `game.o` text goes from 3,817 to 3,913 bytes
   (+96), and the linked text from 9,400 to 9,496 (+96). Data stays at 692 bytes, and indirect calls
   stay at 0 on both sides. The +96 bytes are the array and the candidate handling, not a table.

The candidate count is the honest cost. On seed 1, `construction-13` computed 36 destination pairs,
one per attempt. This version computes 22 × 8 = 176 candidates, because it fills the array before
the first attempt. Across the twenty traces: 521 turns × 8 = 4,168 candidates against 1,017 pairs.
The template does more work per turn. It does not make the game faster.

### 4.1 What the compiler emits

Compile `game.cpp` alone on both sides:

```bash
g++ -std=c++20 -O2 -Icode -c code/game.cpp -o /tmp/generic-test/game14.o
g++ -std=c++20 -O2 -I../construction-13-composition/code \
  -c ../construction-13-composition/code/game.cpp -o /tmp/generic-test/game13.o
size /tmp/generic-test/game13.o /tmp/generic-test/game14.o
nm -C /tmp/generic-test/game14.o | grep -c 'generate_move_candidates'
objdump -d /tmp/generic-test/game14.o | grep -c 'call.*\*'
```

With this compiler and `-O2`, the `nm` count is 0 and the indirect-call count is 0. At `-O0`, the
same `game.cpp` emits one weak symbol, `generate_move_candidates<Knight>`. Record both in
`EVIDENCE.md` §5.2.

The absence of a symbol at `-O2` is not a property of templates. It is an optimization decision. A
larger body, a debug build or a different compiler can leave the function out of line. The
structural claim is narrower and true in both cases: the concrete type is fixed at compile time, and
there is no runtime dispatch to resolve.

### 4.2 Three failure modes worth seeing

The template's contract is invisible until a type is used. See the first failure in the scratch
directory:

```bash
cat > /tmp/generic-test/probe/missing_operations.cpp <<'EOF'
#include "move_generator.h"

struct IncompletePawn
{
};

int main()
{
    IncompletePawn pawn;
    auto candidates = generate_move_candidates(pawn);
    return candidates[0].moveNumber;
}
EOF
g++ -std=c++20 -Wall -Wextra -Werror -O2 -Icode -c \
  /tmp/generic-test/probe/missing_operations.cpp \
  -o /tmp/generic-test/probe/missing_operations.o
```

The compiler reports, in `move_generator.h`:

```text
error: 'const struct IncompletePawn' has no member named 'row'
error: 'const struct IncompletePawn' has no member named 'column'
```

It also reports a cascade at line 23, because the failed expressions cannot initialize a
`MoveCandidate`. The first two errors are the useful ones. The third is noise that a constraint
would remove.

The second failure is about `const`. Change `ScoutingPawn::row()` and `column()` to non-const
members and compile again:

```text
error: passing 'const MutablePawn' as 'this' argument discards qualifiers
```

The template takes `const PieceType &`, so both accessors must be `const`. That requirement is part
of the contract, and it is also invisible in the declaration.

The third failure is about visibility. Move the template body into a `.cpp` file, leave only a
declaration in the header, and link:

```bash
mkdir -p /tmp/generic-test/split
cat > /tmp/generic-test/split/split_generator.h <<'EOF'
#pragma once

#include "move_generator.h"

template <typename PieceType>
std::array<MoveCandidate, moveCandidateCount> split_generate(const PieceType &piece);
EOF
cat > /tmp/generic-test/split/split_generator.cpp <<'EOF'
#include "split_generator.h"

template <typename PieceType>
std::array<MoveCandidate, moveCandidateCount> split_generate(const PieceType &piece)
{
    return generate_move_candidates(piece);
}
EOF
cat > /tmp/generic-test/split/main.cpp <<'EOF'
#include "split_generator.h"

struct ScoutingPawn
{
    int row() const { return 4; }
    int column() const { return 5; }
};

int main()
{
    ScoutingPawn pawn;
    return split_generate(pawn)[0].moveNumber;
}
EOF
g++ -std=c++20 -Wall -Wextra -Werror -O2 -Icode -I/tmp/generic-test/split \
  -o /tmp/generic-test/split/split \
  /tmp/generic-test/split/main.cpp /tmp/generic-test/split/split_generator.cpp
```

```text
undefined reference to `std::array<MoveCandidate, 8ul> split_generate<ScoutingPawn>(
    ScoutingPawn const&)'
```

The compiler must see the body at the point of instantiation. That is why `move_generator.h`
contains the loop and not just a declaration. A template is not a separately compiled function.

The limit of the evidence: the probe and the traces show that one generator serves two types. They
do not show that the turn loop accepts a second piece type. It still names `Knight`. Only the
generator is generic.

Record in `EVIDENCE.md` §5 the five facts, the candidate counts, the code-size numbers and the
three failure modes.

## Step 5 — Run the change test (~15 min)

The change test is the refactor that produced this baseline. Start from `construction-13` and move
the geometry into a template. Tracked files stay untouched.

```bash
lessonDirectory="$(pwd)"
rm -rf /tmp/generic-test/change
cp -r ../construction-13-composition/code /tmp/generic-test/change
cd /tmp/generic-test/change
```

### 5.1 Add the generator

```bash
cat > move_generator.h <<'EOF'
#pragma once

#include <array>

constexpr int moveCandidateCount = 8;

constexpr int knightRowChangePerMove[moveCandidateCount] = {-1, -2, -2, -1, 1, 2, 2, 1};
constexpr int knightColumnChangePerMove[moveCandidateCount] = {2, 1, -1, -2, -2, -1, 1, 2};

struct MoveCandidate
{
    int moveNumber;
    int destinationRow;
    int destinationColumn;
};

template <typename PieceType>
std::array<MoveCandidate, moveCandidateCount> generate_move_candidates(const PieceType &piece)
{
    std::array<MoveCandidate, moveCandidateCount> candidates = {};
    for (int index = 0; index < moveCandidateCount; index += 1)
    {
        candidates[index] = {
            index + 1,
            piece.row() + knightRowChangePerMove[index],
            piece.column() + knightColumnChangePerMove[index],
        };
    }
    return candidates;
}
EOF
```

That is 30 lines in one new file. The body must be here, not in a `.cpp`, because every call
instantiates it.

### 5.2 Remove the inline geometry from the loop

Edit `game.cpp` in three places. First, include the new header:

```cpp
#include "board.h"
#include "knight.h"
#include "move_generator.h"
#include "random_sequence.h"
```

Second, delete the two offset arrays from the anonymous namespace. They now live in the header.

Third, change `tryMove` and its caller:

```cpp
MoveOutcome tryMove(const Board &board, const Knight *otherKnight,
                    const MoveCandidate &candidate, int &destinationRow,
                    int &destinationColumn)
{
    destinationRow = candidate.destinationRow;
    destinationColumn = candidate.destinationColumn;
    ...
}

void playTurn(...)
{
    ...
    printTurnHeader(successfulMoveCount + 1, activeKnight, moveOrder);
    auto moveCandidates = generate_move_candidates(*activeKnight);

    bool moveWasAccepted = false;
    for (...)
    {
        int attemptedMoveNumber = moveOrder[attemptIndex];
        const MoveCandidate &candidate = moveCandidates[attemptedMoveNumber - 1];
        ...
        MoveOutcome outcome = tryMove(board, otherKnight, candidate, destinationRow,
                                      destinationColumn);
        ...
    }
}
```

The diff against `construction-13` has 5 hunks, 10 added lines and 8 removed lines. Every changed
line is either the include, the removed arrays, the `tryMove` signature or the candidate lookup. No
`if`, no loop bound, no rule and no output string changed.

### 5.3 Build and run the 20 traces

```bash
g++ -std=c++20 -Wall -Wextra -Werror -O2 -I. \
  -o generic-change main.cpp game.cpp board.cpp knight.cpp position.cpp \
  random_sequence.cpp
cd "${lessonDirectory}"
for trace in ../../../common/traces/seed-*.txt; do
  traceName="$(basename "${trace}" .txt)"
  seedText="${traceName#seed-}"
  seedNumber="$((10#${seedText}))"
  /tmp/generic-test/change/generic-change --seed "${seedNumber}" \
    --name Learner | cmp - "${trace}"
done
```

A silent loop is 20 of 20. The transcript is identical because the generator produces the same
destinations in the same order.

### 5.4 Count what moved

| | construction-13 | construction-14 | Difference |
|---|---|---|---|
| Files | 11 | 12 | +1 |
| Source lines | 556 | 587 | +31 |
| `game.cpp` lines | 280 | 281 | +1 |
| Public declarations | 19 | 22 | +3 |
| Private data declarations | 9 | 9 | 0 |
| Complete classes | 3 | 4 | +1 |
| Function definitions | 38 | 39 | +1 |
| Translation units | 6 | 6 | 0 |
| Instantiated generator functions | 0 | 1 in the game | +1 |
| `sizeof` the piece | 40 | 40 | 0 |
| `sizeof(MoveCandidate)` | — | 12 | new |
| Candidate array per turn | — | 96 bytes | new |
| `game.o` text | 3,817 | 3,913 | +96 |
| Binary text / data | 9,400 / 692 | 9,496 / 692 | +96 / 0 |
| Indirect calls in `game.o` | 0 | 0 | 0 |
| Traces | 20 of 20 | 20 of 20 | 0 |

The change is not a win on every axis. The source grows by 31 lines, `game.o` grows by 96 bytes,
and the turn loop computes eight candidates where it used to compute one pair per attempt. The
generator is written once and accepts any type with the two accessors, and the geometry no longer
lives inside the rule checker.

### 5.5 The gap: only the generator is generic

In `construction-13`, adding a second piece type meant editing ten helper signatures or copying the
loop. This baseline fixes one part of that problem: the candidate generator accepts both types. It
does not fix the rest. `tryMove`, `printMove`, `playTurn` and `playGame` still name `Knight`, and
`run_game` still constructs two `Knight` objects.

To move a `ScoutingPawn` through the turn loop, you would still need one of these:

- **Duplicate the loop** for the second type. That doubles ten functions and the turn logic.
- **Restore an interface.** That is `construction-12` again, with its table and indirection.
- **Parameterize the loop as well.** That is the second half of this idea, and it is
  `construction-15`, where the type parameter reaches the game and a concept states the contract.

Measure the first option honestly in `EVIDENCE.md` §6.5: ten helper signatures plus `run_game`
would need a second copy or a type parameter. The second option is the polymorphism lesson. The
third is the next one.

The acceptance for this lesson is therefore precise: **one move generator for two piece types; the
game loop and all 20 traces unchanged.** It does not claim the loop can move a second piece type.
Only the generator can, and that is the measured scope of this design.

"Unchanged" means unchanged decisions and output, not a byte-identical file. `game.cpp` gains one
line, and the diff is 10 added and 8 removed lines. No rule and no output string changed.

## Step 6 — The template in the rest of the program (~8 min)

### 6.1 The contract is checked at the call, not at the declaration

A template declaration tells you almost nothing about its requirements. `move_generator.h` declares
one type parameter and a const reference; the requirements are buried in the body at lines 25–26.
The compiler checks them when it instantiates the function, and it reports the error in the header,
not at the call site. That is why the first error in §4.2 points at `move_generator.h:25`, with the
call site shown as a note.

`construction-15` replaces this implicit contract with a named concept. The body will still use
`row()` and `column()`, but the declaration will say so, and a bad type will fail at the call with
one clear message.

### 6.2 One instantiation per type, and a 96-byte array per turn

The game instantiates the generator once, for `Knight`. A second type adds a second instantiation.
Each instantiation is a complete function with its own loop. There is no shared runtime object and
no table.

The returned array is a value: eight `MoveCandidate` records, 96 bytes, built on the stack of
`playTurn` each turn. The compiler may keep it in registers or scalarize it at `-O2`, but the source
semantics are a 96-byte temporary. That is the price of returning a complete candidate list instead
of computing one destination on demand.

This is not a reason to avoid the template. It is a reason to measure before claiming that
parameterization is free. Here the array is small and the game is dominated by `printf`; the cost is
invisible in the transcript and visible in `game.o` as +96 bytes.

### 6.3 Why the body lives in the header

A normal function is compiled once in a `.cpp` and declared in a header. A template is different:
the compiler needs the body to generate the function for each type. If the body is hidden in a
`.cpp`, the call compiles and the link fails with an undefined reference, as §4.2 shows.

That has two consequences:

- `move_generator.h` is a source file in practice. Every translation unit that includes it pays to
  parse the body.
- The header must be self-contained. It includes `<array>` and uses `MoveCandidate` and the offset
  arrays; it does not include `knight.h`, because it does not need it.

The offset arrays are `constexpr` at namespace scope, so each translation unit that includes the
header gets its own copy. In this program only `game.cpp` includes it, so there is one copy in the
binary. If several translation units included it, the copies would be duplicated read-only data,
not a linker error. `inline constexpr` would make them one entity; that is a detail for a larger
program, not a rule change here.

One build note: `run.sh` checks the `.cpp` files and `CMakeLists.txt`, not `move_generator.h`. When
CMake is absent, editing only the header can leave a stale binary. Rebuild by hand, or remove
`build/` before running `./run.sh`.

### 6.4 What stays concrete

The generator is the only generic component. Everything else still names `Knight`:

| Function | Knight parameter | What it may do |
|---|---|---|
| `initializePositions` | `Knight *` | Call `set_position`. |
| `printGameHeader` | `const Knight *` | Read `row`, `column`. |
| `printTurnHeader` | `const Knight *` | Read `role_name`, `row`, `column`. |
| `printMove` | `const Knight *` | Read `role_name`, `row`, `column`. |
| `tryMove` | `const Knight *` for the other knight | Read `row`, `column`. |
| `moveKnight` | `Knight *` | Call `set_position`. |
| `playTurn` | `Knight *` and `const Knight *` | Move the active knight; read the other. |

The generator removes the geometry from this table. It does not remove the type. That distinction
is the whole lesson: a function can be independent of the type it operates on while the surrounding
program is not.

### 6.5 What the compiler emits

`nm -C game14.o` at `-O2` shows no `generate_move_candidates` symbol, because the game's one
instantiation is inlined. At `-O0` it shows one weak symbol. In the probe, `nm` shows two weak
symbols, one per type.

`objdump -d game14.o | grep -c 'call.*\*'` prints 0, as it did for `construction-13`. The template
adds no indirect call and no table. The call is resolved at compile time, like any other free
function.

The honest claim is structural: the concrete type is fixed when the program is compiled, and the
same source produces a separate function for each type. Whether that function is inlined, and
whether the array stays in memory, depends on the optimizer. Measure before claiming a speedup; on
seed 1 this version does more arithmetic, not less.

### 6.6 What the template does not hide

`grep -n 'Knight' game.cpp` still returns many lines: the include, ten helper signatures, the two
constructions and the calls. `grep -n 'Knight' move_generator.h` returns nothing. The type is hidden
from the generator and visible everywhere else.

That is the reversal this lesson measures. The geometry became reusable; the loop did not. The next
lesson has to decide whether to parameterize the loop and how to state the contract that the
generator currently leaves implicit.

## Step 7 — Compare and explain (~5 min)

The comparison in one table:

| Question | construction-13 | construction-14 |
|---|---|---|
| Where the offsets live | `game.cpp` | `move_generator.h` |
| Who computes a destination | `tryMove`, per attempt | the generator, per turn |
| What `tryMove` receives | active knight and move number | one candidate |
| Types the geometry accepts | `Knight` only | any type with `row()` and `column()` |
| When the type is fixed | when the loop is compiled | at each instantiation |
| Copies of the geometry | one | one per instantiated type |
| Runtime dispatch | none | none |
| Candidate array per turn | none | 96 bytes |
| `game.o` text | 3,817 bytes | 3,913 bytes |
| Binary text / data | 9,400 / 692 | 9,496 / 692 |
| Source | 556 lines, 11 files | 587 lines, 12 files |
| Traces | 20 of 20 | 20 of 20 |

Write `EVIDENCE.md` §8. Two parts.

**First, one sentence.** Say what the eight destinations do, without using the words *template*,
*generic*, *type parameter*, *instantiation*, *compiler*, *better*, *worse*, *clean*, *flexible*,
*reusable* or *elegant*. If your sentence needs one of those, you have described the mechanism
rather than the decision.

**Second, one paragraph.** Answer:

- What the generator fixes: the eight destinations are produced in one place, from any square that
  can report its row and column.
- What enforces it: the two accessor expressions at `move_generator.h:25–26` and the `const`
  reference in the parameter.
- What changed from `construction-13`: the offsets left `game.cpp`, `tryMove` receives a candidate
  instead of a move number, and `playTurn` builds the array once per turn.
- What the 17 accessor call sites do: five row reads, five column reads, three position writes and
  four name reads, all direct, plus one generator call.
- What the 20 traces prove: the game's behavior is unchanged after the refactor.
- What the traces do not prove: which function was instantiated, whether it was inlined, or how
  large the candidate array is.
- What the change test costs: one file added, 30 lines; `game.cpp` 10 lines added and 8 removed in
  5 hunks; zero rule changes.
- What the template does not cover: the turn loop still names `Knight` in ten signatures, so a
  second piece type needs a parameterized loop or a restored interface.
- What it saves: the geometry is written once and accepts two types in the probe; `tryMove` no
  longer owns the offset tables.
- What it costs: 31 source lines, 96 bytes of object text, one instantiation per type, a
  header-visible body and late, unconstrained error messages.
- Which later lesson builds on it: `construction-15`, generic classes and concepts, extends the
  parameterization to the engine and states the contract.
- Which later lesson owns lifetime: `construction-16`.

A strong answer keeps four things apart: **what the generator accepts**, **what the loop still
names**, **what the compiler emits**, and **what a run can see**. A weak answer says "the template
makes the code reusable", which collapses all four into a slogan.

## Step 8 — Acceptance check and record (~2 min)

The acceptance criterion for this lesson is: **one move generator for two piece types; the game
loop and all 20 traces unchanged.**

Check your record against it:

- [ ] I located the template in `move_generator.h` and named its type parameter.
- [ ] I found that the header does not include `knight.h` and does not mention `Knight`.
- [ ] I counted the 17 accessor call sites and the one generator call in `game.cpp`.
- [ ] I named the two operations the generator requires and their lines.
- [ ] I predicted the candidate table and observed both columns.
- [ ] I reproduced all 20 traces on the unmodified baseline.
- [ ] I built the instantiation probe and saw the same eight destinations for both types.
- [ ] I saw the two weak symbols at `-O0` and the absence of a symbol at `-O2`.
- [ ] I measured `sizeof(MoveCandidate)=12` and the 96-byte array.
- [ ] I reproduced the missing-operation and non-const errors.
- [ ] I reproduced the undefined reference from a split declaration and definition.
- [ ] I added `move_generator.h` and edited `game.cpp` in a scratch copy.
- [ ] I got 10 added and 8 removed lines in `game.cpp` and zero rule changes.
- [ ] I got 20 of 20 traces after the refactor.
- [ ] I measured the 4,168 candidates against 1,017 destination pairs across the traces.
- [ ] I named the limit of this design: the loop still names `Knight`.
- [ ] I did not edit any tracked code file.
- [ ] I completed `EVIDENCE.md` §1–§10 and the self-check in §12.

The criterion is not "the code uses a template". It is "you can say what the generator accepts,
what the loop still names, how many functions the compiler produced, what the array costs, and what
the design gives up".

## Self-check (binary)

- [ ] I read `move_generator.h`, `tryMove` and `playTurn` and located the generator by line.
- [ ] I compared the inline geometry with the template and named what each one owns.
- [ ] I predicted the candidate table, the change-test cost and the instantiation probe before
      running.
- [ ] I reproduced all 20 traces on the unmodified baseline.
- [ ] I built the probe and saw the same eight destinations for `Knight` and `ScoutingPawn`.
- [ ] I confirmed that the generator produces off-board candidates without rejecting them.
- [ ] I saw the two instantiations by name at `-O0`.
- [ ] I reproduced the missing-operation, non-const and split-definition failures.
- [ ] I moved the geometry into `move_generator.h` in a scratch copy.
- [ ] I showed 20 of 20 traces after the refactor.
- [ ] I counted 18 changed lines in `game.cpp` and zero rule changes.
- [ ] I measured the code-size and candidate-count differences.
- [ ] I named the limit of this design: only the generator is generic.
- [ ] I completed `EVIDENCE.md`.

These are acceptance criteria. If a box is
unchecked, write the next smallest action in `EVIDENCE.md` §10.

## Misconceptions this lesson refutes

| Misconception | Why it fails | What to do instead |
|---|---|---|
| "A template is a function." | It is a pattern; the compiler writes a function per type. | Count the instantiations with `nm` at `-O0`. |
| "The template runs at runtime and chooses a type." | The type is fixed at compile time; there is no table. | Look for indirect calls and vtable symbols; there are none. |
| "One template means one function in the binary." | There is one instantiation per type, unless the optimizer removes it. | Compile the probe at `-O0` and read the two symbols. |
| "The generator validates moves." | It only adds offsets; a pawn at `(1, 1)` gets negative destinations. | Read the pawn column and then read `tryMove`. |
| "The loop is now generic too." | `game.cpp` still names `Knight` in ten signatures. | `grep -n 'Knight' game.cpp` and count. |
| "The template makes the program faster." | On seed 1 it computes 176 candidates against 36 pairs. | Count candidates and attempts; measure before claiming. |
| "The template has no cost because it is header-only." | `game.o` grows by 96 bytes, and every inclusion parses the body. | Measure text and data separately. |
| "The body can live in a `.cpp` like any function." | The compiler needs it at instantiation; the link fails otherwise. | Reproduce the undefined reference. |
| "The template states its requirements." | The requirements are only in the body, at lines 25–26. | Try a type without `row()` and read where the error lands. |
| "Any type with `row()` and `column()` works." | The accessors must also be callable through `const`. | Try non-const accessors and read the qualifier error. |
| "`MoveCandidate` is a class with behaviour." | It is a record with three public fields and no operations. | Point at lines 10–15 of the header. |
| "The template removes the need for an interface." | It removes the interface for the generator only; the loop still names one type. | State the next change before claiming generality. |
| "The 20 traces prove the template works." | They prove behavior, not which function ran or how it was generated. | Use `nm`, `size` and the probe. |
| "Generic code is always shorter." | This version adds 31 lines and one file. | Count the header and the candidate handling. |
| "The old inline version was slower." | It computed fewer destinations per turn, not more. | Compare 1,017 pairs with 4,168 candidates. |

## Next step

`construction-15` — generic classes and concepts. This lesson made one function independent of the
piece type. The next lesson extends the parameterization to the engine and states the generator's
contract with a concept, so a bad type fails at the call with one clear message instead of a cascade
inside the header.

`construction-16` owns the lifetime question that value members only postpone. `construction-12`
and `construction-13` remain the other sides of this contrast; keep both baselines.

See the [lesson catalog](../../../docs/guide/lesson-catalog.md) for the full path.

If either baseline was missing, do not start `construction-15`. The next contrast needs both sides.
