# archaeology-14 — Tests and determinism

> [English](lesson.md) | [Español](lesson-es.md)

> **Status:** drafted · **Concept:** tests and determinism · **Prerequisite:** archaeology-03 ·
> **Estimated time:** 90 minutes
> **Reading:** ~20 minutes

## Why this lesson exists

The archive has four published source variants. Each one seeds with `srand(time(NULL))` and then
draws five values with `rand()`. None of them accepts a seed. None of them ships a test. The
archive's own `05-known-issues.md` states it in one line: "No automated tests and no deterministic
seed."

That is `failure-016`, a professional-practice failure. It changes nothing about the rules of the
game and everything about your ability to prove the rules still hold. Without a seed, you cannot
replay a board. Without a trace, you cannot tell a fixed bug from a moved one. Without a harness,
"it works on my machine" is the whole test plan.

The catalog also lists `failure-003`: `srand(time(NULL))` inside the retry loop. `archaeology-04`
diagnosed it as a correctness defect. This lesson looks at the same call from the testing side,
because it is the exact place where reproducibility was traded away.

The bench is deliberately small: a 44-line program, a 6-line golden file and a 27-line harness. The
program accepts `--seed` and uses the lab's 64-bit LCG. The golden file is the exact transcript for
seed 1. The harness runs the program twice with the same seed, compares the two runs, then compares
the first run with the golden file, and exits non-zero when either check fails.

The lesson is not "add a test framework". It is "make the program replayable, freeze one transcript,
and state precisely what that transcript can and cannot promise".

## What you will be able to do

By the end of this lesson you can:

- explain why a test needs a seed, not just a random number generator;
- separate the four inputs of a deterministic run: seed, generator, program logic and environment;
- read `srand(time(NULL))` inside a retry loop and name the two failures it causes;
- run a seeded program twice and prove the transcripts are identical;
- freeze a golden trace and compare a run against it byte for byte;
- run a different seed and explain which fields change and which cannot;
- state why `rand()` alone is not a portable golden trace;
- write a platform note for a captured legacy trace;
- distinguish an oracle from a test, and a demo from a regression check;
- price non-determinism by incident cost, not by taste;
- keep the archive read-only while you work.

The lesson is done when another
person can reproduce your run and follow your reasons.

## The concept: a test is a promise you can replay

### Determinism is a property of the whole pipeline

A run is reproducible when every input that can change the output is fixed or recorded. In this
program there are four:

| Input | Question | This bench |
|---|---|---|
| Seed | Where does the stream start? | `--seed N`, default 1. |
| Generator | Which algorithm advances the stream? | The lab's 64-bit LCG, written in the file. |
| Logic | What does the program do with each draw? | `position = (position + draw + 1) % 8`, three moves. |
| Environment | What else can change the bytes? | `printf` formats, newline, locale, integer width. |

Fix three and leave one free, and the run is not reproducible. The archive fixes the logic and the
environment, and leaves the seed free by calling `time(NULL)`. That single free input is enough to
make every capture worthless as evidence.

The bench closes the gap by making the seed an argument. It does not remove randomness; it makes
randomness an input instead of an ambient condition. That is the whole idea. A test does not need a
program without randomness. It needs a program whose randomness you can set.

### `rand()` is an interface, not an algorithm

The C++ standard fixes the signature of `rand()` and the guarantee that it returns a value between
`0` and `RAND_MAX`. It does not fix the sequence. Each implementation chooses its own generator.
Two common ones disagree on both the sequence and the range:

| Implementation | `RAND_MAX` | Generator family | Same seed, same numbers? |
|---|---:|---|---|
| glibc (Linux) | 2147483647 | Additive feedback, 31-bit state | Yes, on glibc. |
| Microsoft CRT (MSVC 6.0 / modern) | 32767 | Linear congruential, 15-bit state | Yes, on MSVC. |
| glibc vs MSVC | Different | Different | No. |

So `srand(1); rand()` is not a portable value. It is a value on one platform. A golden trace built
from `rand()` is a trace of the C runtime, not of the game. It can still be useful, but only with a
platform note that says which runtime produced it.

