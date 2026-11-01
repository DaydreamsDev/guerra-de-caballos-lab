# archaeology-02 — Code inventory and call graph

> [English](lesson.md) | [Español](lesson-es.md)

> **Status:** drafted · **Concept:** structural inventory · **Prerequisite:** archaeology-01 ·
> **Estimated time:** 60 minutes
> **Reading:** ~17 minutes

## Why this lesson exists

`archaeology-01` answered one question: does it run? The answer was yes, through a probe, with the
console calls failing. That lesson ended with a working binary and a pile of unread source. This
lesson reads the pile as a system.

The source under study is a single file of 930 lines. Reading it top to bottom gives you a story —
first this happens, then that. A story is not a map. The story skips the parts that bored you, and
those are usually the parts that hold the structure together. An inventory is the map: every unit
counted, every call drawn, nothing left out because it looked unimportant.

The temptation is to jump straight to the defect you already noticed. Line 61 writes one past the
end of `vectab`, and it is visible from space. Do not chase it. A defect found before the map is a
guess with a line number. The protocol fixes the order because the map tells you whether a defect is
local or structural, and that difference decides how much it costs to fix.

This is step 3 of the autopsy protocol, narrowed. The protocol's step 3 also asks for comparisons
between versions. This lesson deliberately drops every comparison and keeps only structure: **what
exists, what calls what, and what state is shared.** Comparing two versions is a different question
with different evidence, and it is not today's.

## What you will be able to do

By the end of this lesson you can:

- count files, functions, globals and entry points in an unfamiliar source file;
- distinguish a prototype from a definition, and a declaration from a mutation;
- fill an inventory table that another engineer could check line by line;
- draw a call graph and answer five questions about it: roots, leaves, cycles, no caller, no callee;
- separate call sites from edges, and explain why the two numbers differ;
- name the largest function, the most connected function and the most mutated global, with evidence;
- label every finding `verified`, `inferred` or `unknown`;
- state what the shape of the graph predicts about the cost of the next change;
- record the whole inventory in `EVIDENCE.md`.

## The concept: structural inventory

A **structural inventory** is a complete, countable list of the units in a source file, plus the
directed edges between them. It has four kinds of unit and one kind of edge.

| Idea | Meaning in this lesson |
|---|---|
| **Unit** | A file, a function, a global, or an entry point. Something you can name and count. |
| **Function** | A definition with a body. A prototype is a promise, not a unit of work. |
| **Global** | A name declared outside every function, whose value outlives a call. |
| **Call edge** | "Function A contains a call to function B." Direction: caller → callee. |
| **Call site** | One occurrence of that call. A→B can be one edge and twenty-four sites. |
| **Root** | A node with no incoming call from inside the file. The program starts here. |
| **Leaf** | A node with no outgoing call to another function in the file. It ends a path. |
| **Cycle** | A path that returns to its own start. Recursion, direct or indirect. |
| **Dead code** | Code that no path from a root can reach. Not the same as a leaf. |
| **Confidence** | How you know: read it (`verified`), deduced it (`inferred`), or guessed (`unknown`). |

### Why count instead of read

Counting is falsifiable. "The largest function is `movcab`, 491 lines" can be checked in ten seconds
and proved wrong in five. "The code is messy" cannot be checked at all. Numbers force you to look at
the whole file instead of the part that caught your eye, and they make you honest about the parts
you skipped: an inventory with a blank row is a question, an essay with a missing section is a lie
by omission.

### Why the call graph is not the program

The graph says who *can* call whom. It does not say how often, under which condition, or with what
state. `impresion` is a leaf, and a beginner reads "leaf" as "unimportant". It is called from three
different functions, which makes it the unit with the most distinct callers in the file, and the one
place where a change to the board format would have to land. Leaf means *terminal*, not *trivial*.

