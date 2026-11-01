# archaeology-05 — Global state and hidden coupling

> [English](lesson.md) | [Español](lesson-es.md)

> **Status:** drafted · **Concept:** global state and hidden coupling · **Prerequisite:**
> archaeology-04 · **Estimated time:** 60 minutes
> **Reading:** ~14 minutes

## Why this lesson exists

`archaeology-04` diagnosed failures that produce a wrong result: an off-by-one, a buffer overflow,
an inverted win condition. Each one has a symptom you can point at. This lesson is different. The
program in front of you works. It places two knights, alternates turns, prints a board and stops
when a knight is captured or trapped. Nothing crashes. Nothing prints the wrong winner.

The defect is that the program has no seam. Every function reaches into the same fifteen file-scope
names, writes some of them, reads others and prints while it does it. A change to the turn order
touches the placement function. A test for one move needs the whole console program. The state is
not wrong; it is **unownable**.

That is why this is a **design** failure, not a correctness failure. The taxonomy in
[`archaeology-track.md`](../../../docs/guide/archaeology-track.md) puts it plainly: the program
works, but the structure makes change expensive. This lesson teaches you to see the cost before you
feel it.

The product is a **map of mutations per global** and a function-level map of reads, writes and
printing side effects. The map is the diagnosis. The fix comes after it, in two sizes.

## What you will be able to do

By the end of this lesson you can:

- tell mutable global state from read-only file-scope constants;
- read a function and list the globals it reads, the globals it writes and what it prints;
- distinguish read coupling, write coupling, print coupling and temporal coupling;
- trace a mutation path from `main` to the cell that changes;
- name the mutation path that is hardest to test, and say why;
- separate what the source proves from what you infer from call sites;
- propose a minimal fix that creates one testable seam;
- propose a structural fix that removes the class, and link it to a Track A rung;
- keep the archive read-only while you work.

The map is done when every global has an owner and every
function has a row.

## The concept: global state and hidden coupling

A **global** is a name declared outside every function. In this file the globals are declared at
lines 16, 17, 18 and 29. Any function can read or write them without saying so in its signature.

**Hidden coupling** is the dependency a reader cannot see from the call. `movcab(cf1, cc1)` looks
like a function of two integers. It is not. It reads the move-order vector, the current move number,
the current knight, the board and both knight positions, and it writes the board, the winner flag
and the move number. The signature is a fraction of the contract.

Three consequences follow, and they are the ones you will map:

1. **You cannot test a function in isolation.** To call `movcab` you must first build the state it
   expects, including state written by three other functions.
2. **You cannot change one rule in one place.** The turn order is set in `cabubicacion`, consumed in
   `impresionorden`, and re-read inside `movcab` through `cab`.
3. **You cannot trust the call graph.** A function that looks pure may print, and a function that
   looks like a query may mutate.

### What counts as global state here

The scanner reports fifteen names. Twelve are mutable state. Three are `const char` symbols used for
printing: `simcab1`, `simcab2` and `sep`. They are file-scope, but they are not state; they cannot
change at runtime. Counting them as mutable is the first precision error this lesson corrects.

| Kind | Names | Mutability |
|---|---|---|
| Board and move order | `vectab[64]`, `vecjugadas[8]` | Mutable arrays. |
| Knight positions and turn | `cab1`, `cab2`, `cf1`, `cc1`, `cf2`, `cc2`, `cab` | Mutable integers. |
| Game progress | `jugada`, `orden`, `ganador` | Mutable integers. |
| Print symbols | `simcab1`, `simcab2`, `sep` | `const char`; read-only. |

`cf1`, `cc1`, `cf2` and `cc2` deserve a second look. They are a derived cache: `conversionvm` writes
them from `cab1` and `cab2`, and `movcab` mutates them through reference parameters. The same
position is stored twice, in two representations, and kept in sync by hand. That is not one global;
it is a small distributed object with no owner.

