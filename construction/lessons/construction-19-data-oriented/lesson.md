# Data-oriented design (orientado a datos)

> [English](lesson.md) | [Español](lesson-es.md)

**Concept:** layout · **Time:** about 90 minutes · **Status:** drafted
**Reading:** ~21 minutes

## Why this lesson exists

`construction-17` ended with an engine that owns three heap objects through `std::unique_ptr`, and
with a board that is a class with private data and methods. `construction-19` keeps all of that:

```cpp
std::unique_ptr<Board<8, 8>> board;
std::unique_ptr<Knight> playerKnight;
std::unique_ptr<Knight> machineKnight;
```

The engine is still 24 bytes. Each game still makes three heap allocations. The board is still a
class, and its operations are still methods. What changes is only the representation *inside*
`Board<8, 8>`:

- `construction-17` stores visited cells as a nested `std::array<std::array<bool, 16>, 16>`.
- `construction-19` stores them as a bit-packed `std::array<std::uint64_t, 4>`, and adds a
  `std::array<std::uint8_t, 256> occupant` so a capture is a table lookup.

That is the whole change. It is a **layout** change, and this lesson measures what layout alone
does and does not buy. The honest answer: layout alone did not reduce allocations, did not shrink
the engine, and did not produce a measurable speedup. The win is a different representation and a
larger board object; ownership is unchanged.

Three honest notes before the comparison.

1. The source grows in two files. `board.h` goes from 75 to 118 lines (+43) and `game_engine.h`
   from 302 to 313 (+11). The twelve game files go from 611 to 686 lines, and the benchmark adds
   21 more. Ten of the twelve game files are byte-identical to `construction-17`.
2. `sizeof(Board<8, 8>)` goes from 256 to 288 (+32). `sizeof(GameEngine<8, 8>)` stays 24 in both.
   Heap allocations for 10,000 games stay at 30,000 in both. `game.o` text grows by 305 bytes, and
   the linked binary text by 304.
3. The wall-clock time does not improve. On this machine, 10,000 games take about 270–285 ms in
   `construction-17` and about 270–290 ms in `construction-19`. The ranges overlap, so the
   difference is noise, not a result. Formatting the transcript dominates the run, and both boards
   are small.

The sentence to keep:

> The layout is part of the design: choose a representation for the data, and measure it. But layout
> is one decision among several. Changing the board's representation does not change who owns the
> board, how big the engine is, or how many heap requests the program makes.

The comparison is not "data-oriented design is faster than object-oriented design". It is narrower:
the board stores the same facts in a different shape, the board object grows by 32 bytes, the engine
and the allocation count do not move, and the transcript stays identical.

## What you will be able to do

By the end of this lesson you can read a board representation, say what it stores and where, measure
its size and its cost in object code, and explain why a layout change alone did not move the
allocations or the clock.

## The concept: layout is the design

### The one-variable rule

This lesson changes one variable. Everything else is held fixed on purpose.

| Decision | In this lesson |
|---|---|
| Ownership | Fixed: `std::unique_ptr<Board<8, 8>>` and two `std::unique_ptr<Knight>` |
| Method style | Fixed: `Board` is a class; `reset`, `was_visited`, `mark_visited`, `occupant_at`, `set_occupant` and `clear_occupant` are methods |
| Engine size | Fixed: 24 bytes in both baselines |
| Allocations | Fixed: three per game in both baselines |
| Board layout | Changed: the nested `bool` array becomes bit-packed words plus an occupant table |

If you changed ownership and layout in the same step, a smaller allocation count could come from
either one. You would have two explanations for one number, and no way to choose. This lesson keeps
ownership and method style fixed so that every number it reports can be attributed to the layout
alone.

That is also why the result is modest. Layout is a real decision, but it is not the decision that
removes the allocator. `construction-16` and `construction-17` own that question. Here the engine
still asks the heap for a board and two knights, because that is the design being held constant.

### Two layouts for the same board

`construction-17` stores the board as a nested array of rows:

```cpp
using CellRow = std::array<bool, 16>;
std::array<CellRow, 16> visited;
```

That is 16 rows of 16 `bool` values: 256 bytes, one byte per cell. Access is
`visited[row][column]`, and the compiler computes the row stride. A capture is detected by comparing
the destination with the other knight's row and column; there is no occupant storage.

`construction-19` keeps the same class and the same methods, and changes only the private members:

```cpp
std::array<std::uint64_t, visitedWordCount> visitedWords;
std::array<std::uint8_t, maximumCellCount> occupant;
```

`visitedWords` packs one bit per cell into four 64-bit words: 32 bytes for all 256 cells.
`occupant` stores one byte per cell: 256 bytes. The total is 288 bytes. Both arrays are sized for
the maximum 16×16 board, not the active 8×8 area. The active cells are a subset, and `is_inside`
rejects coordinates outside them. That is why the visited field is 32 bytes and the occupant field
is 256, not 8 and 64.

