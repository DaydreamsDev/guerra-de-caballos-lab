# Failure cards

> [English](README.md) | [Español](README-es.md)

One card per diagnosed failure in the pinned archive at tag `v1.0.0`. Each card records the
evidence, the symptom, the root cause, the consequence, a test that catches the pattern, a
minimal fix and a structural fix linked to a Track A rung.

The cards are evidence, not repairs. The archive stays read-only; fixes belong in a scratch copy
or in `reference/`.

## Index

| # | Card | Class | Value |
|---:|---|---|---|
| 001 | [Loop bound](failure-001-loop-bound.md) · [ES](failure-001-loop-bound-es.md) | correctness | The board loop writes one cell past 64; the bound must come from the type. |
| 002 | [Void main](failure-002-void-main.md) · [ES](failure-002-void-main-es.md) | correctness | `void main()` is not a standard entry point; the return type is a contract. |
| 003 | [Seed in loop](failure-003-seed-in-loop.md) · [ES](failure-003-seed-in-loop-es.md) | correctness | Reseeding inside the placement retry loop destroys determinism. |
| 004 | [Broken initialization](failure-004-broken-initialization.md) · [ES](failure-004-broken-initialization-es.md) | correctness | Resetting the pointer inside the loop clears only the first cell. |
| 005 | [Inverted win](failure-005-inverted-win.md) · [ES](failure-005-inverted-win-es.md) | correctness | The win condition is inverted between the spec and the code. |
| 006 | [Unbounded name](failure-006-unbounded-name.md) · [ES](failure-006-unbounded-name-es.md) | correctness | Unbounded `cin >> char[250]` overflows the name buffer. |
| 007 | [Global state](failure-007-global-state.md) · [ES](failure-007-global-state-es.md) | design | Twelve mutable globals (plus three constants) make every rule change a multi-function edit and every test a whole-program setup. |
| 008 | [Eight-case switch](failure-008-eight-case-switch.md) · [ES](failure-008-eight-case-switch-es.md) | design | One delta rule is copied into eight 56–61-line case bodies; a table stores the 16 integers once. |
| 009 | [Parallel arrays](failure-009-parallel-arrays.md) · [ES](failure-009-parallel-arrays-es.md) | design | A knight's position is stored twice, as a flat index and as row/column, so the copies can drift. |
| 010 | [Transparent struct](failure-010-transparent-struct.md) · [ES](failure-010-transparent-struct-es.md) | design | `struct stablero { int mvectab; }` renames the storage but hides nothing; callers still index it. |
| 011 | [Switch on move number](failure-011-switch-on-move-number.md) · [ES](failure-011-switch-on-move-number-es.md) | design | Movement is selected by move number and `if (cab==1)`, not by a piece type; a new piece edits the loop. |
| 012 | [I/O inside logic](failure-012-io-inside-logic.md) · [ES](failure-012-io-inside-logic-es.md) | design | `movcab` prints and waits for keys, so the core cannot run without a console. |
| 013 | [Magic numbers](failure-013-magic-numbers.md) · [ES](failure-013-magic-numbers-es.md) | design | Board size, width, glyph bytes and the draw cap are unnamed literals scattered across the code. |
| 014 | [Dead code](failure-014-dead-code.md) · [ES](failure-014-dead-code-es.md) | design | A debug `cout` and a live `enter` guard add noise; the catalog's dead-code label is wrong. |
| 015 | [Platform lock-in](failure-015-platform-lock-in.md) · [ES](failure-015-platform-lock-in-es.md) | robustness | `conio.h`, `iostream.h`, `system("color")`, `system("cls")` and `getch()` bind the build to Windows. |

## Reading order

The first six cards are correctness failures: they produce a wrong result or undefined
behavior. Cards 007–014 are design failures: the program works, but change and test cost is
high. Card 015 is a robustness failure: the build depends on one environment.

## Corrections to the catalog

Two catalog labels do not match the pinned source. The cards state the source, not the
shorthand:

- **failure-011.** The archive has no switch on piece type. It has a switch on the move number
  at `variant-01:239` and nine `if (cab==1)` sites.
- **failure-014.** `char enter=13` is live in `variant-04` (read at line 71, assigned from
  `getch()` at four sites). The debug line `cout<<pisadas<<"aqui"` at `variant-02:1020` is the
  real residue.

Card 013 also records that `200` is not in the archive: it is the draw cap in the modern spec
and harness.

## Lessons

Each card points to the lesson that supplies its test or analysis. The full sequence is in
[`../README.md`](../README.md) and
[`../../docs/guide/archaeology-track.md`](../../docs/guide/archaeology-track.md).
