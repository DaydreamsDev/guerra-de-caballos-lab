# archaeology-08 — Missing module boundary

> [English](lesson.md) | [Español](lesson-es.md)

> **Status:** drafted · **Concept:** module boundary · **Prerequisite:** archaeology-05 ·
> **Estimated time:** 90 minutes
> **Reading:** ~15 minutes
> **Lesson role:** core

## Why this lesson exists

`archaeology-05` showed that the board has no owner. Every function reaches into `vectab` and writes
it. That lesson ended with a question it did not answer: why could every function reach it?

Because there was nothing to reach through. The archived 2008 program is **one translation unit**.
It has 930 lines, eleven functions, one `main`, eleven prototypes and **zero headers**. The board
is declared at line 17 as `int vectab[64]` and every function in the file can index it directly:

```text
17:  int  vectab[64],vecjugadas[8];
63:          vectab[i]=0;
122:         if(vectab[k]==0)
269:  vectab[ccab]=jugada;
293:  vectab[ccab]=jugada;
```

That is not an accident of style. A single file with file-scope state is the shortest path to a
working program for one developer with one compiler and a deadline. It was a reasonable trade for a
local program. The cost arrives later, when a second person needs to know what
the board is allowed to do.

The defect is that **the compiler cannot help**. Nothing in the file says "this is the board's
business". A new function can invent a ninth way to index the array, and the compiler will compile
it. The board is public because everything in a translation unit is public.

This is a **design** failure. The program works. The cost is that every change to the board is a
change to the whole file, and no tool can tell you when you crossed the line.

The product is a **module**: a header that declares what callers may use, and an implementation
that keeps everything else. The proof is not an opinion. It is a compiler error on the line where
a caller tries to touch the representation.

## What you will be able to do

By the end of this lesson you can:

- explain what a translation unit is and why file-scope state has no boundary;
- separate an **interface** (what callers may use) from an **implementation** (how it is done);
- write a header that declares operations and hides index arithmetic;
- show that a caller cannot reach the visited array, with the compiler's own words as evidence;
- state which half of the boundary the compiler enforces and which half the linker enforces;
- list what callers know, what they do not know, and how they learn what the header omits;
- name the changes that stay inside the module and the ones that reach the caller;
- explain why a C++ header still shows private members, and why that is not the same as access;
- distinguish a module boundary from an abstract data type, and say which lesson comes next;
- keep the archive read-only while you work.

The module is done when a stranger can rebuild it from the header
and `main.cpp` alone.

## The concept: module boundary

### Two files, one contract

A **translation unit** is one source file after preprocessing: the `.cpp` plus everything it
`#include`s. The compiler sees translation units one at a time. It never sees the whole program.

A **module boundary** uses that fact. The header declares names and types. The implementation
defines them. The compiler checks each side against the declaration, and the linker joins the two.

| Part | File | Who reads it | What it may contain |
|---|---|---|---|
| Interface | `board.h` | Every caller, and the implementation | Declarations callers need. |
| Implementation | `board.cpp` | The compiler, once | Definitions, private helpers, representation. |
| Caller | `main.cpp` | The compiler, once | Only what the header offers. |

The boundary is not decoration. It changes what is *possible*: a caller that has never seen
`board.cpp` cannot name anything inside it.

### What the compiler enforces, and what it does not

Two different tools enforce two different halves. Confusing them is the most common mistake in this
lesson.

| Question | Enforced by | When | Failure mode |
|---|---|---|---|
| May a caller name this member? | Compiler | Compile time | `error: ... is private within this context` |
| Does every declared function exist? | Linker | Link time | `undefined reference to ...` |
| Does the function do what its name says? | Nobody | Never | Silent wrong behavior |

The third row is the honest one. A boundary stops a caller from **reaching** the array. It does not
stop a caller from calling `mark(-5, 900)`. The header promises a name and a type; the behavior
lives in the body, and in this lesson the body's behavior is documented nowhere.

### Boundary, encapsulation, abstract data type

Three ideas that travel together and are not the same thing:

| Idea | Question it answers | Where it is taught |
|---|---|---|
| Module boundary | Who may see this name? | This lesson. |
| Encapsulation | Who may break this invariant? | `construction-11`, `archaeology-09`. |
| Abstract data type | Can the representation be swapped? | `construction-09`, `archaeology-09`. |

A header gives you the first. It gives you a little of the second, because `private` blocks direct
access. It gives you almost none of the third, because the private member is still written in the
header. That last point is the bridge to the next lesson, and this lesson will not pretend
otherwise.

### The C++ honesty problem

`private` is access control, not information hiding. Look at the baseline header:

```cpp
private:
    static constexpr int rowCount = 8;
    static constexpr int columnCount = 8;
    static constexpr int cellCount = rowCount * columnCount;

    int cell_index(int row, int column) const;
    bool is_inside(int row, int column) const;

    bool visited[cellCount];
```

A caller cannot write `board.visited[0]`. But a caller can read that the board is 64 `bool`s, and
must recompile when that line changes, because the object's size and layout changed. The boundary
buys **access control and rebuild isolation**, not secrecy.

Say that out loud in your record. It is the difference between a lesson that teaches a mechanism and
a lesson that teaches a belief.

## Before you start

Read these in full:

- [`README.md`](README.md) — the lesson card and the acceptance criterion.
- [archaeology-05](../archaeology-05-global-state/lesson.md) — the prerequisite: who owns the state.
- [`../../../docs/guide/archaeology-track.md`](../../../docs/guide/archaeology-track.md) — §3, the
  failure taxonomy; §8, the lesson sequence; §9, the Track A mapping.
- [`../../../docs/guide/lesson-catalog.md`](../../../docs/guide/lesson-catalog.md) — the
  `archaeology-08` row and its acceptance criterion.
- [`../../../docs/archive.md`](../../../docs/archive.md) — the pinned archive, tag and commit.

You also need:

- a local clone of the archive at tag `v1.0.0`;
- a C++20 compiler (`c++`, `g++` or `clang++`);
- a scratch directory outside the archive and outside this repository's tracked files.

> **Rule:** the archive is read-only. Copy what you need into scratch; never edit `original/`.

## The 90-minute path

| Min | Step | What you do | Evidence |
|---:|---|---|---|
| 0–15 | Predict | Predict what a header will and will not fix | Prediction table |
| 15–35 | Inspect | Count the board's reach in the archive; read the shipped module | Reach table and run output |
| 35–60 | Split | Rebuild the module from `main.cpp` alone, in scratch | Your `board.h` and `board.cpp` |
| 60–75 | Compile | Break the boundary on purpose; capture both errors | Exact compiler and linker messages |
| 75–85 | Explain | List what callers know, do not know, and what can change | Boundary table |
| 85–90 | Record | Fill `EVIDENCE.md`; confirm the archive is clean | Portfolio entry |

If the compiler error takes fifteen minutes to reproduce, that is
information about your toolchain, not a failure.

### The engineering check in this lesson

Track B uses the same habit as Track A with a different verb: the module already exists in the
lesson, so you **rebuild** it instead of writing it from nothing.

| Step | Where it happens | What it produces |
|---|---|---|
| **Predict** | Step 1 | A falsifiable claim about what a header fixes. |
| **Inspect** | Step 2 | The count of board mentions and the public surface. |
| **Split** | Step 3 | Your own header and implementation. |
| **Compile** | Step 4 | Two errors: one from the compiler, one from the linker. |
| **Explain** | Step 5 | The caller-knowledge table and the change test. |
| **Record** | Step 6 | The boundary note and `EVIDENCE.md`. |

## Step 1 — Predict (15 min)

Before you read `code/board.h`, write five sentences. Each one must be falsifiable: you must be able
to say what observation would prove it wrong.

1. In the 930-line archive file, how many lines mention `vectab`, and how many headers does the
   whole archive have? Give two numbers.
