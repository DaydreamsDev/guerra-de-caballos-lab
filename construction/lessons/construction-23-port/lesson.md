# Port to another language (portar)

> [English](lesson.md) | [Español](lesson-es.md)

**Concept:** syntax is disposable · **Time:** about 195 minutes · **Status:** drafted
**Reading:** ~20 minutes

## Why this lesson exists

`construction-22` ended with a contract: four named outcomes, one value that carries the answer, and
a caller that decides what happens next. The whole contract lived in C++ types — an `enum class`, a
`struct`, a `switch` without `default`.

This lesson asks a narrower question than "can you write Python". It asks which part of that
contract was the game, and which part was the language.

The C++ baseline is 1,071 lines across 20 files. The port is 245 lines across 2. The same 20 seeds
produce the same bytes.

The sentence to keep:

> A port is not a translation of the code. It is a re-derivation of the behavior from the rules.
> The syntax is disposable; the rules and the transcript are the contract.

Three honest notes before the comparison.

1. **The line counts are not a quality measure.** The C++ baseline carries two probe programs, a
   recording adapter and a parallel runner that the port does not have. Some of the 826 missing
   lines are ceremony; some are evidence. A shorter program is not automatically better,
   and this lesson never claims it is.
2. **The traces do not move.** All 20 golden traces match byte for byte, measured on this machine,
   before and after the change test. That is the behavioral contract, and the port does not touch
   it.
3. **This is not the first Python implementation.**
   [`common/harness/reference.py`](../../../common/harness/reference.py) is 251 lines of Python that
   produced the golden traces in the first place. The port does not import it and does not run it.
   But the two files are close cousins: same constants, same function order, same loop. A match
   between them is evidence that both encode the same rules; it is not independent proof that the
   rules are right. Keep that distinction for the whole lesson.

What this lesson is not: not "Python is better", not "less code is better", not "types are
unnecessary". It is a measurement of what survives when the compiler, the classes and the adapters
are removed and only the rules and the transcript remain.

## What you will be able to do

By the end of this lesson you can:

- explain why syntax is disposable and behavior is the contract;
- reproduce the traces from the Python port and name what changed.

## The concept: four layers, one contract

A port has four layers. Only one of them has to match exactly, and confusing the layers is the usual
reason a port is called "done" when it is not.

| Layer | What it is | Must match? | In the C++ baseline | In the port |
|---|---|---|---|---|
| Behavior | the bytes on stdout for each seed | yes, exactly | 20 traces | 20 traces |
| Rules | LCG, move order, capture, visited, trap, draw | yes, as rules | `game_engine.h`, `board.h` | `run_game`, `build_move_order` |
| Representation | how state is stored | no | `Board`, `Knight`, `Position`, `MoveResult` | tuples, a set, a list |
| Process | argv, stdout, exit status | partly, as a promise | `main` returns 2 | `argparse` exits 2 |

The transcript is the only layer you can compare mechanically. The rules are the explanation of why
it matches. The representation is free. The process interface is a promise to whoever runs the
program, not to the game.

### What "same behavior" means

Same bytes, same order, same trailing newline, for the same 20 seeds and the same player name. Not
"the same winner". Not "looks similar". `cmp` is silent or it is not.

### What the C++ layer held, and where it went

| C++ | Python | What happened to the concept |
|---|---|---|
| `Board` class with an invariant | module constants plus `is_inside_board` | the invariant becomes a predicate |
| `Position` with row and column | `tuple[int, int]` | the type becomes a shape |
| `Knight` with a name and a position | two position variables and a string | identity becomes a label |
| `MoveGenerator` | `KNIGHT_MOVE_CHANGES` plus `build_move_order` | the object becomes data |
| `RandomSequence` | `RandomSequence` | kept, smaller |
| `TranscriptPort` and two adapters | `list[str]`, joined once | the port becomes a list |
| `MoveOutcome`, `MoveResult`, `switch` | branch order and string fragments | the enum becomes words in the trace |
| `GameEngine::playTurn` | the body of `run_game` | the method becomes a loop |

Read the last two rows twice. The four outcome names did not disappear. `outside`, `visited`,
`capture` and `legal` are still in the program — as literal words in the transcript lines. The
contract survived as behavior, not as a type. That is the whole lesson in one sentence.