There is a second trap: `rand()` is global state. Every call advances one shared stream. Add a debug
print that draws a number, reorder two calls, or skip a draw on an error path, and every later value
shifts. The archive shows this in miniature: five draws, and the fifth is `opcion=(rand() % 8)+1`
inside `vecjug()` at `variant-01:180`. Change the placement code and the move generator changes too,
without touching `vecjug()` at all.

The bench avoids both traps: the generator is written in the file, its state is a local variable,
and the draw function takes the bound as an argument. Same source, same seed, same bytes, on every
platform that has a 64-bit unsigned type.

### A golden trace is an oracle, not a test

A golden trace is a file with the expected output. It answers one question: "what should the program
print?" It does not run anything, does not compare anything, and cannot fail. The test is the
combination of three things:

1. a command that produces output;
2. an oracle that says what the output should be;
3. a comparison that returns a non-zero exit code when they differ.

`code/tests/golden-seed-1.txt` is the oracle. `run.sh` is the test. If you delete the `exit 1`
lines, you still have a demo: it prints `same=no` and continues, and a CI job would call it green.
That distinction is the difference between evidence and decoration.

### `srand(time(NULL))` inside the retry loop

Here is the archive's placement function, `variant-01:69-87`:

```cpp
void cabubicacion()
{
    int i=1,tirocab1,tirocab2;

    while  (i==1)
    {
        srand(time(NULL));
        tirocab1= rand() % 51;
        tirocab2= rand() % 51;
        cab1= rand() % 64;
        cab2= rand() % 64;
        if(cab1 != cab2 && tirocab1 != tirocab2)
        {
            i++;
        }
    }
    ...
}
```

The loop retries until the two knights land on different cells and roll different starting numbers.
The seed is recomputed on every attempt. `time(NULL)` has one-second granularity, so every retry
inside the same second receives the same seed, and therefore the same four draws. If those draws
collide, the next attempt collides in exactly the same way. The loop spins until the clock ticks.

Two consequences follow:

- **Reproducibility is gone.** The board depends on the wall clock, which is not an input you can
  write down and replay.
- **Retries are fake.** A retry is supposed to sample a new value. Here it re-samples the same
  value, so the loop is a busy-wait for the next second, not a rejection sampler.

This is the teaching point of the whole lesson: a seed is only useful when it is chosen once,
recorded, and passed in. A seed recomputed from the clock is entropy wearing the name of a seed.

## Before you start

Read these in full:

- [`README.md`](README.md) — the lesson card, the build command and the acceptance criterion.
- [archaeology-03](../archaeology-03-reverse-spec/lesson.md) — the prerequisite: a behavior spec and
  the first traces, taken from the code.
- [`../../../docs/guide/archaeology-track.md`](../../../docs/guide/archaeology-track.md) — §3, the
  failure taxonomy; §5, the severity scale; §6, tone and ethics.

You also need:

- a local clone of the archive source;
- a C++20 compiler (`c++`, `g++` or `clang++`);
- `bash`, `diff` and `cmp`;
- a scratch directory outside `original/`.

> **Rule:** the archive is read-only. Copy any file you want to change into a scratch directory;
> never edit `original/`. The bench in this lesson is also evidence: copy it before you edit.

## The 90-minute path

| Min | Step | What you do | Evidence |
|---:|---|---|---|
| 0–10 | Predict | Commit to answers about seeding, repetition and portability | Prediction table |
| 10–25 | Seed | Read the bench, find the three seams, run seed 1 | `seed=1` transcript |
| 25–35 | Run twice | Run the harness and two manual runs | `same=yes` |
| 35–45 | Compare | Diff against the golden file, byte for byte | `golden=yes`, `cmp` exit 0 |
| 45–70 | Explain | Run other seeds, write the portability note and the cost | Seed table and platform note |
| 70–90 | Record | Fill `EVIDENCE.md`; confirm the archive is clean | Portfolio entry |

If the portability question takes twenty minutes, that investigation is
the lesson.

### The engineering check in this lesson

