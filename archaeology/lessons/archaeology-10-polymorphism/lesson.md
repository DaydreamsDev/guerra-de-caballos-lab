# archaeology-10 — Missing polymorphism

> [English](lesson.md) | [Español](lesson-es.md)

> **Status:** drafted · **Concept:** polymorphism · **Prerequisite:** archaeology-09 ·
> **Estimated time:** 90 minutes
> **Reading:** ~22 minutes
> **Lesson role:** optional · safe to skip

## Why this lesson exists

`archaeology-06` removed a `switch` on the move number and replaced it with a table. The program got
smaller and the rule moved into data. That was the right fix for duplication, and it left a question
open: the loop still decides **which piece** is moving, by reading an integer.

This lesson is about that second decision. Not *which delta*, but *whose deltas*. The difference
matters because the two decisions fail in different ways and are repaired by different mechanisms.

In the archive, the piece is not a type. It is two global coordinate pairs and one integer:

```text
18:  int  cab1,cab2,jugada=1,orden,ganador=1;
29:  int cf1,cc1,cf2,cc2,cab;
```

`cab` is `1` or `2`. It is assigned before each move, and `movcab` reads it to choose which knight's
coordinates and visited cells to touch. There are nine `if (cab==1)` sites in `variant-01`, ten in
the other three variants. `impresionorden` contains four turn blocks that each name a knight, its
coordinates and its counter by hand.

Adding a third piece to that design is not a one-line change. You need a third coordinate pair, a
third counter, a third branch at every `cab==1` site, a fourth turn block, and a movement rule for
the new piece. The switch on move number does not know the piece exists; the loop knows it in nine
places.

That is the defect the catalog calls `failure-011`: **a `switch` on piece type instead of
dispatch**. The program works. The cost is paid by the next piece.

The product of this lesson is one **replacement**: the switch on piece kind becomes virtual
dispatch, and a frozen 400-line trace proves the behavior did not change. The record is the price:
what dispatch bought, what it cost, and what it did not fix.

## What you will be able to do

By the end of this lesson you can:

- explain the difference between a static type and a dynamic type, and which one decides a call;
- describe what a virtual call does at run time, and what it prevents the compiler from doing;
- separate a switch on **piece kind** from a switch on **move number**, and say why they need
  different repairs;
- read the baseline's `Piece` interface and its two implementations;
- freeze the switch's output as an oracle before deleting the switch;
- replace the switch with dispatch in a scratch copy and keep 400 comparisons at zero mismatches;
- add a third piece in each path and count what the change touches, not only how many lines it adds;
- state the real archive cost: nine `cab==1` sites, four turn blocks, and the globals;
- name what dispatch does not fix: the move table, the loop's other knowledge, and the missing
  tests;
- keep the archive read-only while you work.

The replacement is done when
someone else can reproduce the run.

## The concept: dispatch

### Two decisions that look alike

Both of these are `switch` statements. They are not the same problem.

```cpp
switch (moveNumber) { case 1: return Delta{-1, 2}; /* ... */ }
switch (pieceKind)  { case PieceKind::knight: /* the whole rule set */ }
```

| | Switch on move number | Switch on piece kind |
|---|---|---|
| The question | Which **datum** applies to this move? | Which **rule set** applies to this piece? |
| The cases differ by | Constants inside one rule. | Whole algorithms, each with its own constants. |
| The set is | Closed: a knight has eight moves. | Open: the game can grow pieces. |
| The fix | A table. `archaeology-06` already did it. | Dispatch. This lesson. |
| What stays in the loop | The move index. | Nothing about the piece. |

A table of deltas indexed by move number is still dispatch. It dispatches by index. It removes
copied arithmetic; it does not create a boundary around a piece. You can have a perfect move table
and still have no polymorphism, which is exactly the archive's situation.

Conversely, a class hierarchy does not remove the need for a move index. `Knight::move(3, ...)`
still has to know that move 3 is `(-2, -1)`. Polymorphism answers *who moves*; the table answers
*where*. The two mechanisms compose. Confusing them produces the wrong repair: a
`Delta table[pieceKind][moveNumber]` makes the kind switch disappear, but the loop still
enumerates every kind, and a piece whose movement depends on the board cannot be expressed in
that table at all.

### Static type, dynamic type, and the call

A pointer has two types. The **static type** is what the compiler sees. The **dynamic type** is what
the object really is.

```cpp
std::unique_ptr<Piece> knight = std::make_unique<Knight>();
Piece *piece = knight.get();       // static type: Piece*
piece->move(1, Position{0, 0});    // dynamic type: Knight
```

The compiler sees `Piece *`. It cannot know that the object is a `Knight`, because the pointer could
hold any implementation. So it does not emit a call to `Knight::move`. It emits a call through the
object's class table, and the table decides at run time. That is **late binding**, or dynamic
dispatch.

The mechanism is small enough to state precisely:

- every polymorphic object stores one hidden pointer, the **vptr**, to its class's table;
- every polymorphic class has one **vtable**, emitted once per class, holding the addresses of its
  virtual functions;
- a virtual call loads the vptr, loads the slot for that function, and calls through it.

`Piece` declares three virtual functions in this baseline: the destructor, `move` and `role_name`.
`Knight` and `MirrorKnight` each supply two overrides. `= 0` makes `move` and `role_name` pure: the
interface has no body, so `Piece` cannot be instantiated. `override` is a check, not decoration: it
makes the compiler reject a function that only looks like an override.

The virtual destructor is not cosmetic. The objects are owned by `std::unique_ptr<Piece>`; deleting
through a base pointer without a virtual destructor is undefined behavior. The interface pays for
that safety with one vtable slot.

### What a virtual call costs

Late binding is not free, and the cost has several parts:

| Cost | Mechanism | In this baseline |
|---|---|---|
| Object size | One vptr per object. | `sizeof(Piece)` is 8 bytes on this platform. |
| Per-class table | One vtable per class. | Two vtables, emitted once. |
| The call | Load vptr, load slot, indirect call. | 400 calls in the whole run. |
| Inlining | Blocked unless the compiler proves the type. | The loop's calls stay indirect. |
| Prediction | Indirect targets are harder to predict. | Two targets, one per piece. |
| Allocation | One heap object per piece. | Two allocations at startup. |

Two things keep this honest.

First, the compiler can **devirtualize** when it proves the dynamic type. A local `Knight` object, a
`final` class, or a call the optimizer can trace back to one constructor may become a direct call.
That is an optimization inside one function, not a change of design. Do not read a devirtualized
call as evidence that the interface was unnecessary.

Second, the `switch` is not free either. A dense switch on an integer usually becomes a jump table,
which is also an indirect branch, plus the work of checking the enum. Comparing "virtual call" with
"zero cost" is wrong. Compare it with "switch plus a branch per kind".

At the scale of this lesson, 400 calls, neither mechanism matters. If you want to claim a runtime
difference, measure it: 100 million calls, same compiler and flags, several runs, and a note that
the difference may be noise. If the difference is under the noise, the honest statement is "not
measured". The claim of this lesson is change cost, not cycles.

### The change test

A design lesson is judged by a change, not by a shape. The change here is: **add a third piece with
its own movement rule**. A camel, say, with deltas `(1,3)`, `(3,1)`, `(3,-1)`, `(1,-3)`, `(-1,-3)`,
`(-3,-1)`, `(-3,1)`, `(-1,3)`.

In the dispatch path, that is a new class and one registration. In the switch path, it is a new enum
value, a new 22-line branch inside the function that every piece shares, and a new entry wherever
the kind is listed. The measured table is in Step 5. The point is not that one path is shorter — the
line counts are close. The point is **where the edit lands**.

### What the 2008 code actually contains

This is the part where the lesson has to be precise, because the catalog's shorthand is not the
source.

The archive has **no switch on piece kind**. It has a switch on move number at
`original/variant-01-baseline-2008/source/guerra-de-caballos-baseline.cpp:239`, and it has the `cab`
integer with `if (cab==1)` branches. The piece-kind decision is scattered, not centralized.

The baseline in this lesson, `code/polymorphism.cpp`, reconstructs that decision as
`delta_with_switch(PieceKind, int)`. That reconstruction is a **bench**: it isolates the decision so
the two mechanisms can be compared on the same 400 inputs. It is not verbatim 2008 code, and the
`MirrorKnight` is the lab's invention — the archive's two knights move identically.

Say that in your record. A diagnosis that cites the baseline as if it were the original is not
evidence.

### The legacy constraint

A `switch` and an `if` on an integer were reasonable under the constraints: one translation unit,
no tests, no version control, an old compiler, and two pieces of the same kind. There was no second
movement rule to dispatch to. The cost of the design appears when a third piece arrives.

The lesson is not "the code was wrong". It is "the structure made the next piece expensive, and a
dispatch boundary removes that cost".

## Before you start

Read these in full:

- [`README.md`](README.md) — the lesson card, the build command and the acceptance criterion.
- [archaeology-06](../archaeology-06-duplication-switch/lesson.md) — the move-number switch and its
  table. This lesson assumes that repair and does not repeat it.
- [archaeology-09](../archaeology-09-abstract-data-type/README.md) — the prerequisite: an opaque
  type, a representation swap, and the caller that did not change.
- [`../../../docs/guide/archaeology-track.md`](../../../docs/guide/archaeology-track.md) — §3, the
  failure taxonomy; §5, the severity scale; §9, the Track A mapping.
- [`../../../docs/archive.md`](../../../docs/archive.md) — the pinned archive, tag and commit.

You also need:

- a local clone of the archive at tag `v1.0.0`;
- a C++20 compiler (`c++`, `g++` or `clang++`);
- `bash`;
- a scratch directory outside `original/`.

