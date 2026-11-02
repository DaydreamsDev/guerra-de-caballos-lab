# archaeology-07 — Representation leakage and parallel arrays

> [English](lesson.md) | [Español](lesson-es.md)

> **Status:** drafted · **Concept:** representation leakage and records · **Prerequisite:**
> archaeology-05 · **Estimated time:** 60 minutes
> **Reading:** ~18 minutes
> **Lesson role:** optional · safe to skip

## Why this lesson exists

`archaeology-04` diagnosed failures that produce a wrong result. `archaeology-05` diagnosed state
that no function owns. `archaeology-06` diagnosed one rule copied eight times. This lesson is about
a quieter defect: the program works, and the shape of its data is public.

In the 2008 baseline, a knight's position is stored twice. `cab1` and `cab2` hold flat board
indices from `0` to `63`. `cf1`, `cc1`, `cf2` and `cc2` hold the same two positions as row and
column. `conversionvm` writes the row/column copy from the flat index; `conversionmv` converts a
row and a column back to a flat index. Nothing enforces that the two copies agree. The caller keeps
them in sync by hand:

```cpp
cab1 = movcab(cf1, cc1);
```

One line updates `cf1` and `cc1` through reference parameters and writes the returned flat index
into `cab1`. Two representations, two update mechanisms, one assignment. If the function returns
early, or a future edit updates one and not the other, the position drifts.

The board has the same problem. `vectab` is `int[64]`, and every caller indexes it directly:
`vectab[cab1]`, `vectab[ccab]`, `vectab[k]`. The formula `cf * 8 + cc` is not hidden behind an
operation; it is the contract. A caller that reads a cell must know that the board is a flat array
and must convert its coordinates with the same convention.

This is a **design** failure, not a correctness failure. The taxonomy in
[`archaeology-track.md`](../../../docs/guide/archaeology-track.md) names it: the program works, but
the structure makes change expensive. The catalog lists it as `failure-009` (parallel arrays and
loose knight variables) and `failure-010` (a wrapper that is not an abstract data type).

The product is a **record and an owner**: a `Knight` that groups row and column, and a `Board` that
owns its visited cells and exposes `mark` and `visited_count`. The proof is a run: the same
scenario, the same summary, `match=yes`.

## What you will be able to do

By the end of this lesson you can:

- tell a **record** from a bag of parallel variables, and say what the record makes impossible;
- name every place where the 2008 code exposes its board representation;
- explain why two representations of one position are a drift risk, not a convenience;
- count the callers that must change when the board size or internal index formula changes;
- introduce `Knight` and `Board` in a scratch copy and update the caller;
- compare the before and after programs and state exactly what `match=yes` proves;
- state the isolation boundary: the baseline isolates the record/board change, not the archive;
- link the repair to Track A rungs without confusing records with abstract data types;
- keep the archive read-only while you work.

The refactor is done when someone else can reproduce the run.

## The concept: representation leakage and records

### Two representations of one position

The 2008 code stores a knight twice:

| Representation | Names | Range | Produced by | Consumed by |
|---|---|---|---|---|
| Flat board index | `cab1`, `cab2` | `0..63` | `rand() % 64` | `vectab[cab1]`, `vectab[cab2]` |
| Row and column | `cf1`, `cc1`, `cf2`, `cc2` | `0..7` each | `conversionvm` | `movcab(cf1, cc1)` |

`conversionvm` is four assignments:

```cpp
cf1 = cab1 / 8;
cc1 = cab1 % 8;
cf2 = cab2 / 8;
cc2 = cab2 % 8;
```

`conversionmv` is one expression:

```cpp
ccab = cf * 8 + cc;
```

The pair is a hand-maintained cache. The source does not say which one is authoritative. Sometimes
the flat index is: `ordenjuego` writes `vectab[cab1] = jugada`. Sometimes the row/column pair is:
`movcab` checks bounds with `cf` and `cc`. The reader has to decide, per call site, which copy is
current.

### Parallel variables

A record groups fields that belong together. Parallel variables are the opposite: they belong
together but are declared apart.

```cpp
int playerRow = 1;
int playerColumn = 1;
int machineRow = 8;
int machineColumn = 8;
```

Nothing in the language stops a future edit from writing `playerRow = 2` and leaving
`playerColumn` at `1`. Nothing stops a call from mixing the player's row with the machine's column.
The invariant — *a position is a row and a column that move together* — exists only in the
programmer's head.