The same warning applies in the other direction. A graph with no cycles looks clean, but thirteen
edges over twelve nodes is almost a tree, and a tree-shaped program with fifteen globals is not
modular. It is one function's worth of work split across twelve bodies that all reach into the same
drawer. The graph shows the calls; the globals show the coupling.

### The three confidence labels

Every claim in your inventory gets exactly one label. No fourth label, no "probably verified".

| Label | Test | Example |
|---|---|---|
| **Verified** | You read the line, and the line says it. | `movcab` spans 234–724, so it is 491 lines. |
| **Inferred** | The source supports it, but you combined two facts to get there. | `ganador` is never reset, so a game ends after the first capture. |
| **Unknown** | The source does not decide it. | Whether `orden` is always assigned before it is read on every path. |

The middle label is where most real findings live. Do not promote an inference to a fact because it
feels obvious; write the two facts you combined, and let the reader check the join.

## Before you start

Read these in full:

- [`README.md`](README.md) — the lesson card and the acceptance criterion.
- [`CODE-INVENTORY-TEMPLATE.md`](CODE-INVENTORY-TEMPLATE.md) — the table you will fill.
- [`docs/guide/archaeology-track.md`](../../../docs/guide/archaeology-track.md) — §2, the autopsy
  protocol, step 3; §3, the failure taxonomy you are *not* applying yet.
- [`docs/guide/lesson-catalog.md`](../../../docs/guide/lesson-catalog.md) — the `archaeology-01`
  prerequisite and this lesson's acceptance criterion.

You also need:

- a copy of the source outside the read-only original, for example in `/tmp/legacy-probe`;
- `wc`, `grep` and `awk` — all standard;
- a text editor and something to draw with, paper included.

You do **not** need to run the program again. `archaeology-01` already did that. Inventory is a
reading activity, and mixing it with execution invites you to explain behavior instead of structure.

> **Rule:** the original is read-only. Every command in this lesson reads a copy. If you find
> yourself editing the source to make the counting easier, stop and fix the command instead.

## The 60-minute path

| Min | Step | What you do | Evidence |
|---|---|---|---|
| 0–10 | Inspect | Count lines, includes, definitions, declarations | Four command outputs |
| 10–25 | Inventory functions | Fill the function table, 12 rows | Function table |
| 25–35 | Inventory globals | Fill the global table, count writes | Global table |
| 35–50 | Graph | Draw the graph, answer the five questions | Graph + roots/leaves |
| 50–55 | Explain | Name the shape and what it predicts | Shape paragraph |
| 55–60 | Record | Fill `EVIDENCE.md` with confidence labels | Portfolio entry |

If the function table takes twenty minutes because you keep reading bodies, that is information
about the file, not about you. `movcab` is 491 lines; reading it is not a detour, it is the job.

### The engineering check in this lesson

| Step | Where it happens | What it produces |
|---|---|---|
| **Inspect** | Step 1 | The surface: size, includes, definition count, declaration count. |
| **Inventory** | Steps 2–3 | Two tables: functions and globals, with line spans and writers. |
| **Graph** | Step 4 | The call graph, its roots, its leaves, its degree table. |
| **Explain** | Step 5 | One paragraph naming the shape and what it predicts. |
| **Record** | Step 6 | `EVIDENCE.md`, every finding with evidence and confidence. |

## Step 1 — Inspect the surface (10 min)

Work on a copy. Then run four commands and nothing else. Do not open the file in an editor yet; the
editor shows you the middle and hides the edges.

```bash
cp /path/to/original/variant-01-baseline-2008/source/guerra-de-caballos-baseline.cpp .
wc -l guerra-de-caballos-baseline.cpp
grep -n '^#include' guerra-de-caballos-baseline.cpp
grep -nE '^(void|int)[[:space:]]+[A-Za-z0-9_]+[[:space:]]*\(' guerra-de-caballos-baseline.cpp
grep -nE '^(const[[:space:]]+)?(char|int|float|double)[[:space:]]' guerra-de-caballos-baseline.cpp
```

On the reference copy, the four answers are:

```text
930
10:#include<stdio.h>   11:#include<conio.h>   12:#include<iostream.h>
13:#include<stdlib.h>  14:#include<time.h>
23 lines match: 11 prototypes (19–28, 30) and 12 definitions
16, 17, 18, 29
```

That is the whole surface: one file, five includes, twenty-three lines that look like functions, and
four lines of globals. Three of those numbers are traps.

### Trap 1 — prototypes are not functions

Lines 19–28 and 30 are eleven prototypes, each ending in `;`. Lines 32–930 contain twelve
definitions. A naive `grep` cannot tell the two apart, and a learner who trusts its count will
inventory eleven functions that have no body and miss that `main` has no prototype at all. The rule:
**a definition has a `{` after the parameter list; a prototype has a `;`.** When in doubt, ask for
the opening brace.

Twelve definitions, eleven prototypes, one `main` with neither a prototype nor a standard signature.
That is not a defect to fix today; it is a fact to record.

### Trap 2 — the identifier pattern hides a function

The first version of the pattern used `[A-Za-z_]+`, which excludes digits. It returns 21 matches and
silently drops both occurrences of `impresion0`, the definition at line 114 and the prototype at
line 30. A count that is off by two is worse than no count, because it looks complete. Any pattern
that matches identifiers must include digits: `[A-Za-z0-9_]+`.

### Trap 3 — declaration lines are not variables

Lines 16, 17, 18 and 29 declare fifteen names. Four lines. If you count lines you will report four
globals and be wrong by eleven. Count names, not statements:

| Line | Names | Kind |
|---|---|---|
| 16 | `simcab1`, `simcab2`, `sep` | `const char`, set once, never written again |
| 17 | `vectab[64]`, `vecjugadas[8]` | `int` arrays, the board and the move order |
| 18 | `cab1`, `cab2`, `jugada`, `orden`, `ganador` | loose `int` state, one name per fact |
| 29 | `cf1`, `cc1`, `cf2`, `cc2`, `cab` | loose `int` state, declared far from the others |

Fifteen names in four lines, with line 29 sitting between two prototypes. That placement is why the
second cluster is easy to miss: the declaration block ends at line 18, and line 29 hides in the
prototype list. This is a structural fact, not a style complaint. A reader scanning for globals will
find thirteen and lose two.

## Step 2 — Inventory the functions (15 min)

One row per definition, twelve rows. Use the template's columns and these rules:

- **Lines**: definition line to the line with the closing brace, blanks included. State the rule in
  your record so the numbers can be reproduced.
- **Inputs**: parameters, with `&` marked. "None" is a valid answer and a meaningful one.
- **Output**: the return type and what the value means, not just `int`.
- **Side effects**: globals written, console output, input read. A function with none is a pure
  helper; say so.
- **Calls**: user-defined functions only. Library calls belong in side effects.

| Function | Lines | Inputs | Output | Side effects | Calls |
|---|---:|---|---|---|---|
| `main` | 32–55 (24) | none | `void` | console setup, game loop | `inicializacion`, `cabubicacion`, `vecjug`, `impresionorden` |
| `inicializacion` | 57–67 (11) | none | `void` | writes `vectab[0..64]` | none |
| `cabubicacion` | 69–102 (34) | none | `void` | writes `cab1`, `cab2`, `orden`; seeds `rand`; prints | `ordenjuego`, `conversionvm` |
| `conversionvm` | 104–112 (9) | none | `void` | writes `cf1`, `cc1`, `cf2`, `cc2` | none |
| `impresion0` | 114–174 (61) | none | `void` | reads `vectab`, `cab1`, `cab2`; prints | none |
| `vecjug` | 176–205 (30) | none | `void` | writes `vecjugadas[0..7]`; calls `rand` | none |
| `imprevecjug` | 207–224 (18) | none | `void` | reads `vecjugadas`; prints | none |
| `conversionmv` | 226–232 (7) | `cf`, `cc` (by value) | `int` vector index | none — pure | none |
| `movcab` | 234–724 (491) | `cf`, `cc` (by reference) | `int` vector index | writes `vectab`, `jugada`, `ganador`; prints | `conversionmv` ×24, `impresion` ×2 |
| `ordenjuego` | 726–751 (26) | `tirocab1`, `tirocab2` (by ref) | `int` first player | writes `vectab`, `jugada`; prints | `impresion` ×2 |
| `impresionorden` | 753–868 (116) | none | `void` | writes `cab1`, `cab2`, `cab`, `jugada`; prints | `imprevecjug` ×4, `movcab` ×4, `impresion0` ×4, `impresion` ×4 |
| `impresion` | 870–930 (61) | none | `void` | reads `vectab`, `cab1`, `cab2`; prints | none |