### What Python does not give you

- **No compiler.** `-Wall -Wextra -Werror` is gone. A wrong branch is a wrong transcript at run
  time, not an error at build time.
- **No exhaustive `switch`.** Adding a fifth outcome in C++ breaks the build until every `switch`
  handles it. In Python, a missing branch is a missing `elif`, and the trace quietly disagrees.
- **No `MoveResult`.** No value crosses a function boundary carrying the answer. The loop appends to
  the transcript directly and breaks. `construction-22`'s "the caller decides" becomes "the loop
  decides inline".
- **No `const`.** Nothing stops a function from mutating a set it received. The port happens not to;
  the language does not ask.
- **Types are hints.** `tuple[int, int]` documents intent and checks nothing at run time.

None of this is a defect of Python. It is the price of the layer the port chose to drop: the
compile-time contract. The lesson measures that price instead of hiding it.

## Before you start

- [`construction-09`](../construction-09-abstract-data-type/lesson.md) or
  [`construction-12`](../construction-12-polymorphism/lesson.md) — the catalog's formal
  prerequisite. You need to read a small value type, a function and a loop without designing either.
- [`construction-22`](../construction-22-error-contracts/lesson.md) — recommended, not required by
  the catalog. It supplies the contract this port has to carry: four outcomes, bounds first, capture
  before visited, and a caller that decides. If you skip it, read
  [`common/spec/game-rules.md`](../../../common/spec/game-rules.md) §7–§9 before Step 1.
- Python 3.10 or newer. The port uses `tuple[int, int]` and `str | None`. Check with
  `python3 --version`.
- A C++20 compiler (`g++` or `clang++`) only for the comparison side. If you do not have one, the
  golden traces are the recorded C++ output and you can use them as the other side.
- No third-party Python package. `argparse` is in the standard library.

### What you need

- The port: [`code/game.py`](code/game.py), [`code/main.py`](code/main.py) and [`run.sh`](run.sh).
- The C++ side, for the diff:
  [`../construction-22-error-contracts/code/`](../construction-22-error-contracts/code/) and its
  [`run.sh`](../construction-22-error-contracts/run.sh).
- The independent oracle: [`common/harness/reference.py`](../../../common/harness/reference.py).
- The rules: [`common/spec/game-rules.md`](../../../common/spec/game-rules.md) — the LCG in §4, move
  order in §6, legality in §7, visited in §8, capture in §9, trap and draw in §10.
- The transcript format:
  [`common/spec/transcript-format.md`](../../../common/spec/transcript-format.md).
- The 20 traces: [`common/traces/`](../../../common/traces/).
- This learner template: [`EVIDENCE.md`](EVIDENCE.md).

Check that the artifacts are present before you begin, from this lesson directory:

```bash
test -f code/game.py &&
test -f code/main.py &&
test -f run.sh &&
test -f ../../../common/harness/reference.py &&
test -f ../../../common/traces/seed-0001.txt &&
test -f ../construction-22-error-contracts/code/main.cpp &&
test -f ../construction-22-error-contracts/run.sh
```

If a file is missing, the comparison has no second side. Record it in `EVIDENCE.md` §11 and return
when it is available.

## Step 1 — Inventory the port (~20 min)

Start with size and shape:

```bash
wc -l code/game.py code/main.py ../../../common/harness/reference.py
```

```text
  211 code/game.py
   34 code/main.py
  251 ../../../common/harness/reference.py
  496 total
```

Two files, 245 lines for the port. The oracle is one file, 251 lines.

Now the top-level definitions:

```bash
grep -n '^class \|^def ' code/game.py code/main.py
```

```text
code/game.py:27:class RandomSequence:
code/game.py:43:def format_position(position: tuple[int, int]) -> str:
code/game.py:47:def is_inside_board(position: tuple[int, int]) -> bool:
code/game.py:55:def convert_cell_number_to_position(cell_number: int) -> tuple[int, int]:
code/game.py:61:def create_initial_positions(
code/game.py:74:def create_initial_rolls(
code/game.py:84:def build_move_order(starting_move_number: int) -> list[int]:
code/game.py:91:def other_knight(active_knight: str) -> str:
code/game.py:95:def run_game(
code/main.py:6:def parse_arguments() -> argparse.Namespace:
code/main.py:27:def main() -> int:
```

