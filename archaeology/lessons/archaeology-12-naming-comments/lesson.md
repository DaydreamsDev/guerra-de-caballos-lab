# archaeology-12 — Naming and comments

> [English](lesson.md) | [Español](lesson-es.md)

> **Status:** drafted · **Concept:** naming and comments · **Prerequisite:** archaeology-04 ·
> **Estimated time:** 60 minutes
> **Reading:** ~19 minutes

## Why this lesson exists

`archaeology-04` found failures that change behavior. This lesson is about a failure that changes
nothing at run time and still costs money every day: the code works, but its names and comments make
every reader pay to reconstruct what it means.

The catalog calls it `failure-019`, a professional-practice failure. The archive's 930-line baseline
contains 119 lines with `//`. Some comments explain a decision. Many narrate the next statement.
Some are wrong because they were copied from another branch and never updated. The identifiers are
terse Spanish abbreviations: `vectab`, `cab1`, `cf1`, `cc1`, `jugada`, `orden`, `ganador`. None of
them is a defect by itself. The defect is that the name does not carry the decision, so the comment
is asked to carry it, and the comment is not tested.

The bench is deliberately small. `before.cpp` and `after.cpp` compute the same board index with
different names. Both print `index=19`. The output is identical because a rename must preserve
behavior. The lesson is not "make it prettier"; it is "change the name, keep the contract, and write
down why the new name is better".

## What you will be able to do

By the end of this lesson you can:

- read a terse identifier and state which decision it hides;
- rename a function, its parameters and a local without changing behavior;
- decide whether a comment is a reason, a warning, a constraint, narration or a lie;
- delete a comment that a better name replaces, and keep the ones the code cannot say;
- prove a rename with the same output and an explicit `match=yes`;
- name what a rename does not fix: magic numbers, missing bounds checks, missing tests;
- price a naming defect by reader time and incident cost, not by taste;
- keep the archive read-only while you work.

The lesson is done when another
person can reproduce your run and follow your reasons.

## The concept: names and comments

### A name is a compression of a decision

Every identifier is a promise about what the reader may stop thinking about. `calc` promises almost
nothing: it says a calculation happens, which is true of most functions. `board_cell_index` promises
a domain (board), an operation (index) and a result (a cell index). The reader can now skip the body
until they need the formula.

The same test applies to parameters. `f` and `c` are not wrong because they are short; they are
wrong because the body `f * 8 + c` has two plausible readings, row-major and column-major, and the
names do not choose. `row` and `column` choose. The multiplication by 8 then reads as "eight
columns per row", which is the actual rule.

A useful name has four properties:

| Property | Test | Bad | Better |
|---|---|---|---|
| Pronounceable | Can you say it in a review? | `vectab` | `board` |
| Searchable | Can you grep it without noise? | `i`, `aux` | `move_index`, `swap_value` |
| Domain-specific | Does it use the game's words? | `data1` | `knight_cell` |
| Consistent | Does the same concept keep one name? | `cell`, `pos`, `index` | `cell_index` everywhere |

Abbreviation is a budget, not a style. `cfg`, `id`, `url` and `i` are fine because they are
universal in their context. `vectab` is not universal: it is `vector tabla`, a Spanish abbreviation
for a board array. It saved a few keystrokes once and costs every later reader a decode step.

Language is a separate decision. The archive is Spanish because the domain is Spanish. That is not
a defect. The defect would be mixing Spanish and English for the same concept
inside one boundary. This lab uses English identifiers because the lessons are English; the archive
keeps its Spanish names because it is evidence. Pick one language per boundary, write the choice
down, and translate through a glossary (`common-02`), not ad hoc.

### A comment is a contract about what the code cannot say

A comment is not free documentation. It is a second source of truth, and it has no test. When the
code changes and the comment does not, the comment becomes a lie that a reader may trust more than
the code. A stale comment is a special case: it was true once, and the code moved on. Git can show
you when the comment changed; the compiler cannot.

Five kinds of comment are worth keeping:

| Kind | Example | Why it survives |
|---|---|---|
| Reason | `// The sentinel 0 ends the game; 1 is a real move.` | The convention is not in the type. |
| Warning | `// Caller must pass 0 <= row < 8.` | The precondition is not in the type. |
| Constraint | `// The board is 8 by 8: 64 cells, index 0..63.` | The dimensions are not in the type. |
| Workaround | `// MSVC 6.0 rejects the standard form.` | The constraint is invisible in the source. |
| Non-obvious invariant | `// The counter also ends the loop; do not reset it.` | The coupling is not in the type. |