The methods hide the representation exactly as they did in `construction-17`. `was_visited` becomes
a shift and a mask; `mark_visited` becomes an OR with a shifted bit; `occupant_at` becomes a table
lookup. The caller still writes `board->was_visited(row, column)`. Nothing about the call site
changes.

### What changed, precisely

The two layouts differ in four ways, and it helps to keep them apart:

1. **Visited storage.** 17 uses one byte per cell in a nested array. 19 uses one bit per cell in
   four words. The visited field falls from 256 bytes to 32.
2. **Occupant storage.** 17 has no occupant array; it detects a capture by comparing the
   destination with the other knight's coordinates. 19 stores an occupant byte per cell, so a
   capture is a table lookup. The occupant field adds 256 bytes.
3. **Cell access.** 17 indexes two array dimensions. 19 computes a flat index and then a word index
   and a bit offset. The bitboard is compact, but a reader has to know the mapping.
4. **Net board size.** 288 instead of 256. The 224 bytes saved on visited are more than offset by
   the 256 bytes added for occupants.

Everything else stays. The engine still holds three `unique_ptr` members. The board is still a
class with private data and methods. Copy is still deleted and move still defaulted. The knights,
the generator, the rules and the transcript are untouched.

### What layout means here

Layout is about which bytes are close to each other and which bytes are touched together:

- **Spatial locality:** consecutive cells are adjacent in memory. `cell_index(row, column)` maps a
  board coordinate to a flat offset, so a scan over cells touches consecutive addresses.
- **Temporal locality:** the same board is read and written on every turn. The engine does not
  rebuild it between turns.
- **Field grouping:** the 19 layout groups by access pattern. Visited bits are read and written as
  a bitboard; occupants are read and written as bytes. Each array is indexed by the same
  `cell_index`.

This is the struct-of-arrays idea at the board level: two parallel arrays, each indexed by the same
cell index. It is not a universal rule. It pays when a traversal touches one field of many cells.
Here the board is only 288 bytes, and both layouts fit in a handful of cache lines. The locality
argument is real, but the benchmark is not sensitive enough to show it. That is the point of
measuring instead of assuming.

### Why nothing else moved

**The engine did not shrink.** `sizeof(GameEngine<8, 8>)` is 24 in both baselines because the three
members are still three pointers. The board's contents changed; the board's location did not. A
`unique_ptr` is one pointer whether it points at 256 bytes or 288.

**The allocation count did not fall.** The engine still calls `std::make_unique` three times per
game. The board constructor now initializes a different pair of arrays, but the board is still a
separate heap block. 10,000 games still make 30,000 requests in both baselines.

**The wall time did not improve.** The benchmark redirects standard output to `/dev/null`, so the
write syscalls are cheap, but `printf` still formats every line. Each game prints a header, a line
per turn, a line per attempted move, and a footer. Formatting dominates the run. The board is also
small in both layouts, so the working set fits in a few cache lines either way.

**The object code grew.** `game.o` text goes from 4,656 to 4,961 (+305). The bit operations
(`index / 64`, shifts, masks) and the occupant writes are more instructions than a nested array
index and a coordinate comparison. The linked binary text grows by 304. This is a cost that does
move, and it moves in the direction you would expect from a more compact representation with more
per-access arithmetic.

**The transcript did not move.** All 20 golden traces match byte for byte. The rules did not change,
so behavior did not change.

### What the change does not prove

It does not prove that data-oriented design is faster in general. It does not prove that
struct-of-arrays beats every other layout for every workload. It does not prove that cache misses
are irrelevant; it proves that this board is too small and this workload too formatting-bound for
this benchmark to expose them. It does not prove that the engine uses less memory; the engine is the
same size and the board is larger. It does not prove that ownership changed, because it did not.

It proves one narrow thing: the representation inside a class can change while ownership, method
style, engine size, allocation count and behavior stay fixed, at a measurable cost in board bytes
and object text.

## Before you start

- [`construction-05`](../construction-05-array-of-records/lesson.md) — collection and traversal.
  The board is a collection of cells, and this lesson changes how those cells are stored.
- [`construction-17`](../construction-17-stl/lesson.md) — the immediate baseline and the left side
  of the comparison. Its `code/` directory is the artifact this lesson starts from.
- [`construction-16`](../construction-16-ownership/lesson.md) — optional context. The ownership
  model (copy deleted, move defaulted) is inherited from there and is held fixed here.
- A C++20 compiler (`g++` or `clang++`). `cmake` is optional; `run.sh` falls back to a direct
  compile when CMake is absent.

You do not need to know how a bitboard is implemented. You need to read two layouts, say what each
one stores and where, predict the profile, run 10,000 games, and explain which numbers moved and
which did not.

### What you need