The 2008 file has the same shape at a larger scale: four loose coordinate integers, two flat
indices, a turn selector, and a board array. The type system sees seven independent integers and a
board array. The domain sees two knights and a board.

### What leaks

**Representation leakage** means a caller must know how a value is stored in order to use it.

In `parallel_arrays.cpp`, the caller computes the board index three times:

```cpp
visited[(playerRow - 1) * 8 + (playerColumn - 1)] = true;
```

That expression encodes four decisions the caller should not own:

- the board is eight columns wide;
- the storage is row-major;
- coordinates are one-based at the call site;
- the visited set is a flat array of 64 cells.

Change any one of them and the caller changes. The board cannot swap its representation without
editing every call site.

### The record and the owner

The refactor has two moves.

1. **Record.** `Knight` groups `row` and `column`. A position becomes one value that can be
   assigned, passed and compared as a unit.
2. **Owner.** `Board` owns the visited array. It exposes `mark(const Knight &)` and
   `visited_count()`. The index formula moves inside `mark`, where it belongs.

The caller becomes:

```cpp
board.mark(player);
player = Knight{2, 3};
board.mark(player);
```

No index, no width, no origin, no array. The caller states the domain operation.

### What a record buys, and what it does not

A record buys:

- **One place to change.** Add a field to `Knight`, and the field travels with the value.
- **An invariant you can name.** A position is one object, not four variables.
- **A testable unit.** You can construct a `Knight` and compare it without a board.
- **A boundary.** `Board` can change its array to a bitset, a `std::vector` or a 10×10 grid, and
  callers still compile.

A record does not buy:

- **Validation.** `Knight{99, 99}` is still constructible. Range checks need an invariant
  (`construction-10`) or a private constructor (`construction-11`).
- **Ownership.** Grouping row and column does not give the board an owner. `Board` does that.
- **A smaller program.** The record version is longer: 67 lines against 47.

The length is the price. The lesson asks you to price the change, not to count lines for their own
sake.

### Why this is a design failure

Severity is medium in the track's scale: expensive change, fragile structure, fixed on a schedule.
The program does not cause undefined behavior by itself. The undefined behavior in the archive is
the `i <= 64` loop, and that belongs to `archaeology-04`. What the representation causes is:

- **Change amplification.** A board resize touches every index expression.
- **Silent drift.** Two copies of one position can disagree, and no compiler sees it.
- **Review cost.** A reviewer must reconstruct the index convention to read a diff.
- **Test cost.** A unit test must build the representation, not just the domain value.
- **Knowledge cost.** A new reader must learn `8`, `64`, one-based coordinates and the two
  conversion functions before touching a move.

## Before you start

Read these in full:

- [`README.md`](README.md) — the lesson card, the build command and the acceptance criterion.
- [archaeology-05](../archaeology-05-global-state/lesson.md) — the prerequisite: the globals, the
  derived cache `cf1`/`cc1`/`cf2`/`cc2`, and the mutation map.
- [`../../../docs/guide/archaeology-track.md`](../../../docs/guide/archaeology-track.md) — §3, the
  failure taxonomy; §5, the severity scale; §9, the Track A mapping.
- [`../../../docs/archive.md`](../../../docs/archive.md) — the pinned archive, tag and commit.

You also need:

- a local clone of the archive at tag `v1.0.0`;
- a C++20 compiler (`c++`, `g++` or `clang++`);
- `bash`;
- a scratch directory outside `original/`.

> **Rule:** the archive is read-only. Copy the file you want to change into a scratch directory;
> never edit `original/`. The baseline in this lesson is also evidence: copy it before you edit it.

## The 60-minute path

| Min | Step | What you do | Evidence |
|---|---|---|---|
| 0–10 | Predict | Estimate the index sites, the coupled variables and the resize cost | Prediction table |
| 10–25 | Inspect | Read the archive anchors and run the baseline | Anchor notes and `match=yes` |
| 25–40 | Refactor | Introduce `Knight` and `Board` in a scratch copy; update the caller | Scratch file and `golden.txt` |
| 40–45 | Compare | Run the refactor against the frozen summary | Same line, `match=yes` |
| 45–55 | Explain | Price the leakage and name the class | Change test and cost table |
| 55–60 | Record | Fill `EVIDENCE.md`; confirm the archive is clean | Portfolio entry |

If counting the `vectab[` sites takes ten minutes, that count is the
lesson.

### The engineering check in this lesson

