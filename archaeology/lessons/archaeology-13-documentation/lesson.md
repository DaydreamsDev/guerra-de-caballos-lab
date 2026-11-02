# archaeology-13 — Technical documentation for maintainers

> [English](lesson.md) | [Español](lesson-es.md)

> **Status:** drafted · **Concept:** documentation · **Prerequisite:** archaeology-03 ·
> **Estimated time:** 60 minutes
> **Reading:** ~20 minutes
> **Lesson role:** optional · safe to skip

## Why this lesson exists

`archaeology-03` reconstructed a behavior spec from the source. `archaeology-04` through
`archaeology-12` found failures and measured what they cost. None of that work reaches a maintainer
until it is written where the maintainer looks: a README, a behavior spec and commands that run.

The catalog summarizes this failure as `failure-017`: the artifact lacks the documents a maintainer
needs. Measured against the pinned archive, the summary is too broad. The archive has a root
`README.md`, a README per variant, and a `04-build-status.md` that names the historical toolchain.
What it does not have is narrower and more useful: no behavior specification, no exact build
command, no run command and no test command. A maintainer can read what the program is for and
still cannot build it, run it, or tell whether a change broke it.

That gap is the lesson. Documentation is not a description of the code. It is an interface with
three readers:

- the new maintainer, who needs to build and run the artifact today;
- the future you, who needs to remember why a command has three flags;
- the reviewer, who needs to know which claims were verified and which were assumed.

A README that says "build it with the original compiler" is not a build instruction. It is a
memory. A spec that says "the knight moves normally" is not a spec. It is a hope. This lesson turns
both into artifacts a stranger can execute and check.

## What you will be able to do

By the end of this lesson you can:

- inventory the documentation that exists and name the gap that matters;
- write a README that answers what the artifact is, how to build it, how to run it, how to test it
  and what remains unknown;
- write a behavior spec with one citation per clause and one confidence mark per clause;
- write commands that a stranger can paste, including the environment they assume;
- audit every command by running it from a clean directory;
- audit every claim against a line, a command or a named document;
- state unknowns without hiding them behind confident prose;
- keep the document focused on the artifact rather than on its context.

The lesson is done when another person can build, run and test the documented source without asking
you a question.

## The concept: documentation for maintainers

### Three readers, three questions

Documentation fails when it answers the writer's question instead of the reader's. The writer knows
the history, the machine and the shortcuts. The reader knows none of them.

| Reader | Question | What they need |
|---|---|---|
| New maintainer | How do I get this running? | Exact commands, dependencies, expected output. |
| Future you | Why is the command shaped this way? | The reason for each flag and each workaround. |
| Reviewer | How do I know the claim is true? | A source line, a command output or a named document. |

The README is for the first reader. The spec is for the third. The evidence record is for the
second. One document cannot serve all three well, which is why this lesson produces two artifacts
and a record.

### README, spec, test and comment are different contracts

| Artifact | Question it answers | Failure mode |
|---|---|---|
| README | How do I build, run and test this? | Commands that were never executed. |
| Behavior spec | What must happen? | Rules that describe the code instead of the behavior. |
| Test | How do I know it still happens? | A test that checks the implementation, not the contract. |
| Comment | Why is this line shaped this way? | A comment that narrates or lies. |

The README may link to the spec. The spec may link to the test. None of them may pretend to be the
others. A README that contains a full spec becomes stale in two places at once. A spec that contains
build commands cannot be read as a contract.

### An exact command is a contract

A command is exact when a reader can paste it and get the same result. That requires six things:

| Element | Question | Example |
|---|---|---|
| Tool | Which program runs? | `c++`, not "the compiler". |
| Version | Which dialect? | `-std=c++20`. |
| Flags | Which meanings change? | `-Dmain=legacy_main`. |
| Paths | From where? | `$ARCHIVE/...`, run from the lab root. |
| Input | What goes in? | `< /dev/null`. |
| Expected result | What comes out? | Exit 0, two files, one line. |

"We compiled it on Linux" is a memory. The six elements are an instruction.

The same rule applies to the run and the test. A run command without its input redirection is
incomplete: the program may wait for a key that never comes. A test command without its expected
result is a suggestion.

### A claim without a source is an opinion

Every sentence in the README and the spec is one of three kinds:

| Kind | Source | Example |
|---|---|---|
| Verified from a line | File and line number | "The board is 64 cells (`v01:17`)." |
| Observed from a command | Command and its output | "The source is 930 lines (`wc -l`)." |
| Documented elsewhere | Named document | "The historical toolchain is named in `04-build-status.md`." |

If a sentence has none of these, it is an opinion. Move it to the unknowns, or delete it.

The confidence mark is the second half of the discipline. A clause is **verified** when the source
line or the command output shows it. It is **inferred** when the evidence supports it but does not
prove it. It is **unknown** when the evidence is missing. The three marks are not decoration; they
tell the next maintainer how much to trust the sentence.

### Unknowns are content