Track B uses the same habit as Track A, with a different verb: the behavior already exists, so you
**freeze** it before you change anything.

| Step | Where it happens | What it produces |
|---|---|---|
| **Predict** | Step 1 | A falsifiable guess about seeds, repetition and portability. |
| **Seed** | Step 2 | One explicit input and one deterministic path. |
| **Run twice** | Step 3 | Two transcripts and a comparison with an exit code. |
| **Compare** | Step 4 | A byte-level match against a frozen oracle. |
| **Explain** | Step 5 | Why the seed matters and what the trace cannot promise. |
| **Record** | Step 6 | `EVIDENCE.md` with commands, environment and boundaries. |

## Step 1 — Predict (10 min)

Before you open the source, answer these five questions in writing. The point is to commit to
answers you can be wrong about.

1. Does the archive accept a seed on the command line? If not, what decides the starting state?
2. If you run a legacy variant twice within the same second, do you get the same board? Why?
3. If you run the bench twice with `--seed 1`, is the output identical?
4. If you run the bench with `--seed 2`, does the winner change?
5. Can a trace captured with `rand()` on Linux be replayed with the Microsoft compiler?

A useful prediction is falsifiable. "The code is random" is not. "The archive has no seed argument,
so the board is decided by the clock, and two runs in the same second produce the same board" is.

### A worked prediction

Before reading, I would write:

- the archive's `void main()` takes no arguments, so there is no seed input; `time(NULL)` decides
  it;
- two runs in the same second share a seed and therefore share the four placement draws;
- the bench prints the same six lines twice with `--seed 1`, because the seed is the only variable;
- `--seed 2` changes the positions and leaves `winner` and `moves` alone, because the bench
  hard-codes the ending after three moves;
- a glibc trace cannot be replayed on MSVC, because `rand()` is implementation-defined.

Every claim can be checked in the next four steps. If your prediction says "tests are good", you
have made a judgment, not an estimate. Write the mechanism you expect, then test it.

## Step 2 — Seed (15 min)

### Read the bench

```bash
cd archaeology/lessons/archaeology-14-tests-determinism
nl -ba code/seed_control.cpp
```

The program is 44 lines and has three seams worth naming:

| Seam | Lines | What it does |
|---|---:|---|
| Argument parsing | 25–31 | Reads `--seed N`; falls back to 1 when the shape does not match. |
| `set_seed` | 13–16 | Writes the seed into the generator state. Nothing else touches the state. |
| `below` | 18–22 | Advances the LCG and returns `state % bound`. |

The generator is a 64-bit linear congruential generator with the MMIX constants:

```cpp
randomState = randomState * 6364136223846793005ULL + 1442695040888963407ULL;
```

`below(3)` returns `0`, `1` or `2`. The move step adds one, so each move advances the position by
`1..3` cells. The position wraps with `% 8`. Three moves, one line each, then a fixed ending.

Compile it yourself, with the same flags the harness uses:

```bash
mkdir -p build
c++ -std=c++20 -Wall -Wextra -Werror -O2 code/seed_control.cpp -o build/seed-control
./build/seed-control --seed 1
```

Expected output:

```text
seed=1
move=1 position=2
move=2 position=5
move=3 position=0
winner=player
moves=3
```

### The arithmetic behind the trace

The transcript is not magic. You can recompute every position from the seed. The state is printed
here in decimal, truncated to the values that matter:

| Move | State after the draw | `state % 3` | Step | Position |
|---:|---:|---:|---:|---:|
| 1 | 7806831264735756412 | 1 | 2 | 2 |
| 2 | 9396908728118811419 | 2 | 3 | 5 |
| 3 | 11960119808228829710 | 2 | 3 | 0 |

Move 1 starts at position 0, adds 2, lands on 2. Move 2 adds 3, lands on 5. Move 3 adds 3, wraps
past 8 and lands on 0. If your run disagrees with this table, the generator or the constants
changed, not the game.

This is what "deterministic" buys you: the trace is derivable, not merely observed. You can argue
about it, recompute it, and check a port against it.

### Read the archive's seeding

