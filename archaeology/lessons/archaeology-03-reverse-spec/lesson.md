# archaeology-03 — Reverse-engineer the spec

> [English](lesson.md) | [Español](lesson-es.md)

> **Status:** drafted · **Concept:** behavior specification · **Prerequisite:** archaeology-02 ·
> **Estimated time:** 90 minutes
> **Reading:** ~20 minutes

## Why this lesson exists

The archive contains no specification. It contains source code, a few reports and a README per
variant. If you want to know what the 2008 program did, you have two choices: guess from the folder
name, or read the code and state the rules it actually implements. This lesson is the second choice.

That sounds straightforward until you try it. The code mixes three things that a specification must
keep apart: the game rules, the random generator and the console presentation. It stores the board
as a flat `int[64]`, it marks visited cells with move numbers, it prints CP437 bytes, and it ends
the game by setting the same variable that counts moves to zero. A reader who copies the structure
into the spec has written an implementation description, not a behavior specification.

There is a second trap, and it is subtler. The laboratory has a modern specification in
[`common/spec/game-rules.md`](../../../common/spec/game-rules.md). It is clear, complete and
deterministic. The temptation is to read it first and then "find" it in the 2008 file. That is
confirmation bias with a citation. The modern spec is a **comparison target**, not evidence for what
the old program did. Read the old code first, write down what it does, and only then open the modern
document.

The product is two artifacts: a behavior spec reconstructed from the source, and at least one
captured legacy trace. Neither one alone is enough. The source tells you the rules; the trace shows
one seeded run of those rules. The spec without a trace is a reading. The trace without a spec is a
pile of bytes.

## What you will be able to do

By the end of this lesson you can:

- separate game rules, randomness and presentation in a legacy program;
- write a behavior clause that is observable and cites the source lines that support it;
- distinguish what the code does from what a comment, a folder name or the modern spec says it
  should do;
- capture a legacy trace with a fixed seed without editing the archive;
- explain what the seed patch changes and what it cannot change;
- read a CP437 board and reconstruct the first moves from it;
- compare the reconstructed spec with the modern spec and classify every difference;
- state the evidence limits in four categories: verified, inferred, documented only and unknown;
- record the spec, the trace and the comparison in `SPEC-TEMPLATE.md`, `TRACE-TEMPLATE.md` and
  `EVIDENCE.md`.

## The concept: behavior specification

A **behavior specification** is a contract stated in terms of what an observer can see: the board,
the pieces, the legal moves, the order of turns, the end conditions and the output. It is not a
description of the data structures, the function names or the control flow.

Four artifacts are easy to confuse. Keep them apart for the whole lesson.

| Artifact | Question it answers | Example in this project |
|---|---|---|
| **Specification** | What must happen? | "A capture ends the game and the capturing knight wins." |
| **Implementation** | How is it made to happen? | `ganador=0` inside the `movcab` case. |
| **Trace** | What happened in one run? | The 591 lines produced with seed 12345. |
| **Test** | How do we know the implementation still matches? | A harness that replays a seed and compares transcripts. |

The 2008 program has an implementation and, after you capture it, traces. It has no specification
and no test. That asymmetry is the archaeology.

### The code is evidence, not authority

In a maintained system, the specification is the authority and the code must follow it. In an
archaeology, the code is the only surviving witness. You do not get to say "the code is wrong
because the modern spec says otherwise". You say: "the code does X; the modern spec says Y; here is
the difference and here is what it costs."

That does not mean every line is intentional. The code can contain a defect, and the defect is part
of the behavior it actually produced. Record the behavior first. Diagnose it later, in
`archaeology-04`. The protocol fixes the order for a reason: description before judgment.

### Three layers inside one file

The 930 lines contain three layers. A useful spec names them separately.

| Layer | What belongs there | Where it lives in the source |
|---|---|---|
| **Game rules** | Board, pieces, legal moves, capture, trap, end of game | `movcab`, `ordenjuego`, `impresionorden` |
| **Randomness** | Generator, seed, draw order, ranges, redraw policy | `cabubicacion`, `vecjug` |
| **Presentation** | Symbols, board layout, messages, screen clearing, pauses | `impresion`, `impresion0`, `system`, `getch` |

The modern spec puts the rules and the random algorithm in one document and the output format in
another. The legacy program puts all three in one translation unit. Your job is to pull them apart.

