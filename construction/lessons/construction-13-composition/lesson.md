# Composition (composición)

> [English](lesson.md) | [Español](lesson-es.md)

**Concept:** composition over inheritance · **Time:** about 68 minutes ·
**Prerequisites:** construction-12 · **Status:** drafted
**Reading:** ~21 minutes
**Lesson role:** core

## Why this lesson exists

`construction-12` put the piece behind an interface. The game loop held `Piece *` and
`const Piece *`, asked four questions, and named the concrete type in exactly two lines of
`run_game`. The position lived in two integers inside `KnightPiece`, and a virtual table decided
which function answered.

This baseline removes the base class. `Knight` owns a `Position` and forwards the three position
questions to it:

```cpp
class Knight
{
public:
    explicit Knight(const std::string &roleName);
    int row() const;
    int column() const;
    void set_position(int row, int column);
    const char *role_name() const;

private:
    Position position;
    std::string roleName;
};
```

There is no `Piece`, no `virtual`, no `override`, and no table. `Knight::row()` calls
`position.row()`. `Knight::set_position()` calls `position.set()`. The concrete type is no longer
hidden at the root; it is named in every function signature in `game.cpp`.

The sentence to keep:

> Composition puts the part inside the whole and makes the whole answer by forwarding the question.

Three honest notes before the comparison.

1. The source grows: 556 lines across eleven files, against 540 across ten in `construction-12`.
   `game.cpp` shrinks by one line, from 281 to 280. The new `Position` type and its two files add
   16 net lines, and `Knight` repeats the four operations the interface used to declare.
2. The object shrinks: `sizeof(KnightPiece)` is 48 bytes and `sizeof(Knight)` is 40. The eight
   bytes are the pointer to the class table, and the table itself is gone: the linked binary loses
   184 bytes of read-only data. The object code tells a mixed story: `game.o` grows by 129 bytes
   even though the indirection is gone, while `knight_piece.o`'s 665 bytes become 563 across
   `knight.o` and `position.o`.
3. Twenty of twenty traces still pass. The traces record the game, not the structure. They cannot
   tell you whether a table exists.

The comparison with `construction-12` is not "composition is better". It is more precise:
inheritance bought substitutability and paid for it with a base class, a table and indirection.
Composition buys a smaller object and a direct call, and pays for it with a loop that names one
concrete type everywhere. Which bill is smaller depends on the next change, and this lesson
measures both.

## What you will be able to do

By the end of this lesson you can:

- explain composition over inheritance;
- replace inheritance with delegation and keep the traces unchanged.

## The concept: a part inside a whole

`construction-12` asked: can the loop work without knowing which concrete piece it is moving? The
answer was yes, through a base class. This lesson asks a different question: can the piece work
without a base class at all?

Yes, if the position becomes a part. `Knight` has a `Position` member. The member is a complete
object with its own private state and its own operations. `Knight` does not inherit from it; it
contains it and forwards the calls it does not answer itself.

| | construction-12 | construction-13 |
|---|---|---|
| Relation | `KnightPiece` is a `Piece` | `Knight` has a `Position` |
| How position is stored | two integers in `KnightPiece` | one `Position` member in `Knight` |
| How `row()` is answered | `override` of a virtual operation | `Knight::row()` calls `position.row()` |
| Where position logic lives | `knight_piece.cpp` | `position.cpp` |
| What decides a call | the object's dynamic type | the declared type, at compile time |
| Cost per object | table pointer plus state | state only |
| Concrete type in the loop | two constructions | every signature |

### The two classes

`position.h` is thirteen lines and holds one idea: a square is a value with two coordinates and a
guarded writer.

```cpp
#pragma once

class Position
{
public:
    int row() const;
    int column() const;
    void set(int row, int column);

private:
    int rowValue = 0;
    int columnValue = 0;
};
```

The default member initializers make a default-constructed `Position` the square `(0, 0)`. The
game overwrites both coordinates during setup. `Position` has no constructor of its own, so the
compiler generates one that applies those initializers.

`knight.h` adds identity and the four operations the loop already knows:

```cpp
#pragma once

#include "position.h"

#include <string>

class Knight
{
public:
    explicit Knight(const std::string &roleName);
    int row() const;
    int column() const;
    void set_position(int row, int column);
    const char *role_name() const;

private:
    Position position;
    std::string roleName;
};
```

The public surface is almost the same as `KnightPiece`: one constructor and four operations. What
changed is behind it. `Knight` has no base and no virtual declarations. The four operations are
ordinary member functions, resolved from the declared type.

### Delegation is forwarding, not inheritance

`knight.cpp` is the whole mechanism:

```cpp
int Knight::row() const
{
    return position.row();
}

int Knight::column() const
{
    return position.column();
}

void Knight::set_position(int row, int column)
{
    position.set(row, column);
}

const char *Knight::role_name() const
{
    return roleName.c_str();
}
```

Three of the four operations forward. `role_name` does not: the name belongs to `Knight`, not to
`Position`. That is the shape of delegation. The whole answers the question by asking its part, and
it answers alone the questions the part does not own.

Read the pair as a contract and its use:

| | `Position` | `Knight` |
|---|---|---|
| State | `rowValue`, `columnValue`, both private | one `Position` and one `std::string`, both private |
| Operations | `row`, `column`, `set` | constructor, `row`, `column`, `set_position`, `role_name` |
| Relation to the other | none | contains one `Position` |
| What it decides | how a square is stored | who owns the square and the name |
| What it forwards | — | `row`, `column`, `set_position` |

The nesting creates two boundaries. A caller of `Knight` cannot reach `Position`'s fields. `Knight`
itself cannot reach them either: it can only call `position.row()`, `position.column()` and
`position.set()`. The part is encapsulated against its own whole, not only against strangers.

### What composition costs

Composition is not free, and the costs are specific:

- **Forwarding boilerplate.** `Knight` repeats the part's interface. Three methods exist only to
  call three other methods. If `Position` renames `set` to `move`, `Knight::set_position` must
  change in the same commit.
- **No substitutability.** A `Knight` is not a `Piece`, because there is no `Piece`. A function
  that accepts `Knight &` cannot accept any other piece type. `construction-12` could accept a
  `SpareKnightPiece` with three lines at the root; this baseline cannot.
- **The concrete type spreads.** The loop names `Knight` in ten helper signatures plus two
  constructions. The type is not hidden at the root; it is the only type the loop can see.
- **A value member is copied.** Copying a `Knight` copies its `Position`. That is simple and safe
  for two integers, and it becomes a real question when a member owns a resource
  (`construction-16`).
- **The source grows.** One more type and one more translation unit: 556 lines across eleven
  files against 540 across ten. The object shrinks; the source does not.

### What composition does not cover

The loop still owns the movement geometry:

```cpp
constexpr int knightRowChangePerMove[moveOrderMaximum] = {-1, -2, -2, -1, 1, 2, 2, 1};
constexpr int knightColumnChangePerMove[moveOrderMaximum] = {2, 1, -1, -2, -2, -1, 1, 2};
```

`tryMove` indexes those tables at `game.cpp:152–153`. `Position` stores where a piece is; it does
not know how a knight moves. A different movement rule is still a change in `game.cpp`, exactly as
it was in `construction-12`.

The second limit is new. Inheritance let the loop hold one base type and accept any implementation
that answered the four questions. Composition gives the loop one concrete type. That is a real
loss, not a detail. `construction-14` restores one loop for two piece types with a template, and
`construction-15` constrains the template with concepts. Neither needs a base class.

## Before you start

- [`construction-12`](../construction-12-polymorphism/lesson.md) — the catalog prerequisite and
  the other side of the diff. You need its `piece.h`, `knight_piece.h` and `knight_piece.cpp` to
  remove them in Step 5.
- [`construction-11`](../construction-11-encapsulation/lesson.md) — the class boundary. `Position`
  repeats the same discipline: private state, public operations, `const` on readers.
- [`construction-04`](../construction-04-records/lesson.md) — records and aggregation. A member of
  class type is the same idea with a constructor and a boundary.
- [`construction-03`](../construction-03-functions/lesson.md) — you can read a declaration, a
  parameter list and a return type.
- A C++20 compiler (`g++` or `clang++`). `cmake` is optional; `run.sh` falls back to a direct
  compile when CMake is absent.

You do not need to know how your compiler lays out a class. You need to read two class declarations,
follow one forwarding call, and compare two builds.

### What you need

- The composition baseline:
  [`code/main.cpp`](code/main.cpp), [`code/game.h`](code/game.h),
  [`code/game.cpp`](code/game.cpp), [`code/knight.h`](code/knight.h),
  [`code/knight.cpp`](code/knight.cpp), [`code/position.h`](code/position.h),
  [`code/position.cpp`](code/position.cpp), [`code/board.h`](code/board.h),
  [`code/board.cpp`](code/board.cpp), [`code/random_sequence.h`](code/random_sequence.h),
  [`code/random_sequence.cpp`](code/random_sequence.cpp).