In the archive source:

```bash
grep -n 'srand\|rand()' \
    original/variant-01-baseline-2008/source/guerra-de-caballos-baseline.cpp
grep -c 'srand' original/variant-0*/source/*.cpp
grep -rn 'argv\|argc' original --include=*.cpp
```

The first command reports one `srand` and five `rand()` calls: lines 76, 78, 79, 81, 82 and 180. The
second reports one seeding call per source variant. The third prints nothing: no variant reads
`argc` or `argv`. There is no seed argument to pass.

Then read the loop around line 73:

```text
73: while  (i==1)
76:     srand(time(NULL));
78:     tirocab1= rand() % 51;
79:     tirocab2= rand() % 51;
81:     cab1= rand() % 64;
82:     cab2= rand() % 64;
84:     if(cab1 != cab2 && tirocab1 != tirocab2)
```

The seed is recomputed on every retry. That is the defect, and it is visible in four lines.

The other variants repeat the pattern at `variant-02:87`, `variant-03:99` and `variant-04:105`, with
the same five draws. The precursor, `variant-00:36`, seeds once outside its loop — the lesser
version of the same mistake, because it still uses the clock.

### What seed control means here

Seed control is three properties, and the bench has all three:

1. **One input.** The seed is an argument, not an ambient condition.
2. **One path.** Nothing else in the program reads the clock, the environment or a file.
3. **One owner.** A single function writes the generator state, so a reviewer can see every write.

When a program has these, a test can name its input. When it has one of them, the test names a hope.

## Step 3 — Run twice (10 min)

### Run the harness

```bash
./run.sh
```

Expected output:

```text
first=seed=1
move=1 position=2
move=2 position=5
move=3 position=0
winner=player
moves=3
same=yes
golden=yes
```

Read `run.sh` before you trust it. It is 27 lines and every line matters:

- `set -euo pipefail` stops on the first error, including an unset variable;
- `-std=c++20 -Wall -Wextra -Werror -O2` makes a warning a build failure;
- `firstRun="$(...)"` captures stdout, and command substitution strips trailing newlines;
- the `if [[ ... != ... ]]` blocks compare whole transcripts, not fields;
- `exit 1` turns a mismatch into a failing test.

The last two points are the difference between a demo and a check. A script that prints `same=no`
and exits 0 is not a test. A script that exits 1 is.

### Do it by hand

```bash
./build/seed-control --seed 1 > build/run-a.txt
./build/seed-control --seed 1 > build/run-b.txt
diff build/run-a.txt build/run-b.txt
```

`diff` prints nothing and exits 0. Two runs, one seed, zero differences. That is `same=yes` as a
fact, not as a label.

### What `same=yes` proves

- The seed is read and applied.
- The generator state is not shared with anything else that changes between runs.
- The formatting is stable across two processes.
- Nothing in the program reads the clock, the environment or a file.

### What `same=yes` does not prove

| Claim | Proven? | Why |
|---|---|---|
| The program is deterministic for every seed. | No. | One seed was run twice. |
| The program is deterministic on another platform. | No. | Two runs used the same binary and libc. |
| The game logic is correct. | No. | The winner and the move count are hard-coded. |
| The output is byte-identical. | Not by `run.sh`. | Command substitution strips trailing newlines. |
| The test fails when the program changes. | Not yet. | That is Step 4, with a mismatch. |

## Step 4 — Compare (10 min)

### Read the oracle

```bash
cat code/tests/golden-seed-1.txt
```

```text
seed=1
move=1 position=2
move=2 position=5
move=3 position=0
winner=player
moves=3
```

Six lines, no commentary. The file is an oracle: it says what the bytes should be. It cannot run,
and it cannot fail.

### Compare bytes, not vibes

```bash
./build/seed-control --seed 1 | diff - code/tests/golden-seed-1.txt
./build/seed-control --seed 1 | cmp - code/tests/golden-seed-1.txt
```