- The data-oriented baseline:
  [`code/main.cpp`](code/main.cpp), [`code/game.h`](code/game.h),
  [`code/game.cpp`](code/game.cpp), [`code/game_engine.h`](code/game_engine.h),
  [`code/board.h`](code/board.h), [`code/move_generator.h`](code/move_generator.h),
  [`code/knight.h`](code/knight.h), [`code/knight.cpp`](code/knight.cpp),
  [`code/position.h`](code/position.h), [`code/position.cpp`](code/position.cpp),
  [`code/random_sequence.h`](code/random_sequence.h),
  [`code/random_sequence.cpp`](code/random_sequence.cpp), and
  [`code/benchmark.cpp`](code/benchmark.cpp).
- The previous baseline, for the comparison:
  [`code/board.h`](../construction-17-stl/code/board.h),
  [`code/game_engine.h`](../construction-17-stl/code/game_engine.h), and their ten companions in
  [`../construction-17-stl/code/`](../construction-17-stl/code/).
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
test -f code/benchmark.cpp &&
test -f ../construction-17-stl/code/board.h &&
test -f ../construction-17-stl/code/game_engine.h &&
test -f ../../../common/traces/seed-0001.txt
```

If a file is missing, the comparison has no second side. Record the missing file in `EVIDENCE.md`
§10 and return when it is available. A count against a file you have not read is not evidence.

The 8×8 baseline reproduces all 20 traces in `common/traces/` byte for byte, before you change
anything. That is the behavioral contract, and this lesson does not touch it.

## The timed path

This lesson is long by design. Split it into two sittings:

- **Sitting 1 — Core (Steps 1–3, about 40 min).** Stop after Step 3, when both layouts are
  measured and the sizes are known.
- **Sitting 2 — Extension (Steps 4–6, about 45 min).** Start only after Sitting 1. You will
  profile 10,000 games and explain the flat result.

The remaining five minutes are buffer for the record and the self-check.

## Step 1 — Inventory the two layouts (~15 min)

Read `board.h` and `game_engine.h` in both baselines. Do not infer the layout from the README; read
the declarations.

```bash
repoRoot="$(git rev-parse --show-toplevel)"
lesson17="${repoRoot}/construction/lessons/construction-17-stl"
lesson19="${repoRoot}/construction/lessons/construction-19-data-oriented"

grep -n 'class Board\|visited\|occupant\|unique_ptr\|make_unique' \
  "${lesson17}/code/board.h" "${lesson17}/code/game_engine.h"
grep -n 'class Board\|visitedWords\|occupant\|unique_ptr\|make_unique' \
  "${lesson19}/code/board.h" "${lesson19}/code/game_engine.h"
```

Now confirm that ownership and method style are the same on both sides:

```bash
grep -n 'std::unique_ptr\|std::make_unique' "${lesson17}/code/game_engine.h"
grep -n 'std::unique_ptr\|std::make_unique' "${lesson19}/code/game_engine.h"

grep -n '^    \(void\|int\|bool\|std::uint8_t\) ' "${lesson17}/code/board.h"
grep -n '^    \(void\|int\|bool\|std::uint8_t\) ' "${lesson19}/code/board.h"
```

The ownership lines match: three `make_unique` calls in the constructor and three `unique_ptr`
members. Both listings are methods. 19 adds `occupant_at`, `set_occupant` and `clear_occupant`; the
rest keep their names. No method became a free function, and both boards are still classes.

Fill this table from the source:

| Question | construction-17 | construction-19 |
|---|---|---|
| Board type | `Board<8, 8>` (class) | `Board<8, 8>` (class) |
| Visited storage | `std::array<std::array<bool, 16>, 16>` | `std::array<std::uint64_t, 4>` |
| Bits per visited cell | 8 (a `bool`) | 1 |
| Visited bytes | 256 | 32 |
| Occupant storage | none; capture compares coordinates | `std::array<std::uint8_t, 256>` |
| Occupant bytes | 0 | 256 |
| Board bytes | 256 | 288 |
| Engine members | 3 `std::unique_ptr` | 3 `std::unique_ptr` |
| Engine bytes | 24 | 24 |
| Heap allocations per game | 3 | 3 |
| Board operations | methods | methods |
| Engine copy / move | copy deleted, move defaulted | copy deleted, move defaulted |

Then answer in `EVIDENCE.md` §2:

1. Which two files differ between the baselines, and which ten are byte-identical?
2. How many bytes does each visited field use, and how many bits per cell?
3. How many bytes does the occupant field add, and what does it replace?
4. What is `sizeof(Board<8, 8>)` in each baseline?
5. What is `sizeof(GameEngine<8, 8>)` in each, and why is it the same?
6. How many heap allocations does each game make in each baseline?
7. Which member declarations show that ownership did not change?
8. What invariant does the occupant array add, and which class keeps it private?
9. What does a reader have to know to read `was_visited` in 19 that 17 did not require?
10. Which ten files are byte-identical, and what does that prove about the change?

Questions 8 and 9 are the design costs. Question 8: the occupant array must agree with the knights'
positions for every move that continues the game. A non-capturing move clears the old cell and sets
the new one; a capture sets the new cell and ends the game, so the old cell is left stale but never
read again. The array is private, so `Board` still owns the invariant; the engine maintains it by
calling the methods in the right order. Question 9: the bitboard requires the flat index and the
word/bit split, which the class hides but a reader of `board.h` must understand.

## Step 2 — Predict before you run (~10 min)

Write these predictions in `EVIDENCE.md` §3 before running anything. Do not look at the measured
numbers first.

### 2.1 The layout table

| Question | My prediction |
|---|---|
| `sizeof(Board<8, 8>)` in 17 | |
| `sizeof(Board<8, 8>)` in 19 | |
| `sizeof(GameEngine<8, 8>)` in 17 | |
| `sizeof(GameEngine<8, 8>)` in 19 | |
| Heap allocations per game in 17 | |
| Heap allocations per game in 19 | |
| Total allocations for 10,000 games in 17 | |
| Total allocations for 10,000 games in 19 | |
| Files that differ between the baselines | |
| Traces that match, of 20, in each | |

### 2.2 The profile table

| Question | My prediction |
|---|---|
| 10,000-game wall time in 17 | |
| 10,000-game wall time in 19 | |
| Which one is faster | |
| Difference in milliseconds | |
| `game.o` text in 17 and 19 | |
| Binary text in 17 and 19 | |
| Binary data in 17 and 19 | |
| Source lines in 17 and 19 | |

Before you fill the table, decide what your model predicts. The members and the `make_unique`
calls are unchanged, so a model that ties allocations to ownership predicts no movement in the
engine size or the allocation count. A model that ties allocations to the board's representation
predicts a drop. Write your prediction, then measure. For the board size, compare what the bitboard
saves with what the occupant table adds before you decide the direction. The gap between your
prediction and the observation is the lesson, whether it is zero or not.

### 2.3 The explanation test

| Question | My prediction |
|---|---|
| Will the board size change? | |
| Will the engine size change? | |
| Will the allocation count change? | |
| Will the wall time change? | |
| Why? | |
| What does the transcript see? | |
| What does `sizeof(GameEngine<8, 8>)` include? | |
| What does it not include? | |

The last two rows are the ones that are easy to get wrong. `sizeof(GameEngine<8, 8>)` includes the
three `unique_ptr` members. It does not include the board or the knights, which live in separate
heap blocks. Changing the board's contents does not change the size of the pointer that owns it.

## Step 3 — Run the traces and the sizes (~15 min)

Build both baselines with the same flags:

```bash
mkdir -p /tmp/layout-test