### Four couplings to name

| Coupling | What it means | Example in this file |
|---|---|---|
| **Read coupling** | A function depends on a value nobody passed it. | `movcab` reads `cab` to know whose turn it is. |
| **Write coupling** | A function changes state nobody asked it to change. | `ordenjuego` writes `vectab[cab1]`, `vectab[cab2]` and `jugada`. |
| **Print coupling** | A function produces output as a side effect of computing. | `movcab` prints case numbers and calls `impresion()`. |
| **Temporal coupling** | A function only works if another ran first, in a specific order. | `impresionorden` must set `cab` before every `movcab` call. |

Temporal coupling is the hardest to see and the most expensive. The source does not say "call
`conversionvm` before you read `cf1`". The order lives in the body of `cabubicacion`, between a
retry loop and six `cout` lines.

### Why this is a design failure

Severity is about consequence, not ugliness. The globals here do not by themselves cause undefined
behavior. The undefined behavior in this file is the `i <= 64` loop, and that belongs to
`archaeology-04`. What the globals cause is:

- **Medium severity:** every rule change is a multi-function edit;
- **Medium severity:** a single move cannot be tested without a console and eight pre-set globals;
- **High cost in review:** a reviewer cannot tell from a diff which state a function touches.

That is the design class: expensive change, fragile structure. It is fixed on a schedule, not in a
hotfix. The lesson asks you to price it, not to panic about it.

## Before you start

Read these in full:

- [`README.md`](README.md) — the lesson card and the acceptance criterion.
- [`GLOBAL-STATE-MAP-TEMPLATE.md`](GLOBAL-STATE-MAP-TEMPLATE.md) — the map you will fill.
- [archaeology-04](../archaeology-04-correctness-failures/lesson.md) — the prerequisite: symptom,
  root cause, minimal fix, structural fix.
- [`../../../docs/guide/archaeology-track.md`](../../../docs/guide/archaeology-track.md) — §3, the
  failure taxonomy; §5, the severity scale.
- [`../../../docs/archive.md`](../../../docs/archive.md) — the pinned archive and scope.

You also need:

- a local clone of the archive at tag `v1.0.0`;
- `bash` and `grep`;
- a text editor and a way to keep notes;
- [`scan_globals.sh`](scan_globals.sh), which ships with this lesson.

> **Rule:** the archive is read-only. The scanner reads it; it never writes into it.

## The 60-minute path

| Min | Step | What you do | Evidence |
|---|---|---|---|
| 0–10 | Predict | Rank the globals by risk before you read the functions | Prediction table |
| 10–25 | Inspect | Run the scanner; read the four declarations and the twelve functions | Scanner output and notes |
| 25–45 | Map | Fill one row per global and one row per function | `GLOBAL-STATE-MAP-TEMPLATE.md` |
| 45–55 | Explain | Trace three mutation paths; name the hardest to test | Path notes |
| 55–60 | Record | Fill `EVIDENCE.md`; confirm the archive is clean | Portfolio entry |

If `movcab` takes twenty minutes, that is information about `movcab`.

### The engineering check in this lesson

Track B uses the same habit as Track A, with a different verb: the program already exists, so you
**inspect** instead of build.

| Step | Where it happens | What it produces |
|---|---|---|
| **Predict** | Step 1 | A ranked list of globals with a falsifiable risk claim. |
| **Inspect** | Step 2 | The scanner output and the four declarations. |
| **Map** | Step 3 | Reads, writes and prints for every global and function. |
| **Explain** | Step 4 | Three mutation paths and the hardest one to test. |
| **Record** | Step 5 | The map, the fixes and `EVIDENCE.md`. |

## Step 1 — Predict (10 min)

Before you read the functions, look only at the declarations on lines 16–18 and 29. You know the
names and the types. Write one sentence per group:

1. Which global do you expect to be written by the most functions?
2. Which global do you expect to be read but never written after setup?
3. Which global do you expect to carry more than one meaning?
4. Which function do you expect to be the hardest to test, and why?

A useful prediction is falsifiable. "`jugada` is important" is not. "`jugada` is written by three
functions and read by four, and its value 0 is a loop sentinel" is.

### A worked prediction

Take `orden`:

- expected writers: one, in the placement path;
- expected readers: the turn printer;
- expected risk: low, because it never changes after setup;
- expected test cost: low once it is a parameter.

That prediction is useful even if it is wrong. If `orden` turned out to be written inside the turn
loop, the risk would jump. Write it down before you look.

## Step 2 — Inspect (15 min)

### Run the scanner

From the lab root:

```bash
./archaeology/lessons/archaeology-05-global-state/scan_globals.sh /path/to/archive
```

A reference run against the pinned archive prints:

```text
vectab=138
vecjugadas=15
cab1=30
cab2=30
jugada=69
orden=10
ganador=24
cf1=5
cc1=5
cf2=5
cc2=5
cab=15
simcab1=17
simcab2=17
sep=17
```

The counts are a starting point. They count **mentions**, not mutations. A name that appears 138
times may be written in three places; a name that appears 5 times may be written in two. The map is
the work; the count only tells you where to look.

### Read the scanner honestly

On the pinned archive the scanner prints one recognizable function header, not twelve:

```text
function_headers:
226:int conversionmv(int cf,int cc)
```

That is a limitation of the regex, not evidence that the other eleven functions do not exist. Most
headers carry a trailing comment or extra spaces, and the pattern does not allow them. Record the
gap in your evidence boundary. A tool that under-reports is still useful if you say what it misses.

| The scanner gives you | The scanner does not give you |
|---|---|
| Declaration lines. | Which function writes each global. |
| A mention count per name. | Reads versus writes. |
| Headers that match one regex. | Headers with comments or unusual spacing. |
| Names, including `const` symbols. | Mutability, ownership or coupling. |

### Read the source

Read the four declaration lines, then the twelve functions in this order. The order is chosen so
that each function adds one new kind of coupling:

1. `inicializacion` — the simplest writer. One global, no parameters.
2. `conversionvm` — a writer with four outputs and no return value.
3. `conversionmv` — the contrast pair: no global access at all.
4. `vecjug` — a writer of an array, with no parameters.
5. `imprevecjug` — a pure reader that prints.
6. `impresion` and `impresion0` — readers that print and read knight indices.
7. `movcab` — the large one: six global reads, three global writes, printing and reference
   parameters.
8. `ordenjuego` — a function that decides, mutates and prints.
9. `impresionorden` — the orchestrator: reads, writes, prints and paces the console.
10. `cabubicacion` — placement, turn order and reporting in one body.
11. `main` — a loop whose condition is a global written three calls deep.

## Step 3 — Map (20 min)

### The map protocol

For every global, fill six fields:

1. **Declaration.** File and line, plus the type.
2. **Written by.** Every function and line that assigns it. Include indirect writers reached through
   a call, and mark them as indirect.
3. **Read by.** Every function and line that reads it. Include reads used only to index another
   array.
4. **Side effects.** Printing, console control, or a write to another global that happens on the
   same path.
5. **Mutation path.** The shortest call chain from `main` to the write.
6. **Confidence.** High when the assignment is visible; medium when it is reached through a callee;
   low when the value depends on runtime behavior.

For every function, fill four fields: reads, writes, prints, hidden coupling. The last one is a
sentence, not a list.

### Worked global: `jugada`

This is the hardest global in the file, so it is the worked example. Fill it once with the class,
then do the other eleven yourself.

