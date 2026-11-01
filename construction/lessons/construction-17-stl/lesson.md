# Standard Template Library and algorithms (algoritmos)

> [English](lesson.md) | [Español](lesson-es.md)

**Concept:** iteration abstraction · **Time:** about 68 minutes · **Status:** drafted
**Reading:** ~22 minutes

## Why this lesson exists

`construction-16` ended with an engine that owns its resources explicitly and still writes every
traversal by hand:

```cpp
for (int row = 1; row <= RowCount; row += 1)
{
    for (int column = 1; column <= ColumnCount; column += 1)
    {
        visited[row][column] = false;
    }
}
```

This lesson changes three of those traversals and leaves the rest. The board reset becomes
`std::ranges::fill`. Move generation and move ordering become `std::ranges::transform` over
`std::views::iota`. The attempt loop stays, and the reason it stays is the counterexample the
lesson is built around.

The sentence to keep:

> A loop states the steps; an algorithm states the traversal. Replace the loop when the contract is
> "do this to every element", and keep the loop when the contract is "print, decide and mutate, one
> element at a time, until one is accepted".

Three honest notes before the comparison.

1. The source grows by 5 lines, from 606 across twelve files to 611 across twelve. Nine of the
   twelve files are byte-identical to `construction-16`. `board.h` shrinks by 1 line (76 to 75),
   `move_generator.h` grows by 4 (30 to 34), and `game_engine.h` grows by 2 (300 to 302). The diff
   is 26 added lines and 21 removed: five include lines, three algorithm call sites, the
   `std::array` storage, and the loop bodies they replace.
2. Four `for` headers disappear, from nine to five. The three replaced traversals become three
   algorithm calls. The object code barely moves: `game.o` grows by 18 text bytes and the linked
   binary by 16. `sizeof(Board<8, 8>)` stays 256 and `sizeof(GameEngine<8, 8>)` stays 24.
3. The STL does not make this program faster in any way this lesson measures. On this machine, the
   preprocessed input for `game.cpp` grows by 17,827 lines, and the compile time grows from about
   0.50 s to about 0.61 s. What the change buys is the contract at the call site: the name of the
   algorithm says what happens to every element, and the loop body stops being the place where a
   reader has to infer it. Whether that trade is worth it is the learner's call, and the record
   should say which side the learner weighs.

The comparison is not "algorithms are better than loops". It is narrower: three traversals now name
their contract, one traversal cannot name it without hiding effects, and the transcript is identical
before and after. This lesson measures the source, the object code, the compile cost and the one
semantic difference the board reset introduces.

## What you will be able to do

By the end of this lesson you can:

- explain iteration abstraction at the call site;
- replace three traversals with algorithms and keep the transcript unchanged.

## The concept: what a traversal promises

### Three shapes, three names

Every traversal in this program has one of three shapes:

1. **Set every element to the same value.** The loop exists to touch all of them, and the result is
   the same collection in a known state.
2. **Produce one output per input.** The loop maps an index to a value, and the count in equals the
   count out.
3. **Stop at the first element that satisfies a condition.** The loop exists to avoid visiting the
   rest.

The standard library has a name for each shape:

| Shape | Hand-written form | Algorithm | What the name promises |
|---|---|---|---|
| Set every element | nested `for` over rows and columns | `std::ranges::fill` | every element becomes the given value |
| Map one per input | `for` with an index | `std::ranges::transform` | same count in, same count out |
| First match | `for` with `return` or `break` | `std::ranges::any_of`, `find_if` | early exit, one answer |
| Print, decide and mutate | `for` with `continue` and a flag | none fits | effects interleaved with the decision |

The table is the lesson. The first three rows are about the shape of the traversal. The fourth row
is about what happens when the traversal is not only a traversal.

### `views::iota` is not a container

`std::views::iota(0, moveCandidateCount)` produces the integers 0 through 7 on demand. It stores two
integers, not eight elements. It allocates nothing, and it does not own a range of values.

That is why `std::ranges::transform` can take it as a source and `candidates.begin()` as a
destination:

```cpp
std::ranges::transform(
    std::views::iota(0, moveCandidateCount), candidates.begin(),
    [&piece](int index)
    {
        return MoveCandidate{
            index + 1,
            piece.row() + knightRowChangePerMove[index],
            piece.column() + knightColumnChangePerMove[index],
        };
    });
```

Read the call as three facts:

- **Source:** the integers 0 to 7, generated lazily.
- **Destination:** the eight slots of `candidates`, in order.
- **Operation:** one lambda that turns an index into a `MoveCandidate`.

The loop body did the same three things. The difference is where the reader finds them. In the loop,
the bound, the step and the element access are spread across the `for` header and the body. In the
algorithm call, the source range, the output range and the operation are three arguments, and the
name `transform` states the relationship between them.

### What the algorithm does not remove

The iteration is still there. It moved into a header. The index arithmetic is still there. It moved
into the lambda. The branch that decides when to stop is still there, inside the library.

What changed is who writes the loop and what the call site has to state. That is the whole
abstraction: the algorithm fixes the control flow so the call site can describe the data flow.

