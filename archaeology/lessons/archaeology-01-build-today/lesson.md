# archaeology-01 — Build and run legacy code

> [English](lesson.md) | [Español](lesson-es.md)

> **Status:** drafted · **Concept:** build and run legacy code · **Prerequisite:** common-03 ·
> **Estimated time:** 60 minutes
> **Reading:** ~15 minutes

## Why this lesson exists

Every legacy autopsy starts with the same embarrassing question: does it even run? You can read a
930-line file for an hour and build a beautiful theory about its defects, and then discover that the
code has never compiled on the machine in front of you. Reading without running is guessing with
extra steps.

This is step 2 of the Track B protocol: **predict, build and run**. The product is one of two
things, and both are acceptable: a runnable build, or a documented blocker. A blocker is not a
failure. A blocker you can name, quote and reproduce is evidence. "It doesn't work" is not.

The lesson also teaches a harder discipline. Once the program runs, the temptation is to say *"it
works"*. It does not. It compiles through a compatibility layer, it runs to completion, and it
prints something. Whether that something is correct, portable, or the behavior the archive describes
is a different question with a different answer. Separating "it runs" from "it is right" is the
whole skill.

## What you will be able to do

By the end of this lesson you can:

- write a falsifiable prediction about a program you have not run;
- reproduce the exact compiler errors of a naive modern build, quoted verbatim;
- explain what each compatibility change does and what risk it introduces;
- build the unchanged archive source through a probe that lives outside the archive;
- run it under a timeout and record the exit status, the output volume and the first error line;
- explain why the output is not the modern transcript contract;
- state what the probe proves and what it does not prove, in one paragraph;
- record the prediction, the errors, the changes, the output and the boundary in `EVIDENCE.md`.

## The concept: build and run legacy code

A **build** is a transformation from source to a runnable artifact. On a modern machine it has four
inputs: the source, the compiler and its version, the headers it resolves, and the flags. A legacy
build adds a fifth: the environment the code assumed but never declared.

This program assumed a Windows console and a pre-standard C++ library. Neither is present. So the
build has to be split into two questions:

1. **What does the compiler reject?** Those are errors, and they are exact, quotable text.
2. **What did the code assume about the world?** Those are not errors. They are silent assumptions
   that either fail at run time or quietly change what the program does.

| Idea | Meaning in this lesson |
|---|---|
| **Build** | Source plus compiler plus headers plus flags, producing a runnable artifact. |
| **Compatibility shim** | A header you add to satisfy an include the modern library no longer ships. |
| **Build flag as a change** | A flag that alters the source's meaning (`-fpermissive`, `-Dmain=...`). |
| **Blocker** | A failure you can reproduce and quote: command, error, environment. |
| **Timeout** | A hard stop so a legacy loop cannot hang your session. |
| **Evidence boundary** | The line between "it compiled and exited" and "it behaves correctly". |

### Why the naive build is not a waste of time

You could go straight to the probe. Do not. The naive build is the cheapest possible experiment, and
its errors are the most valuable sentences in the lesson: they are the compiler telling you, in its
own words, which assumptions of the old toolchain are gone. A learner who skips the naive build has
to trust the lesson's summary. A learner who runs it has a primary source.

The order matters, too. The compiler stops at the first fatal error, so you fix one assumption at a
time and watch the next one appear. That ladder — `conio.h`, then `iostream.h`, then `void main` —
is the shape of the port, and you discover it instead of being told.

### Why "it ran" is not "it works"

Three separate claims hide inside that sentence:

- **It builds.** A binary exists. This is provable: the compiler exited 0.
- **It runs.** The process reached its own end. This is provable: exit status 0.
- **It is correct.** The behavior matches the specification. This is *not* provable from a run whose
  output you cannot compare to anything.

The third claim needs a contract. This project has one — the modern transcript — and the legacy
binary does not produce it. That is not a defect in the legacy program; it is a different output
format for a different consumer. Confusing the two is how a build lesson turns into a false claim.

## Before you start

Read these in full:

- [`README.md`](README.md) — the lesson card, the probe table and the measured output.
- [`docs/guide/archaeology-track.md`](../../../docs/guide/archaeology-track.md) — §2, the autopsy
  protocol, especially step 2.
- [`docs/archive.md`](../../../docs/archive.md) — the pinned archive and what it excludes.
- [`docs/guide/lesson-catalog.md`](../../../docs/guide/lesson-catalog.md) — the `common-03`
  prerequisite and the `archaeology-01` acceptance criterion.