- The previous baseline, for the diff:
  [`code/piece.h`](../construction-12-polymorphism/code/piece.h),
  [`code/knight_piece.h`](../construction-12-polymorphism/code/knight_piece.h),
  [`code/knight_piece.cpp`](../construction-12-polymorphism/code/knight_piece.cpp) and
  [`code/game.cpp`](../construction-12-polymorphism/code/game.cpp).
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
test -f code/knight.h &&
test -f code/knight.cpp &&
test -f code/position.h &&
test -f code/position.cpp &&
test -f code/board.h &&
test -f code/board.cpp &&
test -f code/random_sequence.h &&
test -f code/random_sequence.cpp &&
test -f ../construction-12-polymorphism/code/piece.h &&
test -f ../construction-12-polymorphism/code/knight_piece.h &&
test -f ../construction-12-polymorphism/code/knight_piece.cpp &&
test -f ../../../common/traces/seed-0001.txt
```

If a file is missing, the comparison has no second side. Record the missing file in `EVIDENCE.md`
§11 and return when it is available. A count against a file you have not read is not evidence.

The baseline reproduces all 20 traces in `common/traces/` byte for byte, before you change
anything. That is the behavioral contract, and it says nothing about how the position is stored.

## Step 1 — Inventory the composition (~10 min)

Read the two new headers first. The shape is the lesson:

```bash
grep -n '^' code/position.h
grep -n '^' code/knight.h
```

```text
1:#pragma once
2:
3:class Position
4:{
5:public:
6:    int row() const;
7:    int column() const;
8:    void set(int row, int column);
9:
10:private:
11:    int rowValue = 0;
12:    int columnValue = 0;
13:};
```

```text
1:#pragma once
2:
3:#include "position.h"
4:
5:#include <string>
6:
7:class Knight
8:{
9:public:
10:    explicit Knight(const std::string &roleName);
11:    int row() const;
12:    int column() const;
13:    void set_position(int row, int column);
14:    const char *role_name() const;
15:
16:private:
17:    Position position;
18:    std::string roleName;
19:};
```

Now the line map of `knight.cpp`. Fill it into `EVIDENCE.md` §2.2 from the source, not from this
table:

| Lines | Contents |
|---|---|
| 3–6 | `Knight::Knight` — default-constructs `position` and copies the role name. |
| 8–11 | `Knight::row` — forwards to `Position::row`. |
| 13–16 | `Knight::column` — forwards to `Position::column`. |
| 18–21 | `Knight::set_position` — forwards to `Position::set`. |
| 23–26 | `Knight::role_name` — returns a pointer into the stored string; no forwarding. |

Counts for this baseline: 5 headers, 6 translation units, 19 public declarations, 9 private data
declarations, 3 complete class definitions, 0 incomplete types, 38 definitions, 17 internal helpers
in `game.cpp`, 1 internal helper in `main.cpp`, 1 entry point.

Compare with `construction-12`: 5 headers, 5 translation units, 21 public declarations, 8 private
data declarations, 3 complete classes, 35 definitions. Composition removes five declarations from
`piece.h` and five from `knight_piece.h`, and adds five to `knight.h` and three to `position.h`:
21 − 5 − 5 + 5 + 3 = 19. The board is untouched: its 7 public operations, 5 private declarations
and 21-line header are byte-identical.

Then answer in `EVIDENCE.md` §2:

1. Which class contains a `Position`, and on which line?
2. How many public operations does `Knight` declare? Name them.
3. Which of them forward to `Position`, and which does not?
4. Which operation of `Position` changes its state?
5. How many data members does `Position` have? Are they public?
6. Does any header in `code/` contain `virtual`, `override` or `: public`?
7. How many call sites in `game.cpp` ask a knight for `row`, `column`, `set_position` or
   `role_name`? Count the operators, not the lines.
8. In how many function signatures does `game.cpp` name `Knight`?
9. Where is the concrete type constructed?
10. What would break if `Position` made `rowValue` public?

Question 7 has a precise answer: 19 call sites, on 13 lines. Six `row`, six `column`, three
`set_position`, four `role_name`. The count is the same as `construction-12`; only the resolution
changed.

Question 8 has a precise answer: ten helper signatures plus `run_game`. The helpers are
`initializePositions`, `printGameHeader`, `printTurnHeader`, `printMove`, `selectActiveKnight`,
`selectOtherKnight`, `moveKnight`, `tryMove`, `playTurn` and `playGame`.

Question 6 has a precise answer: no. `grep -rn 'virtual\|override\|: public' code/` returns
nothing. That is the structural claim of this baseline.

## Step 2 — Predict before you run (~10 min)

### 2.1 The delegation table

Write these predictions before running anything. Every expression names a `Knight`; there is no
base type to hide behind.

| Expression | Which function runs | Predicted output | Confidence |
|---|---|---|---|
| `playerKnight.set_position(3, 4)` | | | |
| `playerKnight.row()` | | | |
| `playerKnight.column()` | | | |
| `playerKnight.role_name()` | | | |
| `asKnight.row()` through `const Knight &` | | | |
| `copy.set_position(5, 6)` after `Knight copy = playerKnight` | | | |

The last row is the one to take seriously. The copy is a separate object with its own `Position`.
Predict what happens to the original, and whether the two objects share anything.

### 2.2 The change test

The change is the one that produced this baseline: start from `construction-12` and replace the
interface with a member.

| Question | My prediction |
|---|---|
| Files I will delete | |
| Files I will add | |
| Lines I will change in `game.cpp` | |
| Functions in `game.cpp` whose signature changes | |
| Logic lines I will change (not names) | |
| Seeds that match, of 20, after the change | |
| New concepts the change requires | |

The fifth row is the acceptance. Predict zero before you measure it. A refactor that changes a rule
is a different lesson.

### 2.3 The size and indirection probe

| Question | My prediction |
|---|---|
| `sizeof(Position)` | |
| `sizeof(Knight)` | |
| `sizeof(KnightPiece)` | |
| Bytes saved per object | |
| `nm -C knight.o` lines with `vtable` or `typeinfo` | |
| Indirect calls in `game12.o` | |
| Indirect calls in `game13.o` | |
| `game.o` text: grows, shrinks or stays the same? | |

The sixth and seventh rows are the mechanism test. One design emits calls through a table; the
other emits direct calls. Predict both counts before you compile anything.

## Step 3 — Run the baseline and the delegation probe (~10 min)

First the traces, unmodified:

```bash
mkdir -p /tmp/composition-test
./run.sh --seed 1 --name Learner > /tmp/composition-test/baseline-0001.txt
diff -u ../../../common/traces/seed-0001.txt /tmp/composition-test/baseline-0001.txt
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
  -o build/composition-baseline code/main.cpp code/game.cpp code/board.cpp \
  code/knight.cpp code/position.cpp code/random_sequence.cpp