Everything else is a candidate for deletion. `// increment i` above `i++` is narration.
`// calculates the index` above `board_cell_index` is narration with extra steps. A comment that
describes *what* the next line does can almost always be replaced by a name that says it once.

The rule for this lesson is blunt: **if a better name can replace the comment, replace the
comment**. If the comment says something the name cannot — a reason, a warning, a unit, a
constraint — keep it and make it precise.

### What the legacy code actually contains

The bench is a reconstruction, and the record must say so. `conversionmv` at
`variant-01:226` is exactly `cf * 8 + cc`. The bench reduces it to `calc` and shortens `cf, cc` to
`f, c` so the single-letter failure is visible in eight lines. The archive already uses two-letter
parameters; the bench compresses them. That is a teaching compression, not a claim about the
original.

What is verbatim is the naming style and the comment style:

| Anchor | `variant-01` | What it shows |
|---|---:|---|
| Header comment with a program description | 1–8 | A description, not a contract. |
| `//librerias necesarias para el programa` | 9 | Narration of the next `#include`. |
| `simcab1=21,simcab2=156,sep=219` | 16 | Three magic numbers with no named meaning. |
| `vectab[64],vecjugadas[8]` | 17 | Two different concepts in one comment. |
| `cab1,cab2,jugada,orden,ganador` | 18 | Comment mentions three of five names. |
| `inicializacion()//... ke kontiene ...` | 19 | Orthographic error in the only explanation. |
| `conversionvm()//// ...` | 22 | Doubled comment marker; near-identical twin name. |
| `conversionmv(int cf,int cc)` | 226 | The formula the bench isolates. |
| `srand(time(NULL));// cambia la semilla` | 76 | Comment repeats the call. |
| `tirocab1=...;//... para el caballo 2` | 78 | Comment names the wrong knight. |
| `if ( cab==1 )//... no haya estado ...` | 310, 366, 422, 478, 591 | Comment describes the next check, not this one. |
| `i==8&&cab==1` trapped-knight check | 702 | A long comment that explains a real rule. |
| `ganador=0` on capture | 257 and 15 more | Name says "winner"; zero means a winner exists. |

The archive's own `05-known-issues.md` says the same: comments often narrate the obvious, contain
orthographic errors, or describe behavior that changed. That is the archive admitting the debt, not
the lab inventing it.

Two facts are worth pausing on.

First, `ganador` is initialized to `1` and set to `0` when a knight captures the other. The printing
code then asks `if(ganador==0)`. The name suggests a winner, but the value is an inverted flag:
`0` means "the game has a winner". Renaming it `winner` would preserve the bug in a nicer word.
The honest rename is `game_has_winner` with the polarity fixed, or `winner_exists` with `true` on
capture. That is a behavior change and belongs in a separate step, not in a pure rename.

Second, `jugada` is both the move counter and the end-of-game sentinel: it increments into
`vectab[ccab]`, and setting it to `0` ends the `while (jugada!=0)` loop. One name cannot carry both
meanings. A rename alone would hide the coupling; the real fix is two variables, which is
`archaeology-05` territory. Naming exposes design; it does not replace it.

## Before you start

Read these in full:

- [`README.md`](README.md) — the lesson card, the build command and the acceptance criterion.
- [`RENAME-TEMPLATE.md`](RENAME-TEMPLATE.md) — the table you will fill.
- [archaeology-04](../archaeology-04-correctness-failures/lesson.md) — the prerequisite: failure
  cards, minimal fixes and the difference between symptom and root cause.
- [`../../../docs/guide/archaeology-track.md`](../../../docs/guide/archaeology-track.md) — §3, the
  failure taxonomy; §5, the severity scale; §6, tone and ethics.
- [`../../../docs/archive.md`](../../../docs/archive.md) — the pinned archive, tag and commit.

You also need:

- a local clone of the archive at tag `v1.0.0`;
- a C++20 compiler (`c++`, `g++` or `clang++`);
- `bash`;
- a scratch directory outside `original/`.