c++ -std=c++20 -Wall -Wextra -Werror -O2 -I"${lesson17}/code" \
  -o /tmp/layout-test/baseline17 \
  "${lesson17}/code/main.cpp" "${lesson17}/code/game.cpp" \
  "${lesson17}/code/knight.cpp" "${lesson17}/code/position.cpp" \
  "${lesson17}/code/random_sequence.cpp"

c++ -std=c++20 -Wall -Wextra -Werror -O2 -I"${lesson19}/code" \
  -o /tmp/layout-test/baseline19 \
  "${lesson19}/code/main.cpp" "${lesson19}/code/game.cpp" \
  "${lesson19}/code/knight.cpp" "${lesson19}/code/position.cpp" \
  "${lesson19}/code/random_sequence.cpp"
```

Check all 20 traces on each side:

```bash
check_traces() {
  local binary="$1"
  local passed=0
  local total=0
  local trace
  for trace in "${repoRoot}"/common/traces/seed-*.txt; do
    local traceName
    local seedText
    local seedNumber
    traceName="$(basename "${trace}" .txt)"
    seedText="${traceName#seed-}"
    seedNumber="$((10#${seedText}))"
    total=$((total + 1))
    if "${binary}" --seed "${seedNumber}" --name Learner | cmp -s - "${trace}"; then
      passed=$((passed + 1))
    fi
  done
  printf '%s: %d/%d traces matched\n' "${binary}" "${passed}" "${total}"
}

check_traces /tmp/layout-test/baseline17
check_traces /tmp/layout-test/baseline19
```

```text
/tmp/layout-test/baseline17: 20/20 traces matched
/tmp/layout-test/baseline19: 20/20 traces matched
```

Now the sizes. The probe goes in the scratch directory, never in the tracked files:

```bash
cat > /tmp/layout-test/sizes.cpp <<'EOF'
#include "game_engine.h"

#include <cstdio>

int main()
{
    std::printf("sizeof(Board<8,8>)=%zu\n", sizeof(Board<8, 8>));
    std::printf("sizeof(GameEngine<8,8>)=%zu\n", sizeof(GameEngine<8, 8>));
    std::printf("sizeof(Knight)=%zu\n", sizeof(Knight));
    std::printf("sizeof(Position)=%zu\n", sizeof(Position));
    return 0;
}
EOF

c++ -std=c++20 -O2 -I"${lesson17}/code" \
  -o /tmp/layout-test/sizes17 /tmp/layout-test/sizes.cpp \
  "${lesson17}/code/knight.cpp" "${lesson17}/code/position.cpp"