You also need:

- a C++ compiler (`c++`, `g++` or `clang++`) and its version;
- `bash`, `timeout`, `wc` and `head` — all standard;
- a local clone of the archive at tag `v1.0.0`;
- the probe in [`code/`](code/), which ships with this lesson.

You do **not** need a Windows machine, an old compiler, or a virtual machine. You need to be honest
about what a modern machine can and cannot prove.

> **Rule:** the archive is read-only. Every change in this lesson happens in a scratch directory or
> in the probe's own files. If you find yourself editing
> `original/variant-01-baseline-2008/source/guerra-de-caballos-baseline.cpp`, stop.

## The 60-minute path

| Min | Step | What you do | Evidence |
|---|---|---|---|
| 0–10 | Predict | Write what you expect the program to do, before running it | Prediction note |
| 10–20 | Naive build | Try a modern compile; quote every error in order | Error ladder |
| 20–35 | Probe build | Read the shims, then build through the probe | Compatibility table |
| 35–45 | Run and observe | Run under a timeout; record status, volumes, first error | Run record |
| 45–52 | Compare | Prediction against observation, claim by claim | Comparison table |
| 52–60 | Explain and record | The boundary paragraph and `EVIDENCE.md` | Portfolio entry |

If the error ladder takes twenty minutes because you want to understand
each include, that is information about the ladder, not about you.

### The engineering check in this lesson

| Step | Where it happens | What it produces |
|---|---|---|
| **Predict** | Step 1 | Behavior, win condition and failure mode, in writing. |
| **Build and run** | Steps 2–4 | Exact errors, the compatibility table, the run record. |
| **Compare** | Step 5 | One verdict per predicted behavior. |
| **Explain** | Step 6 | Why the gap exists, with evidence. |
| **Record** | Step 7 | `EVIDENCE.md`, with the boundary stated. |

## Step 1 — Predict before you build (10 min)

You have the source and you have not run it. Write down, in your own words:

1. **The interface.** What does the player see? One screen, or a scrolling log?
2. **The interaction.** Does the program wait for input, and at which points?
3. **The randomness.** What is random, and is the game reproducible if you run it twice?
4. **The end condition.** How does the game finish, and what does it print when it does?
5. **The failure mode.** What do you expect to break first on Linux?

Then write one falsifiable sentence about the whole set. For example:

> I expect the program to draw a board, wait for a key between turns, place the knights randomly,
> and end when one knight lands on the other; on Linux I expect the Windows console calls to fail
> and the board symbols to print as garbage.

That sentence can be wrong in at least three places. Good. A prediction you cannot falsify teaches
nothing.

### A worked prediction

Take claim 2, the interaction, from the source alone:

- expected answer: yes, the program pauses between turns;
- expected evidence: `getch()` at line 42 and again at lines 781, 806, 835, 861;
- expected verdict on Linux: the pause disappears, because `getch()` is not available.

Read the includes before you decide: `conio.h` at line 11 is the header that provides `getch()`, and
it is the first thing a modern compiler will reject. The prediction and the error ladder are about
to meet.

## Step 2 — Run the naive build and record the ladder (10 min)

Copy the source out of the archive first. Never compile in place if you can avoid it, and never let
a build write into the archive:

```bash
mkdir -p /tmp/legacy-probe && cd /tmp/legacy-probe
cp /path/to/archive/original/variant-01-baseline-2008/source/guerra-de-caballos-baseline.cpp .
```

Then the naive attempt, with the locale pinned so the messages are reproducible:

```bash
LC_ALL=C c++ -std=c++20 -fsyntax-only guerra-de-caballos-baseline.cpp
```

Expected first error, verbatim:

```text
guerra-de-caballos-baseline.cpp:11:9: fatal error: conio.h: No such file or directory
   11 | #include<conio.h>
      |         ^~~~~~~~~
compilation terminated.
```

Note the line number: 11. The `conio.h` include comes *before* `iostream.h` at line 12, so the
compiler stops there and you never see the second missing header. That is why you fix one thing at a
time.

Now satisfy only `conio.h` and `iostream.h` with minimal shims and compile again:

```bash
mkdir -p compat
printf '#pragma once\ninline int getch(){return 0;}\n' > compat/conio.h
printf '#pragma once\n#include <iostream>\nusing namespace std;\n' > compat/iostream.h
LC_ALL=C c++ -std=c++20 -fsyntax-only -Icompat guerra-de-caballos-baseline.cpp
```

Expected second error, verbatim:

```text
guerra-de-caballos-baseline.cpp:32:1: error: '::main' must return 'int'
   32 | void  main()//funcion principal
      | ^~~~
```

Two errors, two different classes of problem:

| Error | Class | What it tells you |
|---|---|---|
| `conio.h` not found | missing header | The code targets a platform library that no longer exists. |
| `'::main' must return 'int'` | language rule | `void main()` was tolerated by the old compiler and is not C++ now. |

The second one is the interesting one. `-fpermissive` will downgrade it, and that is exactly what
`-fpermissive` means: *"I know this is not valid; build it anyway."* Record that you used it, and
record why. A build flag that suppresses a language rule is a compatibility change, not a
convenience.

### If your ladder differs

It probably will, slightly. Record what you saw, not what this page says:

- a different compiler may word the messages differently, or report `iostream.h` first;
- a non-English locale will translate the text — that is why the commands above pin `LC_ALL=C`;
- a compiler with `-std=gnu++20` as default may accept more before it complains.

The rule: the ladder is your ladder. Quote your output.

## Step 3 — Build through the probe (15 min)

The probe in [`code/`](code/) does four things: copies the source to a temporary directory, compiles
it with the compatibility headers, links a standard `int main` wrapper, and runs the result under a
timeout. Read all four files before you run it.

| File | What it does | Risk it introduces |
|---|---|---|
| `code/compat/iostream.h` | Maps the pre-standard header to `<iostream>` and injects `using namespace std;` | Global namespace injection can change overload resolution. |
| `code/compat/conio.h` | Provides an inline `getch()` that returns `0` immediately | The program no longer waits for a key; interaction is gone. |
| `code/legacy_wrapper.cpp` | Declares `legacy_main` and calls it from `int main` | The entry point is renamed, not repaired. |
| `code/build_probe.sh` | Copies, compiles, links, runs with `timeout 3s`, prints six lines | The timeout can truncate a slow run and report a false blocker. |

The compile line is the part to understand:

```bash
"${compiler}" -std=c++20 -fpermissive -Dmain=legacy_main \
    -I"${scriptDirectory}/compat" -c "${scratchDirectory}/legacy.cpp" \
    -o "${scratchDirectory}/legacy.o"
```

Three decisions live in those two lines:

- `-fpermissive` — accept `void main()` and other pre-standard constructs as warnings instead of
  errors.
- `-Dmain=legacy_main` — rename the entry point at preprocessing time, so the wrapper can own
  `int main` without editing the source. The archive file keeps its original bytes.
- `-Icode/compat` — put the shims ahead of the system headers. `iostream.h` and `conio.h` resolve to
  the lesson's files, not to anything the compiler ships.

None of these is a source change. That distinction is the point of the exercise: **the probe proves
the source can be adapted, not that the source was adapted.**

Now run it from the lesson directory:

```bash
./code/build_probe.sh /path/to/archive
```

A recorded run on the reference machine, six lines:

```text
source=<archive>/original/variant-01-baseline-2008/source/guerra-de-caballos-baseline.cpp
compile=ok
run_status=0
stdout_lines=614
stderr_lines=26
stderr_first=sh: línea 1: color: orden no encontrada
```

Do not expect your two line counts to match those. Only `compile=ok` and `run_status=0` are stable;
the volumes depend on how many turns the game took. Step 4 explains why, and how to prove it.

Read the sixth line carefully. It is not a compiler error and it is not this lesson's error. It is
the operating system reporting that the shell command `color` does not exist. The program called
`system("color 1f")` at line 38 and `system("cls")` at line 43, and on Linux those are shell-outs to
commands that were never installed. The message is localized, so your machine may print
`command not found` instead of `orden no encontrada`.

## Step 4 — Run, observe, and measure the variance (10 min)

The six lines above are a summary. Now look at the artifacts the probe deletes, by running the same
steps by hand once:

```bash
cd /tmp/legacy-probe
c++ -std=c++20 -fpermissive -Dmain=legacy_main -Icompat -c guerra-de-caballos-baseline.cpp
c++ -std=c++20 /path/to/lesson/code/legacy_wrapper.cpp guerra-de-caballos-baseline.o -o probe
timeout 3s ./probe < /dev/null > out.txt 2> err.txt; echo "status=$?"
wc -l out.txt err.txt
head -1 err.txt
```