`diff` prints nothing and exits 0. `cmp` prints nothing and exits 0, which means the streams are
byte-identical, including the final newline. This is stricter than `run.sh`, which compares strings
after stripping trailing newlines. Both are valid; they answer different questions. `run.sh` asks
"is the transcript the same?", `cmp` asks "are the bytes the same?".

Run it again:

```bash
./run.sh
```

`golden=yes` appears only after both checks pass: the two runs agree, and the first run matches the
file. The order matters. A golden match on a program that changes between runs is a coincidence, not
a result.

### What equality proves

- The seed parser produces the same seed the golden file was captured with.
- The generator constants and the draw order are unchanged.
- The move formula and the `% 8` wrap are unchanged.
- The format strings and the line order are unchanged.
- The run is replayable on this machine with this compiler.

### What equality does not prove

| Claim | Proven? | Why |
|---|---|---|
| The game is correct. | No. | The transcript is three moves of a bench. |
| The trace is portable. | No. | It was captured on one compiler and one libc. |
| The seed is the only input. | Partly. | No clock, no files; locale is still ambient. |
| The golden file is complete. | No. | It covers one seed and three moves. |
| The archive behaves the same. | No. | The archive has no seed argument. |
| The test catches every regression. | No. | A change outside these six lines is invisible. |

### When a mismatch appears

A golden mismatch is information, not an emergency. The usual causes, in order of frequency:

1. **The seed was not parsed.** `--seed 2` or a different argument shape changes the transcript.
2. **The generator changed.** A different multiplier, increment or word width changes every draw.
3. **The draw order changed.** A new draw before the loop shifts all three positions.
4. **The formula changed.** `+ 1` removed, or `% 8` changed to `% 64`.
5. **The format changed.** A space, a label or a newline is enough.
6. **The environment changed.** A locale-aware format, or a platform where `int` is not 32 bits.

Diagnose by printing the seed line first. If `seed=` differs, the problem is upstream of the
generator. If `seed=` matches and the first position differs, the problem is in the draw or the
formula. If only the last line differs, the problem is in the format.

Do not edit the golden file to make the run pass. That converts a failing test into a false one.

## Step 5 — Explain (25 min)

### A different seed

```bash
./build/seed-control --seed 2
./build/seed-control --seed 7
```

Observed:

| Seed | Move 1 | Move 2 | Move 3 | Winner | Moves |
|---:|---:|---:|---:|---|---:|
| 1 | 2 | 5 | 0 | player | 3 |
| 2 | 2 | 3 | 4 | player | 3 |
| 7 | 3 | 4 | 7 | player | 3 |

Seed 2 starts like seed 1 and diverges at move 2, because the first draw happens to give the same
step. That is normal: different seeds can share a prefix. The stream is different, not the shape.

The winner and the move count do not change, and that is the honest reading of this bench: the
ending is hard-coded after the loop. The seed changes the random path, not the script. A real game
would end because a knight captured the other, and then the seed would change the ending too. This
bench cannot show that, and the record must say so.

### Why `rand()` alone is not portable

`rand()` gives you an interface and a range, not a sequence. Three facts follow:

- **The sequence is implementation-defined.** glibc and MSVC produce different numbers for the same
  seed, and neither promises to keep its sequence across versions.
- **`RAND_MAX` differs.** On this machine it is 2147483647; on MSVC it is 32767. A `% 51` draw uses
  different bits of the state on each platform.
- **The state is global.** Any new call, in any translation unit, shifts every later draw.

So a golden trace captured with `rand()` is a trace of a runtime, not of a program. It is still
useful as a regression test *on that runtime*. It is not a specification, and it cannot be handed to
another team as "the expected output" without a note.

### The platform note a captured legacy trace needs

If you capture a transcript from a legacy binary, record at least these fields. A trace without them
is an anecdote:

| Field | Why it is needed | Example |
|---|---|---|
| Compiler and version | Different codegen and different CRT. | Microsoft Visual C++ 6.0. |
| C runtime / libc | Owns `rand()` and `RAND_MAX`. | MSVCRT, `RAND_MAX` 32767. |
| Architecture and word size | Affects integer width and overflow. | x86, 32-bit `int`. |
| Seed source | Without it there is nothing to replay. | `time(NULL)` — not replayable. |
| Exact seed value | The only reproducible input. | Captured by instrumenting a scratch copy. |
| Command line | Arguments change the path. | `guerra-de-caballos.exe`. |
| Output bytes | Line endings and encoding are part of the trace. | CRLF, CP437 console output. |
| Scope statement | Says what the trace is valid for. | "This binary, this runtime, this build." |

The archive fails at the fourth row: `time(NULL)` is not an input you can write down. To capture a
trace at all, you must patch a scratch copy to seed with a fixed value, and then the trace belongs
to the patched copy, not to the published artifact. Say that in the record. A trace of a modified
binary is still evidence, but it is evidence about the modification.

### Severity and cost

The track's severity scale puts "non-reproducible results" at **high**: fix in the minimal patch.
That is right, and the reason is economic, not aesthetic.

Without a seed:

- a bug report says "the knight started on top of the other one" and nobody can reproduce it;
- a fix cannot be proven, because the before and after boards differ for reasons unrelated to the
  fix;
- a refactor cannot be checked, because every run is a new experiment;
- a port cannot be validated, because there is no expected transcript to compare;
- a flaky test gets blamed on the test, when the real cause is an unseeded generator.

The cost is paid at the worst moment: during an incident, when the clock is running and the only
question is "did this change break it?". A seed costs one argument. Its absence costs the ability to
answer that question at all.

### Two fixes, two sizes

**Minimal fix — add the seed and freeze one trace.** Accept `--seed N`, seed once before the first
draw, and check in a golden file with a harness that exits non-zero on mismatch. That is what this
bench does in 77 lines across three files. It removes the wall clock from the path and gives you a
regression check for the transcript.

**Structural fix — own the generator and the trace.** Move the generator behind a small interface,
inject it into the code that needs randomness, and specify its algorithm and width in the
documentation. Keep the golden traces in version control, run them in CI, and record the platform
note for any trace that still depends on the environment. In the archive, that means replacing
`rand()` with a named generator and passing a seed from `main`, which also requires fixing
`void main()`.

Track A links: `common-01` (deterministic harness), `common-04` (golden traces), `construction-08`
(module boundary) and `construction-20` (hexagonal, where the console adapter owns the environment).

### What seed control does not fix

- The hard-coded winner in the bench. A seeded trace does not make the game logic correct.
- The modulo bias in `% bound`. It is small here and real in general; use a rejection method when it
  matters.
- The archive's `void main()`. A seed argument needs a standard `main(int, char**)`.
- The archive's global state. Seeding a global generator is still global state.
- The missing input validation. `--seed abc` becomes seed 0 through `atoi`.
- The platform note. A portable generator removes the runtime dependency; it does not remove the
  need to record the environment for legacy captures.
- The rest of the game. Three moves are not the 930-line original.

## Step 6 — Record (20 min)

Fill [`EVIDENCE.md`](EVIDENCE.md). At minimum:

- setup: compiler, libc, kernel and shell;
- the five predictions, written before inspection;
- the three seams and the generator constants;
- the archive anchors: one `srand` and five `rand()` per variant, no `argv`, line numbers;
- the seed-1 arithmetic table and the observed transcript;
- two runs with the same seed and the `diff` result;
- the byte comparison with `cmp` and its exit code;
- seeds 2 and 7, with the fields that changed and the fields that did not;
- the platform note for a captured legacy trace;
- the evidence boundary: one seed, one bench, one environment;
- the archive integrity check.

A sentence such as "the program is deterministic now" is an opinion. "Seed 1 produces positions
2, 5, 0 twice, `cmp` exits 0 against the golden file, seeds 2 and 7 produce different positions, and
the archive has no `argv`" is a record.

The acceptance criterion for this lesson is exactly two facts, both produced by the harness:

```text
same=yes
golden=yes
```

If you cannot produce both, the unticked line names the next step. There is no penalty for repeating
one.

## The original in detail

Facts you can cite, with line numbers from `variant-01`:

| Location | What is there |
|---:|---|
| 32 | `void main()` — no `argc`, no `argv`, no seed input. |
| 69–87 | `cabubicacion()` — placement and starting order. |
| 73 | `while (i==1)` — retry until the draws are distinct. |
| 76 | `srand(time(NULL));` — reseeded on every retry. |
| 78–79 | Two `rand() % 51` draws for the starting order. |
| 81–82 | Two `rand() % 64` draws for the cells. |
| 84 | The distinctness check that drives the retry. |
| 180 | `opcion=(rand() % 8)+1` — the fifth draw, inside `vecjug()`. |
| 930 | End of file; no test, no build file, no seed. |

The other variants repeat the pattern: `variant-02:87`, `variant-03:99`, `variant-04:105` for the
seed, and `variant-02:231`, `variant-03:245`, `variant-04:257` for the fifth draw. The precursor,
`variant-00:36`, seeds once outside its loop, which is better and still clock-dependent.

The archive's `05-known-issues.md` records both defects in its own words: `srand(time(NULL))` inside
the retry loop is "non-reproducible placement; repeated seeding", and the engineering-practice
section says "No automated tests and no deterministic seed." The lab is not inventing the diagnosis.

## What this harness proves and does not prove

| Claim | Supported? | Why |
|---|---|---|
| Seed 1 produces the golden transcript. | Yes | `golden=yes`, `cmp` exit 0. |
| Two runs with seed 1 are identical. | Yes | `same=yes`, empty `diff`. |
| A different seed changes the positions. | Yes | Seeds 2 and 7 differ from seed 1. |
| The program is portable. | No | One compiler, one libc. |
| The generator is unbiased. | No | `% bound` is biased in general. |
| The game logic is correct. | No | The ending is hard-coded. |
| The archive is now testable. | No | It has no seed argument and no standard `main`. |
| The harness catches every regression. | No | It covers six lines and one seed. |
| The golden file is a test by itself. | No | It is an oracle; `run.sh` is the test. |
| The trace is valid for another platform. | No | It needs the platform note. |

This table is the honesty check. A record that says "the code is tested" without naming the seed,
the platform and the scope is overreaching. Say "for seed 1, on this compiler" when that is what
you ran.

## Self-check (binary)

- [ ] I wrote the five predictions before opening the source.
- [ ] I read `code/seed_control.cpp` and named the three seams.
- [ ] I compiled with `-std=c++20 -Wall -Wextra -Werror -O2` and recorded the compiler.
- [ ] I ran `--seed 1` and saw the six-line transcript.
- [ ] I recomputed at least one position from the LCG state.
- [ ] I ran `./run.sh` and saw `same=yes` and `golden=yes`.
- [ ] I ran the program twice by hand and `diff` was empty.
- [ ] I compared with `cmp` and recorded its exit code.
- [ ] I ran seeds 2 and 7 and recorded the positions.
- [ ] I explained why the winner and the move count did not change.
- [ ] I recorded the archive anchors: one `srand`, five `rand()`, no `argv`.
- [ ] I explained why `srand(time(NULL))` inside the retry loop is worse than no seed.
- [ ] I stated why `rand()` is implementation-defined and why `RAND_MAX` matters.
- [ ] I wrote a platform note for a captured legacy trace.
- [ ] I distinguished the oracle from the test.
- [ ] I named what seed control does not fix: winner, bias, `void main`, globals, validation.
- [ ] I priced the defect by incident cost, not by taste.
- [ ] I linked the structural fix to `common-01` and `common-04`.
- [ ] The archive is unmodified (`git status` in the archive is clean).

If a box is not ticked, the unticked box names the next step. There is no penalty for repeating one.

## Misconceptions this lesson refutes