Add the spans and you get 888 lines; the remaining 42 are the header, the includes, the globals and
the blank lines between functions. If your total does not land on 930, you missed a gap. That is the
check that the table is complete.

### What the table already tells you

Four numbers, before any interpretation:

1. **One function is half the file.** `movcab` is 491 lines, 52.8% of 930. The next largest is 116.
   The ratio is 4:1, and against the smallest function it is 70:1.
2. **The four largest functions hold 78%.** `movcab` 491 + `impresionorden` 116 + `impresion` 61 +
   `impresion0` 61 = 729 lines. One moves; the other three print or coordinate output.
3. **Ten of twelve return nothing.** Only `conversionmv` and `ordenjuego` return a value, and both
   return a small integer. The file's work happens through side effects, not through results.
4. **Two functions take references, one writes them.** `movcab` writes `cf` and `cc`;
   `ordenjuego` takes `tirocab1` and `tirocab2` by reference and never assigns either. That is a
   verified fact with an obvious question attached, and the question belongs in findings.

### The line-span trap

If you count "statements" instead of lines, `movcab` looks like 40 statements because eight `case`
bodies are copies of each other. Both numbers are true and they measure different things. Record the
line span, because that is what a reviewer pays for when they open the file, and note the
duplication as a separate observation with its own evidence.

## Step 3 — Inventory the globals (10 min)

One row per name, fifteen rows. The hard column is **Mutated by**, and it has a precise definition:

> A mutation is an assignment (`=`), an increment (`++`) or a decrement (`--`). A comparison (`==`,
> `!=`, `<=`, `>=`) is a read. So is passing the name to `cout`.

Get this wrong and `vectab` looks like it is written 59 times. It is written 21 times. The other 38
matches are comparisons and reads.

| Name | Type | Declared at | Mutated by | Read by |
|---|---|---|---|---|
| `simcab1`, `simcab2`, `sep` | `const char` | 16 | nothing — `const` | `cabubicacion`, `ordenjuego`, `impresionorden`, `impresion0`, `impresion` |
| `vectab[64]` | `int[64]` | 17 | `inicializacion` (1), `ordenjuego` (4), `movcab` (16) | `movcab`, `impresion0`, `impresion` |
| `vecjugadas[8]` | `int[8]` | 17 | `vecjug` (2) | `imprevecjug`, `movcab` |
| `cab1`, `cab2` | `int` | 18 | `cabubicacion` (2), `impresionorden` (4) | `conversionvm`, `ordenjuego`, `movcab`, `impresion0`, `impresion` |
| `jugada` | `int` | 18 | `ordenjuego` (4), `impresionorden` (8), `movcab` (2) | `main`, `movcab`, `ordenjuego`, `impresionorden` |
| `orden` | `int` | 18 | `cabubicacion` (1) | `impresionorden` |
| `ganador` | `int` | 18 | `movcab` (16) | `impresionorden` |
| `cf1`, `cc1`, `cf2`, `cc2` | `int` | 29 | `conversionvm` (4), `movcab` (via `&`) | `cabubicacion`, `impresionorden`, `movcab` |
| `cab` | `int` | 29 | `impresionorden` (4) | `movcab` |

