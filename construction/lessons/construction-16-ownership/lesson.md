# Ownership and resource lifetime (propiedad y ciclo de vida de recursos)

> [English](lesson.md) | [Español](lesson-es.md)

**Concept:** resource lifetime · **Time:** about 70 minutes · **Prerequisites:** construction-15 ·
**Status:** drafted
**Reading:** ~25 minutes
**Lesson role:** core

## Why this lesson exists

`construction-15` ended with three value members:

```cpp
Board<RowCount, ColumnCount> board;
Knight playerKnight;
Knight machineKnight;
```

Nobody asked who destroys them, because the answer was automatic: the engine dies, the members die,
in reverse order, and nothing was allocated. The class had no lifetime problem to solve.

This lesson asks the question the value members hid. Who owns a resource? Who frees it? What happens
when someone copies the owner? The baseline answers all three in the type:

```cpp
std::unique_ptr<Board<RowCount, ColumnCount>> board;
std::unique_ptr<Knight> playerKnight;
std::unique_ptr<Knight> machineKnight;
```

Copy construction and copy assignment are deleted. Move construction and move assignment are
defaulted. There is no raw `new` and no raw `delete` anywhere in the engine.

The sentence to keep:

> Ownership is the answer to "who destroys this, and when?". `unique_ptr` turns that answer into a
> type: one owner, destruction at the owner's death, copying refused at compile time.

Three honest notes before the comparison.

1. The source grows by 8 lines, from 598 across twelve files to 606 across twelve. Eleven of the
   twelve files are byte-identical to `construction-15`; only `game_engine.h` changed, from 292 to
   300. The diff is one include, one constructor, four special members, three data members, and
   `board.` becoming `board->` at ten call sites.
2. The engine shrinks from 336 bytes to 24, but the program does not shrink. The board and the two
   knights moved to the heap. Owned bytes go from 336 to 360: 24 bytes of pointers plus the same 336
   bytes of objects. Each engine now costs three allocations, and the linked text grows by 242
   bytes.
3. For these two types, the value members already had no leaks and no raw `new`. This lesson does
   not claim `unique_ptr` fixes a bug that existed. It claims something narrower: ownership becomes
   explicit, copying becomes a compile error, and the lifetime rule is written once instead of being
   implied by three declarations. The price is three allocations and one pointer indirection on
   every board access. Step 6 measures that price.

The comparison is not "smart pointers are better". It is more precise: this baseline buys a stated
owner and a compile-time refusal to copy, and it pays with allocation, indirection and a moved-from
state that `run()` cannot handle. This lesson measures both sides and names the state that the
defaulted move leaves behind.

## What you will be able to do

By the end of this lesson you can:

- explain ownership, lifetime and the special members;
- show unique_ptr ownership, deleted copy and available move.

## The concept: who destroys this, and when?

### Three questions, one answer

Every resource in a program raises the same three questions:

1. **Who allocates it?** Which line creates it, and how many times?
2. **Who frees it?** Which line destroys it, and does that line run on every path, including the
   error paths?
3. **What happens when the owner is copied?** Two owners, one resource, or two resources?

Value members answer all three implicitly. The engine is on the stack, so the board is on the stack.
The compiler writes the destructor, so nobody writes a `delete`. Copying the engine copies the
board, which is correct but silent.

`unique_ptr` answers the same three questions in the type system:

| Question | Value member | `unique_ptr` member |
|---|---|---|
| Who allocates? | the object's storage | `std::make_unique`, at the constructor |
| Who frees? | the compiler's destructor | the `unique_ptr` destructor, at the owner's death |
| Copy the owner? | allowed, copies the value | refused at compile time |
| Move the owner? | allowed, copies or moves the value | allowed, transfers the pointer |
| Cost per access | none | one indirection |
| Cost per object | none | one allocation |

The table is the whole lesson. Everything after this point measures one row or another.

### The four special members

A class has four operations that decide how it is copied and moved. The compiler writes all four
unless you say otherwise. This baseline says otherwise:

```cpp
GameEngine(const GameEngine &) = delete;
GameEngine &operator=(const GameEngine &) = delete;
GameEngine(GameEngine &&) = default;
GameEngine &operator=(GameEngine &&) = default;
```

Read the four lines as a contract:

| Member | Line | Meaning |
|---|---|---|
| Copy constructor | 46 | Two engines cannot own the same board. Refused. |
| Copy assignment | 47 | An existing engine cannot take another engine's board. Refused. |
| Move constructor | 48 | A new engine can take the pointers from a dying one. Allowed. |
| Move assignment | 49 | An existing engine can take the pointers from another. Allowed. |

Here is the subtle part, and the reason this lesson exists. Because `unique_ptr` is move-only, the
compiler would already have deleted the copy operations on its own. Lines 46 and 47 change nothing
about what compiles. What they change is where the error points and who is responsible for the rule.

Without lines 46–47, a copy attempt fails inside `std::unique_ptr`, with a message about a deleted
constructor in a standard header. With them, the message names `GameEngine` and line 46. Step 5
shows both messages side by side. Explicit deletion is documentation that the compiler enforces.

The move operations are the opposite case. The compiler would also write them, and it would write
them correctly: member-wise move of three `unique_ptr`s. Line 48 and 49 say `= default` to make the
decision visible. A reader does not have to know the rules of implicit special-member generation to
know that this class is movable.

### What `unique_ptr` does and does not do

What it does:

- Guarantees exactly one owner. Two `unique_ptr`s cannot point at the same object.
- Destroys the pointee when the owner is destroyed, including when the owner is a member of an
  object whose constructor throws partway through.
