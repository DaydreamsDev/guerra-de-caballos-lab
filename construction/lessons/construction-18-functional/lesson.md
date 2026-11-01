# Functional style: purity and immutability (funcional)

> [English](lesson.md) | [Español](lesson-es.md)

**Concept:** purity and immutability · **Time:** about 85 minutes · **Status:** drafted
**Reading:** ~22 minutes

## Why this lesson exists

`construction-17` ended with a question it could not answer. Three traversals had a name, and one
loop did not, because its per-element work prints, mutates and branches. Naming the loop would have
hidden its effects. This lesson asks a different question about the same code:

> Which of those effects has to exist at all, and which one can be replaced by a value?

The answer is narrower than "make the program functional". One function is already pure and does not
change. One class becomes a value that is copied instead of mutated. One container keeps the copies.
And the game loop still mutates, because a turn still has to print, decide and move a knight.

The sentence to keep:

> A pure function returns the same value for the same input and changes nothing. An immutable value
> is never changed after it is built; a "change" produces a new value. When the old values are kept,
> undo stops being a computation and becomes a lookup.

Three honest notes before the comparison.

1. One file changes. `game_engine.h` grows from 302 to 346 lines, and the diff adds 44 lines and
   removes none. The other eleven files are byte-identical to `construction-17`. That includes
   `board.h` and `move_generator.h`: purity in move generation and value semantics in the board were
   already there. This lesson adds immutability at the history boundary, not purity at the leaves.
2. The structure barely moves: 5 `for` headers, 3 algorithm call sites, 3 lambdas, 41 to 45 named
   function bodies, 31 to 34 public declarations. What changes is the size of the engine, from 24 to
   56 bytes, because it now owns a `std::vector` and a `std::size_t`.
3. The cost is memory, and it grows with the game. On this machine the 20 golden seeds allocate
   between 8,272 and 32,848 bytes each, against a flat 336 bytes in `construction-17`. Undo becomes
   cheap in time and expensive in space. That trade is the lesson, and the record should say which
   side the learner weighs.

The comparison is not "immutable is better than mutable". It is narrower: one container of snapshots
makes undo and redo a value copy instead of a replay of the move rules, the transcript is identical
before and after, and the engine pays for the history in bytes.

## What you will be able to do

By the end of this lesson you can:

- explain purity and immutability;
- implement undo and redo through board history.

## The concept: a value that is never changed

### Purity is a contract, not a keyword

`generate_move_candidates` is a pure function of a piece:

```cpp
template <typename PieceType>
std::array<MoveCandidate, moveCandidateCount> generate_move_candidates(const PieceType &piece)
{
    std::array<MoveCandidate, moveCandidateCount> candidates = {};
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
    return candidates;
}
```

Four properties make it pure, and none of them is a language keyword:

1. **The input is read-only.** `const PieceType &` cannot be written through.
2. **The output is returned.** The function does not write to a parameter or to a member.
3. **It reads no hidden state.** No board, no random sequence, no global counter.
4. **Same input, same output.** Two calls with the same piece return the same eight records.

This function is byte-identical to `construction-17`. The lesson's diff does not touch it. That is
the first honest point: **purity was already present in the leaves, and this lesson does not add it
there.** What the lesson adds is a place where a whole object is treated as a value.

### Immutability is a property of the storage

`Board` is a value type:

```cpp
using CellRow = std::array<bool, maximumColumnCount>;

std::array<CellRow, maximumRowCount> visited;
```

`sizeof(Board<8, 8>)` is 256 bytes. There is no pointer, no `new`, no indirection. A copy of a board
is a 256-byte copy, and the copy shares nothing with the original.

That last sentence is the whole mechanism. Consider the alternative:

```cpp
std::vector<Board<RowCount, ColumnCount> *> history; // not this baseline
```

With pointers, `history.push_back(board.get())` stores the address of the *current* board. The next
`mark_visited` would change the snapshot too, because the snapshot and the board would be the same
object. The history would record nothing. Immutability here is not a discipline that callers
promise to respect; it is a consequence of storing values.

### Undo is a lookup, not a replay

There are two ways to implement undo.

**Replay.** To undo move 21, throw the state away and re-run moves 1 to 20 from the seed. This is
correct only if the replay reproduces every input: the random sequence state, the starting
positions, the roll order, the move order. It costs O(k) work to undo move k, and it costs a
deterministic replay contract that this program does not have, because `random_sequence` keeps
hidden state that advances.

**Snapshot.** Keep every board after every accepted move. To undo, step the index back and copy:

```cpp
bool undo_last_move()
{
    if (historyIndex == 0)
    {
        return false;
    }
    historyIndex -= 1;
    *board = history[historyIndex];
    return true;
}
```

That is 2 lines of state change and one 256-byte assignment, regardless of which move you undo. It
does not know the move rules. It cannot get the move rules wrong.

| Property | Replay | Snapshot |
|---|---|---|
| Work to undo move k | O(k) moves replayed | O(1) index step plus 256 bytes |
| Needs the move rules | yes, all of them | no |
| Needs the random sequence to rewind | yes | no |
| Needs a deterministic replay contract | yes | no |
| Memory | one board | one board per move |
| Can be wrong when a rule changes | yes | no |

The table is the lesson. The snapshot wins on every row except memory, and memory is the row this
lesson measures.

### The redo branch

`record_history` is five lines, and the first two are the ones that matter:

