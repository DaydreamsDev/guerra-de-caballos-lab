# failure-008 — Eight hand-copied case bodies encode one delta rule

> [English](failure-008-eight-case-switch.md) | [Español](failure-008-eight-case-switch-es.md)

- **Class:** design
- **Severity:** medium
- **Concept violated:** one rule belongs in one place; the varying part belongs in data
- **Archive:** tag `v1.0.0`,
  `original/variant-01-baseline-2008/source/guerra-de-caballos-baseline.cpp`

## Evidence

- **Variant:** `variant-01-baseline-2008`. The same switch is at `variant-02:290`,
  `variant-03:316` and `variant-04:330`.
- **Lines:** 239–695. The eight case labels are at 241, 302, 359, 415, 471, 527, 584
  and 640.
- **Source:**

```cpp
switch(vecjugadas[i])                    // line 239
{
case 1:                                  // delta (-1, +2)
    if (cf-1>=0&&cf-1<=7&&cc+2>=0&&cc+2<=7)
    {
        ccab=conversionmv(cf-1,cc+2);
        // ... the same skeleton: turn, visited, capture, apply, record, print ...
    }
case 2:                                  // line 302, delta (-2, +1)
    if (cf-2>=0&&cf-2<=7&&cc+1>=0&&cc+1<=7)
    {
        ccab=conversionmv(cf-2,cc+1);
```

The case bodies occupy 455 lines. Strip comments, whitespace and digits and the token
sequence is the same. The only values that change are sixteen integers: a row delta and a
column delta for each of eight moves. The source says so itself at line 301:

```text
//los demas casos tienen la misma estructura que esta explicada en el caso 1
```

## Symptom

The program works. Every legal move lands on the right square, the board records it and the
game ends correctly. The defect is invisible in a single run. It appears when the rule
changes: a corrected delta or a ninth move must be applied in eight places, and a missed copy
diverges silently.

The compiler cannot compare case 3 with case 6. A review sees a 60-line diff for a two-integer
change.

## Root cause

The code stores one rule as eight code copies instead of one table of data. Each case also
contains two nearly identical turn branches, so the apply block exists sixteen times. The
delta is the varying part; bounds, turn, visited, capture, apply, record and print are the
skeleton. The skeleton was copied with the data.

This is a knowledge clone, not a textual clone. The bodies differ in comments, whitespace and
signs, which is why a byte diff does not reveal the duplication.

## Consequence

Change amplification: one rule change touches eight bodies. Silent divergence: a fix applied
to seven bodies leaves one case behaving differently. Review cost: the rule disappears behind
the copies. Merge cost: two independent case edits conflict in one function.

The program is correct for the inputs it receives. The cost is paid by the next rule change,
not by the current run.

## Test that catches it

`archaeology/lessons/archaeology-06-duplication-switch/run.sh` compiles the baseline and
compares the switch with a table for 8 moves over a 5×5 grid:

```text
comparisons=200 mismatches=0
```

The switch is the oracle; the table is the candidate. The lesson also runs a change test:
append a ninth delta and a ninth table entry, then count the touched places in both paths. In
the switch path that is a new case body of about 56–61 lines plus the loop bound.

The baseline isolates the delta. It does not test bounds, visited cells, capture or turn.

## Minimal fix

Replace the eight case bodies with one delta table and one lookup:

```cpp
struct Delta { int row; int column; };

constexpr Delta moveDeltas[8] = {
    {-1, +2}, {-2, +1}, {-2, -1}, {-1, -2},
    {+1, -2}, {+2, -1}, {+2, +1}, {+1, +2},
};

Delta deltaFor(int moveNumber) { return moveDeltas[moveNumber - 1]; }
```

Keep a range guard. The table loses the switch's `default` behavior for out-of-range input.

## Structural fix

Extract the whole move application into one function that takes a `Delta` and the state it
needs:

```cpp
MoveResult applyMove(Position from, Delta delta, const Board &board, int turn);
```

The function checks bounds, visited cells and capture, applies the delta, records the move and
returns the outcome. The eight cases disappear and the table stays as data. The two turn
branches collapse into one call.

Track A rungs: [`construction-03-functions`][c03] for the skeleton,
[`construction-05-array-of-records`][c05] for the table, and
[`construction-19-data-oriented`][c19] for the idea that the deltas are data, not code.

[c03]: ../../construction/lessons/construction-03-functions/lesson.md
[c05]: ../../construction/lessons/construction-05-array-of-records/lesson.md
[c19]: ../../construction/lessons/construction-19-data-oriented/lesson.md

## Confidence

High. The eight labels, the 455-line span and the repeated skeleton are visible. The 200
comparisons are a run, not an inference. The claim is limited to the delta calculation.

## Note on the archive

This card is evidence, not a repair of the archive. The archive stays read-only; the
extraction belongs in a scratch copy, and the baseline stays available as the oracle.