The write counts, verified by counting single `=` and `++`:

| Name | Writes | Writing functions | Verdict |
|---|---:|---:|---|
| `vectab` | 21 | 3 | most mutated |
| `jugada` | 14 | 3 | the game clock |
| `ganador` | 16 | 1 | one value, sixteen places |
| `vecjugadas` | 2 | 1 | rebuilt every turn |
| `orden` | 1 | 1 | written once, read once |

### Three facts the table forces you to see

**`vectab` is the shared whiteboard.** Twenty-one writes from three functions, and reads from three.
No function owns the board; every function reaches into the same array. That is the coupling that
the call graph cannot show.

**`ganador` is a sticky flag.** Sixteen assignments, all of them `ganador=0`, and not one reset to
`1`. The declaration at line 18 starts it at `1` and the program never restores it. Whether that
matters is a behavior question for `archaeology-03`; that it is never reset is a structural fact
you can verify today.

**`orden` is a single-writer, single-reader channel.** `cabubicacion` writes it at line 89,
`impresionorden` reads it at lines 756 and 812. Nothing else touches it. One writer and one reader
is the cheapest possible coupling, and it is worth naming so that the expensive couplings stand out.

### The state you cannot see in the table

`cf1`, `cc1`, `cf2`, `cc2` and `cab` are written in two different ways: directly by `conversionvm`,
and through reference parameters inside `movcab`. The second path does not appear in the global
table as a write by name, because the parameter is called `cf`, not `cf1`. If you only grep for
`cf1=`, you will miss every move the knight makes. Record the aliasing explicitly: **`movcab`
mutates `cf1`/`cc1` or `cf2`/`cc2` through its reference parameters, depending on `cab`.** That
sentence is the difference between an inventory and a grep dump.

## Step 4 — Build the call graph (15 min)

For each function, read its body and list the user-defined functions it calls. Ignore `cout`,
`rand`, `getch`, `system`: they are library or platform calls, and putting them in the graph buries
the structure under noise. Then draw the edges.

```text
main
├── inicializacion                         leaf
├── cabubicacion
│   ├── ordenjuego
│   │   └── impresion                      leaf
│   └── conversionvm                       leaf
├── vecjug                                 leaf
└── impresionorden
    ├── imprevecjug                        leaf
    ├── movcab
    │   ├── conversionmv                   leaf
    │   └── impresion                      leaf
    ├── impresion0                         leaf
    └── impresion                          leaf
```

Now the five questions from the template, answered from the drawing:

- **Roots:** `main` only. It is the one node with no incoming call from inside the file.
- **Leaves:** `inicializacion`, `conversionvm`, `vecjug`, `imprevecjug`, `conversionmv`,
  `impresion0`, `impresion`. Seven nodes with no outgoing call to another function in the file.
- **Cycles:** none. The graph is acyclic; no function reaches itself by any path.
- **Functions with no caller:** `main`. Every other function has at least one.
- **Functions with no callee:** the same seven leaves.

### Edges are not call sites

`movcab` contains twenty-four calls to `conversionmv`: three per `case`, eight cases. In the graph
that is **one edge**. If you count call sites you get fifty user-function calls across thirteen
distinct edges:

| Caller | Distinct callees | Call sites |
|---|---:|---:|
| `main` | 4 | 4 |
| `cabubicacion` | 2 | 2 |
| `movcab` | 2 | 26 (24 + 2) |
| `ordenjuego` | 1 | 2 |
| `impresionorden` | 4 | 16 (4 × 4) |
| **Total** | **13 edges** | **50 sites** |

Both numbers are evidence. The edge count tells you the shape; the site count tells you where the
text lives. Report them separately and never let one stand in for the other.

### Degree: the most connected node

Degree is in-edges plus out-edges, counting distinct neighbours.