```cpp
void record_history()
{
    history.erase(history.begin() + static_cast<std::ptrdiff_t>(historyIndex + 1),
                  history.end());
    history.push_back(*board);
    historyIndex = history.size() - 1;
}
```

`erase` deletes everything after the current index before appending the new snapshot. That is the
rule "a new move after undo erases the redo branch". After an undo, the snapshots in front of the
index describe a future that no longer follows from the present. Keeping them would let redo return
to a board that never came after the current one.

### What the boundary does not cover

`history` stores `Board<RowCount, ColumnCount>`. It does not store the knight positions, the
`playerIsActive` flag, or the random sequence state.

So `undo_last_move()` restores the visited cells and nothing else. It is an honest board-level undo,
not a game-level undo. A full undo needs a `GameState` record that holds the board, both knights,
the turn flag and the generator position, and then the history becomes a history of that record.

This is the lesson's real limit, and Step 4.4 measures it rather than hiding it.

## Before you start

- [`construction-17`](../construction-17-stl/lesson.md) — the immediate baseline and the left side
  of the diff in Step 6. Its `code/` directory is the artifact this lesson starts from.
- [`construction-16`](../construction-16-ownership/lesson.md) — optional context. The three
  `unique_ptr` members survive here, and they are why the engine owns its parts instead of copying
  them.
- [`construction-05`](../construction-05-array-of-records/lesson.md) — collection and traversal. The
  board is a collection of cells, and a snapshot is a copy of that collection.
- [`construction-09`](../construction-09-abstract-data-type/lesson.md) — optional context. The
  boundary between what the engine exposes and what it hides is what makes the history private.
- A C++20 compiler (`g++` or `clang++`). `cmake` is optional; `run.sh` falls back to a direct
  compile when CMake is absent.

You do not need to know how `std::vector` grows. You need to read one assignment, say what it
copies, and show that the transcript did not change.

### What you need

- The functional baseline:
  [`code/main.cpp`](code/main.cpp), [`code/game.h`](code/game.h),
  [`code/game.cpp`](code/game.cpp), [`code/game_engine.h`](code/game_engine.h),
  [`code/board.h`](code/board.h), [`code/move_generator.h`](code/move_generator.h),
  [`code/knight.h`](code/knight.h), [`code/knight.cpp`](code/knight.cpp),
  [`code/position.h`](code/position.h), [`code/position.cpp`](code/position.cpp),
  [`code/random_sequence.h`](code/random_sequence.h),
  [`code/random_sequence.cpp`](code/random_sequence.cpp).
- The previous baseline, for the diff:
  [`code/game_engine.h`](../construction-17-stl/code/game_engine.h) and its eleven companions in
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
test -f ../construction-17-stl/code/game_engine.h &&
test -f ../../../common/traces/seed-0001.txt
```

If a file is missing, the comparison has no second side. Record the missing file in `EVIDENCE.md`
§11 and return when it is available. A count against a file you have not read is not evidence.

The 8×8 baseline reproduces all 20 traces in `common/traces/` byte for byte, before you change
anything. That is the behavioral contract, and this lesson does not touch it.

## Step 1 — Inventory the state (~10 min)

Start with what changed and what did not:

```bash
for file in board.h move_generator.h game_engine.h; do
  diff -u "../construction-17-stl/code/${file}" "code/${file}"
