# failure-009 — A knight's position is stored twice in parallel variables

> [English](failure-009-parallel-arrays.md) | [Español](failure-009-parallel-arrays-es.md)

- **Class:** design
- **Severity:** medium
- **Concept violated:** one value has one representation and one owner
- **Archive:** tag `v1.0.0`,
  `original/variant-01-baseline-2008/source/guerra-de-caballos-baseline.cpp`

## Evidence

- **Variant:** `variant-01-baseline-2008`. The pointer variants keep the same split.
- **Lines:** 17, 18, 29, 106–109, 229 and 766.
- **Source:**

```cpp
int  vectab[64],vecjugadas[8];              // line 17
int  cab1,cab2,jugada=1,orden,ganador=1;    // line 18
int cf1,cc1,cf2,cc2,cab;                    // line 29

cf1=cab1/8;  cc1=cab1%8;                    // lines 106–107
cf2=cab2/8;  cc2=cab2%8;                    // lines 108–109
ccab=cf*8+cc;                               // line 229

cab1=movcab(cf1,cc1);                       // line 766
```

`cab1` and `cab2` are flat board indices in `0..63`. `cf1`, `cc1`, `cf2` and `cc2` are the
same two positions as row and column in `0..7`. `conversionvm` writes the row/column copy from
the flat index; `conversionmv` converts back. Nothing enforces that the copies agree.

The board is `int vectab[64]`, and every caller indexes it directly: `vectab[cab1]`,
`vectab[ccab]`, `vectab[k]`. The formula `cf * 8 + cc` is the public contract.

## Symptom

The program works while the two copies stay in sync. A move updates `cf` and `cc` through
reference parameters and writes the returned flat index into `cab1` in one line. If the
function returns early, or a future edit updates one copy and not the other, the position
drifts. The board then marks a cell for one position and checks capture for another.

The drift is silent: both values are valid integers, so no compiler and no bounds check sees
the disagreement.

## Root cause

Four loose coordinate integers, two flat indices and a board array are treated as independent
variables. The domain invariant — a position is a row and a column that move together — exists
only in the programmer's head. The representation is public, so every caller must know the
board width, the row-major formula and the one-based printing convention.

`movcab(int &cf, int &cc)` makes the split worse: it mutates the row/column copy and returns
the flat copy, so one call updates one position through two mechanisms.

## Consequence

Change amplification: a board resize touches every index expression. Silent drift: the two
copies can disagree and no compiler sees it. Review cost: a reviewer must reconstruct the
index convention to read a diff. Test cost: a unit test must build the representation, not just
the domain value.

The program does not cause undefined behavior by itself. The `i <= 64` write is `failure-001`.

## Test that catches it

`archaeology/lessons/archaeology-07-representation-leakage/run.sh` runs the same scenario
through a parallel-array version and a record version and compares the summary:

```text
parallel=player=2,3 machine=3,4 visited=3
record=player=2,3 machine=3,4 visited=3
match=yes
```

The lesson freezes the parallel output before refactoring, then updates the caller to
`board.mark(player)` and `player = Knight{2, 3}`. The test proves the record version performs
the same three marks for this scenario. It does not prove equivalence for every position.

## Minimal fix

Group the two coordinates into one value:

```cpp
struct Knight
{
    int row;
    int column;
};
```

Moving a knight becomes one assignment: `player = Knight{2, 3};`. The caller still computes
the board index, but the row and column can no longer drift apart. This removes the
parallel-variable half of the defect, not the representation leakage.

## Structural fix

Give the board an owner. `Board` keeps a private array and exposes `mark(const Knight &)` and
`visited_count()`. The index formula moves inside `mark`, where it belongs. The caller states
the domain operation and no longer needs the width, the origin or the storage.

Track A rungs: [`construction-04-records`][c04] for `Knight`,
[`construction-05-array-of-records`][c05] for the board cells,
[`construction-09-abstract-data-type`][c09] for representation independence, and
[`construction-11-encapsulation`][c11] for the private array.

[c04]: ../../construction/lessons/construction-04-records/lesson.md
[c05]: ../../construction/lessons/construction-05-array-of-records/lesson.md
[c09]: ../../construction/lessons/construction-09-abstract-data-type/lesson.md
[c11]: ../../construction/lessons/construction-11-encapsulation/lesson.md

## Confidence

High that the two representations exist and are updated by different mechanisms. Medium on
how often they drift in the original runs: the source shows the risk, not a measured failure.

## Note on the archive

This card is evidence, not a repair of the archive. The archive stays read-only; the record and
the owner belong in a scratch copy or in `reference/`.