A document that hides a blocker is worse than a document that names it. The reader will discover the
blocker anyway, after losing an afternoon. Naming it costs one line and saves the afternoon.

A useful unknown has three parts:

1. the question;
2. why it matters to a maintainer;
3. what would resolve it.

"The output is weird" is not an unknown. "The output line count varies between runs because the
source reseeds from the clock (`v01:76`); a fixed seed would resolve it, but the source has no seed
option" is.

### The scope test

A maintainer document answers four questions: how do I build it, how do I run it, how do I test it,
and what must the behavior be. If a sentence does not help answer one of those, it belongs in a
different record.

This test is stricter than it looks. It removes history, motivation, ownership, editorial notes and
anything else that is about the artifact's context rather than its use. Those facts may matter
somewhere, but they do not help a maintainer compile a file. Keeping them out is not censorship; it
is audience discipline.

### What the pinned archive actually contains

The archive is not undocumented. It is under-documented in a specific way.

| Document | Question it answers | What it does not answer |
|---|---|---|
| `README.md` | What the archive is and which variants exist. | How to build any variant. |
| `original/variant-01-baseline-2008/README.md` | Where the source is and what mechanism it uses. | What the rules are, or how to run it. |
| `04-build-status.md` | Which historical toolchain was used and why a modern build fails. | An exact modern command. |
| `documentation/en/01-technical-notes.md` | What each function does, in prose. | A behavior contract with citations. |
| `05-known-issues.md` | Which defects are known. | A test plan or a pass condition. |
| the source file | The implementation. | The specification. |

The gap is not "no prose". The gap is "no executable instructions and no contract". That is the gap
this lesson fills.

## Before you start

Read these in full:

- [`README.md`](README.md) — the lesson card and the acceptance criterion.
- [`README-TEMPLATE.md`](README-TEMPLATE.md) — the maintainer README you will fill.
- [`SPEC-TEMPLATE.md`](SPEC-TEMPLATE.md) — the behavior spec you will fill.
- [archaeology-03](../archaeology-03-reverse-spec/lesson.md) — the prerequisite: how to reconstruct
  a spec from source lines.
- [archaeology-01](../archaeology-01-build-today/lesson.md) — the compatibility probe whose headers
  and wrapper this lesson reuses.
- [`../../../docs/archive.md`](../../../docs/archive.md) — the pinned archive version.
- [`../../../common/spec/game-rules.md`](../../../common/spec/game-rules.md) — the modern spec, used
  only as a comparison target, never as evidence for the legacy source.

You also need:

- a local clone of the archive at the pinned version;
- a GCC-compatible C++20 compiler (`c++` or `g++`);
- `bash`, `timeout`, `wc`, `grep` and `cp`;
- a scratch directory outside the archive and outside the lesson templates.

> **Rule:** the archive is read-only. Copy the source into a scratch directory; never edit
> `original/`. The lesson templates are also inputs: copy them to your scratch documentation
> directory before you fill them.

Set two variables once per shell:

```bash
export ARCHIVE=/path/to/guerra-de-caballos-archive
export DOCBENCH=/tmp/guerra-doc-bench
```

`ARCHIVE` points at the pinned clone. `DOCBENCH` is the scratch directory for the source copy, the
compatibility headers and the build artifacts. Every command below uses these two names.

## The 60-minute path

| Min | Step | What you do | Evidence |
|---|---|---|---|
| 0–8 | Inspect | Inventory the existing documents and read the source anchors | Inventory table |
| 8–23 | Write | Fill the README template, including exact commands | README draft |
| 23–35 | Write | Fill the behavior spec template with citations and marks | Spec draft |
| 35–43 | Run | Execute every documented command from a clean directory | Run log |
| 43–52 | Verify | Audit every command and every claim | Audit tables |
| 52–57 | Explain | State the cost and the limits of the documentation | Explanation |
| 57–60 | Record | Fill `EVIDENCE.md` and check the archive | Portfolio entry |

If the build takes ten minutes because the compatibility headers surprise you, that investigation is
the lesson. Do not skip the run to save time; an unrun command is a guess.

### The engineering check in this lesson

| Step | Where it happens | What it produces |
|---|---|---|
| **Inspect** | Step 1 | The documentation inventory and the source anchors. |
| **Write** | Steps 2–3 | A README and a behavior spec. |
| **Run** | Step 4 | A command log with exit status and observed output. |
| **Verify** | Step 5 | One verdict per command and per claim. |
| **Explain** | Step 6 | The cost of the gap and what the docs do not fix. |
| **Record** | Step 7 | `EVIDENCE.md` with commands, sources and unknowns. |

## Step 1 — Inspect (8 min)

### Inventory what exists

From the lab root, with `$ARCHIVE` set:

```bash
wc -l "$ARCHIVE/README.md" \
    "$ARCHIVE/original/variant-01-baseline-2008/README.md" \
    "$ARCHIVE/04-build-status.md"
grep -n "build\|compile\|g++\|cl.exe\|main" \
    "$ARCHIVE/README.md" "$ARCHIVE/04-build-status.md" | head -20
```

