# archaeology-00 — Orientation: read a legacy codebase

> [English](lesson.md) | [Español](lesson-es.md)

> **Status:** drafted · **Concept:** mental model · **Prerequisite:** common-04 ·
> **Estimated time:** 45 minutes
> **Reading:** ~17 minutes
> **Lesson role:** core

## Why this lesson exists

A legacy file does not arrive with a manual. It arrives as one translation unit, a pile of
prototypes, a few file-scope names and a `main` that calls things you have never seen. The first job
is not to judge it, fix it or explain its history. The first job is to build a model good enough to
answer: **what is this program, and where would I look next?**

Every later lesson in this track asks a question that starts with "where". Where is the state? Where
does a move happen? Where does output come from? Without a map, each question restarts the reading
from zero. With a map, each question lands on a line.

This lesson is the first pass. It produces three things:

1. a **code map** with the entry point, functions, state and data flow;
2. a **one-sentence purpose** you can defend against the source;
3. a **next question** specific enough to answer in the following lesson.

It does not diagnose a defect, propose a repair or rate the code. Those are later jobs, and they are
cheaper once the map exists.

## What you will be able to do

By the end of this lesson you can:

- write a falsifiable prediction about an unfamiliar program before reading its function bodies;
- locate the entry point and state what starts and ends the program;
- read the prototypes as a table of contents and group the functions by job;
- fill one row per function with purpose, callers, callees, reads and writes;
- separate mutable globals from read-only file-scope constants;
- trace one path from input to output through the state that changes;
- record a build/run result, including a build that does not succeed;
- compress the whole reading into one sentence that predicts behavior;
- write a next question that names a file, a line or a symbol.

## The concept: mental model

A **mental model** is a small set of claims that lets you predict what a program will do and where a
change will land. It is not a summary of the file. A summary tells you what exists; a model tells
you how the parts relate.

A useful model has four layers:

| Layer | Question it answers | What the map records |
|---|---|---|
| **Boundary** | Where does the program start and stop? | Entry file, entry function, exit condition. |
| **Units** | What are the named pieces and what do they do? | One row per function: purpose, callers, callees. |
| **State** | What survives between calls? | Globals and structures, with writers and readers. |
| **Flow** | How does input become output? | Input, transformation, output, mutation sites. |

The four layers are read in that order. The boundary is cheap and tells you where the story begins.
The units give you vocabulary. The state explains why the units cannot be read in isolation. The
flow turns the vocabulary into a sentence.

### A map is not a summary

Compare these two statements about the same file:

- **Summary:** "The file has twelve functions and fifteen file-scope names."
- **Map:** "`main` calls `vecjug`, then `impresionorden`; `impresionorden` sets `cab` and calls
  `movcab`; `movcab` writes `vectab` and `ganador`; `main` loops while `jugada != 0`."

The first is inventory. The second is a model: it lets you predict that a change to `cab` affects
the move function, and that the loop ends when something writes `jugada = 0`. Inventory is a
by-product of the map, not the goal.

### Confidence has four levels

Every claim in your map carries a confidence. Use these four labels and do not blur them:

| Label | Meaning | Example |
|---|---|---|
| **Read** | The claim is visible at a cited line. | `ganador = 0` appears at line 257. |
| **Ran** | You observed it in a build or a run. | The naive build stopped at `conio.h`. |
| **Inferred** | The claim follows from call sites, not from a body you read. | `main` reaches `movcab` through `impresionorden`. |
| **Unknown** | You cannot tell yet. | Which branch runs first at runtime. |

The distinction matters because a model built from reading and a model built from running fail in
different ways. A read claim can be checked by opening the file. A run claim is tied to your
machine. An inference can be wrong even when both of its endpoints are right.

### Reading order beats reading volume

Do not read the file from line 1 to line 930. Three passes are faster and produce a better model:

1. **Boundary pass.** Find `main`, read it completely, and note every call it makes.
2. **Units pass.** Read the prototypes and the first and last lines of each function. Group the
   functions by job. Do not read long bodies yet.