This matters for the cost column. `std::ranges::transform` is a function template. The compiler sees
its body, inlines it, and generates roughly the same instructions a hand-written loop would. The
text size in Step 4 confirms it: 18 bytes of difference in `game.o`, not a new runtime layer.

### The attempt loop is the counterexample

Here is the loop that stays:

```cpp
bool moveWasAccepted = false;
for (int attemptIndex = 0;
     attemptIndex < moveCandidateCount && !moveWasAccepted;
     attemptIndex += 1)
{
    int attemptedMoveNumber = moveOrder[attemptIndex];
    const MoveCandidate &candidate = moveCandidates[attemptedMoveNumber - 1];
    int destinationRow = 0;
    int destinationColumn = 0;
    MoveOutcome outcome = tryMove(otherKnight, candidate, destinationRow,
                                  destinationColumn);

    if (outcome == MoveOutcome::outsideBoard)
    {
        printIllegalAttempt(attemptedMoveNumber, destinationRow,
                            destinationColumn, "outside");
        continue;
    }
    if (outcome == MoveOutcome::visited)
    {
        printIllegalAttempt(attemptedMoveNumber, destinationRow,
                            destinationColumn, "visited");
        continue;
    }
    if (outcome == MoveOutcome::capture)
    {
        printCaptureAttempt(attemptedMoveNumber, destinationRow,
                            destinationColumn, otherKnightName);
        successfulMoveCount += 1;
        printMove(successfulMoveCount, activeKnight, destinationRow,
                  destinationColumn, true);
        board->mark_visited(destinationRow, destinationColumn);
        winnerName = activeKnightName;
        moveWasAccepted = true;
        continue;
    }

    printLegalAttempt(attemptedMoveNumber, destinationRow, destinationColumn);
    successfulMoveCount += 1;
    printMove(successfulMoveCount, activeKnight, destinationRow,
              destinationColumn, false);
    board->mark_visited(destinationRow, destinationColumn);
    moveKnight(activeKnight, destinationRow, destinationColumn);
    playerIsActive = !playerIsActive;
    moveWasAccepted = true;
}
```

Four properties keep this loop a loop:

1. **Every attempt produces output before the decision is complete.** Illegal attempts print a line
   and continue. The transcript is built by the loop, not after it.
2. **The accepted attempt mutates four pieces of state.** `successfulMoveCount`, the board, the
   active knight and `playerIsActive` all change inside the iteration.
3. **There are four outcomes, not two.** `outsideBoard`, `visited`, `capture` and `legal` take
   different paths, and two of them end the turn.
4. **The loop has two ways to stop.** The bound can be reached, or `moveWasAccepted` can become
   true. The second one is the early exit.

An algorithm can be forced to do this. `std::ranges::find_if` with a predicate that prints and
records the outcome would compile. But then the predicate decides and prints and mutates, and its
name promises one decision. `std::ranges::for_each` cannot break, so the early exit would need a
flag or an exception. `std::ranges::any_of` returns a `bool` and loses which outcome occurred.

The precise rule is not "early exit means keep the loop". `Board::is_valid` has an early exit and a
pure predicate, so `std::ranges::any_of` would express it honestly. The rule is: **when the
per-element work has effects that the algorithm's name does not mention, the loop is the honest
form.** The attempt loop prints, mutates and branches; the algorithm would hide all three behind a
predicate.

`Board::is_valid` is a leftover in this baseline. It is a traversal with a pure predicate that could
become `any_of`, and it was not changed. It is also not called by the game, so changing it would not
be observable in the transcript. Step 5 gives the learner a transcript-visible traversal instead.

## Before you start

- [`construction-05`](../construction-05-array-of-records/lesson.md) — collection and traversal.
  The board is a collection of cells, and a traversal is the unit that visits them. This lesson
  changes the form of that unit.
- [`construction-16`](../construction-16-ownership/lesson.md) — the immediate baseline and the left
  side of the diff in Step 6. Its `code/` directory is the artifact this lesson starts from, even
  though the catalog lists `construction-05` as the formal prerequisite.
- [`construction-14`](../construction-14-generic-functions/lesson.md) — optional context. The
  algorithms are function templates, but you do not need to write one to use one.
- A C++20 compiler (`g++` or `clang++`). `cmake` is optional; `run.sh` falls back to a direct
  compile when CMake is absent. The ranges algorithms used here are C++20, not C++23.

You do not need to know how `std::ranges::transform` is implemented. You need to read one loop, say
what it promises, replace it with an algorithm that promises the same thing, and show that the
transcript did not change.

### What you need

- The STL baseline:
  [`code/main.cpp`](code/main.cpp), [`code/game.h`](code/game.h),
  [`code/game.cpp`](code/game.cpp), [`code/game_engine.h`](code/game_engine.h),
  [`code/board.h`](code/board.h), [`code/move_generator.h`](code/move_generator.h),
  [`code/knight.h`](code/knight.h), [`code/knight.cpp`](code/knight.cpp),
  [`code/position.h`](code/position.h), [`code/position.cpp`](code/position.cpp),
  [`code/random_sequence.h`](code/random_sequence.h),
  [`code/random_sequence.cpp`](code/random_sequence.cpp).