The first command gives the size of each document. The second shows which build words appear. Read
the results as a maintainer: does any line contain a command you could paste? In the pinned archive,
the answer is no. The build document names a historical toolchain and lists modern blockers; it does
not give a command.

Fill the inventory table in `EVIDENCE.md` with one row per document: path, question it answers,
question it leaves open.

### Read the source anchors

The source is 930 lines. You do not need to read all of it to document it; you need the anchors that
the spec will cite.

```bash
wc -l "$ARCHIVE/original/variant-01-baseline-2008/source/guerra-de-caballos-baseline.cpp"
file "$ARCHIVE/original/variant-01-baseline-2008/source/guerra-de-caballos-baseline.cpp"
grep -c '//' \
    "$ARCHIVE/original/variant-01-baseline-2008/source/guerra-de-caballos-baseline.cpp"
grep -n "void  main\|#include\|srand\|rand()\|while (jugada\|ganador=0\|quedo encerrado" \
    "$ARCHIVE/original/variant-01-baseline-2008/source/guerra-de-caballos-baseline.cpp"
```

Record the exact output. On the pinned source:

- `wc -l` reports `930`;
- `file` reports `C source, ASCII text`;
- `grep -c '//'` reports `119`;
- the anchors appear at the lines used throughout this lesson.

Do not trust these numbers from the lesson. Run the commands and write what your machine printed. A
number copied from a lesson is not evidence; it is a quotation.

### Choose the scope

Write one sentence before you write the README:

> This document covers `<source path>` at `<version>`, built with `<compiler>` on `<platform>`, for
> a maintainer who needs to build, run and smoke-test it.

If the sentence needs two sources, two compilers or two platforms, cut it down. A document that
covers everything covers nothing exactly.

## Step 2 — Write the README (15 min)

From the lab root, copy the template into the scratch documentation directory:

```bash
mkdir -p "$DOCBENCH/docs"
cp archaeology/lessons/archaeology-13-documentation/README-TEMPLATE.md \
    "$DOCBENCH/docs/README.md"
```

Fill every section. The template is a checklist, not a suggestion.

### What it is

Three bullets, no history:

- **Purpose:** what the artifact does, in one sentence.
- **Scope:** which source, which version, which platform.
- **What it is not:** the nearest wrong assumption. "Not a port", "not deterministic", "not the
  modern transcript".

The "what it is not" line is the most valuable one. It stops the reader from expecting a property
the artifact never had.

### Build

The build section has four parts: dependencies, commands, flags and expected result.

Start with the compatibility layer. The source includes `conio.h` and `iostream.h` (`v01:11-12`),
and defines `void main()` (`v01:32`). The headers and the wrapper ship with `archaeology-01`; copy
them into the scratch directory so the build does not depend on the lesson tree at compile time.
Run these commands from the lab root.

```bash
mkdir -p "$DOCBENCH/compat"
cp "$ARCHIVE/original/variant-01-baseline-2008/source/guerra-de-caballos-baseline.cpp" \
    "$DOCBENCH/legacy.cpp"
cp archaeology/lessons/archaeology-01-build-today/code/compat/conio.h \
   archaeology/lessons/archaeology-01-build-today/code/compat/iostream.h \
   "$DOCBENCH/compat/"
cp archaeology/lessons/archaeology-01-build-today/code/legacy_wrapper.cpp \
   "$DOCBENCH/legacy_wrapper.cpp"
```

Then the two compile commands:

```bash
c++ -std=c++20 -fpermissive -Dmain=legacy_main -I"$DOCBENCH/compat" \
    -c "$DOCBENCH/legacy.cpp" -o "$DOCBENCH/legacy.o"
c++ -std=c++20 "$DOCBENCH/legacy_wrapper.cpp" "$DOCBENCH/legacy.o" \
    -o "$DOCBENCH/legacy-probe"
```

Each flag has a reason, and the README must give it:

| Flag | Reason | Risk |
|---|---|---|
| `-std=c++20` | The compiler dialect used for the recorded build. | The source is older; the dialect is a choice, not a claim. |
| `-fpermissive` | The probe passes it for the historical dialect. | It can downgrade diagnostics; it is not a correctness guarantee. |
| `-Dmain=legacy_main` | Renames the legacy entry point so the wrapper can call it. | The source is compiled with a macro that changes a name. |
| `-I"$DOCBENCH/compat"` | Resolves `conio.h` and `iostream.h` to the shims. | The shims are not the historical headers. |
| `-c` | Produces an object file; the wrapper supplies `main`. | None; it is a build stage. |

The README also states the expected result: `legacy.o` and `legacy-probe` exist, and both commands
exit 0. It states the known blocker: a naive build fails at `conio.h` because the modern toolchain
does not ship it.

Do not write "install the dependencies" without naming them. The dependencies here are one source
file, two headers and one wrapper, and the commands copy all four.