c++ -std=c++20 -O2 -I"${lesson19}/code" \
  -o /tmp/layout-test/sizes19 /tmp/layout-test/sizes.cpp \
  "${lesson19}/code/knight.cpp" "${lesson19}/code/position.cpp"

/tmp/layout-test/sizes17
/tmp/layout-test/sizes19
```

The output on this platform:

```text
sizeof(Board<8,8>)=256
sizeof(GameEngine<8,8>)=24
sizeof(Knight)=40
sizeof(Position)=8
sizeof(Board<8,8>)=288
sizeof(GameEngine<8,8>)=24
sizeof(Knight)=40
sizeof(Position)=8
```

The first four lines are `construction-17`; the last four are `construction-19`. The board grew by
32 bytes. The engine did not move.

Now the object code:

```bash
c++ -std=c++20 -O2 -I"${lesson17}/code" -c "${lesson17}/code/game.cpp" \
  -o /tmp/layout-test/game17.o
c++ -std=c++20 -O2 -I"${lesson19}/code" -c "${lesson19}/code/game.cpp" \
  -o /tmp/layout-test/game19.o
size /tmp/layout-test/game17.o /tmp/layout-test/game19.o
size /tmp/layout-test/baseline17 /tmp/layout-test/baseline19
```

```text
   text	   data	    bss	    dec	    hex	filename
   4656	      0	      0	   4656	   1230	/tmp/layout-test/game17.o
   4961	      0	      0	   4961	   1361	/tmp/layout-test/game19.o
   text	   data	    bss	    dec	    hex	filename
   9668	    692	     24	  10384	   2890	/tmp/layout-test/baseline17
   9972	    692	     24	  10688	   29c0	/tmp/layout-test/baseline19
```

`game.o` grows by 305 text bytes. The linked binary grows by 304 text bytes. Data and BSS are
identical: 692 and 24. The extra beyond `game.o` is link-time layout and alignment, not a second
implementation. The direction is what matters: the bit operations and the occupant writes are real
instructions, and they show up in the text size.

Record the commands, the sizes and the object numbers in `EVIDENCE.md` §4.

If either trace check is not 20 of 20, stop. That is a different baseline from the one this lesson
describes; record it in `EVIDENCE.md` §10 and do not adjust the prediction to match.

## Step 4 — Profile 10,000 games (~20 min)

The benchmark constructs a fresh engine for each seed, runs the game with standard output
redirected to `/dev/null`, and reports elapsed milliseconds on standard error. `code/benchmark.cpp`
ships with the 19 baseline. Use the same probe for both sides so the comparison is symmetric:

```bash
cat > /tmp/layout-test/benchmark.cpp <<'EOF'
#include "game_engine.h"

#include <chrono>
#include <cstdio>

int main()
{
    std::freopen("/dev/null", "w", stdout);
    auto startTime = std::chrono::steady_clock::now();
    for (int seedValue = 1; seedValue <= 10000; seedValue += 1)
    {
        GameEngine<8, 8> engine;
        engine.run(seedValue, "Learner");
    }
    auto endTime = std::chrono::steady_clock::now();
    auto elapsedMilliseconds =
        std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime);
    std::fprintf(stderr, "milliseconds=%lld\n",
                 static_cast<long long>(elapsedMilliseconds.count()));
    return 0;
}
EOF

c++ -std=c++20 -O2 -I"${lesson17}/code" -o /tmp/layout-test/bench17 \
  /tmp/layout-test/benchmark.cpp "${lesson17}/code/knight.cpp" \
  "${lesson17}/code/position.cpp" "${lesson17}/code/random_sequence.cpp"

c++ -std=c++20 -O2 -I"${lesson19}/code" -o /tmp/layout-test/bench19 \
  /tmp/layout-test/benchmark.cpp "${lesson19}/code/knight.cpp" \
  "${lesson19}/code/position.cpp" "${lesson19}/code/random_sequence.cpp"
```

If `/dev/null` does not exist on your platform, drop the `freopen` line and redirect standard
output when you run the binary.

Run each side three times:

```bash
for run in 1 2 3; do /tmp/layout-test/bench17; done
for run in 1 2 3; do /tmp/layout-test/bench19; done
```

The output on this machine:

```text
milliseconds=275
milliseconds=271
milliseconds=276
milliseconds=278
milliseconds=273
milliseconds=268
```

The first three lines are `construction-17`; the last three are `construction-19`. The ranges
overlap, and a different run can put either side first. Record your own three values for each side;
do not copy these. The lesson's claim is "within noise", not "19 is faster".

### 4.1 Count the allocations

The benchmark counts nothing. Add a counting `operator new` in the scratch directory:

```bash
cat > /tmp/layout-test/allocations.cpp <<'EOF'
#include "game_engine.h"

#include <cstdio>
#include <cstdlib>
#include <new>

namespace
{
std::size_t allocationCount = 0;
}

void *operator new(std::size_t size)
{
    allocationCount += 1;
    if (void *pointer = std::malloc(size == 0 ? 1 : size))
    {
        return pointer;
    }
    throw std::bad_alloc();
}