3. **State and flow pass.** List the file-scope names, then follow one value from input to output.

The long bodies come last, and only the parts your map cannot explain. A 491-line function with
eight near-identical cases is not 491 lines of new information; it is one idea repeated eight times.

## Before you start

Read these in full:

- [`README.md`](README.md) — the lesson card and the acceptance criterion.
- [`CODE-MAP-TEMPLATE.md`](CODE-MAP-TEMPLATE.md) — the map you will fill.
- [`EVIDENCE.md`](EVIDENCE.md) — the record you will fill.
- [`archaeology/legacy/README.md`](../../legacy/README.md) — where the pinned source lives.

You also need:

- a local clone of the pinned source;
- a C++ compiler (`c++`, `g++` or `clang++`) for the light build check;
- `wc`, `grep` and a text editor;
- a scratch directory outside the source tree.

> **Rule:** the source is read-only. You may copy it to a scratch directory to build it. You may not
> edit the original, rename its files or write build output into its tree.

## The 45-minute path

| Min | Step | What you do | Evidence |
|---|---|---|---|
| 0–7 | Predict | Read the declarations only; write five predictions | Prediction table |
| 7–18 | Inspect | Read the boundary, the prototypes and the functions in order | Reading notes |
| 18–30 | Map | Fill one row per function and one row per global | `CODE-MAP-TEMPLATE.md` |
| 30–36 | Build and run | Attempt the naive build; record the result or the blocker | Build record |
| 36–42 | Explain | Write the one-sentence purpose and test it | Purpose sentence |
| 42–45 | Record | Write the next question and fill `EVIDENCE.md` | Portfolio entry |

If the map takes twenty minutes because `movcab` is large, that is information about `movcab`, not
about you. The time box is a guide for the first pass, not a hard cutoff.

### The engineering check in this lesson

| Step | Where it happens | What it produces |
|---|---|---|
| **Predict** | Step 1 | Five claims about behavior, state and failure, in writing. |
| **Inspect** | Step 2 | The boundary, the function groups and the names you must explain. |
| **Map** | Step 3 | One row per function and per global, with confidence labels. |
| **Explain** | Step 5 | A one-sentence purpose that predicts behavior. |
| **Record** | Step 6 | `EVIDENCE.md`, the build result and the next question. |

## Step 1 — Predict before you read the bodies (7 min)

Look only at three things:

1. the file name and its line count (`wc -l`);
2. the include list and the file-scope declarations (the first 30 lines);
3. the prototypes, if the file has them.

Do not read a function body yet. The declarations are enough to make a first guess, and a guess made
before the bodies is the only one you can falsify cleanly.

Write answers to these five questions:

1. **Purpose.** In one line, what does this program do?
2. **Interface.** Does the player see one screen, a scrolling log, or no output at all?
3. **State.** Which declarations look like mutable state, and which look like constants?
4. **Boundary.** What starts the program, and what condition ends it?
5. **Failure.** What do you expect to break first on your machine?

Then write one falsifiable sentence about the whole set. For example:

> I expect a console game that places two knights on an 8x8 board, moves them in a random order and
> ends when one lands on the other; I expect the build to fail on a missing console header before
> anything runs.

That sentence can be wrong in at least three places. Good. A prediction you cannot falsify teaches
nothing.

### A worked prediction from the declarations

Take the first thirty lines:

- `const char simcab1=21, simcab2=156, sep=219;` — three file-scope names whose values look like
  character codes. The `const` says they are read-only. They are print symbols, not state.
- `int vectab[64], vecjugadas[8];` — two arrays. A 64-cell array next to an 8-cell array suggests a
  board and a list of moves.
- `int cab1, cab2, jugada=1, orden, ganador=1;` — loose integers. Names like `cab1` and `cab2`
  suggest two pieces; `jugada` is initialized to 1 and looks like a counter; `ganador` starts at 1
  and may be a flag.
- The prototypes name their own jobs: `inicializacion`, `cabubicacion`, `impresion`, `vecjug`,
  `movcab`, `ordenjuego`, `impresionorden`.