Track B uses the same habit as Track A, with a different verb: the behavior already exists, so you
**freeze** it instead of specifying it from scratch.

| Step | Where it happens | What it produces |
|---|---|---|
| **Predict** | Step 1 | Falsifiable counts for index sites and resize cost. |
| **Inspect** | Step 2 | The archive anchors and the baseline run. |
| **Refactor** | Step 3 | A scratch copy with `Knight` and `Board`. |
| **Compare** | Step 4 | The same summary from both versions. |
| **Explain** | Step 5 | The change test and the cost of leakage. |
| **Record** | Step 6 | `EVIDENCE.md` with counts, diff and result. |

## Step 1 — Predict (10 min)

Before you open the source, answer these four questions in writing. The point is to commit to a
number you can be wrong about.

1. How many times does `parallel_arrays.cpp` compute a board index by hand?
2. How many variables must change together to move one knight?
3. If the board becomes 10×10, how many lines change in each version?
4. What does `Knight` hide, and what does it not hide?

A useful prediction is falsifiable. "Parallel arrays are bad" is not. "The caller computes the index
three times, moving a knight touches two integers, and a 10×10 board touches four lines in the
parallel version and two inside `Board`" is.

### A worked prediction

Before reading, I would write:

- index sites: three, one per mark;
- coupled variables: two per knight, row and column;
- resize cost: the array size and three index expressions in the parallel version; the array size
  and one formula inside `Board` in the record version;
- what `Knight` hides: that a position is two integers; what it does not hide: the valid range, the
  board width, or the storage.

Every claim can be checked. That is the standard. If your prediction says "several", you have not
predicted anything yet; the source already told you there are four coordinate variables.

## Step 2 — Inspect (15 min)

### The archive anchors

Open `original/variant-01-baseline-2008/source/guerra-de-caballos-baseline.cpp` at tag `v1.0.0`.
The file is 930 lines. Use this table to navigate; the line numbers are from the pinned archive.

| Line | What is there |
|---:|---|
| 17 | `int vectab[64],vecjugadas[8];` — the flat board. |
| 18 | `int cab1,cab2,jugada=1,orden,ganador=1;` — flat knight indices. |
| 29 | `int cf1,cc1,cf2,cc2,cab;` — the row/column cache and turn selector. |
| 56–66 | `inicializacion` — clears the board. |
| 61–63 | `for (i=0;i<=64;i++) vectab[i]=0;` — the off-by-one from `archaeology-04`. |
| 68–101 | `cabubicacion` — random placement. |
| 81–82 | `cab1=rand()%64; cab2=rand()%64;` — flat indices are born here. |
| 90 | `conversionvm();` — the row/column cache is derived once. |
| 104–112 | `conversionvm` — writes `cf1`, `cc1`, `cf2`, `cc2`. |
| 226–232 | `conversionmv` — `ccab=cf*8+cc`. |
| 234 | `movcab(int &cf,int &cc)` — mutates row/column, returns a flat index. |
| 247 | `ccab=conversionmv(cf-1,cc+2);` — a target cell computed by hand. |
| 252 | `vectab[ccab]==0||vectab[ccab]==vectab[cab2]` — the move logic reads the flat board. |
| 269 | `vectab[ccab]=jugada;` — the board stores the move number. |
| 726–751 | `ordenjuego` — writes `vectab[cab1]` and `vectab[cab2]`. |
| 766 | `cab1=movcab(cf1,cc1);` — two representations updated in one line. |
| 791 | `cab2=movcab(cf2,cc2);` — the same pattern for the other knight. |
| 821 | `cab2=movcab(cf2,cc2);` — repeated under the other turn order. |
| 846 | `cab1=movcab(cf1,cc1);` — repeated again. |
| 870–930 | `impresion` — walks `vectab[k]` and compares with `vectab[cab1]`. |

Four counts are worth taking:

| Measure | Count |
|---|---:|
| Lines containing `vectab[` | 80 |
| Lines containing `conversionmv(` | 26 |
| `conversionmv` calls (excluding prototype and definition) | 24 |
| `movcab` call sites | 4 |

The counts are mentions, not distinct decisions. They tell you where the representation is exposed.
Read the sites to see what each one assumes.

### The dual representation in one line

Line 766 is the whole lesson:

```cpp
cab1=movcab(cf1,cc1);
```