One class and eight functions in `game.py`; two functions in `main.py`. No header, no translation
unit, no build step, no third-party import.

Now the part that must not move. The LCG:

```bash
grep -n 'RANDOM_\|KNIGHT_MOVE_CHANGES\|DEFAULT_MAXIMUM_MOVE_COUNT' code/game.py
```

```text
5:KNIGHT_MOVE_CHANGES = (
20:DEFAULT_MAXIMUM_MOVE_COUNT = 200
22:RANDOM_MULTIPLIER = 6364136223846793005
23:RANDOM_INCREMENT = 1442695040888963407
24:RANDOM_MASK = (1 << 64) - 1
```

Compare with the C++ side:

```bash
grep -rn '6364136223846793005\|1442695040888963407\|200' \
    ../construction-22-error-contracts/code/random_sequence.cpp \
    ../construction-22-error-contracts/code/game_engine.h
```

The constants are the same decimal integers. The mask is the same `2^64 - 1`, written as a shift
instead of a literal. A port that changed one digit would still run, still produce a transcript, and
fail every trace.

Counts for this baseline. The counting rule is stated so you can reproduce it:

- Files in the port: 2 (plus `run.sh`, which is not source).
- Source lines: 245 (`game.py` 211, `main.py` 34).
- Headers: 0.
- Translation units: 0.
- Build steps: 0.
- Third-party dependencies: 0.
- Top-level definitions: 11 (1 class, 10 functions).
- Values in the LCG: 3 constants plus one 64-bit state.
- Move offsets: 8, in fixed order.
- Golden traces: 20.

Then answer in `EVIDENCE.md` §2:

1. Which file holds the LCG constants, and which file holds the game loop?
2. How many lines does each file have, and what is the total?
3. Where does the C++ baseline keep the four outcomes? Where does the port keep them?
4. What replaces `Board`, `Knight` and `Position`?
5. What replaces `TranscriptPort` and its two adapters?
6. What replaces `MoveResult`?
7. How many top-level definitions does `game.py` have?
8. Which file decides the process exit status, and through what call?

Question 8 has a precise answer: `argparse` decides. `parse_arguments` does not catch the error and
`main` does not return 2; `argparse` prints usage and calls `sys.exit(2)` inside the library. The
status is the same as the C++ baseline, the mechanism is not.

## Step 2 — Predict before you run (~25 min)

Write these predictions before running anything. Do not read Step 3 first.

### 2.1 The structure table

| Question | My prediction |
|---|---|
| Files in the C++ baseline | |
| Files in the port | |
| Source lines in the C++ baseline | |
| Source lines in the port | |
| Headers in the port | |
| Build steps in the port | |
| Top-level definitions in `game.py` | |
| Golden traces that match | |

The row that is easy to get wrong is "top-level definitions". The port has no `Board`, no `Knight`
and no `GameEngine`, but it still needs the LCG, the initial positions, the initial rolls, the move
order, the bounds check and the loop. Removing classes does not remove functions.

### 2.2 The behavior table

| Question | My prediction |
|---|---|
| Seeds that match, of 20 | |
| Traces that end in a capture | |
| Traces that end in a trap | |
| Traces that end in a draw | |
| Longest game, in moves | |
| Traces containing at least one `illegal=visited` line | |

Predict the last three by reasoning about the rules, not by reading the traces. A game ends in a
capture or a trap; a draw needs 200 moves. The board is 8×8 and both knights share the visited set,
so a long game is possible but not guaranteed.

### 2.3 The timing table

| Question | My prediction |
|---|---|
| 20 games through `run.sh`, wall clock | |
| 20 games in one Python process | |
| 20 C++ processes, wall clock | |
| 20 bare `python3 -c 'pass'` startups | |

Predict the in-process number well below the `run.sh` number. If your four numbers are close
together, you are predicting computation; the difference is mostly startup.

### 2.4 The change-test table

| Question | My prediction |
|---|---|
| Traces that fail if capture is checked after visited | |
| Traces that fail if the trap winner is flipped | |
| Traces that fail if the draw limit changes from 200 to 199 | |

For the third row, think before you answer: a rule can only break a trace that reaches it.

## Step 3 — Run the port (~50 min)

### 3.1 The 20 traces