| Misconception | Why it is wrong | What to do instead |
|---|---|---|
| "A random program cannot be tested." | Randomness can be an input; a seeded run is replayable. | Make the seed an argument and freeze a trace. |
| "Same seed means same output everywhere." | `rand()` sequences are implementation-defined. | Own the generator, or record the platform. |
| "`rand()` is standard, so it is portable." | The interface is standard; the sequence is not. | Specify the algorithm and width. |
| "A golden file is a test." | It is an oracle; it cannot run or fail. | Pair it with a command, a comparison and an exit code. |
| "Running twice proves determinism." | It proves one seed on one environment. | State the seed, the platform and the scope. |
| "`srand(time(NULL))` is a seed." | It is entropy; it cannot be written down and replayed. | Seed once from an explicit value. |
| "A retry loop with `srand` inside samples again." | Within one second it re-samples the same values. | Seed once, outside the loop. |
| "Flaky tests are a test problem." | Usually the program is unseeded or shares state. | Fix the input, not the assertion. |
| "Tests need a framework." | A script with an exit code is a test. | Use the smallest harness that can fail. |
| "Capturing output by hand is a trace." | A trace needs a command, an environment and a comparison. | Record all three and compare bytes. |
| "A green run means the game is correct." | The bench hard-codes the ending. | Say what the trace covers. |

## Terms used here

| Term | One-line meaning |
|---|---|
| Determinism | The same inputs produce the same output. |
| Seed | The explicit value that starts a pseudo-random stream. |
| Entropy | A value taken from the environment, such as the clock. |
| Pseudo-random generator | A deterministic algorithm that looks random. |
| LCG | Linear congruential generator: `state = state * a + c`. |
| `RAND_MAX` | The largest value `rand()` may return on this runtime. |
| Implementation-defined | The standard allows each implementation to choose. |
| Golden trace | A checked-in file with the expected output. |
| Oracle | The source of truth a test compares against. |
| Harness | The command that runs the program and compares output. |
| Exit code | The number a program returns; non-zero means failure. |
| Regression | A behavior that used to work and no longer does. |
| Platform note | The environment record that makes a trace interpretable. |
| Modulo bias | Unequal probabilities caused by `% bound`. |
| Evidence boundary | The line between what the run proves and what you infer. |

## What “done” means here

The **Self-check (binary)** above is the authoritative checklist. If every box is ticked, the
lesson is done for you. `meta.yml` mirrors the same criteria for tooling.

## Next step

- Continue to `archaeology-15` — static analysis and compiler warnings. A trace in the tree is only
  useful if the build can still tell you when the code stops compiling cleanly.
- If the retry loop surprised you, copy `cabubicacion()` into a scratch file, replace `time(NULL)`
  with a fixed seed, and log the four draws per attempt. You will see the collisions repeat.
- If you want the harder trace, capture the archive's output on two compilers and diff them. The
  difference is the portability note, made visible.
- If you disagree with the severity, write the context that changes it. A one-off demo and a
  published library do not price the same missing seed.

## References

- [`README.md`](README.md) — the lesson card, the build command and the acceptance criterion.
- [`code/seed_control.cpp`](code/seed_control.cpp) — the seed-controlled baseline.
- [`code/tests/golden-seed-1.txt`](code/tests/golden-seed-1.txt) — the oracle for seed 1.
- [`run.sh`](run.sh) — compiles, runs twice, compares and exits non-zero on mismatch.
- [`EVIDENCE.md`](EVIDENCE.md) — the portfolio record you fill in.
- [`../../../docs/guide/archaeology-track.md`](../../../docs/guide/archaeology-track.md) — §3, the
  failure taxonomy; §5, the severity scale; §6, tone and ethics.
- [`../../../docs/guide/lesson-catalog.md`](../../../docs/guide/lesson-catalog.md) — the lesson
  sequence and the `archaeology-14` acceptance criterion.
- [archaeology-03](../archaeology-03-reverse-spec/lesson.md) — the prerequisite: the behavior spec.
- [archaeology-04](../archaeology-04-correctness-failures/lesson.md) — the correctness reading of
  `failure-003`.
- [archaeology-11](../archaeology-11-input-output/lesson.md) — the testability boundary this lesson
  assumes.
- [common-01](../../../common/lessons/common-01-deterministic-harness/lesson.md) — the deterministic
  harness.
- [common-04](../../../common/lessons/common-04-golden-traces/lesson.md) — golden traces as a
  behavioral contract.