- `cf1` and `cc1` are passed by reference; the callee mutates them.
- `cab1` receives the return value, which is the new flat index.
- The caller now holds two copies of the same position, updated by two mechanisms.

If every move is rejected, `cf` and `cc` keep the old position, but the function still returns the
last target `ccab` it computed. The caller stores that value in `cab1`. The source does not state
the postcondition. That is what a record and an owner remove: one value, one update.

### The baseline

The baseline is two small programs, not the archive. Read them in this order.

`code/parallel_arrays.cpp` is 47 lines:

| Part | Lines | What it is |
|---|---:|---|
| `Summary` | 4–11 | Five loose integers for the output. |
| `run_scenario` | 13–38 | The caller. |
| Knight variables | 15–18 | Four coordinate integers. |
| Board | 19 | `std::array<bool, 64>`. |
| Mark sites | 21, 24, 27 | Three hand-computed indices. |
| Count loop | 29–36 | Scans the array. |
| `main` | 40–47 | Prints the summary. |

`code/record.cpp` is 67 lines:

| Part | Lines | What it is |
|---|---:|---|
| `Knight` | 4–8 | A row and a column. |
| `Board` | 10–33 | The owner. |
| `mark` | 13–16 | Converts a `Knight` to an index, inside the class. |
| `visited_count` | 18–29 | Scans the private array. |
| Private array | 31–32 | `std::array<bool, 64>`. |
| `Summary` | 35–42 | The same output shape. |
| `run_scenario` | 44–58 | The caller, now record-shaped. |
| `main` | 60–67 | Prints the same summary. |

### Run the baseline

From this lesson's directory:

```bash
cd archaeology/lessons/archaeology-07-representation-leakage
./run.sh
```

Expected output:

```text
parallel=player=2,3 machine=3,4 visited=3
record=player=2,3 machine=3,4 visited=3
match=yes
```

The script compiles both files with:

```text
-std=c++20 -Wall -Wextra -Werror -O2
```

`-Werror` matters: the baseline is warning-clean under the flags it ships with. If your compiler
rejects it, record the compiler and the diagnostic; do not silence the warning to make the run pass.

### Read the baseline honestly

The baseline is a bench, not the game. It isolates two changes:

1. four loose coordinate integers become two `Knight` records;
2. the caller stops computing board indices; `Board::mark` does it.

It does not reproduce:

- the archive's `cab1` flat index and `cf1`/`cc1` row/column cache stored at the same time;
- the archive's board payload: `vectab` stores move numbers, the baseline stores `bool`;
- random placement, turn selection, capture, printing or the game loop;
- the `i <= 64` off-by-one, the globals, or the eight-case switch.

Say that in your record. A passing run is proof about the bench, not about the archive.

### Check your prediction

The parallel version computes an index at lines 21, 24 and 27: three sites. Moving a knight touches
two integers. A 10×10 board changes four lines in the parallel version (the array size and three
index formulas) and two lines inside `Board` (the array size and the formula). The caller does not
change in the record version. That is the design argument in one measurement.

## Step 3 — Refactor (15 min)

### Freeze the output first

Before you change anything, capture the parallel version's output:

```bash
cd archaeology/lessons/archaeology-07-representation-leakage
mkdir -p build/scratch
./build/parallel-arrays > build/scratch/golden.txt
cat build/scratch/golden.txt
```

Expected:

```text
player=2,3 machine=3,4 visited=3
```

That line is the oracle. If you refactor first and then compare against a summary you wrote from the
new code, the comparison is circular.

### Copy the baseline

```bash
cp code/parallel_arrays.cpp build/scratch/record_attempt.cpp
```

From here on, edit `build/scratch/record_attempt.cpp`. Do not edit `code/parallel_arrays.cpp` or
`code/record.cpp`; they are the before and after references that the next learner will run.

### Introduce the record

Replace the four coordinate integers with two values:

```cpp
struct Knight
{
    int row;
    int column;
};
```

Then the scenario starts:

```cpp
Knight player{1, 1};
Knight machine{8, 8};
```

Moving a knight becomes one assignment instead of two:

```cpp
player = Knight{2, 3};
```

The compiler now sees one value. You cannot update the row and forget the column.

### Give the board an owner

Replace the public array with a class:

```cpp
class Board
{
public:
    void mark(const Knight &knight)
    {
        visited[(knight.row - 1) * 8 + (knight.column - 1)] = true;
    }

    int visited_count() const
    {
        int count = 0;
        for (bool cell : visited)
        {
            if (cell)
            {
                count += 1;
            }
        }
        return count;
    }

private:
    std::array<bool, 64> visited = {};
};
```