### Determinism is part of the contract

A specification that says "random" is incomplete. A specification that says "draw a number from 0
to 50 with this generator, in this order, and redraw in this way" is reproducible. The difference
matters because the legacy program is **not** reproducible: it calls `srand(time(NULL))` and
`rand()`, and it has no seed argument.

The probe patches the seed in a scratch copy. That intervention makes a trace possible, and it also
changes the program. Record both facts. A trace produced by a patched program is evidence about the
patched program first, and about the original second. The gap between them is part of the lesson.

### The citation rule

Every clause in your spec must carry one of three marks:

- **Verified**: the source lines say it, and you quote them.
- **Inferred**: the source implies it, but no single line states it. Say what you inferred it from.
- **Unknown**: the source cannot answer it. Say what evidence would.

A clause without a mark is an opinion. The templates have room for the mark; use it.

## Before you start

Read these in full:

- [`README.md`](README.md) — the lesson card, the capture command and the acceptance criterion.
- [`docs/guide/archaeology-track.md`](../../../docs/guide/archaeology-track.md) — §2, the autopsy
  protocol, especially step 4, "Behavior spec".
- [`docs/archive.md`](../../../docs/archive.md) — the pinned archive, commit and scope.
- [`docs/guide/lesson-catalog.md`](../../../docs/guide/lesson-catalog.md) — the `archaeology-02`
  prerequisite and the `archaeology-03` acceptance criterion.

You also need:

- a C++ compiler (`c++`, `g++` or `clang++`) and its version;
- `bash`, `timeout`, `wc`, `sha256sum` and `grep` — all standard;
- a local clone of the archive at tag `v1.0.0`;
- [`SPEC-TEMPLATE.md`](SPEC-TEMPLATE.md) and [`TRACE-TEMPLATE.md`](TRACE-TEMPLATE.md);
- [`code/capture_trace.sh`](code/capture_trace.sh), which ships with this lesson.

> **Rule:** the archive is read-only. `capture_trace.sh` copies the source to a temporary directory
> and patches the seed there. It never writes into the archive. If you find yourself editing
> `original/variant-01-baseline-2008/source/guerra-de-caballos-baseline.cpp`, stop.

## The 90-minute path

| Min | Step | What you do | Evidence |
|---|---|---|---|
| 0–10 | Predict | Write what you expect the program to do, before reading the source | Prediction note |
| 10–30 | Inspect | Read the source in execution order; build the rule ledger | Rule ledger with line citations |
| 30–50 | Capture | Run `capture_trace.sh` with a fixed seed; read the board and messages | Trace file and capture record |
| 50–65 | Compare | Put the reconstructed rules next to the modern spec | Difference table |
| 65–80 | Explain | Classify the differences and state the evidence limits | Boundary paragraph |
| 80–90 | Record | Fill both templates and `EVIDENCE.md` | Portfolio entry |

If one function takes twenty minutes because the control flow is
tangled, that is information about the function, not about you.

### The engineering check in this lesson

Track B uses the same habit as Track A, with one substitution: the program is already built, so you
**capture** instead of build.

| Step | Where it happens | What it produces |
|---|---|---|
| **Predict** | Step 1 | Five falsifiable claims about rules, randomness and output. |
| **Inspect** | Step 2 | A rule ledger with line numbers and confidence marks. |
| **Capture** | Step 3 | A seeded trace, its status and its volumes. |
| **Compare** | Step 4 | A difference table against the modern spec. |
| **Explain** | Step 5 | Why the differences exist and where the evidence stops. |
| **Record** | Step 6 | The two filled templates and `EVIDENCE.md`. |

## Step 1 — Predict before you read (10 min)

You have the archive and you have not opened the source. Write five predictions:

1. **Board and pieces.** What size is the board, how many knights are there, and how are they shown?
2. **Start.** How is the first player chosen, and where do the knights start?
3. **Move priority.** How does a knight choose among its eight moves?
4. **Randomness.** What is random, and would two runs with the same seed be identical?
5. **End.** How does the game finish, and what does it print when it does?

Then write one falsifiable sentence about the whole set. For example:

> I expect an 8×8 board with two knights, a random roll to choose who starts, random placement, a
> rotated order of the eight knight moves, and an end when one knight captures the other; I expect
> the program to be non-reproducible because it seeds from the clock.

