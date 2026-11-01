# failure-010 — `struct stablero` is a transparent wrapper, not an abstract data type

> [English](failure-010-transparent-struct.md) | [Español](failure-010-transparent-struct-es.md)

- **Class:** design
- **Severity:** medium
- **Concept violated:** a type is defined by its operations, not by its layout
- **Archive:** tag `v1.0.0`,
  `original/variant-03-pointers-dev-2008/source/guerra-de-caballos-pointers-dev.cpp`

## Evidence

- **Variants:** `variant-03-pointers-dev-2008` and `variant-04-pointers-final-2008`.
- **Lines:** `variant-03:34–44`, `variant-03:86`; `variant-04:44–49`, `variant-04:92`.
- **Source:**

```cpp
struct stablero {          // variant-03, line 34
    int  mvectab;          // line 36
};
struct stablero vectab[64];                         // line 42
struct stablero *pvectab,*pvectab1,*pvectab2;       // line 44

pvectab->mvectab=0;        // line 86, the translation of vectab[i]=0
```

`variant-04` repeats the idea with another name: `struct tab { int vectab; }` at lines 44–45,
`struct tab tablero[64]` at line 48, and `ptablero->vectab=0` at line 92.

The field is the interface. `variant-03` has 102 lines that mention `mvectab`; `variant-04` has
146 lines that mention `tablero`. The wrapper renamed the type and changed nothing about the
dependency.

## Symptom

The program works. The struct compiles, the array is declared and the pointer loop clears the
board. A reader might conclude that the board is now a type. It is not: every caller still
names the storage, still knows that a cell is an integer and still writes the field directly.

The mechanical translation is visible. `vectab[i]=0;` became `pvectab->mvectab=0;`. The
pointer moved; the coupling stayed.

## Root cause

The code wrapped one integer in a struct and called it a type. A struct with one public
field has no operations, no invariant and no hidden representation. The only honest test of an
abstract data type is a swap: replace the representation, keep the interface and run the same
caller. The 2008 code cannot pass that test, because there is no interface to keep.

`struct svecjugadas { int mjugadas; }` fails the same test for the move list. The archive has
two wrappers and zero abstractions.

## Consequence

The representation is still public, so a change to the board's storage reaches every caller.
Renaming `mvectab` to `visited` would stop 102 lines from compiling; changing only the type
would let most of them compile, because C++ converts integers to `bool`. That is worse: the
coupling survives the type change.

The program does not become incorrect. The cost is change amplification and a false sense of
encapsulation.

## Test that catches it

`archaeology/lessons/archaeology-09-abstract-data-type/run.sh` builds the same `main.cpp`
against two representations and compares the output:

```text
array=visited=2 first=1 second=1
bitmask=visited=2 first=1 second=1
match=yes
```

The caller and the header are hashed before and after; only the implementation file on the
link line changes. The lesson also captures the compiler rejecting `sizeof(Board)`, a stack
`Board` and `board->visited` through an incomplete type.

That is the swap test the wrapper cannot pass. The archive has no header and no operation to
keep.

## Minimal fix

Replace the transparent struct with an opaque type and operations:

```cpp
// board.h
struct Board;
Board *board_create();
void board_destroy(Board *);
void board_reset(Board *);
void board_mark(Board *, int row, int column);
bool board_is_visited(const Board *, int row, int column);
std::size_t board_visited_count(const Board *);
```

The definition lives in one `.cpp`. The caller can hold and pass a `Board *` and cannot size,
allocate or inspect it. This is the smallest change that removes the field from the contract.

## Structural fix

Use a class with a pimpl, or a factory that returns a value type with an out-of-line
destructor. Then the caller gets RAII, copy and move, and the representation stays hidden.
The C-style handle buys representation independence and pays with manual lifetime; the pimpl
returns the lifetime check to the compiler at the cost of an indirection per operation.

Track A rungs: [`construction-09-abstract-data-type`][c09] for the swap,
[`construction-10-adt-invariants`][c10] for validity,
[`construction-11-encapsulation`][c11] for the class, and
[`construction-16-ownership`][c16] for the lifetime.

[c09]: ../../construction/lessons/construction-09-abstract-data-type/lesson.md
[c10]: ../../construction/lessons/construction-10-adt-invariants/lesson.md
[c11]: ../../construction/lessons/construction-11-encapsulation/lesson.md
[c16]: ../../construction/lessons/construction-16-ownership/lesson.md

## Confidence

High that the wrapper is transparent and that callers name the field. High that the archive
has no header. Medium on the exact line counts in other variants: the cited counts are from
`grep -c` on the pinned files.

## Note on the archive

This card is evidence, not a repair of the archive. The archive stays read-only; the opaque
type belongs in a scratch copy or in `reference/`.