The formula is still there. It is now in one place, behind an operation. That is the difference
between hiding a representation and deleting it. The representation still exists; the caller no
longer depends on it.

### Update the caller

The caller is `run_scenario`. In this baseline there is exactly one. The archive has several places
that touch the representation:
`ordenjuego`, `impresion`, `movcab` and the four `movcab` call sites in `impresionorden`.

| Call site in `parallel_arrays.cpp` | Replacement in `record.cpp` |
|---|---|
| Four declarations, lines 15–18 | Two `Knight` values and one `Board`, lines 46–48 |
| `visited[(playerRow-1)*8+(playerColumn-1)] = true;` line 21 | `board.mark(player);` line 50 |
| `playerRow = 2; playerColumn = 3;` lines 22–23 | `player = Knight{2, 3};` line 51 |
| `visited[(playerRow-1)*8+(playerColumn-1)] = true;` line 24 | `board.mark(player);` line 52 |
| `machineRow = 3; machineColumn = 4;` lines 25–26 | `machine = Knight{3, 4};` line 53 |
| `visited[(machineRow-1)*8+(machineColumn-1)] = true;` line 27 | `board.mark(machine);` line 54 |
| Inline count loop, lines 29–36 | `board.visited_count()` line 57 |
| `Summary{playerRow, playerColumn, machineRow, machineColumn, visitedCount}` line 37 | `Summary{player.row, player.column, machine.row, machine.column, board.visited_count()}` lines 56–57 |

That is the "callers updated" evidence: three index computations and one summary construction
become three domain calls and one method call. Count the lines you touched; do not describe the
change as "cleaner" without a number.

### Compile and run

```bash
c++ -std=c++20 -Wall -Wextra -Werror -O2 \
    build/scratch/record_attempt.cpp -o build/scratch/record_attempt
./build/scratch/record_attempt
```

Expected:

```text
player=2,3 machine=3,4 visited=3
```

Compare with the frozen file:

```bash
diff <(./build/scratch/record_attempt) build/scratch/golden.txt
```

No output means the lines match. Then compare with the shipped record version:

```bash
diff <(./build/scratch/record_attempt) <(./build/record)
```

No output means your refactor and `code/record.cpp` agree on this scenario.

### The optional change test

In a second scratch copy, change the board to 10×10 and count the touched lines.

| Change | Parallel version | Record version |
|---|---|---|
| Board 8×8 → 10×10 | Array size plus three index formulas | Array size plus one formula inside `Board` |
| Add a third knight | Two new integers plus one index formula per mark | One new `Knight` plus one `mark` call per mark |
| Change the internal index formula | Three formulas at the call sites | One formula inside `Board` |
| Add a `captured` flag to a knight | One new integer, passed separately | One new field in `Knight` |

Do not claim a line count you did not measure. The table is a prediction until you run it.

## Step 4 — Compare (5 min)

### What the run says

```text
parallel=player=2,3 machine=3,4 visited=3
record=player=2,3 machine=3,4 visited=3
match=yes
```

Both programs place the player at `(2,3)`, the machine at `(3,4)` and count three visited cells. The
`match=yes` line is produced by `run.sh` only when the two output strings are equal.

### What equality proves

- The record version performs the same three marks as the parallel version for this scenario.
- `Knight` carries the same row and column values through the same sequence.
- `Board::mark` computes the same cells as the hand-written index expressions.
- `visited_count()` returns the same count as the inline loop.
- The caller no longer needs the board's width, index formula or storage to express the scenario.

### What equality does not prove

| Claim | Proven? | Why |
|---|---|---|
| The record version is equivalent for every position. | No. | One scenario, three marks. |
| The archive's behavior is preserved. | No. | The baseline is not the archive. |
| `Board::mark` rejects out-of-range coordinates. | No. | It has no bounds check. |
| The archive's `cab1`/`cf1` dual storage is reproduced. | No. | The baseline stores one representation. |
| The archive's board payload is preserved. | No. | `bool` here, move number there. |
| The record version is faster. | No. | No measurement was made. |
| The record version is shorter. | No. | 67 lines against 47. |

### The oracle direction

The parallel version is the "before"; the record version is the "after". Both were written for this
lesson. Neither is the 2008 program. The archive is the historical witness; the baseline is the
bench.