### Run

The run command must include the input redirection:

```bash
timeout 3s "$DOCBENCH/legacy-probe" < /dev/null \
    > "$DOCBENCH/stdout.txt" 2> "$DOCBENCH/stderr.txt"
printf 'run_status=%s\n' "$?"
wc -l "$DOCBENCH/stdout.txt" "$DOCBENCH/stderr.txt"
```

Why each part exists:

- `timeout 3s` stops a program that waits for a key. The compatibility `getch()` is a no-op, but the
  timeout is the guard that makes the command safe on any machine.
- `< /dev/null` makes the input explicit. A reader should not have to guess whether the program
  reads stdin.
- the two redirections separate the transcript from the failed console commands.
- `printf` records the exit status. A status of `124` means the timeout fired; it is data, not a
  crash.
- `wc -l` records the volume. The volume varies because the source reseeds from the clock
  (`v01:76`).

The README states the seed control: none. The source calls `srand(time(NULL))` inside the placement
retry loop, so two runs are not comparable. This is a fact a maintainer must know before trying to
reproduce a bug.

### Test

There are no golden traces for this source. The test section says so, then gives the strongest test
that exists:

```bash
grep -q 'Juega primero el caballo' "$DOCBENCH/stdout.txt" \
  && echo 'smoke=pass' || echo 'smoke=fail'
```

The test checks that the program reached the start-order announcement. It does not check the rules,
the winner, the board or the encoding. The README states that boundary in one line: "What is not
covered: determinism, rule correctness, capture and trap paths, encoding."

A weak test that is documented is better than a strong test that does not exist. The reader knows
exactly what the command proves.

### Behavior spec and evidence

The README points to `SPEC.md` and states what the spec covers and does not cover. The evidence
section records the source path, the commands run, the results and the unknowns. It does not repeat
the spec.

### Weak and strong sentences

| Weak | Strong | Why |
|---|---|---|
| "Build with the original compiler." | The two `c++` commands above, with flags and expected files. | The reader can execute the second. |
| "The program prints the board." | "Eight rows of eight cells, using bytes `0x15`, `0x9C`, `0xDB` (`v01:16`, `v01:870-928`)." | The claim is observable and cited. |
| "The game is random." | "`srand(time(NULL))` at `v01:76`; no seed option; two runs differ." | The mechanism is named. |
| "Tests pass." | The smoke command, its output and its coverage boundary. | The reader can reproduce the verdict. |
| "See the docs." | A relative link to `SPEC.md` and the section name. | The reader lands on the claim. |

## Step 3 — Write the behavior spec (12 min)

From the lab root, copy the template:

```bash
cp archaeology/lessons/archaeology-13-documentation/SPEC-TEMPLATE.md \
    "$DOCBENCH/docs/SPEC.md"
```

The spec is not a tour of the functions. It is a contract stated in observable terms. Each clause
has four parts: the rule, the source lines, the confidence mark and the boundary.

### A worked clause table

| Clause | Spec text | Source | Mark |
|---|---|---|---|
| Board | 8 rows by 8 columns, stored as 64 integers. | `v01:17`, `v01:226-229` | verified |
| Pieces | Two knights, one per player; no other pieces. | `v01:18`, `v01:81-82` | verified |
| Start cells | Two distinct cells, each `rand() % 64`; redraw while equal. | `v01:81-83` | verified |
| Start order | Each knight rolls `rand() % 51`; redraw while equal; the higher roll starts. | `v01:78-83`, `v01:730-748` | verified |
| Turn order | The starter moves first; turns alternate until the game ends. | `v01:753-868` | verified |
| Move order | Each turn draws `rand() % 8 + 1` and evaluates the eight L-moves in that rotation. | `v01:176-205` | verified |
| Legal move | The destination is inside the board and empty or occupied by the other knight. | `v01:245-252` | verified |
| Capture | Moving onto the other knight sets `ganador=0`; the mover wins and the game ends. | `v01:254-257`, `v01:768-772` | verified |
| Trap | If all eight moves fail, `jugada=0` and the game ends. | `v01:702-717` | verified |
| Draw | No draw clause exists in the source. | `v01:44`, `grep -n empate` returns nothing | inferred |
| Output | Console text with bytes `0x15`, `0x9C`, `0xDB`; no transcript format. | `v01:16`, `v01:870-928` | verified |
| Randomness | `srand(time(NULL))`; `rand()` for placement, rolls and move order. | `v01:76`, `v01:78-82`, `v01:180` | verified |

Read the draw row twice. The absence of a branch is evidence, but "there is no draw" is an inference
from the whole control flow. Mark it inferred and say what would change the answer: a path that ends
the game without a capture or a trap.

The output row needs the same care. The source names three integer values (`v01:16`) but not a code
page. CP437 is the mapping that turns `0x15`, `0x9C` and `0xDB` into the board glyphs. A byte check
of the archived output records the code page. If you cannot reproduce that check, mark the code page
as inferred and cite the byte values as verified.