> **Rule:** the archive is read-only. Copy the file you want to change into a scratch directory;
> never edit `original/`. The baseline in this lesson is also evidence: copy it before you delete
> anything.

## The 90-minute path

| Min | Step | What you do | Evidence |
|---|---|---|---|
| 0–10 | Predict | Estimate the interface size, the call cost and the third-piece edit | Prediction table |
| 10–30 | Inspect | Read the interface, the switch and the archive anchors; run the baseline | Anchor notes and `comparisons=400 mismatches=0` |
| 30–55 | Dispatch | Freeze the oracle, remove the switch, keep the classes | Scratch copy and `golden.txt` |
| 55–65 | Compare | Run the dispatch version against the frozen trace | 400 comparisons, 0 mismatches |
| 65–80 | Explain | Price the virtual call, run the change test, separate the two switches | Cost table and change test |
| 80–90 | Record | Fill `EVIDENCE.md`; confirm the archive is clean | Portfolio entry |

If the vtable explanation takes twenty minutes, that explanation is the
lesson.

### The engineering check in this lesson

Track B uses the same habit as Track A, with a different verb: the behavior already exists, so you
**freeze** it instead of specifying it from scratch.

| Step | Where it happens | What it produces |
|---|---|---|
| **Predict** | Step 1 | A falsifiable estimate of interface size, call cost and edit cost. |
| **Inspect** | Step 2 | The interface, the two switches, the archive anchors and the baseline run. |
| **Dispatch** | Step 3 | A scratch copy with classes and no switch. |
| **Compare** | Step 4 | 400 comparisons against the frozen oracle, 0 mismatches. |
| **Explain** | Step 5 | The change test, the call cost and the two-switch distinction. |
| **Record** | Step 6 | `EVIDENCE.md` with counts, diff and result. |

## Step 1 — Predict (10 min)

Before you open the source, answer these five questions in writing. The point is to commit to
numbers you can be wrong about.

1. How many virtual functions does the `Piece` interface declare?
2. How many lines is `delta_with_switch`, and how many branches does it contain?
3. What does one virtual call cost in machine terms, and how many calls does the run make?
4. If you add a third piece, how many existing declarations must change in each path?
5. Which of the two switches — move number or piece kind — does the archive actually contain?

A useful prediction is falsifiable. "Virtual calls are slow" is not. "One indirect call through a
vtable slot, 400 calls per run, and the cost is irrelevant at this scale" is.

### A worked prediction

Before reading, I would write:

- interface: three virtual declarations — destructor, `move`, `role_name`;
- switch: about 50 lines, two inner switches of eight cases plus a default each;
- call: one vptr load and one indirect call, 400 calls, so the cost is invisible next to `printf`;
- third piece: one new class in the dispatch path, one enum value and one long branch in the switch
  path;
- archive: the switch is on the move number; the piece is an integer called `cab`.

Every claim can be checked. That is the standard. If your prediction says "polymorphism is better",
you have not predicted anything yet; that is a conclusion, not an estimate.

## Step 2 — Inspect (20 min)

### Read the interface

```bash
cd archaeology/lessons/archaeology-10-polymorphism
grep -n 'virtual\|override\|class ' code/polymorphism.cpp
```

The baseline is 163 lines. Its parts:

| Part | Lines | What it is |
|---|---:|---|
| `Position`, `Delta` | 5–17 | A row and a column; the two integers that vary. |
| `PieceKind` | 19–23 | The enum the switch dispatches on. |
| `knightDeltas`, `mirrorDeltas` | 25–33 | The two rule sets as data. |
| `Piece` | 35–41 | Three virtual declarations, two of them pure. |
| `Knight`, `MirrorKnight` | 43–71 | Two implementations, each with `move` and `role_name`. |
| `delta_with_switch` | 73–123 | The oracle: a switch on kind, then a switch on move. |
| `move_with_switch` | 125–129 | Applies the switch's delta to a position. |
| `main` | 131–163 | 2 pieces × 8 moves × 25 positions = 400 comparisons. |

Read `Piece` twice. The interface answers exactly two questions: *where do you go from here on move
n?* and *what are you called?* That is enough for this lesson and not enough for a whole game. The
limit is the subject of Step 5.

Note what the interface does **not** contain: position, capture, visited cells, turn order. The
baseline has no board. It is a bench for one decision.

### Read the switch

`delta_with_switch` is 51 lines for sixteen integers. The outer switch selects the rule set; each
inner switch selects the move. The inner switches end in `default: return Delta{0, 0};`.

That `default` deserves a pause. With `-Wall -Wextra -Werror`, the compiler warns when a `switch` on
an enum omits a case — but only when the switch has no `default`. The inner `default` suppresses the
warning for the most likely mistake in this code: adding a piece and forgetting one of its eight
moves. I verified it: a `Camel` branch with seven cases compiles clean and returns `(0, 0)` for the
eighth. The piece silently does not move on that move.

