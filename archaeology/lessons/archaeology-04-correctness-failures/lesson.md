# archaeology-04 — Correctness failures

> [English](lesson.md) | [Español](lesson-es.md)

> **Status:** drafted · **Concept:** correctness failures · **Prerequisite:** archaeology-03 ·
> **Estimated time:** 90 minutes
> **Reading:** ~17 minutes

## Why this lesson exists

`archaeology-03` reconstructed the behavior of the archived program and captured a trace. It
described what the code does. This lesson asks the next question: which of those behaviors are
defects, what do they cost, and what would a test have caught?

The archive preserves six correctness failures. They are evidence, not a to-do list. The product of
this lesson is six cards and one test run, not a patched archive. That distinction is the whole
point: the archive records the defect, the line and the consequence; a repair belongs in a labeled
adaptation or in the laboratory.

The six failures are small enough to read in one sitting and different enough to teach six kinds of
reasoning: an off-by-one, a non-standard entry point, a seed in a retry loop, a pointer reset, an
unbounded read and an inverted win condition. Each one has a symptom that is easy to see and a root
cause that is not.

## What you will be able to do

By the end of this lesson you can:

- define a correctness failure and distinguish it from a design, robustness or professional-practice
  failure;
- separate a symptom from a root cause;
- write a failure card with evidence, consequence, a test, a minimal fix and a structural fix;
- run the failure harness against the archive and read the six `detected` lines;
- explain what each test checks and what it does not check;
- choose a minimal fix before a structural fix;
- link a structural fix to a Track A rung without turning the lesson into a rewrite;
- rank six failures by cost in a real codebase;
- keep the archive read-only while still demonstrating a repair in a scratch copy.

## The concept: correctness failure

A **correctness failure** is a behavior that contradicts the program's own claim or a stated
contract. The program may compile, run and print a board. The result is still wrong, undefined or
non-reproducible. Correctness is about the result, not about the elegance of the code.

Track B separates five classes. The class determines the lesson, not the file.

| Class | Question | Example in this archive |
|---|---|---|
| **Correctness** | Does the program do what it claims? | `i <= 64` over a 64-element array. |
| **Design** | Does the structure make change expensive? | Global mutable state; eight copied `case` bodies. |
| **Robustness** | Does it work outside one environment? | `conio.h`, `system("color 1f")`. |
| **Professional practice** | Can someone else build, use or trust it? | No README, no spec, no tests. |
| **Pedagogical artifact** | Does the lesson teach what it claims? | `variant-01` contains no pointers. |

The six cards in this lesson are correctness failures. Several also have robustness consequences: a
buffer overflow is a security problem, and `void main` is a portability problem. Classify by the
primary question, then note the secondary cost.

### Symptom vs root cause

The symptom is what an observer sees. The root cause is the decision that produced it. A card that
stops at the symptom is a bug report; a card that reaches the root cause is a diagnosis.

| Failure | Symptom | Root cause |
|---|---|---|
| 001 | The loop writes `vectab[64]` on a 64-element array. | `<` became `<=`; the bound is a second literal. |
| 002 | The entry point returns `void`. | A compiler extension was treated as the language rule. |
| 003 | Placement changes between runs; a fixed seed can hang. | The seed is reset inside the retry loop. |
| 004 | Only the first board cell is cleared. | The pointer is reset to element 0 every iteration. |
| 005 | A long name overwrites memory after `nombre`. | `cin >>` has no width and the array size is not in the call. |
| 006 | The machine is declared winner when the knight eats all pawns. | The end predicate and the winner label are independent. |

The distinction matters because the fixes are different. A symptom patch at the wrong layer hides
the defect; a root-cause fix removes the class.

### The card protocol

Every failure is documented as a card. The cards in `cards/` use the same nine fields. The fields
are mandatory; a card without evidence is an opinion.

1. **Evidence.** The variant, the line and a source snippet. Quote the code, not a memory of the
   code. If the same pattern appears in several variants, list them.