This matters because a comparison between two programs you wrote can agree for the wrong reason.
Freeze the before output first, then refactor. Do not generate the golden line from the record
version and call the match a proof.

## Step 5 — Explain (10 min)

### Price the leakage

| Cost | Evidence in the archive |
|---|---|
| The board width is copied into every index expression. | 80 lines contain `vectab[`; `conversionmv` hard-codes `8`. |
| Two copies of one position must be kept in sync. | `cab1` and `cf1`/`cc1`; `conversionvm` writes one from the other. |
| The update mechanism is split. | `cab1=movcab(cf1,cc1)` mutates by reference and returns a flat index. |
| A caller must know the storage to read a cell. | `impresion` walks `vectab[k]` and compares with `vectab[cab1]`. |
| A board resize touches every caller. | `conversionmv(cf,cc)` is called at 24 sites. |
| A new reader must learn the convention first. | `8`, `64`, zero-based rows and columns, one-based printing (`cf1+1`). |

### The class, not the instance

The instance is "four loose integers and a flat array". The class is "a value whose representation
is public". Look for the class elsewhere in the archive:

- `ordenjuego` writes `vectab[cab1]` and `vectab[cab2]` directly — placement knows the storage;
- `impresion` compares `vectab[k]` with `vectab[cab1]` — printing knows the storage;
- `movcab(int &cf,int &cc)` returns a flat index while mutating row and column — one contract, two
  representations;
- `struct stablero { int mvectab; }` in variant-03 and variant-04 is a transparent wrapper around
  one integer — a type that hides nothing (`failure-010`, the next lesson);
- the magic numbers `64` and `8` appear in initialization, conversion and bounds checks.

The systemic fix is not "replace these four variables". It is "give the value an owner and expose
operations, not storage". That fix removes the class.

### Two fixes, two sizes

**Minimal fix — the record.** Introduce `Knight` and keep the flat array public:

```cpp
struct Knight
{
    int row;
    int column;
};
```

The caller still computes `(knight.row - 1) * 8 + (knight.column - 1)`, but the two coordinates can
no longer drift apart. This is the smallest change that removes the parallel-variable half of the
defect.

**Structural fix — the owner.** Introduce `Board` with a private array and `mark(Knight)`. The
caller stops indexing. This is the baseline's `record.cpp`. It removes the leakage half.

**Stronger fix — the invariant.** Give `Knight` a validity check and `Board::mark` a bounds check,
so an invalid position is rejected instead of written out of bounds. That is `construction-10`
(invariants) and `construction-11` (encapsulation), not this lesson.

Track A links: `construction-04` (records) for `Knight`, `construction-05` (array of records) for
the board's cells, `construction-09` (abstract data type) for representation independence, and
`construction-11` (encapsulation) for the private array.

Do not cite `construction-09` as if the baseline already achieves it. `Board` hides the array, but
the type is still a class with a narrow interface; a full abstract data type also fixes the
invariant and the lifecycle. Say which rung you reached.

### The tradeoff, stated fairly

The record costs new types, construction syntax and more lines. The owner costs an interface and a
place to put the bounds check. The parallel version costs every future change to the board.

In a one-shot program with no tests, four integers and a flat array are easy to read. In a
maintained program with a second caller, the record and the owner are cheaper. The lesson asks you
to choose with the change test, not with taste.

### What the fix does not fix

- `Board` still stores `std::array<bool, 64>`; the representation is hidden, not eliminated.
- `Board::mark` has no bounds check. `mark(Knight{0, 0})` computes index `-9` and writes out of
  bounds. The archive's `movcab` checks bounds before converting; the baseline does not.
- The one-based convention is still inside `Board`.
- `Summary` still has five parallel fields. It is an output DTO, not the domain model; do not
  confuse the two.
- The archive's `cab1`/`cf1` dual storage is not reproduced; the baseline stores one representation.
- The globals, printing, turn selection, random placement, capture and game loop are absent.
- The `i <= 64` off-by-one remains a separate correctness card from `archaeology-04`.

## Step 6 — Record (5 min)

Fill [`EVIDENCE.md`](EVIDENCE.md):

- setup and archive pin;
- the prediction table, written before inspection;
- the archive anchors and the four counts;
- the baseline run and its exact output;
- the frozen `golden.txt` and the refactor diff;
- the caller table: what changed at each site;
- the comparison result and the oracle direction;
- the evidence boundary: what the run proves and what it does not;
- the archive integrity check.