The dispatch path has a table with the same hazard: an `std::array<Delta, 8>` with seven
initializers zero-fills the eighth, also silently. Keep that in mind when someone claims the
compiler will protect you. It protects the kind switch, not the data inside a rule set.

### Read the archive anchors

In the pinned archive at tag `v1.0.0`:

| Anchor | `variant-01` | What it shows |
|---|---:|---|
| `switch(vecjugadas[i])` | 239 | Dispatch on the move number, not on the piece. |
| `int cf1,cc1,cf2,cc2,cab;` | 29 | The piece is four loose coordinates and an integer. |
| `int cab1,cab2,jugada=1,...` | 18 | Two pieces, two counters, no type. |
| `if (cab==1)` sites | 9 | The piece-kind decision, scattered through `movcab`. |
| `impresionorden` | 753 | The turn loop that names each knight by hand. |
| `cab=1;` / `cab=2;` | 765, 790, 820, 844 | Four turn blocks, two per starting order. |
| `movcab(int &cf, int &cc)` | 234 | The movement function takes coordinates, not a piece. |

The other variants carry the same shape: the switch is at 290 in `variant-02`, 316 in `variant-03`
and 330 in `variant-04`. The pointer variants changed the container; they did not add a piece type.

Count the `cab==1` sites yourself:

```bash
grep -c 'cab==1' original/variant-01-baseline-2008/source/guerra-de-caballos-baseline.cpp
```

Nine in `variant-01`, ten in the others (the records variant adds one in the printer). That number
is the real cost of a third piece in 2008: nine to ten sites, plus the four turn blocks, plus the
globals, plus a movement rule.

### Run the baseline

```bash
./run.sh
```

Expected output:

```text
comparisons=400 mismatches=0
```

The script compiles with:

```text
-std=c++20 -Wall -Wextra -Werror -O2
```

`-Werror` matters: the baseline is warning-clean under the flags it ships with. If your compiler
rejects it, record the compiler and the diagnostic; do not silence the warning to make the run pass.

The run compares the switch path with the dispatch path for every piece, every move and every
position in the 5×5 grid from `-2` to `+2`. The grid exercises positive and negative deltas without
a board.

### Check your prediction

The interface has three virtual declarations, not two: the destructor counts. `delta_with_switch` is
51 lines with two inner switches of eight cases. The run makes 400 virtual calls and 400 switch
calls. The archive's switch is on the move number; the piece is `cab`.

If your prediction said the archive has a piece-kind switch, you found the gap between the catalog's
shorthand and the source. Write it down; it is part of the diagnosis.

## Step 3 — Dispatch (25 min)

### Make the scratch copy

Create a scratch directory outside `original/`. `build/` is ignored by this repository and is a good
place:

```bash
mkdir -p archaeology/lessons/archaeology-10-polymorphism/build/scratch
cd archaeology/lessons/archaeology-10-polymorphism/build/scratch
cp ../../code/polymorphism.cpp polymorphism.cpp
```

From here on, edit `build/scratch/polymorphism.cpp`. Do not edit `code/polymorphism.cpp`; it is the
baseline the next learner will run.

### Freeze the oracle before deleting it

If you delete `delta_with_switch` and then compare dispatch against a table you wrote from the same
deltas, you have compared the code with itself. Zero mismatches would prove nothing.

Capture the switch's output first. Replace the body of `main` with this emission loop, before
removing anything:

```cpp
std::array<PieceKind, 2> kinds = {PieceKind::knight, PieceKind::mirror};

for (int pieceIndex = 0; pieceIndex < 2; pieceIndex += 1)
{
    for (int moveNumber = 1; moveNumber <= 8; moveNumber += 1)
    {
        for (int row = -2; row <= 2; row += 1)
        {
            for (int column = -2; column <= 2; column += 1)
            {
                Position result = move_with_switch(kinds[pieceIndex], moveNumber,
                                                   Position{row, column});
                std::printf("%d %d %d %d %d %d\n", pieceIndex, moveNumber, row, column,
                            result.row, result.column);
            }
        }
    }
}
```

Keep `return 0;` after the loop. Compile and run it, and save stdout as `golden.txt`:

```bash
c++ -std=c++20 -Wall -Wextra -Werror -O2 polymorphism.cpp -o polymorphism-golden
./polymorphism-golden > golden.txt
wc -l golden.txt
```

`wc -l` must print `400`. Each line is one observation: piece index, move number, starting row,
starting column, resulting row, resulting column. That file is now the oracle. The switch is no
longer needed to produce it.

### Remove the switch

In the scratch copy:

1. delete `delta_with_switch` (51 lines) and `move_with_switch` (5 lines);
2. delete the `PieceKind` enum (5 lines), which nothing else needs;
3. keep `Piece`, `Knight`, `MirrorKnight` and the two delta tables;
4. rewrite `main` to keep the two `make_unique` lines and the `pieces` array, read `golden.txt`
   and call `pieces[pieceIndex]->move(...)`;