From that alone you can predict a board game with two pieces, a random placement, a move generator
and a console renderer. You cannot yet predict the win condition or the order of turns. Write both
as questions, not as facts.

## Step 2 — Inspect (11 min)

### Read the boundary first

Open `main`. On the pinned source it begins at line 32. Read every line of it before anything else,
and answer four questions:

- What is the first call?
- What is the last call?
- What is the loop condition?
- What can change the loop condition?

The body is short:

```cpp
void main()
{
    system("color 1f");
    inicializacion();
    cabubicacion();
    cout << endl;
    getch();
    system("cls");
    while (jugada != 0)
    {
        if (jugada != 0)
        {
            vecjug();
            impresionorden();
        }
    }
}
```

The excerpt is normalized: the source uses tabs, blank lines and inline comments.

Four facts come out of it immediately:

1. The program is a loop gated by one global, `jugada`.
2. Setup is two calls: clear the board, then place the pieces.
3. Most output is produced inside the calls; `main` prints only a newline.
4. The program has no `return`; it ends when the loop condition becomes false and control falls off
   the end of `main`.

That last fact is a boundary claim, not a defect report. Record it as "ends by falling off the end",
with the line number.

### Read the prototypes as a table of contents

The prototypes at lines 19–30 are a compressed index of the file. Group them by job instead of
reading them alphabetically:

| Job | Functions | What the group tells you |
|---|---|---|
| Setup | `inicializacion`, `cabubicacion`, `ordenjuego` | Clear state, place pieces, decide who starts. |
| Move generation | `vecjug`, `imprevecjug` | Build and print the order in which moves are tried. |
| Move execution | `movcab`, `conversionmv`, `conversionvm` | Try a move, convert between board indices and row/column pairs. |
| Output | `impresion`, `impresion0`, `impresionorden` | Draw the board, draw the capture state, orchestrate the turn. |

The grouping is a hypothesis. You will correct it when you read the bodies. That is the point: a
table of contents is allowed to be wrong, as long as the map records the correction.

### Read functions in an order that adds one idea at a time

Use this order. Each function adds one new kind of relationship, so the map grows instead of
restarting:

1. **`inicializacion`** — the simplest writer. One global array, no parameters, one loop.
2. **`conversionvm` and `conversionmv`** — a contrast pair. One writes four globals and returns
   nothing; the other takes two parameters, touches no global and returns one value. The pair shows
   you the difference between a function with hidden outputs and a pure conversion.
3. **`vecjug` and `imprevecjug`** — a writer and a reader of the same array. Read them together.
4. **`ordenjuego`** — a function that decides, mutates the board, increments the move counter and
   prints. It is the first place where "one function, several jobs" becomes visible.
5. **`impresion` and `impresion0`** — pure readers of the board that print. The second is the
   capture variant; read the loops and note the extra flag that prevents drawing both pieces twice.
6. **`movcab`** — read case 1 and the `default` branch only. Then look at the shape of the other
   seven cases. They repeat the same guard, the same conversion and the same board write.
7. **`impresionorden`** — the orchestrator. Read it after `movcab`, because it sets the turn
   selector and calls the move function.
8. **`main` again** — now the loop condition has a writer you can name.

### Skip repetition on purpose

`movcab` runs from line 234 to line 724. Most of that is eight cases with the same structure:

```text
check the case number and the game flag
check the board boundary
convert the candidate cell
check the turn selector
check the destination cell
maybe set the winner flag
print the case number
update the row and column
write the move number into the board
break
```

Read one case completely. For the other seven, confirm that the offsets change and the shape does
not. Record "eight repeated cases" as a structural observation, not as eight separate findings. The
repetition is the subject of a later lesson; here it only tells you where not to spend your first
pass.

### Mark what you did not read

At the end of the pass, write three lists:

- **Read in full:** `main`, the two conversions, `vecjug`, `imprevecjug`, `ordenjuego`, one case of
  `movcab`, `impresionorden`.