```

Now the delegation probe. It goes in the scratch directory, never in the tracked files:

```bash
mkdir -p /tmp/composition-test/probe
cat > /tmp/composition-test/probe/delegation.cpp <<'EOF'
#include "knight.h"
#include "position.h"

#include <cstdio>

namespace
{
void report(const Knight &knight)
{
    std::printf("role=%s at=%d,%d\n", knight.role_name(), knight.row(), knight.column());
}

void move(Knight &knight, int row, int column)
{
    knight.set_position(row, column);
}
}

int main()
{
    Knight playerKnight("player");
    move(playerKnight, 3, 4);
    report(playerKnight);

    Knight machineKnight("machine");
    machineKnight.set_position(7, 2);
    const Knight &asKnight = machineKnight;
    report(asKnight);

    Knight copy = playerKnight;
    copy.set_position(5, 6);
    std::printf("original=%d,%d copy=%d,%d\n", playerKnight.row(), playerKnight.column(),
                copy.row(), copy.column());

    std::printf("sizeof(Position)=%zu sizeof(Knight)=%zu\n", sizeof(Position), sizeof(Knight));
    return 0;
}
EOF
g++ -std=c++20 -Wall -Wextra -Werror -O2 -Icode \
  -o /tmp/composition-test/probe/delegation \
  /tmp/composition-test/probe/delegation.cpp code/knight.cpp code/position.cpp
/tmp/composition-test/probe/delegation
```

The output on this platform:

```text
role=player at=3,4
role=machine at=7,2
original=3,4 copy=5,6
sizeof(Position)=8 sizeof(Knight)=40
```

Read the third line against the copy. `copy` started as a copy of `playerKnight`, then moved to
`(5, 6)`. The original stayed at `(3, 4)`. The `Position` was copied with the `Knight`; the two
objects share no state. That is value semantics, and it is a direct consequence of holding the part
by value instead of pointing at it.

The fourth line is the storage cost: `Position` is two integers, 8 bytes. `Knight` adds the
`std::string`, 32 bytes on this platform, for 40 total. The exact numbers depend on the platform;
record yours.

If the first diff is not empty, stop. That is a different baseline from the one this lesson
describes; record it in `EVIDENCE.md` §11 and do not adjust the prediction to match.

## Step 4 — Compare: what delegation proves (~8 min)

Open `knight.cpp` at the forwards and `game.cpp` at the call sites. Five facts, read against the
two headers.

1. **There is no inheritance left.** `grep -rn 'virtual\|override\|: public' code/` returns no
   lines. `nm -C knight.o` contains no `vtable for Knight` and no `typeinfo for Knight`.
2. **The 19 call sites are direct calls.** They sit on 13 lines and ask the same four questions:

   | Lines | Function | Calls |
   |---|---|---|
   | 39–52 | `initializePositions` | `set_position` ×2 |
   | 64–75 | `printGameHeader` | `row` ×2, `column` ×2 |
   | 86–100 | `printTurnHeader` | `role_name` ×1, `row` ×1, `column` ×1 |
   | 121–131 | `printMove` | `role_name` ×1, `row` ×1, `column` ×1 |
   | 144–147 | `moveKnight` | `set_position` ×1 |
   | 149–169 | `tryMove` | `row` ×2, `column` ×2 |
   | 171–237 | `playTurn` | `role_name` ×2 |

3. **Delegation lives in three methods.** `knight.cpp:8–21` contains `row`, `column` and
   `set_position`, and each body is one forwarding call. `role_name` at lines 23–26 reads the
   string directly. The whole adds no position logic of its own.
4. **The object lost its table pointer.** `sizeof(Knight)` is 40 and `sizeof(KnightPiece)` is 48.
   The linked binary's read-only data falls from 876 to 692 bytes: the vtable, typeinfo and typeinfo
   name are 184 bytes that no longer exist.
5. **The transcript is still driven by `role_name()`.** In a scratch copy, change the two
   constructor arguments from `"player"` and `"machine"` to `"scout"` and `"rival"`, rebuild, and
   diff seed 1. Exactly 45 of the 89 lines change: the 22 `who=` lines, the 21 `move=` lines, the
   `trapped=` line and the `winner=` line. Every changed line contains a role name. Every position,
   every `order=` list and every `try=` line is identical. Reproduce it:

   ```bash
   rm -rf /tmp/composition-test/rename
   cp -r code /tmp/composition-test/rename
   cd /tmp/composition-test/rename
   sed -i 's/playerKnight("player")/playerKnight("scout")/' game.cpp
   sed -i 's/machineKnight("machine")/machineKnight("rival")/' game.cpp
   g++ -std=c++20 -Wall -Wextra -Werror -O2 -I. \
     -o rename-baseline main.cpp game.cpp board.cpp knight.cpp position.cpp \
     random_sequence.cpp
   ./rename-baseline --seed 1 --name Learner > /tmp/composition-test/renamed-0001.txt
   diff /tmp/composition-test/baseline-0001.txt \
     /tmp/composition-test/renamed-0001.txt | grep -c '^<'
   cd -
   ```

   The last command prints `45`: 45 lines were replaced. The diff has 90 lines because each
   replaced line appears once as removed and once as added.

Fact 5 is the bridge between the two baselines. The name is still read from the object, so the
observable behavior is unchanged. What changed is how the position question travels: `Knight::row`
calls `Position::row` instead of a table slot pointing at `KnightPiece::row`.

To see the code generation, compile `game.cpp` alone on both sides and count indirect calls:

```bash
g++ -std=c++20 -O2 -Icode -c code/game.cpp -o /tmp/composition-test/game13.o
g++ -std=c++20 -O2 -I../construction-12-polymorphism/code \
  -c ../construction-12-polymorphism/code/game.cpp -o /tmp/composition-test/game12.o