done
```

Only `game_engine.h` produces output. Now the counts:

```bash
grep -rn 'for (' code/ | wc -l
grep -rn 'ranges::\|views::' code/ | wc -l
grep -n 'history' code/game_engine.h
```

```text
5
3
```

The `for` count and the algorithm count are the same as `construction-17`. This lesson removes no
loop and adds no algorithm. The `history` grep is the new surface.

`game_engine.h`, history-relevant ranges:

| Lines | Contents |
|---|---|
| 8–16 | Includes. `<cstddef>` and `<vector>` are new. |
| 54–57 | `history_size()`. Public, `const`. |
| 59–68 | `undo_last_move()`. Public. Refuses at index 0. |
| 70–79 | `redo_last_move()`. Public. Refuses at the end. |
| 81–101 | `run`. Clears the history, pushes the initial board, sets index 0. |
| 104–108 | Members: `board`, two knights, `history`, `historyIndex`. |
| 231–237 | `record_history()`. Erase the redo branch, push, move the index. |
| 262–330 | `playTurn`. Calls `record_history` at line 308 and line 320. |

`board.h`, value-relevant ranges:

| Lines | Contents |
|---|---|
| 65–74 | `maximumRowCount`, `maximumColumnCount`, `CellRow`, and the `std::array` storage. |
| 42–45 | `reset()`. `std::ranges::fill` over the storage. |
| 47–63 | `was_visited` and `mark_visited`. The only mutator is `mark_visited`. |

Counts for this baseline. The counting rule is stated so you can reproduce it:

- Headers: 7. Translation units: 5. Complete class definitions: 5.
- Named function bodies written by hand: 45, up from 41. The four new ones are `history_size`,
  `undo_last_move`, `redo_last_move` and `record_history`.
- Public declarations: 34, up from 31. The three new ones are `history_size`, `undo_last_move` and
  `redo_last_move`.
- Lambdas: 3. `for` headers: 5. Algorithm call sites: 3. All unchanged.
- Source lines: 655, which is 611 plus 44.
- Files that differ from `construction-17`: 1 of 12.

Then answer in `EVIDENCE.md` §2:

1. Which file changed, and by how many lines?
2. Which two files did not change that you expected to change, and why did they not?
3. How many named function bodies are there, and which four are new?
4. How many public declarations are there, and which three are new?
5. Where is `record_history` called, and on which lines?
6. What type does `history` store, and how large is one element?
7. What does `undo_last_move` do to `historyIndex`?
8. What does `record_history` erase before it pushes?
9. Which parts of the game state are not in the history?
10. Which function in this baseline is pure, and which properties prove it?

Question 2 has a precise answer: `move_generator.h` and `board.h` did not change, because purity in
move generation and value semantics in the board were already true in `construction-17`. The new
thing is not the value type; it is the container of values.

## Step 2 — Predict before you run (~10 min)

Write these predictions before running anything. Do not look at the answers in Step 3 first.

### 2.1 The structure table

| Question | My prediction |
|---|---|
| Files that differ from `construction-17` | |
| Source lines in `construction-17` | |
| Source lines here | |
| `game_engine.h` lines, before and after | |
| Added / removed lines in the diff | |
| Named function bodies, before and after | |
| Public declarations, before and after | |
| `for` headers, before and after | |
| Algorithm call sites, before and after | |
| Lambdas, before and after | |

The row that is easy to get wrong is the last three. This lesson changes none of them, and a learner
who expects "functional style" to remove loops will predict a drop.

### 2.2 The size and memory table

| Question | My prediction |
|---|---|
| `sizeof(Board<8, 8>)` | |
| `sizeof(GameEngine<8, 8>)`, before and after | |
| `sizeof(std::vector<Board<8, 8>>)` | |
| Heap allocations per game, before and after | |
| Bytes allocated per game, seed 1, before and after | |
| `history_size()` after seed 1 | |
| `history_size()` for the largest of the 20 seeds | |
| `game.o` text bytes, before and after | |
| Binary text bytes, before and after | |
| Binary data bytes, before and after | |
| Seeds that match, of 20 | |

Predict `history_size()` as `moves + 1`. Seed 1 reports 21 moves, so predict 22 snapshots: the
initial board plus one per accepted move.

Predict the allocation count as small and non-constant. A `std::vector` reallocates as it grows, so
the count depends on how many moves the game lasts.

### 2.3 The change test predictions

The change test is: observe that a board snapshot is really restored, from outside the engine, and
show that the transcript is unchanged.

| Question | My prediction |
|---|---|
| What I will add to the scratch copy | |
| Which file I will edit | |
| `sizeof(GameEngine<8, 8>)` after the change | |
| Does the board change on undo? | |
| Is the board identical after undo then redo? | |
| Seeds that match, of 20, after the change | |
| Undo successes from the end of seed 1 | |
| Redo successes from the start | |

Predict the size as unchanged. A member function that returns a reference adds no data member.

## Step 3 — Run the baseline (~15 min)

First the traces, unmodified:

```bash
mkdir -p /tmp/functional-test
./run.sh --seed 1 --name Learner > /tmp/functional-test/baseline-0001.txt
diff -u ../../../common/traces/seed-0001.txt /tmp/functional-test/baseline-0001.txt
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
  -o build/functional-baseline code/main.cpp code/game.cpp code/knight.cpp \
  code/position.cpp code/random_sequence.cpp
```

The transcript ends with:

```text
winner=machine
moves=21
```

Now the probe. It goes in the scratch directory, never in the tracked files:

```bash
cat > /tmp/functional-test/probe.cpp <<'EOF'
#include "game_engine.h"

#include <cstdio>
#include <vector>

int main()
{
    std::printf("sizeof(Board<8,8>)=%zu\n", sizeof(Board<8, 8>));
    std::printf("sizeof(GameEngine<8,8>)=%zu\n", sizeof(GameEngine<8, 8>));
    std::printf("sizeof(Knight)=%zu\n", sizeof(Knight));
    std::printf("sizeof(Position)=%zu\n", sizeof(Position));
    std::printf("sizeof(MoveCandidate)=%zu\n", sizeof(MoveCandidate));
    std::printf("sizeof(std::vector<Board<8,8>>)=%zu\n",
                sizeof(std::vector<Board<8, 8>>));

    GameEngine<8, 8> engine;
    engine.run(1, "Learner");
    std::printf("history=%zu\n", engine.history_size());
    std::printf("undo=%d\n", engine.undo_last_move());
    std::printf("redo=%d\n", engine.redo_last_move());

    int undoCount = 0;
    while (engine.undo_last_move())
    {
        undoCount += 1;
    }
    int redoCount = 0;
    while (engine.redo_last_move())
    {
        redoCount += 1;
    }
    std::printf("undo_successes=%d\n", undoCount);
    std::printf("redo_successes=%d\n", redoCount);
    return 0;
}
EOF
g++ -std=c++20 -Wall -Wextra -Werror -O2 -Icode \
  -o /tmp/functional-test/probe /tmp/functional-test/probe.cpp \
  code/game.cpp code/knight.cpp code/position.cpp code/random_sequence.cpp
/tmp/functional-test/probe | tail -9
```

The output on this platform:

```text
sizeof(Board<8,8>)=256
sizeof(GameEngine<8,8>)=56
sizeof(Knight)=40
sizeof(Position)=8
sizeof(MoveCandidate)=12
sizeof(std::vector<Board<8,8>>)=24
history=22
undo=1
redo=1
undo_successes=21
redo_successes=21
```

Read the last four lines carefully. The engine ends at index 21 of 22 snapshots. One undo returns
true, one redo returns true, and the board is back where it started. Then the boundary is exact: 21
undoes succeed and the 22nd returns false, because index 0 is the initial board and there is nothing
before it. From index 0, 21 redos succeed and the 22nd returns false, because index 21 is the last
snapshot.

Now the allocation cost, with a counter on `operator new`:

```bash
cat > /tmp/functional-test/alloc.cpp <<'EOF'
#include "game_engine.h"