From this lesson directory:

```bash
for trace in ../../../common/traces/seed-*.txt; do
    traceName="$(basename "${trace}" .txt)"
    seedText="${traceName#seed-}"
    seedNumber="$((10#${seedText}))"
    ./run.sh --seed "${seedNumber}" --name Learner | cmp - "${trace}"
done
```

A silent loop is 20 of 20. If `cmp` prints a byte offset, stop: that is a different baseline from
the one this lesson describes.

### 3.2 The C++ side

The traces are the recorded C++ output, but run the baseline once to see both programs side by side:

```bash
diff <(./run.sh --seed 7 --name Learner) \
     <(../construction-22-error-contracts/run.sh --seed 7 --name Learner)
```

No output. Two languages, one transcript. Repeat with a seed that ends in a trap, for example
seed 1, and with one that ends in a capture, for example seed 2.

If the C++ `run.sh` has to build, it uses CMake when available and falls back to a direct compile.
The first invocation is slow; the comparison is not about that build.

### 3.3 The third implementation

```bash
python3 ../../../common/harness/reference.py --seed 7 --name Learner \
    | cmp - <(./run.sh --seed 7 --name Learner)
```

No output. The oracle that generated the traces and the port agree on seed 7.

Now the honest caveat, as a command:

```bash
diff -u ../../../common/harness/reference.py code/game.py | head -40
```

The diff is mostly renames: `DeterministicRandom` becomes `RandomSequence`, `random_below` becomes
`below`, `position_is_inside_board` becomes `is_inside_board`, and the loop is reformatted. This is
not two independent designs converging. It is one design written twice in the same language. The
trace match is real; the independence is partial. Say so in `EVIDENCE.md` §5.

### 3.4 The timing, measured honestly

```bash
start=$(date +%s.%N)
for n in $(seq 1 20); do ./run.sh --seed "$n" --name Learner > /dev/null; done
end=$(date +%s.%N)
echo "$end - $start" | bc
```

On this machine, with Python 3.14.8:

```text
.867748884
```

Twenty games through twenty separate `run.sh` processes take about 0.87 s. The same 20 games through
the C++ baseline take about 0.03 s:

```bash
start=$(date +%s.%N)
for n in $(seq 1 20); do ../construction-22-error-contracts/run.sh \
    --seed "$n" --name Learner > /dev/null; done
end=$(date +%s.%N)
echo "$end - $start" | bc
```

```text
.026168650
```

Do not read that as "Python is 30 times slower". Measure what the number is made of:

```bash
start=$(date +%s.%N)
for n in $(seq 1 20); do python3 -c 'pass'; done
end=$(date +%s.%N)
echo "$end - $start" | bc
```

```text
.256494341
```

Twenty empty interpreters already cost 0.26 s. Now the game work without process startup:

```bash
start=$(date +%s.%N)
python3 - <<'EOF'
import sys
sys.path.insert(0, "code")
from game import run_game
for seed in range(1, 21):
    run_game(seed, "Learner")
EOF
end=$(date +%s.%N)
echo "$end - $start" | bc
```

```text
.015848224
```

Twenty full games in one process take about 0.016 s, under a millisecond per game. The 0.87 s is
almost entirely twenty interpreter launches plus the `run.sh` wrapper. The honest statement is: on
this machine, the Python figure is dominated by process startup, so it is not a core-to-core
comparison. A benchmark would need a loop inside one process on both sides, and this lesson does not
run one.

### 3.5 The argument path

```bash
./run.sh --seed 1
echo "exit=$?"
```

```text
usage: main.py [-h] --seed INTEGER --name TEXT
main.py: error: the following arguments are required: --name
exit=2
```

Compare with the C++ baseline, which also exits 2:

```bash
../construction-22-error-contracts/run.sh --seed 1
echo "exit=$?"
```

Same status, different owner. In C++, `main` returned 2 and the process ended normally. In Python,
`argparse` called `sys.exit(2)` from inside the library. `construction-22` made a point of the core
never calling `exit`; the port's argument layer delegates that decision. The transcript is
unaffected because no game starts.

### 3.6 Prediction and observation

Fill `EVIDENCE.md` §4.6 before moving on. The gap is the result, including when it is zero.

## Step 4 — Compare (~30 min)