| Field | Entry |
|---|---|
| Declaration | `variant-01:18` — `int cab1,cab2,jugada=1,orden,ganador=1;` |
| Written by | `ordenjuego` (733, 735, 743, 745); `impresionorden` (772, 778, 797, 803, 827, 832, 852, 858); `movcab` (708, 716) |
| Read by | `main` (44, 46); `movcab` (243, 304, 360, 416, 472, 528, 585, 641, 697); `impresionorden` (756, 761, 775, 782, 800, 807, 814, 829, 836, 855, 862) |
| Side effects | None of its own. It gates printing and board writes in its readers. |
| Mutation path | `main` → `ordenjuego` → `jugada`; `main` → `impresionorden` → `jugada`; `main` → `impresionorden` → `movcab` → `jugada` |
| Hidden coupling | Three meanings in one integer: move number, loop sentinel (`0`), and special-case value (`2`). |
| Confidence | High. Every assignment is visible in the source. |

The three meanings are the lesson. `jugada` is incremented to number the moves, tested against `0`
to stop the game, and tested against `2` to skip a print. One name, three contracts, and no
comment says so.

### Worked function: `movcab`

| Field | Entry |
|---|---|
| Reads | `vecjugadas`, `jugada`, `cab`, `vectab`, `cab1`, `cab2` |
| Writes | `ganador` (257, 282, …), `vectab[ccab]` (269, 293, …), `jugada` (708, 716) |
| Prints | Case numbers (262, 286, …), "No hay movimientos posibles" (704, 712), and calls `impresion()` (705, 713) |
| Hidden coupling | The signature promises two in/out references. The body has six global inputs, three global outputs and console output. |

Now compare it with `conversionmv`:

```cpp
int conversionmv(int cf,int cc)
{
	int ccab;
	ccab=cf * 8 +cc;
	return ccab;
}
```

Two parameters, one return, no globals, no printing. It is the only function in the file you can
test with a single call and no setup. That is the contrast pair for the whole lesson: the same file
already contains the shape you want. The problem is not that the code could not express it; the
problem is that the shape was not applied to the state.

### Anchor index

Use this table to check your work, not to replace it. It gives the declaration and the first
writer/reader; you still enumerate the rest.

| Global | Declared | First function write | First read | Prints nearby |
|---|---|---|---|---|
| `vectab` | 17 | 63 (`inicializacion`) | 122 (`impresion0`) | 124, 132 |
| `vecjugadas` | 17 | 188 (`vecjug`) | 216 (`imprevecjug`) | 210 |
| `cab1` | 18 | 81 (`cabubicacion`) | 95 (`cabubicacion`) | 95 |
| `cab2` | 18 | 82 (`cabubicacion`) | 97 (`cabubicacion`) | 97 |
| `jugada` | 18 | 732 (`ordenjuego`) | 44 (`main`) | 737 |
| `orden` | 18 | 89 (`cabubicacion`) | 756 (`impresionorden`) | 759 |
| `ganador` | 18 | 257 (`movcab`) | 768 (`impresionorden`) | 770 |
| `cf1` | 29 | 106 (`conversionvm`) | 96 (`cabubicacion`) | 96 |
| `cc1` | 29 | 107 (`conversionvm`) | 96 (`cabubicacion`) | 96 |
| `cf2` | 29 | 108 (`conversionvm`) | 98 (`cabubicacion`) | 98 |
| `cc2` | 29 | 109 (`conversionvm`) | 98 (`cabubicacion`) | 98 |
| `cab` | 29 | 765 (`impresionorden`) | 249 (`movcab`) | 759 |

The three `const char` symbols are not in the table because they are not mutations. Add a note in
your map that you excluded them, and why.

## Step 4 — Explain (10 min)

### Three mutation paths

Write each path as a chain of calls and end at the cell or variable that changes.

**Path 1 — placement.**

```text
main → inicializacion → vectab[0..64]
main → cabubicacion → cab1, cab2
     → ordenjuego → vectab[cab1], vectab[cab2], jugada
     → impresion (reads vectab)
     → conversionvm → cf1, cc1, cf2, cc2
```

**Path 2 — turn.**