> **Rule:** the archive is read-only. Copy any file you want to change into a scratch directory;
> never edit `original/`. The bench in this lesson is also evidence: copy it before you edit.

## The 60-minute path

| Min | Step | What you do | Evidence |
|---|---|---|---|
| 0–8 | Predict | Guess what each name means and whether a rename changes output | Prediction table |
| 8–20 | Inspect | Read both programs, run them, census the archive's names and comments | Anchor notes and `match=yes` |
| 20–35 | Rename | Fill the rename table, edit a scratch copy, compile | Renamed file and reasons |
| 35–43 | Compare | Run baseline and renamed copy, compare output | `before=index=19`, `after=index=19` |
| 43–53 | Explain | Audit every comment and price the defect | Comment table and cost note |
| 53–60 | Record | Fill `EVIDENCE.md`; confirm the archive is clean | Portfolio entry |

If the comment audit takes fifteen minutes, that investigation is the lesson.

### The engineering check in this lesson

Track B uses the same habit as Track A, with a different verb: the behavior already exists, so you
**preserve** it while you change the names.

| Step | Where it happens | What it produces |
|---|---|---|
| **Predict** | Step 1 | A falsifiable guess about meaning and output. |
| **Inspect** | Step 2 | The two programs, the run, and the archive anchors. |
| **Rename** | Step 3 | A scratch copy with new names and a filled table. |
| **Compare** | Step 4 | Identical output and an explicit `match=yes`. |
| **Explain** | Step 5 | Comment audit and cost. |
| **Record** | Step 6 | `EVIDENCE.md` with commands, reasons and boundaries. |

## Two sittings

This lesson is long by design. Split it into two sittings:

- **Sitting 1 — Core (Steps 1–3, about 35 min).** Stop after Step 3, when the names are renamed
  and the diff is visible.
- **Sitting 2 — Extension (Steps 4–6, about 25 min).** Start only after Sitting 1. You will
  compare, explain and record the rename.

## Step 1 — Predict (8 min)

Before you open the source, answer these four questions in writing. The point is to commit to
answers you can be wrong about.

1. What does `calc` compute: a row, a column, a board cell index, or a distance?
2. What do `f` and `c` stand for, and in which order are they multiplied?
3. If you rename the function and both parameters, does the printed output change?
4. How many of the archive's comments explain a reason rather than narrate a statement?

A useful prediction is falsifiable. "The names are bad" is not. "`calc` is row-major with `f` as the
row and `c` as the column, so `calc(2, 3)` is 19, and a rename cannot change that" is.

### A worked prediction

Before reading, I would write:

- `calc` returns a board cell index, not a geometric calculation;
- `f` is `fila` (row), `c` is `columna` (column), so the formula is `row * 8 + column`;
- the output stays `index=19`, because names are erased by the compiler;
- most comments narrate; a few carry a reason or a warning.

Every claim can be checked. If your prediction says "the code is unreadable", you have made a
judgment, not an estimate. Write the meaning you expect, then test it.

## Step 2 — Inspect (12 min)

### Read the bench

```bash
cd archaeology/lessons/archaeology-12-naming-comments
nl -ba code/before.cpp
nl -ba code/after.cpp
diff -u code/before.cpp code/after.cpp
```

`before.cpp` is 13 lines:

```cpp
#include <cstdio>

// calcula el indice del tablero
int calc(int f, int c)
{
    return f * 8 + c;
}

int main()
{
    std::printf("index=%d\n", calc(2, 3));
    return 0;
}
```

`after.cpp` is 12 lines. The only differences are the comment, the function name and the parameter
names. The expression, the constant, the call and the output are identical.

Read the diff twice. The first time, ask what changed. The second time, ask what did **not** change:
the `8`, the lack of bounds checks, the single call site, the absence of a test. A rename is a
surgical change, and the diff is the proof that it stayed surgical.

### Run the baseline

```bash
./run.sh
```

Expected output:

```text
before=index=19
after=index=19
match=yes
```

The script compiles both files with:

```text
-std=c++20 -Wall -Wextra -Werror -O2
```

`-Werror` matters. A rename that introduces an unused parameter or an unused variable will fail the
build, which is exactly the feedback you want. If your compiler rejects the baseline, record the
compiler and the diagnostic; do not silence the warning to make the run pass.

### Census the archive's names

In the pinned archive at tag `v1.0.0`:

