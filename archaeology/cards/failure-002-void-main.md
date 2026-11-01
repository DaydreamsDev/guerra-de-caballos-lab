# failure-002 — `void main()` is not a standard entry point

> [English](failure-002-void-main.md) | [Español](failure-002-void-main-es.md)

- **Class:** correctness
- **Severity:** high
- **Concept violated:** the entry point is a contract with the runtime
- **Archive:** tag `v1.0.0`,
  `original/variant-01-baseline-2008/source/guerra-de-caballos-baseline.cpp`

## Evidence

- **Variant and line:** `variant-01:32`, `variant-02:37`, `variant-04:52`.
- **Contrast:** `variant-03:47` uses `int main()`.
- **Source:**

```cpp
void  main()             // variant-01, line 32
{
    system("color 1f");
    inicializacion();
    cabubicacion();
    // ...
}
```

The declaration returns `void`. Standard C++ requires `main` to return `int`. Microsoft Visual C++
6.0 accepted `void main` as an extension, which is why the source survived a real build in 2008.

## Symptom

The entry point has no return type. A conforming compiler rejects the program, and a build that
enables `-Werror` fails before the game starts. When a permissive compiler accepts it, the process
exit status is not defined by the source.

## Root cause

The program was written for a compiler that allowed the extension, and the entry point was never
treated as a contract. The original code did not need a status code, so the declaration was chosen
for convenience. The source records a local build convention, not the language rule.

## Consequence

Portability failure. The same file that built in 2008 fails in a modern toolchain or in CI. A test
runner, a shell script or a parent process cannot read a meaningful exit status. The defect is
visible at build time, which makes it cheaper than the memory bugs, but it blocks every other
experiment until it is fixed.

## Test that catches it

The second block of `tests/run_failure_tests.sh`. It greps for `void main` in `variant-01` and for
`int main` in `variant-03`, and prints `failure-002=detected` only when both patterns match. The
pair is deliberate: one variant shows the failure, the other shows the standard form.

The check reads the source text. It does not compile the file, so it detects the declaration, not
the compiler's verdict.

## Minimal fix

```cpp
int main()
{
    // ...
    return 0;
}
```

If the program reaches its normal end, `return 0;` states the status explicitly. The implicit
return in `main` is also valid, but the explicit form is clearer in a lesson about contracts.

## Structural fix

Keep `main` as a thin adapter: read no game state, call one function that runs the game, and return
its status. The game function can then be called from a test without a process boundary.

```cpp
int runGame();
int main() { return runGame(); }
```

Track A rung: [`construction-03-functions`][c03] (functions, parameters and return values).

[c03]: ../../construction/lessons/construction-03-functions/lesson.md

## Confidence

High. The declaration is visible at the cited lines, and the language rule is unambiguous. The only
uncertainty is the 2008 compiler's exact extension behavior, which is not needed to diagnose the
portability defect.

## Note on the test

The test is an isolated reproduction of the failure pattern. It is not a test of the original
program. The grep proves that the declaration exists; it does not prove how any particular compiler
will react.
