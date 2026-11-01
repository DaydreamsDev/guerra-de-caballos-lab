# failure-014 — The dead-code label does not match the live `enter` guard

> [English](failure-014-dead-code.md) | [Español](failure-014-dead-code-es.md)

- **Class:** design
- **Severity:** low
- **Concept violated:** code that cannot affect the result should not ship; live code should
  not hide behind a dead-code label
- **Archive:** tag `v1.0.0`,
  `original/variant-04-pointers-final-2008/source/guerra-de-caballos-pointers-final.cpp`

## Evidence

- **Variants:** `variant-04` for `enter`; `variant-02` for the debug line; `variant-01` for the
  `default` branch.
- **Lines:** `variant-04:31`, `variant-04:71`, `variant-04:880`; `variant-02:1020`;
  `variant-01:696–719`.
- **Source:**

```cpp
char enter=13;                              // variant-04, line 31
while (jugada!=0&&enter==13)                // line 71
enter=getch();                              // line 880, also 905, 934, 961

cout<<pisadas<<"aqui";                      // variant-02, line 1020
```

`enter` is not dead. It is read by the loop condition at line 71 and assigned from `getch()`
at four sites. It is a global key code that also acts as a loop condition, initialized to the
magic value `13`.

The debug line at `variant-02:1020` is the last statement of `impresion` (the function starts
at line 920). It runs on every board print and appends the player's display character and the
word `aqui` to the output.

The `default:` branch at `variant-01:696–719` is not dead either. It is the fall-through target
after a failed case, and it is the only path that advances `i` and detects a trapped knight.
`archaeology-02` labels it unreachable; the loop trace does not support that label.

## Symptom

The board output carries a debug marker. The game ends when the player presses any key other
than Enter, because `enter` is both a key code and a loop condition. A reader who trusts the
catalog looks for dead code and finds live code instead.

The compiler is silent. A scratch build of all four variants with `-Wall -Wextra -Wunused`
(after normalizing `void main` in the scratch copy) produces no `-Wunused-variable` diagnostic:
the candidates are file-scope names with external linkage, and the compiler does not report
those. No unused local was found in the four variants.

## Root cause

Three different problems are grouped under one label:

- **Debug residue.** `cout<<pisadas<<"aqui";` was left in the board printer. It executes, so it
  is not dead; it is unwanted output.
- **A live guard with a bad shape.** `char enter=13` is a global key code used as a loop
  condition. It is live, but its meaning is a magic number and its scope is the whole file.
- **A mislabeled branch.** The `default` branch is the trap handler, not dead code. The label
  came from a reachability argument that ignored the fall-through and the `i++`.

The common cause is analysis by name rather than by control flow. A grep for an unused name
cannot prove dead code when the name has external linkage, and a branch that looks like a
default is not dead if the loop depends on it.

## Consequence

The output is noisy and the exit behavior is surprising. A reviewer spends time on a
non-defect and may delete the trap handler, which would remove the only way the game ends by
a trapped knight. The real defect — a key code used as a loop condition — survives the
review.

The program does not crash. The cost is noise, false confidence and a risky deletion.

## Test that catches it

`archaeology/lessons/archaeology-15-static-analysis/run.sh` shows the triage method on a bench
with one dead local:

```text
warnings=1
fixed=ok
match=yes
```

The lesson compiles with `-Wall -Wextra -Werror`, reads the diagnostic, verifies the claim,
classifies it and deletes the declaration. Applied to the archive, the same flags produce zero
`-Wunused-variable` diagnostics. That is the evidence boundary: the compiler cannot see
file-scope names with external linkage.

The debug line has a one-line check: `grep -n 'cout<<pisadas<<"aqui"'` on `variant-02`. The
`enter` usage has a one-line check: `grep -n '\benter\b'` on `variant-04`, which returns the
declaration, the loop condition and four assignments.

## Minimal fix

Delete the debug line. Replace the global key code with an explicit pause result:

```cpp
bool pause_for_key();          // returns false when the player asks to stop
while (jugada != 0 && pause_for_key())
{
    // ...
}
```

Do not delete the `default` branch. Keep the trap handler and validate `i` before indexing
`vecjugadas`. If the branch is truly unreachable in a later refactor, remove it with a test
that proves the trap path is preserved.

## Structural fix

Route diagnostics through a logging port that is compiled out in release builds. Make the pause
an input adapter that returns a value, not a global character. Use a checked index for
`vecjugadas` (`std::array::at` or an explicit guard), so the trap path is a stated precondition
rather than an implicit fall-through.

Track A rungs: [`construction-03-functions`][c03] for explicit inputs and return values, and
[`construction-17-stl`][c17] for checked access to a fixed-size sequence.

[c03]: ../../construction/lessons/construction-03-functions/lesson.md
[c17]: ../../construction/lessons/construction-17-stl/lesson.md

## Confidence

High that `enter` is live in `variant-04` and that the debug line is in `variant-02`. High that
the archive produces no `-Wunused-variable` diagnostic under `-Wall -Wextra`. Medium on the
catalog's intent: the label may describe a different revision, but the pinned source does not
support it.

## Note on the archive

This card is evidence, not a repair of the archive. The archive stays read-only; the debug
removal and the pause adapter belong in a scratch copy or in `reference/`.
