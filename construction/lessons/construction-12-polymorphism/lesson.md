# Polymorphism (polimorfismo)

> [English](lesson.md) | [Español](lesson-es.md)

**Concept:** late binding (ligadura dinámica) · **Time:** about 90 minutes ·
**Status:** drafted
**Reading:** ~21 minutes

## Why this lesson exists

`construction-11` put the board behind a class boundary: the state became private, the operations
became public, and the compiler kept ordinary callers outside. The piece did not get the same
treatment. The game loop still held a record and read its fields:

```cpp
struct Knight
{
    int row;
    int column;
};
```

The name was not even stored. It was computed from the turn:

```cpp
const char *activeKnightName = playerIsActive ? "player" : "machine";
```

This baseline replaces the record with an interface. `piece.h` declares four operations and a
virtual destructor. `knight_piece.h` implements them. `game.cpp` holds `Piece *` and
`const Piece *`, and every read or write of a piece goes through a virtual call.

The sentence to keep:

> Late binding is the call site asking the object which function to run, instead of deciding for it.

Three honest notes before the comparison.

1. The source grows: 540 lines across ten files, against 488 across seven in `construction-11`.
   `game.cpp` shrinks from 287 to 281 lines; the interface and its implementation add 58 lines in
   three files.
2. The object code grows: `game.o` goes from 3,549 to 3,688 bytes of text (+139).
   `knight_piece.o` adds 665 bytes. The linked binary goes from 8,626 to 9,583 bytes of text
   (+957). The cost is one pointer per object and one table per class, plus indirect calls.
3. Twenty of twenty traces still pass. A trace records `who=player`; it does not record which
   function produced that word.

The comparison with `construction-11` is not "the interface is better". It is more precise: the
record made the loop read the representation. The interface makes the loop ask four questions and
stay ignorant of how the answers are implemented. The concrete type has not disappeared; it moved to
two lines in `run_game`.

## What you will be able to do

By the end of this lesson you can explain late binding and apply the lesson's change test.

## The concept: a call resolved at run time

`construction-11` asked: can a caller construct an invalid board state? No, because the writers are
private and guarded. This lesson asks a different question: can the game loop work without knowing
which concrete piece it is moving?

The answer is now yes, through the interface. `game.cpp` holds a pointer whose static type is
`Piece *`. When it writes `piece->row()`, the compiler does not know whether the object is a
`KnightPiece` or something else. It emits a call through the object's table, and the object's
dynamic type decides which function runs. That is late binding.

| | construction-11 | construction-12 |
|---|---|---|
| What the loop holds | `Knight *` | `Piece *` |
| How position is read | field access, `knight->row` | virtual call, `piece->row()` |
| How the name is chosen | `playerIsActive ? "player" : "machine"` | `piece->role_name()` |
| Where the concrete type is named | `run_game`, `Knight playerKnight` | `run_game`, `KnightPiece playerPiece` |
| What decides the call | the compiler, from the static type | the object, from its dynamic type |
| What a new same-behavior type costs | edits in the loop | two new files and three lines at the root |

### The interface

`piece.h` is eleven lines, and every one of them matters:

```cpp
#pragma once

class Piece
{
public:
    virtual ~Piece() = default;
    virtual int row() const = 0;
    virtual int column() const = 0;
    virtual void set_position(int row, int column) = 0;
    virtual const char *role_name() const = 0;
};
```

Five virtual declarations: four operations and the destructor. The `= 0` makes an operation pure:
it has no body in `Piece`, so `Piece` cannot be instantiated. The class is an interface, not a base
with a default implementation.

Three operations are `const`. That is what lets the loop hold `const Piece *` for the piece it is
not moving and still read its position and name. `set_position` is the only non-`const` operation,
and it needs a `Piece *`. The same discipline as `construction-11` carries over: the signature says
whether a function observes or mutates, and the compiler checks it.

`knight_piece.h` implements the interface and adds the state:

```cpp
class KnightPiece : public Piece
{
public:
    explicit KnightPiece(const std::string &roleName);
    int row() const override;
    int column() const override;
    void set_position(int row, int column) override;
    const char *role_name() const override;

private:
    int rowValue;
    int columnValue;
    std::string roleName;
};
```

The four `override` keywords are a check, not decoration: they tell the compiler to reject a
function that does not actually override a virtual operation. Step 6 shows what happens without
them.

Read the pair together. `Piece` is the contract; `KnightPiece` is one fulfillment of it:

| | `Piece` | `KnightPiece` |
|---|---|---|
| State | none | `rowValue`, `columnValue`, `roleName`, all private |
| Operations | five pure virtual declarations | four overrides plus a constructor |
| Instantiation | forbidden by `= 0` | allowed |
| What the loop sees | the interface | only through the interface |
| What it decides | which questions exist | how each question is answered |

The concrete class is still encapsulated in the `construction-11` sense: its state is private and
its writers are members. The interface adds a second boundary: the loop cannot name the concrete
class at all.

### What the loop asks

The loop never asks "are you a knight?". It asks four questions:

| Role | Operation | Implemented by | What the loop asks |
|---|---|---|---|
| Identity | `role_name()` | `KnightPiece` | "player" or "machine" |
| Position read | `row()`, `column()` | `KnightPiece` | the current square |
| Position write | `set_position()` | `KnightPiece` | move to a square |
| Destruction | `~Piece()` | the compiler | destroy through a base pointer |

Those four questions are the whole contract. A new type that answers them can stand where
`KnightPiece` stands. A type that needs a fifth question cannot, and that limit is real. See the
last subsection of this section.

### What late binding costs

Late binding is not free, and the cost is measurable:

- Each polymorphic object stores one pointer to its class table. On this platform `sizeof(Piece)`
  is 8 bytes and `sizeof(KnightPiece)` is 48: 8 for the pointer, 8 for the two integers, 32 for the
  `std::string`.
- Each polymorphic class has one table, emitted once in the object file. `nm -C knight_piece.o`
  shows `vtable for KnightPiece` and `typeinfo for KnightPiece`; the table is per class, not per
  object.
- Each virtual call is an indirect call through that table. The source has 19 call sites through
  the interface; at `-O2` six of them are devirtualized inside `run_game`, and the loop's own calls
  remain indirect.
- `game.o` grows by 139 bytes of text. The linked binary grows by 957 bytes of text and 184 bytes
  of data.

The compiler can devirtualize when it proves the dynamic type. In `run_game`, it inlines the
initialization and header helpers, sees two local `KnightPiece` objects, and replaces six virtual
calls with direct calls. That is an optimization inside one function. The loop's own parameters are
still `Piece *`, so the loop still compiles against the interface. Do not confuse a devirtualized
call with an early-bound design.

### What the interface does not cover

The loop still owns the knight's movement geometry:

```cpp
constexpr int knightRowChangePerMove[moveOrderMaximum] = {-1, -2, -2, -1, 1, 2, 2, 1};
constexpr int knightColumnChangePerMove[moveOrderMaximum] = {2, 1, -1, -2, -2, -1, 1, 2};
```

`tryMove` indexes those two tables at `game.cpp:153–154`. The interface asks where a piece is and
what it is called; it does not ask how it moves. A new piece that moves like a knight can be added
with zero edits in the loop. A new piece with a different movement rule cannot: `tryMove` has to
learn a new question, either as another interface operation or as a branch.

That is the precise acceptance boundary of this lesson, and it is also the honest limit of the
claim "add a piece; zero edits in the loop". You will measure both sides in Step 5.

## Before you start

- [`construction-11`](../construction-11-encapsulation/lesson.md) — the catalog prerequisite. The
  class boundary is the substrate for this lesson, and this lesson is a diff against it.
- [`construction-10`](../construction-10-adt-invariants/lesson.md) — the board invariant, so you
  know what the class still guarantees while the piece changes shape.
- [`construction-07`](../construction-07-pointers/lesson.md) — pointers and references. The loop now
  holds `Piece *` and `const Piece *`, and dispatch needs one of the two.
- [`construction-03`](../construction-03-functions/lesson.md) — you can read a function declaration,
  a parameter list and a return type.
- A C++20 compiler (`g++` or `clang++`). `cmake` is optional; `run.sh` falls back to a direct
  compile when CMake is absent.

You do not need to know the vtable layout of your compiler. You need to read a class declaration,
identify a pure virtual function, and compile one small file twice.

### What you need

- The polymorphism baseline:
  [`code/main.cpp`](code/main.cpp), [`code/game.h`](code/game.h),
  [`code/game.cpp`](code/game.cpp), [`code/piece.h`](code/piece.h),
  [`code/knight_piece.h`](code/knight_piece.h), [`code/knight_piece.cpp`](code/knight_piece.cpp),
  [`code/board.h`](code/board.h), [`code/board.cpp`](code/board.cpp),
  [`code/random_sequence.h`](code/random_sequence.h),
  [`code/random_sequence.cpp`](code/random_sequence.cpp).