2. **Symptom.** What an observer sees, in one or two sentences. No diagnosis yet.
3. **Root cause.** The decision that produced the symptom. This is the sentence a reviewer should
   remember.
4. **Consequence.** The cost in real work: undefined behavior, a crash, a wrong result, a security
   finding, a flaky test, a blocked port. Severity is about consequence, not about how ugly the code
   looks.
5. **Test that catches it.** Name the test and say what it checks. A test that executes the pattern
   is stronger than a test that greps the source; both are useful if you state the difference.
6. **Minimal fix.** The smallest change that removes the harm. One character, one moved call, one
   bounded extraction. It does not have to be beautiful.
7. **Structural fix.** The design change that prevents the class of failure. Link it to a Track A
   rung so the learner can see the concept behind the patch.
8. **Confidence.** High, medium or low, with the reason. The source can be certain while the
   original intent is not.
9. **Note on the test.** Say that the test is an isolated reproduction, not a test of the original.

The order is a protocol, not a preference. **Minimal fix before structural fix.** The track is
explicit: never jump from the failure catalog to the rewrite. The minimal fix proves that you
understand the harm and that the harm can be removed without changing the design. The structural fix
then addresses the class, with a price you can name.

### No repair in the archive

The archive is frozen at tag `v1.0.0`. `original/` is evidence. Do not edit it, do not apply a patch
inside it, and do not "fix it while you are there".

- A repair goes in a scratch copy, a labeled adaptation or the laboratory.
- The test harness does not include the archive; it reproduces the pattern in isolation.
- If you edit the archive, the evidence is gone and the next learner cannot reproduce your finding.

This is not bureaucracy. The defect is the artifact. A repaired archive would document the repair,
not the 2008 program.

## Before you start

Read these in full:

- [`README.md`](README.md) — the lesson card, the test command and the acceptance criterion.
- [`../archaeology-03-reverse-spec/lesson.md`](../archaeology-03-reverse-spec/lesson.md) — the
  prerequisite: the behavior spec and the trace.
- [`../../../docs/guide/archaeology-track.md`](../../../docs/guide/archaeology-track.md) — §3, the
  failure taxonomy, and §4, the failure card template.
- [`../../../docs/archive.md`](../../../docs/archive.md) — the pinned archive and scope.
- The archive's `05-known-issues.md` — the six rows this lesson turns into cards.

You also need:

- a C++20 compiler (`c++`, `g++` or `clang++`) and its version;
- `bash` and `grep`;
- a local clone of the archive at tag `v1.0.0`;
- [`tests/failure_tests.cpp`](tests/failure_tests.cpp) and
  [`tests/run_failure_tests.sh`](tests/run_failure_tests.sh), which ship with this lesson.

> **Rule:** the archive is read-only. The harness reads it with `grep`; it never writes into it.

## The 90-minute path

| Min | Step | What you do | Evidence |
|---|---|---|---|
| 0–10 | Predict | Write one sentence per failure before reading the cards | Prediction table |
| 10–25 | Reproduce | Run the harness; read the six `detected` lines | Test output |
| 25–55 | Diagnose | Read the six locations; separate symptom from root cause | Six cards |
| 55–70 | Fix minimally | Write the smallest patch for each failure | Minimal-fix table |
| 70–80 | Fix structurally | Link each class to a Track A rung; rank by cost | Structural table and ranking |
| 80–90 | Record | Fill `EVIDENCE.md`; check the archive is clean | Portfolio entry |

If one failure takes twenty minutes because the control flow is
tangled, that is information about the failure, not about you.

### The engineering check in this lesson

Track B uses the same habit as Track A, with a different verb: the program is already built, so you
**reproduce** instead of build.

| Step | Where it happens | What it produces |
|---|---|---|
| **Predict** | Step 1 | Six falsifiable claims about what each failure looks like. |
| **Reproduce** | Step 2 | The six `detected` lines and the command that produced them. |
| **Diagnose** | Step 3 | Six cards with symptom, root cause and consequence. |
| **Fix** | Step 4 | A minimal patch per card, in a scratch copy. |
| **Test** | Step 4 | The test that catches each pattern, named and explained. |
| **Record** | Step 5 | The cards, the ranking and `EVIDENCE.md`. |