Four observations to record, in this order.

**1. It exits on its own.** `status=0` means the process reached the end of `main` and returned. On
a machine where the program loops forever, `timeout` returns `124` instead. That difference — `0` vs
`124` — is the difference between a runnable build and a blocker, and it belongs in your record.

**2. The output volume is not stable.** Line counts move between runs: 614 and 26 in one session,
223 and 9 in another, 936 and 40 in a third. The count tracks how many turns the game took, and the
turn count depends on where the knights started. A single number is not a contract; it is a sample.

**3. Same second, same bytes.** Run the binary twice within one wall-clock second and the two
outputs are byte-identical. Wait two seconds and run again, and they differ. The cause is at line
76: `srand(time(NULL))` seeds from a clock with one-second resolution. This is not determinism. It
is non-determinism with a coarse clock, and mistaking one for the other is exactly the failure that
`common-01` exists to prevent.

**4. The symbols are raw CP437 bytes.** The board is drawn with `simcab1 = 21`, `simcab2 = 156` and
`sep = 219` (line 16). Those are single bytes: `0x15`, `0x9C`, `0xDB`. They are not UTF-8, so a
UTF-8 terminal shows replacement characters or nothing at all. Hexdump a board line and you will see
`dbdb dbdb 2020 ...` — the wall is `0xDB`, repeated. This is a text encoding problem, not a
rendering bug in your terminal.

**5. The screen is a scrollback.** The program intends to repaint one screen: `system("cls")` before
each turn. On Linux `cls` fails, nothing is cleared, and every turn is appended. The count of `cls`
failures in `stderr` equals the number of turns. You are reading a log of every screen the program
wanted to show, concatenated.

## Step 5 — Compare prediction and observation (7 min)

One table, one verdict per prediction. Use exactly three verdicts; do not invent a fourth.

| Verdict | Test | What to record |
|---|---|---|
| **Confirmed** | The observation matches what you wrote. | The line of output or the status that confirms it. |
| **Contradicted** | The observation is different, and you can say how. | Both versions, side by side. |
| **Unresolved** | You cannot tell from this run. | What you would need to observe. |

### Worked comparison

| # | Predicted | Observed | Verdict |
|---|---|---|---|
| 1 | One screen repainted | Append-only log, `cls` fails | Contradicted |
| 2 | Waits for a key between turns | No pause; `getch()` returns 0 | Contradicted |
| 3 | Random placement, not reproducible | Identical within one second, different after | Contradicted (in the interesting way) |
| 4 | Ends when a knight lands on the other | Prints a winner line, exits 0 | Confirmed |
| 5 | Console calls fail on Linux | 26 `stderr` lines, first is `color` | Confirmed |

Row 3 is the one to sit with. Your prediction was "not reproducible". The observation was
"reproducible sometimes". Both of you were partly right, and the reason is a one-second clock, not a
seed. That is a better mental model than either the prediction or the naive conclusion.

## Step 6 — Explain the boundary (5 min)

Write one paragraph, without syntax words, that answers: what did this build prove, what did it fail
to prove, and where exactly does the evidence stop?

Use this shape if you need one:

> The probe proves **\_\_\_**. It does not prove **\_\_\_**, because **\_\_\_**. The evidence stops
> at **\_\_\_**.

### The four claims this probe does not support

| Claim | Why the probe cannot support it |
|---|---|
| "The code is portable." | Two shims and a permissive flag are the opposite of portability. |
| "The output is correct." | Nothing compares it to a specification. The game ends; that is all. |
| "The historical toolchain was Visual C++ 6.0." | That claim lives in the private vault. The public archive cannot prove it. |
| "This reproduces the modern transcript." | The modern transcript is a `key=value` log; this prints CP437 bytes. |

The last row matters most for the rest of Track B. The modern contract in
[`common/traces/`](../../../common/traces/) looks like this:

```text
seed=1
player=Learner
roll_player=11
roll_machine=20
start=machine
turn=1 who=machine at=4,4 order=1,2,3,4,5,6,7,8
move=1 machine 4,4->3,6
```