That sentence can be wrong in at least three places. Good. A prediction you cannot falsify teaches
nothing.

### A worked prediction

Take claim 4, randomness:

- expected answer: the program seeds from the clock and calls `rand()`;
- expected evidence: `srand(time(NULL))` and `rand()` in the placement function;
- expected verdict: without a seed argument, two runs are not comparable.

That prediction is specific enough to fail. If the program turns out to draw the same numbers within
one clock second, the prediction is wrong in an interesting way. Write it down before you look.

## Step 2 — Inspect the source (20 min)

Read the file in execution order, not from top to bottom. The declarations at the top are a map; the
behavior is in the functions.

| Order | Lines | What to read for |
|---:|---|---|
| 1 | 16–18, 29 | Constants, globals and the variables that carry state |
| 2 | 32–55 | Entry point and the outer game loop |
| 3 | 57–67 | Board initialization |
| 4 | 69–112 | Placement, rolls and index-to-coordinate conversion |
| 5 | 176–205 | Move-order generation |
| 6 | 226–232 | Coordinate-to-index conversion |
| 7 | 234–724 | Move evaluation, capture and trap |
| 8 | 726–751 | Who starts and how the starting cells are marked |
| 9 | 753–868 | The turn loop |
| 10 | 870–930 | Board printing |

As you read, fill a **rule ledger**. One row per behavior, with the lines that support it and a
confidence mark.

| Rule | Lines | Evidence | Mark |
|---|---:|---|---|
| The board is 8×8 | 874–875 | Two nested loops, `0..7` each | Verified |
| The board is stored as 64 cells | 17, 61–65 | `int vectab[64]` | Verified |
| A cell index is `row*8+column` | 226–232 | `conversionmv` | Verified |
| A knight move is one of eight offsets | 241–695 | Eight `case` bodies | Verified |
| A capture sets `ganador=0` | 254–258 | The check against the other knight | Verified |
| A trap ends the game | 702–717 | `jugada=0` after eight failed moves | Verified |

### Finding 1: the board is flat, but the display is a grid

The board is `int vectab[64]` (line 17). A cell is addressed by `row*8+column` (line 229). The
placement function converts an index to zero-based row and column with `cab/8` and `cab%8`
(lines 106–109), then prints `fila:cf+1` and `columna:cc+1` (lines 96–98). So the internal grid is
0..7 and the displayed grid is 1..8.

That is a presentation detail, not a rule change. The modern spec uses 1..8 directly. When you write
the spec, state the displayed coordinates and note the internal convention only if it affects
behavior.

### Finding 2: the starting cells are marked as visited

This is the finding most readers miss. In `ordenjuego`, the first player's starting cell is written
with `jugada` (1), then `jugada` is incremented to 2, and the second player's cell is written with 2
(lines 730–748). The first real move therefore gets move number 3.

Because the visited check is `vectab[ccab]==0 || vectab[ccab]==vectab[other]` (lines 252, 276), a
nonzero starting cell is treated as visited. A knight cannot re-enter its own starting cell, and the
other knight cannot enter it except to capture. The modern spec says starting cells are **not**
visited. That is a real rule difference, not an encoding difference.

### Finding 3: the move order is drawn once per pair of turns

The outer loop calls `vecjug()` once, then `impresionorden()` (lines 48–49). `impresionorden()`
handles both knights' turns (lines 756–868) and never calls `vecjug()` again. So the same eight-move
order is used for the first player's move and the second player's reply. The next outer iteration
draws a new order.

You can see it in the trace. With seed 12345, the first two turns both print
`El vector de movimiento es: 2,3,4,5,6,7,8,1.` (trace lines 41 and 64). The modern spec draws a new
order on every turn. That is another real difference.

### Finding 4: `rand() % 51` is not 1..50

Lines 78–79 read:

```cpp
tirocab1= rand() % 51;
tirocab2= rand() % 51;
```

The comment calls them "tiros" but does not state a range. `rand() % 51` produces 0..50 inclusive.
The modern spec says 1..50. The difference is small and real. Record the code's range, not the range
you expected from the modern document.

### Finding 5: the seed is reset inside the retry loop

`cabubicacion` runs a `while (i==1)` loop (line 73). Inside it, every iteration calls
`srand(time(NULL))` (line 76), then draws two rolls and two positions (lines 78–82). If either pair
collides, the loop repeats and reseeds from the clock (line 83).