## Step 1 — Predict (10 min)

Before you open the cards, write one sentence per failure. Use the source locations from the README
and the known-issues table, but do not read the diagnosis yet.

For each one, answer three questions:

1. What would the code look like if this failure were present?
2. What would an observer see when it runs?
3. What test would catch it without needing the original binary?

A useful prediction is falsifiable. "The loop is wrong" is not. "The loop writes one element past
the array, so a sanitizer reports a global-buffer-overflow" is.

### A worked prediction

Take failure-003, the seed in the retry loop:

- expected code: `srand(time(NULL))` inside the `while` that retries a collision;
- expected symptom: two runs differ; a fixed seed can repeat the same collision;
- expected test: re-seed the same value twice and observe that the sequence restarts.

That prediction can fail in an interesting way. If the seed were outside the loop, the second run
would differ but the retry would still progress. Write the prediction down before you look.

## Step 2 — Run the tests (15 min)

From this lesson directory:

```bash
bash tests/run_failure_tests.sh /path/to/archive
```

A reference run on this machine prints:

```text
failure-001=detected
failure-003=detected
failure-004=detected
failure-005=detected
failure-002=detected
failure-006=detected
```

The order is fixed. The C++ harness prints the first four lines; the shell script prints the last
two. The script exits `0` when all six say `detected`.

### What `detected` means

`detected` means the test reproduced the pattern. It does not mean the archived program was
compiled, run or proven broken at runtime.

| Line | Produced by | What it checks |
|---|---|---|
| `failure-001=detected` | `loop_bound_test()` | A `<= 64` loop clears a sentinel at index 64. |
| `failure-003=detected` | `seed_in_loop_test()` | Re-seeding the same value restarts the sequence. |
| `failure-004=detected` | `broken_initialization_test()` | Resetting the pointer clears only index 0. |
| `failure-005=detected` | `inverted_win_test()` | The all-pawns-eaten state maps to `machine`. |
| `failure-002=detected` | shell `grep` | `void main` in variant-01 and `int main` in variant-03. |
| `failure-006=detected` | shell `grep` | `cin>>` into the name buffer in variant-02. |

The harness compiles `tests/failure_tests.cpp` with `-std=c++20 -Wall -Wextra -Werror -O2`. It is a
small program with four functions; it does not include the archive. The shell checks read the
archive's text with `grep`.

### If a line says `not-detected`

Do not change the test first. Check, in this order:

1. the archive path — the script expects `original/variant-.../source/...`;
2. the tag — the line numbers and the text belong to `v1.0.0`;
3. the compiler — the harness needs C++20 and `-Werror`;
4. the pattern — `grep` is literal enough for `void main`, `int main` and `cin>>`.

A failing check is evidence about the check before it is evidence about the code.

### The order hides a dependency

`set -euo pipefail` is active. The C++ harness runs first; if it returns non-zero, the script stops
before the two shell checks. A `not-detected` in the first four lines therefore hides 002 and 005.
Fix the harness first, then re-run.

## Step 3 — Diagnose the six failures (30 min)

The cards are the product. This step is a worked example followed by five short diagnoses. Read the
full card for each failure; the lesson gives you the thread to pull.

### failure-001 — the worked card

Open [`../../cards/failure-001-loop-bound.md`](../../cards/failure-001-loop-bound.md). Read it once,
then map each field to the source.

**Evidence.** `variant-01:61`, with the same pattern in `variant-02:72`, `variant-03:82` and
`variant-04:88`:

```cpp
int vectab[64];          // line 17
for (i=0;i<=64;i++)      // line 61
{
    vectab[i]=0;
}
```

**Symptom.** The loop writes `vectab[64]`, one element past the end. The program usually continues.

**Root cause.** The array size and the loop bound are two literals. `<` became `<=`.

**Consequence.** Undefined behavior. A global array may overwrite an adjacent global; a local copy
may corrupt the stack. The failure is silent until it is not.