A sentence such as "the record is cleaner" is an opinion. "Three index sites, 80 `vectab[` mentions,
24 `conversionmv` calls, one caller updated, 67 lines against 47, `match=yes`" is a diagnosis.

## The original in detail

Facts you can cite, with line numbers from `variant-01` at tag `v1.0.0`:

| Location | What is there |
|---:|---|
| 17 | `int vectab[64],vecjugadas[8];` — flat board and move order. |
| 18 | `int cab1,cab2,jugada=1,orden,ganador=1;` — flat knight indices. |
| 29 | `int cf1,cc1,cf2,cc2,cab;` — row/column cache and turn selector. |
| 61–63 | `for (i=0;i<=64;i++) vectab[i]=0;` — the off-by-one. |
| 81–82 | `cab1=rand()%64; cab2=rand()%64;` — flat indices are created. |
| 90 | `conversionvm();` — the cache is derived once. |
| 106–109 | `cf1=cab1/8; cc1=cab1%8; cf2=cab2/8; cc2=cab2%8;` |
| 229–230 | `ccab=cf*8+cc; return ccab;` — the inverse conversion. |
| 234 | `int movcab(int &cf,int &cc)` — mutates row/column, returns flat. |
| 247 | `ccab=conversionmv(cf-1,cc+2);` — a target cell. |
| 252 | `vectab[ccab]==0||vectab[ccab]==vectab[cab2]` — move logic reads storage. |
| 269 | `vectab[ccab]=jugada;` — the board stores the move number. |
| 732, 734 | `vectab[cab1]=jugada; vectab[cab2]=jugada;` — placement writes storage. |
| 766 | `cab1=movcab(cf1,cc1);` — two representations, one line. |
| 791, 821, 846 | The same call under the other turn orders. |
| 884 | `vectab[k]!=vectab[cab1]&&vectab[k]!=vectab[cab2]` — printing reads storage. |
| 878–920 | `impresion` walks the flat array with `k++`. |

The counts in this lesson are line mentions, not distinct semantic decisions. A line may contain
two `vectab[` reads, and a `conversionmv` call may compute the same target twice. Measure your own
copy if you need exact numbers.

## What the refactor proves and does not prove

| Claim | Supported? | Why |
|---|---|---|
| The record version produces the same summary for this scenario. | Yes | The run printed `match=yes`. |
| The record version is equivalent for every board and position. | No | One scenario, three marks. |
| The caller no longer computes the board index. | Yes | The index is inside `Board::mark`. |
| The archive's behavior is preserved. | No | The baseline is not the archive. |
| `Board` hides its storage from callers. | Yes | The array is private. |
| `Board` eliminates the flat representation. | No | It still stores `std::array<bool, 64>`. |
| `Board::mark` validates coordinates. | No | There is no bounds check. |
| The record version is shorter. | No | 67 lines against 47. |
| The record version is faster. | Not measured | Do not claim it. |
| The change test is cheaper with the record. | By line count | Count the touched lines yourself. |

This table is the honesty check. An evidence entry that says "the refactor is correct" without
naming the sample is overreaching. Say "for this scenario" when that is what you ran.

## Self-check (binary)

- [ ] I wrote the four predictions before opening the source.
- [ ] I found the declarations at lines 17, 18 and 29.
- [ ] I read `conversionvm` and `conversionmv` and explained the two representations.
- [ ] I counted the `vectab[` mentions: 80.
- [ ] I counted the `conversionmv(` mentions: 26 lines, 24 calls.
- [ ] I found the four `movcab` call sites: 766, 791, 821, 846.
- [ ] I ran `./run.sh` and saw `match=yes`.
- [ ] I recorded the compiler and flags.
- [ ] I froze `golden.txt` before refactoring.
- [ ] I introduced `Knight` in a scratch copy.
- [ ] I introduced `Board` with a private array and `mark`.
- [ ] I updated the caller and counted the sites.
- [ ] The scratch refactor printed the same summary.
- [ ] I diffed my refactor against `code/record.cpp`.
- [ ] I stated the isolation boundary: record/board change only, not the archive.
- [ ] I noted that `Board` still stores `std::array<bool, 64>`.
- [ ] I noted that `Board::mark` has no bounds check.
- [ ] I linked the fix to `construction-04`, `construction-05`, `construction-09` and
      `construction-11`.
- [ ] I did not claim a performance improvement.
- [ ] The archive is unmodified (`git status` in the archive is clean).