- Refuses to copy, at compile time, with no runtime check and no flag.
- Costs the same as a raw pointer in size: 8 bytes on this platform, measured in Step 3.
- Moves in constant time and does not throw. A move is three pointer assignments.

What it does not do:

- It does not remove `new` and `delete` from the program. It moves them into `std::make_unique` and
  `std::default_delete`. `game.o` still references `operator new` and `operator delete`; Step 4
  shows the symbols. The allocation did not disappear, the call site did.
- It does not make the pointee `const`. `unique_ptr<Knight>` still lets you call `set_position`.
- It does not make access free. Every `board->` is one load that `board.` did not need.
- It does not define what a moved-from engine can do. After a move, the pointers are null and
  `run()` dereferences them. Step 5 reproduces the crash.
- It does not make the class copyable in a deep sense. If you want a copy, you must write one, and
  then you own the decision about what a copy means.

The last two are the honest limits, and this lesson records them rather than hiding them.

### Why three allocations and not one

The engine has three owned objects: one board and two knights. Each `make_unique` is one allocation.
A single allocation for all three would be possible with one struct and one `unique_ptr` to it, and
that variant is worth naming: it would trade three allocations for one, and it would couple the
three lifetimes into one. This baseline keeps them separate because the board and the two knights
have independent identities, and because the lesson is about the ownership rule, not about
allocation count. Step 6 records the count as a cost, not as a defect.

## Before you start

- [`construction-15`](../construction-15-generic-classes/lesson.md) — the baseline this lesson
  starts from. Its `game_engine.h` is the left side of the structural diff in Step 6.
- [`construction-11`](../construction-11-encapsulation/lesson.md) — the class boundary. `Position`
  and `Knight` keep private state and public operations. Ownership only makes sense once a class has
  a boundary to own behind.
- [`construction-09`](../construction-09-abstract-data-type/lesson.md) — representation
  independence. The board's public operations did not change when its storage moved to the heap.
- A C++20 compiler (`g++` or `clang++`). `cmake` is optional; `run.sh` falls back to a direct
  compile when CMake is absent.

You do not need to know how `unique_ptr` is implemented. You need to read one header, answer who
allocates and who frees, and show that the compiler refuses to copy the owner.

### What you need

- The ownership baseline:
  [`code/main.cpp`](code/main.cpp), [`code/game.h`](code/game.h),
  [`code/game.cpp`](code/game.cpp), [`code/game_engine.h`](code/game_engine.h),
  [`code/board.h`](code/board.h), [`code/move_generator.h`](code/move_generator.h),
  [`code/knight.h`](code/knight.h), [`code/knight.cpp`](code/knight.cpp),
  [`code/position.h`](code/position.h), [`code/position.cpp`](code/position.cpp),
  [`code/random_sequence.h`](code/random_sequence.h),
  [`code/random_sequence.cpp`](code/random_sequence.cpp).
- The previous baseline, for the diff:
  [`code/game_engine.h`](../construction-15-generic-classes/code/game_engine.h) and its eleven
  companions in
  [`../construction-15-generic-classes/code/`](../construction-15-generic-classes/code/).
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
test -f ../construction-15-generic-classes/code/game_engine.h &&
test -f ../../../common/traces/seed-0001.txt
```

If a file is missing, the comparison has no second side. Record the missing file in `EVIDENCE.md`
§11 and return when it is available. A count against a file you have not read is not evidence.

The 8×8 baseline reproduces all 20 traces in `common/traces/` byte for byte, before you change
anything. That is the behavioral contract, and this lesson does not touch it.

## Step 1 — Inventory ownership (~12 min)

Read the header first. The shape is the lesson:

```bash
grep -n 'unique_ptr\|make_unique\|GameEngine(const\|GameEngine(GameEngine\|operator=' \
  code/game_engine.h