void operator delete(void *pointer) noexcept
{
    std::free(pointer);
}

void operator delete(void *pointer, std::size_t) noexcept
{
    std::free(pointer);
}

int main()
{
    std::freopen("/dev/null", "w", stdout);
    for (int seedValue = 1; seedValue <= 10000; seedValue += 1)
    {
        GameEngine<8, 8> engine;
        engine.run(seedValue, "Learner");
    }
    std::fprintf(stderr, "allocations=%zu\n", allocationCount);
    return 0;
}
EOF

c++ -std=c++20 -O2 -I"${lesson17}/code" -o /tmp/layout-test/alloc17 \
  /tmp/layout-test/allocations.cpp "${lesson17}/code/knight.cpp" \
  "${lesson17}/code/position.cpp" "${lesson17}/code/random_sequence.cpp"

c++ -std=c++20 -O2 -I"${lesson19}/code" -o /tmp/layout-test/alloc19 \
  /tmp/layout-test/allocations.cpp "${lesson19}/code/knight.cpp" \
  "${lesson19}/code/position.cpp" "${lesson19}/code/random_sequence.cpp"

/tmp/layout-test/alloc17
/tmp/layout-test/alloc19
```

```text
allocations=30000
allocations=30000
```

The override counts every `operator new` call in the process. In this program the only allocations
are the engine's three per game, so the count maps directly to the engine. It is 30,000 on both
sides because the layout change did not touch the three `make_unique` calls. The board's arrays are
inside the heap block that `make_unique<Board<8, 8>>` already creates; packing visited cells into
four words does not remove that block.

Record the times and the allocation counts in `EVIDENCE.md` §5.

## Step 5 — Compare (~15 min)

**The change test.** Measure six things on both baselines: `sizeof(Board<8, 8>)`,
`sizeof(GameEngine<8, 8>)`, heap allocations for 10,000 games, wall time, `game.o` text, and the
number of matching traces. The board size and the object text move. The engine size, the allocation
count and the wall time do not. The traces stay identical. The table below records all six.

| Metric | construction-17 | construction-19 | Difference |
|---|---|---|---|
| Source lines (game) | 611 | 686 | +75 |
| Source lines (total) | 611 | 707 | +96 |
| `board.h` lines | 75 | 118 | +43 |
| `game_engine.h` lines | 302 | 313 | +11 |
| `sizeof(Board<8, 8>)` | 256 | 288 | +32 |
| `sizeof(GameEngine<8, 8>)` | 24 | 24 | 0 |
| `sizeof(Knight)` | 40 | 40 | 0 |
| Heap allocations per game | 3 | 3 | 0 |
| Heap allocations per 10,000 games | 30,000 | 30,000 | 0 |
| 10,000-game wall time | about 270–285 ms | about 270–290 ms | within noise |
| `game.o` text | 4,656 | 4,961 | +305 |
| Binary text | 9,668 | 9,972 | +304 |
| Binary data | 692 | 692 | 0 |
| Binary BSS | 24 | 24 | 0 |
| Traces | 20 of 20 | 20 of 20 | 0 |

### 5.1 What moved

- **The board representation.** Visited cells went from one byte each to one bit each. Occupants
  went from "not stored" to one byte each. The class now has two arrays instead of one.
- **The board size.** 256 to 288. The bitboard saves 224 bytes on visited, and the occupant array
  adds 256.
- **The object text.** +305 bytes in `game.o`. The bit operations (`index / 64`, shifts, masks) and
  the occupant writes are more instructions than a nested array index and a coordinate comparison.
- **The source.** +75 lines in the game files, +96 with the benchmark.

### 5.2 What did not move

- **The engine size.** 24 bytes in both. The three `unique_ptr` members are unchanged.
- **The allocation count.** 30,000 in both. Three `make_unique` calls per game in both.
- **The wall time, within noise.** This is the number the lesson refuses to spin.
- **The transcript.** All 20 traces match on both sides.
- **The rules.** Move order, capture, trap, draw and the 200-move limit are the same.
- **The ownership contract.** Copy is deleted, move is defaulted in both.
- **The method style.** `Board` is a class with private data and methods in both.
- **Ten of the twelve game files.** `main.cpp`, `game.h`, `game.cpp`, `move_generator.h`,
  `knight.h`, `knight.cpp`, `position.h`, `position.cpp`, `random_sequence.h` and
  `random_sequence.cpp` are byte-identical.

### 5.3 What it costs

- **32 more bytes per board.** 288 instead of 256. The occupant array costs more than the bitboard
  saves.
- **305 more text bytes in `game.o`.** 304 in the linked binary. The compact representation is not
  free at the instruction level.
- **Less readable cell access.** `visited[row][column]` becomes
  `(visitedWords[index / 64] >> (index % 64)) & 1`. The class hides it, but a reader of `board.h`
  has to know the mapping.
- **A maintained invariant.** The occupant array must agree with the knights' positions for every
  move that continues the game. A non-capturing move clears the old cell and sets the new one; a
  capture sets the new cell and ends the game, so the old cell is stale but never read again. A
  missed clear on a non-capturing move produces a ghost occupant, and the transcript can show it if
  a knight later returns to that cell. The array is private, so `Board` still enforces its own
  invariants; the engine maintains this one through the methods.
- **More source.** 75 lines in the game files, 96 with the benchmark.
- **No allocation win and no speed win.** The two numbers a reader might expect from "data-oriented
  design" are flat here, because the design held ownership fixed.

None of these is a defect. They are the price of a more compact visited representation plus a
capture table, and the record should list them next to the benefit.

### 5.4 The honest result

A layout change is easy to oversell. The compact visited field is real: 256 bytes become 32. The
capture table is real: a comparison becomes a lookup. But the board object is larger, the engine is
the same size, the allocator is still called three times per game, and the run does not get faster.
The change bought a representation, not a resource profile.

That is the result to report. A learner who says "the board is bit-packed, so the program is
faster" has skipped the measurement. A learner who says "the program is not faster, so the layout
change was pointless" has skipped the representation. The lesson is to report both and explain why
they diverge.

Record the comparison table in `EVIDENCE.md` §6.

## Step 6 — Explain and record (~10 min)

Write `EVIDENCE.md` §7. Two parts.

**First, one sentence.** Say what changed between the two baselines and what the change decides,
without using the words *cache*, *struct-of-arrays*, *array-of-structs*, *data-oriented*, *better*,
*faster*, *slower* or *optimization*. If your sentence needs one of those, you have named the
mechanism or the verdict rather than the decision.

**Second, one paragraph.** Answer:

- What the layout decides: visited bits and occupants are stored in separate flat arrays, each
  indexed by the same cell index.
- What the bitboard decides: 256 cells fit in four 64-bit words, and a read or write is a shift and
  a mask.
- What the occupant array decides: a capture is a table lookup, not a comparison with the other
  knight's coordinates, and the board must maintain the table on every move that continues the
  game.
- What stayed fixed: the engine owns the board and both knights through three `unique_ptr` members,
  and `Board` is still a class with methods.
- What the sizes prove: the board grows by 32 bytes and the engine stays at 24.
- What the allocation count proves: 30,000 requests stay 30,000, because the three `make_unique`
  calls did not change.
- What the wall time proves: the run does not get faster, because formatting dominates and the
  board is small either way.
- What the object code proves: the bit operations and occupant writes cost 305 text bytes in
  `game.o`.
- What the transcript proves: the behavior is unchanged.
- What the lesson does not prove: that this layout is faster in general, that layout removes the
  allocator, or that cache misses are irrelevant everywhere.
- What the next lesson owns: ports and adapters, `construction-20`.

A strong answer keeps four things apart: **what the layout promises** (fields grouped by access),
**what stayed fixed** (ownership, methods, engine size, allocations), **what the compiler pays**
(305 text bytes and a larger board), and **what a run can see** (the transcript unchanged, the time
unchanged, the allocation count unchanged). A weak answer says "data-oriented design is faster" or
"the engine holds its state inline"; both are false for this baseline.

Then check your record against the acceptance criteria in the self-check below.

## Self-check (binary)

- [ ] I compared the two layouts field by field and named the two files that differ.
- [ ] I predicted the layout and profile tables before running.
- [ ] I reproduced all 20 traces on both baselines.
- [ ] I measured `sizeof(Board<8, 8>)` at 256 and 288.
- [ ] I measured `sizeof(GameEngine<8, 8>)` at 24 in both.
- [ ] I counted the allocations on both sides: 30,000 and 30,000.
- [ ] I ran 10,000 games on both sides and recorded the elapsed time.
- [ ] I measured `game.o` text at 4,656 and 4,961, and the binary text at 9,668 and 9,972.
- [ ] I explained why the board grew by 32 bytes: the bitboard saves 224, the occupant array adds
      256.
- [ ] I explained why the engine size and the allocation count did not move: ownership is unchanged.
- [ ] I explained why the wall time did not move: formatting dominates and the board is small.
- [ ] I named the occupant invariant and the bitboard's readability cost.
- [ ] I did not edit any tracked code file.
- [ ] I completed `EVIDENCE.md` §1–§9 and the self-check in §11.

These are acceptance criteria. If a box is unchecked, write the next smallest action in
`EVIDENCE.md` §9.

## Misconceptions this lesson refutes

| Misconception | Why it fails | What to do instead |
|---|---|---|
| "The engine now holds its state inline." | The members are still three `unique_ptr`; the engine is 24 bytes in both. | Read the member declarations and measure `sizeof`. |
| "Heap allocations fell to zero." | Each game still calls `make_unique` three times: 30,000 for 10,000 games. | Run the allocation probe on both sides. |
| "The engine grew to 368 bytes." | It is 24 bytes in both baselines. | Measure `sizeof(GameEngine<8, 8>)` in each. |
| "Methods became free functions." | `Board` is still a class, and the operations are still methods. | Read `board.h` and the call sites. |
| "A data-oriented design must remove the heap." | Layout and ownership are separate decisions; this lesson holds ownership fixed. | Change one decision at a time and attribute the numbers. |
| "The bitboard makes the whole board smaller." | Visited falls from 256 to 32 bytes, but occupant adds 256; the board grows to 288. | Compare fields, then the whole object. |
| "The layout change made the program faster." | The measured ranges overlap; the difference is noise. | Report the range, not a single sample. |
| "The layout change saved memory." | The board is 32 bytes larger and the engine is unchanged. | Sum the board and the engine before comparing. |
| "`sizeof(GameEngine<8, 8>)` includes the board." | It includes the pointers only; the board is in a separate heap block. | Read the members, not just the size. |
| "The occupant array is free because it replaces a comparison." | It adds 256 bytes, writes on every move, and grows the object text. | Count the bytes, the writes and the text. |
| "Struct-of-arrays is always better than array-of-structs." | It depends on the access pattern and the working-set size. Here the board is small either way. | Name the access pattern before choosing a layout. |
| "Redirecting stdout to `/dev/null` removes the formatting cost." | `/dev/null` removes the write cost, not the formatting cost. | Say what the benchmark includes: construction, generation, formatting. |
| "Three allocations per game is 30,000 for 10,000 games, so it must show." | The lesson measures the whole run; the allocation count alone does not prove a speedup. | Report both numbers and explain the difference. |
| "The transcript proves the layout is better." | It proves behavior is unchanged; it says nothing about cost. | Pair the trace check with the profile. |
| "The bitboard is just an optimization." | It is a representation decision with a readability cost and a size tradeoff. | Explain the mapping and the field sizes. |
| "A class cannot be data-oriented." | The layout lives inside the class; the public interface is unchanged. | Separate representation from ownership and interface. |
| "The 19 baseline changed the rules." | All 20 traces match; only `board.h` and `game_engine.h` differ. | Diff the files and run the traces. |

## Terms used here

| Term | Meaning in this lesson |
|---|---|
| Layout | How a value's fields are arranged in memory: order, size, padding and grouping. |
| Bitboard | A representation that stores one bit per cell in an array of machine words. |
| Struct-of-arrays | Grouping fields by access pattern into parallel arrays instead of per-record structs. |
| Spatial locality | The tendency of nearby addresses to be used close together in time. |
| Temporal locality | Reusing the same address or object across nearby operations. |
| Working set | The bytes a computation actually touches during a run. |
| Heap allocation | A request to the allocator, here through `std::make_unique`. |
| Object text | The machine-code bytes in an object file or binary, reported by `size`. |
| One-variable rule | Change one design decision per comparison so the numbers can be attributed. |
| Golden trace | A stored transcript that the program must reproduce byte for byte. |

## What "done" means here

You are done when every box in **Self-check (binary)** above is checked and `EVIDENCE.md` §1–§9 and
§11 are filled with your own measurements. There is no score. If a box is unchecked, write the next
smallest action in `EVIDENCE.md` §9 and do that action before moving on.

The criterion is not "the code is data-oriented". It is "you can run the profile, report the board
size, the engine size, the allocation count, the wall time and the object text, and explain which
numbers moved while the ownership stayed fixed".

## Next step

`construction-20` owns ports and adapters. The engine still prints directly with `std::printf`, so
the core cannot run without a console. A hexagonal design puts an interface between the game and its
output, which makes the same core serve a console adapter and a test adapter. The layout question is
settled narrowly here: the board representation changed, the board grew by 32 bytes, and the engine,
the allocator and the clock did not move. The next lesson asks who is allowed to talk to that state.

If you want the other lens first, `construction-18` owns purity and immutability. It asks whether
the board should be replaced instead of mutated, a different answer to the same cost question.

See the [lesson catalog](../../../docs/guide/lesson-catalog.md) for the full path.

If either baseline was missing, do not start `construction-19`. The comparison needs both sides.

## References

- [`construction-17` — Standard Template Library and algorithms](../construction-17-stl/lesson.md),
  the baseline this lesson compares against.
- [`construction-17` code](../construction-17-stl/code/) — `board.h` and `game_engine.h` are the
  left side of every table here.
- [`construction-16` — Ownership](../construction-16-ownership/lesson.md) — the ownership model
  held fixed in this lesson.
- [`common/spec/game-rules.md`](../../../common/spec/game-rules.md) — board, move order, capture,
  trap, draw and the 200-move limit.
- [`common/spec/transcript-format.md`](../../../common/spec/transcript-format.md) — the byte-level
  output contract.
- [`common/traces/`](../../../common/traces/) — the 20 golden traces.
- [`EVIDENCE.md`](EVIDENCE.md) — the learner record for this lesson.
- [cppreference: `std::array`](https://en.cppreference.com/w/cpp/container/array) and
  [cppreference: `std::unique_ptr`](https://en.cppreference.com/w/cpp/memory/unique_ptr) — the two
  library types that define the layouts compared here.
