# failure-001 — Loop bound writes one element past the board

> [English](failure-001-loop-bound.md) | [Español](failure-001-loop-bound-es.md)

- **Class:** correctness
- **Severity:** critical
- **Concept violated:** the boundary is part of the type
- **Archive:** tag `v1.0.0`,
  `original/variant-01-baseline-2008/source/guerra-de-caballos-baseline.cpp`

## Evidence

- **Variant:** `variant-01-baseline-2008`. The same pattern appears at `variant-02:72`,
  `variant-03:82` and `variant-04:88`.
- **Line:** 61.
- **Source:**

```cpp
int vectab[64];          // line 17

for (i=0;i<=64;i++)      // line 61
{
    vectab[i]=0;
}
```

The condition is `<=`, so the body runs for `i = 0..64`: 65 iterations over a 64-element array.
The last write is `vectab[64]`, one element past the end.

## Symptom

The program initializes a board that has 64 cells and then writes a 65th cell. Nothing in the
output reports the extra write. The program usually continues, which is what makes the defect
dangerous: the damage is silent until it is not.

## Root cause

The array size and the loop bound are two separate literals. The bound was written from a 1-based
mental model, or copied from a loop that counts 64 items inclusively, and `<` became `<=`. The
compiler cannot compare `i` with the array length because the length is not part of the loop
expression. A second literal is free to drift.

## Consequence

Undefined behavior. In this translation unit `vectab` is a global array, so the extra write may
land on `vecjugadas`, on another global, or on padding. In a copy where the board is local, it may
corrupt the stack canary or an adjacent object. The visible outcomes range from no symptom to a
wrong board, a crash or a security finding. AddressSanitizer reports it immediately; a demo hides
it for years.

## Test that catches it

`loop_bound_test()` in `tests/failure_tests.cpp`. It creates a 65-element `std::array`, writes the
sentinel value `7` at index 64, runs the same `<= 64` loop and returns `true` when the sentinel was
cleared. The 65th element makes the reproduction defined; the original's 65th write is not.
`tests/run_failure_tests.sh` prints `failure-001=detected` when the test returns `true`.

The test checks the bound, not the original memory layout. It does not compile or run the archived
file.

## Minimal fix

```cpp
for (i = 0; i < 64; i++)
{
    vectab[i] = 0;
}
```

One character removes the out-of-bounds write. It does not stop the next loop from repeating the
mistake.

## Structural fix

Store the board in `std::array<int, 64>` and clear it with `board.fill(0)`, or iterate with a
range-based `for`. The bound then comes from the type and cannot be typed wrong.

Track A rung: [`construction-02-structured`][c02] and
[`construction-17-stl`][c17].

[c02]: ../../construction/lessons/construction-02-structured/lesson.md
[c17]: ../../construction/lessons/construction-17-stl/lesson.md

## Confidence

High. The declaration, the loop and the write are visible at lines 17 and 61. The undefined behavior
follows from the language rule, not from a guess about the runtime.

## Note on the test

The test is an isolated reproduction of the failure pattern. It is not a test of the original
program, and a passing run says nothing about the 2008 binary.