**Test.** `loop_bound_test()` uses a 65-element array, writes a sentinel at index 64 and runs the
same `<= 64` loop. It returns `true` when the sentinel was cleared. The 65th element makes the
reproduction defined; the original's 65th write is not.

**Minimal fix.** `i < 64`.

**Structural fix.** `std::array<int, 64>` plus `fill` or a range-based loop. The bound comes from
the type. Track A: `construction-02` (structured loops) and `construction-17` (STL).

**Confidence.** High. The declaration, the loop and the write are visible.

**Note.** The test is an isolated reproduction, not a test of the original.

That is the whole card. Every other card has the same shape. The work is not filling the template;
it is finding the sentence that belongs in **root cause**.

### failure-002 — the entry point

`variant-01:32`, `variant-02:37` and `variant-04:52` declare `void main()`. `variant-03:47` declares
`int main()`, which gives you the contrast pair.

The symptom is a declaration with no return type. The root cause is a compiler extension treated as
the language rule: Visual C++ 6.0 accepted `void main`, so the source built. The consequence is
portability: a conforming compiler or a `-Werror` build rejects it, and the process exit status is
not defined by the source.

The test is the shell block that greps `void main` in variant-01 and `int main` in variant-03. It
checks the text, not the compiler's verdict. The minimal fix is `int main()` with `return 0;`. The
structural fix keeps `main` as a thin adapter around `int runGame()`. Track A: `construction-03`.

### failure-003 — the seed in the retry loop

`variant-01:76`, `variant-02:87`, `variant-03:99` and `variant-04:105` call `srand(time(NULL))`
inside the `while` that retries a placement collision.

The symptom is non-reproducible placement. The root cause is that the seed is reset on every
attempt, so the generator restarts instead of advancing. The loop can only progress when the clock
changes. With a fixed seed patch, a collision repeats forever: `archaeology-03` records seed 9999 as
`run_status=124`.

The test is `seed_in_loop_test()`. It uses a small LCG, seeds it with `1`, records the first value,
re-seeds with `1` three times and checks whether the next value repeats. It detects repeated
seeding, not `rand()`. The minimal fix moves the seed before the loop, or takes it as a parameter.
The structural fix injects a random source. Track A: `construction-03` and `construction-20`.

### failure-004 — the pointer reset

`variant-03:84-86`:

```cpp
for (i=0;i<=64;i++)
{
    pvectab=&vectab[0];         // line 84
    *pvectab=*(pvectab+i);      // line 85
    pvectab->mvectab=0;         // line 86
}
```

The symptom is that only `vectab[0]` is cleared. The root cause is the reset inside the body: every
write targets element 0, and the copy on line 85 is overwritten immediately. The consequence is a
board that looks initialized only because globals start at zero; a second game or a reused board
keeps stale moves.

The test is `broken_initialization_test()`: eight ones, a pointer reset every iteration, and a check
that index 0 is zero while index 1 is still one. The minimal fix indexes the array directly. The
structural fix uses `std::array<stablero, 64>` and `fill`. Track A: `construction-05` and
`construction-07`.

This card overlaps failure-001 at `variant-03:82`. The loop has both the `<=` bound and the pointer
reset. Keep the two fixes separate in your record; a combined patch would hide which reasoning fixed
which harm.

### failure-006 — the unbounded name

`variant-02:46`, `variant-03:56` and `variant-04:62` read `cin>>nom.nombre` into
`char nombre[250]`. The extraction has no width. A name longer than 249 characters writes past the
array.

The symptom is memory corruption after the name. The root cause is the C array plus the default
formatted extraction: the call cannot know the array size. The consequence is a buffer overflow,
with undefined behavior for every long input and a security cost that depends on the layout.

The test is the shell block that greps `cin>>` in variant-02. It detects the pattern, not an actual
overflow. The minimal fix is `cin >> std::setw(sizeof(nom.nombre)) >> nom.nombre;` or
`cin.getline(nom.nombre, sizeof nom.nombre);`. The structural fix is `std::string` plus
`std::getline` and validation. Track A: `construction-17` and `construction-22`.

