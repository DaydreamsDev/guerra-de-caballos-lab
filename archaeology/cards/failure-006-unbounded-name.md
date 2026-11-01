# failure-006 — Unbounded name input overflows `char nombre[250]`

> [English](failure-006-unbounded-name.md) | [Español](failure-006-unbounded-name-es.md)

- **Class:** correctness
- **Severity:** critical
- **Concept violated:** input length is part of the buffer contract
- **Archive:** tag `v1.0.0`,
  `original/variant-02-records-2008/source/guerra-de-caballos-records.cpp`

## Evidence

- **Variant and line:** `variant-02:46`, `variant-03:56`, `variant-04:62`.
- **Source:**

```cpp
struct movimiento {
    char nombre[250];       // variant-02, line 32
};
struct movimiento nom;
// ...
cin>>nom.nombre;            // variant-02, line 46
```

`operator>>` for `char *` reads until whitespace and has no width limit. The array size is not part
of the extraction.

## Symptom

A name longer than 249 characters writes past the end of `nombre`. The program does not check the
length, does not warn and does not truncate. The bytes after the array are overwritten with the
remaining input and a terminating NUL.

## Root cause

The program uses a fixed C array and the default formatted extraction. `cin >> charArray` cannot
know the array's size; the programmer must supply a width. The code trusts the user to respect a
bound that the code never states. The same record also stores the name for display, so the input is
not validated anywhere else.

## Consequence

Buffer overflow. Adjacent members, the stack frame or heap metadata can be overwritten. The outcome
depends on the layout and the mitigations: a crash, corrupted game state, or code execution. At
minimum the behavior is undefined for every long input. The name is the first thing the program
asks for, so the defect is reachable before any game logic runs.

## Test that catches it

The third block of `tests/run_failure_tests.sh`. It greps for `cin>>` in `variant-02` and prints
`failure-006=detected`. The check confirms that the unbounded extraction exists; it does not feed a
long name and does not observe the overflow.

A stronger test would call the input function with a 300-character string and assert that the name
is truncated and the surrounding canary is intact. That test is not in this harness because the
original function is tied to `cin` and the console.

## Minimal fix

Limit the extraction to the buffer size, reserving one byte for the terminator:

```cpp
#include <iomanip>
cin >> std::setw(sizeof(nom.nombre)) >> nom.nombre;
```

`setw(250)` reads at most 249 characters and appends the NUL. The alternative is
`cin.getline(nom.nombre, sizeof nom.nombre);`, which also stops at the buffer size.

The minimal fix prevents the overflow. It does not validate the content, reject empty names or
handle names with spaces.

## Structural fix

Use `std::string` and `std::getline`, then validate the length and the content explicitly:

```cpp
std::string name;
std::getline(std::cin, name);
if (name.empty() || name.size() > 249)
{
    // reject or truncate with a stated policy
}
```

Track A rung: [`construction-17-stl`][c17] (standard strings and containers) and
[`construction-22-error-contracts`][c22] (input validation and error contracts).

[c17]: ../../construction/lessons/construction-17-stl/lesson.md
[c22]: ../../construction/lessons/construction-22-error-contracts/lesson.md

## Confidence

High that the extraction is unbounded and that the buffer is 250 bytes. Medium on the practical
exploitability, which depends on the compiler, the stack layout and the platform mitigations. The
undefined behavior is not in doubt; the attack path is.

## Note on the test

The test is an isolated reproduction of the failure pattern. It is not a test of the original
program. The grep proves the pattern; it does not prove an overflow at runtime.