- The previous baseline, for the comparison: the
  [`game.cpp`](../construction-11-encapsulation/code/game.cpp) of `construction-11`, especially the
  `struct Knight` and the places where the knight name is chosen.
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
test -f code/piece.h &&
test -f code/knight_piece.h &&
test -f code/knight_piece.cpp &&
test -f code/board.h &&
test -f code/board.cpp &&
test -f code/random_sequence.h &&
test -f code/random_sequence.cpp &&
test -f ../construction-11-encapsulation/code/game.cpp &&
test -f ../../../common/traces/seed-0001.txt
```

If a file is missing, the comparison has no second side. Record the missing file in `EVIDENCE.md`
§11 and return when it is available. A count against a file you have not read is not evidence.

The baseline reproduces all 20 traces in `common/traces/` byte for byte, before you change
anything. That is the behavioral contract, and it is silent about which function ran.

## Step 1 — Inventory the interface (~12 min)

Read the two new headers first. The shape is the lesson:

```bash
grep -n '^' code/piece.h
grep -n '^' code/knight_piece.h
```

```text
1:#pragma once
2:
3:class Piece
4:{
5:public:
6:    virtual ~Piece() = default;
7:    virtual int row() const = 0;
8:    virtual int column() const = 0;
9:    virtual void set_position(int row, int column) = 0;
10:    virtual const char *role_name() const = 0;
11:};
```

```text
1:#pragma once
2:
3:#include "piece.h"
4:
5:#include <string>
6:
7:class KnightPiece : public Piece
8:{
9:public:
10:    explicit KnightPiece(const std::string &roleName);
11:    int row() const override;
12:    int column() const override;
13:    void set_position(int row, int column) override;
14:    const char *role_name() const override;
15:
16:private:
17:    int rowValue;
18:    int columnValue;
19:    std::string roleName;
20:};
```

Now the line map of `knight_piece.cpp`. Fill it into `EVIDENCE.md` §2.2 from the source, not from
this table:

| Lines | Contents |
|---|---|
| 3–6 | `KnightPiece::KnightPiece` — initializes both coordinates to 0 and copies the role name. |
| 8–11 | `KnightPiece::row` |
| 13–16 | `KnightPiece::column` |
| 18–22 | `KnightPiece::set_position` — the only writer. |
| 24–27 | `KnightPiece::role_name` — returns a pointer into the stored string. |

Counts for this baseline: 5 headers, 5 translation units, 21 public declarations, 8 private data
declarations, 3 complete class definitions, 0 incomplete types, 35 definitions, 17 internal helpers
in `game.cpp`, 1 internal helper in `main.cpp`, 1 entry point.

Compare with `construction-11`: 11 public declarations and 30 definitions, 1 complete class, 3
headers, 4 translation units, 5 private data declarations. The interface and its implementation add
10 public declarations, 5 definitions and 2 classes. The board is untouched: its 7 public
operations, 5 private declarations and 21-line header are byte-identical.

Then answer in `EVIDENCE.md` §2:

1. Which line declares the destructor, and why is it virtual?
2. How many pure virtual operations does `Piece` declare? Name them.
3. Which operations are `const`, and what does that let a `const Piece *` do?
4. Which operation changes the object's state?
5. Which class implements the interface, and on which lines?
6. How many data members does `KnightPiece` have? Are they public?
7. How many virtual call sites are in `game.cpp`? Count `->row()`, `->column()`,
   `->set_position` and `->role_name`.
8. Which lines in `run_game` name the concrete type?
9. Does the loop contain a `switch`, a type tag, or a test on the piece's type?
10. What does `= 0` prevent, and what would change if `row()` lost it?

Question 7 has a precise answer: 19. Six `row`, six `column`, four `role_name`, three
`set_position`. Step 4 maps them to functions.

Question 8 has two answers: lines 264–265, where `KnightPiece playerPiece("player")` and
`KnightPiece machinePiece("machine")` are constructed. The include at line 4 is the only other
place that names the header. Nothing inside `playTurn`, `tryMove`, `printTurnHeader`, `printMove` or
`playGame` names a concrete type.

Question 9 has a precise answer: no. The loop has no type test. That is the property the change
test measures.

## Step 2 — Predict before you run (~12 min)

### 2.1 The dispatch table

Write these predictions before running anything. Every expression has static type `Piece *` or
`const Piece *` and points to a `KnightPiece`.

| Expression | Which function runs | Predicted output | Confidence |
|---|---|---|---|
| `playerPiece.set_position(3, 4)` through `KnightPiece` | | | |
| `piece->row()` through `const Piece &` | | | |
| `piece->column()` through `const Piece &` | | | |
| `piece->role_name()` through `const Piece &` | | | |
| `otherPiece->row()` through `const Piece *` | | | |
| `fromPiece->role_name()` in `printMove` | | | |

The second row is the one to take seriously. The static type is `Piece`; the dynamic type is
`KnightPiece`. If the call were resolved from the static type, it would have to call
`Piece::row`, which has no body. Predict which function actually runs, and what value it returns.

### 2.2 The change test

| Question | My prediction |
|---|---|
| Files I will add to implement a second `Piece` type | |
| Lines I will change in `game.cpp` | |
| Edits inside `playTurn`, `tryMove`, `printTurnHeader`, `printMove`, `playGame` | |
| Seeds that match, of 20, with the substitute at the construction site | |
| Seeds that match, of 20, after restoring `KnightPiece` | |
| New concepts the change requires | |

The third row is the acceptance. Predict zero before you measure it. The first two rows are cost,
not correctness.

### 2.3 The binding probe

| Question | My prediction |
|---|---|
| Output of `piece.row()` through `KnightPiece` after `set_position(3, 4)` | |
| Output of `piece.row()` through `const Piece &` with `row` virtual | |
| Output of the same call with `row` non-virtual and a base body returning 0 | |
| `sizeof(Piece)` | |
| `sizeof(KnightPiece)` | |
| `game.o` text size: grows, shrinks or stays the same? | |

The third row is the mechanism test. One keyword decides whether the same call expression reaches
the derived function or the base function. Predict the two values before you edit anything.

## Step 3 — Run the baseline and the dispatch probe (~12 min)

First the traces, unmodified:

```bash
mkdir -p /tmp/polymorphism-test
./run.sh --seed 1 --name Learner > /tmp/polymorphism-test/baseline-0001.txt
diff -u ../../../common/traces/seed-0001.txt /tmp/polymorphism-test/baseline-0001.txt
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
  -o build/polymorphism-baseline code/main.cpp code/game.cpp code/board.cpp \
  code/knight_piece.cpp code/random_sequence.cpp