- **Read in part:** `movcab` (one case plus the default), `impresion` and `impresion0` (the loops).
- **Not read:** the seven remaining cases, the exact print formatting, the console escape behavior.

A map that says "not read" is more trustworthy than one that implies total coverage. The unread list
is also your next-question shortlist.

## Step 3 — Map (12 min)

Fill [`CODE-MAP-TEMPLATE.md`](CODE-MAP-TEMPLATE.md). The template has six sections; the work is in
sections 2 and 3. It shows three sample rows in each table; add rows until every function and every
file-scope name has one.

### One row per function

For every function, fill purpose, called by, calls, reads and writes. Use names, not line ranges, in
the relationship columns; put line numbers in the notes.

Worked rows:

| Function | Purpose | Called by | Calls | Reads | Writes |
|---|---|---|---|---|---|
| `main` | Set up, then loop until the game ends | operating system | `inicializacion`, `cabubicacion`, `vecjug`, `impresionorden` | `jugada` | nothing directly |
| `conversionmv` | Convert a row/column pair to a board index | `movcab` | none | parameters `cf`, `cc` | local `ccab` |
| `movcab` | Try the eight moves in order; apply the first legal one | `impresionorden` | `conversionmv`, `impresion` | `vecjugadas`, `jugada`, `cab`, `vectab`, `cab1`, `cab2` | `cf`, `cc`, `vectab`, `ganador`, `jugada` |

Three rows are enough to show the shape. The first row is the boundary. The second is the pure
function. The third is the coupled one. If your map has only the first kind, you have described the
file, not modelled it.

### One row per global

For every file-scope name, record type, written by, read by and why it exists. Then separate state
from constants:

| Name | Type | Written by | Read by | Why it exists |
|---|---|---|---|---|
| `vectab` | `int[64]` | `inicializacion`, `ordenjuego`, `movcab` | `movcab`, `impresion`, `impresion0` | Board cells; stores move numbers and piece positions. |
| `jugada` | `int` | `ordenjuego`, `movcab`, `impresionorden` | `main`, `ordenjuego`, `movcab`, `impresionorden` | Move counter, loop sentinel and a special-case value. |
| `simcab1`, `simcab2`, `sep` | `const char` | never | `cabubicacion`, `ordenjuego`, `impresion`, `impresion0`, `impresionorden` | Print symbols. Read-only; not state. |

The third row is the precision check. A file-scope name is not automatically mutable state. If it is
`const` and never assigned after initialization, it belongs in the map as a constant, with a note
saying you excluded it from the state table.

### Follow one value end to end

Pick one value and trace it. The cheapest is `jugada`, because it appears in the loop condition, in
the board writes and in the move function. Write the path as a chain:

```text
main (44) -> while jugada != 0
  ordenjuego (733, 735) -> jugada++ twice during placement
  movcab (269) -> vectab[ccab] = jugada
  movcab (708, 716) -> jugada = 0 when no move is possible
  impresionorden (772, 797) -> jugada = 0 when a piece is captured
main (44) -> loop exits
```

That chain is a data-flow claim, and it is testable: every arrow has a line. If an arrow has no
line, mark it **Inferred** and write what you would read to confirm it.

### The rule for blanks

A blank cell is a question, not a failure. Write `?` and one of the confidence labels. At the end
of the pass, the `?` cells are your next-question list. A map with three honest `?` cells is better
than a map with three invented answers.

## Step 4 — Build and run, lightly (6 min)

This step produces one field of the map: **build/run result**. It is not the build lesson. The goal
is to know whether the source runs on your machine, and to record the first observable fact either
way.

Copy the source to a scratch directory, then try the naive build:

```bash
mkdir -p /tmp/legacy-orientation && cd /tmp/legacy-orientation
cp /path/to/source/guerra-de-caballos-baseline.cpp .
LC_ALL=C c++ -std=c++20 guerra-de-caballos-baseline.cpp -o legacy
```

On a modern toolchain the first error is expected to be a missing header:

```text
guerra-de-caballos-baseline.cpp:11:9: fatal error: conio.h: No such file or directory
```