objdump -d /tmp/composition-test/game12.o | grep -c 'call.*\*'
objdump -d /tmp/composition-test/game13.o | grep -c 'call.*\*'
objdump -dr -C /tmp/composition-test/game13.o | grep 'R_X86_64_PLT32' | grep -c 'Knight::'
```

With this compiler and `-O2`, the first count is 13 and the second is 0. The third prints 24: 22
accessor calls plus two constructor calls. The accessor count is larger than the 19 call sites
because the optimizer duplicates a few calls when it inlines the small helpers into `run_game`. The
five distinct symbols are `Knight::row`, `Knight::column`, `Knight::set_position`,
`Knight::role_name` and the constructor. Record all three in `EVIDENCE.md` §5.2.

The limit of the evidence: the probe and the traces show that the same answers reach the same
places. They do not show that the loop could accept a different piece type. Only the change test
does, and here it shows the opposite of `construction-12`.

Record in `EVIDENCE.md` §5 the five facts, the call-site table, the rename diff count and the
indirection counts.

## Step 5 — Run the change test (~15 min)

The change test is the refactor that produced this baseline. Start from `construction-12` and
remove the base class. Tracked files stay untouched.

```bash
lessonDirectory="$(pwd)"
rm -rf /tmp/composition-test/change
cp -r ../construction-12-polymorphism/code /tmp/composition-test/change
cd /tmp/composition-test/change
```

### 5.1 Delete the interface

```bash
rm piece.h knight_piece.h knight_piece.cpp
```

Three files, 58 lines, and five virtual declarations plus four overrides leave the build. Nothing
else in the program included `piece.h` except `game.cpp` and `knight_piece.h`.

### 5.2 Add the part

```bash
cat > position.h <<'EOF'
#pragma once

class Position
{
public:
    int row() const;
    int column() const;
    void set(int row, int column);

private:
    int rowValue = 0;
    int columnValue = 0;
};
EOF
cat > position.cpp <<'EOF'
#include "position.h"

int Position::row() const
{
    return rowValue;
}

int Position::column() const
{
    return columnValue;
}

void Position::set(int row, int column)
{
    rowValue = row;
    columnValue = column;
}
EOF
```

That is 30 lines in two new files.

### 5.3 Add the whole

```bash
cat > knight.h <<'EOF'
#pragma once

#include "position.h"

#include <string>

class Knight
{
public:
    explicit Knight(const std::string &roleName);
    int row() const;
    int column() const;
    void set_position(int row, int column);
    const char *role_name() const;

private:
    Position position;
    std::string roleName;
};
EOF
cat > knight.cpp <<'EOF'
#include "knight.h"

Knight::Knight(const std::string &roleName)
    : position(), roleName(roleName)
{
}

int Knight::row() const
{
    return position.row();
}

int Knight::column() const
{
    return position.column();
}

void Knight::set_position(int row, int column)
{
    position.set(row, column);
}

const char *Knight::role_name() const
{
    return roleName.c_str();
}
EOF
```

That is 45 lines in two more files. The constructor's `: position()` is redundant, because a
`Position` is default-constructed anyway; it is written to make the member explicit.

### 5.4 Rename the loop

The game rules do not change. Only the type names change. Apply the rename and prove that no
`Piece` remains:

```bash
python3 - <<'EOF'
from pathlib import Path
path = Path("game.cpp")
text = path.read_text()
text = text.replace('#include "knight_piece.h"\n#include "piece.h"', '#include "knight.h"')
text = text.replace("KnightPiece", "Knight")
text = text.replace("playerPiece", "playerKnight")
text = text.replace("machinePiece", "machineKnight")
text = text.replace("activePiece", "activeKnight")
text = text.replace("otherPiece", "otherKnight")
text = text.replace("fromPiece", "fromKnight")
text = text.replace("selectActivePiece", "selectActiveKnight")
text = text.replace("selectOtherPiece", "selectOtherKnight")
text = text.replace("movePiece", "moveKnight")
text = text.replace("Piece *", "Knight *")
text = text.replace("void moveKnight(Knight *piece,", "void moveKnight(Knight *knight,")
text = text.replace("    piece->set_position(destinationRow, destinationColumn);",
                    "    knight->set_position(destinationRow, destinationColumn);")