```

Now the dispatch probe. It goes in the scratch directory, never in the tracked files:

```bash
mkdir -p /tmp/polymorphism-test/probe
cat > /tmp/polymorphism-test/probe/binding.cpp <<'EOF'
#include "knight_piece.h"
#include "piece.h"

#include <cstdio>

namespace
{
void report(const Piece &piece)
{
    std::printf("role=%s at=%d,%d\n", piece.role_name(), piece.row(), piece.column());
}

void move(Piece &piece, int row, int column)
{
    piece.set_position(row, column);
}
}

int main()
{
    KnightPiece playerPiece("player");
    move(playerPiece, 3, 4);
    report(playerPiece);

    KnightPiece machinePiece("machine");
    machinePiece.set_position(7, 2);
    const Piece &asInterface = machinePiece;
    report(asInterface);

    std::printf("sizeof(Piece)=%zu sizeof(KnightPiece)=%zu\n", sizeof(Piece),
                sizeof(KnightPiece));
    return 0;
}
EOF
g++ -std=c++20 -Wall -Wextra -Werror -O2 -Icode \
  -o /tmp/polymorphism-test/probe/binding \
  /tmp/polymorphism-test/probe/binding.cpp code/knight_piece.cpp
/tmp/polymorphism-test/probe/binding
```

The output on this platform:

```text
role=player at=3,4
role=machine at=7,2
sizeof(Piece)=8 sizeof(KnightPiece)=48
```

Read the first two lines against the static types. `report` takes `const Piece &`, and it printed
the position that `move` wrote through `Piece &`. The calls reached `KnightPiece::row` and
`KnightPiece::role_name`, not a base implementation. That is late binding in one screen.

The third line is the storage cost: one pointer in `Piece`, and 48 bytes in `KnightPiece` because of
the `std::string`. The exact numbers depend on the platform; record yours.

If the first diff is not empty, stop. That is a different baseline from the one this lesson
describes; record it in `EVIDENCE.md` §11 and do not adjust the prediction to match.

## Step 4 — Compare: what the dispatch proves (~10 min)

Open `game.cpp` at the call sites and `knight_piece.cpp` at the implementations. Five facts, read
against the interface.

1. **The program names the concrete type only at the root.** `grep -n 'KnightPiece' code/game.cpp`
   returns two lines: the two constructions at lines 264–265. The include at line 4 brings the
   header in. Every other function sees `Piece *` or `const Piece *`.
2. **There are 19 virtual call sites.** They are not scattered at random; each one asks one of the
   four questions:

   | Lines | Function | Virtual calls |
   |---|---|---|
   | 40–53 | `initializePositions` | `set_position` ×2 |
   | 65–76 | `printGameHeader` | `row` ×2, `column` ×2 |
   | 87–101 | `printTurnHeader` | `role_name` ×1, `row` ×1, `column` ×1 |
   | 122–132 | `printMove` | `role_name` ×1, `row` ×1, `column` ×1 |
   | 145–148 | `movePiece` | `set_position` ×1 |
   | 150–170 | `tryMove` | `row` ×2, `column` ×2 |
   | 172–238 | `playTurn` | `role_name` ×2 |

3. **The table is per class, the pointer is per object.** `nm -C knight_piece.o` shows
   `vtable for KnightPiece`, `typeinfo for KnightPiece` and `typeinfo name for KnightPiece`.
   `sizeof(Piece)` is 8, which is the pointer. The table itself is not inside the object.
4. **The call is late, not early.** The probe printed `at=3,4` through `const Piece &`. The base
   has no implementation to fall back to, and the derived function ran. Step 6 removes `virtual`
   and shows the same expression return `0`.
5. **The transcript is driven by `role_name()`.** In a scratch copy, change the two constructor
   arguments from `"player"` and `"machine"` to `"scout"` and `"rival"`, rebuild, and diff seed 1.
   Exactly 45 of the 89 lines change: the 22 `who=` lines, the 21 `move=` lines, the `trapped=`
   line and the `winner=` line. Every changed line contains a role name. Every position, every
   `order=` list and every `try=` line is identical.

Fact 5 is the strongest evidence that the loop asks rather than assumes. The name is not computed
from `playerIsActive` anymore; it is read from the object, and the output changes exactly where the
name appears. Reproduce it:

```bash
rm -rf /tmp/polymorphism-test/rename
cp -r code /tmp/polymorphism-test/rename
cd /tmp/polymorphism-test/rename
sed -i 's/playerPiece("player")/playerPiece("scout")/' game.cpp
sed -i 's/machinePiece("machine")/machinePiece("rival")/' game.cpp
g++ -std=c++20 -Wall -Wextra -Werror -O2 -I. \
  -o rename-baseline main.cpp game.cpp board.cpp knight_piece.cpp random_sequence.cpp