5. keep the output line `comparisons=400 mismatches=0`;
6. keep the exit code: zero only when there are no mismatches.

A compact reader looks like this:

```cpp
std::FILE *golden = std::fopen("golden.txt", "r");
if (golden == nullptr)
{
    std::printf("cannot open golden.txt\n");
    return 2;
}

int pieceIndex = 0;
int moveNumber = 0;
int row = 0;
int column = 0;
int expectedRow = 0;
int expectedColumn = 0;
while (std::fscanf(golden, "%d %d %d %d %d %d", &pieceIndex, &moveNumber, &row, &column,
                   &expectedRow, &expectedColumn) == 6)
{
    Position actual = pieces[pieceIndex]->move(moveNumber, Position{row, column});
    comparisonCount += 1;
    if (actual.row != expectedRow || actual.column != expectedColumn)
    {
        mismatchCount += 1;
    }
}
std::fclose(golden);
```

The piece index in the trace is now the array index. That is the only thing the trace knows about
the kind, and it is the right amount: the test names a position in a list, not a type.

Compile with the same flags and run from the directory that contains `golden.txt`:

```bash
c++ -std=c++20 -Wall -Wextra -Werror -O2 polymorphism.cpp -o polymorphism
./polymorphism
```

Expected output, unchanged:

```text
comparisons=400 mismatches=0
```

The switch is gone from the scratch copy. The classes remain. The test still passes.

### What you removed

| Removed | Lines |
|---|---:|
| `delta_with_switch` | 51 |
| `move_with_switch` | 5 |
| `PieceKind` | 5 |
| Old `main` body | 33 |

What replaced them: a 400-line text file and about 30 lines of reader. The production code got
smaller; the evidence got bigger. That trade is the point of a frozen oracle.

Do not delete `code/polymorphism.cpp`. The baseline is the artifact that lets the next person
reproduce your comparison.

## Step 4 — Compare (10 min)

### What the run says

```text
comparisons=400 mismatches=0
```

Four hundred observations, zero differences. For every piece, every move 1–8 and every starting
position in the 5×5 grid, the virtual call returns the same `Position` as the frozen switch.

### What equality proves

- The two delta tables in the classes match the deltas hard-coded in the switch.
- The index convention `moveNumber - 1` maps 1–8 to entries 0–7 in both paths.
- Dispatch selects the same rule set the enum selected.
- For these 400 inputs, replacing the switch is behavior-preserving.

### What equality does not prove

| Claim | Proven? | Why |
|---|---|---|
| Dispatch is equivalent for every `int` move number. | No. | Only 1–8 were tested. |
| Out-of-range input behaves like the switch. | No. | The switch returned `(0, 0)`; the table is unchecked. |
| The archive's `movcab` was preserved. | No. | The baseline isolates the delta. |
| A third piece will work. | No. | No third piece is in the trace. |
| Dispatch is faster. | No. | No measurement was made. |
| Dispatch is easier to extend. | By the change test, yes. | Count touched places, not nanoseconds. |

### The oracle direction

The order matters. The switch is the oracle because it is the code that existed. Dispatch is the
candidate because it is the code you are trusting. If you generate both from the same table, the
comparison is circular.

This is the same rule as `archaeology-06`, applied to a different decision: **freeze the old
behavior before you change the structure**.

### When a mismatch appears

Do not "fix" the class until the mismatch is understood. Print the failing observation:

```text
piece=1 move=6 row=-1 column=2 expected=(1,1) actual=(1,-1)
```

The tuple tells you whether the delta is wrong, the sign is wrong, or the index is off by one. A
mismatch is information about the replacement, not a failure of the lesson.

## Step 5 — Explain (15 min)

### The change test, measured

Add a camel to each path in a separate scratch copy. The dispatch addition is a delta table and a
class:

```cpp
constexpr std::array<Delta, 8> camelDeltas = {
    Delta{1, 3}, Delta{3, 1}, Delta{3, -1}, Delta{1, -3},
    Delta{-1, -3}, Delta{-3, -1}, Delta{-3, 1}, Delta{-1, 3},
};

class Camel : public Piece
{
public:
    Position move(int moveNumber, Position position) const override
    {
        const Delta &delta = camelDeltas[moveNumber - 1];
        return Position{position.row + delta.rowChange, position.column + delta.columnChange};
    }

    const char *role_name() const override
    {
        return "camel";
    }
};
```

The switch addition is one enum value and one branch inside `delta_with_switch`. I measured both on
this baseline:

| Measure | Switch path | Dispatch path |
|---|---:|---:|
| Lines added to the rule | 23 | 20 |
| Existing rule declarations edited | 2 | 0 |
| New types | 0 | 1 |
| Harness lines to register it | 3 | 3 |
| Forgot one move: does it compile clean? | Yes, returns `(0, 0)` | Yes, entry is `(0, 0)` |
| Can it answer "what are you called?" | No | Yes, `role_name` |
| Merge conflict with another piece's rule | Yes, same function | No |

