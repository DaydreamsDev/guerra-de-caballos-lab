# failure-007 — Mutable globals make every function a hidden contract

> [English](failure-007-global-state.md) | [Español](failure-007-global-state-es.md)

- **Class:** design
- **Severity:** medium
- **Concept violated:** state must have an owner and a visible interface
- **Archive:** tag `v1.0.0`,
  `original/variant-01-baseline-2008/source/guerra-de-caballos-baseline.cpp`

## Evidence

- **Variant:** `variant-01-baseline-2008`. The same state returns in `variant-02`, `variant-03`
  and `variant-04`, sometimes behind the pointer wrappers.
- **Lines:** 17, 18, 29 and 234.
- **Source:**

```cpp
int  vectab[64],vecjugadas[8];              // line 17
int  cab1,cab2,jugada=1,orden,ganador=1;    // line 18
int cf1,cc1,cf2,cc2,cab;                    // line 29

int movcab(int &cf,int &cc)                 // line 234
```

`movcab` reads `vecjugadas`, `jugada`, `cab`, `vectab`, `cab1` and `cab2`. It writes
`ganador`, `vectab[ccab]` and `jugada`. It also prints the case number at line 262 and
calls `impresion()` at line 705. The signature names two integers.

The scanner in `archaeology-05` counts the mentions: `vectab=138`, `vecjugadas=15`,
`cab1=30`, `cab2=30`, `jugada=69`, `orden=10`, `ganador=24`, `cf1=5`, `cc1=5`,
`cf2=5`, `cc2=5`, `cab=15`. The counts are mentions, not assignments.

## Symptom

The program works. It places two knights, alternates turns, prints a board and stops on a
capture or a trap. Nothing crashes and nothing prints the wrong winner. The defect is that
no function states the state it uses. A reader cannot tell from `movcab(cf1, cc1)` that the
call depends on six globals, writes three more and needs a console.

A single move cannot be called in isolation. The test must build the board, the move order,
the move number, the turn selector and both knight positions first. The setup is longer than
the assertion.

## Root cause

The file-scope names are a shared scratchpad. Any function can read or write them without
declaring the dependency. Three specific collisions make the coupling worse:

- `jugada` is a move counter, a loop sentinel (`0`) and a special-case value (`2`);
- `ganador` is only a flag; `cab` says who won;
- `cab1` and `cf1`/`cc1` are two copies of one position, kept in sync by hand.

The call graph therefore understates the dependency. `impresion` looks like a leaf, but it
reads the board, both knights and the winner position.

## Consequence

Every rule change becomes a multi-function edit. The turn order is set in `cabubicacion`,
consumed in `impresionorden` and re-read inside `movcab`. A test needs the whole program and
a console. An incident cannot be reproduced from a single input, because the failing state
has no boundary where it can be printed.

This is not undefined behavior by itself. The `i <= 64` write is `failure-001`; the globals
are a design cost, not a memory error.

## Test that catches it

`archaeology/lessons/archaeology-05-global-state/scan_globals.sh` produces the declaration
lines and the mention counts. The lesson's replay check is the behavioral test: extract one
turn into a scratch copy and call it twice with the same arguments. If the second call
differs, the difference is the hidden state. The filled `GLOBAL-STATE-MAP-TEMPLATE.md` is the
record: one row per global, one row per function.

The scanner does not prove a mutation. It counts mentions; the map is the evidence.

## Minimal fix

Create one seam. Make `movcab` state its inputs and return its result:

```cpp
struct MoveResult {
    bool moved;
    bool captured;
    int  cell;
};

MoveResult movcab(int &cf, int &cc, int knight, int moveNumber,
                  const int (&order)[8], int board[64]);
```

Move the printing to `impresionorden`. This does not remove the globals; it makes one
function callable without the console and without reading the caller.

## Structural fix

Give the state an owner. A `GameState` record holds the board, both knights, the move number
and the outcome. A `Board` owns its cells and a `Knight` owns its position, so `cab1` and
`cf1`/`cc1` cannot drift. Every function takes the state it needs as a parameter.

Track A rungs: [`construction-11-encapsulation`][c11] for the private invariant,
[`construction-13-composition`][c13] for the board inside the game, and
[`construction-08-modular`][c08] for the boundary that makes the owner visible.

[c08]: ../../construction/lessons/construction-08-modular/lesson.md
[c11]: ../../construction/lessons/construction-11-encapsulation/lesson.md
[c13]: ../../construction/lessons/construction-13-composition/lesson.md

## Confidence

High that the cited functions read and write the named globals. Medium on the runtime order
of mutations: the map is read from the source, not traced with a debugger.

## Note on the archive

This card is evidence, not a repair of the archive. The archive stays read-only; any fix
belongs in a scratch copy or in `reference/`, never in `original/`.