### Separate the three layers

The spec keeps rules, randomness and presentation apart. A reader who copies the implementation
into the spec has written a description, not a contract.

- **Rules:** board, start, turns, legal move, capture, trap.
- **Randomness:** generator, seed, redraw conditions, non-reproducibility.
- **Presentation:** console text, console glyphs, board layout, Spanish strings.

The separation matters because a change to the presentation must not look like a change to the
rules. If the board is redrawn in UTF-8, the rules are unchanged; the spec should make that obvious.

### The boundary of the spec

The spec covers the source as written. It does not cover:

- what the historical compiler did with undefined behavior;
- what the private historical binary printed;
- how often a game ends by capture versus trap;
- how a CP437 terminal renders the bytes on the maintainer's machine.

Those are unknowns, not gaps in the writing. Put them in the spec's unknown section with a
resolution path.

## Step 4 — Run every command (8 min)

### Use a clean directory

Open a new shell and set the two variables again. Do not rely on your history, your current
directory or a file you created earlier.

```bash
export ARCHIVE=/path/to/guerra-de-caballos-archive
export DOCBENCH=/tmp/guerra-doc-bench
rm -rf "$DOCBENCH/compat" "$DOCBENCH/legacy.cpp" "$DOCBENCH/legacy.o" \
       "$DOCBENCH/legacy-probe" "$DOCBENCH/legacy_wrapper.cpp" \
       "$DOCBENCH/stdout.txt" "$DOCBENCH/stderr.txt"
mkdir -p "$DOCBENCH/compat"
```

Copy the source and the headers as the README says, then paste the README's blocks in order. The
point is not to prove that you can type; it is to prove that the document is self-contained.

### Record the four results

For every command block, record:

| Field | Why |
|---|---|
| Command | The exact text, or a stable short form. |
| Directory | Commands that depend on the working directory are not portable. |
| Exit status | `0` is the expected path; anything else is evidence. |
| Observed output | The lines the command printed, or the file sizes it produced. |
| Expected output | What the README promised. |
| Match? | Yes or no. A "no" means the README is wrong, not the machine. |

On this machine, the build block exits `0` and produces `legacy.o` and `legacy-probe`. The run block
exits `0`; stdout has hundreds of lines and the count varies between runs. The first stderr line is
the failed Windows console command, quoted verbatim in the record. The smoke test prints
`smoke=pass`.

Your numbers will differ. Record yours. A lesson that ships one number and calls it universal is
doing the same thing this lesson warns against.

### When a command fails

Do not edit the transcript. Edit the README. The common causes are:

- a missing `$DOCBENCH` directory;
- a relative path that assumed the lab root;
- a compiler that does not accept `-fpermissive`;
- a timeout that fired before the program finished;
- a locale that changes the stderr text.

Each cause is a missing sentence in the README. Add the sentence, rerun from a clean directory, and
record both attempts.

## Step 5 — Verify every claim (9 min)

### Command audit

Run the README from top to bottom in one fresh shell. Tick each block.

| Check | Pass condition |
|---|---|
| Every `bash` block runs | No command needs an undocumented variable or file. |
| Every command has an expected result | The README says what success looks like. |
| Every path is defined | `$ARCHIVE` and `$DOCBENCH` are set before first use. |
| Every input is explicit | stdin, stdout and stderr are named. |
| Every failure is documented | The naive build blocker and the timeout status are stated. |

A command that only works because of a file left in your home directory is not documented. Delete
the file and rerun.

### Test the documented blocker

The README says a naive build fails at `conio.h`. Prove it:

```bash
c++ -std=c++20 -c "$DOCBENCH/legacy.cpp" -o "$DOCBENCH/naive.o" \
    2> "$DOCBENCH/naive.err" \
  && echo 'naive=unexpected-pass' || echo 'naive=fail-as-documented'
head -n 1 "$DOCBENCH/naive.err"
```

Expected: `naive=fail-as-documented`, and the first stderr line names `conio.h`. If the build
succeeds, the README's blocker is wrong for your compiler; update the sentence and record the
compiler.

### Claim audit

Take every sentence that asserts a fact and give it a source.

| Claim | Source | Type | Verdict |
|---|---|---|---|
| The source is 930 lines. | `wc -l` output | command | verified |
| The board is 8 by 8. | `v01:17`, `v01:226-229` | line | verified |
| The game has no draw. | `v01:44`; `grep -n empate` returns nothing | inference | inferred |
| The output bytes are `0x15`, `0x9C`, `0xDB`. | `v01:16` | line | verified |
| The code page is CP437. | a byte check of the archived output | inference | inferred |
| The historical toolchain was MSVC 6.0. | `04-build-status.md` | document | documented, not verified here |
| The smoke test covers the rules. | the test command | command | false; it covers one substring |

The last row is the useful one. A claim can be false even when the command passes. The test checks
that the program reached the start announcement; it does not check the rules.

### The scope check