#include <cstdio>
#include <cstdlib>
#include <new>

static std::size_t allocationCount = 0;
static std::size_t allocatedBytes = 0;

void *operator new(std::size_t size)
{
    allocationCount += 1;
    allocatedBytes += size;
    return std::malloc(size == 0 ? 1 : size);
}

void operator delete(void *memory) noexcept
{
    std::free(memory);
}

int main()
{
    for (int seed = 1; seed <= 20; seed += 1)
    {
        allocationCount = 0;
        allocatedBytes = 0;
        GameEngine<8, 8> engine;
        std::fflush(stdout);
        engine.run(seed, "Learner");
        std::fprintf(stderr, "seed=%d history=%zu allocations=%zu bytes=%zu\n",
                     seed, engine.history_size(), allocationCount, allocatedBytes);
        std::fflush(stdout);
    }
    return 0;
}
EOF
g++ -std=c++20 -O2 -Wno-mismatched-new-delete -Icode \
  -o /tmp/functional-test/alloc /tmp/functional-test/alloc.cpp \
  code/game.cpp code/knight.cpp code/position.cpp code/random_sequence.cpp
/tmp/functional-test/alloc > /dev/null
```

```text
seed=1 history=22 allocations=9 bytes=16464
seed=2 history=29 allocations=9 bytes=16464
seed=3 history=29 allocations=9 bytes=16464
seed=4 history=41 allocations=10 bytes=32848
seed=5 history=50 allocations=10 bytes=32848
seed=6 history=21 allocations=9 bytes=16464
seed=7 history=11 allocations=8 bytes=8272
seed=8 history=27 allocations=9 bytes=16464
seed=9 history=44 allocations=10 bytes=32848
seed=10 history=23 allocations=9 bytes=16464
seed=11 history=9 allocations=8 bytes=8272
seed=12 history=37 allocations=10 bytes=32848
seed=13 history=15 allocations=8 bytes=8272
seed=14 history=18 allocations=9 bytes=16464
seed=15 history=29 allocations=9 bytes=16464
seed=16 history=21 allocations=9 bytes=16464
seed=17 history=10 allocations=8 bytes=8272
seed=18 history=13 allocations=8 bytes=8272
seed=19 history=35 allocations=10 bytes=32848
seed=20 history=48 allocations=10 bytes=32848
```

The same probe against `construction-17` reports `allocations=3 bytes=336` for every seed. The three
allocations are the `unique_ptr` targets: one board and two knights.

The bytes follow a rule you can check by hand. A vector that ends with capacity 16 allocated
1 + 2 + 4 + 8 + 16 = 31 blocks of 256 bytes, which is 7,936, plus the 336 bytes of the three
`unique_ptr` targets: 8,272. Capacity 32 gives 63 blocks: 16,464. Capacity 64 gives 127 blocks:
32,848. The allocation count is the number of doublings plus the three `unique_ptr` targets.

The live memory at the end of a game is smaller than the bytes allocated: a game that reaches 50
snapshots uses 50 × 256 = 12,800 bytes of a 16,384-byte buffer. The rest is the abandoned buffers
along the way.

The worst case is bounded, and the bound is in the source: `maximumMoveCount` is 200, so a game has
at most 200 accepted moves and at most 201 snapshots. The vector would reach capacity 256, which is
65,536 bytes, plus the 336 bytes of the three `unique_ptr` targets.

Now the object code and the compile cost:

```bash
g++ -std=c++20 -O2 -Icode -c code/game.cpp -o /tmp/functional-test/game18.o
g++ -std=c++20 -O2 -I../construction-17-stl/code \
  -c ../construction-17-stl/code/game.cpp -o /tmp/functional-test/game17.o
size /tmp/functional-test/game17.o /tmp/functional-test/game18.o
g++ -std=c++20 -O2 -Icode -o /tmp/functional-test/bin18 \
  code/main.cpp code/game.cpp code/knight.cpp code/position.cpp code/random_sequence.cpp
g++ -std=c++20 -O2 -I../construction-17-stl/code -o /tmp/functional-test/bin17 \
  ../construction-17-stl/code/main.cpp ../construction-17-stl/code/game.cpp \
  ../construction-17-stl/code/knight.cpp ../construction-17-stl/code/position.cpp \
  ../construction-17-stl/code/random_sequence.cpp
size /tmp/functional-test/bin17 /tmp/functional-test/bin18
g++ -std=c++20 -Icode -E code/game.cpp | wc -l
g++ -std=c++20 -I../construction-17-stl/code -E ../construction-17-stl/code/game.cpp | wc -l
```

```text
   text	   data	    bss	    dec	    hex	filename
   4656	      0	      0	   4656	   1230	/tmp/functional-test/game17.o
   7240	      0	      0	   7240	   1c48	/tmp/functional-test/game18.o
   text	   data	    bss	    dec	    hex	filename
   9668	    692	     24	  10384	   2890	/tmp/functional-test/bin17
  12368	    700	     24	  13092	   3324	/tmp/functional-test/bin18
  76303
  72259