Open `game.py` and `main.py` next to `game_engine.h`, `board.h`, `knight.h` and `game.cpp`. Five
facts, read against the measurements.

1. **The rules are all there, in the same order.** Bounds first, then capture, then visited, then
   legal. The starting move and the rotation are the same. The LCG is called the same number of
   times in the same order.
2. **The representation is gone.** No `Board`, no `Knight`, no `Position`, no `MoveResult`. A
   position is a two-integer tuple, the visited cells are a set of tuples, and the transcript is a
   list of strings.
3. **The error contract became control flow.** In C++, `tryMove` returned one value and `playTurn`
   branched on it. In Python, the loop appends the line, increments the counter, sets the winner and
   breaks, all in the same block. There is no value to inspect and no second caller.
4. **The compiler's job moved to the traces.** The four outcomes are still distinguished, but by the
   text `illegal=outside`, `illegal=visited`, `capture=...` and `legal`. A missing case produces a
   wrong transcript, not a build error.
5. **The process interface is nearly the same.** `--seed` and `--name` keep their names, stdout
   keeps its bytes, and a bad argument still exits 2. `--max-moves` is not in the port; the oracle
   has it.

### 4.1 Three things that changed

Name these in your own words in `EVIDENCE.md` §5, with a line of evidence for each.

1. **Representation and ownership.** `Board`, `Knight`, `Position`, `MoveGenerator`,
   `TranscriptPort` and the two adapters collapse into module constants, tuples, a set, a list and
   functions. There is no object to own the board invariant; `is_inside_board` checks a coordinate
   when asked.
2. **The compile-time contract.** `MoveOutcome` with four values, an exhaustive `switch`, and
   `MoveResult` carrying the destination are replaced by branch order and literal strings. Nothing
   checks the port at build time, and there is no public `evaluate_move` to probe.
3. **The build and the process.** A compiled binary becomes an interpreted script behind `run.sh`.
   Each run pays interpreter startup, and `argparse` owns the usage error and calls `sys.exit(2)`.

A fourth change is worth naming if you want it: 1,071 lines across 20 files become 245 across 2. It
is a consequence of the first three, not an independent decision.

### 4.2 Three things that did not change

1. **The generator.** `6364136223846793005`, `1442695040888963407`, the `2^64` mask, one state
   update per draw, the same number of draws in the same order.
2. **The move order and the check order.** Eight offsets in the same sequence, the same rotation
   from the starting move, bounds before capture, capture before visited, visited before legal.
3. **The transcript.** Every header line, every `turn=`, every `try=`, every `move=`, the trap line,
   the 200-move cap, `winner=` and `moves=`. All 20 traces match byte for byte.

Also unchanged: the command-line names `--seed` and `--name`, the stdout-only output, and the exit
status 2 for bad arguments.

### 4.3 What the port dropped and did not replace

This is the part a port hides if nobody looks. Five things are missing, and each one is a piece of
evidence the C++ baseline had.

| Dropped | What it provided | What replaces it |
|---|---|---|
| `parallel_runner.cpp` | two games in parallel, determinism checked | nothing; the port is single-process |
| `error_probe.cpp` and `evaluate_move` | the four outcomes without running a game | nothing; outcomes exist only as strings |
| `adapter_probe.cpp` | the recording adapter exercised without the console | nothing; only stdout remains |
| `RecordingTranscript` | the transcript as data, inspectable in-process | nothing; the transcript is stdout |
| `--max-moves` | a short game, to reach the draw on purpose | nothing; the cap is a constant |

The oracle keeps `--max-moves`; the port does not. The consequence is measurable: the draw path in
`run_game` cannot be reached by the 20 seeds, and without the flag it cannot be reached on purpose
either. That is a gap in the port's evidence, not in its rules. Record it in §7.

### 4.4 The behavior diff

```bash
for trace in ../../../common/traces/seed-*.txt; do
    traceName="$(basename "${trace}" .txt)"
    seedText="${traceName#seed-}"
    seedNumber="$((10#${seedText}))"
    diff <(./run.sh --seed "${seedNumber}" --name Learner) "${trace}"
done
```

No output, 20 times. That is the only diff that matters for behavior. The structural diff is not a
diff at all: there is no line-by-line correspondence between `game_engine.h` and `run_game`. A port
is a rewrite that agrees on output.