On the original, a collision makes the program spin until the clock's one-second resolution changes.
With the probe's `srand(SEED)` patch, the same seed is applied on every iteration, so a collision
seed spins **forever**. Seed 9999 is such a seed: its two positions are both 19. The probe returns
`run_status=124` after the five-second timeout and produces no stdout. That is a probe artifact, not
proof that the original hangs.

### Finding 6: the game has no draw

`jugada` is both the move counter and the game-over flag. It starts at 1, becomes 3 after placement,
and increments after each successful move (lines 778, 803, 832, 858). A capture sets `ganador=0`
(lines 257, 282, and the equivalent blocks in the other cases). A trap sets `jugada=0` (lines 708,
716). There is no 200-move limit and no `draw` state. The game ends only on capture or trap.

Because every successful move marks a new cell and the board has 64 cells, a trap is inevitable
within a finite number of moves. The absence of a draw rule is still a specification difference.

### What to do when the source is ambiguous

The code has comments that narrate, comments that lie, and identifiers in Spanish with typos. Do not
resolve ambiguity by choosing the nicer reading. Write the ambiguity down:

- what the code does on the path you can trace;
- what the comment claims;
- which one you used, and why;
- what a second trace would test.

That is the difference between a spec and a guess.

## Step 3 — Capture a trace (20 min)

From this lesson directory, run:

```bash
./code/capture_trace.sh /path/to/archive 12345 /tmp/legacy-trace-12345.txt
```

The script copies the source to a temporary directory, replaces `srand(time(NULL));` with
`srand(12345);`, compiles with `-std=c++20 -fpermissive -Dmain=legacy_main -Icode/compat`, links the
wrapper, and runs the result with `timeout 5s`. It prints five lines.

A reference run on this machine:

```text
seed=12345
output=/tmp/legacy-trace-12345.txt
run_status=0
stdout_lines=591
stderr_lines=25
```

Your numbers will differ if your C library, compiler or machine differs. Only `run_status=0` and the
shape of the output are stable. The line counts are a sample, not a contract.

### What the five lines mean

| Line | Meaning | What it does not mean |
|---|---|---|
| `seed=12345` | The value substituted into the scratch copy | That the original accepted a seed argument |
| `output=...` | Where stdout was written | That the file is a modern transcript |
| `run_status=0` | The process reached its own end | That the behavior is correct |
| `stdout_lines=591` | Lines of board and messages | That 591 is a stable property |
| `stderr_lines=25` | Lines from failed `system` calls | That the program reported errors |

The 25 stderr lines are the Linux shell complaining about `color` and `cls`. The program calls
`system("color 1f")` once (line 38) and `system("cls")` between turns, plus once at startup
(lines 43, 784, 809, 838, 864). On Linux those commands do not exist, so each call writes one
localized error line. The count is one `color` plus one `cls` per move. It is not a program error.

### Read the trace

The first screen is a board, then the rolls and positions:

```text
Juega primero el caballo 2 <sym> porque:

El tiro del caballo 1 <sym> fue:

15

El tiro del caballo 2 <sym> fue:

44

El numero aleatorio para ubicar el caballo 1 <sym> fue:  45

En terminos de filas y columnas es: fila:6 columna:6

El numero aleatorio para ubicar el caballo 2 <sym> fue:  59

En terminos de filas y columnas es: fila:8 columna:4
```

`<sym>` stands for the raw CP437 byte. The actual file contains `0x15` for knight 1 and `0x9C` for
knight 2, not the text `<sym>`. A UTF-8 terminal shows replacement characters.

The rolls are 15 and 44. The higher roll is 44, so knight 2 starts. The positions are 45 and 59:
index 45 is row 6, column 6; index 59 is row 8, column 4. The board at the top of the trace shows
knight 2 with move number 1 and knight 1 with move number 2, because the first player's starting
cell is written first.

The trace ends with a capture:

```text
evaluo los casos: 6, 7, 8, 1.El  CABALLO 1 <sym> HA GANADO
```

The last board shows move number 26 on the capturing cell. The two starting placements consumed
numbers 1 and 2, so the game took 24 actual knight moves. That distinction belongs in your trace
record.

### Prove that the seed makes it reproducible

Run the same seed twice and compare the bytes:

```bash
./code/capture_trace.sh /path/to/archive 12345 /tmp/trace-a.txt
./code/capture_trace.sh /path/to/archive 12345 /tmp/trace-b.txt
sha256sum /tmp/trace-a.txt /tmp/trace-b.txt
```

The two hashes match. This is determinism **of the patched program**: same seed, same libc, same
compiler, same bytes. It is not determinism of the original, which seeds from the clock and has no
seed argument.

### Run a second seed and watch the variance

The line counts move with the seed because the game length moves with the seed. Measured on the
reference machine with glibc:

| Seed | `run_status` | stdout lines | stderr lines | End |
|---:|---:|---:|---:|---|
| 12345 | 0 | 591 | 25 | capture, knight 1 |
| 1 | 0 | 476 | 20 | trap |
| 7 | 0 | 729 | 31 | capture, knight 1 |
| 42 | 0 | 522 | 22 | capture, knight 1 |
| 777 | 0 | 269 | 11 | capture, knight 2 |
| 2024 | 0 | 1074 | 46 | trap |
| 9999 | 124 | 0 | 1 | placement loop did not finish |

Two rows deserve attention. Seed 1 ends in a trap, which is the only way to see the trap message
without a capture. Seed 9999 is the collision case from Finding 5: the patched program never leaves
the placement loop. A `124` here is a statement about the probe, not about the 2008 binary.

### What the trace proves

- The patched source compiles and runs on this machine.
- For this seed and this libc, the program produces a specific board and a specific ending.
- The board uses CP437 bytes and Spanish messages.
- The program has no transcript, no turn numbers and no coordinates in machine-readable form.

### What the trace does not prove

- That the original 2008 run produced these bytes.
- That the original used the same `rand()` sequence.
- That every seed ends the same way.
- That the modern transcript can be reproduced from this trace.

## Step 4 — Compare with the modern spec (15 min)

Open [`common/spec/game-rules.md`](../../../common/spec/game-rules.md) only now. Put the two
documents side by side and classify each clause as **same rule**, **different rule** or **not
observable in the legacy output**.

### What matches

These are genuine agreements, and they are worth stating:

- the board is 8×8;
- there are two knights, one per side;
- each knight moves with the same eight offsets;
- a move is legal only inside the board and onto an unvisited cell, except for a capture;
- the active knight tries the rotated order and takes the first legal move;
- a capture ends the game immediately;
- a trapped knight ends the game;
- after a successful move, the other knight moves;
- the displayed coordinates are 1..8.

### What differs

| # | Modern spec | Legacy source | Lines | Class |
|---:|---|---|---|---|
| 1 | 64-bit LCG with fixed constants | C library `rand()` | 78–82, 180 | Different generator |
| 2 | Seed is an input | `srand(time(NULL))`, no argument | 76 | Different control |
| 3 | Positions first, then rolls | Rolls first, then positions | 78–82 | Different draw order |
| 4 | Rolls 1..50 | Rolls 0..50 | 78–79 | Different range |
| 5 | Redraw only the colliding value | Redraw all four values | 83 | Different policy |
| 6 | New move order every turn | New order every pair of turns | 48–49, 753–868 | Different rule |
| 7 | Starting cells are not visited | Starting cells hold 1 and 2 | 730–748 | Different rule |
| 8 | Draw after 200 moves | No draw | 44, 708, 716 | Missing rule |
| 9 | Trap records the other knight as winner | Prints "quedo encerrado", no winner line | 702–717 | Different output |
| 10 | UTF-8 `key=value` transcript | CP437 board and Spanish prose | 870–930 | Different format |
| 11 | Move number starts at 1 | First real move is 3 | 730–748 | Different numbering |
| 12 | `player` / `machine` | `caballo 1` / `caballo 2` | 737, 747 | Different vocabulary |

### The three classes of difference

Not every row means the same thing. Sort them before you explain them.

**Encoding differences.** The same rule, expressed differently. Displayed coordinates are 1..8 in
both, but the legacy stores 0..7. The winner is the same knight, but the labels differ. A move is
the same offset, but the legacy prints a move number instead of a coordinate pair. These differences
matter for a transcript adapter, not for the game.

**Rule differences.** The behavior itself is different. Starting cells are visited in the legacy and
not in the modern spec. The move order is drawn per pair of turns, not per turn. Rolls are 0..50,
not 1..50. There is no draw. These rows make a naive "same seed, same game" claim false.

