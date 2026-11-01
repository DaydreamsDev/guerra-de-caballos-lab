# Track B — Archaeology (Arqueología)
> [English](archaeology-track.md) | [Español](archaeology-track-es.md)

> Take the legacy code as it is. Diagnose it like a real codebase (código heredado). Explain every
> failure by its real-world cost. Then show what it should look like.

## Goal and audience

**Goal:** the learner can read unfamiliar code, separate symptom (síntoma) from root cause (causa
raíz), prioritize by cost, and propose an evidence-backed improvement.

**Verification:** failure cards, tests and acceptance criteria.

**Audience:** juniors, reviewers, maintainers, professionals. No prior knowledge of this codebase
required.

**Framing:** a postmortem focused on causes and costs. The question is “what made this code work,
what does it cost now, and what would we change?”.

---

## 1. Why an autopsy, not a “best practices” list

A list of rules is forgettable. A real artifact with real consequences is not.

The legacy code gives us things a textbook cannot:

- **Real failures:** off-by-one, non-determinism, broken initialization, misleading names.
- **Real constraints:** an old compiler, a Windows console, no version control, no tests.
- **Real design debt:** global state, duplication, hidden coupling, representation leakage.
- **Real build and portability problems:** obsolete headers, platform commands, undefined behavior.

The learner practices the actual job: read, diagnose, prioritize, fix, test, explain.

---

## 2. The autopsy protocol (protocolo de autopsia)

Every Track B lesson uses the same evidence spine: predict → inspect → verify → explain → record.

| # | Step | Output |
|---|---|---|
| 1 | **Orientation** (orientación): read the entry point, functions and data flow | Code map |
| 2 | **Predict, build and run** (predecir, compilar y ejecutar): write what you expect the legacy program to do, then reproduce it | Prediction + runnable artifact or documented blocker |
| 3 | **Code inventory** (inventario): files, functions, globals and call graph | Inventory and call graph |
| 4 | **Behavior spec** (especificación de comportamiento): reverse-engineer the rules | Spec + golden traces |
| 5 | **Failure catalog** (catálogo de fallos): classify by root cause | Failure cards |
| 6 | **Severity and cost** (severidad y coste): impact in real work | Prioritized list |
| 7 | **Minimal fix** (arreglo mínimo): smallest change that removes the harm | Patch + test |
| 8 | **Structural fix** (arreglo estructural): the design change that prevents the class of failure | Refactor + link to Track A rung |
| 9 | **Test that would have caught it** (prueba que lo habría detectado) | Test case |
| 10 | **Retrospective** (retrospectiva): what we would do today, and why | Reflection note |

**Rule:** never jump from step 5 to step 8. The minimal fix first; the rewrite is a last resort with
a price.

**The engineering check applies here too.** Before running the legacy program, predict what it will
do — the output, the win condition, the failure. After running, explain the gap. The same predict →
run → compare → explain habit, applied to someone else's code, is how reviewers build a mental model
without guessing.

---

## 3. Failure taxonomy (taxonomía de fallos)

Five classes. The class determines the lesson, not the file.

### Correctness (corrección)

The program does not do what it claims.

Examples in the original:

- `for (i = 0; i <= 64; i++)` over a 64-element array — writes one past the end.
- `void main()` — not standard C++; not portable.
- `srand(time(NULL))` inside the retry loop — non-deterministic placement and repeated seeding.
- Broken initialization in
  `original/variant-03-pointers-dev-2008/source/guerra-de-caballos-pointers-dev.cpp:84-86` —
  `pvectab` is reset to `&vectab[0]` every iteration, so only element 0 is ever cleared.
- Inverted win condition in the precursor — “TRIUNFO PARA LA MIQUINA” when the knight eats all
  pawns.
- `cin >> nombre.vectorNombre` into `char[250]` — unbounded read.

### Design (diseño)

The program works, but the structure makes change expensive.

- Global mutable state (`tablero`, `ordenJugadas`, `jugada`, `ganador`).
- Hidden temporal coupling: `ordenjuego` mutates `jugada` and prints.
- Input and output mixed with logic: `cout` inside `moverJugador`.
- Eight ~48-line `case` bodies, hand-copied.
- Primitive obsession / parallel arrays: the board is `int[64]`, the knight is four loose variables.
- Representation leakage: every function indexes `tablero[i]`.
- No module boundary: one translation unit, no headers.
- No abstract data type: `struct stablero { int mvectab; }` is a transparent wrapper around one int
  (variant-03 and variant-04); variant-02 has no board type.