Read the first row honestly: the line counts are close. Anyone who argues for dispatch on line count
is arguing from a rounding error. The argument is the other rows.

**Blast radius.** The switch edit lands inside `delta_with_switch`, the function every piece shares.
A concurrent change to the knight's rule conflicts with it in review and in merge. The class edit
lands in a new type that no existing declaration names. Zero existing lines move.

**Open vs closed.** The switch path must add the kind to `PieceKind` and to the shared function. In
the archive it must also add a branch at nine `cab==1` sites and a fourth turn block. The dispatch
path adds a type and one registration line. Both paths still edit the harness array and the loop
bound; that part is a wash, and the bench's fixed arrays exaggerate it.

**Silent omission.** Forget one of the camel's eight moves in the switch and the program compiles
clean with `-Wall -Wextra -Werror`, because the inner `default` swallows the missing case and
returns `(0, 0)`. The piece simply does not move on move 7. Dispatch has the same hazard in its
table, so do not oversell the compiler: what dispatch removes is the *kind* omission, not the
*datum* omission.

**A name and a place.** `role_name` exists in the interface and in both classes. The switch path has
nowhere to put it: the name is not a delta, so it does not belong in `delta_with_switch`. This is
the first sign that a piece is more than its movement rule, and the reason `construction-12` puts
identity and position in the interface as well.

### The real archive cost

The baseline is a bench. The archive is the bill.

| Change for a third piece | Where it lands in 2008 |
|---|---|
| A third coordinate pair and counter | New globals beside `cf1,cc1,cf2,cc2` and `cab1,cab2`. |
| A third value of `cab` | Every `if (cab==1)` site: 9 in `variant-01`, 10 in the others. |
| A third turn block | `impresionorden`, which already has four blocks at 765, 790, 820 and 844. |
| A movement rule | `movcab`, whose switch is on the move number and has no notion of kind. |
| A third printed symbol | `simcab1`, `simcab2` and `impresion0`. |

That is what "adding a piece edits the switch and the game loop" means concretely. It is not one
edit. It is a change to the shape of the program, performed by hand at every site that knows `cab`.

### What a virtual call costs, stated precisely

| Cost | This baseline |
|---|---|
| One vptr per object | `sizeof(Piece)` is 8 bytes. |
| One vtable per class | Two, emitted once in the object file. |
| One indirect call per move | 400 calls in the run. |
| Inlining blocked at the loop | The loop holds `Piece *`; the call stays indirect. |
| Devirtualization possible | Yes, where the compiler proves the type; not in this loop. |
| Heap allocation | Two objects, once. |

The switch costs a jump table or a compare chain, plus the enum check. Neither mechanism is free,
and at 400 calls neither is visible. If you want a number, measure it and record compiler, flags,
CPU and repetitions. If you cannot see the difference above the noise, write "not measured". That
sentence is worth more than an invented percentage.

### Two fixes, two sizes

**Minimal fix — dispatch on kind.** Replace `delta_with_switch` with the `Piece` interface and one
class per kind. Keep the move table inside each class. This is what Step 3 does. It removes the
kind switch and the enum. It does not remove the move table, the board, the globals or the missing
tests.

**Structural fix — a piece that owns its state.** Move position, identity and movement into the
piece, and let the loop hold `Piece &` without coordinates:

```cpp
class Piece
{
public:
    virtual ~Piece() = default;
    virtual Position position() const = 0;
    virtual void set_position(Position position) = 0;
    virtual Position move(int moveNumber) const = 0;
    virtual const char *role_name() const = 0;
};
```

Now the loop does not pass `cf` and `cc` by reference, and `cab` disappears. Track A links:
`construction-11` (encapsulation) for the private state, `construction-12` (polymorphism) for the
interface, and `construction-13` (composition) for the next question: should a piece *have* a
movement rule instead of *being* one?

The archive's `movcab(int &cf, int &cc)` is exactly the signature that this fix removes. The
function mutates two integers the caller owns; the piece owns neither.

### What dispatch does not fix

- The move table. `moveNumber - 1` is still an unchecked index; `move(9, ...)` is undefined
  behavior.
- The `default` that returned `(0, 0)`. If you keep a table, keep a guard or state the precondition.
- The globals. Dispatch in a scratch copy does not remove `vectab`, `jugada` or `ganador`.
- The printing and the turn loop. `impresionorden` still names pieces by hand.
- The missing tests. One frozen trace for one decision is not a test suite.
- The `cab==1` sites in the archive. The lab's bench has none; the original has nine.

### The tradeoff, stated fairly

Dispatch costs an interface, a vtable, an allocation and one more concept to learn. It buys a
boundary: the loop stops enumerating kinds, and a new piece is a new type. In a one-shot program
with two identical pieces, the switch is simpler and there is nothing to extend. In a program that
expects a third piece, the switch charges the next person at every site that knows `cab`.