2. If the board moves behind a header, how many of the eleven functions must change?
3. What exactly will the compiler reject when `main.cpp` touches the visited array?
4. Will the program's output change?
5. What will the header still reveal about the representation?

### A worked prediction

> "`vectab` is mentioned in more than a hundred lines, because every case body writes it. Moving it
> behind a header changes every function that indexes it, but changes no behavior, so the output
> stays `visited=2 first=1 second=1`. The compiler will reject `board.visited[0]` with a privacy
> error. The header will still show that the board is an array of 64 `bool`s, because C++ requires
> the member to be declared there."

That prediction is useful even where it is wrong. If the output changed, you would have found a
behavior that depended on the representation being visible — which is exactly the coupling this
lesson is about.

## Step 2 — Inspect (20 min)

### Count the board's reach in the archive

From the archive root:

```bash
grep -c "vectab" original/variant-01-baseline-2008/source/guerra-de-caballos-baseline.cpp
grep -n "vectab" original/variant-01-baseline-2008/source/guerra-de-caballos-baseline.cpp | head -20
find original -name "*.h" -o -name "*.hpp" | wc -l
```

A reference run gives:

```text
138
0
```

One hundred and thirty-eight mentions of one array, and not a single header in the archive. The
mentions are not evenly spread; they cluster where the eight case bodies write the move number.

| Region | Lines | What it does with the board |
|---|---|---|
| `main` | 32–55 | Never touches it; the loop condition is `jugada`. |
| `inicializacion` | 57–67 | Clears it, with the off-by-one from `archaeology-04`. |
| `impresion0` | 114–174 | Reads it to decide what to print. |
| `impresion` | 870–930 | Reads it to draw the grid. |
| `movcab` | 234–724 | Reads and writes it: 48 mentions, 16 of them writes. |
| `ordenjuego` | 726–751 | Writes two cells and increments `jugada`. |

Five of the eleven functions index the array directly, and `movcab` alone accounts for 48 of the
mentions. `main` never touches it. That is the reach a header has to interrupt.

### Read the shipped module

The lesson ships the split already done. Read the three files in this order, and read `main.cpp`
first:

1. [`code/main.cpp`](code/main.cpp) — 13 lines. This is the caller's entire view of the board.
2. [`code/board.h`](code/board.h) — 23 lines. This is the contract.
3. [`code/board.cpp`](code/board.cpp) — 55 lines. This is the representation.

Then run it:

```bash
./run.sh
```

Expected output:

```text
visited=2 first=1 second=1
```

Read `main.cpp` again with the question: *what would this file have to know if `Board` were still
`int vectab[64]`?* The answer is the index formula, the row and column bounds, and the meaning of
zero. None of those appear in the file. That absence is the product of this lesson.

### The public surface

| Declaration | Header line | What the signature promises |
|---|---|---|
| `Board()` | 8 | A board exists. |
| `void reset()` | 9 | Every cell becomes unvisited. |
| `void mark(int row, int column)` | 10 | The cell becomes visited. |
| `bool is_visited(int row, int column) const` | 11 | Answers whether the cell is visited. |
| `std::size_t visited_count() const` | 12 | Counts visited cells. |

Five names. Compare that with the archive: eleven functions, eleven prototypes, four declaration
lines, and no statement anywhere of what a caller may rely on.

> **The gap the header does not close.** `mark(-5, 900)` compiles. So does
> `is_visited(0, 0)`. The header says nothing about coordinates, and the body silently ignores
> out-of-range input. The behavior is real, undocumented, and therefore not part of any contract a
> reviewer can check. Record it; `archaeology-13` is where it gets written down.

## Step 3 — Split (25 min)

This is the change test. You will rebuild the module so that the shipped `main.cpp` compiles and
runs **without being edited**.

### Set up the scratch copy

```bash
mkdir -p /tmp/board-split
cp archaeology/lessons/archaeology-08-module-boundary/code/main.cpp /tmp/board-split/
cd /tmp/board-split
```

`main.cpp` is now your only fixed point. Do not touch it. Everything else is yours to write.

### Write the header

Three rules decide what goes in:

1. **Declare only what a caller uses.** Four operations and a constructor. Nothing else.
2. **Do not declare helpers.** `cell_index` and `is_inside` are implementation details. They belong
   in `board.cpp` as free functions in an anonymous namespace.
3. **Declare the data member, but nothing about it.** C++ requires the member in the class
   definition. That is a language limit, not a design choice; say so in your record instead of
   pretending the header hides it.

The reference implementation of rule 2 shrinks the header from 23 lines to 17 and grows `board.cpp`
from 55 to 60. Nothing else changes, and the output is identical:

```text
visited=2 first=1 second=1
```

That is the trade in one measurement: six lines leave the header, and the implementation grows by
five. The header is now the caller's view, not the module's anatomy.

### Write the implementation

Move the definitions, keep the behavior. The body you write must satisfy the four promises above,
including the two the header does not state:

- rows and columns are **1-based**, `1..8`;
- out-of-range input is **ignored** by `mark` and **false** by `is_visited`.

If you cannot reproduce that behavior from `main.cpp` alone, that is the lesson: the header is not
the contract, it is half of one.

### Compile your version

```bash
c++ -std=c++20 -Wall -Wextra -Werror -O2 board.cpp main.cpp -o board-split
./board-split
```

| Check | Expected |
|---|---|
| Compile | No diagnostics; `-Werror` turns any warning into a failure. |
| Output | `visited=2 first=1 second=1` |
| `main.cpp` edits | Zero. |
| Header size | 12–20 lines is the honest range; the reference is 17. |

If your header is longer than 20 lines, ask which declaration a caller actually needs. If it is
shorter than 12, you probably dropped a promise the caller depends on.

## Step 4 — Compile (15 min)

Now break the boundary on purpose. This is not vandalism; it is the only way to see what the
boundary buys.

### The compiler's half

Add one line to a scratch copy of `main.cpp`:

```cpp
std::printf("%d\n", board.visited[0] ? 1 : 0);
```

Compile. The reference capture is from GCC; the line numbers depend on where you put the line:

```text
main.cpp: In function ‘int main()’:
main.cpp:8:31: error: ‘bool Board::visited [64]’ is private within this context
    8 |     std::printf("%d\n", board.visited[0] ? 1 : 0);
      |                               ^~~~~~~
In file included from main.cpp:1:
board.h:22:10: note: declared private here
   22 |     bool visited[cellCount];
      |          ^~~~~~~
```

Three things are in that message, and all three matter:

- the compiler knows the member exists;
- the compiler knows it is private;
- the compiler tells you exactly where the declaration is.

Compare with the archive, where the same line would compile silently and corrupt the board.

### The linker's half

Delete the definition of `reset()` from your `board.cpp` but keep the declaration. Compile a caller
that uses it:

```text
/usr/bin/ld: main.cpp:(.text.startup+0x10): undefined reference to `Board::reset()'
collect2: error: ld returned 1 exit status
```

The compiler accepted the call, because the header promised it. The linker refused, because the
promise had no body. Two tools, two halves, one boundary.

### The test that would have caught the class of failure

A build that compiles the module and the caller as separate translation units, with `-Werror`, and
that greps the caller for the representation's name:

```bash
c++ -std=c++20 -Wall -Wextra -Werror -c board.cpp -o board.o
c++ -std=c++20 -Wall -Wextra -Werror -c main.cpp -o main.o
c++ board.o main.o -o board-split
grep -n "visited\[" main.cpp && exit 1
```

The first three commands prove the boundary compiles from both sides. The fourth proves the caller
never names the array. In the archive, the equivalent grep for `vectab` would have matched five
functions.

## Step 5 — Explain (10 min)

### What callers know and do not know

Fill this from your own module, not from the shipped one.

| Question | Caller knows? | How the caller learns it |
|---|---|---|
| Which operations exist | Yes | The header. |
| Parameter and return types | Yes | The header. |
| Which operations mutate | Yes | `const` on the query methods. |
| That rows and columns are 1-based | No | Only by reading `board.cpp` or the tests. |
| That out-of-range input is ignored | No | Only by reading `board.cpp`. |
| That the board is 8×8 | No | Only by reading `board.cpp` or by probing. |
| That the board is an array of 64 `bool`s | It is visible, but unusable | The header's private section. |
| The index formula | No | `board.cpp`. |
| That `reset` is redundant after construction | No | `board.cpp`: the constructor zeroes. |

Two rows deserve a second look. "Visible but unusable" is the C++ compromise, and it is why the next
lesson exists. "`reset` is redundant" is a real contract ambiguity in the shipped module: the
constructor already produces an empty board, so a caller cannot tell whether `reset()` is required
before use. The header offers two ways to reach the same state and says nothing about which one is
the contract. Record it as a defect of the module, not of the caller.

### The change test

A design lesson is judged by a change, not by a shape. Two lists, both taken from the module you
built.

**Changes that stay inside the module:**

| Change | Caller source | Caller recompile | Output |
|---|---|---|---|
| `bool visited[64]` → one 64-bit mask | Unchanged | Yes | Identical. |
| `bool[64]` → `std::array<bool, 64>` | Unchanged | Yes | Identical. |
| Index formula `(row-1)*8+(col-1)` → `(row-1)*8+col-1` | Unchanged | Yes | Identical. |
| `visited_count` loop → popcount | Unchanged | Yes | Identical. |
| Helpers moved out of the class into the `.cpp` | Unchanged | Yes | Identical. |
| Add a private cache of the count | Unchanged | Yes | Identical. |

**Changes that reach the caller:**

| Change | Why it reaches the caller |
|---|---|
| Rename `mark` to `set_visited` | The name is in the header; every call site edits. |
| Make `is_visited` non-`const` | The declaration changes; callers holding a `const Board&` break. |
| Switch to 0-based coordinates | Compiles, runs, and silently means something else. |
| Make out-of-range `mark` throw | Compiles, runs, and changes behavior at the edge. |
| Add a public data member | Object size changes; every caller recompiles. |

Note the difference between the two tables. The first column is "recompile, not rewrite": the caller
needs no edit, but the build still touches it, because the representation is in the header. That is
the exact cost the next lesson removes.

### Two fixes, two sizes

**Minimal fix.** The split you just wrote. It is minimal in the sense that matters here: `main.cpp`
is untouched, behavior is identical, and the compiler now refuses the representation. That is enough
for the acceptance criterion.

**Structural fix.** Move the representation out of the header entirely — an opaque type, a
`std::unique_ptr<Impl>` and a forward declaration, or a free-function API over an incomplete type.
Then changing `bool[64]` to a mask stops recompiling callers too. Track A links: `construction-08`
(modular programming) for the boundary, `construction-09` (abstract data type) for the swap,
`construction-11` (encapsulation) for the invariant. This lesson stops at the first rung on
purpose; `archaeology-09` takes the second.

### What the boundary does not fix

- It does not make the board correct. `archaeology-04` owns the off-by-one.
- It does not give the board an owner. `archaeology-05` owns the ownership map.
- It does not state the contract. The 1-based coordinates and the silent out-of-range policy are
  still undocumented; that is `archaeology-13`.
- It does not stop a caller from calling `mark(-5, 900)`. Access control is not validation.
- It does not hide the representation from the compiler. Only the next rung does that.

## Step 6 — Record (5 min)

Fill [`EVIDENCE.md`](EVIDENCE.md):

- setup and archive pin;
- the prediction table, written before Step 2;
- the reach count and the region table;
- the module you wrote, with line counts before and after;
- the exact compiler error and the exact linker error;
- the caller-knowledge table;
- the two change lists;
- the evidence boundary: verified, inferred, unknown;
- the archive integrity check.

A sentence such as "the module hides the board" is an opinion. A pasted compiler error with a line
number is evidence.

## The original in detail

Facts you will cite repeatedly, with line numbers from the pinned archive
(`original/variant-01-baseline-2008/source/guerra-de-caballos-baseline.cpp`):

| Location | What is there |
|---|---|
| 16 | `const char simcab1=21,simcab2=156,sep=219;` — print symbols, not state. |
| 17 | `int vectab[64],vecjugadas[8];` — the board, at file scope, in the open. |
| 18 | `int cab1,cab2,jugada=1,orden,ganador=1;` — positions, move, order, winner. |
| 19–30 | Eleven prototypes, written by hand, in the same file as the definitions. |
| 29 | `int cf1,cc1,cf2,cc2,cab;` — the derived cache and the turn selector. |
| 32–55 | `void main()` — calls four functions and loops on a global. |
| 57–67 | `inicializacion` — `for (i=0;i<=64;i++) vectab[i]=0;`. |
| 63 | The off-by-one write. One past the end, every run. |
| 114–174 | `impresion0` — reads `vectab` to decide which knight to draw. |
| 122–162 | Thirteen read sites inside `impresion0`. |
| 234 | `int movcab(int &cf,int &cc)` — two references, six globals. |
| 269, 293, 329, 352, 385, 408, 441, 464, 497, 520, 554, 577, 610, 633, 666, 689 | `vectab[ccab]=jugada;` — sixteen writes, one per case per knight. |
| 726–751 | `ordenjuego` — writes two cells and prints. |
| 753–868 | `impresionorden` — orchestrates, prints, calls `getch`. |
| 870–930 | `impresion` — reads the board and draws the grid. |

The archive contains **zero** `.h` or `.hpp` files across all four variants. That is a fact about
the artifact, not an inference.

## What the boundary proves and does not prove

| Claim | Supported? | Why |
|---|---|---|
| A caller cannot name the visited array. | Yes | The compiler rejects it; you captured the message. |
| Every declared function has a body. | Yes, for the functions the caller uses | The linker checks only what is referenced. |
| The output is unchanged by the split. | Yes | Two runs, same line, same numbers. |
| The caller no longer needs the index formula. | Yes | `main.cpp` does not contain it. |
| The representation is hidden from the compiler. | No | It is written in the header's private section. |
| Callers survive a representation change without recompiling. | No | The object layout changes; they rebuild. |
| The module documents its coordinate system. | No | Neither the header nor any file says "1-based". |
| The module rejects invalid input. | No | It ignores it, silently. |
| The 2008 program would compile today unchanged. | No | `iostream.h`, `conio.h`, `void main`; see `archaeology-16`. |

This table is the honesty check. The strongest claim in this lesson is a compiler message; the
weakest is "the design is better". Keep them apart.

## Self-check (binary)

- [ ] I wrote five falsifiable predictions before reading `board.h`.
- [ ] I counted the `vectab` mentions and the headers in the archive.
- [ ] I read `main.cpp` before `board.h`.
- [ ] I ran the shipped module and got `visited=2 first=1 second=1`.
- [ ] I rebuilt `board.h` and `board.cpp` in scratch without editing `main.cpp`.
- [ ] My header declares no implementation helpers.
- [ ] My header is between 12 and 20 lines, and I can justify each declaration.
- [ ] I captured the exact compiler error for `board.visited[0]`.
- [ ] I captured the exact linker error for a declared but undefined function.
- [ ] I filled the caller-knowledge table, including the rows the header does not answer.
- [ ] I listed three changes that stay inside the module and three that reach the caller.
- [ ] I explained why a representation change forces a recompile but not an edit.
- [ ] I stated the difference between a module boundary and an abstract data type.
- [ ] I recorded that `reset()` is redundant after construction.
- [ ] The archive is unmodified (`git status` in the archive is clean).

If a box is not ticked, the unticked box names the next step. There is no penalty for repeating one.

## Misconceptions this lesson refutes

| Misconception | Why it is wrong | What to do instead |
|---|---|---|
| "A header hides the implementation." | The private member is declared in the header; the compiler sees it. | Say "access control", not "hiding". |
| "`private` is security." | Anyone can read the header and any caller can be recompiled. | Treat it as a compile-time access rule. |
| "Splitting a file is refactoring." | A split with no contract is two files with the same coupling. | Declare operations; keep the representation in the `.cpp`. |
| "The compiler checks that the function is correct." | It checks names and types only. | Add a behavioral contract and a test. |
| "If it compiles, the boundary works." | A declared-but-undefined function compiles and fails at link time. | Compile and link separately, then run. |
| "The module can reject bad coordinates." | It ignores them; nothing validates. | Record the policy; validation is a later lesson. |
| "`reset()` must be called before use." | The constructor already produces an empty board. | Record the ambiguity and pick one contract. |
| "A header makes the code portable." | The archive's problems are `conio.h` and `iostream.h`, not file layout. | Keep `archaeology-16` separate. |
| "More headers mean more modularity." | Splitting into ten headers with shared globals hides nothing. | One boundary per owned state. |
| "The boundary removes the need for tests." | It removes one class of misuse, not wrong behavior. | Keep the golden trace. |
| "This is the same lesson as the abstract data type." | This one hides names; that one hides representation. | Continue to `archaeology-09` and compare. |

## Terms used here

| Term | One-line meaning |
|---|---|
| Translation unit | One source file after preprocessing, compiled on its own. |
| Module boundary | The line between what callers may use and what the module keeps. |
| Interface | The declarations a caller is allowed to depend on. |
| Implementation | The definitions, helpers and representation behind the interface. |
| Access control | The compiler's rule about who may name a member. |
| Link-time check | The linker's rule that every used declaration has a definition. |
| Information hiding | Keeping the representation out of the caller's view and build. |
| Representation | How the state is stored, as opposed to what it means. |
| Change test | The edit you measure to judge whether a boundary helps. |
| Recompile, not rewrite | A change that forces a rebuild but no source edit in callers. |
| Opaque type | A type whose definition the caller never sees. |
| Behavioral contract | What the operations mean, beyond their names and types. |

## What “done” means here

The **Self-check (binary)** above is the authoritative checklist. If every box is ticked, the
lesson is done for you. `meta.yml` mirrors the same criteria for tooling.

## Next step

- Continue to `archaeology-09` — missing abstract data type. The header you wrote still shows
  `bool visited[64]`. That lesson removes it, and then a representation change stops forcing a
  caller recompile.
- If your header grew past 20 lines, list each declaration and the caller that needs it. Anything
  with no caller is a leak.
- If the compiler error surprised you, re-read the difference between access control and information
  hiding. That distinction is the whole lesson.
- If `mark(-5, 900)` compiled and did nothing, write the contract sentence the header is missing.
  Then decide whether the function should ignore, clamp or report. Do not implement it yet.
- If you want a stronger test, swap your representation for a 64-bit mask and confirm the caller
  needs no edit. That experiment is the next lesson's opening move.

## References

- [`README.md`](README.md) — the lesson card and the acceptance criterion.
- [`code/board.h`](code/board.h) — the reference interface.
- [`code/board.cpp`](code/board.cpp) — the reference implementation.
- [`code/main.cpp`](code/main.cpp) — the caller that knows only five names.
- [`run.sh`](run.sh) — compiles both translation units and runs the result.
- [`EVIDENCE.md`](EVIDENCE.md) — the portfolio record you fill in.
- [`../../../docs/guide/archaeology-track.md`](../../../docs/guide/archaeology-track.md) — §3, the
  failure taxonomy; §8, the lesson sequence; §9, the Track A mapping.
- [`../../../docs/guide/lesson-catalog.md`](../../../docs/guide/lesson-catalog.md) — the
  `archaeology-08` acceptance criterion.
- [`../../../docs/archive.md`](../../../docs/archive.md) — the pinned archive, tag and commit.
- [archaeology-05](../archaeology-05-global-state/lesson.md) — the prerequisite: who owns the state.
- Archive: <https://github.com/DaydreamsDev/DaydreamsDev-guerra-de-caballos-archive>, tag `v1.0.0`,
  commit `284e144e41403afbf0cc44422876425cacfb2bd5`.
