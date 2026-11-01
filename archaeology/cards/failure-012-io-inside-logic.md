# failure-012 — `movcab` prints and waits for keys inside the movement logic

> [English](failure-012-io-inside-logic.md) | [Español](failure-012-io-inside-logic-es.md)

- **Class:** design
- **Severity:** medium
- **Concept violated:** the core decides the game; an adapter decides the console
- **Archive:** tag `v1.0.0`,
  `original/variant-01-baseline-2008/source/guerra-de-caballos-baseline.cpp`

## Evidence

- **Variant:** `variant-01-baseline-2008`. `variant-02` adds the input side.
- **Lines:** 234–724, 262, 704, 712, 42–43 and 781–864.
- **Source:**

```cpp
int movcab(int &cf,int &cc)                 // line 234
{
    // ...
    cout<<" 1.";                            // line 262
    // ...
    cout<<"No hay movimientos posibles ..."; // line 704
    impresion();                            // line 705
}

getch();                                    // line 42, and 781, 806, 835, 861
system("cls");                              // line 43, and 784, 809, 838, 864
system("color 1f");                         // line 38
```

`movcab` occupies about 491 lines. Nineteen of them contain `cout`. It moves a knight, decides
whether the game ended and narrates both. The turn loop clears the screen and waits for a key
before the next turn.

## Symptom

To observe one move you must run the program, sit through the screen clears and press a key.
There is no function that returns a move result. A test cannot call the movement rule, because
the call needs a terminal.

The output is mixed with the computation. The same function that writes `vectab[ccab]` also
prints the case number. A failed move prints a message and ends the game from inside the
movement function.

## Root cause

The console was the product, so printing from `movcab` avoided passing a board and a transcript
around. `getch()` and `system("cls")` were the available console controls. The code never
separated the device from the policy: the movement function owns the screen, the pause and the
narration.

The coupling is not only `cout`. In `variant-02`, the display character `pisadas` is threaded
through `cabubicacion`, `ordenjuego` and `movcab`. A presentation detail contaminates three
domain signatures.

## Consequence

The core cannot run without a console. A unit test, a script, a replay tool or a second
interface must reimplement the program or fake the terminal. The output format is not pinned,
so a wording change can break a test that counts lines.

The program is correct for a human at a console. The cost is paid by the first non-console
caller.

## Test that catches it

`archaeology/lessons/archaeology-11-input-output/run.sh` builds a console binary and a test
binary. The test binary links `core.cpp` and `test.cpp` only; it never sees `console.cpp`:

```text
console_lines=8
test_output=lines=8 winner=player moves=3
test=ok
match=yes
```

The lesson proves the core has no console dependency at the object level:

```bash
c++ -std=c++20 -Wall -Wextra -Werror -O2 -c code/core.cpp -o build/core.o
nm -C -u build/core.o | grep -E 'printf|cout|getch|fwrite|puts'
```

The filter prints nothing and exits 1. That is the result: the core object has no undefined
console symbol.

The bench is not the archive. It isolates the separation; it does not call `movcab`.

## Minimal fix

Return a result and move the printing to the caller:

```cpp
struct MoveResult
{
    bool moved;
    bool captured;
    int cell;
};

MoveResult movcab(int &cf, int &cc);
```

`movcab` no longer prints. The caller decides what to show. This is the shape of the baseline:
`run_game` returns a `GameResult`, and `console.cpp` prints it.

## Structural fix

Define the result as domain data, not as text. Give the console a renderer, the test an
assertion adapter and the entry point an input adapter:

```text
argv / stdin ──► input adapter ──► run_game ──► GameResult ──► console adapter
                                                     └───────► test adapter
```

The format lives in one place and the policy lives where it belongs. A test can link the core
without a terminal.

Track A rungs: [`construction-20-hexagonal`][c20] for ports and adapters, and
[`construction-08-modular`][c08] for the module boundary that makes a core possible.

[c08]: ../../construction/lessons/construction-08-modular/lesson.md
[c20]: ../../construction/lessons/construction-20-hexagonal/lesson.md

## Confidence

High that `movcab` prints and that the turn loop waits for keys. High that the core cannot be
called without a console. Medium on the exact count of `cout` lines in other variants: the
counts are from `grep -c` on the pinned files.

## Note on the archive

This card is evidence, not a repair of the archive. The archive stays read-only; the separation
belongs in a scratch copy or in `reference/`.