Read every sentence and ask the four questions from the scope test. A quick way to find drift is to
list the headings and check that each one serves build, run, test or behavior:

```bash
grep -n '^#' "$DOCBENCH/docs/README.md" "$DOCBENCH/docs/SPEC.md"
```

The output is the outline. If a heading does not serve one of the four questions, move it to a
different record. The same test applies to a sentence: a year in a folder name is an identifier,
but a paragraph about when something happened is context.

### The reproducibility check

Run the build and the smoke test twice. The build should produce the same artifacts. The smoke test
should pass both times. The stdout line count should not be identical, because the source is not
deterministic. If the count is identical twice, do not conclude that the program is deterministic;
run a third time and record what happened.

## Step 6 — Explain the cost (5 min)

### Why documentation fails

Most missing documentation is not lazy. It is written from memory, after the work, by the person who
no longer needs it. That produces three predictable failures:

- the command omits a flag that the writer's shell already had;
- the spec describes the code's structure instead of its behavior;
- the unknown is omitted because the writer knows the answer.

The fix is procedural, not moral: run the commands from a clean directory, cite every clause, and
write the unknown down before it becomes invisible.

### What the gap costs

A maintainer who cannot build loses the first hour. A maintainer who builds with the wrong flags
loses the first day. A maintainer who trusts an uncited spec loses the first incident.

The cost is low per reader and high per handoff. That is why documentation is a professional
practice failure rather than a correctness failure: nothing crashes, and the organization pays in
repeated onboarding.

### Two fixes, two sizes

**Minimal fix — this README and this spec.** One source, one platform, one compiler, exact commands,
cited clauses, named unknowns. It removes the immediate blocker and gives the next reader a starting
point.

**Structural fix — make the documentation executable.** Put the commands in a script that CI runs in
a clean container. Make the spec the authority and link it from the source. Add a test that compares
the documented output with the observed output. Track A links: `common-04` (build and run),
`common-05` (golden traces) and `construction-20` (the boundary where presentation is adapted).

### What this documentation does not fix

- It does not make the program deterministic. `srand(time(NULL))` is still there.
- It does not add a regression test. The smoke test checks one substring.
- It does not make the source portable. The compatibility headers are a shim.
- It does not prove the rules are correct. It records what the source does.
- It does not replace the code. A README cannot fix an off-by-one.

Documentation makes the next change cheaper. It does not make the current code correct.

## Step 7 — Record (3 min)

Fill [`EVIDENCE.md`](EVIDENCE.md):

- setup: archive path, compiler, shell, scratch directories;
- the documentation inventory;
- the source anchors with line numbers;
- the README decisions, including the commands and the flags;
- the spec clauses with citations and confidence marks;
- the command log with exit status and observed output;
- the claim audit;
- the unknowns and their resolution paths;
- the archive integrity check.

A sentence such as "the README is clear" is an opinion. "Two commands, both exit 0, `legacy.o` and
`legacy-probe` exist, the smoke test prints `smoke=pass`, the spec has 12 clauses and two inferred
clauses (draw and code page)" is a record.

## The original in detail

Facts you can cite, with line numbers from `variant-01` at the pinned version:

| Location | What is there |
|---:|---|
| 10–14 | The five `#include` lines, including `conio.h` and `iostream.h`. |
| 16 | `simcab1=21`, `simcab2=156`, `sep=219`: integer values; the source names no code page. |
| 17 | `vectab[64]`, `vecjugadas[8]`: the board and the move order. |
| 18 | `cab1`, `cab2`, `jugada`, `orden`, `ganador`: the mutable state. |
| 32 | `void main()`: not standard C++. |
| 38, 43 | `system("color 1f")`, `system("cls")`: Windows console calls. |
| 42 | `getch()`: the only input. |
| 44 | `while (jugada!=0)`: the game loop and the end sentinel. |
| 61 | `for (i=0;i<=64;i++)`: one write past the 64-element board. |
| 76 | `srand(time(NULL))`: reseeded inside the placement retry loop. |
| 78–83 | Rolls `rand() % 51` and cells `rand() % 64`; redraw on collision. |
| 104–112 | `conversionvm`: cell index to row and column. |
| 176–205 | `vecjug`: a random rotation of the eight move cases. |
| 226–229 | `conversionmv`: `cf * 8 + cc`. |
| 234–724 | `movcab`: eight hand-copied cases and the trap check. |
| 254–257 | Capture sets `ganador=0`. |
| 702–717 | All eight cases fail: `jugada=0`, trapped message. |
| 726–751 | `ordenjuego`: the higher roll starts. |
| 753–868 | `impresionorden`: turns, capture announcement, board redraw. |
| 870–928 | `impresion`: the 8 by 8 board, using the three integer values. |
| 930 | The closing brace; the file ends here; no tests, no build file. |

The source is 930 lines and 119 of them contain `//`. That is a documentation surface, not a
behavior contract: as `archaeology-12` documents, comments narrate, explain or lie, and none of them
is executed.

## A worked README

