# failure-004 — Broken pointer initialization clears only the first cell

> [English](failure-004-broken-initialization.md) |
> [Español](failure-004-broken-initialization-es.md)

- **Class:** correctness
- **Severity:** high
- **Concept violated:** an initialization loop must advance the cursor
- **Archive:** tag `v1.0.0`,
  `original/variant-03-pointers-dev-2008/source/guerra-de-caballos-pointers-dev.cpp`

## Evidence

- **Variant:** `variant-03-pointers-dev-2008`.
- **Lines:** 84–86.
- **Source:**

```cpp
for (i=0;i<=64;i++)
{
    pvectab=&vectab[0];         // line 84
    *pvectab=*(pvectab+i);      // line 85
    pvectab->mvectab=0;         // line 86
}
```

`pvectab` is a `struct stablero *` and `vectab` is `struct stablero vectab[64]`.

## Symptom

Only `vectab[0]` is cleared. Cells 1 through 63 keep whatever they held before the call. In a fresh
process the globals are zero-initialized, so the board looks correct and the bug is invisible. On a
second game, a reused board or a test that pre-fills the array, the stale values remain.

## Root cause

The pointer is reset to `&vectab[0]` inside every iteration, so every write targets element 0. The
line `*pvectab=*(pvectab+i)` does not advance the cursor either: it copies element `i` into element
0, and the next line immediately overwrites that copy with zero. The intended loop was probably
`pvectab = &vectab[i]; pvectab->mvectab = 0;`, but the reset was left inside the body.

## Consequence

Initialization is a no-op for 63 of 64 cells. A second run starts with the previous run's move
numbers, so the game state is wrong in a way that looks like a gameplay bug. The failure is
especially expensive because a fresh demo passes: the defect hides until the board is reused.

## Test that catches it

`broken_initialization_test()` in `tests/failure_tests.cpp`. It creates an eight-element array of
ones, resets the pointer to the first element on every iteration and clears through it. It returns
`true` when index 0 is zero and index 1 is still one, which is the signature of the reset bug.
`tests/run_failure_tests.sh` prints `failure-004=detected`.

The test reproduces the pointer reset, not the `stablero` struct and not the original array.

## Minimal fix

Remove the reset and the copy; index the array directly:

```cpp
for (i = 0; i < 64; i++)
{
    vectab[i].mvectab = 0;
}
```

This also removes the `<=` bound from failure-001 in this function. The two cards overlap at
`variant-03:82`; keep the fixes separate in your record so the reasoning stays visible.

## Structural fix

Use `std::array<stablero, 64>` and `fill` or a range-based loop. The container owns its length, and
there is no cursor to reset by accident.

Track A rung: [`construction-05-array-of-records`][c05] (array of records) and
[`construction-07-pointers`][c07] (pointer arithmetic and lifetime).

[c05]: ../../construction/lessons/construction-05-array-of-records/lesson.md
[c07]: ../../construction/lessons/construction-07-pointers/lesson.md

## Confidence

High. The reset, the copy and the write are visible at lines 84–86. The "only first cell" effect
follows from the control flow, and the test reproduces it.

## Note on the test

The test is an isolated reproduction of the failure pattern. It is not a test of the original
program. It uses plain integers so the pointer bug is visible without the rest of the game.