The lesson asks you to choose with the change test, not with taste. If your context says the game
will never have a third piece, the switch is defensible. Write that context down; it is part of the
diagnosis.

## Step 6 — Record (10 min)

Fill [`EVIDENCE.md`](EVIDENCE.md):

- setup and archive pin;
- the five predictions, written before inspection;
- the interface inventory: three virtual declarations, two overrides per class;
- the switch anchors and the nine `cab==1` sites;
- the baseline run and its exact output;
- the oracle capture: command, `golden.txt`, line count;
- the replacement diff: what was removed, what was kept, what was added;
- the comparison result and the oracle direction;
- the change test: 23 lines in the switch, 20 in the dispatch, and where they land;
- the call cost, with the measurement you did or the sentence "not measured";
- the evidence boundary: what the run proves and what it does not;
- the archive integrity check.

A sentence such as "polymorphism is cleaner" is an opinion. "Three virtual declarations, 400
comparisons, 0 mismatches, 23 lines in the shared function versus 20 in a new type, nine `cab==1`
sites in the original" is a diagnosis.

## The original in detail

Facts you can cite, with line numbers from `variant-01` at tag `v1.0.0`:

| Location | What is there |
|---:|---|
| 18 | `cab1,cab2,jugada,orden,ganador` — two pieces as two counters. |
| 29 | `cf1,cc1,cf2,cc2,cab` — coordinates and the kind as an integer. |
| 234 | `int movcab(int &cf,int &cc)` — movement takes coordinates, not a piece. |
| 239 | `switch(vecjugadas[i])` — dispatch on the move number. |
| 249, 310, 366, 422, 478, 534, 591, 647 | `if (cab==1)` inside the eight case bodies. |
| 702 | `if (i==8&&cab==1)` — the trapped-knight check, also kind-aware. |
| 753 | `void impresionorden()` — the turn loop. |
| 765, 790, 820, 844 | `cab=1;` and `cab=2;` — four turn blocks. |
| 114 | `impresion0` — printing that names each knight. |

The same shape appears in the other variants:

| Variant | `switch` line | `cab==1` sites |
|---|---:|---:|
| `variant-01-baseline-2008` | 239 | 9 |
| `variant-02-records-2008` | 290 | 10 |
| `variant-03-pointers-dev-2008` | 316 | 10 |
| `variant-04-pointers-final-2008` | 330 | 10 |

The pointer variants changed the container, not the decision. That is worth noticing: a syntax
change did not add a piece type.

## What the replacement proves and does not prove

| Claim | Supported? | Why |
|---|---|---|
| Dispatch reproduces the switch for 400 inputs. | Yes | The run printed 0 mismatches. |
| Dispatch is equivalent for all inputs. | No | The test covers 8 moves × 25 positions × 2 pieces. |
| The archive's `movcab` behavior is preserved. | No | The baseline isolates the delta. |
| The replacement removes the kind switch. | Yes | `delta_with_switch` and `PieceKind` are gone. |
| The replacement removes the move-number switch. | No | Each class still indexes its table. |
| A third piece needs zero loop edits. | Yes, in the bench | One registration at the construction site. |
| A third piece needs zero edits in the archive. | No | Nine `cab==1` sites and four turn blocks. |
| Dispatch is faster. | Not measured | Do not claim it. |
| Dispatch removes the unchecked index. | No | `moveNumber - 1` remains. |
| Dispatch removes the globals. | No | They are outside this bench. |

This table is the honesty check. A record that says "the refactor is correct" without naming the
sample is overreaching. Say "for these 400 inputs" when that is what you ran.

## Self-check (binary)

- [ ] I wrote the five predictions before opening the source.
- [ ] I named the three virtual declarations and the two overrides per class.
- [ ] I read `delta_with_switch` and counted its two inner switches.
- [ ] I found the archive's switch at line 239 and confirmed it is on the move number.
- [ ] I counted the `cab==1` sites: 9 in `variant-01`, 10 in the others.
- [ ] I ran `./run.sh` and saw `comparisons=400 mismatches=0`.
- [ ] I recorded the compiler and flags.
- [ ] I copied the baseline into a scratch directory before editing.
- [ ] I captured `golden.txt` from `move_with_switch` before deleting it.
- [ ] `golden.txt` has 400 lines.
- [ ] I removed `delta_with_switch`, `move_with_switch` and `PieceKind`.
- [ ] The dispatch version prints `comparisons=400 mismatches=0`.
- [ ] I added a third piece in both paths and counted the touched places.
- [ ] I explained why the line counts are close and why the edit location still differs.
- [ ] I explained what a virtual call costs, with a measurement or "not measured".
- [ ] I separated the switch on piece kind from the switch on move number.
- [ ] I stated that the baseline reconstructs the kind switch and is not verbatim 2008 code.
- [ ] I noted that the archive's two knights move identically.
- [ ] I noted that the inner `default` hides a missing move.
- [ ] I linked the structural fix to `construction-11`, `construction-12` and `construction-13`.
- [ ] The archive is unmodified (`git status` in the archive is clean).