| Node | In | Out | Degree |
|---|---:|---:|---:|
| `impresionorden` | 1 | 4 | **5** |
| `main` | 0 | 4 | 4 |
| `movcab` | 1 | 2 | 3 |
| `cabubicacion` | 1 | 2 | 3 |
| `impresion` | 3 | 0 | 3 |
| `ordenjuego` | 1 | 1 | 2 |
| the six remaining leaves | 1 | 0 | 1 |

Two different "most connected" answers are defensible, and you must say which one you mean:

- **`impresionorden` has the highest degree (5).** It touches four subsystems and is touched by the
  root. It is the coordination point of the file.
- **`impresion` has the highest in-degree (3).** Three different functions print through it, which
  makes it the unit with the most distinct callers and the single choke point for output format.

Write both, labelled. A reviewer who asks "most connected?" and gets one number without a definition
has caught you being vague.

### Depth and reachability

The longest path is three edges: `main → impresionorden → movcab → impresion`. The graph is shallow
and wide, which is typical of a file written as a sequence of steps rather than a hierarchy. All
twelve functions are reachable from `main`, so there are **zero dead functions**. That is a finding,
and a mildly surprising one: a 930-line file with no tests and no headers still has no orphan.

The one unreachable piece is smaller than a function. The `default:` branch of the `switch` inside
`movcab` (lines 696–719) can only run if `vecjugadas[i]` is outside 1–8, and `vecjug` is the only
writer of that array, filling exactly 1–8 at lines 188 and 199. So the branch is **unreachable by
construction**, not by accident. Record it as dead code with its evidence, and resist the urge to
call it a bug; that judgment needs the behavior spec.

## Step 5 — Explain the shape (5 min)

You have three tables and a graph. Now answer three questions in plain language, then compress the
answers into one paragraph.

1. **Where does the work live?** `movcab` (491) and `impresionorden` (116) hold 607 lines, 65% of
   the file. Everything else is setup, conversion or printing.
2. **Where does the state live?** In fifteen globals declared in four lines. No function owns its
   data; the board, the clock, the turn and the winner are all reachable from everywhere.
3. **What does the graph say about change?** A rule change lands in a 491-line function. A board
   format change lands in two printers, `impresion` and `impresion0`, which share `vectab` and the
   same symbol constants; a move-log change lands in `imprevecjug`, which shares `vecjugadas` only.
   A state change lands in the global block and propagates to every reader.

### A worked shape paragraph

> The file is one flat system: a single root, thirteen call edges, seven leaves, no cycles, and
> fifteen globals that every function can reach. The work is concentrated in two bodies, `movcab`
> and `impresionorden`, which together hold 65% of the lines, while the remaining ten functions are
> converters, printers and one initializer. Because the state lives outside every function, the call
> graph understates the coupling: `impresion` looks like a leaf, but it reads the board, the knights
> and the winner's position, so a change to the board representation reaches it from three callers.
> The shape predicts that any change to movement is expensive, any change to output is cheap but
> triplicated, and any change to state is cheap to write and expensive to verify.

Read that paragraph again and notice what it does not say. It does not say the code is bad. It does
not name a defect. It describes a shape, and the shape has costs. That is the whole discipline.

### What the shape predicts

Predictions are the bridge to the next lessons. Write two, each as a count you could check:

- **Adding a third knight** would touch seven of the twelve functions — `cabubicacion`,
  `ordenjuego`, `conversionvm`, `movcab`, `impresionorden`, `impresion`, `impresion0` — plus the
  global block (`cab1`/`cab2`, `cf1`/`cc1`/`cf2`/`cc2`, `cab`). If your count differs, you found a
  dependency the graph missed.
- **Changing the board format** would touch `impresion` and `impresion0`: two functions, no logic.
  That is the cheap change, and the graph explains why.

Do not implement either. The predictions are inventory output; acting on them is a later step with a
test attached.

## Step 6 — Record (5 min)

Fill [`EVIDENCE.md`](EVIDENCE.md). It asks for the commands, the three tables, the graph, the five
answers, the findings and the confidence labels.