Record the exact command and the exact first error. Do not fix it here. The adapted build, the
compatibility headers and the timeout belong to `archaeology-01`; this lesson only needs the result.

There are three acceptable outcomes:

| Outcome | What you record | What it means for the map |
|---|---|---|
| **Builds and runs** | Build command, run command, first output line, exit status | The boundary claim is confirmed by a run. |
| **Builds, does not run** | Build command, run command, first error or timeout | The boundary is read, not observed. |
| **Does not build** | Build command, first error verbatim, line number | The build result is a blocker; the map is still valid. |

A blocker is a result. "It doesn't build" is not; the quoted error is. If the build succeeds, run it
under a timeout so a legacy loop cannot hang your session:

```bash
timeout 3s ./legacy < /dev/null > out.txt 2> err.txt; echo "status=$?"
head -1 out.txt
```

Record `status`, the first output line and the first error line. If `status=124`, the program did
not finish in three seconds; record that as "ran, did not finish", not as "broken".

## Step 5 — Explain (6 min)

### Check the prediction against the map (2 min)

The prediction is not decoration. Before you compress the model, give each of the five predictions
a verdict, using the map as evidence:

| Verdict | Test | What to record |
|---|---|---|
| **Confirmed** | The map or the build result matches the prediction. | The row or line that confirms it. |
| **Contradicted** | The map says something different, and you can name it. | Both versions, side by side. |
| **Unresolved** | The map cannot decide it yet. | What you would read or run next. |

A contradicted prediction is the most useful row. It marks the place where your first reading was
wrong, which is exactly where the model learned something.

The one-sentence purpose is the compression test for the whole map. If the sentence is right, you
can predict a change. If it is vague, the map still has a hole.

Use this shape:

> This program **\_\_\_\_** by **\_\_\_\_** and stops when **\_\_\_\_**.

Worked sentence:

> This program plays a two-knight chase on an 8x8 board by trying the eight knight moves in an
> order that starts at a different move each turn, and stops when one knight lands on the other or
> a knight has no legal move.

Test the sentence three ways:

1. **Prediction.** If I change the move order, what changes? The sentence says the order starts at
   a different move each turn, so the path changes but the stop conditions do not.
2. **Compression.** Does the sentence cover the entry point, the state and the output without naming
   every function? It should.
3. **Falsifiability.** Which line would prove it wrong? For the stop condition, `jugada = 0` in
   `movcab` and in `impresionorden`. For the chase, the `ganador = 0` writes.

If you cannot answer one of the three, the sentence is not done. Rewrite it, not the map.

### What the sentence must not include

- **Quality.** "It is badly structured" is a judgment, not a purpose.
- **Environment.** "It targets a console" is context, not behavior.
- **Intention.** "The program was meant to be a game" is not testable from the source.
- **Implementation detail.** "It uses `switch`" describes the code, not what the program does.

The sentence answers "what is this program?" A reviewer should be able to read it and know what to
expect when the binary runs.

### If one sentence is not enough

Write a second sentence, but keep it separate:

> It reads no meaningful input: the key presses are pauses, and the randomness comes from the clock.

That sentence records an interface fact that the purpose sentence does not need. Two sentences are
fine; a paragraph is a sign that the map is still doing the work.

## Step 6 — Record (3 min)

Fill [`EVIDENCE.md`](EVIDENCE.md). It asks for the setup, the prediction, the entry point, the
function and state tables, the data flow, the build/run result and the mental model.

Two habits:

- **Cite lines.** "`movcab` writes `vectab`" is an opinion; "`movcab` writes `vectab` at 269" is a
  claim you can check.
- **Label confidence.** Read, ran, inferred, unknown. A map without labels hides its weakest parts.

### Write the next question

The next question is part of the deliverable, not an afterthought. A good question has four
properties:

1. **Specific.** It names a symbol, a file or a line.
2. **Answerable.** The source can answer it, or a small experiment can.
3. **Consequential.** The answer changes at least one row of the map.
4. **Bounded.** It fits one sitting, not a rewrite.