- No polymorphism: a `switch` on move type instead of dispatch.
- Magic numbers: 64, 8, 21, 156, 219.
- Dead code: `char enter = 13` never reassigned; debug `cout << pisadas << "aqui"`.
- Misnamed abstractions: `struct movimiento { char nombre[250]; }` holds a name.

### Robustness (robustez)

The program works only in one environment.

- `conio.h`, `iostream.h`, `system("color 1f")` — Windows/Microsoft Visual C++ 6.0 lock-in.
- No input validation.
- No error handling; failure paths `exit` or print and continue.
- No bounds checks on the name buffer.
- `rand() % 64` — modulo bias (minor, but a teaching moment).

### Professional practice (práctica profesional)

The code works, but the artifact is not shareable or maintainable.

- No README, no specification, no build instructions.
- No tests, no harness, no determinism.
- Binary artifacts present in the private raw archive (not published): `.exe`, `.obj`, `.pdb`,
  `.ncb`, `.opt`, `.ilk`, `.pch`.
- IDE project and workspace files in the original folders (kept in the private raw vault, not
  published); duplicates of two variants in the raw archive.
- Comments that narrate the obvious, lie, or are corrupted by encoding (CP1252).

---

## 4. Failure card template (ficha de fallo)

Every failure is documented as a card; the catalog in §7 is the index. A card has this shape:

```markdown
### failure-001 — Off-by-one in board initialization

- **Class:** correctness
- **Concept violated:** the boundary is part of the type (el límite es parte del tipo)
- **Paradigm that prevents it:** structured loops + range-based iteration
- **Severity:** high (undefined behavior; may corrupt adjacent memory)
- **Evidence:** `original/variant-02-records-2008/source/guerra-de-caballos-records.cpp:72`
- **Symptom:** the loop writes `vectab[64]` on a 64-element array.
- **Root cause:** `<` was written as `<=`; the bound was copied from a 1-based mental model.
- **Why it matters in real work:** out-of-bounds writes are a classic source of crashes
  and security bugs; they are silent until they aren't.
- **Minimal fix:** `i < 64`.
- **Structural fix:** use a range-based loop or `std::array::fill` so the bound cannot be
  written wrong.
- **Test that would have caught it:** a sanitizer build (`-fsanitize=address`) or a unit
  test that checks the last element.
- **Track A link:** `construction-02` (structured loops) and `construction-05` (array of records).
- **Lesson:** the boundary is part of the type; express it once.
```

**Fields are mandatory.** A card without evidence is an opinion.

---

## 5. Severity scale (escala de severidad)

| Level | Meaning | Action |
|---|---|---|
| **Critical** | Undefined behavior, data loss, security, legal blocker | Fix before publishing |
| **High** | Wrong behavior, non-reproducible results, hard incident | Fix in the minimal patch |
| **Medium** | Expensive change, fragile structure | Structural fix, scheduled |
| **Low** | Readability, naming, comments | Improve when touching the file |

Severity is about **consequence**, not about how ugly the code looks.

---

## 6. Tone and ethics (tono y ética)

- **Blameless.** This code was written for an old compiler under the constraints of its time.
  Judge the decision, not the person.
- **Context first.** Explain the constraint before judging the decision.

---

## 7. The failure catalog

These are the highest-value failures for real work. Each becomes a lesson or part of one.