### failure-005 — the inverted win

`variant-00:299` prints `TRIUNFO PARA LA MIQUINA` when all four pawn cells are no longer `-1`, which
is the state after the knight has eaten all four pawns. The winner label is hard-coded to the
machine; the predicate is four negations; there is no `Outcome` value or player model.

The symptom is a wrong result. The root cause is that the end condition and the winner were written
independently. The consequence is a wrong scoreboard, transcript or test. The test is
`inverted_win_test()`: it maps `allPawnsEaten = true` to `"machine"` and returns true when the
result is not `"player"`. The minimal fix names the predicate and states the winner explicitly. The
structural fix is an `Outcome` enum and one `evaluateOutcome()` function. Track A:
`construction-10`.

The original intent is the one thing the source cannot settle. The card marks it as an inference.

## Step 4 — Choose the minimal fix (15 min)

For each card, write the smallest patch that removes the harm. Do it in a scratch copy, not in the
archive. The point is not to produce a beautiful diff; it is to prove that the harm is removable
without changing the design.

| Card | Minimal patch | What it does not fix |
|---|---|---|
| 001 | `i < 64` | The next loop can repeat the literal. |
| 002 | `int main()` and `return 0;` | The game is still tied to the process. |
| 003 | Move `srand` before the loop, or pass a seed | The sequence is still platform-specific. |
| 004 | Index `vectab[i]` directly | The board still has a separate length literal. |
| 005 | `setw(sizeof(nom.nombre))` or `getline` | Content validation and spaces. |
| 006 | Name the predicate; state the correct winner | No full result model. |

Two rules for this step:

1. **One harm, one patch.** Do not fix the off-by-one and the pointer reset in one change if the
   cards are separate. You lose the evidence of which change did what.
2. **No rewrite.** If the patch grows past a few lines, you have moved to Step 5. That is allowed,
   but label it as the structural fix and keep the minimal one in the record.

### What a minimal fix is not

A minimal fix is not a temporary hack you are ashamed of. It is the first fix: the one that stops
the harm and buys time for the design change. The track's rule is "minimal fix first; the rewrite is
a last resort with a price." A minimal fix that is correct and tested is a professional result.

## Step 5 — Choose the structural fix (10 min)

The structural fix addresses the class, not the instance. Link it to a Track A rung so the concept
is visible.

| Card | Structural change | Track A rung | Why it prevents the class |
|---|---|---|---|
| 001 | `std::array` + `fill` or range-for | `construction-02`, `construction-17` | The bound comes from the type. |
| 002 | Thin `main` around `int runGame()` | `construction-03` | The entry point is one adapter with a status. |
| 003 | Seed parameter or `RandomSource` | `construction-03`, `construction-20` | Randomness is injected, not hidden in the loop. |
| 004 | Array of records + index/iterator | `construction-05`, `construction-07` | The cursor cannot reset by accident. |
| 005 | `std::string` + validation | `construction-17`, `construction-22` | The buffer owns its length and the input has a contract. |
| 006 | `Outcome` + `evaluateOutcome()` | `construction-10` | One decision, one source of truth, testable without `cout`. |

The rung is a link, not a mandate. You are not required to complete `construction-17` to diagnose
failure-001. You are required to name the concept that makes the failure class disappear.

## Step 6 — Rank by cost (10 min)

Severity is about consequence. Rank the six in your context and write the reason. A reference order
for a codebase that accepts untrusted input:

| Rank | Card | Cost | Why |
|---:|---|---|---|
| 1 | failure-006 | Critical | Buffer overflow on the first input; security and corruption. |
| 2 | failure-001 | Critical | Undefined behavior on every initialization; silent. |
| 3 | failure-003 | High | Non-reproducible runs; a fixed seed can hang. |
| 4 | failure-004 | High | Wrong board on reuse; invisible in a fresh demo. |
| 5 | failure-005 | High | Wrong winner; corrupts every downstream result. |
| 6 | failure-002 | High | Portability; visible at build time, cheap to fix. |

