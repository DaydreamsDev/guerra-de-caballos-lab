# failure-005 — Inverted win condition in the 2007 precursor

> [English](failure-005-inverted-win.md) | [Español](failure-005-inverted-win-es.md)

- **Class:** correctness
- **Severity:** high
- **Concept violated:** the end condition and the winner are one decision
- **Archive:** tag `v1.0.0`,
  `original/variant-00-precursor-2007/source/ajedrez-caballo-2007.txt`

## Evidence

- **Variant:** `variant-00-precursor-2007`.
- **Line:** 299, inside `pruebatriunfo()`; the condition is at lines 296–297.
- **Source:**

```cpp
if( tablero[peon1f][peon1c]!=-1&&tablero[peon2f][peon2c]!=-1
&&tablero[peon3f][peon3c]!=-1&&tablero[peon4f][peon4c]!=-1 )
{
    cout<<"\n\nTRIUNFO PARA LA MIQUINA :D\n\n";   // line 299
    a=0;
}
```

The pawn sentinel is `-1`. A pawn that has been eaten is no longer at its original cell, so the
cell no longer holds `-1`. All four `!= -1` checks are true when all four pawns have been eaten.

## Symptom

When the last pawn is captured, the program prints `TRIUNFO PARA LA MIQUINA` and stops. The only
piece that moves and captures is the knight; the message credits the machine. The player sees the
knight eat the last pawn and then reads that the machine won.

## Root cause

The end predicate and the winner label are written independently. The predicate is four negated
sentinel checks; the label is a hard-coded string. There is no `Outcome` value and no player model
that says which side owns the knight. The comment above the function says the machine eats the
pawns, but comments in this archive are known to describe an older or imagined behavior. The code
never derives the winner from the side that moved.

## Consequence

The game reports the wrong result. Any scoreboard, transcript or test that reads the message is
wrong. The four negations also make the next change easy to invert again: a reader has to simulate
the sentinel to know whether the condition means "all present" or "all eaten". The defect is a
correctness failure, not a presentation preference.

## Test that catches it

`inverted_win_test()` in `tests/failure_tests.cpp`. It maps `allPawnsEaten = true` to the string
`"machine"` and returns `true` when that result is not `"player"`. The test reproduces the mapping
that the card diagnoses: the winning side is hard-coded to the machine. `tests/run_failure_tests.sh`
prints `failure-005=detected`.

The test does not build the board, move the knight or read the original source. It isolates the
decision table.

## Minimal fix

Name the predicate, state the winner explicitly and make the condition positive:

```cpp
const bool allPawnsCaptured =
    tablero[peon1f][peon1c] != -1 &&
    tablero[peon2f][peon2c] != -1 &&
    tablero[peon3f][peon3c] != -1 &&
    tablero[peon4f][peon4c] != -1;

if (allPawnsCaptured)
{
    cout << "\n\nTRIUNFO PARA EL JUGADOR :D\n\n";
    a = 0;
}
```

If the machine owns the knight, keep the machine label and rename the predicate to
`allPawnsCaptured`. The point is to derive the label from the actor, not to hard-code it. The
minimal fix removes the wrong winner; it does not add a full game-result model.

## Structural fix

Represent the result as a value, not a string printed inside a predicate:

```cpp
enum class Outcome { Playing, PlayerWins, MachineWins, Trap };
Outcome evaluateOutcome(const Board &board);
```

One function decides the outcome; the presentation layer maps `Outcome` to a message. The decision
can then be tested without `cout`.

Track A rung: [`construction-10-adt-invariants`][c10]
(invariants and a single source of truth for state).

[c10]: ../../construction/lessons/construction-10-adt-invariants/lesson.md

## Confidence

High that the condition is four `!= -1` checks and the message is hard-coded to the machine. High
that the code fires when all four pawns have been eaten. Medium on the intended winner,
because the source has no player model and the comment may be wrong. The card records the mismatch;
the intent is an inference.

## Note on the test

The test is an isolated reproduction of the failure pattern. It is not a test of the original
program. It encodes the mapping the card diagnoses, not the board state that triggers it.