**Unobservable differences.** The legacy output cannot tell you some things at all: whether the
original code intended a draw rule, whether the 0..50 range was deliberate, or what the original
`rand()` did on the legacy toolchain. Mark these unknown. Do not fill them with the modern spec.

### The randomness boundary

This is the deepest difference. The modern spec defines a 64-bit linear congruential generator with
named constants. The legacy program calls `rand()`, whose algorithm is implementation-defined. On
glibc it is an additive feedback generator; on the Microsoft C runtime of the era it was a different
LCG. Even if you patch the same seed into both, the sequences diverge.

So the legacy trace is **not** a golden trace for the modern engine. It is a behavioral sample of a
different program. You can compare the rules, the board layout and the end conditions. You cannot
compare the number sequences. If you want to prove that the modern engine reproduces the legacy
game, you need a common generator and a transcript format that the legacy program never had.

That conclusion is the lesson. The modern spec is a designed contract; the legacy program is an
implementation that never had one. The differences are not automatically defects. Some are choices,
some are accidents, and some are simply lost.

## Step 5 — Explain the limits (15 min)

Write one paragraph that answers: what did the source prove, what did the trace show, what did you
infer, and where does the evidence stop?

Use this shape if you need one:

> The source proves **\_\_\_**. The trace shows **\_\_\_** for seed **\_\_\_**. I inferred
> **\_\_\_** from **\_\_\_**, with **\_\_\_** confidence. The evidence stops at **\_\_\_**.

### The four categories

Put every claim you rely on into exactly one category.

| Category | Test | Examples from this lesson |
|---|---|---|
| **Verified from source** | You can quote the lines. | The eight offsets; the `rand()%51` range; the starting-cell writes; the trap branch. |
| **Observed in a trace** | You ran it and read the bytes. | The 591 lines for seed 12345; the rolls 15 and 44; the capture by knight 1. |
| **Inferred from structure** | The code implies it, but no line states it. | That the same move order is used for both turns; that the starting cells count as visited. |
| **Unknown** | The source cannot answer it. | The original `rand()` sequence; the original intent; whether a draw rule was considered. |

### The four claims the trace does not support

| Claim | Why this trace cannot support it |
|---|---|
| "This is what the 2008 program printed." | The seed was patched and the build uses a modern libc. |
| "The modern engine can reproduce this trace." | The generators are different; the formats are different. |
| "The game is deterministic." | Only the patched program is; the original seeds from the clock. |
| "The rules are correct." | Correctness needs a contract; this lesson reconstructs one, it does not validate it. |

### The honest sentence

Finish with one sentence you would defend in a code review:

> *"The 2008 source implements an 8×8 two-knight game with a C-library `rand()`, a clock seed, a
> per-pair move order and starting cells marked as visited; a patched build with seed 12345 produces
> a CP437 board and ends in a capture, and the trace is evidence about that patched build, not about
> the original toolchain or the modern transcript."*

If your sentence is shorter and stronger than that, check which clause you dropped.

## Step 6 — Record (10 min)

Fill three documents:

1. [`SPEC-TEMPLATE.md`](SPEC-TEMPLATE.md) — the reconstructed behavior spec, with line citations and
   confidence marks.
2. [`TRACE-TEMPLATE.md`](TRACE-TEMPLATE.md) — the capture record, the observed start, the first
   moves, the end and the limits.
3. [`EVIDENCE.md`](EVIDENCE.md) — the portfolio record: setup, prediction, rule ledger, capture,
   comparison, limits and next step.

Two habits worth keeping:

- **Cite the line, not the memory.** "The starting cells are visited" is a claim. `ordenjuego`,
  lines 730–748, is evidence.
- **Record the environment.** Compiler, libc, archive commit, seed and command. A trace without an
  environment is a rumor.

## The original in detail

Facts you will cite repeatedly, with line numbers from the pinned archive:

| Line | What is there | Why it matters for the spec |
|---:|---|---|
| 16 | `simcab1=21`, `simcab2=156`, `sep=219` | CP437 symbols, not UTF-8. |
| 17–18 | `vectab[64]`, `vecjugadas[8]`, loose knight variables | The state is global. |
| 32 | `void main()` | Not standard C++; the probe renames it. |
| 38 | `system("color 1f")` | Windows console call; fails on Linux. |
| 42 | `getch()` | Pause; the shim makes it a no-op. |
| 43 | `system("cls")` | Screen clear; fails on Linux, once per turn. |
| 44 | `while (jugada!=0)` | The game loop; `jugada` is also the move counter. |
| 61 | `for (i=0;i<=64;i++)` | Off-by-one; diagnose it in `archaeology-04`. |
| 76 | `srand(time(NULL))` | Clock seed, inside the retry loop. |
| 78–79 | `rand() % 51` | Rolls 0..50, not 1..50. |
| 81–82 | `rand() % 64` | Positions 0..63. |
| 83 | Collision check | Redraws all four values. |
| 104–112 | `conversionvm` | Index to zero-based row and column. |
| 176–205 | `vecjug` | Rotated move order, one draw per call. |
| 226–232 | `conversionmv` | `row*8+column`. |
| 234–724 | `movcab` | Eight cases, capture, trap. |
| 252 | `vectab[ccab]==0` or the other knight's cell | Visited check; starting cells are nonzero. |
| 257 | `ganador=0` | Capture flag. |
| 702–717 | Trap branch | Prints "quedo encerrado", sets `jugada=0`. |
| 730–748 | `ordenjuego` | First player's cell gets 1, second gets 2. |
| 753–868 | `impresionorden` | Two turns per call, one order vector. |
| 870–930 | `impresion` | 8×8 board with move numbers and knight symbols. |

## What counts as a spec clause

A clause is testable when an observer could decide whether it happened.

| Weak clause | Why it is weak | Strong clause |
|---|---|---|
| "The board is an `int[64]`." | That is a data structure, not behavior. | "The board is 8×8; a cell is `row,column` with both from 1 to 8." |
| "The code calls `rand()%51`." | That is an implementation detail unless the generator is part of the contract. | "The legacy rolls are drawn from 0..50 inclusive, in this order." |
| "It prints the board." | Not testable. | "It prints eight rows; each cell is four `0xDB` bytes and two spaces when empty." |
| "The knight moves randomly." | Not testable. | "The order is a rotation starting at a value drawn from 1..8." |
| "The game ends." | Not testable. | "The game ends on capture or trap; there is no draw." |

The generator is the exception. For a reproducible spec, the generator is part of the contract, so
"the code calls `rand()`" becomes "the legacy uses the platform's `rand()`, which is
implementation-defined". That is a boundary, not a detail.

## What this lesson does not do

- It does not diagnose a single defect. The off-by-one at line 61 is visible and it is not today's
  subject. Note it and move on.
- It does not fix anything. The archive is read-only and the probe is a capture adapter.
- It does not claim the legacy trace is a golden trace for the modern engine.
- It does not prove the 2008 runtime behavior, the original `rand()` sequence or the toolchain.
- It does not infer the original intent. It records the behavior and the limits.
- It does not validate the modern spec. It uses it as a comparison target.

If you catch yourself explaining *why* a rule is wrong, stop and write it as a question for
`archaeology-04`.

## Self-check (binary)

- [ ] I wrote five predictions before opening the source.
- [ ] I read the source in execution order and built a rule ledger with line citations.
- [ ] I separated game rules, randomness and presentation.
- [ ] I recorded that the starting cells are marked as visited.
- [ ] I recorded that the move order is drawn once per pair of turns.
- [ ] I recorded that the rolls are 0..50, not 1..50.
- [ ] I explained why `srand(time(NULL))` is inside the retry loop.
- [ ] I captured at least one trace with `capture_trace.sh` and recorded all five lines.
- [ ] I ran the same seed twice and confirmed identical bytes.
- [ ] I ran a second seed and recorded how the line counts changed.
- [ ] I explained the seed 9999 `run_status=124` as a probe artifact.
- [ ] I read the CP437 board and reconstructed the first moves.
- [ ] I compared the reconstructed spec with `common/spec/game-rules.md`.
- [ ] I classified each difference as encoding, rule or unobservable.
- [ ] I stated the randomness boundary and why the legacy trace is not a modern golden trace.
- [ ] I filled `SPEC-TEMPLATE.md`, `TRACE-TEMPLATE.md` and `EVIDENCE.md`.
- [ ] The archive is unmodified (`git status` in the archive is clean).