The legacy binary cannot produce that. Not because it is broken, but because it was never asked to:
it was written for a human looking at a colored screen, not for a test harness reading lines. Any
future comparison between the two needs a reconstruction step, and that reconstruction is a separate
claim with its own evidence.

### The honest sentence

Finish with one sentence you would be willing to defend in a code review:

> *"The archive source compiles on a modern compiler through a two-header compatibility shim and a
> permissive flag, runs to completion with the console calls failing, and prints a CP437 board; it
> does not produce the modern transcript, and no run of it can prove the historical toolchain."*

If your sentence is shorter and stronger than that, check which clause you dropped.

## Step 7 — Record (5 min)

Fill [`EVIDENCE.md`](EVIDENCE.md). It asks for the environment, the prediction, the error ladder,
the compatibility table, the run record, the comparison, the boundary and the next step.

Two habits worth keeping:

- **Paste, do not paraphrase.** `compile=ok` is evidence. "It compiled fine" is a memory.
- **Record the version of everything.** Compiler, `bash`, `timeout`, the archive commit. A build
  result without versions is a rumor.

## The original in detail

Facts you will cite repeatedly, with line numbers from the pinned archive:

| Line | What is there | Why it matters for a build |
|---|---|---|
| 10–14 | `stdio.h`, `conio.h`, `iostream.h`, `stdlib.h`, `time.h` | Two of the five headers do not exist in modern C++. |
| 16 | `simcab1=21, simcab2=156, sep=219` | CP437 code points as `char`, not UTF-8. |
| 17–18 | `vectab[64]`, `vecjugadas[8]`, loose knight variables | Global state; the whole program is one unit. |
| 32 | `void  main()` | Not standard C++; the second error in the ladder. |
| 38 | `system("color 1f")` | A Windows shell command; fails on Linux. |
| 42 | `getch()` | Console input from `conio.h`; the shim removes the pause. |
| 43 | `system("cls")` | A Windows shell command; fails on Linux, once per turn. |
| 76 | `srand(time(NULL))` | One-second seed resolution; explains the variance pattern. |
| 180 | `(rand() % 8)+1` | Move-order randomization; another source of run-to-run difference. |
| 930 | end of file | No tests, no build file, no `main` returning `int`. |

The archive has no build file at all: no `Makefile`, no `CMakeLists.txt`, no project file in the
public set. That absence is itself a finding, and it belongs in `archaeology-02`, not here. Here it
only explains why this lesson ships a probe instead of a command from the archive.

## What counts as a blocker

The acceptance criterion is "a runnable build **or** a documented blocker". A blocker is acceptable
when it has all four parts:

1. **The exact command**, copy-pasteable, with flags and paths.
2. **The exact output**, quoted, not summarized.
3. **The environment**: compiler and version, OS, shell, archive commit.
4. **The next experiment** you would try, and what result would change your mind.

A blocker without part 4 is a dead end. A blocker with part 4 is a research note.

Typical blockers on this lesson, and what they look like:

| Symptom | Likely cause | What to record |
|---|---|---|
| `run_status=124` | The program never reaches the end within 3 s | That it ran, that it did not finish, and for how long you let it run |
| `compile=failed` | Compiler too old for `-std=c++20`, or a shim conflict | The compiler version and the first error |
| Link error on `legacy_main` | `-Dmain=legacy_main` did not apply to the compile step | The two commands, side by side |
| Empty `stdout` | The binary wrote to a different stream, or exited early | `run_status` and `stderr` |

## What this lesson does not do

- It does not diagnose a single defect. `for (i=0;i<=64;i++)` at line 61 is visible and it is not
  today's subject. Note it and move on.
- It does not fix anything. The archive is read-only and the probe is a build adapter.
- It does not claim portability. Two shims are not a port.
- It does not validate the output against a specification. That is `archaeology-03`.
- It does not settle the toolchain question. That boundary was set in `archaeology-00`.
- It does not produce the modern transcript. It shows why a reconstruction is needed.

If you catch yourself explaining *why* the board is wrong, stop and write it as a question for
`archaeology-04`.

## Self-check (binary)

- [ ] I wrote five predictions before running anything.
- [ ] I ran the naive build and quoted the first error verbatim, with the line number.
- [ ] I satisfied the headers one at a time and quoted the second error.
- [ ] I can explain what `-fpermissive` accepts and why that is a compatibility change.
- [ ] I can explain what `-Dmain=legacy_main` does and why the archive stays untouched.
- [ ] I ran `build_probe.sh` and recorded all six output lines.
- [ ] I recorded `run_status` and know what `124` would have meant.
- [ ] I observed that line counts vary, and I can name two causes.
- [ ] I observed that two runs in the same second are identical, and I know why that is not
      determinism.