## Step 5 — Change test: one rule, in the port only (~40 min)

The acceptance names the change test: **run the port against all 20 seeds, compare with the C++
baseline, change one rule in the port only, observe which traces fail, and restore it.**

Never edit the tracked `code/` directory. Copy it:

```bash
rm -rf /tmp/port-change
cp -r code /tmp/port-change
```

`main.py` imports `game` from its own directory, so running `/tmp/port-change/main.py` uses the copy
without any environment variable.

### 5.1 Change A: swap capture and visited

In `/tmp/port-change/game.py`, the capture block is lines 160–174 and the visited block is lines
176–181. Move the visited block above the capture block. Nothing else changes.

This is the rule `construction-22` said was not a style choice. Now you can see what it costs in a
language with no compiler to enforce it.

Run all 20 and print the failures:

```bash
for n in $(seq 1 20); do
    if ! python3 /tmp/port-change/main.py --seed "$n" --name Learner \
        | cmp -s - "../../../common/traces/seed-$(printf '%04d' "$n").txt"; then
        echo "seed $n differs"
    fi
done
```

Measured on this machine:

```text
seed 2 differs
seed 3 differs
seed 7 differs
seed 8 differs
seed 9 differs
seed 10 differs
seed 13 differs
seed 14 differs
seed 15 differs
seed 17 differs
seed 19 differs
```

Eleven of 20. Compare that list with the games that end in a capture:

```bash
grep -l 'capture=' ../../../common/traces/seed-*.txt
```

The same eleven files. The explanation is narrow: the swap changes an answer only when a destination
is both the other knight's cell and already visited. In these eleven games the capture lands on a
visited cell, and from that line on the transcript diverges. The other nine games end in a trap and
never attempt a capture, so they cannot see the change.

### 5.2 Change B: flip the trap winner

Restore the copy first, then make a smaller change:

```bash
rm -rf /tmp/port-change
cp -r code /tmp/port-change
```

Line 204 of `game.py` is:

```python
winner_knight = other
```

Change it to `winner_knight = active_knight`. Run the same loop.

Measured on this machine:

```text
seed 1 differs
seed 4 differs
seed 5 differs
seed 6 differs
seed 11 differs
seed 12 differs
seed 16 differs
seed 18 differs
seed 20 differs
```

Nine of 20, exactly the games with a `no_legal_move` line. A rule can only be observed by a trace
that reaches it.

### 5.3 Restore and confirm

```bash
rm -rf /tmp/port-change
```

Then re-run the loop from Step 3.1 in the tracked directory. It is silent again: 20 of 20. Nothing
had to be reverted, because the tracked files were never touched.

### 5.4 What the change test proves, and what it does not

It proves four things.

1. The traces are a rule detector, and they have a resolution: 11 seeds see the capture order, 9 see
   the trap, 0 see the draw.
2. The port has no compile-time guard. The comparison is the only guard, and it is external.
3. A rule you cannot reach is a rule you have not tested. The draw branch is code, not evidence.
4. The port and the C++ baseline agree on every seed that exercises the two rules you just broke.

It does not prove that the port is correct for other seeds, that the C++ and Python do not share a
bug, or that either is fast. The two Python implementations are cousins, so a shared misreading of
the rules would survive both. Only the C++ trace match, or a reading of the spec, catches that.

### 5.5 A third change, if you want one

The draw limit is `DEFAULT_MAXIMUM_MOVE_COUNT = 200`. Copy `code/` again, change the constant to 199
in the copy and run the 20 seeds. Predict the result before you look: no trace reaches 200 moves, so
nothing should fail. If your prediction is "some traces fail", the surprise is the lesson — the
limit is real code and untested behavior.

## Step 6 — Compare the numbers (~10 min)

| Metric | C++ `construction-22` | Python port | Difference |
|---|---:|---:|---:|
| Files | 20 | 2 | −18 |
| Source lines | 1,071 | 245 | −826 |
| Headers | 11 | 0 | −11 |
| Translation units (console build) | 6 | 0 | −6 |
| Build steps | 1 | 0 | −1 |
| Third-party dependencies | 0 | 0 | 0 |
| Extra programs (probes and runner) | 3 | 0 | −3 |
| Golden traces | 20 of 20 | 20 of 20 | 0 |
| 20 games, separate processes | 0.026 s | 0.868 s | +0.842 s |
| 20 games, one process | not measured | 0.016 s | — |
| 20 interpreter startups | — | 0.256 s | — |
| Exit status for bad arguments | 2 | 2 | 0 |