Two habits worth keeping:

- **Paste the command, not the memory.** `wc -l` returning `930` is evidence. "About nine hundred
  lines" is a rumor.
- **Give every finding a line number and a label.** A finding without `file:line` is an opinion; a
  finding without a label is an opinion wearing a suit.

## The original at a glance

| Metric | Value | Where it comes from |
|---|---:|---|
| Files | 1 | `find` in the source directory |
| Lines | 930 | `wc -l` |
| Function definitions | 12 | `grep` + brace check |
| Function prototypes | 11 | lines 19–28 and 30 |
| Global names | 15 | lines 16, 17, 18, 29 |
| Global declaration lines | 4 | same |
| Distinct call edges | 13 | call graph |
| Call sites | 50 | per-body count |
| Roots | 1 | `main` |
| Leaves | 7 | call graph |
| Cycles | 0 | traversal |
| Dead functions | 0 | traversal |
| Largest function | `movcab`, 491 lines | 234–724 |
| Most connected | `impresionorden`, degree 5 | degree table |
| Most called | `impresion`, in-degree 3 | degree table |
| Most mutated global | `vectab`, 21 writes | write count |
| Deepest path | 3 edges | `main → impresionorden → movcab → impresion` |

## What counts as a finding

A finding has three parts, and it is not a finding until it has all three:

1. **A claim** that could be false.
2. **Evidence**: a file and line, or a command and its output.
3. **A confidence label**: `verified`, `inferred` or `unknown`.

| Weak | Why it fails | Strong |
|---|---|---|
| "The code is too long." | Not falsifiable. | "`movcab` is 491 of 930 lines (52.8%), verified at 234–724." |
| "There are too many globals." | No count, no consequence. | "Fifteen global names in four declaration lines; `vectab` has 21 writes from 3 functions." |
| "`ganador` is buggy." | Diagnosis without a spec. | "`ganador` is assigned 16 times, always `0`, never reset (inferred consequence)." |
| "`impresion` is dead code." | Wrong: it is a leaf, not unreachable. | "`impresion` is a leaf with in-degree 3: called from `ordenjuego`, `movcab`, `impresionorden`." |

The last row is the one to memorize. **Leaf is not dead.** Dead means no path from a root; leaf
means no path onward. A leaf is where a path ends, and in this file three of the seven leaves are
printers, which is exactly where the output contract lives.

## What this lesson does not do

- It does not diagnose a defect. The off-by-one at line 61 is recorded as a structural fact — the
  initializer writes index 64 of a 64-element array — and handed to `archaeology-04`. Naming it is
  inventory; explaining its cost is diagnosis.
- It does not compare versions or track where a file came from. That question has its own evidence
  and its own lesson.
- It does not measure performance. Line counts are not run times.
- It does not refactor. A 491-line function is a finding, not an instruction.
- It does not write tests. The test that would catch a structural problem comes after the
  behavior specification.
- It does not replace the behavior spec. The graph says who calls whom, not what the program means.

## Self-check (binary)

- [ ] I ran the four inspection commands and recorded their output.
- [ ] I separated the eleven prototypes from the twelve definitions, with the brace rule.
- [ ] I counted fifteen global names, not four declaration lines.
- [ ] My function table has twelve rows and its line spans sum to 888, plus 42 for the rest.
- [ ] I identified `movcab` as the largest function, with its 491-line span.
- [ ] I counted writes as `=`, `++`, `--` only, and got 21 for `vectab`.
- [ ] I recorded that `ganador` is assigned sixteen times and never reset.
- [ ] I recorded that `movcab` mutates `cf1`/`cc1` or `cf2`/`cc2` through references.
- [ ] I drew the graph and named one root and seven leaves.
- [ ] I reported thirteen edges and fifty call sites as separate numbers.
- [ ] I gave two defensible "most connected" answers and defined degree.
- [ ] I found zero dead functions and one unreachable `switch` branch.
- [ ] Every finding has a line number and a confidence label.
- [ ] I wrote the shape paragraph and two checkable predictions.
- [ ] I filled `EVIDENCE.md` and the original is unmodified.