The order is not universal. If the program is a one-shot demo with no user input, failure-006 may be
lower. If the program is a test oracle, failure-003 may be first. The skill is the reasoning, not
the rank.

### Cost is not the same as difficulty

Failure-002 is high severity and a two-line fix. Failure-005 is critical and a one-line fix.
Failure-003 is high and needs a design decision about the seed. Rank by what happens if you do not
fix it, then use effort as a tie-breaker.

## Step 7 — Record (10 min)

Fill [`EVIDENCE.md`](EVIDENCE.md):

- setup and environment;
- the prediction table, written before the cards;
- the test command and the six lines;
- the card ledger: variant, line, symptom, root cause, test, fixes, confidence;
- the severity ranking with reasons;
- the evidence limits;
- the archive integrity check.

The cards already contain the full diagnosis. The evidence record is where you show that you ran the
tests, that you know what they check and that you did not edit the archive.

## The original in detail

Facts you will cite repeatedly, with line numbers from the pinned archive:

| Variant | Lines | What is there |
|---|---|---|
| `variant-00` | 296–299 | `pruebatriunfo()`: four `!= -1` checks and the machine win message. |
| `variant-01` | 17, 32, 61, 76 | `vectab[64]`, `void main`, `<= 64`, `srand` in the retry loop. |
| `variant-02` | 32, 37, 46, 72, 87 | `char nombre[250]`, `void main`, `cin>>`, `<= 64`, `srand`. |
| `variant-03` | 31, 47, 56, 82, 84–86, 99 | `char nombre[250]`, `int main`, `cin>>`, `<= 64`, pointer reset, `srand`. |
| `variant-04` | 33, 52, 62, 88, 105 | `char nombre[250]`, `void main`, `cin>>`, `<= 64`, `srand`. |

The archive's `05-known-issues.md` is the index. The cards are the diagnosis. The line numbers are
the common ground between them.

## What the tests prove and do not prove

| Claim | Supported? | Why |
|---|---|---|
| The pattern exists at the cited line. | Yes | The source snippet and the line number. |
| The original program crashes or corrupts memory. | No | The original is not compiled or run. |
| The original program is exploitable. | No | No input is fed; the layout is not tested. |
| The minimal fix removes the harm. | Only in the reproduction | The patch is not applied to the archive. |
| The structural fix prevents the class. | By design argument | The rung is a concept, not a measured result. |
| `void main` fails on every compiler. | No | Some compilers accept it as an extension. |
| `cin >>` overflows for every long input. | The pattern, yes; the runtime, no | The check is a grep, not a fuzz test. |

This table is the honesty check for the whole lesson. A card that claims more than this table allows
is overreaching.

## Self-check (binary)

- [ ] I wrote six predictions before reading the cards.
- [ ] I ran `tests/run_failure_tests.sh` and saw six `detected` lines.
- [ ] I can name the four tests in the C++ harness and the two shell checks.
- [ ] I can explain why `detected` is not proof that the original is broken.
- [ ] I wrote six cards with all nine fields.
- [ ] Each card cites the variant, the line and a source snippet.
- [ ] Each card separates symptom from root cause.
- [ ] Each card states the consequence in real work.
- [ ] Each card names the test and what it checks.
- [ ] Each card gives a minimal fix before a structural fix.
- [ ] Each structural fix links to a Track A rung.
- [ ] I ranked the six by cost and wrote the reason.
- [ ] I recorded the evidence limits, including the inference about failure-005.
- [ ] The archive is unmodified (`git status` in the archive is clean).

If a box is not ticked, the unticked box names the next step. There is no penalty for repeating one.

## Misconceptions this lesson refutes