- [ ] I confirmed the board is raw CP437 bytes, not UTF-8.
- [ ] I compared my prediction with the observation, with a verdict per row.
- [ ] I wrote the boundary paragraph, including the four claims the probe does not support.
- [ ] I filled `EVIDENCE.md`.
- [ ] The archive is unmodified (`git status` in the archive is clean).

If a box is not ticked, the unticked box names the next step. There is no penalty for repeating one.

## Misconceptions this lesson refutes

| Misconception | Why it is wrong | What to do instead |
|---|---|---|
| "If it compiles, the port is done." | Two shims and `-fpermissive` mean the code is being tolerated, not ported. | Call it an adapted build, and list the adaptations. |
| "`-fpermissive` is just a flag." | It downgrades language rules; the program builds in a state the standard forbids. | Record it as a semantic change, next to the shims. |
| "Exit status 0 means correct behavior." | It means the process reached its end. Correctness needs a contract. | Compare against a specification or a trace. |
| "The output is garbage, so the program is broken." | The bytes are valid CP437; your terminal speaks UTF-8. | Hexdump before you judge. |
| "Line counts differ, so the program is non-deterministic." | They also match within the same second. The seed has one-second resolution. | Name the clock, not the dice. |
| "A blocker means the lesson failed." | The acceptance criterion accepts a blocker with a command, output, environment and next step. | Write the four parts. |
| "The probe modifies the archive." | It copies to a temp directory and renames `main` at preprocessing time. | Verify with `git status` in the archive. |
| "The build proves the original toolchain." | The probe proves the current compiler can build the source; it does not prove the original toolchain. | Record both claims separately. |

## Terms used here

| Term | One-line meaning |
|---|---|
| Build | Source plus compiler plus headers plus flags, producing a runnable artifact. |
| Naive build | The first attempt, with no compatibility help, whose errors are the evidence. |
| Error ladder | The sequence of errors you get by fixing one assumption at a time. |
| Compatibility shim | A header supplied to satisfy an include the modern library dropped. |
| `-fpermissive` | A flag that downgrades some language errors to warnings. |
| Renamed entry point | `main` rewritten to `legacy_main` at preprocessing time, source untouched. |
| Timeout | A hard stop so a legacy loop cannot hang the session. |
| CP437 | The IBM code page the board symbols come from; not UTF-8. |
| Blocker | A reproducible failure with command, output, environment and next experiment. |
| Evidence boundary | The line between what a run proves and what it only suggests. |

## What “done” means here

The **Self-check (binary)** above is the authoritative checklist. If every box is ticked, the
lesson is done for you. `meta.yml` mirrors the same criteria for tooling.

## Next step

- Continue to `archaeology-02` — code inventory and call graph. You now know the code runs; the next
  question is which file copied which, and how confident you can be about it.
- If the run left you curious about the defect at line 61, write it down for `archaeology-04`. Do
  not chase it now; the protocol fixes the order for a reason.
- If `run_status` was `124`, re-run with a longer timeout before declaring a blocker. A slow
  machine is not a broken program, and a premature blocker is a false finding.
- If the CP437 output surprised you, hexdump one board line and one transcript line side by side.
  The difference between the two formats is the subject of `archaeology-03`.

## References

- [`README.md`](README.md) — the lesson card, the probe table and the measured output.
- [`EVIDENCE.md`](EVIDENCE.md) — the record you fill in.
- [`docs/guide/archaeology-track.md`](../../../docs/guide/archaeology-track.md) — §2, the autopsy
  protocol; §3, the robustness failures.
- [`docs/archive.md`](../../../docs/archive.md) — the pinned archive, commit and scope.
- [`common-03`](../../../common/lessons/common-03-build-and-run/lesson.md) — the prerequisite: one
  command that behaves the same from any directory.
- [`common/traces/`](../../../common/traces/) — the modern transcript contract, for comparison.
- Archive: <https://github.com/DaydreamsDev/DaydreamsDev-guerra-de-caballos-archive>, tag `v1.0.0`,
  source `original/variant-01-baseline-2008/source/guerra-de-caballos-baseline.cpp` (930 lines).