If a box is not ticked, the unticked box names the next step. There is no penalty for repeating one.

## Misconceptions this lesson refutes

| Misconception | Why it is wrong | What to do instead |
|---|---|---|
| "A record is just a bag of variables." | A record groups fields so they move as one value; the bag does not. | Name the invariant the grouping enforces. |
| "Parallel arrays are fine if I am careful." | Care is not a compiler check; two copies can drift. | Give the value a type and the storage an owner. |
| "The record version is shorter." | It is 67 lines against 47. | Count the change cost, not the line count. |
| "`Board` removes the flat array." | It still stores `std::array<bool, 64>`. | Say "hides", not "eliminates". |
| "`match=yes` proves the archive still works." | The baseline is not the archive. | State the isolation boundary. |
| "`match=yes` proves equivalence for all positions." | One scenario, three marks. | State the sample. |
| "The record version is faster." | No measurement was made. | Argue with touched lines, not cycles. |
| "`Knight` validates the position." | `Knight{99, 99}` is constructible. | Add an invariant or a private constructor. |
| "The archive's `cab1` and `cf1` are independent." | They are two copies of one position. | Mark the derived cache and its sync points. |
| "A class with a private array is a full abstract data type." | Hiding storage is one property; invariants and lifecycle are others. | Say which rung you reached. |
| "The fix should be applied to `original/`." | The archive is evidence and must stay read-only. | Refactor in a scratch copy. |

## Terms used here

| Term | One-line meaning |
|---|---|
| Representation | How a value is stored: flat index, row/column, array, bitset. |
| Representation leakage | A caller must know the storage to use the value. |
| Parallel arrays | Separate variables or arrays that must be kept in sync by hand. |
| Record | A type that groups fields into one value. |
| Owner | The type responsible for a piece of state and its invariant. |
| Encapsulation | Hiding state behind operations. |
| Abstract data type | A type defined by its operations, independent of its representation. |
| Invariant | A property that must always hold, for example `1 <= row <= 8`. |
| Change test | A concrete change whose touched places measure the design. |
| Derived cache | A second copy of a value, kept in sync by hand. |
| Isolation boundary | The line between what the baseline reproduces and what it omits. |
| Design failure | The program works, but the structure makes change expensive. |

## What “done” means here

The **Self-check (binary)** above is the authoritative checklist. If every box is ticked, the
lesson is done for you. `meta.yml` mirrors the same criteria for tooling.

## Next step

- Continue to `archaeology-08` — missing module boundary. The board is now an owner inside one
  translation unit; the next question is what a header would expose.
- If the record was easy but the caller update was not, you have found the difference between
  grouping data and hiding representation. Write one sentence about which fix you would ship first.
- If you want the stronger invariant, go to `archaeology-09` and `construction-10`. Do not conflate
  the private array with a validated type.
- If you disagree with the severity, write the context that changes it. A one-shot demo and a
  maintained game do not price the same leakage.

## References

- [`README.md`](README.md) — the lesson card, the build command and the acceptance criterion.
- [`code/parallel_arrays.cpp`](code/parallel_arrays.cpp) — the before: loose coordinates and a flat
  board.
- [`code/record.cpp`](code/record.cpp) — the after: `Knight` and `Board`.
- [`run.sh`](run.sh) — compiles with `-std=c++20 -Wall -Wextra -Werror -O2` and compares the output.
- [`EVIDENCE.md`](EVIDENCE.md) — the portfolio record you fill in.
- [`../../../docs/guide/archaeology-track.md`](../../../docs/guide/archaeology-track.md) — §3, the
  failure taxonomy; §5, the severity scale; §9, the Track A mapping.
- [`../../../docs/guide/lesson-catalog.md`](../../../docs/guide/lesson-catalog.md) — the lesson
  sequence and the `archaeology-07` acceptance criterion.
- [`../../../docs/archive.md`](../../../docs/archive.md) — the pinned archive, tag and commit.
- [archaeology-05](../archaeology-05-global-state/lesson.md) — the prerequisite: the globals and
  the derived row/column cache.
- [archaeology-06](../archaeology-06-duplication-switch/lesson.md) — the previous lesson:
  duplication and extraction.
- Archive: <https://github.com/DaydreamsDev/DaydreamsDev-guerra-de-caballos-archive>, tag `v1.0.0`,
  commit `284e144e41403afbf0cc44422876425cacfb2bd5`.