```text
main → vecjug → vecjugadas[0..7]
main → impresionorden → cab
     → movcab → ganador, vectab[ccab], jugada
     → impresion (reads vectab)
     → cab1 or cab2 (return value)
     → jugada++
```

**Path 3 — termination.**

```text
movcab (default) → jugada=0
impresionorden (ganador==0) → jugada=0
main (jugada==0) → loop exits
```

The paths overlap. That is the point: the same variable is the move counter in Path 1, the turn
gate in Path 2 and the stop condition in Path 3.

### The hardest path to test

Path 2 through `impresionorden` is the hardest. To exercise one turn you must:

- set `orden`, `jugada`, `ganador`, `cab`, `cf1`, `cc1`, `vecjugadas` and `vectab`;
- provide a console, because the function calls `getch()` and `system("cls")`;
- capture stdout, because the assertions are mixed with `cout`;
- accept that the function writes `cab1` or `cab2` as a side effect.

Eight globals, two console calls and an output stream. A unit test for one move is not impossible;
it is a reimplementation of the program. That sentence belongs in your map under "hardest to test".

### The cost

Price the design failure in the currency of real work:

| Cost | Evidence in this file |
|---|---|
| A rule change touches several functions. | Turn order is set in `cabubicacion`, used in `impresionorden`, re-read in `movcab`. |
| A test needs the whole program. | `movcab` needs six globals; `impresionorden` needs eight plus a console. |
| A reviewer cannot see the contract. | `movcab(int&, int&)` hides six inputs and three outputs. |
| State can drift. | `cab1` and `cf1`/`cc1` are two representations of one position. |
| A bug is hard to reproduce. | The failing state is implicit; there is no value to print at the boundary. |

### Two fixes, two sizes

**Minimal fix.** Create one seam. The smallest useful one is to make `movcab` state its inputs and
return its result:

```cpp
struct MoveResult {
    bool moved;
    bool captured;
    int  cell;
};

MoveResult movcab(int &cf, int &cc, int knight, int moveNumber,
                  const int (&order)[8], int board[64]);
```

This does not remove the globals. It makes one function testable without the console and without
reading the caller. Keep the printing in `impresionorden`; a function that computes should not also
print.

**Structural fix.** Give the state an owner. A `GameState` record holds the board, the two knights,
the move number and the outcome, and every function takes it explicitly. Then a `Board` type owns
its cells and a `Knight` type owns its position, so `cab1` and `cf1`/`cc1` cannot drift. Track A
links: `construction-11` (encapsulation) for the private invariant, `construction-13` (composition)
for the board inside the game, with `construction-08` (modular) and `construction-09` (abstract data
type) as the earlier rungs.

Do not skip the minimal fix. The track's rule is minimal first; the rewrite is a last resort with a
price. The minimal fix proves you understand the harm. The structural fix argues the class.

### What the fixes do not fix

The minimal fix does not remove `jugada`'s three meanings; it only stops one function from reading
them implicitly. The structural fix does not fix the `i <= 64` off-by-one; that is a correctness
card from `archaeology-04`. Keep the two repairs separate in your record.

## Step 5 — Record (5 min)

Fill [`EVIDENCE.md`](EVIDENCE.md):

- setup and archive pin;
- the prediction table, written before the map;
- the scanner output, including the header gap;
- the map of mutations per global;
- the function table;
- the three mutation paths and the hardest to test;
- the evidence boundary;
- the minimal and structural fixes with the Track A link;
- the archive integrity check.

The map is the evidence. A sentence such as "`movcab` is coupled" is an opinion; a row with six
reads, three writes and a print site is a diagnosis.

## The original in detail

Facts you will cite repeatedly, with line numbers from the pinned archive:

| Location | What is there |
|---|---|
| 16 | `const char simcab1=21,simcab2=156,sep=219;` — read-only symbols. |
| 17 | `int vectab[64],vecjugadas[8];` — board and move order. |
| 18 | `int cab1,cab2,jugada=1,orden,ganador=1;` — positions, move, order, winner. |
| 29 | `int cf1,cc1,cf2,cc2,cab;` — derived row/column cache and turn selector. |
| 61–63 | `for (i=0;i<=64;i++) vectab[i]=0;` — the off-by-one from `archaeology-04`. |
| 89 | `orden=ordenjuego(tirocab1,tirocab2);` — a return value assigned to a global. |
| 104–112 | `conversionvm` — writes four globals, returns nothing. |
| 226–232 | `conversionmv` — the only global-free function. |
| 234 | `movcab(int &cf,int &cc)` — the signature that hides the coupling. |
| 239 | `switch(vecjugadas[i])` — reads the order vector. |
| 249 | `if (cab==1)` — reads the turn selector. |
| 257 | `ganador=0;` — inverted winner flag, set on capture. |
| 269 | `vectab[ccab]=jugada;` — the move number becomes the cell value. |
| 708, 716 | `jugada=0;` — termination written from inside the move function. |
| 726–751 | `ordenjuego` — writes two cells, increments `jugada`, prints. |
| 753–868 | `impresionorden` — orchestrates turns, prints, calls `getch`. |
| 765, 790, 820, 844 | `cab=1` or `cab=2` — the turn selector is set before each call. |
| 870–930 | `impresion` — reads the board and both knight indices, prints. |

## What the map proves and does not prove

| Claim | Supported? | Why |
|---|---|---|
| A global is written at the cited line. | Yes | The assignment is visible. |
| A function depends on a global it does not receive. | Yes | The read is visible in the body. |
| A function prints as a side effect. | Yes | The `cout` call is in the body. |
| The mutation order at runtime is the order in the map. | Only as a reading of the source | The map is static; no run is traced. |
| A value is written by a callee, not the caller. | Inferred | The call site shows the callee, not its body. |
| The design failure causes a wrong result. | No | The program works; the cost is change and testability. |
| The minimal fix removes the coupling. | Only at one seam | The globals remain. |
| The structural fix removes the class. | By design argument | The rung is a concept, not a measured result. |
| The 2008 compiler behaved like a modern one. | No | The source is the only witness. |

This table is the honesty check. A map that claims to have traced a run is overreaching; say
"read from the source" when that is what you did.

## Self-check (binary)

- [ ] I wrote a ranked prediction before reading the functions.
- [ ] I ran `scan_globals.sh` and recorded the mention counts.
- [ ] I recorded that the scanner recognized one header, not twelve, and why.
- [ ] I separated the twelve mutable globals from the three `const char` symbols.
- [ ] Every global has a declaration, a writer list, a reader list and a mutation path.
- [ ] Every function has reads, writes, prints and one sentence of hidden coupling.
- [ ] I marked indirect writes reached through a callee.
- [ ] I named the three meanings of `jugada`.
- [ ] I explained why `ganador` needs `cab` to identify the winner.
- [ ] I traced three mutation paths from `main` to the changed value.
- [ ] I named the hardest path to test and said why.
- [ ] I wrote the evidence boundary: verified, inferred, unknown.
- [ ] I proposed a minimal fix that creates one seam.
- [ ] I proposed a structural fix with a Track A link (`construction-11`, `construction-13`).
- [ ] The archive is unmodified (`git status` in the archive is clean).

If a box is not ticked, the unticked box names the next step. There is no penalty for repeating one.

## Misconceptions this lesson refutes