path.write_text(text)
EOF
grep -n 'Piece' game.cpp
diff -u "${lessonDirectory}/../construction-12-polymorphism/code/game.cpp" game.cpp | head -40
```

The `grep` prints nothing. The diff has 14 hunks, 43 lines added and 44 removed, and every changed
line is a type or identifier rename. No `if`, no loop bound, no rule, and no output string changed.

### 5.5 Build and run the 20 traces

```bash
g++ -std=c++20 -Wall -Wextra -Werror -O2 -I. \
  -o composition-change main.cpp game.cpp board.cpp knight.cpp position.cpp \
  random_sequence.cpp
cd "${lessonDirectory}"
for trace in ../../../common/traces/seed-*.txt; do
  traceName="$(basename "${trace}" .txt)"
  seedText="${traceName#seed-}"
  seedNumber="$((10#${seedText}))"
  /tmp/composition-test/change/composition-change --seed "${seedNumber}" \
    --name Learner | cmp - "${trace}"
done
```

A silent loop is 20 of 20. The transcript is identical because no rule changed. That is the
acceptance criterion of this lesson: replace the inheritance with a member and forwarding, and keep
the behavior.

### 5.6 Count what moved

| | construction-12 | construction-13 | Difference |
|---|---|---|---|
| Files | 10 | 11 | +1 |
| Source lines | 540 | 556 | +16 |
| Public declarations | 21 | 19 | −2 |
| Private data declarations | 8 | 9 | +1 |
| Complete classes | 3 | 3 | 0 |
| Virtual declarations | 5 | 0 | −5 |
| Overrides | 4 | 0 | −4 |
| Forwarding methods | 0 | 3 | +3 |
| `sizeof` the piece | 48 | 40 | −8 |
| vtable and typeinfo data | 184 bytes | 0 | −184 |
| Indirect calls in `game.o` | 13 | 0 | −13 |
| `game.o` text | 3,688 | 3,817 | +129 |
| Binary text / data | 9,583 / 876 | 9,400 / 692 | −183 / −184 |
| Traces | 20 of 20 | 20 of 20 | 0 |

The change is a net trade, not a win on every axis. The source grows by 16 lines, `game.o` grows by
129 bytes, and the loop becomes coupled to `Knight` in ten signatures. The object loses its table
pointer, the binary loses 184 bytes of data, and thirteen indirect calls become direct calls.

### 5.7 The gap: substitutability is gone

In `construction-12`, this compiled and ran:

```cpp
class SpareKnightPiece : public Piece { /* four operations */ };
SpareKnightPiece playerPiece("player");
initializePositions(board, &playerPiece, &machinePiece);
```

In this baseline there is no `Piece`, so the same substitution has nowhere to attach. To make the
loop accept a second piece type you have three options, and each has a price:

- **Duplicate the loop** for the second type. That doubles ten functions and the turn logic.
- **Restore an interface.** That is `construction-12` again, with its table and indirection.
- **Parameterize the loop.** That is `construction-14`: one template, no base class, and the loop
  names a type parameter instead of `Knight`.

Measure the first option honestly in `EVIDENCE.md` §6.5: ten helper signatures plus `run_game`
would need a second copy or a type parameter. The second option is the previous lesson. The third
is the next one.

The acceptance for this lesson is therefore precise: **replace the inheritance with a member and
three forwards; behavior and all 20 traces unchanged.** It does not claim the loop can accept a new
piece type. It cannot, and that is the measured cost of this design.

## Step 6 — The composition in the rest of the program (~8 min)

### 6.1 A member is a value, not a reference

The probe showed it: `Knight copy = playerKnight` copies the `Position`, and moving the copy leaves
the original at `(3, 4)`. Nothing is shared. That is different from holding a `Position *`, where
two knights could point at the same square and one move would change both.

Value membership is the simplest ownership rule: the whole owns the part, the part dies with the
whole, and a copy is independent. `construction-16` examines what happens when a member owns heap
memory instead of two integers.

### 6.2 Forwarding couples the two interfaces

`Knight::set_position` calls `position.set`. If `Position` renames `set` to `move`, the forwarding
method stops compiling until it is updated. That is a feature: the compiler points at the exact
line. It is also a cost: every change in the part's interface propagates to every whole that
forwards it.

Try it in the scratch copy: rename `Position::set` to `Position::move` in `position.h` and
`position.cpp`, rebuild, and read the error. It names `Knight::set_position` at `knight.cpp:20`.
Restore the name before continuing.

### 6.3 `const` still separates readers from writers

`tryMove` takes `const Knight *` for both knights and can call `row`, `column` and `role_name`. It
cannot call `set_position`. `playTurn` holds `Knight *` for the active knight and calls
`set_position` through `moveKnight`. The compiler checks the same rule it checked on `Board` in
`construction-11` and on `Piece` in `construction-12`.

| Function | Knight parameter | What it may do |
|---|---|---|
| `initializePositions` | `Knight *` | Call `set_position`. |
| `printGameHeader` | `const Knight *` | Read `row`, `column`. |
| `printTurnHeader` | `const Knight *` | Read `role_name`, `row`, `column`. |
| `printMove` | `const Knight *` | Read `role_name`, `row`, `column`. |
| `tryMove` | `const Knight *` | Read `row`, `column`. |
| `moveKnight` | `Knight *` | Call `set_position`. |
| `playTurn` | `Knight *` and `const Knight *` | Move the active knight; read the other. |

Fill the `EVIDENCE.md` §7 table with these answers.

### 6.4 No virtual destructor, because there is no base

`Knight` owns a `Position` and a `std::string` by value. When a `Knight` goes out of scope, both
members are destroyed in reverse declaration order, and the compiler generates that code. There is
no `delete` through a base pointer, so there is no destructor question.

This is a real simplification, and it is conditional. The moment a member owns a resource through a
raw pointer, the whole needs a destructor, a copy constructor and a copy assignment, or a smart
pointer. `construction-16` owns that question.

### 6.5 What the compiler emits

`nm -C knight.o` shows `Knight::row`, `Knight::column`, `Knight::set_position` and
`Knight::role_name`, and no `vtable` or `typeinfo`. `objdump -d game13.o | grep -c 'call.*\*'`
prints 0, against 13 in `game12.o`.

The direct call is not automatically faster in a way you can see in this program: the game is
dominated by `printf`. The honest claim is structural, not a benchmark: there is no indirection to
resolve and no table to load. Whether that matters depends on how often the call runs and what else
the function does. Measure before claiming a speedup.

### 6.6 What composition does not hide

In `construction-12`, `grep -n 'KnightPiece' game.cpp` returned two lines: the two constructions.
In this baseline, `Knight` appears in ten helper signatures and two constructions. The loop is
coupled to one concrete type.

That is the reversal the lesson is about. Inheritance moved the type decision to the root and kept
the loop generic over the interface. Composition moves the type into every signature and keeps the
call direct. Neither is free; they spend the complexity in different places.

## Step 7 — Compare and explain (~5 min)

The comparison in one table:

| Question | construction-12 | construction-13 |
|---|---|---|
| Relation | is-a | has-a |
| Loop parameter | `Piece *` / `const Piece *` | `Knight *` / `const Knight *` |
| Where position lives | two integers in `KnightPiece` | a `Position` member |
| Who answers `row` | virtual `KnightPiece::row` | `Knight::row` → `Position::row` |
| Call resolution | dynamic type | declared type, compile time |
| Concrete type in the loop | two constructions | ten signatures and two constructions |
| A second same-behavior type | two files, three root lines | ten signatures or a template |
| `sizeof` the piece | 48 | 40 |
| vtable and typeinfo | 184 bytes | 0 |
| Indirect calls in `game.o` | 13 | 0 |
| `game.o` text | 3,688 bytes | 3,817 bytes |
| Binary text / data | 9,583 / 876 | 9,400 / 692 |
| Source | 540 lines, 10 files | 556 lines, 11 files |
| Traces | 20 of 20 | 20 of 20 |

Write `EVIDENCE.md` §8. Two parts.

**First, one sentence.** Say what the piece does with its square, without using the words
*composition*, *inheritance*, *delegation*, *class*, *object*, *virtual*, *pointer*, *compiler*,
*better*, *worse*, *clean*, *flexible*, *reusable* or *elegant*. If your sentence needs one of
those, you have described the mechanism rather than the decision.

**Second, one paragraph.** Answer:

- What composition fixes: the square becomes a named part with one implementation, and `Knight`
  owns it by value.
- What enforces it: private state in both types, `Position`'s public operations as the only access,
  and `const` on the readers.
- What changed from `construction-12`: no base, no virtual declarations, no table; three forwards
  and a direct call.
- What the 19 call sites do: six position reads, six column reads, three position writes and four
  name reads, all direct.
- What the 20 traces prove: the game's behavior is unchanged after the refactor.
- What the traces do not prove: which function ran, whether a table exists, or how large the object
  is.
- What the change test costs: three files deleted, four added, one edited; 43 lines added and 44
  removed in `game.cpp`, all renames; zero rule changes.
- What composition does not cover: substitutability. The loop names `Knight` in ten signatures, so
  a second piece type needs a template or a restored interface. The movement geometry is still in
  `tryMove`.
- What it saves: 8 bytes per object, 184 bytes of table data, and 13 indirect calls.
- What it costs: 16 more source lines, one more file, three forwarding methods, and a loop coupled
  to one type.
- Which later lesson builds on it: `construction-14`, generic functions, restores one loop for two
  types without a base; `construction-15` adds constraints.
- Which later lesson owns lifetime: `construction-16`.

A strong answer keeps four things apart: **what the loop can name**, **what the part decides**,
**what the compiler emits**, and **what a run can see**. A weak answer says "composition is
better", which collapses all four into a verdict.

## Step 8 — Acceptance check and record (~2 min)

The acceptance criterion for this lesson is: **replace inheritance with a member and forwarding;
behavior and all 20 traces unchanged.**

Check your record against it:

- [ ] I identified the `Position` member in `knight.h` and the three forwarding methods in
      `knight.cpp`.
- [ ] I counted the 19 call sites in `game.cpp` and mapped them to functions.
- [ ] I found that no header contains `virtual`, `override` or `: public`.
- [ ] I named the ten helper signatures that mention `Knight`.
- [ ] I predicted the delegation table and observed the forwarded values.
- [ ] I reproduced all 20 traces on the unmodified baseline.
- [ ] I built the delegation probe and saw `role=player at=3,4` through `const Knight &`.
- [ ] I measured `sizeof(Position)=8` and `sizeof(Knight)=40`.
- [ ] I confirmed that copying a knight copies its position and shares nothing.
- [ ] I deleted the three interface files in a scratch copy and added `Position` and `Knight`.
- [ ] I renamed the loop with 43 added and 44 removed lines and zero rule changes.
- [ ] I got 20 of 20 traces after the refactor.
- [ ] I counted 13 indirect calls in `game12.o` and 0 in `game13.o`.
- [ ] I confirmed that `nm` shows no vtable or typeinfo for `Knight`.
- [ ] I named substitutability as the limit of this design.
- [ ] I explained why no virtual destructor is needed here.
- [ ] I did not edit any tracked code file.
- [ ] I completed `EVIDENCE.md` §1–§10 and the self-check in §12.

The criterion is not "the code uses composition". It is "you can say what the part owns, what the
whole forwards, which type the loop can name, what the object saves, and what the design gives up".

## Self-check (binary)

- [ ] I read `position.h`, `knight.h` and `knight.cpp` and located the forwards by line.
- [ ] I compared `KnightPiece` with `Knight` and `Position` and named what each one exposes.
- [ ] I predicted the delegation table, the change-test cost and the size probe before running.
- [ ] I reproduced all 20 traces on the unmodified baseline.
- [ ] I built the delegation probe and saw the forwarded values.
- [ ] I confirmed that copying a knight copies its position.
- [ ] I replaced the inheritance with a member and forwarding in a scratch copy.
- [ ] I showed 20 of 20 traces after the refactor.
- [ ] I counted zero rule changes and 87 changed lines in `game.cpp`.
- [ ] I measured the object, table and indirection differences.
- [ ] I named substitutability as the limit of composition.
- [ ] I explained why no virtual destructor is needed.
- [ ] I completed `EVIDENCE.md`.

These are acceptance criteria. If a box is
unchecked, write the next smallest action in `EVIDENCE.md` §10.

## Misconceptions this lesson refutes

| Misconception | Why it fails | What to do instead |
|---|---|---|
| "Composition means no classes." | Composition is a relation between classes: has-a instead of is-a. | Point at the member and at the two class declarations. |
| "Delegation is inheritance with extra steps." | There is no base, no substitutability and no dynamic dispatch. | Try to pass a `Knight` where a `Piece` was expected; the type does not exist. |
| "Composition always uses fewer lines." | Here it uses 16 more lines and one more file. | Count the new type, its translation unit and the forwards. |
| "Composition always shrinks the binary." | `game.o` grows by 129 bytes; the data shrinks by 184. | Measure text and data separately. |
| "The loop no longer knows the concrete type." | It knows it more: ten signatures name `Knight`. | Count the signatures, not the constructions. |
| "A member is a pointer." | `Position position` is a value; copying the whole copies the part. | Run the copy probe and read the original. |
| "Delegation removes the need for an interface." | It removes this interface; a second type still needs a template or a base. | State the next change before claiming generality. |
| "`Position` is just two integers, so it is free." | It is 8 bytes, a constructor, and a call boundary the compiler may inline. | Measure `sizeof` and the object code. |
| "The virtual destructor is still needed." | There is no base and no delete through a base; members destroy themselves. | Check for ownership through a raw pointer before adding one. |
| "The 20 traces prove composition." | They prove behavior, not structure. | Use `nm`, `size` and the indirection count. |
| "Composition is better than inheritance." | It is cheaper for this change and worse for the next one. | Name the change and compare its cost. |
| "The refactor changed the algorithm." | Only names and the storage of position changed. | Diff the rules: zero changed lines. |
| "Forwarding is duplication to remove later." | The forwards are the boundary; removing them removes the part's encapsulation. | Keep them, or let the whole expose the part deliberately. |

## Next step

`construction-14` — generic functions. This lesson removed the base class and paid with a loop
coupled to `Knight`. The next lesson writes one move generator for two piece types with a template,
so the loop names a type parameter instead of a concrete type, and no table returns.

`construction-15` adds concepts to constrain that template. `construction-16` owns the lifetime
question that value membership only postpones. `construction-12` remains the other side of this
contrast; keep both baselines.

See the [lesson catalog](../../../docs/guide/lesson-catalog.md) for the full path.

If either baseline was missing, do not start `construction-14`. The next contrast needs both sides.