### 6.1 What it buys

- One behavior, two languages. The rules were in the spec, not in the C++ type system.
- A reader can see the whole game in 245 lines, in one sitting, without a build.
- The port is a second reader of the spec. Where the C++ and Python disagree, one of them misread
  the rules.
- No toolchain. A Python 3.10 interpreter is the whole dependency.

### 6.2 What it costs

- The compile-time contract. No exhaustive switch, no `MoveResult`, no `-Werror`.
- The probes and the recording adapter. The port has no in-process way to ask "what would this move
  return".
- The parallel runner. `construction-21`'s concurrency does not survive.
- Startup cost per run, which dominates any many-process measurement.
- The `--max-moves` option, so the draw cannot be reached on purpose.

### 6.3 The gap: the port is evidence, not proof

Four limits are worth naming in `EVIDENCE.md` §7.

1. **The cousin problem.** `reference.py` and `game.py` share constants, function names and loop
   shape. Agreement between them is weaker than agreement between C++ and Python.
2. **The draw is unreachable.** The longest golden game is 49 moves; the cap is 200. The branch that
   sets `winner_knight = "draw"` never runs in the 20 traces.
3. **The error contract is gone.** The four outcomes exist as strings. There is no value a caller
   can inspect, so `construction-22`'s acceptance cannot be repeated in the port.
4. **The timing is a startup measurement.** 0.87 s against 0.03 s says how long twenty launches
   take, not how fast the game is.

The precise acceptance sentence is narrower than "the port works":

> The learner can run the Python port, match all 20 golden traces, change one rule in the port only,
> explain which traces fail and why, restore it, and name three things that changed and three that
> did not.

Behavior matched, rules located, representation free. Nothing about performance parity or
translation fidelity.

## Step 7 — Explain (~15 min)

Write `EVIDENCE.md` §8. Two parts.

**First, one sentence.** Say what survived the change of language and what did not, without using
the words *syntax*, *language*, *translate*, *better*, *worse*, *cleaner* or *faster*. If your
sentence needs one of those, you have named the surface rather than the contract.

**Second, one paragraph.** Answer:

- What changed: representation and ownership; the compile-time contract; the build and the process.
- What stayed: the LCG, the move order, the check order, the transcript, the 20 traces.
- What the change test showed: 11 seeds see capture order, 9 see the trap, 0 see the draw.
- What the port dropped: the probes, the recording adapter, the parallel runner, `--max-moves`.
- What the numbers mean: 245 against 1,071 lines; 0.87 s of startup against 0.03 s of processes.
- What the traces prove: two implementations agree on 20 seeds, byte for byte.
- What they do not prove: that either is right on an unseen seed, or that the draw branch works.
- What the next lesson owns: the capstone, `construction-24`.

A strong answer keeps four things apart: **what the rules are** (the spec), **what the transcript
records** (the bytes), **what the representation chose** (tuples, a set, a list), and **what the
process does** (interpreter startup, `argparse`, status 2). A weak answer says "the port is the same
program in Python", which is false at the type level and unproven at the rule level.

## Step 8 — Acceptance check and record (~5 min)

The acceptance criterion for this lesson is: **the same 20 traces from Python, and at least three
things that changed and three that did not.**

Check your record against it:

- [ ] I read `game.py`, `main.py` and `run.sh`.
- [ ] I counted 245 lines across 2 files and 1,071 across 20 for the C++ baseline.
- [ ] I found the three LCG constants and the eight move offsets in both implementations.
- [ ] I predicted the structure, behavior, timing and change-test tables before measuring.
- [ ] I reproduced all 20 traces from the port, byte for byte.
- [ ] I diffed the port against the C++ baseline on a capture seed and a trap seed.
- [ ] I ran `reference.py` and explained why it is an oracle but not an independent design.
- [ ] I measured 20 games through `run.sh`, 20 in one process, 20 C++ processes and 20 startups.
- [ ] I stated that startup dominates the Python number instead of claiming a core comparison.
- [ ] I ran the bad-argument path and saw status 2 from both implementations.
- [ ] I named three things that changed and three that did not, with evidence.
- [ ] I swapped capture and visited in a scratch copy and saw 11 seeds fail.
- [ ] I flipped the trap winner in a scratch copy and saw 9 seeds fail.
- [ ] I explained why the other seeds cannot see each change.
- [ ] I recorded that the 200-move draw is not reached by any golden trace.
- [ ] I restored the copy and confirmed 20 of 20 again.
- [ ] I did not edit any tracked code file.
- [ ] I completed `EVIDENCE.md` §1–§10 and the self-check in §12.