./rename-baseline --seed 1 --name Learner > /tmp/polymorphism-test/renamed-0001.txt
diff /tmp/polymorphism-test/baseline-0001.txt \
  /tmp/polymorphism-test/renamed-0001.txt | grep -c '^<'
cd -
```

The last command prints `45`: 45 lines were replaced. The diff has 90 lines because each replaced
line appears once as removed and once as added.

To see both call forms, compile `game.cpp` alone and disassemble it:

```bash
g++ -std=c++20 -O2 -Icode -c code/game.cpp -o /tmp/polymorphism-test/game.o
g++ -std=c++20 -O2 -Icode -c code/knight_piece.cpp \
  -o /tmp/polymorphism-test/knight_piece.o
objdump -dr -C /tmp/polymorphism-test/game.o | grep -E 'KnightPiece|call.*\*'
nm -C /tmp/polymorphism-test/knight_piece.o | grep -E 'vtable|typeinfo'
```

The `KnightPiece::` relocations appear inside `run_game`, where the compiler proved the dynamic
type. The indirect `call *0x18(%rax)` forms appear where the dynamic type is not known. Record both
in `EVIDENCE.md` §5.2.

The limit of the evidence: the probe and the traces show that the calls reach the implementation.
They do not show that the loop would accept a different implementation without edits. Only the
change test does.

Record in `EVIDENCE.md` §5 the five facts, the call-site table and the rename diff count.

## Step 5 — Run the change test (~20 min)

Copy the baseline. Tracked files stay untouched.

```bash
lessonDirectory="$(pwd)"
rm -rf /tmp/polymorphism-test/change
cp -r code /tmp/polymorphism-test/change
cd /tmp/polymorphism-test/change
```

### 5.1 Add a second implementation

The new type deliberately keeps knight movement, so the golden traces remain the contract. It is a
different class with the same four answers:

```bash
cat > spare_knight_piece.h <<'EOF'
#pragma once

#include "piece.h"

#include <string>

class SpareKnightPiece : public Piece
{
public:
    explicit SpareKnightPiece(const std::string &roleName);
    int row() const override;
    int column() const override;
    void set_position(int row, int column) override;
    const char *role_name() const override;

private:
    int rowValue;
    int columnValue;
    std::string roleName;
};
EOF
cat > spare_knight_piece.cpp <<'EOF'
#include "spare_knight_piece.h"

SpareKnightPiece::SpareKnightPiece(const std::string &roleName)
    : rowValue(0), columnValue(0), roleName(roleName)
{
}

int SpareKnightPiece::row() const
{
    return rowValue;
}

int SpareKnightPiece::column() const
{
    return columnValue;
}

void SpareKnightPiece::set_position(int row, int column)
{
    rowValue = row;
    columnValue = column;
}

const char *SpareKnightPiece::role_name() const
{
    return roleName.c_str();
}
EOF
```

That is 47 lines in two new files. No existing file has changed.

### 5.2 Exercise it through the interface

Before touching the game, prove that the new type answers the same questions:

```bash
cat > /tmp/polymorphism-test/probe/spare.cpp <<'EOF'
#include "piece.h"
#include "spare_knight_piece.h"

#include <cstdio>

namespace
{
void report(const Piece &piece)
{
    std::printf("role=%s at=%d,%d\n", piece.role_name(), piece.row(), piece.column());
}
}

int main()
{
    SpareKnightPiece piece("player");
    piece.set_position(3, 4);
    report(piece);
    return 0;
}
EOF
g++ -std=c++20 -Wall -Wextra -Werror -O2 -I. \
  -o /tmp/polymorphism-test/probe/spare \
  /tmp/polymorphism-test/probe/spare.cpp spare_knight_piece.cpp
/tmp/polymorphism-test/probe/spare
```

```text
role=player at=3,4
```

The function takes `const Piece &` and knows nothing about `SpareKnightPiece`. That is the property
the loop needs.

### 5.3 Substitute it at the construction site

Change only the root. The include and the two constructions are the whole diff:

```bash
python3 - <<'EOF'
from pathlib import Path
path = Path("game.cpp")
text = path.read_text()
text = text.replace('#include "piece.h"', '#include "piece.h"\n#include "spare_knight_piece.h"')
text = text.replace(
    '    KnightPiece playerPiece("player");\n    KnightPiece machinePiece("machine");',
    '    SpareKnightPiece playerPiece("player");\n'
    '    SpareKnightPiece machinePiece("machine");')
path.write_text(text)
EOF
diff -u "${lessonDirectory}/code/game.cpp" game.cpp
```

```text
@@ -3,6 +3,7 @@
 #include "board.h"
 #include "knight_piece.h"
 #include "piece.h"
+#include "spare_knight_piece.h"
 #include "random_sequence.h"
@@ -261,8 +262,8 @@
     Board board;
     board.reset();