If a box is not ticked, the unticked box names the next step. There is no penalty for repeating one.

## Misconceptions this lesson refutes

| Misconception | Why it is wrong | What to do instead |
|---|---|---|
| "A leaf is dead code." | Leaf means no outgoing call; dead means no path from a root. | Traverse from the root and label the two separately. |
| "More lines means more important." | `movcab` is large because eight cases are copied, not because it does more. | Report the span and the duplication as two facts. |
| "The prototype list is the function list." | Eleven prototypes, twelve definitions, and `main` has no prototype. | Require a `{` after the parameter list. |
| "A global is a global because it says `int`." | Three `const char` names are globals too, and five more hide at line 29. | Count names in the declaration block, wherever it sits. |
| "Counting calls gives the edge count." | Fifty call sites collapse to thirteen distinct edges. | Count both, and say which one you mean. |
| "The call graph shows execution order." | It shows who may call whom, not when or how often. | Read the conditions in the body for order. |
| "A `void` function does nothing." | Ten of twelve are `void` and all ten mutate state or print. | Record side effects, not return values, as the work. |
| "One file means a simple system." | One file with fifteen globals and thirteen edges has no boundary at all. | Draw the state table next to the call graph. |
| "`grep` found it, so it is true." | A bare `grep` counts comparisons as writes and prototypes as functions. | State the pattern and the exclusion rule. |

## Terms used here

| Term | One-line meaning |
|---|---|
| Inventory | A complete, countable list of units and their relations. |
| Definition | A function with a body; the only thing that can be called. |
| Prototype | A declaration of a signature with no body. |
| Call edge | Caller → callee, counted once per pair. |
| Call site | One occurrence of a call; many per edge are possible. |
| Root | A node with no incoming call from inside the file. |
| Leaf | A node with no outgoing call to another function in the file. |
| Cycle | A path that returns to its own start. |
| Dead code | Code unreachable from any root. |
| Degree | Distinct in-edges plus out-edges. |
| Side effect | A write to state or a console interaction. |
| Verified / inferred / unknown | How you know: read it, deduced it, or cannot decide it. |

## What “done” means here

The **Self-check (binary)** above is the authoritative checklist. If every box is ticked, the
lesson is done for you. `meta.yml` mirrors the same criteria for tooling.

## Next step

- Continue to `archaeology-03` — behavior spec. The inventory tells you which functions exist; the
  spec tells you what they are supposed to do. Start with `movcab`, because it holds half the file.
- If the graph left you with a function you cannot explain, write it down for `archaeology-04`. Do
  not read it again today; a second reading without a question is just fatigue.
- If your write count for `vectab` differs from 21, re-run the command and publish the pattern you
  used. Counts are evidence only when they are reproducible.
- If the shape paragraph turned into a list of complaints, delete it and start from the three
  questions. Description first, judgment later.

## References

- [`README.md`](README.md) — the lesson card and the acceptance criterion.
- [`CODE-INVENTORY-TEMPLATE.md`](CODE-INVENTORY-TEMPLATE.md) — the tables and graph you fill.
- [`EVIDENCE.md`](EVIDENCE.md) — the record you fill in.
- [`docs/guide/archaeology-track.md`](../../../docs/guide/archaeology-track.md) — §2, the autopsy
  protocol, step 3; §3, the taxonomy you will apply in `archaeology-04`.
- [`docs/guide/lesson-catalog.md`](../../../docs/guide/lesson-catalog.md) — prerequisites and
  acceptance criteria.
- [`archaeology-01`](../archaeology-01-build-today/lesson.md) — the prerequisite: a runnable build
  and the measured output.
- Source under study: `original/variant-01-baseline-2008/source/guerra-de-caballos-baseline.cpp`,
  930 lines.