The criterion is not "the port is shorter". It is "the behavior survived the language change, and
you can name what did not".

## Self-check (binary)

- [ ] I read `game.py`, `main.py` and `run.sh`.
- [ ] I explained what a port is: a re-derivation of behavior, not a line-by-line translation.
- [ ] I named the four layers and which one must match exactly.
- [ ] I counted 245 lines across 2 files and compared with 1,071 across 20.
- [ ] I found the LCG constants, the move offsets and the check order in both implementations.
- [ ] I predicted the four tables before running.
- [ ] I reproduced all 20 traces, byte for byte.
- [ ] I compared the port with the C++ baseline on at least two seeds.
- [ ] I ran the oracle and explained the cousin problem.
- [ ] I measured the four timing numbers and stated what they mean.
- [ ] I ran the bad-argument path and recorded status 2.
- [ ] I named three things that changed and three that did not.
- [ ] I changed one rule in a scratch copy, observed the failing seeds, and explained them.
- [ ] I restored the copy and confirmed 20 of 20.
- [ ] I named the unreachable draw and the dropped probes.
- [ ] I completed `EVIDENCE.md`.

These are acceptance criteria. If a box is
unchecked, write the next smallest action in `EVIDENCE.md` §10.

## Misconceptions this lesson refutes

| Misconception | Why it fails | What to do instead |
|---|---|---|
| "A port is a line-by-line translation." | There is no line correspondence between `game_engine.h` and `run_game`. | Compare behavior, then rules. |
| "Fewer lines means a better program." | The C++ baseline carries probes, an adapter and a parallel runner. | Count what was dropped, not just what was removed. |
| "Python is 30 times slower." | 0.87 s is twenty interpreter launches; 20 games in one process take 0.016 s. | Separate startup from work. |
| "The port proves the rules are right." | The oracle and the port are cousins and could share a misreading. | Compare with C++, or read the spec. |
| "The four outcomes disappeared." | They survive as `illegal=outside`, `illegal=visited`, `capture=` and `legal`. | Follow the strings in the transcript. |
| "Python's type hints check the port." | Hints are not enforced at run time. | Treat them as documentation. |
| "The port has no error contract." | It has no returned contract; the loop decides inline. | Say which layer was dropped. |
| "Capture and visited can be checked in any order." | Swapping them fails 11 of 20 traces. | Keep capture before visited, as in `construction-22`. |
| "The trap rule is tested by every trace." | Only 9 traces reach a trap. | Count the `no_legal_move` lines. |
| "The 200-move draw is tested." | The longest golden game is 49 moves. | Record the gap; do not claim coverage. |
| "`argparse` returning 2 is the same as `main` returning 2." | `argparse` calls `sys.exit(2)` inside the library. | Separate the return from the jump, again. |
| "`run.sh` is a build step." | It only locates `main.py` and execs the interpreter. | Call it a launcher. |
| "The port keeps the concurrency." | `parallel_runner.cpp` was not ported. | List what the port dropped. |
| "The port is the first Python version." | `reference.py` generated the golden traces. | Call it an oracle, not a first. |
| "Matching transcripts prove equal performance." | They prove equal output. | Measure separately, and say what you measured. |

## Next step

`construction-24` is the capstone. It asks you to carry a new rule across the project, and the port
is the evidence that the contract lives in the rules rather than in the types. A rule that exists
only in one implementation is not a rule yet; it is a habit of one file.

See the [lesson catalog](../../../docs/guide/lesson-catalog.md) for the full path.

If the C++ baseline was missing, do not treat the traces alone as the second side. The traces record
the output; the baseline records the decisions that produced it.