| Bad question | Why it is weak | Better question |
|---|---|---|
| "How does the game work?" | Too broad; the map already answers it. | "Does `movcab` always write `vectab` before returning?" |
| "Is this code good?" | A judgment, not a source question. | "Which functions write `jugada`, and in what order?" |
| "What does `cab` mean?" | Ambiguous; the name does not say whether it is a position or a turn. | "Which function assigns `cab` before each call to `movcab`?" |
| "What is the bug?" | Presupposes a defect and skips the map. | "Can `ccab` be returned uninitialized when all eight cases fail?" |

The last row is a good next question because it points at `movcab`'s return path and can be answered
by reading the `default` branch and the declaration of `ccab`.

## The source in detail

Facts you will cite repeatedly, with line numbers from the pinned source:

| Line | What is there | Why it matters for the map |
|---|---|---|
| 16 | `const char simcab1=21, simcab2=156, sep=219;` | Read-only print symbols, not state. |
| 17 | `int vectab[64], vecjugadas[8];` | Board and move-order array. |
| 18 | `int cab1, cab2, jugada=1, orden, ganador=1;` | Positions, move counter, turn order, winner flag. |
| 29 | `int cf1, cc1, cf2, cc2, cab;` | Row/column cache and turn selector. |
| 32 | `void main()` | Entry point; non-standard return type. |
| 38 | `system("color 1f")` | Console setup; environment dependency. |
| 42, 43 | `getch()`, `system("cls")` | Pauses and screen clearing; input is not data. |
| 44 | `while (jugada != 0)` | The loop condition is one global. |
| 61 | `for (i=0;i<=64;i++)` | The clear loop writes one past the array; note, do not chase. |
| 76 | `srand(time(NULL))` | Randomness seeded from the clock inside the placement loop. |
| 89 | `orden=ordenjuego(...)` | A return value assigned to a global. |
| 104–112 | `conversionvm` | Writes four globals, returns nothing. |
| 176–205 | `vecjug` | Builds the eight-move order from one random start. |
| 226–232 | `conversionmv` | Pure conversion: two parameters, one return, no globals. |
| 234–724 | `movcab` | Eight repeated move cases; read one and the default. |
| 726–751 | `ordenjuego` | Writes two board cells, increments `jugada`, prints. |
| 753–868 | `impresionorden` | Sets `cab`, calls `movcab`, handles capture, prints. |
| 870–930 | `impresion` | Reads the board and both positions; prints. |

The line numbers are anchors, not a substitute for reading. If your copy differs by a few lines,
record your own numbers; the structure is what you are mapping.

## What the map proves and does not prove

| Claim | Supported? | Why |
|---|---|---|
| A function calls another at the cited line. | Yes | The call is visible in the body. |
| A global is written at the cited line. | Yes | The assignment is visible. |
| A function reads a global it does not receive. | Yes | The read is visible in the body. |
| The runtime order of calls is the order in the map. | Only as a reading | The map is static; no run is traced. |
| The value a variable holds at a given moment. | Inferred | The declaration gives the type; the value needs a run or a trace. |
| The program's purpose is the sentence you wrote. | Inferred | The sentence is a model, not a proof. |
| The build result holds on another machine. | No | It is tied to your compiler and environment. |
| The code is correct or incorrect. | No | Correctness needs a specification and a test. |

This table is the honesty check. A map that claims to have traced a run is overreaching; write
"read from the source" when that is what you did.

## Self-check (binary)

- [ ] I wrote five predictions before reading any function body.
- [ ] I read `main` completely and named what starts and ends the program.
- [ ] I grouped the prototypes by job and corrected the grouping after reading.
- [ ] I read one case of `movcab` and the `default` branch, and recorded the repetition.
- [ ] Every function has a row with purpose, callers, callees, reads and writes.
- [ ] Every mutable global has a row with type, writers, readers and a reason to exist.
- [ ] I separated the `const char` print symbols from mutable state.
- [ ] I traced one value from input to output with a line number per arrow.
- [ ] I attempted the build and recorded the command and the first observable result.
- [ ] I wrote the one-sentence purpose and tested it three ways.
- [ ] I wrote a next question that names a symbol or a line.
- [ ] I labelled every uncertain claim as inferred or unknown.
- [ ] The source is unmodified and no build artifact was written into its tree.