This is the shape the template asks for, filled for `variant-01`. It is an example, not a
replacement for your run: your compiler, your paths and your output volume belong in your copy.

````markdown
# Guerra de Caballos — baseline

> Build, run and smoke-test the archived baseline through a compatibility wrapper.

## What it is

- Purpose: study the behavior of the archived two-knight console program on a modern Linux host.
- Scope: one source file, one compiler, one console; the source is not edited.
- What it is not: not a port, not a bug-fix branch, not deterministic, not the modern transcript
  format.

## Build

Run from the lab root. Set the archive path and a scratch directory:

```bash
export ARCHIVE=/path/to/guerra-de-caballos-archive
export DOCBENCH=/tmp/guerra-doc-bench
```

Prepare the compatibility layer and the source copy:

```bash
mkdir -p "$DOCBENCH/compat"
cp "$ARCHIVE/original/variant-01-baseline-2008/source/guerra-de-caballos-baseline.cpp" \
    "$DOCBENCH/legacy.cpp"
cp archaeology/lessons/archaeology-01-build-today/code/compat/conio.h \
   archaeology/lessons/archaeology-01-build-today/code/compat/iostream.h \
   "$DOCBENCH/compat/"
cp archaeology/lessons/archaeology-01-build-today/code/legacy_wrapper.cpp \
   "$DOCBENCH/legacy_wrapper.cpp"
```

Compile:

```bash
c++ -std=c++20 -fpermissive -Dmain=legacy_main -I"$DOCBENCH/compat" \
    -c "$DOCBENCH/legacy.cpp" -o "$DOCBENCH/legacy.o"
c++ -std=c++20 "$DOCBENCH/legacy_wrapper.cpp" "$DOCBENCH/legacy.o" \
    -o "$DOCBENCH/legacy-probe"
```

- Toolchain: `c++`; record `c++ --version`.
- Dependencies: two compatibility headers and one wrapper, copied above.
- Expected result: `legacy.o` and `legacy-probe` exist; both commands exit 0.
- Known blockers: a naive build fails at `conio.h`; the source uses `void main`, `iostream.h`
  and Windows console calls (`v01:12`, `v01:32`, `v01:38`, `v01:43`).

## Run

```bash
timeout 3s "$DOCBENCH/legacy-probe" < /dev/null \
    > "$DOCBENCH/stdout.txt" 2> "$DOCBENCH/stderr.txt"
printf 'run_status=%s\n' "$?"
wc -l "$DOCBENCH/stdout.txt" "$DOCBENCH/stderr.txt"
```

- Input: none; stdin is `/dev/null`, and `getch()` is a no-op in the compatibility header.
- Output: stdout is the console transcript; stderr holds the failed console commands.
- Seed control: none. `srand(time(NULL))` at `v01:76` reseeds on every placement attempt.
- Expected result: `run_status=0`; stdout has at least one line; the line count varies per run.

## Test

```bash
grep -q 'Juega primero el caballo' "$DOCBENCH/stdout.txt" \
  && echo 'smoke=pass' || echo 'smoke=fail'
```

- Golden traces: none.
- Comparison method: a substring smoke test for the start-order line.
- What is not covered: determinism, rule correctness, capture and trap paths, encoding.

## Behavior spec

- Spec file: `SPEC.md`.
- What the spec covers: board, start, turn order, legal move, capture, trap, randomness, output.
- What the spec does not cover: the historical compiler, undefined behavior, exact output volume.

## Evidence

- Source path: `$ARCHIVE/original/variant-01-baseline-2008/source/guerra-de-caballos-baseline.cpp`.
- Commands run: the build, run and test blocks above, in a clean shell.
- Results: both build commands exit 0; run exits 0; the smoke test prints `smoke=pass`.
- What remains unknown: the historical binary's exact output, the game-length distribution,
  the CP437 rendering on other terminals.
````

## What the documentation proves and does not prove

| Claim | Supported? | Why |
|---|---|---|
| The commands run on the recorded machine. | Yes | The command log has exit status and output. |
| The commands run on every machine. | No | Only one compiler and one platform were tested. |
| The spec matches the cited source lines. | Yes | Each clause names its lines. |
| The spec is complete. | No | The unknown section names what is missing. |
| The smoke test catches regressions. | No | It checks one substring. |
| The program is deterministic. | No | `srand(time(NULL))` at `v01:76`. |
| The behavior is correct. | Not addressed | The spec records behavior, not correctness. |
| The historical toolchain was MSVC 6.0. | Documented only | `04-build-status.md` says so; this lesson did not verify it. |
| The documentation is maintainer-ready. | Partly | A reviewer must still judge the prose. |

This table is the honesty check. A README that claims "fully documented" after one smoke test is
overreaching. Say "for this source, this compiler and this test" when that is what you ran.

## Self-check (binary)