- The previous baseline, for the diff:
  [`code/board.h`](../construction-16-ownership/code/board.h),
  [`code/move_generator.h`](../construction-16-ownership/code/move_generator.h),
  [`code/game_engine.h`](../construction-16-ownership/code/game_engine.h), and their nine
  companions in [`../construction-16-ownership/code/`](../construction-16-ownership/code/).
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
test -f ../construction-16-ownership/code/board.h &&
test -f ../construction-16-ownership/code/move_generator.h &&
test -f ../construction-16-ownership/code/game_engine.h &&
test -f ../../../common/traces/seed-0001.txt
```

If a file is missing, the comparison has no second side. Record the missing file in `EVIDENCE.md`
§11 and return when it is available. A count against a file you have not read is not evidence.

The 8×8 baseline reproduces all 20 traces in `common/traces/` byte for byte, before you change
anything. That is the behavioral contract, and this lesson does not touch it.

## Step 1 — Inventory the traversals (~10 min)

Start with the count. This is the number the acceptance is about:

```bash
grep -rn 'for (' code/ | wc -l
```

```text
5
```

The same count against `../construction-16-ownership/code/` is 9. The difference is four
loop headers: two in `Board::reset`, one in `generate_move_candidates`, one in `buildMoveOrder`.
The five that remain:

| Location | Loop |
|---|---|
| `board.h:27` | outer row loop in `is_valid` |
| `board.h:29` | inner column loop in `is_valid` |
| `game_engine.h:139` | move-order print loop in `printTurnHeader` |
| `game_engine.h:235` | attempt loop in `playTurn` |
| `main.cpp:17` | argument loop in `parseArguments` |

Now the algorithm calls:

```bash
grep -rn 'ranges::\|views::' code/
```

```text
code/board.h:44:        std::ranges::fill(visited, CellRow{});
code/move_generator.h:23:    std::ranges::transform(
code/move_generator.h:24:        std::views::iota(0, moveCandidateCount), candidates.begin(),
code/game_engine.h:125:        std::ranges::transform(
code/game_engine.h:126:            std::views::iota(0, moveCandidateCount), moveOrder,
```

Three call sites, two of them `transform` over `iota`, one `fill`.

`board.h`, traversal-relevant ranges:

| Lines | Contents |
|---|---|
| 3–4 | Includes. `<algorithm>` provides `fill`; `<array>` provides `std::array`. |
| 6–7 | The template header and `class Board`. |
| 25–40 | `is_valid()`. Nested `for`, early `return false`. Unchanged. |
| 42–45 | `reset()`. One `std::ranges::fill` call. |
| 47–63 | `was_visited` and `mark_visited`. Unchanged. |
| 65–74 | Private constants, `CellRow`, and the `std::array` storage. |

`move_generator.h`, traversal-relevant ranges:

| Lines | Contents |
|---|---|
| 3–5 | Includes. `<algorithm>` and `<ranges>` are new; `<array>` was already there. |
| 9–10 | The eight row and column changes, as `constexpr` arrays. |
| 12–17 | `MoveCandidate`. |
| 19–34 | `generate_move_candidates`. One `transform` over `iota`. |

`game_engine.h`, traversal-relevant ranges:

| Lines | Contents |
|---|---|
| 123–131 | `buildMoveOrder`. One `transform` over `iota`. |
| 133–148 | `printTurnHeader`. One `for` with a separator. Unchanged. |
| 234–286 | `playTurn`'s attempt loop. Unchanged. |

Counts for this baseline. The counting rule is stated so you can reproduce it:

- Headers: 7 (`board.h`, `game.h`, `game_engine.h`, `knight.h`, `move_generator.h`, `position.h`,
  `random_sequence.h`).
- Translation units: 5 (`main.cpp`, `game.cpp`, `knight.cpp`, `position.cpp`,
  `random_sequence.cpp`).
- Complete class definitions: 5 (`Board`, `GameEngine`, `Knight`, `Position`, `MoveCandidate`).
- Named function bodies written by hand: 41, the same 41 as `construction-16`. The three lambdas are
  new and are counted separately.
- Lambdas: 3, one per algorithm call site.
- `for` headers: 5, down from 9.
- Algorithm call sites: 3 (`fill`, `transform`, `transform`).
- Source lines: 611, which is 606 plus 5.
- Files that differ from `construction-16`: 3 of 12.

Then answer in `EVIDENCE.md` §2:

1. Which three traversals became algorithms, and on which lines?
2. Which four `for` headers disappeared?
3. How many `for` headers remain, and where?
4. How many algorithm call sites are there, and which algorithms?
5. Which header provides `fill`, and which provides `transform` and `iota`?
6. Which file grew the most, and by how many lines?
7. Which files are byte-identical to `construction-16`?
8. What does `Board::reset` now clear that the old loop did not?
9. Which remaining traversal has a pure early exit and could become `any_of`?
10. Which remaining traversal cannot become an algorithm without hiding effects?

Questions 8 and 10 have precise answers, and both are developed later. Question 8: the new
`fill` clears all 16×16 cells, while the old loop cleared only the active 8×8 area. Question 10:
the attempt loop, for the four reasons in the concept section.

## Step 2 — Predict before you run (~10 min)

Write these predictions before running anything. Do not look at the answers in Step 3 first.

### 2.1 The structure table

| Question | My prediction |
|---|---|
| `for` headers in `construction-16` | |
| `for` headers here | |
| Algorithm call sites here | |
| Source lines in `construction-16` | |
| Source lines here | |
| Files that differ | |
| Which file shrinks | |
| Which file grows the most | |
| Lambdas added | |

The row that is easy to get wrong is the third. Three traversals were replaced, but one of them was
a nested loop, so four loop headers became three algorithm calls.

### 2.2 The size and text table

| Question | My prediction |
|---|---|
| `sizeof(Board<8, 8>)` here | |
| `sizeof(GameEngine<8, 8>)` here | |
| `sizeof(std::array<MoveCandidate, 8>)` | |
| `game.o` text bytes, before and after | |
| Binary text bytes, before and after | |
| Binary data bytes, before and after | |
| Heap allocations per engine, before and after | |
| Preprocessed lines for `game.cpp`, before and after | |
| Compile time for `game.cpp`, before and after | |
| Seeds that match, of 20, after the change | |

Predict the allocation row as zero and zero. `views::iota` is a lazy view; it does not allocate.
The engine still allocates three times because of `unique_ptr`, and that is `construction-16`'s
cost, not this lesson's.

Predict the compile-time row last. It is the one that makes the STL's price visible, and it is
machine-specific.

### 2.3 The change test predictions

The change test is: replace one traversal that is on the transcript path with an algorithm, and show
that the transcript is unchanged.

| Question | My prediction |
|---|---|
| The traversal I will replace | |
| Its line range | |
| The algorithm I will use | |
| `for` headers after the change | |
| The transcript changes? | |
| Seeds that match, of 20, after the change | |
| The attempt loop after the change | |

Predict the attempt loop as unchanged. If your change touches it, you have picked the counterexample
by mistake.

## Step 3 — Run the baseline (~10 min)

First the traces, unmodified:

```bash
mkdir -p /tmp/stl-test
./run.sh --seed 1 --name Learner > /tmp/stl-test/baseline-0001.txt
diff -u ../../../common/traces/seed-0001.txt /tmp/stl-test/baseline-0001.txt
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
  -o build/stl-baseline code/main.cpp code/game.cpp code/knight.cpp \
  code/position.cpp code/random_sequence.cpp
```

Now the sizes. The probe goes in the scratch directory, never in the tracked files:

```bash
cat > /tmp/stl-test/sizes.cpp <<'EOF'
#include "game_engine.h"

#include <cstdio>

int main()
{
    std::printf("sizeof(Board<8,8>)=%zu\n", sizeof(Board<8, 8>));
    std::printf("sizeof(GameEngine<8,8>)=%zu\n", sizeof(GameEngine<8, 8>));
    std::printf("sizeof(Knight)=%zu\n", sizeof(Knight));
    std::printf("sizeof(Position)=%zu\n", sizeof(Position));
    std::printf("sizeof(MoveCandidate)=%zu\n", sizeof(MoveCandidate));
    std::printf("sizeof(std::array<MoveCandidate,8>)=%zu\n",
                sizeof(std::array<MoveCandidate, moveCandidateCount>));
    return 0;
}
EOF
g++ -std=c++20 -Wall -Wextra -Werror -O2 -Icode \
  -o /tmp/stl-test/sizes /tmp/stl-test/sizes.cpp \
  code/knight.cpp code/position.cpp
/tmp/stl-test/sizes
```

The output on this platform:

```text
sizeof(Board<8,8>)=256
sizeof(GameEngine<8,8>)=24
sizeof(Knight)=40
sizeof(Position)=8
sizeof(MoveCandidate)=12
sizeof(std::array<MoveCandidate,8>)=96
```

Every number matches `construction-16`. `std::array` did not change the board's size, and the
algorithms did not change the engine's size.

Now the object code:

```bash
g++ -std=c++20 -O2 -Icode -c code/game.cpp -o /tmp/stl-test/game17.o
g++ -std=c++20 -O2 -I../construction-16-ownership/code \
  -c ../construction-16-ownership/code/game.cpp -o /tmp/stl-test/game16.o
size /tmp/stl-test/game16.o /tmp/stl-test/game17.o
```

```text
   text	   data	    bss	    dec	    hex	filename
   4638	      0	      0	   4638	   121e	/tmp/stl-test/game16.o
   4656	      0	      0	   4656	   1230	/tmp/stl-test/game17.o
```

Eighteen bytes. Link both programs and compare the binaries:

```bash
g++ -std=c++20 -O2 -Icode -o /tmp/stl-test/bin17 \
  code/main.cpp code/game.cpp code/knight.cpp code/position.cpp code/random_sequence.cpp
g++ -std=c++20 -O2 -I../construction-16-ownership/code -o /tmp/stl-test/bin16 \
  ../construction-16-ownership/code/main.cpp ../construction-16-ownership/code/game.cpp \
  ../construction-16-ownership/code/knight.cpp ../construction-16-ownership/code/position.cpp \
  ../construction-16-ownership/code/random_sequence.cpp
size /tmp/stl-test/bin16 /tmp/stl-test/bin17
```

```text
   text	   data	    bss	    dec	    hex	filename
   9652	    692	     24	  10368	   2880	/tmp/stl-test/bin16
   9668	    692	     24	  10384	   2890	/tmp/stl-test/bin17
```

Text grows by 16 bytes. Data and BSS do not move. Neither object file contains an indirect call:

```bash
objdump -d -C /tmp/stl-test/game16.o | grep -c 'call.*\*' || true
objdump -d -C /tmp/stl-test/game17.o | grep -c 'call.*\*' || true
```

```text
0
0
```

The algorithms are templates, so the calls are direct and inlined. The STL did not add a dispatch
layer.

The compile cost is real, and it is the number the STL is usually not asked about:

```bash
g++ -std=c++20 -Icode -E code/game.cpp | wc -l
g++ -std=c++20 -I../construction-16-ownership/code \
  -E ../construction-16-ownership/code/game.cpp | wc -l
for run in 1 2 3; do
  /usr/bin/time -f '%e' g++ -std=c++20 -O2 -Icode -c code/game.cpp \
    -o /tmp/stl-test/t17.o
done
for run in 1 2 3; do
  /usr/bin/time -f '%e' g++ -std=c++20 -O2 -I../construction-16-ownership/code \
    -c ../construction-16-ownership/code/game.cpp -o /tmp/stl-test/t16.o
done
```

```text
54432
72259
0.50
0.51
0.49
0.62
0.61
0.60
```

The preprocessed input grows by 17,827 lines, about 33 percent. The compile time grows by about
0.11 s, about 22 percent, on this machine and this compiler. Your numbers will differ; the direction
should not.

If the first diff is not empty, stop. That is a different baseline from the one this lesson
describes; record it in `EVIDENCE.md` §11 and do not adjust the prediction to match.

## Step 4 — Compare: what the algorithms prove (~10 min)

Open `board.h`, `move_generator.h` and `game_engine.h`. Five facts, read against the measurements.

1. **Three traversals now name their contract.** `fill` says every element gets the same value.
   `transform` says one output per input. The call site no longer contains the bound, the step or
   the element access.
2. **The board reset changed more than its form.** The old loop cleared 64 cells, rows 1–8 and
   columns 1–8. The new `fill` clears all 256 cells of the 16×16 storage. The transcript cannot see
   the difference, because cells outside the active area are never marked and `is_valid` only checks
   that they are false. The work per reset is four times larger, once per game.
3. **`std::array` is a value, not a pointer.** `sizeof(Board<8, 8>)` is still 256. The nested
   `std::array<bool, 16>` has the same layout as `bool[16][16]`, and `CellRow{}` is a zeroed row.
   The change buys value semantics and range support, not a smaller board.
4. **`views::iota` allocates nothing.** The engine still makes three allocations, and they come from
   the three `make_unique` calls that `construction-16` introduced. The algorithms add no
`operator new` call.
5. **The object code barely moves.** `game.o` grows by 18 text bytes, the binary by 16. The compiler
   inlined the algorithm bodies and generated the same kind of loop the source no longer writes.

### 4.1 The exact diff

Three files changed. `diff -u` against `construction-16` gives 26 added lines and 21 removed:

```bash
for file in board.h move_generator.h game_engine.h; do
  diff -u "../construction-16-ownership/code/${file}" "code/${file}"
done
```

| File | Added | Removed | Net | What changed |
|---|---|---|---|---|
| `board.h` | 7 | 8 | -1 | Two includes, one `fill`, `std::array` storage |
| `move_generator.h` | 12 | 8 | +4 | Two includes, one `transform` over `iota` |
| `game_engine.h` | 7 | 5 | +2 | One include, one `transform` over `iota` |
| Total | 26 | 21 | +5 | Three call sites, five includes |

The nine other files are byte-identical. The rules, the transcript format, the ownership model and
the turn loop are untouched.

### 4.2 What did not change

- The transcript. All 20 traces match, byte for byte.
- The rules. Move order, capture, trap, draw and the 200-move limit are the same.
- The interfaces. `reset`, `was_visited`, `mark_visited`, `generate_move_candidates` and
  `buildMoveOrder` keep their signatures.
- The ownership model. Three `unique_ptr` members, copy deleted, move defaulted.
- The sizes. Board 256, engine 24, knight 40, candidate 12.
- The allocation count. Three per engine, unchanged.
- Indirect calls in `game.o`. Zero before, zero after.

### 4.3 What it costs

- Five include lines, and the transitive include weight that comes with `<algorithm>` and
  `<ranges>`. On this machine, the preprocessed input for `game.cpp` grows by 17,827 lines.
- About 0.11 s more compile time for `game.cpp` on this machine.
- 18 more text bytes in `game.o`, 16 in the linked binary.
- Four times the writes per board reset, because `fill` clears the whole 16×16 storage.
- One more thing to read: the lambda. The loop body is still there, now as an expression that
  returns a value.

None of these is a defect. They are the price of moving the traversal into a named algorithm, and
the record should list them next to the benefit.

### 4.4 The reset semantics, precisely

`construction-16` stored `bool visited[16][16]` and reset rows 1..`RowCount`, columns
1..`ColumnCount`. `construction-17` stores `std::array<std::array<bool, 16>, 16>` and calls
`std::ranges::fill(visited, CellRow{})`, which assigns a zeroed row to every one of the 16 rows.

Both versions start from `visited{}` in the constructor, so both start all-false. `mark_visited`
refuses coordinates outside the active area, and `was_visited` refuses them too. `is_valid` checks
that the outside area stays false. Therefore the two versions are observationally equivalent, and
the extra 192 writes per reset are invisible.

That is the kind of difference a transcript cannot catch, and it is worth naming: an algorithm can
widen the range it touches even when its name sounds like the loop it replaced.

## Step 5 — Change test: replace one transcript-visible traversal (~12 min)

The acceptance names the change test: **remove a traversal loop, use an algorithm, and show that the
transcript is unchanged.**

The three algorithms in the baseline are already the "after" side. For the change test, take a loop
that is still there and that the transcript can see. `printTurnHeader` is the right one: it prints
the move order on every turn, so a mistake changes every trace.

### 5.1 Make a scratch copy

Never edit the tracked `code/` directory. Copy it:

```bash
rm -rf /tmp/stl-test/change
cp -r code /tmp/stl-test/change
```

### 5.2 Replace the loop

In `/tmp/stl-test/change/game_engine.h`, replace lines 139–146 with:

```cpp
std::ranges::for_each(
    std::views::iota(0, moveCandidateCount),
    [moveOrder](int orderIndex)
    {
        std::printf("%d", moveOrder[orderIndex]);
        if (orderIndex + 1 < moveCandidateCount)
        {
            std::printf(",");
        }
    });
```

The loop header, the bound and the step are gone. The source range, the operation and the separator
rule are the three arguments a reader can see. The lambda still uses an index, because the separator
depends on the position; that arithmetic did not disappear, it moved.

### 5.3 Build and compare

```bash
g++ -std=c++20 -Wall -Wextra -Werror -O2 -I/tmp/stl-test/change \
  -o /tmp/stl-test/change-bin \
  /tmp/stl-test/change/main.cpp /tmp/stl-test/change/game.cpp \
  /tmp/stl-test/change/knight.cpp /tmp/stl-test/change/position.cpp \
  /tmp/stl-test/change/random_sequence.cpp
for trace in ../../../common/traces/seed-*.txt; do
  traceName="$(basename "${trace}" .txt)"
  seedText="${traceName#seed-}"
  seedNumber="$((10#${seedText}))"
  /tmp/stl-test/change-bin --seed "${seedNumber}" --name Learner | cmp - "${trace}"
done
grep -rn 'for (' /tmp/stl-test/change | wc -l
```

The loop is silent, and the count prints `4`. The transcript is unchanged on all 20 seeds, and one
more `for` header is gone.

### 5.4 What the change test proves, and what it does not

It proves that a traversal on the output path can change form without changing output. It does not
prove that the algorithm is faster: this lesson measured text size and compile time, not run time.
It does not prove that the result is clearer to every reader: the lambda is longer than the loop
body it replaced, and a reader who does not know `for_each` has to look it up.

It also does not license changing the attempt loop. Try it if you want to see the failure: a
`find_if` whose predicate prints will compile, and the result will hide the printing, the mutation
and the four outcomes inside a name that promises one decision. Record that attempt in
`EVIDENCE.md` §6 if you make it; the failure is part of the evidence.

## Step 6 — Compare with `construction-16` (~8 min)

| Metric | construction-16 | construction-17 | Difference |
|---|---|---|---|
| Files | 12 | 12 | 0 |
| Headers | 7 | 7 | 0 |
| Translation units | 5 | 5 | 0 |
| Source lines | 606 | 611 | +5 |
| `board.h` | 76 | 75 | -1 |
| `move_generator.h` | 30 | 34 | +4 |
| `game_engine.h` | 300 | 302 | +2 |
| Other 9 files | — | byte-identical | 0 |
| `for` headers | 9 | 5 | -4 |
| Algorithm call sites | 0 | 3 | +3 |
| Lambdas | 0 | 3 | +3 |
| Complete classes | 5 | 5 | 0 |
| Named function bodies | 41 | 41 | 0 |
| Public declarations | 31 | 31 | 0 |
| `sizeof(Board<8, 8>)` | 256 | 256 | 0 |
| `sizeof(GameEngine<8, 8>)` | 24 | 24 | 0 |
| Heap allocations per engine | 3 | 3 | 0 |
| `game.o` text | 4,638 | 4,656 | +18 |
| Binary text / data | 9,652 / 692 | 9,668 / 692 | +16 / 0 |
| Preprocessed lines, `game.cpp` | 54,432 | 72,259 | +17,827 |
| Compile time, `game.cpp` | ~0.50 s | ~0.61 s | +0.11 s |
| Indirect calls in `game.o` | 0 | 0 | 0 |
| Traces | 20 of 20 | 20 of 20 | 0 |

### 6.1 What it buys

- The contract is at the call site. `fill` and `transform` say what happens to every element, and a
  reader does not have to reconstruct the bound and the step.
- The output range is explicit. `transform` takes `candidates.begin()` or `moveOrder`, so the
  destination is an argument rather than a name captured by the loop body.
- The traversal is reusable. The same `fill` and `transform` work on any range with the right
  element type; the loop had to be rewritten for each collection.
- The index arithmetic is isolated in a lambda. A reader who cares about the mapping reads the
  lambda; a reader who cares about the traversal reads the call.
- The board storage gained value semantics and range support without changing its size.

### 6.2 What it costs

- Five include lines and 17,827 preprocessed lines for `game.cpp`.
- About 0.11 s more compile time on this machine.
- 18 more text bytes in `game.o`, 16 in the binary.
- Four times the writes per reset, because `fill` clears the full 16×16 storage.
- One lambda per call site, and a reader who does not know the algorithm must look it up.
- A semantic widening that the transcript cannot see: the reset now touches cells outside the
  active area.
- One header that is not self-contained: `game_engine.h` calls `std::ranges::transform` but includes
  `<ranges>`, not `<algorithm>`. It gets `transform` through `move_generator.h`, which includes
  `<algorithm>`. Add the include directly if you touch that header.

### 6.3 The gap: three traversals changed, four did not

The baseline is not "no manual loops". It is three named traversals and five `for` headers. The
remaining ones are deliberate:

- `printTurnHeader` prints a separated list. It has a per-element effect and a separator rule. The
  change test shows it can become `for_each`, but the baseline leaves it as a loop.
- The attempt loop prints, mutates and branches. It is the counterexample.
- `Board::is_valid` has a pure early exit and could become `any_of`. It is unused by the game, so
  the change would not be observable.
- `parseArguments` walks `argv`. It is parsing, not a traversal of a collection, and a `for` is the
  honest shape.

The precise acceptance sentence is narrower than "the program uses the STL":

> The learner can remove a traversal loop, replace it with an algorithm, and show that all 20
> transcripts are byte-identical.

One traversal, one algorithm, one unchanged transcript. Nothing about speed, readability for every
reader, or removing every loop.

## Step 7 — Explain (~6 min)

Write `EVIDENCE.md` §8. Two parts.

**First, one sentence.** Say what changed between the two baselines and what the change decides,
without using the words *STL*, *ranges*, *algorithm*, *loop*, *iterator*, *better*, *worse*,
*clean* or *modern*. If your sentence needs one of those, you have named the mechanism rather than
the decision.

**Second, one paragraph.** Answer:

- What `fill` decides: every cell of the storage becomes false, including the cells outside the
  active area.
- What `transform` decides: one output per input, in order, with the mapping written as a lambda.
- What `views::iota` decides: the indices are generated, not stored, and nothing is allocated.
- What the `for` count proves: four loop headers disappeared, and five remain.
- What the remaining loops decide: output with separators, the attempt contract, a pure early exit
  that is unused, and argument parsing.
- What the diff proves: three files changed, nine did not.
- What the sizes prove: the board and the engine did not change size.
- What the object code proves: 18 text bytes, no new allocation, no indirect call.
- What the compile numbers prove: the abstraction has a compile-time price.
- What the transcript proves: the change is invisible in behavior.
- What the attempt loop proves: a traversal with effects is not the same shape as a traversal that
  only produces a value.
- What the next lesson owns: purity and immutability, `construction-18`.

A strong answer keeps four things apart: **what the traversal promises** (fill, map, first match),
**what the call site states** (source, destination, operation), **what the compiler pays** (include
weight, compile time, 18 bytes), and **what a run can see** (the transcript, unchanged). A weak
answer says "the STL replaces loops", which is false in both directions: the loop is still inside
the algorithm, and five loops are still in this source.

## Step 8 — Acceptance check and record (~2 min)

The acceptance criterion for this lesson is: **remove a traversal loop, use an algorithm, and show
that the transcript is unchanged.**

Check your record against it:

- [ ] I located the three algorithm call sites and their lines.
- [ ] I located the four `for` headers that disappeared and the five that remain.
- [ ] I explained why `Board::reset` now clears 256 cells instead of 64.
- [ ] I explained why `views::iota` does not allocate.
- [ ] I predicted the structure, size and text tables before measuring.
- [ ] I reproduced all 20 traces on the unmodified baseline.
- [ ] I replaced `printTurnHeader`'s loop in a scratch copy with `for_each` over `iota`.
- [ ] The scratch build passed with `-Wall -Wextra -Werror`.
- [ ] All 20 traces matched after the change.
- [ ] The `for` count in the scratch copy dropped from 5 to 4.
- [ ] I measured `sizeof(Board<8, 8>)` at 256 and `sizeof(GameEngine<8, 8>)` at 24.
- [ ] I measured `game.o` text at 4,656 and the binary text at 9,668.
- [ ] I measured the preprocessed input and the compile time, and recorded both.
- [ ] I explained why the attempt loop stays.
- [ ] I named `Board::is_valid` as a traversal that could be `any_of` and was not changed.
- [ ] I did not edit any tracked code file.
- [ ] I completed `EVIDENCE.md` §1–§10 and the self-check in §12.

The criterion is not "the code uses the STL". It is "you can name what a traversal promises, replace
one with an algorithm, and show that the behavior did not move".

## Self-check (binary)

- [ ] I read `board.h`, `move_generator.h` and `game_engine.h` and located the three algorithm calls
      by line.
- [ ] I counted the `for` headers before and after: 9 and 5.
- [ ] I compared the STL baseline with `construction-16` and named the three changed files.
- [ ] I predicted the structure, size and text tables before running.
- [ ] I reproduced all 20 traces on the unmodified baseline.
- [ ] I replaced one transcript-visible traversal with an algorithm in a scratch copy.
- [ ] I showed that all 20 traces still match after the change.
- [ ] I measured the board at 256 bytes and the engine at 24.
- [ ] I measured `game.o` at 4,656 text bytes and the binary at 9,668.
- [ ] I measured the preprocessed input and the compile time.
- [ ] I named the attempt loop as the counterexample and gave the four properties that keep it a
      loop.
- [ ] I named the semantic difference in `Board::reset`: 256 cells instead of 64.
- [ ] I completed `EVIDENCE.md`.

These are acceptance criteria. If a box is
unchecked, write the next smallest action in `EVIDENCE.md` §10.

## Misconceptions this lesson refutes

| Misconception | Why it fails | What to do instead |
|---|---|---|
| "The STL removes loops." | The loop is inside the algorithm; five `for` headers remain in this source. | Count both: algorithm calls and remaining `for` headers. |
| "Algorithms are always faster." | This lesson measures +18 text bytes, more compile time, and no run-time benchmark. | State which cost you measured, and do not claim the one you did not. |
| "`std::ranges::fill` clears only the active 8×8 area." | It fills all 16 rows of the 16×16 storage. | Read the storage type and the fill argument. |
| "`views::iota` allocates an array of indices." | It is a lazy view; it stores two integers. | Check that the allocation count did not change. |
| "`transform` can write past the output range." | The output range must have room; here it is a fixed `std::array` or a fixed C array. | Count the source size and the destination size. |
| "Any loop can become an algorithm." | The attempt loop interleaves printing, mutation and branching. | Ask what the per-element work does, not how many elements there are. |
| "Early exit always means keep the loop." | `any_of` expresses a pure early exit; `is_valid` is such a case. | Separate the early exit from the effects around it. |
| "The transcript proves the algorithm is correct." | It proves behavior is unchanged; it says nothing about efficiency or readability. | Pair the trace check with the size and compile measurements. |
| "`std::array` is a pointer." | It is a value type wrapping a raw array; here it is still 256 bytes. | Measure `sizeof(Board<8, 8>)`. |
| "Ranges require C++23." | The baseline builds with `-std=c++20`. | Check the build flag, not the version you remember. |
| "The algorithm version has no index." | The lambda still receives an index; the arithmetic moved. | Read the lambda parameter. |
| "The source got shorter." | It grew by 5 lines. | Count lines, not loop headers. |
| "`for_each` can break." | It cannot; there is no early exit in its contract. | Use `any_of` or `find_if` for a pure search, or keep the loop. |
| "`std::ranges::transform` and `std::transform` are the same call." | The ranges version takes a range; the classic version takes two iterators. | Read the first argument. |
| "Each header includes everything it uses." | `game_engine.h` calls `transform` but gets `<algorithm>` transitively through `move_generator.h`. | Include `<algorithm>` directly when you touch that header. |
| "The reset difference does not matter because the transcript matches." | It is four times the writes per reset, and a transcript cannot see writes. | Name the range each version touches. |
| "The attempt loop is just an optimization." | It is a contract: print each attempt, decide, mutate, stop at the first accepted. | Read the four properties in the concept section. |

## Next step

`construction-18` owns purity and immutability. The engine still mutates the board, the knights and
the turn flag in place. A functional style would return a new state and keep the old one, which
makes undo and redo a matter of keeping history instead of reversing effects. The traversal question
is settled here: the loops that remain are the ones whose effects the algorithms cannot name. The
next lesson asks whether those effects should exist at all.

See the [lesson catalog](../../../docs/guide/lesson-catalog.md) for the full path.

If either baseline was missing, do not start `construction-17`. The next contrast needs both sides.