If a box is not ticked, the unticked box names the next step. There is no penalty for repeating one.

## Misconceptions this lesson refutes

| Misconception | Why it is wrong | What to do instead |
|---|---|---|
| "Read the file from top to bottom." | A long file is mostly repetition; linear reading spends time on the largest body first. | Read the boundary, then the units, then the flow. |
| "The map is a summary of the file." | A summary lists what exists; a map states relationships. | Write callers, callees, writers and readers. |
| "Every file-scope name is state." | `simcab1`, `simcab2` and `sep` are `const char`. | Separate constants from mutable state. |
| "A function signature is its contract." | `movcab(int&, int&)` also reads six globals and writes three. | Read the body and record hidden reads and writes. |
| "If I cannot explain a function, I should read it again." | A named gap is progress; re-reading without a question repeats the same pass. | Write the question and mark the row `?`. |
| "The build must succeed for the lesson to count." | A quoted blocker is a result; the map does not depend on a binary. | Record command, error and line number. |
| "One sentence is too short to be useful." | The sentence is a compression test; if it is vague, the map has a hole. | Rewrite the sentence until it predicts a change. |
| "The next question should be the biggest open problem." | A big question cannot be answered in one sitting. | Choose the smallest question that changes the map. |
| "A static map is weaker than a debugger." | Both are evidence; the map is what a reviewer can read without running. | State which claims are read and which are run. |

## Terms used here

| Term | One-line meaning |
|---|---|
| Mental model | A small set of claims that predicts behavior and locates change. |
| Boundary | The entry point and the condition that ends the program. |
| Unit | A named function or group of functions with one job. |
| State | A value that survives between calls and can change. |
| Data flow | The path from input through transformation to output. |
| Mutation site | A line where state changes. |
| Caller | A function that calls another. |
| Callee | A function that is called. |
| Read claim | A claim visible at a cited line. |
| Run claim | A claim observed in a build or execution. |
| Inference | A claim that follows from call sites but was not read directly. |
| Evidence boundary | The line between what the source proves and what you assume. |

## What “done” means here

The **Self-check (binary)** above is the authoritative checklist. If every box is ticked, the
lesson is done for you. `meta.yml` mirrors the same criteria for tooling.

## Next step

- Continue to `archaeology-01` — build and run. You now know what the program is; the next question
  is whether it can be made to run without editing it.
- If your build stopped at `conio.h`, write that down as a blocker and move on. The adapted build is
  `archaeology-01`'s job, not a detour here.
- If `movcab` is still opaque, read only case 1 and the `default` branch again, and write the
  question that remains. Do not read all eight cases in this lesson.
- If your one-sentence purpose feels vague, replace one abstract verb with a concrete one. "Handles
  the game" is vague; "tries eight moves starting at a random one" is concrete.
- If you noticed a defect, write it in the evidence as a parked observation. Do not chase it now;
  the next lessons have a place for it.

## References

- [`README.md`](README.md) — the lesson card and the acceptance criterion.
- [`CODE-MAP-TEMPLATE.md`](CODE-MAP-TEMPLATE.md) — the map you fill in.
- [`EVIDENCE.md`](EVIDENCE.md) — the record you fill in.
- [`archaeology/legacy/README.md`](../../legacy/README.md) — where the pinned source lives.
- [`common-04`](../../../common/lessons/common-04-build-and-run/lesson.md) — the prerequisite: one
  command that behaves the same from any directory.
- [archaeology-01](../archaeology-01-build-today/lesson.md) — the adapted build and the run record.
- [`docs/guide/archaeology-track.md`](../../../docs/guide/archaeology-track.md) — the
  predict-before-run habit and the failure taxonomy used by later lessons.