```bash
grep -c '//' original/variant-01-baseline-2008/source/guerra-de-caballos-baseline.cpp
grep -n 'vectab\|vecjugadas\|cab1\|cf1\|jugada\|orden\|ganador' \
    original/variant-01-baseline-2008/source/guerra-de-caballos-baseline.cpp | head -20
```

The first command reports `119`. The file is 930 lines, so roughly one line in eight carries a
comment.

Now read the declarations at lines 15–30. They are the densest naming surface in the file:

```text
16: const char simcab1=21,simcab2=156,sep=219;//simbolos usados para imprimir el tablero
17: int  vectab[64],vecjugadas[8];//vector que almacena la informacion del tablero ...
18: int  cab1,cab2,jugada=1,orden,ganador=1;//variables para identificar la posicion ...
```

Line 16 declares three magic numbers without names. Line 17 puts two different concepts in one
comment. Line 18 names three of the five variables and omits `orden` and `ganador`. None of these is
a crash. All of them are reader debt.

Count the references before you propose a global rename:

```bash
grep -c 'vectab' original/variant-01-baseline-2008/source/guerra-de-caballos-baseline.cpp
grep -c 'jugada' original/variant-01-baseline-2008/source/guerra-de-caballos-baseline.cpp
```

`vectab` appears on 80 lines and `jugada` on 76 lines. That is the difference between renaming a
local in the bench and renaming a global in the archive. The bench is one file with one call site;
the archive is a single translation unit where every name is global.

### Check your prediction

`calc(2, 3)` is `2 * 8 + 3 = 19`. `f` is the row and `c` is the column. The output does not change
under a rename. Most comments narrate; the header describes the program, and the trapped-knight
comment explains a rule.

If your prediction said the comments explain more than they do, you found the first gap: most of
them restate the next statement. Write it down. The gap between prediction and observation is the
learning signal.

## Step 3 — Rename (15 min)

### Make the scratch copy

```bash
mkdir -p build/scratch
cp code/before.cpp build/scratch/renamed.cpp
```

From here on, edit `build/scratch/renamed.cpp`. Do not edit `code/before.cpp`; it is the baseline
the next learner will run.

### Fill the rename table

Open [`RENAME-TEMPLATE.md`](RENAME-TEMPLATE.md) and fill the first table. A reason is not "it is
clearer". A reason names the decision the old name hid and the reader who was paying for it.

| Old name | New name | Reason | Scope |
|---|---|---|---|
| `calc` | `board_cell_index` | `calc` names an action every function performs; the result is a board cell index. | Function, one file. |
| `f` | `row` | `f` could be `fila` or `factor`; the multiplication by 8 makes it the row. | Parameter, one function. |
| `c` | `column` | `c` could be `column`, `count` or `cell`; the `+ c` term is the column offset. | Parameter, one function. |

Three rows, three reasons, three scopes. If a row says "more descriptive", rewrite it until it names
the ambiguity that the new name removes. Your names can differ: `cell_index` or `board_index` are
also defensible if the reason names the reading they choose. The acceptance criterion is the reason,
not the exact word.

### Apply the rename

```cpp
#include <cstdio>

int board_cell_index(int row, int column)
{
    return row * 8 + column;
}

int main()
{
    std::printf("index=%d\n", board_cell_index(2, 3));
    return 0;
}
```

Compile with the shipped flags and run:

```bash
c++ -std=c++20 -Wall -Wextra -Werror -O2 build/scratch/renamed.cpp \
    -o build/scratch/renamed
./build/scratch/renamed
```

Expected output:

```text
index=19
```

### What the rename does not touch

- The constant `8`. It is still a magic number; naming it `board_width` is a different change.
- The bounds. `board_cell_index(9, 9)` returns 81, outside a 64-cell board.
- The call site's arguments. `(2, 3)` is still a row and a column, now visibly so.
- The output format. `index=%d` is a contract with the caller and did not change.
- The missing test. One `printf` is a demo, not a regression test.

This boundary is the point of a minimal rename. You changed three names and one comment, and nothing
else. If your diff contains a new helper, a new constant or a new check, you have left the lesson.

### The archive-scale version

A global rename in the archive is mechanical but not free. `vectab` appears on 80 lines, `jugada`
on 76, and both are globals in one 930-line file. The professional sequence is:

1. rename one concept at a time;
2. compile after each rename;
3. keep behavior changes in a separate commit;
4. use the compiler, not a find-and-replace, to find the call sites;
5. record the count of touched references.

Do not rename `jugada` to `move_number` and call it done. It is also the sentinel that ends the
game. A pure rename would make the dual meaning harder to see, not easier. That is a design fix,
not a naming fix.

## Step 4 — Compare (8 min)

### Run both paths

```bash
./run.sh
c++ -std=c++20 -Wall -Wextra -Werror -O2 build/scratch/renamed.cpp \
    -o build/scratch/renamed
./build/scratch/renamed
```

Observed:

```text
before=index=19
after=index=19
match=yes
index=19
```

The renamed copy prints the same line as `before.cpp` and `after.cpp`. Three names changed, zero
bytes of output changed.

### What equality proves

- The compiler resolves the new names to the same expression.
- The argument order is still row, then column. Swapping them would print `index=26`.
- The rename did not change the constant, the operator or the format string.
- For this input, the rename is behavior-preserving.

### What equality does not prove

| Claim | Proven? | Why |
|---|---|---|
| The rename is correct for every input. | No. | Only `(2, 3)` was run. |
| The new names are the best names. | No. | That is a review judgment, not a test. |
| The function is correct for a real board. | No. | No bounds check; `(9, 9)` returns 81. |
| The comments are now true. | No. | The bench has one comment; the archive has 119 lines of them. |
| The archive's globals were renamed. | No. | The bench has no globals. |
| The code is easier to maintain. | Partly. | The reason table is the argument; the run only proves equality. |

### The oracle direction

The baseline is the oracle because it is the code that existed. The renamed copy is the candidate
because it is the code you are trusting. If you generate both from the same new names, the
comparison is circular.

This is the same rule as `archaeology-06` and `archaeology-10`, applied to a smaller change:
**freeze the old behavior before you change the words**.

### When a mismatch appears

A rename should not change output. If it does, the cause is almost always one of these:

- the arguments were swapped (`index=26` instead of `19`);
- the constant was transcribed as `9` or the operator as `+` instead of `*`;
- the format string was edited along with the names;
- the call still points at the old function because the edit was partial.

Print the failing expression and compare it with the baseline. A mismatch is a transcription bug,
not a reason to change the expected output.

## Step 5 — Explain (10 min)

### Audit the comments

Fill the second table in [`RENAME-TEMPLATE.md`](RENAME-TEMPLATE.md). For every comment you touch,
record four things: where it is, what it said, what it says now, and why. The four verbs are
**keep**, **rewrite**, **delete** and **repair**.

| Location | Before | After | Reason |
|---|---|---|---|
| `before.cpp:3` | `// calcula el indice del tablero` | *(deleted)* | The function name now says it; the comment was narration. |
| `v01:1–8` | Program description | Keep | The code does not say what the program is for. |
| `v01:9` | `//librerias necesarias...` | *(deleted)* | The `#include` lines say it. |
| `v01:16` | `//simbolos usados...` | `// Board glyphs: knight, knight, wall.` | The comment lists numbers; the names should say what they draw. |
| `v01:18` | Names three of five variables | Rewrite or delete | The comment is incomplete and will drift. |
| `v01:19` | `//inicializa ... ke kontiene...` | *(deleted)* | `clear_board()` says it; the typo goes with it. |
| `v01:40` | `// // designa...` | *(deleted)* | Doubled marker and narration. |
| `v01:76` | `// cambia la semilla` | *(deleted)* | `srand(time(NULL))` is the statement. |
| `v01:78` | `//... para el caballo 2` | Repair or delete | It describes `tirocab1` as knight 2; the comment lies. |
| `v01:310` | `//... no haya estado...` | *(deleted)* | The `if` tests whose turn; the visited check is the next line. |
| `v01:702` | Long trapped-knight comment | Shorten to the rule | The rule is real; the narration is not. |

Two rows deserve a longer note.

**The lie at 310.** The comment says "verifies the knight has not been in that position before", but
the statement is `if (cab==1)`, which selects the moving knight. The visited check is the next `if`.
The comment was copied from the branch below and never updated. A reader who trusts it will look for
a visited check in the wrong place. This is the most expensive kind of comment: not useless, but
wrong.