If a box is not ticked, the unticked box names the next step. There is no penalty for repeating one.

## Misconceptions this lesson refutes

| Misconception | Why it is wrong | What to do instead |
|---|---|---|
| "The modern spec tells you what the old program did." | The modern spec is a designed contract; the old program never had one. | Read the source first, then compare. |
| "The code is the spec." | The code is an implementation; it contains defects and presentation details. | State observable behavior, then cite the lines. |
| "A comment is documentation." | Comments narrate, lie or describe an older version. | Quote the code path, not the comment. |
| "`rand()%51` means 1..50." | It means 0..50 inclusive. | Record the actual range. |
| "The trace is deterministic, so the program is." | Only the patched build is; the original seeds from the clock. | Separate the probe from the original. |
| "Same seed means same game in both implementations." | The generators and the draw order differ. | Compare rules, not number sequences. |
| "A `124` means the original hangs." | The probe's `srand(SEED)` turns a collision into an infinite loop. | Record it as a probe artifact. |
| "The starting cells are empty." | They hold move numbers 1 and 2 and count as visited. | Check `ordenjuego` and the visited test. |
| "The board prints a trail." | It prints move numbers; the trail is implicit in nonzero cells. | Describe the actual cell content. |
| "No draw rule means the game can run forever." | The board is finite and every move marks a cell; a trap ends it. | State the finite argument and the missing rule separately. |

## Terms used here

| Term | One-line meaning |
|---|---|
| Behavior specification | A contract stated in observable terms, independent of implementation. |
| Rule ledger | A table of behaviors with the source lines that support each one. |
| Trace | The observable output of one run with a fixed seed. |
| Determinism | Same inputs, same outputs, every time. |
| Seed | The value that initializes the random generator. |
| Generator | The algorithm that produces the random sequence. |
| Draw order | The sequence in which random values are requested. |
| Redraw policy | Which value is drawn again after a collision. |
| Encoding difference | The same rule expressed in a different format. |
| Rule difference | A behavior that is genuinely different. |
| Evidence boundary | The line between what you verified and what you inferred. |
| Golden trace | A trace used as a regression contract for a specific implementation. |

## What “done” means here

The **Self-check (binary)** above is the authoritative checklist. If every box is ticked, the
lesson is done for you. `meta.yml` mirrors the same criteria for tooling.

## Next step

- Continue to `archaeology-04` — correctness failures. You now have a spec and a trace; the next
  question is which behaviors are defects, and what they cost.
- If the trap ending surprised you, run seed 1 and read the last board. The trap is the second end
  condition, and it has no winner line.
- If the move-order finding was hard to see, put trace lines 41 and 64 side by side. The same vector
  for two turns is the whole evidence.
- If you want to test the randomness boundary, implement the modern LCG and print the first four
  values for seed 12345. Compare them with the legacy trace. They will not match.
- If `run_status` was `124`, re-run with a different seed before declaring a blocker. The collision
  is a property of the seed, not of the program.

## References

- [`README.md`](README.md) — the lesson card, the capture command and the acceptance criterion.
- [`SPEC-TEMPLATE.md`](SPEC-TEMPLATE.md) — the behavior spec you fill in.
- [`TRACE-TEMPLATE.md`](TRACE-TEMPLATE.md) — the trace record you fill in.
- [`EVIDENCE.md`](EVIDENCE.md) — the portfolio record you fill in.
- [`docs/guide/archaeology-track.md`](../../../docs/guide/archaeology-track.md) — §2, the autopsy
  protocol; §3, the failure taxonomy.
- [`docs/archive.md`](../../../docs/archive.md) — the pinned archive, commit and scope.
- [`common/spec/game-rules.md`](../../../common/spec/game-rules.md) — the comparison target.
- [`common/spec/transcript-format.md`](../../../common/spec/transcript-format.md) — the modern
  output contract the legacy program does not produce.
- [`archaeology-02`](../archaeology-02-code-inventory/lesson.md) — the prerequisite: inventory
  and lineage.
- Archive: <https://github.com/DaydreamsDev/DaydreamsDev-guerra-de-caballos-archive>, tag `v1.0.0`,
  commit `284e144e41403afbf0cc44422876425cacfb2bd5`,
  source `original/variant-01-baseline-2008/source/guerra-de-caballos-baseline.cpp` (930 lines).