If a box is not ticked, the unticked box names the next step. There is no penalty for repeating one.

## Misconceptions this lesson refutes

| Misconception | Why it is wrong | What to do instead |
|---|---|---|
| "Polymorphism means inheritance." | Inheritance is one way to share code; dispatch is the point. | Add a type through the interface and count loop edits. |
| "A table is polymorphism." | A table dispatches by index and the loop still enumerates kinds. | Separate the datum decision from the kind decision. |
| "The archive has a switch on piece type." | It has a switch on move number and `if (cab==1)` sites. | Cite line 239 and the nine branches. |
| "Virtual calls are too slow to use here." | 400 calls, one vptr each; the cost is invisible. | Measure, or write "not measured". |
| "The switch is faster, so it is better." | A dense switch is also an indirect branch. | Price change cost, not just cycles. |
| "Dispatch makes the code shorter." | Here it is 20 lines versus 23. | Count touched places and blast radius. |
| "The compiler will catch a missing piece." | `-Wall -Werror` catches the kind switch only without `default`; the inner `default` hides a missing move. | Test the behavior; do not rely on the warning. |
| "Zero mismatches proves the refactor is correct." | It proves equality on 400 sampled inputs with a frozen oracle. | State the sample and the oracle direction. |
| "The baseline is the original rule." | It isolates the delta and invents `MirrorKnight`. | Write the isolation boundary in the record. |
| "Delete the old code, then test the new one." | Without a frozen oracle, the comparison is circular. | Capture the golden trace first. |
| "Dispatch fixes the globals too." | The bench has no board, no turn loop and no printing. | List what is outside the bench. |

## Terms used here

| Term | One-line meaning |
|---|---|
| Polymorphism | One interface, many implementations. |
| Late binding | The dynamic type decides which function runs. |
| Static type | The type the compiler sees at the call site. |
| Dynamic type | The type of the object at run time. |
| Dispatch | Choosing which code runs, by value or by type. |
| vptr | The hidden pointer each polymorphic object stores to its class table. |
| vtable | The per-class table of virtual function addresses. |
| Devirtualization | Replacing an indirect call with a direct one when the type is provable. |
| Pure virtual | A declaration with `= 0`; the interface has no body. |
| Override | An implementation of a virtual function, checked by the compiler. |
| Rule set | The movement geometry that belongs to one kind of piece. |
| Change test | A concrete change whose touched places measure the design. |
| Blast radius | How much existing code a change forces you to touch or re-review. |
| Oracle | The implementation whose behavior you trust while you replace it. |
| Golden trace | A frozen record of outputs used to compare a new implementation. |
| Evidence boundary | The line between what the run proves and what you infer. |

## What “done” means here

The **Self-check (binary)** above is the authoritative checklist. If every box is ticked, the
lesson is done for you. `meta.yml` mirrors the same criteria for tooling.

## Next step

- Continue to `archaeology-11` — input, output and testability. The core still prints from inside
  the logic, and this bench still has no board.
- If the interface felt too small for a real piece, you found the limit that `construction-12`
  discusses: movement in the interface is enough for a delta, not for a game.
- If you want the harder version, add a piece whose move depends on the board. The table cannot hold
  it; the interface can, if you change the signature.
- If you disagree with the severity, write the context that changes it. A two-piece demo and a game
  with a planned expansion do not price the same switch.

## References

- [`README.md`](README.md) — the lesson card, the build command and the acceptance criterion.
- [`code/polymorphism.cpp`](code/polymorphism.cpp) — the baseline: interface, implementations,
  switch and comparison.
- [`run.sh`](run.sh) — compiles with `-std=c++20 -Wall -Wextra -Werror -O2` and runs the baseline.
- [`EVIDENCE.md`](EVIDENCE.md) — the portfolio record you fill in.
- [`../../../docs/guide/archaeology-track.md`](../../../docs/guide/archaeology-track.md) — §3, the
  failure taxonomy; §5, the severity scale; §9, the Track A mapping.
- [`../../../docs/guide/lesson-catalog.md`](../../../docs/guide/lesson-catalog.md) — the lesson
  sequence and the `archaeology-10` acceptance criterion.
- [`../../../docs/archive.md`](../../../docs/archive.md) — the pinned archive, tag and commit.
- [archaeology-06](../archaeology-06-duplication-switch/lesson.md) — the move-number switch and its
  table.
- [archaeology-09](../archaeology-09-abstract-data-type/README.md) — the opaque type.
- [construction-12](../../../construction/lessons/construction-12-polymorphism/lesson.md) — the
  modern polymorphism lesson and the cost of late binding.
- Archive: <https://github.com/DaydreamsDev/DaydreamsDev-guerra-de-caballos-archive>, tag `v1.0.0`,
  commit `284e144e41403afbf0cc44422876425cacfb2bd5`.