-    KnightPiece playerPiece("player");
-    KnightPiece machinePiece("machine");
+    SpareKnightPiece playerPiece("player");
+    SpareKnightPiece machinePiece("machine");
     initializePositions(board, &playerPiece, &machinePiece);
```

Three lines added, two removed, and every one of them is in `run_game`. The loop is untouched.

Build and run the 20 traces:

```bash
g++ -std=c++20 -Wall -Wextra -Werror -O2 -I. \
  -o polymorphism-change main.cpp game.cpp board.cpp knight_piece.cpp \
  spare_knight_piece.cpp random_sequence.cpp
cd "${lessonDirectory}"
for trace in ../../../common/traces/seed-*.txt; do
  traceName="$(basename "${trace}" .txt)"
  seedText="${traceName#seed-}"
  seedNumber="$((10#${seedText}))"
  /tmp/polymorphism-test/change/polymorphism-change --seed "${seedNumber}" \
    --name Learner | cmp - "${trace}"
done
```

A silent loop is 20 of 20. The transcript is identical because the new type answers the same four
questions with the same values.

### 5.4 Restore the original and recount

Put `KnightPiece` back at the construction site, rebuild, and run the 20 traces again. Also 20 of
20. The original baseline still passes with the original type; adding a file did not disturb it.

Record both counts. They are different claims:

- 20 of 20 with `SpareKnightPiece` proves the loop accepts a different implementation.
- 20 of 20 with `KnightPiece` proves the baseline is unchanged.

### 5.5 The gap: movement is not in the interface

Now try a piece with a different movement rule. The loop computes a destination from two constant
tables:

```cpp
destinationRow = activePiece->row() + knightRowChangePerMove[moveNumber - 1];
destinationColumn = activePiece->column() + knightColumnChangePerMove[moveNumber - 1];
```

No virtual call is involved in the movement rule. A piece that moves differently needs a new
question in the interface, for example `move_delta(moveNumber, rowChange, columnChange)`, and a
change in `tryMove` to ask it. In a scratch copy, that extension costs:

- 2 new lines in `piece.h` for the pure virtual declaration,
- 2 new lines in `knight_piece.h` and 8 in `knight_piece.cpp` for the knight's answer,
- about 7 changed lines in `tryMove`, replacing two table lookups with one call,
- about 56 lines for the second implementation.

The knight game still matches 20 of 20 after the extension, which proves the extension preserved
behavior. The new piece has no golden trace, which proves the contract does not cover it. That is
not a failure of the interface; it is the boundary of what this interface abstracts. Record it in
`EVIDENCE.md` §6.5.

The acceptance for this lesson is therefore precise: **add a piece that answers the same four
operations; zero edits in the game loop.** A piece with a new movement rule is a different change,
and it costs an interface change.

## Step 6 — The interface in the rest of the program (~8 min)

### 6.1 Remove `virtual` and watch the call change

Copy `piece.h`, `knight_piece.h` and `knight_piece.cpp` to a second scratch directory. In that copy
only, give `row` a base body and remove `virtual` from it and `override` from its declaration:

```cpp
// piece.h
int row() const { return 0; }
```

```cpp
// knight_piece.h
int row() const;
```

Write the probe and build it against the copy:

```bash
rm -rf /tmp/polymorphism-test/early
mkdir -p /tmp/polymorphism-test/early
cp code/piece.h code/knight_piece.h code/knight_piece.cpp /tmp/polymorphism-test/early/
cd /tmp/polymorphism-test/early
cat > /tmp/polymorphism-test/probe/early.cpp <<'EOF'
#include "knight_piece.h"
#include "piece.h"

#include <cstdio>

int main()
{
    KnightPiece piece("player");
    piece.set_position(3, 4);

    std::printf("through KnightPiece: row=%d\n", piece.row());

    const Piece &asInterface = piece;
    std::printf("through Piece:      row=%d\n", asInterface.row());
    return 0;
}
EOF
g++ -std=c++20 -Wall -Wextra -Werror -O2 -I. \
  -o /tmp/polymorphism-test/probe/early \
  /tmp/polymorphism-test/probe/early.cpp knight_piece.cpp