- [ ] I inventoried the existing documents and named the gap.
- [ ] I wrote the scope sentence before drafting the README.
- [ ] I filled every section of `README-TEMPLATE.md`.
- [ ] The build section names the toolchain, flags, dependencies and expected files.
- [ ] The run section names the input, output, seed control and expected status.
- [ ] The test section names the command and what it does not cover.
- [ ] I filled `SPEC-TEMPLATE.md` with one clause per rule.
- [ ] Every spec clause cites source lines and carries a confidence mark.
- [ ] Rules, randomness and presentation are separate.
- [ ] I ran every README command from a clean directory.
- [ ] I recorded exit status and observed output for every command block.
- [ ] I audited every factual claim against a line, a command or a document.
- [ ] I stated the unknowns with a resolution path.
- [ ] The scope check passes.
- [ ] The archive is unmodified (`git status` is clean).
- [ ] `EVIDENCE.md` is filled.

If a box is not ticked, the unticked box names the next step. There is no penalty for repeating one.

## Misconceptions this lesson refutes

| Misconception | Why it is wrong | What to do instead |
|---|---|---|
| "Documentation is prose about the code." | Prose that cannot be executed or checked is a memory. | Write commands and cite clauses. |
| "The README should explain the history." | A maintainer needs to build, run and test first. | Keep the artifact's context out of the build path. |
| "A command is exact if it worked once." | It may depend on a shell variable, a directory or a file. | Rerun from a clean directory and name every input. |
| "The spec can copy the function names." | Function names are implementation; the contract is behavior. | State what an observer sees, then cite the lines. |
| "If the source has no draw branch, there is no draw." | That is an inference from the control flow. | Mark it inferred and say what would change the answer. |
| "A passing smoke test means the rules are covered." | The test checks one substring. | State the coverage boundary next to the command. |
| "Unknowns make the document look weak." | Hidden unknowns make the reader lose an afternoon. | Name the unknown and the resolution path. |
| "One README can serve maintainers and historians." | The audiences ask different questions. | Write the maintainer README; keep context elsewhere. |
| "The archive's README already documents the build." | It names the toolchain and the blockers; it gives no command. | Add the command you actually ran. |
| "A compatibility shim is a port." | The source still assumes the old dialect and console. | Say "shim" and list what it changes. |

## Terms used here

| Term | One-line meaning |
|---|---|
| Maintainer README | The document that tells a maintainer how to build, run and test. |
| Behavior spec | A contract stated in observable terms, with citations. |
| Confidence mark | Verified, inferred or unknown, attached to a clause. |
| Command contract | Tool, version, flags, paths, input and expected result. |
| Clean-directory run | Executing the document without relying on prior shell state. |
| Smoke test | A small check that the artifact starts and produces a known marker. |
| Golden trace | A recorded output used as the reference for future runs. |
| Compatibility shim | A header or wrapper that supplies what the old environment assumed. |
| Undefined behavior | A construct whose result the language does not define. |
| Scope sentence | One sentence naming the source, version, compiler and audience. |
| Evidence boundary | The line between what was run and what was assumed. |
| Documentation debt | The recurring cost of a document that cannot be executed or trusted. |

## What “done” means here

The **Self-check (binary)** above is the authoritative checklist. If every box is ticked, the
lesson is done for you. `meta.yml` mirrors the same criteria for tooling.

## Next step

- Continue to `archaeology-14` — tests and determinism. The README now says there are no golden
  traces; the next lesson builds them.
- If the compatibility build surprised you, keep the naive error and the working command side by
  side. The pair is the whole build document.
- If you want the harder version, document `variant-03` or `variant-04` and compare the two build
  paths. The differences belong in the README, not in a comment.
- If a claim has no source, delete it or move it to the unknowns. A shorter document with citations
  is stronger than a longer one without them.

## References

- [`README.md`](README.md) — the lesson card and the acceptance criterion.
- [`README-TEMPLATE.md`](README-TEMPLATE.md) — the maintainer README you fill.
- [`SPEC-TEMPLATE.md`](SPEC-TEMPLATE.md) — the behavior spec you fill.
- [`EVIDENCE.md`](EVIDENCE.md) — the portfolio record you fill.
- [archaeology-03](../archaeology-03-reverse-spec/lesson.md) — the prerequisite: reconstructing a
  spec from source lines.
- [archaeology-01](../archaeology-01-build-today/lesson.md) — the compatibility probe reused here.
- [`../../../docs/guide/archaeology-track.md`](../../../docs/guide/archaeology-track.md) — §2, the
  autopsy protocol; §3, the professional-practice class; §5, the severity scale.
- [`../../../docs/guide/lesson-catalog.md`](../../../docs/guide/lesson-catalog.md) — the lesson
  sequence and the `archaeology-13` acceptance criterion.
- [`../../../docs/archive.md`](../../../docs/archive.md) — the pinned archive version.
- [`../../../common/spec/game-rules.md`](../../../common/spec/game-rules.md) — the modern spec, used
  as a comparison target.
- [`../../../common/harness/README.md`](../../../common/harness/README.md) — a maintainer README
  with exact commands and a stated test boundary.