| ID | Failure | Class | Why it matters |
|---|---|---|---|
| failure-001 | `i <= 64` over a 64-element array | correctness | Silent memory corruption |
| failure-002 | `void main()` | correctness | Portability; undefined behavior |
| failure-003 | `srand(time(NULL))` inside a retry loop | correctness | Non-determinism; flaky tests |
| failure-004 | Broken pointer initialization in `_dev` | correctness | Wrong behavior that looks plausible |
| failure-005 | Inverted win condition in the precursor | correctness | The spec and the code disagree |
| failure-006 | Unbounded name input | correctness | Buffer overflow |
| failure-007 | Global mutable state | design | Incidents are hard to reproduce; tests impossible |
| failure-008 | Eight hand-copied `case` bodies | design | Every rule change touches eight places |
| failure-009 | Parallel arrays / loose knight variables | design | Data can drift; invariants unenforceable |
| failure-010 | `struct stablero { int mvectab; }` is not an abstract data type | design | Representation leaks; no swap possible |
| failure-011 | `switch` on move number and `if (cab == 1)` | design | New move edits the loop |
| failure-012 | Input and output inside the movement logic | design | Core untestable; adapter impossible |
| failure-013 | Magic numbers | design | Meaning lost; changes error-prone |
| failure-014 | Debug `cout` and live `enter` guard | design | Noise; the catalog label is wrong |
| failure-015 | `conio.h` / `system("color")` | robustness | Platform lock-in |
| failure-016 | No tests, no harness, no seed | professional-practice | No regression safety; no comparison |
| failure-017 | No README, spec, or build instructions | professional-practice | Nobody can build or trust it |
| failure-018 | Binaries and IDE files in the original working folders | professional-practice | Stale artifacts; not reproducible from source |
| failure-019 | Comments that narrate or lie | professional-practice | Maintenance debt; false documentation |

---

## 8. Lesson sequence (secuencia de lecciones)

| ID | Lesson | Focus |
|---|---|---|
| archaeology-00 | Orientation: read a legacy codebase | Map |
| archaeology-01 | Build and run legacy code | Build |
| archaeology-02 | Code inventory and call graph | Inventory |
| archaeology-03 | Reverse-engineer the spec from the code | Specify |
| archaeology-04 | Correctness failures (failure-001…failure-006) | Diagnose |
| archaeology-05 | Global state and hidden coupling (failure-007) | Diagnose |
| archaeology-06 | Duplication and the eight-case switch (failure-008) | Diagnose |
| archaeology-07 | Representation leakage and parallel arrays (failure-009, failure-010) | Diagnose |
| archaeology-08 | Missing module boundary | Diagnose |
| archaeology-09 | Missing abstract data type | Diagnose |
| archaeology-10 | Missing polymorphism (failure-011) | Diagnose |
| archaeology-11 | Input and output and testability (failure-012) | Diagnose |
| archaeology-12 | Naming and comments (failure-019) | Diagnose |
| archaeology-13 | Technical documentation for maintainers | Diagnose |
| archaeology-14 | Tests and determinism (failure-016) | Diagnose |
| archaeology-15 | Static analysis and compiler warnings | Diagnose |
| archaeology-16 | Platform lock-in (failure-015) | Diagnose |
| archaeology-17 | Code review of a legacy function | Diagnose |
| archaeology-18 | Refactor workshop: legacy → Track A rung | Refactor |
| archaeology-19 | Review checklist and final retrospective | Review |

---

## 9. Mapping failures to Track A

| Failure class | Track A rung that prevents it |
|---|---|
| Off-by-one, loops | `construction-02` structured loops |
| Global state, coupling | `construction-08` modular, `construction-09` abstract data type |
| Parallel arrays, loose variables | `construction-04` records, `construction-05` array of records |
| Representation leakage | `construction-09` abstract data type |
| Switch on type | `construction-12` polymorphism, `construction-14` generic |
| Input and output mixed with logic | `construction-20` (hexagonal) |
| No determinism / tests | `common-01` harness |
| Platform lock-in | `construction-08` modular + `construction-20` hexagonal |
| Mislabeled artifact | `common-02` glossary + every contrast pair |

The mapping is bidirectional: Track B says *what hurts*; Track A says *what removes the hurt*.

---

## 10. Systemic triage (triaje sistémico)

Do not fix failures one by one. Group them by root cause and fix the class.

Example: `i <= 64`, `j <= 7`, magic `64`, and duplicated board loops are not four bugs. They are one
root cause: **the board's size is not a type**. One change (`std::array<Cell, N>` or a named
constant with a range loop) removes the class.

The learner practices the professional move: **find the class, not the instance**.

---

## 11. What Track B deliberately does not do

- Does not rewrite everything to “modern C++”.
- Does not treat style as a failure without a consequence.
- Does not separate code from its context.
- Does not publish private material.

---

## 12. Minimum viable product (scope)

`archaeology-00` through `archaeology-10` + the first 15 failure cards. That is enough to prove the
format and to cover correctness, design and the top professional gaps. The full track is now drafted
through `archaeology-19`; all fifteen cards are drafted.