| Misconception | Why it is wrong | What to do instead |
|---|---|---|
| "Global state is always a correctness bug." | This program works; the cost is change and testability. | Classify it as a design failure and price it. |
| "Every file-scope name is mutable state." | `simcab1`, `simcab2` and `sep` are `const char`. | Separate constants from state in the map. |
| "A high mention count means a high mutation count." | The scanner counts mentions, not assignments. | Enumerate the writes yourself. |
| "`movcab` is a function of two integers." | It reads six globals and writes three. | Read the body, not the signature. |
| "`ordenjuego` only decides who starts." | It also writes the board, advances `jugada` and prints. | List all four jobs. |
| "`ganador` says who won." | It is a flag: `0` means someone won; `cab` says who. | Record the pair, not the flag alone. |
| "`jugada` is just a counter." | It is also the loop sentinel and a special-case value. | Name all three meanings. |
| "`cab1` and `cf1`/`cc1` are independent." | They are two representations of one position. | Mark the derived cache and its sync points. |
| "The minimal fix is a throwaway." | It creates the first testable seam and proves the harm is removable. | Keep it, then argue the structural fix. |
| "The structural fix should be applied now." | The track forbids jumping from the map to the rewrite. | Minimal first; structural second, with a price. |
| "A static map is weaker than a debugger." | Both are evidence; the map is what a reviewer can read without running. | State which claims are read and which are run. |

## Terms used here

| Term | One-line meaning |
|---|---|
| Global state | A value declared outside every function and reachable by all of them. |
| Hidden coupling | A dependency that is not visible in the function signature. |
| Read coupling | A function depends on a value nobody passed it. |
| Write coupling | A function changes state nobody asked it to change. |
| Print coupling | A function produces output as a side effect of computing. |
| Temporal coupling | A function only works if another ran first, in a given order. |
| Mutation path | The call chain from `main` to the value that changes. |
| Derived cache | A second copy of a value, kept in sync by hand. |
| Sentinel | A special value that marks a state, here `jugada == 0`. |
| Seam | A place where behavior can be observed or replaced without editing the caller. |
| Design failure | The program works, but the structure makes change expensive. |
| Evidence boundary | The line between what the source proves and what you infer. |

## What “done” means here

The **Self-check (binary)** above is the authoritative checklist. If every box is ticked, the
lesson is done for you. `meta.yml` mirrors the same criteria for tooling.

## Next step

- Continue to `archaeology-06` — duplication and the eight-case switch. The same `movcab` body
  repeats eight times; the map you built here is the input for counting what changes together.
- If `jugada` was the hardest global, re-read the three meanings and write the smallest type that
  could hold them separately. Do not implement it yet; name it.
- If `movcab` was the hardest function, count its parameters and its globals. The difference is the
  hidden contract.
- If you want a stronger test, extract one turn in a scratch copy and call it twice with the same
  state. The globals will make the second call differ; that difference is the coupling.
- If you disagree with the severity, write the context that changes it. A one-shot demo and a
  maintained game do not price the same defect.

## References

- [`README.md`](README.md) — the lesson card and the acceptance criterion.
- [`GLOBAL-STATE-MAP-TEMPLATE.md`](GLOBAL-STATE-MAP-TEMPLATE.md) — the map you fill in.
- [`scan_globals.sh`](scan_globals.sh) — declarations, headers and mention counts.
- [`EVIDENCE.md`](EVIDENCE.md) — the portfolio record you fill in.
- [`../../../docs/guide/archaeology-track.md`](../../../docs/guide/archaeology-track.md) — §3, the
  failure taxonomy; §5, the severity scale; §9, the Track A mapping.
- [`../../../docs/guide/lesson-catalog.md`](../../../docs/guide/lesson-catalog.md) — the lesson
  sequence and the `archaeology-05` acceptance criterion.
- [`../../../docs/archive.md`](../../../docs/archive.md) — the pinned archive, tag and commit.
- [archaeology-04](../archaeology-04-correctness-failures/lesson.md) — the prerequisite: symptom,
  root cause, minimal fix, structural fix.
- Archive: <https://github.com/DaydreamsDev/DaydreamsDev-guerra-de-caballos-archive>, tag `v1.0.0`,
  commit `284e144e41403afbf0cc44422876425cacfb2bd5`.