```

```text
40:        : board(std::make_unique<Board<RowCount, ColumnCount>>()),
41:          playerKnight(std::make_unique<Knight>("player")),
42:          machineKnight(std::make_unique<Knight>("machine"))
46:    GameEngine(const GameEngine &) = delete;
47:    GameEngine &operator=(const GameEngine &) = delete;
48:    GameEngine(GameEngine &&) = default;
49:    GameEngine &operator=(GameEngine &&) = default;
71:    std::unique_ptr<Board<RowCount, ColumnCount>> board;
72:    std::unique_ptr<Knight> playerKnight;
73:    std::unique_ptr<Knight> machineKnight;
```

Now the absence test. This is the grep that the acceptance names:

```bash
grep -n '\bnew\b\|\bdelete\b\|malloc\|free(' code/*.h code/*.cpp
```

```text
code/game_engine.h:46:    GameEngine(const GameEngine &) = delete;
code/game_engine.h:47:    GameEngine &operator=(const GameEngine &) = delete;
```

Two matches, and neither one frees memory. Both are the keyword `delete` used to forbid an
operation. That is a coincidence of vocabulary worth noticing: the word that removes a function is
the same word that frees an allocation, and this file uses only the first meaning.

Fill the line map into `EVIDENCE.md` §2.2 from the source, not from this table.

`game_engine.h`, ownership-relevant ranges:

| Lines | Contents |
|---|---|
| 1–13 | Includes. Line 12 adds `<memory>`. |
| 35–36 | The template header and `class GameEngine`. |
| 39–44 | Constructor: three `make_unique` calls in declaration order. |
| 46–47 | Copy constructor and copy assignment, deleted. |
| 48–49 | Move constructor and move assignment, defaulted. |
| 51–68 | `run()`. Line 54 calls `board->reset()`. |
| 71–73 | The three owning members. |
| 75–216 | Helpers that read through `board->` and the two knights. |
| 218–284 | `playTurn`. |
| 286–299 | `playGame`. |

The rest of the file is `construction-15` with `board.` replaced by `board->` and `&knight` replaced
by `knight.get()`.

Counts for this baseline. The counting rule is stated so you can reproduce it:

- Headers: 7 (`board.h`, `game.h`, `game_engine.h`, `knight.h`, `move_generator.h`, `position.h`,
  `random_sequence.h`).
- Translation units: 5 (`main.cpp`, `game.cpp`, `knight.cpp`, `position.cpp`,
  `random_sequence.cpp`).
- Complete class definitions: 5 (`Board`, `GameEngine`, `Knight`, `Position`, `MoveCandidate`).
- Function bodies written by hand: 41, the same 41 as `construction-15`.
- Special members declared with `= delete` or `= default`: 4. They are declarations, not
  hand-written bodies, so they are counted separately.
- Private data declarations in headers: 10, counting the three owning members, the two `Position`
  fields, the two `Knight` fields, `Board::visited`, and the two `Board` maximums.
- Public declarations: 31, which is 27 from `construction-15` plus the four special members.
- Source lines: 606, which is 598 plus 8.

Then answer in `EVIDENCE.md` §2:

1. Which three members own resources, and on which lines?
2. Where are the copy operations deleted, and which lines?
3. Where are the move operations defaulted, and which lines?
4. How many `make_unique` calls are there, and where?
5. Which header provides `unique_ptr` and `make_unique`?
6. How many matches does the raw `new`/`delete` grep return, and what are they?
7. Does the initialization order at lines 40–42 match the declaration order at lines 71–73?
8. What does line 54 call, and what did it call in `construction-15`?
9. Which two expressions replaced `&playerKnight` and `&machineKnight`?
10. How many of the twelve files differ from `construction-15`?

Question 7 has a precise answer: yes, board, player, machine, in that order. If it did not, GCC
would warn with `-Wreorder`, and `-Werror` would stop the build. The order matters for a reason:
members are destroyed in reverse declaration order, so the board outlives both knights. Nothing here
depends on that, but a reader should know which way the rule runs.

Question 9 has a precise answer: `playerKnight.get()` and `machineKnight.get()`. `.get()` returns
the raw pointer without transferring ownership. The engine still owns both knights; `playTurn` only
borrows them for the duration of a turn.

Question 10 has a precise answer: one. Run `diff -rq` on the two `code/` directories and count.

## Step 2 — Predict before you run (~10 min)

### 2.1 The size table

Write these predictions before running anything. Do not look at the answers in Step 3 first.

| Question | My prediction |
|---|---|
| `sizeof(GameEngine<8, 8>)` in `construction-15` | |
| `sizeof(GameEngine<8, 8>)` here | |
| `sizeof(Board<8, 8>)` | |
| `sizeof(std::unique_ptr<Board<8, 8>>)` | |
| `sizeof(Knight)` | |
| Total owned bytes: engine plus board plus two knights | |
| Heap allocations per engine | |
| Heap frees per engine, after destruction | |
| Linked binary text bytes, before and after | |
| Linked binary data bytes, before and after | |

The row to take seriously is the fourth. A `unique_ptr` is one pointer. If you predicted 256 or 264,
you predicted the pointee. The pointee is not inside the engine any more; that is the whole change.

The row that is easy to get wrong is the sixth. The engine shrinks by 312 bytes, but the objects it
owns do not shrink. Add the three numbers and compare with 336.

### 2.2 The allocation table

| Question | My prediction |
|---|---|
| `make_unique` calls in the constructor | |
| Allocations counted after constructing one engine | |
| Frees counted after that engine dies | |
| Allocations for twenty engines | |
| Frees after those twenty die | |
| Does the count depend on the seed? | |
| Does the count depend on the number of turns? | |

The last two rows are the structural test. The allocation happens in the constructor, not in the
game loop. If the count changed with the seed, allocation would be inside `playTurn`, and that would
be a different design.

### 2.3 The change test predictions

The change test is: ask the compiler whether this engine can be copied and whether it can be moved.

| Question | My prediction |
|---|---|
| `std::is_copy_constructible_v<GameEngine<8, 8>>` | |
| `std::is_copy_assignable_v<GameEngine<8, 8>>` | |
| `std::is_move_constructible_v<GameEngine<8, 8>>` | |
| `std::is_move_assignable_v<GameEngine<8, 8>>` | |
| A program that copies an engine compiles? | |
| The error names which line? | |
| A program that moves an engine, then runs the moved-to object, matches seed 1? | |
| A program that moves an engine, then runs the moved-from object, exits how? | |
| Seeds that match, of 20, after the change | |

Predict the last row as 20. The change is a type-level rule; it does not touch the game.

Predict the second-to-last row before you run it. The moved-from engine has three null pointers, and
`run()` calls `board->reset()`. This is not a subtle prediction once you write down what a move does
to the source object.

## Step 3 — Run the baseline (~10 min)

First the traces, unmodified:

```bash
mkdir -p /tmp/ownership-test
./run.sh --seed 1 --name Learner > /tmp/ownership-test/baseline-0001.txt
diff -u ../../../common/traces/seed-0001.txt /tmp/ownership-test/baseline-0001.txt
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
  -o build/ownership-baseline code/main.cpp code/game.cpp code/knight.cpp \
  code/position.cpp code/random_sequence.cpp
```

Now the sizes. The probe goes in the scratch directory, never in the tracked files:

```bash
cat > /tmp/ownership-test/sizes.cpp <<'EOF'
#include "game_engine.h"

#include <cstdio>

int main()
{
    std::printf("sizeof(Board<8,8>)=%zu\n", sizeof(Board<8, 8>));
    std::printf("sizeof(GameEngine<8,8>)=%zu\n", sizeof(GameEngine<8, 8>));
    std::printf("sizeof(unique_ptr<Board<8,8>>)=%zu\n",
                sizeof(std::unique_ptr<Board<8, 8>>));
    std::printf("sizeof(Knight)=%zu\n", sizeof(Knight));
    std::printf("sizeof(Position)=%zu\n", sizeof(Position));
    return 0;
}
EOF
g++ -std=c++20 -Wall -Wextra -Werror -O2 -Icode \
  -o /tmp/ownership-test/sizes /tmp/ownership-test/sizes.cpp \
  code/knight.cpp code/position.cpp
/tmp/ownership-test/sizes
```

The output on this platform:

```text
sizeof(Board<8,8>)=256
sizeof(GameEngine<8,8>)=24
sizeof(unique_ptr<Board<8,8>>)=8
sizeof(Knight)=40
sizeof(Position)=8
```

Twenty-four bytes, three pointers, and no board inside. The board is 256 bytes somewhere else.

Now the allocation counter. This probe replaces the global `operator new` and `operator delete` with
counting versions, so it works without a leak detector:

```bash
cat > /tmp/ownership-test/count.cpp <<'EOF'
#include "game_engine.h"

#include <cstdio>
#include <cstdlib>
#include <new>

namespace
{
long allocationCount = 0;
long deallocationCount = 0;
}

void *operator new(std::size_t size)
{
    allocationCount += 1;
    if (void *memory = std::malloc(size))
    {
        return memory;
    }
    throw std::bad_alloc();
}

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wmismatched-new-delete"
void operator delete(void *memory) noexcept
{
    if (memory != nullptr)
    {
        deallocationCount += 1;
    }
    std::free(memory);
}

void operator delete(void *memory, std::size_t) noexcept
{
    operator delete(memory);
}
#pragma GCC diagnostic pop

int main()
{
    {
        GameEngine<8, 8> engine;
        std::printf("after construction: allocations=%ld frees=%ld\n",
                    allocationCount, deallocationCount);
    }
    std::printf("after destruction: allocations=%ld frees=%ld\n",
                allocationCount, deallocationCount);
    return 0;
}
EOF
g++ -std=c++20 -Wall -Wextra -Werror -O2 -Icode \
  -o /tmp/ownership-test/count /tmp/ownership-test/count.cpp \
  code/knight.cpp code/position.cpp code/random_sequence.cpp
/tmp/ownership-test/count
```

```text
after construction: allocations=3 frees=0
after destruction: allocations=3 frees=3
```

Three and three. Change `GameEngine<8, 8> engine;` to `GameEngine<8, 8> engines[20];` and the same
probe prints `60` and `60`. The count scales with the number of engines, not with the game.

The `#pragma` is there because GCC 16 warns when a replaced `operator new` returns `malloc` memory
that a replaced `operator delete` frees. The warning is about the probe, not about the engine. If
your compiler does not emit it, the pragma is harmless.

Now the symbol counts, to see whether the move operations cost anything:

```bash
cat > /tmp/ownership-test/sym.cpp <<'EOF'
#include "game_engine.h"

int main()
{
    GameEngine<8, 8> engine;
    return engine.run(1, "Learner");
}
EOF
g++ -std=c++20 -O0 -Icode -c /tmp/ownership-test/sym.cpp \
  -o /tmp/ownership-test/sym-O0.o
g++ -std=c++20 -O2 -Icode -c /tmp/ownership-test/sym.cpp \
  -o /tmp/ownership-test/sym-O2.o
nm -C /tmp/ownership-test/sym-O0.o | grep -c 'GameEngine<8, 8>'
nm -C /tmp/ownership-test/sym-O2.o | grep -c 'GameEngine<8, 8>'
```

```text
24
1
```

The same 24 and 1 as `construction-15`. Now look for the move operations by name:

```bash
nm -C /tmp/ownership-test/sym-O0.o | grep 'GameEngine<8, 8>::GameEngine' | sort | uniq -c
```

```text
      1 n GameEngine<8, 8>::GameEngine()
      2 W GameEngine<8, 8>::GameEngine()
```

Three constructor symbols: two weak (the default constructor, emitted once as a weak definition and
once as a local alias) and one local. No symbol is named `GameEngine(GameEngine&&)`. The defaulted
move constructor is defined in the class body, and the compiler inlines it at the one call site that
needs it. A move of three pointers is not worth a function call.

The destructor does get a symbol:

```bash
nm -C /tmp/ownership-test/sym-O0.o | grep '~GameEngine'
```

```text
W GameEngine<8, 8>::~GameEngine()
```

That symbol is where the three frees live. The engine did not write a destructor; the compiler wrote
one that destroys three `unique_ptr`s, and each of those calls `delete` on its pointee.

If the first diff is not empty, stop. That is a different baseline from the one this lesson
describes; record it in `EVIDENCE.md` §11 and do not adjust the prediction to match.

## Step 4 — Compare: what the `unique_ptr` proves (~10 min)

Open `game_engine.h` and `game.cpp`. Five facts, read against the measurements.

1. **The engine no longer contains the board.** `sizeof(GameEngine<8, 8>)` is 24, down from 336.
   Three pointers, eight bytes each. The board is 256 bytes on the heap.
2. **The objects did not shrink.** `sizeof(Board<8, 8>)` is still 256 and `sizeof(Knight)` is still
   40. Owned bytes are 24 + 256 + 80 = 360, up from 336. The 24 extra bytes are the three pointers.
   You traded 312 bytes of stack for 360 bytes of heap plus 24 bytes of bookkeeping.
3. **The allocation is real.** `game.o` has undefined references to `operator new` and
   `operator delete`. The engine source has no `new` and no `delete`. Both statements are true at
   once, and a reader who checks only one of them will draw the wrong conclusion.
4. **The allocation count is fixed by the constructor.** Three per engine, three frees at death, and
   twenty engines give sixty and sixty. The game loop allocates nothing.
5. **The move operations cost no symbol.** The defaulted move constructor is inlined. The destructor
   does get a symbol, and that is where the frees are.

### 4.1 The relocation that proves the point

```bash
g++ -std=c++20 -O2 -Icode -c code/game.cpp -o /tmp/ownership-test/game16.o
nm -C /tmp/ownership-test/game16.o | grep -i 'operator new\|operator delete'
```

```text
                 U operator delete(void*, unsigned long)
                 U operator new(unsigned long)
```

`U` means undefined: this translation unit calls them and does not define them. The linker supplies
them from the C++ runtime. `std::make_unique` calls `operator new`, and `std::default_delete` calls
`operator delete`. The ownership rule moved the call into a library type; it did not remove it.

Compare the text size:

```bash
g++ -std=c++20 -O2 -I../construction-15-generic-classes/code \
  -c ../construction-15-generic-classes/code/game.cpp \
  -o /tmp/ownership-test/game15.o
size /tmp/ownership-test/game15.o /tmp/ownership-test/game16.o
```

```text
   text	   data	    bss	    dec	    hex	filename
   4392	      0	      0	   4392	   1128	/tmp/ownership-test/game15.o
   4638	      0	      0	   4638	   121e	/tmp/ownership-test/game16.o
```

`game.o` grows by 246 bytes. The linked binary grows by 242:

```text
   text	   data	    bss	    dec	    hex	filename
   9410	    692	     24	  10126	   278e	construction-15 binary
   9652	    692	     24	  10368	   2880	construction-16 binary
```

Data and BSS do not move. The board left the stack, but the stack is not in the binary; the heap is
not either. The text growth is the allocation, the destructor and the extra load per access.

### 4.2 What did not change

- The transcript. All 20 traces match, byte for byte. Ownership is not observable in the output.
- The rules. Move order, capture, trap, draw, and the 200-move limit are untouched.
- The board's interface. `reset`, `was_visited`, `mark_visited`, `row_count`, `column_count` are the
  same calls; only the syntax to reach them changed from `.` to `->`.
- The piece contract. `PositionedPiece` and its `static_assert` are unchanged.
- The dimension constraints. `Board` still checks 1..16 at `board.h:72–73`.
- Indirect calls in `game.o`. Zero before, zero after. `unique_ptr` adds no virtual dispatch; the
  destructor is not virtual, and `default_delete` is resolved at compile time.

The last point is worth stating because it is a common assumption. Smart pointers do not imply a
vtable. They are a template with a pointer inside, and the delete is a direct call.

## Step 5 — Run the change test (~12 min)

The change test is the one the acceptance names: ask the compiler what it will allow.

### 5.1 The compile-time probe

```bash
cat > /tmp/ownership-test/probe.cpp <<'EOF'
#include "game_engine.h"

#include <type_traits>

static_assert(!std::is_copy_constructible_v<GameEngine<8, 8>>);
static_assert(!std::is_copy_assignable_v<GameEngine<8, 8>>);
static_assert(std::is_move_constructible_v<GameEngine<8, 8>>);
static_assert(std::is_move_assignable_v<GameEngine<8, 8>>);
static_assert(std::is_destructible_v<GameEngine<8, 8>>);

int main()
{
    return 0;
}
EOF
g++ -std=c++20 -Wall -Wextra -Werror -O2 -Icode -c \
  /tmp/ownership-test/probe.cpp -o /tmp/ownership-test/probe.o
```

No output and a `.o` file: the five assertions hold. Note that `-c` is enough. The probe needs no
`knight.cpp`, no `position.cpp`, no `random_sequence.cpp`, because it never runs the engine. The
question is answered by the type system alone, which is the point.

The three assertions the acceptance names are the first three. The fourth and fifth are there to
show that move assignment and destruction are also available; without them, "movable" would be a
claim about half the operations.

### 5.2 The copy failures

Now ask for the copy the class refuses:

```bash
cat > /tmp/ownership-test/copy_fail.cpp <<'EOF'
#include "game_engine.h"

int main()
{
    GameEngine<8, 8> original;
    GameEngine<8, 8> copy(original);
    return 0;
}
EOF
g++ -std=c++20 -Wall -Wextra -Werror -O2 -Icode -c \
  /tmp/ownership-test/copy_fail.cpp -o /tmp/ownership-test/copy_fail.o
```

The compiler reports, at the `static_assert`-free line that tries the copy:

```text
code/game_engine.h:46:5: error: use of deleted function
  'GameEngine<RowCount, ColumnCount>::GameEngine(
     const GameEngine<RowCount, ColumnCount>&) [with int RowCount = 8;
     int ColumnCount = 8]'
   46 |     GameEngine(const GameEngine &) = delete;
      |     ^~~~~~~~~~
```

The same for assignment:

```bash
cat > /tmp/ownership-test/copy_assign.cpp <<'EOF'
#include "game_engine.h"

int main()
{
    GameEngine<8, 8> original;
    GameEngine<8, 8> other;
    other = original;
    return 0;
}
EOF
g++ -std=c++20 -Wall -Wextra -Werror -O2 -Icode -c \
  /tmp/ownership-test/copy_assign.cpp -o /tmp/ownership-test/copy_assign.o
```

```text
code/game_engine.h:47:17: error: use of deleted function
  'GameEngine<RowCount, ColumnCount>&
   GameEngine<RowCount, ColumnCount>::operator=(
     const GameEngine<RowCount, ColumnCount>&) [with int RowCount = 8;
     int ColumnCount = 8]'
   47 |     GameEngine &operator=(const GameEngine &) = delete;
      |                 ^~~~~~~~
```

Both errors point at the class, at the line that states the rule. That is the difference between an
implicit deletion and an explicit one. To see the implicit version, comment out lines 46 and 47 in a
scratch copy of the header and repeat the build. The error moves into `std::unique_ptr` and names a
deleted constructor inside a standard header. The rule did not change; only its address did.

Do that experiment in `/tmp`, never in `code/`. The tracked header stays as it is.

### 5.3 The move that works

```bash
cat > /tmp/ownership-test/move_demo.cpp <<'EOF'
#include "game_engine.h"

int main()
{
    GameEngine<8, 8> original;
    GameEngine<8, 8> moved(std::move(original));
    return moved.run(1, "Learner");
}
EOF
g++ -std=c++20 -Wall -Wextra -Werror -O2 -Icode \
  -o /tmp/ownership-test/move_demo /tmp/ownership-test/move_demo.cpp \
  code/knight.cpp code/position.cpp code/random_sequence.cpp
/tmp/ownership-test/move_demo > /tmp/ownership-test/moved-0001.txt
cmp /tmp/ownership-test/moved-0001.txt ../../../common/traces/seed-0001.txt
```

Empty `cmp`: the moved-to engine plays the same game. Three pointers changed owner, and the behavior
did not. That is what a move is for.

### 5.4 The moved-from state

Now the other side of the same move:

```bash
cat > /tmp/ownership-test/moved_from.cpp <<'EOF'
#include "game_engine.h"

int main()
{
    GameEngine<8, 8> original;
    GameEngine<8, 8> moved(std::move(original));
    return original.run(1, "Learner");
}
EOF
g++ -std=c++20 -Wall -Wextra -Werror -O2 -Icode \
  -o /tmp/ownership-test/moved_from /tmp/ownership-test/moved_from.cpp \
  code/knight.cpp code/position.cpp code/random_sequence.cpp
/tmp/ownership-test/moved_from > /tmp/ownership-test/moved_from.txt
echo "exit=$?"
```

On this platform:

```text
Segmentation fault (core dumped)
exit=139
```

Read the reason, not the number. A moved-from `unique_ptr` is null. `run()` calls `board->reset()`,
which dereferences a null pointer. The C++ rule says a moved-from object is *valid but unspecified*:
you may destroy it and you may assign to it, and that is all. Calling `run()` on it is outside the
contract, and the contract violation is undefined behavior, not an error code.

This is the honest gap in the design, and it has a name: `run()` assumes the engine is in the
constructed state, and nothing in the type enforces that. Three repairs are possible, and this
lesson does not pick one:

1. Make `run()` check the pointers and return an error. That is an error contract, which is
   `construction-22`.
2. Make `run()` a free function that takes the engine by reference and is called only on a
   constructed object. That is a design change, not a lifetime change.
3. Do not move engines at all: construct them where they are used, as `game.cpp` does at line 8.

The baseline uses the third option by accident: `game.cpp` never moves an engine. The move
operations exist because the acceptance requires them, and because a class that owns resources
should state what copying and moving mean. A class that is movable and not runnable after a move is
a normal C++ class; the lesson is that "movable" and "usable after a move" are different claims.

### 5.5 The leak evidence when no detector runs

A leak detector is the usual evidence for this lesson, and on this machine neither one runs:

```bash
command -v valgrind
g++ -fsanitize=address /tmp/ownership-test/asan.cpp -o /tmp/ownership-test/asan
```

```text
valgrind: absent
/usr/bin/ld.bfd: cannot find /usr/lib64/libasan.so.8.0.0
```

So the record has two kinds of evidence, and it must keep them apart.

**Structural evidence.** Read it, do not run it:

- Three owning members, all `std::unique_ptr` (`game_engine.h:71–73`).
- Three `make_unique` calls, one per member, all in the constructor (`game_engine.h:40–42`).
- No raw `new`, no raw `delete`, no `malloc`, no `free` in any of the twelve files. The only
  `delete` tokens are the two deleted copy operations.
- Copy deleted, so two engines cannot share one pointer.
- Move defaulted, so ownership transfers instead of duplicating.
- No hand-written destructor, so there is no path on which a free can be forgotten or repeated.

**Empirical evidence.** Run it:

- The counting probe prints 3 allocations and 3 frees for one engine, 60 and 60 for twenty.
- `game.o` references `operator new` and `operator delete`, so the allocations are real and the
  matching calls exist.
- The destructor symbol exists, so there is a place where the frees run.

What this evidence does not prove: that the frees happen exactly once on every path, including a
path where the constructor throws. The counting probe covers the normal path. The exception path is
argued structurally, not measured, because no exception occurs in this program and no sanitizer is
available to force one.

Record that gap in `EVIDENCE.md` §5.5. Do not write "no leaks, verified by ASan" when ASan did not
run. Write "no leaks on the normal path, counted; structural argument for the exception path; no
sanitizer available on this machine". That sentence is worth more than a false checkmark.

If you do have a detector, run it and record its exact output instead. The lesson does not depend on
which tool produced the evidence; it depends on the record saying which tool did.

## Step 6 — Compare with `construction-15` (~8 min)

The structural diff is where the ownership decision shows its cost.

| Metric | construction-15 | construction-16 | Difference |
|---|---|---|---|
| Files | 12 | 12 | 0 |
| Headers | 7 | 7 | 0 |
| Translation units | 5 | 5 | 0 |
| Source lines | 598 | 606 | +8 |
| `game_engine.h` | 292 | 300 | +8 |
| Other 11 files | — | byte-identical | 0 |
| Complete classes | 5 | 5 | 0 |
| Hand-written function bodies | 41 | 41 | 0 |
| Special members declared | 0 | 4 | +4 |
| Public declarations | 27 | 31 | +4 |
| `sizeof(GameEngine<8, 8>)` | 336 | 24 | -312 |
| Heap allocations per engine | 0 | 3 | +3 |
| Owned bytes, total | 336 | 360 | +24 |
| `game.o` text | 4,392 | 4,638 | +246 |
| Binary text / data | 9,410 / 692 | 9,652 / 692 | +242 / 0 |
| Indirect calls in `game.o` | 0 | 0 | 0 |
| Traces | 20 of 20 | 20 of 20 | 0 |

### 6.1 The exact diff

Only one file changed:

```bash
diff -u ../construction-15-generic-classes/code/game_engine.h code/game_engine.h
```

The diff has nine hunks: 27 lines added and 19 removed, for a net growth of eight. In words:

| Kind of change | Count |
|---|---|
| `#include <memory>` added | 1 |
| Constructor lines rewritten | 3 |
| Special members added: two deleted, two defaulted | 4 |
| Data members rewritten as `unique_ptr` | 3 |
| `board.` accesses rewritten as `board->` | 10 |
| Knight member accesses rewritten as `->` | 6 |
| `&knight` expressions rewritten as `knight.get()` | 4 |

That is the whole ownership change. The turn loop, the rules and the transcript format are
untouched.

### 6.2 What it buys

- One stated owner. `board` owns the board; nothing else can.
- Copy refused at compile time, with the error at the class, not in a standard header.
- Move allowed, so the engine can be returned from a function or stored in a container.
- Exception safety in the constructor. If the third `make_unique` throws, the two
  already-constructed `unique_ptr` members are destroyed and their pointees freed. With three raw
  pointers and a hand-written destructor, the same constructor leaks two allocations unless you add
  a `try`/`catch`. This is a real advantage, argued from the language rules, not measured here.
- No hand-written destructor, so no path that forgets a free.
- A smaller engine: 24 bytes instead of 336, which matters if you store many engines.

### 6.3 What it costs

- Three allocations per engine. At 20 engines, 60 allocations; at 10,000 engines, 30,000. That is a
  real cost for the data-oriented lesson in `construction-19`.
- One indirection per board access. Ten call sites in `game_engine.h` now load a pointer first.
- 242 more bytes of text, 246 in `game.o`.
- A moved-from state that `run()` cannot handle. The class is movable, but not usable after a move.
- No deep copy. If you need two engines with the same position, this class cannot give you one. You
  would write a `clone()` or a copy constructor that allocates new pointees, and then you own the
  decision about what "the same position" means.
- The engine is no longer trivially copyable, which changes what containers and algorithms accept.

### 6.4 The gap: ownership is explicit, but the board is not shared

`GameEngine` owns the board, and `playTurn` borrows two knights through `.get()`. That is a clean
ownership story inside one engine. It says nothing about two engines, two threads, or one engine
handed to two functions.

The baseline also keeps the random sequence in a file-scope variable (`random_sequence.cpp:7`).
Ownership of the board and the knights is explicit; ownership of the generator state is not. `run()`
resets it with `random_set_seed(seed)` at line 53, which is why two engines run in sequence produce
the same transcript. Two engines run at the same time would share one generator, and the transcripts
would depend on the interleaving. That is the concurrency problem, and `construction-21` owns it.
This lesson names it and stops.

The precise acceptance sentence is narrower than "the program has no leaks":

> The learner can show that the engine owns its resources through `unique_ptr`, that copying is
> explicitly deleted, that moving is available, and that the 20 traces still pass.

Four claims, four pieces of evidence: the three members, lines 46–47, lines 48–49, and the trace
loop. Nothing about sharing, threading or deep copies.

## Step 7 — Explain (~6 min)

Write `EVIDENCE.md` §8. Two parts.

**First, one sentence.** Say what changed between the two engines and what the change decides,
without using the words *smart pointer*, *ownership*, *lifetime*, *RAII*, *destructor*, *better*,
*worse*, *clean*, *safe* or *modern*. If your sentence needs one of those, you have named the
mechanism rather than the decision.

**Second, one paragraph.** Answer:

- What the three members decide: one owner each, freed at the engine's death.
- What lines 46–47 decide: two engines cannot share a board, and the refusal is a compile error.
- What lines 48–49 decide: ownership can transfer, in constant time, without copying the board.
- What the `new`/`delete` grep proves: no call site in the engine allocates or frees by hand.
- What the `nm` output proves: the allocation still happens, through `operator new`.
- What the counting probe proves: three allocations and three frees per engine, on the normal path.
- What the traces prove: the ownership change is invisible in the transcript.
- What the moved-from crash proves: movable is not the same as usable after a move.
- What the cost is: three allocations, one indirection, 242 bytes of text, 24 extra bytes of
  bookkeeping.
- What the value-member version already had: no leaks, no raw `new`, and a copy that worked.
- What the next lesson owns: algorithmic abstraction, `construction-17`.
- What the concurrency lesson owns: the shared generator state, `construction-21`.

A strong answer keeps four things apart: **who owns** (the three `unique_ptr`s), **what the compiler
refuses** (copy), **what the compiler allows** (move), and **what a run can see** (the transcript,
unchanged). A weak answer says "smart pointers manage memory automatically", which collapses the
first three into a slogan and ignores the fourth.

## Step 8 — Acceptance check and record (~2 min)

The acceptance criterion for this lesson is: **show that the engine owns its resources through
`unique_ptr`, that copying is explicitly deleted, that moving is available, and that the 20 traces
still pass.**

Check your record against it:

- [ ] I located the three owning members and their lines.
- [ ] I located the two deleted copy operations and the two defaulted move operations.
- [ ] I confirmed that the raw `new`/`delete` grep returns only the two `= delete` lines.
- [ ] I confirmed that `game.o` still references `operator new` and `operator delete`.
- [ ] I compiled the probe with the three `static_assert`s and it built.
- [ ] I reproduced the copy-construction error and read the line it names.
- [ ] I reproduced the copy-assignment error and read the line it names.
- [ ] I moved an engine and the moved-to engine reproduced seed 1.
- [ ] I ran the moved-from engine and recorded what happened.
- [ ] I counted three allocations and three frees for one engine.
- [ ] I counted sixty and sixty for twenty engines.
- [ ] I measured `sizeof(GameEngine<8, 8>)` at 24 and `sizeof(Board<8, 8>)` at 256.
- [ ] I reproduced all 20 traces on the unmodified baseline.
- [ ] I recorded that no leak detector was available, and which evidence I used instead.
- [ ] I did not edit any tracked code file.
- [ ] I completed `EVIDENCE.md` §1–§10 and the self-check in §12.

The criterion is not "the code uses `unique_ptr`". It is "you can say who owns what, what the
compiler refuses, what it allows, and what the run shows".

## Self-check (binary)

- [ ] I read `game_engine.h` and located the three owners, the four special members and the
      `make_unique` calls by line.
- [ ] I compared the ownership baseline with `construction-15` and named the eight added lines.
- [ ] I predicted the size table, the allocation table and the change test before running.
- [ ] I reproduced all 20 traces on the unmodified baseline.
- [ ] I compiled the probe with `static_assert(!std::is_copy_constructible_v<GameEngine<8, 8>>)`,
      `static_assert(!std::is_copy_assignable_v<GameEngine<8, 8>>)` and
      `static_assert(std::is_move_constructible_v<GameEngine<8, 8>>)`.
- [ ] I reproduced both copy errors and read the line each one names.
- [ ] I moved an engine and matched seed 1 with the moved-to object.
- [ ] I ran the moved-from engine and recorded the result.
- [ ] I counted allocations and frees for one engine and for twenty.
- [ ] I measured the engine at 24 bytes and the board at 256.
- [ ] I recorded that no leak detector was available and used structural plus counted evidence.
- [ ] I named what the ownership change does not cover: the shared generator, and the moved-from
      state.
- [ ] I completed `EVIDENCE.md`.

These are acceptance criteria. If a box is
unchecked, write the next smallest action in `EVIDENCE.md` §10.

## Misconceptions this lesson refutes

| Misconception | Why it fails | What to do instead |
|---|---|---|
| "`unique_ptr` removes `new` and `delete` from the program." | It moves them into `make_unique` and `default_delete`; `game.o` still calls them. | `nm -C game.o \| grep 'operator new'`. |
| "The engine has no allocation because `grep new` is empty." | The grep searches the source, not the object file. | Search both, and say which one you searched. |
| "Deleting the copy operations is what makes the class non-copyable." | `unique_ptr` already does that; the lines change the diagnostic, not the rule. | Comment them out in a scratch header and compare the errors. |
| "`= default` on the move operations does nothing." | It states the decision and keeps the operations available when a member would otherwise suppress them. | Remove one and check `is_move_constructible_v`. |
| "A moved-from engine is still usable." | Its pointers are null; `run()` dereferences them. | Run it and record the crash; then name the repair. |
| "`unique_ptr` adds a vtable." | It is a template with one pointer; deletion is a direct call. | Count indirect calls in `game.o`; there are none. |
| "The engine is smaller, so the program uses less memory." | The engine is 24 bytes, but it owns 336 bytes on the heap plus 24 bytes of pointers. | Add the owned bytes and compare with 336. |
| "The board is copied into the `unique_ptr`." | The pointer points at a heap object; the board is not inside the engine. | Measure `sizeof(GameEngine)` and `sizeof(Board)`. |
| "No leaks means no bugs." | The moved-from crash is a bug that a leak detector would not report. | Test the moved-from path separately. |
| "Ownership solves sharing." | It forbids sharing inside one engine and says nothing about two engines or two threads. | Read the generator at `random_sequence.cpp:7`. |
| "`unique_ptr` makes access free." | Every `board->` is one load that `board.` did not need. | Count the ten call sites and the 242 extra text bytes. |
| "A copy would be trivial to add back." | A deep copy needs a decision about what to copy and who owns the result. | Write down the copy constructor you would need; note what it allocates. |
| "The 20 traces prove the ownership is correct." | They prove the behavior is unchanged; they say nothing about frees. | Use the counting probe and the structural argument. |
| "`std::move` moves something." | It is a cast to an rvalue reference; the move constructor does the work. | Read the move constructor: three pointer assignments. |
| "Value members were leaking." | They never allocated, so there was nothing to leak. | Say what the change buys, not what it fixes. |

## Next step

`construction-17` owns algorithmic abstraction. The engine still writes its loops by hand: the move
order, the candidate scan, the board reset. `std::array`, `find_if` and ranges express the same
traversals without the manual index. The ownership question is settled here and does not reopen
there; the loop bodies are the next thing to change.

Two threads stay open and belong to later lessons. The generator state at
`random_sequence.cpp:7` is still global, so two engines share it, and `construction-21` owns that
problem. The moved-from engine is still runnable in the type system and not runnable in fact, and
`construction-22` owns the error contract that would make it a value instead of a crash.

See the [lesson catalog](../../../docs/guide/lesson-catalog.md) for the full path.

If either baseline was missing, do not start `construction-16`. The next contrast needs both sides.