**The near-twin names at 22 and 226.** `conversionvm` converts a vector index to a row and column;
`conversionmv` converts a row and column to a vector index. The names differ by two letters and
describe opposite directions. No comment can repair that reliably; the names must change to
`cell_to_row_column` and `row_column_to_cell`. This is a naming defect that a comment made worse,
because both comments use the same words in a different order.

### Severity and cost

The track's severity scale puts naming, comments and readability at **low**: improve when touching
the file. That is correct per incident and misleading per year. A bad name costs a few seconds on
every read. A lying comment costs trust, and trust is what makes a reviewer stop checking.

The cost becomes high when:

- a new maintainer onboards and cannot tell `conversionvm` from `conversionmv`;
- an incident review trusts a comment that describes the old behavior;
- the same concept carries three names in one file;
- a reviewer stops checking because the comments were right before.

State the cost in your context. A private one-off script and a published library do not price the
same name.

### Two fixes, two sizes

**Minimal fix — rename and repair the comments you touched.** Rename `calc`, `f` and `c`; delete
the narrating comment. This is what Steps 3–5 do. It removes the local ambiguity and leaves the
magic `8`, the missing bounds check and the missing test in place.

**Structural fix — make the names part of the contract.** Add a glossary (`common-02`), a review
rule for comments, and a naming table for the archive's globals. In the archive, that means one
name per concept and a comment policy. Track A links: `common-02` (glossary), `construction-03`
(functions and parameters) and `construction-08` (module boundary).

### What a rename does not fix

- The magic `8`. It is still an unnamed board width.
- The missing bounds check. `board_cell_index(9, 9)` is still 81.
- The dual meaning of `jugada`. A rename can hide it; only a design change removes it.
- The inverted `ganador` flag. Renaming it `winner` would preserve the polarity bug.
- The globals. The bench has none; the archive has `vectab`, `vecjugadas` and the rest.
- The missing tests. One `printf` is a demo.
- The 119 comment lines in the archive. You audited a sample, not the file.

### The tradeoff, stated fairly

A rename is cheap in a local scope and expensive across a public boundary. In the bench, three names
and one call site: five minutes. In the archive, `vectab` appears on 80 lines and every one is a
compile-checked edit; a global rename is a review-sized change even when the tool does the typing.

Comments have the opposite tradeoff. Deleting a narrating comment is free. Deleting a reason comment
is expensive, because the reason may exist nowhere else. That is why the audit asks for a reason per
row: the table is the only place where "keep" and "delete" are distinguished by evidence rather than
taste.

## Step 6 — Record (7 min)

Complete sections 3 and 4 of [`RENAME-TEMPLATE.md`](RENAME-TEMPLATE.md) as well: the before and
after output, the compiler, the match, what you did not change, the names that cannot change without
touching callers, and the comments that are still needed.

Fill [`EVIDENCE.md`](EVIDENCE.md):

- setup and archive pin;
- the four predictions, written before inspection;
- the bench inventory: 13 lines before, 12 after, one call site;
- the archive anchors: 119 comment lines, 80 lines mentioning `vectab`, 76 mentioning `jugada`;
- the baseline run and its exact output;
- the rename table with a reason per row;
- the comment audit with a verb per row;
- the comparison result and the oracle direction;
- the evidence boundary: what the run proves and what it does not;
- the archive integrity check.

A sentence such as "the names are clearer" is an opinion. "Three names, one comment deleted, same
`index=19`, 119 comment lines in the archive" is a record.

## The original in detail

Facts you can cite, with line numbers from `variant-01` at tag `v1.0.0`:

| Location | What is there |
|---:|---|
| 1–8 | Header comment: program description. |
| 9 | `//librerias necesarias para el programa` — narration. |
| 15 | `//variables,vectores y prototipos...` — narration. |
| 16 | `simcab1=21,simcab2=156,sep=219` — three magic numbers. |
| 17 | `vectab[64],vecjugadas[8]` — two concepts, one comment. |
| 18 | `cab1,cab2,jugada=1,orden,ganador=1` — five names, three explained. |
| 19 | `inicializacion()` with `ke kontiene` — typo in the only explanation. |
| 22 | `conversionvm()////` — doubled marker and near-twin name. |
| 32 | `void main()//funcion principal` — narration on a portability defect. |
| 40 | `cabubicacion();// // designa...` — doubled marker. |
| 61 | `for (i=0;i<=64;i++)` — the off-by-one the comments did not catch. |
| 76 | `srand(time(NULL));// cambia la semilla` — narration. |
| 78 | `tirocab1=...;//... para el caballo 2` — wrong knight. |
| 226 | `int conversionmv(int cf,int cc)` — the formula the bench isolates. |
| 249 | `if ( cab==1 )//verifica de quien es el turno...` — correct comment. |
| 310, 366, 422, 478, 591 | `if ( cab==1 )` with the visited-check comment — wrong statement. |
| 702 | Trapped-knight check with a long but accurate rule comment. |
| 930 | End of file; no tests, no build file. |