```

The engine grew by 2,584 text bytes in `game.o` and 2,700 in the binary. That is the vector
machinery and the two copies of the board assignment, inlined. Data grows by 8 bytes.

Compile time, three runs each:

```bash
for run in 1 2 3; do
  /usr/bin/time -f '%e' g++ -std=c++20 -O2 -Icode -c code/game.cpp \
    -o /tmp/functional-test/t18.o
done
for run in 1 2 3; do
  /usr/bin/time -f '%e' g++ -std=c++20 -O2 -I../construction-17-stl/code \
    -c ../construction-17-stl/code/game.cpp -o /tmp/functional-test/t17.o
done
```

```text
0.72
0.72
0.72
0.60
0.63
0.61
```

About 0.11 s more, and 4,044 more preprocessed lines for `game.cpp`. Your numbers will differ; the
direction should not.

If the first diff is not empty, stop. That is a different baseline from the one this lesson
describes; record it in `EVIDENCE.md` §11 and do not adjust the prediction to match.

## Step 4 — Compare: what the history proves (~15 min)

Open `game_engine.h` and `board.h`. Five facts, read against the measurements.

1. **The snapshot is a value, not an address.** `history` holds `Board<RowCount, ColumnCount>` by
   value. `sizeof(Board<8, 8>)` is 256, and one `push_back` copies 256 bytes. Nothing is shared
   between a snapshot and the live board.
2. **Undo does not know the rules.** `undo_last_move` steps `historyIndex` and assigns
   `*board = history[historyIndex]`. There is no call to `tryMove`, no move order, no dice. The
   function cannot be wrong about a rule it never reads.
3. **The redo branch is erased, not flagged.** `record_history` calls `erase` on the tail before
   `push_back`. There is no "invalid" marker to forget to check.
4. **The engine grew, the board did not.** `sizeof(GameEngine<8, 8>)` goes from 24 to 56, because a
   `std::vector` is 24 bytes and `historyIndex` is 8. `sizeof(Board<8, 8>)` stays 256.
5. **The memory cost is per move and bounded.** 256 bytes per snapshot, at most 201 snapshots, at
   most 65,536 bytes of buffer for this board size.

### 4.1 The exact diff

One file changed. `diff -u` against `construction-17` gives 44 added lines and none removed:

```bash
diff -u ../construction-17-stl/code/game_engine.h code/game_engine.h
```

| Region | Added | Removed | What changed |
|---|---|---|---|
| Includes | 2 | 0 | `<cstddef>`, `<vector>` |
| Public API | 27 | 0 | `history_size`, `undo_last_move`, `redo_last_move` |
| `run` | 3 | 0 | Clear, push the initial board, set index 0 |
| Members | 2 | 0 | `history`, `historyIndex` |
| `record_history` | 8 | 0 | Erase the tail, push, move the index |
| `playTurn` | 2 | 0 | Two `record_history()` calls |
| Total | 44 | 0 | One file, eleven unchanged |

Not one line of logic was deleted. The change is purely additive.

### 4.2 What did not change

- The transcript. All 20 traces match, byte for byte.
- The rules. Move order, capture, trap, draw and the 200-move limit are the same.
- The turn loop. `playTurn` still prints each attempt, mutates four pieces of state, handles four
  outcomes and stops in two ways.
- `generate_move_candidates`. Byte-identical, still pure.
- `board.h`. Byte-identical, still a 256-byte value.
- The traversal count. 5 `for` headers, 3 algorithm call sites, 3 lambdas.
- The three `unique_ptr` members. The engine still owns its board and its two knights.
- The allocation count for the three parts. 3 allocations, 336 bytes, plus the history.

### 4.3 What it costs

| Metric | construction-17 | construction-18 | Difference |
|---|---|---|---|
| `sizeof(GameEngine<8, 8>)` | 24 | 56 | +32 |
| Allocations, seed 1 | 3 | 9 | +6 |
| Bytes allocated, seed 1 | 336 | 16,464 | +16,128 |
| Bytes allocated, largest seed | 336 | 32,848 | +32,512 |
| Live snapshots, seed 1 | 0 | 22 | +22 |
| Live board memory, seed 1 | 256 | 5,632 | +5,376 |
| `game.o` text | 4,656 | 7,240 | +2,584 |
| Binary text | 9,668 | 12,368 | +2,700 |
| Preprocessed lines | 72,259 | 76,303 | +4,044 |
| Compile time | ~0.61 s | ~0.72 s | +0.11 s |

The live board memory row is the honest one: 22 snapshots × 256 bytes = 5,632 bytes held at the end
of seed 1, against 256 bytes before. The allocated-bytes row is larger because a growing vector
abandons its old buffers.

None of this is a defect. It is the price of keeping every state instead of recomputing it, and the
record should list it next to the benefit.

### 4.4 The boundary, precisely

The history stores the board. It does not store:

- the row and column of `playerKnight`,
- the row and column of `machineKnight`,
- the `playerIsActive` flag, which is a local in `run` and passed by reference through the loop,
- the state of the random sequence, which lives in `random_sequence.cpp` and advances on every call.

Therefore `undo_last_move()` after a game restores the visited cells of the previous turn and leaves
everything else at its final value. The probe in Step 3 shows exactly that: `undo=1`, and the
transcript is not involved, because undo prints nothing.

This is not a bug in the baseline; it is the size of the boundary the baseline chose. A game-level
undo needs a state record:

```cpp
struct GameState
{
    Board<8, 8> board;
    Knight player;
    Knight machine;
    bool playerIsActive;
    std::uint64_t generatorState;
};
```

Then `history` becomes `std::vector<GameState>`, and `undo_last_move` assigns the whole record. The
cost per snapshot rises from 256 bytes to about 256 + 40 + 40 + 1 + 8, which is 345 bytes before
padding. That is the next change, and it is not this lesson.

### 4.5 Undo by lookup against undo by replay, with numbers

| Question | Replay | Snapshot |
|---|---|---|
| Work to undo the last move of seed 1 | 21 turns replayed | one index step, one 256-byte copy |
| Work to undo the first move of seed 1 | 1 turn replayed | 20 index steps, one 256-byte copy |
| Code that must be correct | every rule and the generator | one assignment |
| Memory held | 256 bytes | 5,632 bytes for seed 1 |
| Memory held, worst case | 256 bytes | 65,536 bytes of buffer |
| Behavior visible in the transcript | none | none |

Both columns are invisible to the transcript. That is why the acceptance needs a probe as well as
the traces.

## Step 5 — Change test: observe the snapshot from outside (~15 min)

The acceptance names the change test: **undo and redo a move through board history and show that all
20 traces still pass.**

The probe in Step 3 calls `undo_last_move` and `redo_last_move`, but it cannot see the board:
`board` is private and there is no accessor. To observe that the snapshot is really restored, add a
read-only accessor in a scratch copy.

### 5.1 Make a scratch copy

Never edit the tracked `code/` directory. Copy it:

```bash
rm -rf /tmp/functional-test/change
cp -r code /tmp/functional-test/change
```

### 5.2 Add the accessor

In `/tmp/functional-test/change/game_engine.h`, after `history_size()` (line 57), add:

```cpp
    const Board<RowCount, ColumnCount> &current_board() const
    {
        return *board;
    }