| Misconception | Why it is wrong | What to do instead |
|---|---|---|
| "A failure card is a bug report." | A bug report describes a symptom; a card diagnoses a cause and tests a fix. | Reach the root cause and name the test. |
| "The test proves the original is broken." | The tests reproduce patterns in isolation; they do not run the archive. | State what the test checks and what it does not. |
| "Fix the archive while you are there." | The archive is evidence; a repair destroys the artifact. | Patch a scratch copy or a labeled adaptation. |
| "`void main` always fails." | Some compilers accept it as an extension. | Say "not standard, not portable", not "never compiles". |
| "The loop writes into `vecjugadas`." | The write is undefined; the target depends on the layout. | Say "may corrupt adjacent memory" and cite the UB. |
| "`srand` in the loop only affects randomness." | It can also make a collision retry forever. | Connect the seed to the hang in archaeology-03. |
| "The pointer bug clears nothing." | It clears element 0; the other 63 cells keep their values. | Say "only the first cell is cleared". |
| "The win condition is fine because the comment agrees." | Comments in this archive narrate or lie; the code and the message disagree. | Trust the executed path, not the comment. |
| "A minimal fix is a temporary hack." | It is the first fix: it removes the harm and buys time for the design change. | Keep the minimal fix and then argue the structural one. |
| "The structural fix is the real fix, so skip the minimal one." | The track forbids jumping from the catalog to the rewrite. | Minimal first; structural second, with a price. |

## Terms used here

| Term | One-line meaning |
|---|---|
| Correctness failure | A result that contradicts the program's claim or contract. |
| Symptom | What an observer sees when the failure happens. |
| Root cause | The decision that produced the symptom. |
| Consequence | The cost of the failure in real work. |
| Minimal fix | The smallest change that removes the harm. |
| Structural fix | The design change that prevents the class of failure. |
| Track A rung | A construction lesson that teaches the concept behind the structural fix. |
| Isolated reproduction | A small program that reproduces a pattern without the original. |
| Undefined behavior | The language imposes no requirement on the result. |
| Buffer overflow | A write past the end of a fixed-size buffer. |
| Non-reproducible | The same inputs do not produce the same result. |
| Sentinel | A special value that marks a state, such as `-1` for a pawn. |

## What “done” means here

The **Self-check (binary)** above is the authoritative checklist. If every box is ticked, the
lesson is done for you. `meta.yml` mirrors the same criteria for tooling.

## Next step

- Continue to `archaeology-05` — global state and hidden coupling. The correctness cards show what
  goes wrong; the design lessons show why the structure makes it easy to go wrong.
- If failure-003 was the hardest, re-read the seed 9999 row in `archaeology-03`. The hang is the
  clearest link between a correctness defect and a testability defect.
- If failure-004 looked harmless, run the initialization twice in a scratch copy and print the
  board. The first run hides the bug; the second run shows it.
- If you want a stronger test for failure-006, write a small function that takes a string and a
  buffer size, then call it with 300 characters. Keep it out of the archive.
- If you disagree with the rank, write the alternative and the context that justifies it. The
  ranking is an argument, not a fact.

## References

- [`README.md`](README.md) — the lesson card, the test command and the acceptance criterion.
- [`cards/`](../../cards/) — the failure cards, English and Spanish.
- [`tests/failure_tests.cpp`](tests/failure_tests.cpp) — the four isolated reproductions.
- [tests/run_failure_tests.sh](tests/run_failure_tests.sh) — the harness and the two source checks.
- [`EVIDENCE.md`](EVIDENCE.md) — the portfolio record you fill in.
- [`../../../docs/guide/archaeology-track.md`](../../../docs/guide/archaeology-track.md) — §3, the
  failure taxonomy; §4, the card template; §5, the severity scale.
- [`../../../docs/archive.md`](../../../docs/archive.md) — the pinned archive, commit and scope.
- [`../../../docs/guide/lesson-catalog.md`](../../../docs/guide/lesson-catalog.md) — the lesson
  sequence and the `archaeology-04` acceptance criterion.
- [`../archaeology-03-reverse-spec/lesson.md`](../archaeology-03-reverse-spec/lesson.md) — the
  prerequisite: the behavior spec and the trace.
- Archive: <https://github.com/DaydreamsDev/DaydreamsDev-guerra-de-caballos-archive>, tag `v1.0.0`,
  commit `284e144e41403afbf0cc44422876425cacfb2bd5`.