/tmp/polymorphism-test/probe/early
```

```text
through KnightPiece: row=3
through Piece:      row=0
```

The same expression `asInterface.row()` now calls the base function, because the call is resolved
from the static type at compile time. That is early binding. The one keyword is the switch.

This is why the lesson's sentence says the call site asks the object. Without `virtual`, the call
site decides from the type it can see.

### 6.2 `override` is a check, not decoration

Change the declaration in `knight_piece.h` to `int row_value() const override;` and rename the
definition in `knight_piece.cpp` to `KnightPiece::row_value`. The compiler rejects the declaration:

```text
error: 'int KnightPiece::row_value() const' marked 'override', but does not override
```

Now remove `override` and keep the typo. The declaration compiles, and `KnightPiece` silently stays
abstract because `Piece::row` is still pure. The error appears later, at the construction site:

```text
error: cannot declare variable 'piece' to be of abstract type 'KnightPiece'
note: because the following virtual functions are pure within 'KnightPiece':
note: 'virtual int Piece::row() const'
```

`override` moves the error from the use site to the declaration, where the mistake is. Keep it on
every override.

### 6.3 The destructor is virtual for a reason

`virtual ~Piece() = default;` costs one table slot and buys correct destruction through a base
pointer. This program never deletes a piece through `Piece *`: both pieces are local objects in
`run_game`, and their destructors run at the end of the function. So the traces cannot see the
difference.

The interface must still be safe for the next user. If a `Piece *` ever owns heap memory, `delete`
through the base must run `KnightPiece::~KnightPiece`, which destroys the `std::string`. Without a
virtual destructor, it would run only `~Piece` and leak the string. `construction-16` owns lifetime
and examines this in detail.

### 6.4 Dispatch needs a reference or a pointer

`Piece` is abstract, so this does not compile:

```cpp
void report(Piece piece);   // error: cannot declare a parameter of abstract type
```

The abstract base prevents the slice by accident. If `Piece` were concrete, the parameter would copy
only the base subobject and drop the derived state; the call inside would then use the base
implementation. Dispatch works through `Piece &` or `Piece *`, which keep the dynamic type.

### 6.5 `const` still separates readers from writers

`tryMove` takes `const Piece *` for both pieces and can call `row`, `column` and `role_name`. It
cannot call `set_position`. `playTurn` holds `Piece *` for the active piece and calls
`set_position` through `movePiece`. The compiler checks the same rule it checked on `Board` in
`construction-11`, now on the piece interface.

| Function | Piece parameter | What it may do |
|---|---|---|
| `initializePositions` | `Piece *` | Call `set_position`. |
| `printGameHeader` | `const Piece *` | Read `row`, `column`. |
| `printTurnHeader` | `const Piece *` | Read `role_name`, `row`, `column`. |
| `printMove` | `const Piece *` | Read `role_name`, `row`, `column`. |
| `tryMove` | `const Piece *` | Read `row`, `column`. |
| `movePiece` | `Piece *` | Call `set_position`. |
| `playTurn` | `Piece *` and `const Piece *` | Move the active piece; read the other. |

Fill the `EVIDENCE.md` §7 table with these answers.

## Step 7 — Compare and explain (~4 min)

The comparison in one table:

| Question | construction-11 | construction-12 |
|---|---|---|
| What the loop holds | `Knight *` | `Piece *` |
| How it reads position | field access | virtual call |
| How it names the piece | boolean → literal | `role_name()` |
| Where the concrete type appears | `run_game` | `run_game`, two lines |
| What decides a call | the compiler, from the static type | the object, from its dynamic type |
| What a new same-behavior type costs | edits in the loop | two files and three lines at the root |
| What the interface does not cover | — | the movement geometry |
| Dispatch sites in `game.cpp` | 0 | 19 |
| `game.o` text | 3,549 bytes | 3,688 bytes |
| Binary text | 8,626 bytes | 9,583 bytes |
| Traces | 20 of 20 | 20 of 20 |

Write `EVIDENCE.md` §8. Two parts.

**First, one sentence.** Say what the call site decides, without using the words *virtual*,
*polymorphism*, *interface*, *class*, *pointer*, *compiler*, *dispatch*, *late binding*, *better*,
*worse*, *clean*, *flexible*, *extensible*, *elegant* or *ugly*. If your sentence needs one of
those, you have described the mechanism rather than the decision.

**Second, one paragraph.** Answer:

- What the interface fixes: which four questions the loop may ask a piece, and which operations are
  read-only.
- What enforces it: pure virtual declarations, `override` on every implementation, `const` on the
  observers, and a virtual destructor.
- What makes it different from `construction-11`: the loop no longer reads fields or computes a
  name from the turn; it asks the object.
- What the 19 call sites do: position reads, position writes, and name reads.
- What the 20 traces prove: the game's behavior is unchanged with the original type and with the
  substitute at the root.
- What the traces do not prove: which function ran. The binding probe and the `virtual` removal
  show that.
- What the change test costs: 47 new lines in two files and three lines at the construction site,
  zero loop edits, zero broken traces.
- What the interface does not cover: the movement geometry, which stays in `tryMove`.
- What the concrete type still costs: it is named once, in `run_game`, and that is the composition
  root.
- What late binding costs at run time: one pointer per object, one table per class, indirect calls,
  and lost inlining where the compiler cannot prove the type.
- Which later lesson builds on the interface: `construction-13`, composition, and `construction-14`,
  generic functions.
- Which later lesson owns lifetime: `construction-16`.

A strong answer keeps four things apart: **what the call site knows**, **what the object decides**,
**what the compiler emits**, and **what a run can see**. A weak answer says "the function is
virtual", which collapses the first three into a keyword.

## Step 8 — Acceptance check and record (~2 min)

The acceptance criterion for this lesson is: **add a piece type; zero edits in the game loop; the
original traces unchanged.**

Check your record against it:

- [ ] I identified the five virtual declarations in `piece.h` and the four overrides in
      `knight_piece.h`.
- [ ] I counted the 19 virtual call sites in `game.cpp` and mapped them to functions.
- [ ] I found that `KnightPiece` is named only in the two constructions, and its header is included
      once.
- [ ] I predicted the dispatch table and observed the derived functions run.
- [ ] I reproduced all 20 traces on the unmodified baseline.
- [ ] I built the binding probe and saw `role=player at=3,4` through `const Piece &`.
- [ ] I measured `sizeof(Piece)=8` and `sizeof(KnightPiece)=48`.
- [ ] I added `SpareKnightPiece` in a scratch file and exercised it through `Piece &`.
- [ ] I substituted it at the construction site with three lines and got 20 of 20 traces.
- [ ] I restored `KnightPiece` and got 20 of 20 traces with the original type.
- [ ] I confirmed zero edits in `playTurn`, `tryMove`, `printTurnHeader`, `printMove` and
      `playGame`.
- [ ] I removed `virtual` in a scratch copy and saw the same call return the base value.
- [ ] I explained the `override` error and the abstract-type error.
- [ ] I explained why the destructor is virtual even though the game never deletes a piece.
- [ ] I named the movement geometry as the limit of the interface.
- [ ] I did not edit any tracked code file.
- [ ] I completed `EVIDENCE.md` §1–§10 and the self-check in §12.

The criterion is not "the class uses virtual". It is "you can say which call sites depend on the
interface, which concrete type is still named at the root, what a new implementation costs, and
what the interface does not cover".

## Self-check (binary)

- [ ] I read `piece.h`, `knight_piece.h` and `knight_piece.cpp` and located the interface by line.
- [ ] I compared `struct Knight` with `Piece` and `KnightPiece` and named what each one exposes.
- [ ] I predicted the dispatch table, the change-test cost and the binding probe before running.
- [ ] I reproduced all 20 traces on the unmodified baseline.
- [ ] I built the dispatch probe and saw the derived functions run through `const Piece &`.
- [ ] I added a second `Piece` implementation in a scratch file.
- [ ] I showed 20 of 20 traces with the substitute at the construction site.
- [ ] I showed 20 of 20 traces after restoring the original `KnightPiece`.
- [ ] I counted zero edits in the game loop.
- [ ] I removed `virtual` in a scratch copy and explained the early-binding result.
- [ ] I explained the `override` check and the virtual destructor.
- [ ] I named what the interface does not cover: the movement geometry.
- [ ] I completed `EVIDENCE.md`.

These are acceptance criteria. If a box is
unchecked, write the next smallest action in `EVIDENCE.md` §10.

## Misconceptions this lesson refutes

| Misconception | Why it fails | What to do instead |
|---|---|---|
| "Polymorphism means many classes." | Without one call site that accepts several implementations, there is no polymorphism, only several types. | Point at the call site and count the implementations it accepts. |
| "`virtual` makes each object store a table." | The object stores one pointer; the table is per class and emitted once. | Read `sizeof(Piece)` and the `nm` output. |
| "The loop no longer knows the concrete type." | It still names `KnightPiece` in `run_game`. It no longer branches on it. | Separate "named once at the root" from "branched on everywhere". |
| "Adding a piece needs zero edits anywhere." | The construction site must name the new type. | Count loop edits and root edits separately. |
| "Any new piece works without edits." | Only one that answers the same four operations and moves like a knight. | Extend the interface before claiming a new movement rule is free. |
| "`override` is decoration." | Without it, a signature drift silently leaves the class abstract. | Keep `override` on every implementation and read the error. |
| "The destructor does not need `virtual` because we never delete." | True today; false the moment a `Piece *` owns heap memory. | Make the base destructor virtual when the type is designed for polymorphic use. |
| "Dynamic dispatch is free." | One pointer per object, one table per class, indirect calls, lost inlining. | Measure `sizeof`, object text and binary text. |
| "A value parameter is fine." | An abstract base cannot be a parameter; a concrete base would slice. | Pass `Piece &` or `Piece *`. |
| "The interface is the whole design." | It covers identity and position, not movement. | Put movement in the interface before claiming the loop is closed. |
| "The 20 traces prove the dispatch." | They pass with the original and the substitute; they cannot see which function ran. | Use the binding probe and the `virtual` removal. |
| "`virtual` must be repeated on the override." | Virtuality is inherited; repeating it is legal but redundant. | Write `override`, not `virtual` again. |
| "Devirtualization means the design changed." | The optimizer proved the dynamic type inside one function; the source still depends on the interface. | Separate the source contract from one optimized build. |

## Next step

`construction-13` — composition. This lesson put the four operations behind a base class and used
inheritance to share them. Composition asks the next question: what if the piece holds a movement
rule as a member and delegates to it, instead of inheriting from a base? The change test will
replace inheritance with delegation and compare the cost.

`construction-14` then takes the same four operations and writes one move generator for two piece
types with a template, without a base class at all. `construction-16` owns the lifetime question
that the virtual destructor only hints at.

See the [lesson catalog](../../../docs/guide/lesson-catalog.md) for the full path.

If either baseline was missing, do not start `construction-13`. The next contrast needs both sides.