The naming surface, with suggested replacements. These are proposals, not a mandate; each one has a
cost at the call site.

| Archive name | Meaning | Suggested name | Note |
|---|---|---|---|
| `vectab` | Board occupancy, 64 cells | `board` | 80 lines mention it. |
| `vecjugadas` | Order of the eight moves | `move_order` | 8 entries. |
| `cab1`, `cab2` | Knight cell index | `knight_one_cell`, `knight_two_cell` | Global. |
| `cf1`, `cc1` | Knight one row and column | `knight_one_row`, `knight_one_column` | Four globals. |
| `jugada` | Move number and end sentinel | `move_number` + `game_over` | Needs a design change. |
| `orden` | Which knight starts | `first_player` | Compared with `1` and `2`. |
| `ganador` | Inverted winner flag | `game_has_winner` | Fix the polarity separately. |
| `cab` | Which knight is moving | `moving_knight` | Eight `if (cab==1)` sites plus the trapped checks. |
| `ccab` | Destination cell | `destination_cell` | Local to `movcab`. |
| `tirocab1`, `tirocab2` | Dice roll for the start order | `knight_one_roll`, `knight_two_roll` | Local. |
| `simcab1`, `simcab2` | Console glyph for a knight | `knight_one_glyph`, `knight_two_glyph` | Glyph constant. |
| `sep` | Console wall glyph | `wall_glyph` | Glyph constant. |
| `conversionvm` | Cell to row and column | `cell_to_row_column` | Opposite of the next row. |
| `conversionmv` | Row and column to cell | `row_column_to_cell` | Near-twin name. |

Read the `jugada` and `ganador` rows twice. They are the cases where a rename is not enough. The
name is not merely unclear; it is carrying two meanings or an inverted convention. Renaming without
changing the design would make the code look better and behave the same, which is exactly the trap
this lesson warns about.

## What the rename proves and does not prove

| Claim | Supported? | Why |
|---|---|---|
| The renamed copy prints the same output. | Yes | `index=19` from both paths. |
| The rename is behavior-preserving for all inputs. | No | Only `(2, 3)` was run. |
| The new names remove the local ambiguity. | Yes, by the reason table | `row` and `column` choose the reading. |
| The new names are the best possible names. | No | That is a review judgment. |
| The comment deletion is safe. | Yes, for the bench | The name carries the meaning. |
| The archive's comments are all repaired. | No | A sample was audited. |
| The archive's globals were renamed. | No | The bench has no globals. |
| The function is correct for a real board. | No | No bounds check. |
| The code is now tested. | No | One `printf` is a demo. |

This table is the honesty check. A record that says "the code is cleaner" without naming the sample
is overreaching. Say "for this input" when that is what you ran.

## Self-check (binary)

- [ ] I wrote the four predictions before opening the source.
- [ ] I read both `before.cpp` and `after.cpp` and diffed them.
- [ ] I ran `./run.sh` and saw `before=index=19`, `after=index=19`, `match=yes`.
- [ ] I recorded the compiler and flags.
- [ ] I filled all three rows of the rename table with a reason and a scope.
- [ ] I renamed the function and both parameters in a scratch copy.
- [ ] The renamed copy prints `index=19`.
- [ ] I deleted or rewrote the narrating comment and recorded why.
- [ ] I classified at least six archive comments as keep, rewrite, delete or repair.
- [ ] I cited the wrong-knight comment at line 78 and the wrong-statement comment at 310.
- [ ] I named what the rename does not fix: magic `8`, bounds, `jugada`, `ganador`, tests.
- [ ] I priced the defect by reader time and incident cost, not by taste.
- [ ] I linked the structural fix to `common-02` and `construction-03`.
- [ ] The archive is unmodified (`git status` in the archive is clean).