```

This is a `const` accessor that returns a reference. It exposes reading and forbids writing. It adds
no data member and changes no logic.

### 5.3 Fingerprint the board

```bash
cat > /tmp/functional-test/fingerprint.cpp <<'EOF'
#include "game_engine.h"

#include <cstdio>
#include <string>

static std::string fingerprint(const Board<8, 8> &board)
{
    std::string bits;
    for (int row = 1; row <= 8; row += 1)
    {
        for (int column = 1; column <= 8; column += 1)
        {
            bits += board.was_visited(row, column) ? '1' : '0';
        }
    }
    return bits;
}

int main()
{
    GameEngine<8, 8> engine;
    engine.run(1, "Learner");
    std::string afterGame = fingerprint(engine.current_board());
    std::printf("history=%zu\n", engine.history_size());
    std::printf("undo=%d\n", engine.undo_last_move());
    std::string afterUndo = fingerprint(engine.current_board());
    std::printf("redo=%d\n", engine.redo_last_move());
    std::string afterRedo = fingerprint(engine.current_board());
    std::printf("state_restored=%d\n", afterGame == afterRedo);
    std::printf("undo_changed_board=%d\n", afterGame != afterUndo);
    std::printf("sizeof(GameEngine<8,8>)=%zu\n", sizeof(GameEngine<8, 8>));
    return 0;
}
EOF
g++ -std=c++20 -Wall -Wextra -Werror -O2 -I/tmp/functional-test/change \
  -o /tmp/functional-test/fingerprint /tmp/functional-test/fingerprint.cpp \
  /tmp/functional-test/change/game.cpp /tmp/functional-test/change/knight.cpp \
  /tmp/functional-test/change/position.cpp /tmp/functional-test/change/random_sequence.cpp
/tmp/functional-test/fingerprint | tail -6
```

```text
history=22
undo=1
redo=1
state_restored=1
undo_changed_board=1
sizeof(GameEngine<8,8>)=56
```

Three facts, each one measured:

- `undo_changed_board=1`: the 64-bit fingerprint of the visited cells differs after the undo, so the
  undo did restore an earlier board and not the current one.
- `state_restored=1`: after undo then redo, the fingerprint equals the fingerprint at the end of the
  game. The value round-trips.
- `sizeof(GameEngine<8,8>)=56`: the accessor added no data.

### 5.4 Show the transcript is unchanged

```bash
g++ -std=c++20 -Wall -Wextra -Werror -O2 -I/tmp/functional-test/change \
  -o /tmp/functional-test/change-bin \
  /tmp/functional-test/change/main.cpp /tmp/functional-test/change/game.cpp \
  /tmp/functional-test/change/knight.cpp /tmp/functional-test/change/position.cpp \
  /tmp/functional-test/change/random_sequence.cpp
for trace in ../../../common/traces/seed-*.txt; do
  traceName="$(basename "${trace}" .txt)"
  seedText="${traceName#seed-}"
  seedNumber="$((10#${seedText}))"
  /tmp/functional-test/change-bin --seed "${seedNumber}" --name Learner | cmp - "${trace}"
