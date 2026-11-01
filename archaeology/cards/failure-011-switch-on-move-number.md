# failure-011 — Movement is selected by move number, not by a piece type

> [English](failure-011-switch-on-move-number.md) |
> [Español](failure-011-switch-on-move-number-es.md)

- **Class:** design
- **Severity:** medium
- **Concept violated:** a piece should own its rule; the loop should not enumerate kinds
- **Archive:** tag `v1.0.0`,
  `original/variant-01-baseline-2008/source/guerra-de-caballos-baseline.cpp`

## Evidence

- **Variant:** `variant-01-baseline-2008`. The same shape is at `variant-02:290`,
  `variant-03:316` and `variant-04:330`.
- **Lines:** 29, 239, 249 and 765.
- **Source:**

```cpp
int cf1,cc1,cf2,cc2,cab;                    // line 29

switch(vecjugadas[i])                       // line 239
{
case 1:
    // ...
    if (cab==1)                             // line 249
    {
        // the knight-1 branch
    }
    // ... the same block for the other knight ...
}

cab=1;                                      // line 765, one of four turn blocks
```

The archive has **no switch on piece kind**. It has a switch on the move number and a
`cab` integer with `if (cab==1)` branches. The piece-kind decision is scattered: nine
`if (cab==1)` sites in `variant-01`, ten in the other variants, plus four turn blocks at
765, 790, 820 and 844.

The catalog's shorthand, "`switch` on piece type", describes the design smell, not the
source. Cite the source: the switch is on `vecjugadas[i]`, and the piece is `cab`.

## Symptom

The program works with two identical knights. Adding a third piece is not a one-line change.
It requires a third coordinate pair, a third counter, a third branch at every `cab==1` site, a
fourth turn block and a movement rule for the new piece. The switch on move number does not
know the piece exists; the loop knows it in nine places.

The reader cannot answer "where is a knight's movement rule?" with one location. The rule is
spread across the move-number cases and the turn branches.

## Root cause

The piece is not a type. It is two global coordinate pairs, two flat indices and an integer
selector. The loop names each knight by hand, and `movcab(int &cf, int &cc)` takes coordinates
rather than a piece. The move-number switch answers *where* the piece goes; the `cab` branches
answer *whose* coordinates change. Both decisions live in the same body.

A table of deltas indexed by move number is still dispatch by index. It removes copied
arithmetic; it does not create a boundary around a piece. That is why `failure-008` and this
card are different repairs.

## Consequence

A new piece edits the shared movement function and the turn loop. A change to one piece's rule
risks the other, because both share the same case bodies. A test must set `cab`, the
coordinates, the board and the move order to exercise one turn.

The program is correct for two knights. The cost is paid by the next piece, not by the current
run.

## Test that catches it

`archaeology/lessons/archaeology-10-polymorphism/run.sh` compares a switch-on-kind baseline
with a dispatch version for 2 pieces × 8 moves × 25 positions:

```text
comparisons=400 mismatches=0
```

The lesson then adds a camel to each path and counts the touched places. The dispatch addition
is a delta table and a class; the switch addition is an enum value and a branch inside the
shared function. The line counts are close; the difference is where the edit lands.

The baseline reconstructs the kind decision. It is not verbatim 2008 code, and the archive's
two knights move identically. The test proves the replacement for 400 sampled inputs, not the
archive's behavior.

## Minimal fix

Introduce a piece type and let it answer the movement question:

```cpp
class Piece
{
public:
    virtual ~Piece() = default;
    virtual Position move(int moveNumber, Position position) const = 0;
    virtual const char *role_name() const = 0;
};
```

`Knight` keeps its own delta table. The loop holds `Piece *` and stops naming `cab`. Keep the
move-number table inside the class; the table still answers *where*, the type answers *who*.

## Structural fix

Move position, identity and movement into the piece, and let the loop hold `Piece &` without
coordinates:

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

Now `cab` disappears, and `movcab(int &cf, int &cc)` is replaced by a call on the piece. The
turn loop no longer enumerates kinds.

Track A rungs: [`construction-11-encapsulation`][c11] for the private state,
[`construction-12-polymorphism`][c12] for the interface, and
[`construction-13-composition`][c13] for the next question: should a piece *have* a movement
rule instead of *being* one?

[c11]: ../../construction/lessons/construction-11-encapsulation/lesson.md
[c12]: ../../construction/lessons/construction-12-polymorphism/lesson.md
[c13]: ../../construction/lessons/construction-13-composition/lesson.md

## Confidence

High that the archive has no piece-kind switch and that `cab` is an integer selector. High that
the nine `if (cab==1)` sites exist in `variant-01`. Medium on the exact edit count for a third
piece: it depends on how many turn blocks and print sites the change also touches.

## Note on the archive

This card is evidence, not a repair of the archive. The archive stays read-only; the piece type
belongs in a scratch copy or in `reference/`.