If a box is not ticked, the unticked box names the next step. There is no penalty for repeating one.

## Misconceptions this lesson refutes

| Misconception | Why it is wrong | What to do instead |
|---|---|---|
| "Renaming is cosmetic." | A name is the reader's compression of a decision; a bad one costs every read. | Rename with a reason table and prove output equality. |
| "Short names are always bad." | `i`, `id` and `cfg` are conventional and local. | Judge by scope and ambiguity, not by length. |
| "Spanish identifiers are the defect." | The language is not the problem; the missing decision is. | Keep the domain language and make the name specific. |
| "Every comment is documentation." | A comment without a test becomes a second truth that drifts. | Keep reasons, warnings and constraints; delete narration. |
| "A comment can fix a bad name." | The comment is not checked and will be copied with the code. | Fix the name; keep the comment only for what the name cannot say. |
| "A lying comment is harmless." | It sends the reader to the wrong statement, which is worse than no comment. | Delete or repair it with the line number as evidence. |
| "A rename and a behavior fix can share a commit." | Then the diff no longer proves the rename preserved behavior. | Separate the two changes. |
| "Renaming `ganador` to `winner` fixes it." | The value is inverted; the new name would preserve the bug. | Rename to `game_has_winner` and fix the polarity separately. |
| "The bench is the original code." | The bench compresses `conversionmv` and shortens `cf, cc` to `f, c`. | State the reconstruction boundary in the record. |

## Terms used here

| Term | One-line meaning |
|---|---|
| Identifier | A name the compiler resolves to a declaration. |
| Narration comment | A comment that restates what the next statement does. |
| Reason comment | A comment that explains why the code is shaped this way. |
| Stale comment | A comment that described an earlier version of the code. |
| Oracle | The implementation whose behavior you trust while you change the code. |
| Behavior-preserving | The output is unchanged for the tested inputs. |
| Scope | The region a name can be changed in without touching callers. |
| Magic number | A literal whose meaning is not named. |
| Evidence boundary | The line between what the run proves and what you infer. |

## What “done” means here

The **Self-check (binary)** above is the authoritative checklist. If every box is ticked, the
lesson is done for you. `meta.yml` mirrors the same criteria for tooling.

## Next step

- Continue to `archaeology-13` — technical documentation for maintainers. The names are only half of
  the contract; the README and the rules are the other half.
- If you want the harder rename, take `vectab` across the archive in a scratch copy and count the
  references. The compiler is the map; the diff is the price.
- If you disagree with the severity, write the context that changes it. A private script and a
  published library do not price the same name.

## References

- [`README.md`](README.md) — the lesson card, the build command and the acceptance criterion.
- [`RENAME-TEMPLATE.md`](RENAME-TEMPLATE.md) — the rename and comment tables you fill.
- [`code/before.cpp`](code/before.cpp) — the baseline with `calc`, `f`, `c` and the narrating
  comment.
- [`code/after.cpp`](code/after.cpp) — the same behavior with descriptive names and no comment.
- [`run.sh`](run.sh) — compiles both with `-std=c++20 -Wall -Wextra -Werror -O2` and compares.
- [`EVIDENCE.md`](EVIDENCE.md) — the portfolio record you fill in.
- [`../../../docs/guide/archaeology-track.md`](../../../docs/guide/archaeology-track.md) — §3, the
  failure taxonomy; §5, the severity scale; §6, tone and ethics.
- [`../../../docs/guide/lesson-catalog.md`](../../../docs/guide/lesson-catalog.md) — the lesson
  sequence and the `archaeology-12` acceptance criterion.
- [`../../../docs/archive.md`](../../../docs/archive.md) — the pinned archive, tag and commit.
- [archaeology-04](../archaeology-04-correctness-failures/lesson.md) — the prerequisite.
- [archaeology-10](../archaeology-10-polymorphism/lesson.md) — the same freeze-before-change habit.
- [common-02](../../../common/lessons/common-02-glossary/lesson.md) — the glossary that keeps one
  name per concept across languages.
- Archive: <https://github.com/DaydreamsDev/DaydreamsDev-guerra-de-caballos-archive>, tag `v1.0.0`,
  commit `284e144e41403afbf0cc44422876425cacfb2bd5`.