done
```

The loop is silent. All 20 traces match with the accessor present.

### 5.5 What the change test proves, and what it does not

It proves that a board snapshot is restored by a value copy, and that adding a read-only window into
the engine changes neither the size nor the output.

It does not prove that undo is complete. The fingerprint covers the visited cells only, because the
history covers the visited cells only. To fingerprint the knights and the turn flag you would need
them in the snapshot, which is the `GameState` change from Step 4.4.

It also does not prove that the redo branch is erased. No public call in this baseline performs a
move after an undo: the game loop never undoes, and `run` clears the history. The erase is a
read-the-code fact, visible in `record_history`, and Step 3's integer model shows what the two lines
do to a sequence. If you want to run it, make `record_history` public in the scratch copy and call
it after an undo, then check that `history_size()` stopped growing. Record that attempt in
`EVIDENCE.md` §6.5 if you make it.

## Step 6 — Compare with `construction-17` (~10 min)

| Metric | construction-17 | construction-18 | Difference |
|---|---|---|---|
| Files | 12 | 12 | 0 |
| Headers | 7 | 7 | 0 |
| Translation units | 5 | 5 | 0 |
| Source lines | 611 | 655 | +44 |
| `game_engine.h` | 302 | 346 | +44 |
| Other 11 files | — | byte-identical | 0 |
| Complete classes | 5 | 5 | 0 |
| Named function bodies | 41 | 45 | +4 |
| Public declarations | 31 | 34 | +3 |
| Lambdas | 3 | 3 | 0 |
| `for` headers | 5 | 5 | 0 |
| Algorithm call sites | 3 | 3 | 0 |
| `sizeof(Board<8, 8>)` | 256 | 256 | 0 |
| `sizeof(GameEngine<8, 8>)` | 24 | 56 | +32 |
| Heap allocations per game | 3 | 8 to 10 | +5 to +7 |
| Bytes allocated per game | 336 | 8,272 to 32,848 | +7,936 to +32,512 |
| `game.o` text | 4,656 | 7,240 | +2,584 |
| Binary text / data | 9,668 / 692 | 12,368 / 700 | +2,700 / +8 |
| Preprocessed lines, `game.cpp` | 72,259 | 76,303 | +4,044 |
| Compile time, `game.cpp` | ~0.61 s | ~0.72 s | +0.11 s |
| Traces | 20 of 20 | 20 of 20 | 0 |

### 6.1 What it buys

- Undo and redo exist at all. `construction-17` had no way to step backwards; this baseline does, in
  two public functions.
- Undo cannot disagree with the rules, because it never reads them.
- The history is a plain value container. Copy it, serialize it, or compare two snapshots byte by
  byte; nothing needs a custom clone.
- The snapshot is protected from later mutation by value semantics, not by discipline.
- The boundary is explicit. `history` is private, and the only way to move through it is the three
  public functions.

### 6.2 What it costs

- 256 bytes per accepted move, plus vector slack. Up to 65,536 bytes of buffer at the 200-move cap.
- 32 more bytes in every `GameEngine`, whether or not the game uses undo.
- 2,584 more text bytes in `game.o`, and 2,700 in the binary.
- 4,044 more preprocessed lines and about 0.11 s more compile time for `game.cpp`.
- A second copy of the state that can drift from the live one if a future change mutates the board
  without calling `record_history`. The two `record_history()` calls in `playTurn` are the only
  thing keeping them in step.
- A partial undo that a reader may mistake for a full one. Step 4.4 is the correction.

### 6.3 The gap: immutability at one boundary, mutation everywhere else

The baseline is not a functional program. It is a mutable game with an immutable history:

- `playTurn` mutates the board, the active knight and the turn flag in place.
- `mark_visited` is a mutator, and it is called on the live board.
- `tryMove` is `const`, but it writes two results through `int &` parameters. Its signature says
  "read-only" while its parameter list says "three outputs".
- `generate_move_candidates` is the only function that is pure in both signature and body.
- `record_history` copies the board, so the copy is a value; the loop that produced it is not.

The precise acceptance sentence is narrower than "the program is functional":

> The learner can undo and redo a move through board history and show that all 20 transcripts are
> byte-identical.

Two functions, one container, twenty unchanged traces. Nothing about removing mutation from the turn
loop, and nothing about making the whole game immutable.

## Step 7 — Explain (~8 min)

Write `EVIDENCE.md` §8. Two parts.

**First, one sentence.** Say what changed between the two baselines and what the change decides,
without using the words *functional*, *pure*, *immutable*, *snapshot*, *history*, *better*, *worse*,
*clean* or *modern*. If your sentence needs one of those, you have named the mechanism rather than
the decision.

**Second, one paragraph.** Answer:

- What `generate_move_candidates` decides: the same piece produces the same eight candidates, and
  nothing outside the piece is read.
- What `Board` decides: it is 256 bytes of value, and a copy shares nothing with its source.
- What `history` decides: every accepted move leaves a value behind, and the index names which one
  is current.
- What `undo_last_move` decides: one index step and one assignment, with no knowledge of the rules.
- What `record_history` decides: the future after the current index is discarded before a new
  present is appended.
- What the diff proves: one file changed, eleven did not, and the changed file added no loop and no
  algorithm.
- What the sizes prove: the engine grew by 32 bytes and the board did not move.
- What the allocation numbers prove: 336 bytes became 8,272 to 32,848, and the growth follows the
  vector's doubling.
- What the traces prove: the new bookkeeping is invisible in behavior.
- What the probe proves: undo and redo really move the board, and the boundaries return `false`.
- What the boundary does not cover: knights, turn flag and generator state.
- What the next lesson owns: data layout and locality, `construction-19`.

A strong answer keeps four things apart: **what is a value** (the board, the snapshots), **what is
still mutated** (the live board, the knight, the turn flag), **what the change buys** (undo as a
lookup), and **what it costs** (256 bytes per move). A weak answer says "the program is functional
now", which is false in both directions: most of the state is still mutable, and the one pure
function did not change in this lesson.

## Step 8 — Acceptance check and record (~2 min)

The acceptance criterion for this lesson is: **undo and redo work through board history, and all 20
traces still pass.**

Check your record against it:

- [ ] I located the four new functions and the three new public declarations by line.
- [ ] I explained why `board.h` and `move_generator.h` did not change.
- [ ] I explained why a snapshot must be a value and not a pointer.
- [ ] I explained what `record_history` erases and why.
- [ ] I predicted the structure, size and memory tables before measuring.
- [ ] I reproduced all 20 traces on the unmodified baseline.
- [ ] I ran the probe and read `history=22`, `undo=1`, `redo=1`.
- [ ] I found the boundaries: 21 undos and 21 redos for seed 1.
- [ ] I measured the allocation cost across the 20 seeds.
- [ ] I added a read-only accessor in a scratch copy and fingerprinted the board.
- [ ] `undo_changed_board=1` and `state_restored=1` in the scratch build.
- [ ] All 20 traces matched with the accessor present.
- [ ] `sizeof(GameEngine<8, 8>)` was 56 before and after the accessor.
- [ ] I named the three parts of the state that the history does not store.
- [ ] I explained that undo is a value copy, not a replay of the move rules.
- [ ] I did not edit any tracked code file.
- [ ] I completed `EVIDENCE.md` §1–§10 and the self-check in §12.

The criterion is not "the code is functional". It is "you can step backwards through a container of
values and show that the behavior did not move".

## Self-check (binary)

- [ ] I read `game_engine.h` and located `history_size`, `undo_last_move`, `redo_last_move`,
      `record_history` and the two call sites by line.
- [ ] I compared the baseline with `construction-17` and named the one changed file.
- [ ] I predicted the structure, size and memory tables before running.
- [ ] I reproduced all 20 traces on the unmodified baseline.
- [ ] I ran the probe and read the history size, the undo, the redo and the two boundaries.
- [ ] I measured the allocations and bytes across the 20 seeds.
- [ ] I measured `sizeof(GameEngine<8, 8>)` at 56 and `sizeof(Board<8, 8>)` at 256.
- [ ] I measured `game.o` at 7,240 text bytes and the binary at 12,368.
- [ ] I observed a restored board from outside the engine in a scratch copy.
- [ ] I showed that all 20 traces still match after the change.
- [ ] I named the three parts of the state that the history does not store.
- [ ] I explained the memory tradeoff and its 200-move bound.
- [ ] I completed `EVIDENCE.md`.

These are acceptance criteria. If a box is
unchecked, write the next smallest action in `EVIDENCE.md` §10.

## Misconceptions this lesson refutes

| Misconception | Why it fails | What to do instead |
|---|---|---|
| "The program is functional now." | Only `generate_move_candidates` is pure, and it was already pure. The turn loop still mutates. | Name which value is immutable and which state is still mutated. |
| "`const` makes a function pure." | `tryMove` is `const` and writes two results through `int &` parameters. | Check the parameters as well as the qualifier. |
| "A snapshot can be a `Board *`." | The next `mark_visited` would change the snapshot, because it is the same object. | Store the value, and check that a copy shares nothing. |
| "Undo replays the moves." | `undo_last_move` steps an index and assigns a value. It never calls the rules. | Read the body: two state changes, no loop. |
| "Undo restores the whole game." | The history holds the board only; the knights, the turn flag and the generator are elsewhere. | Fingerprint what you claim to restore. |
| "The transcript proves undo works." | Undo prints nothing, so no trace can see it. | Pair the trace check with a probe. |
| "The redo branch is kept and skipped." | `record_history` erases the tail before appending. | Read the `erase` call and its arguments. |
| "History memory is unbounded." | `maximumMoveCount` caps the game at 200 moves, so at most 201 snapshots. | Compute the worst case from the constant. |
| "Bytes allocated equals memory held." | A growing vector abandons its old buffers; the sum of allocations is larger than the live buffer. | Compare live snapshots × 256 with allocated bytes. |
| "`history_size()` counts moves." | It counts snapshots, including the initial board. | Check seed 1: 21 moves, 22 snapshots. |
| "Undo is free." | It is free in time and costs 256 bytes per move. | State both sides. |
| "The engine size did not change." | It went from 24 to 56 bytes. | Measure `sizeof(GameEngine<8, 8>)`. |
| "A value type cannot be large." | 256 bytes is copied per move, and that is the cost this lesson measures. | Count the copies: one per accepted move. |
| "Adding an accessor changes the layout." | A member function adds no data. | Measure the size after the change. |
| "Undo at index 0 succeeds." | It returns `false` and changes nothing. | Run the boundary loop and count. |
| "The generator state is in the snapshot." | It lives in `random_sequence.cpp` and advances on every call. | Read where the generator state is stored. |
| "Pure means no side effects, so printing is forbidden." | Printing is an effect; purity is about the value the function returns and the state it touches. | Separate the value contract from the I/O. |
| "Immutability means the board is never written." | The live board is written on every accepted move. | Point at the boundary: history, not the board. |

## Next step

`construction-19` owns data layout and locality. The history is a vector of 256-byte boards, and the
board is 256 bytes of `bool` where one bit per cell would fit in 8 bytes. Copying 32 times more
bytes than necessary is invisible in this lesson and visible in a profile of 10,000 games. The
question that follows is not "is the value immutable" but "how is the value laid out in memory".

See the [lesson catalog](../../../docs/guide/lesson-catalog.md) for the full path.

If either baseline was missing, do not start `construction-18`. The next contrast needs both sides.
