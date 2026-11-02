# Error contracts (errores)

> [English](lesson.md) | [Español](lesson-es.md)

**Concept:** recovery · **Time:** about 75 minutes ·
**Prerequisites:** construction-09, construction-21 · **Status:** drafted
**Reading:** ~24 minutes
**Lesson role:** optional · safe to skip

## Why this lesson exists

`construction-21` ended with a core whose randomness belonged to the engine. The move check still
handed its answer back through two out-parameters:

```cpp
MoveOutcome tryMove(const Knight *otherKnight, const MoveCandidate &candidate,
                    int &destinationRow, int &destinationColumn) const
```

The caller had to declare the two integers before the call, and the enum said nothing about them:

```cpp
int destinationRow = 0;
int destinationColumn = 0;
MoveOutcome outcome = tryMove(otherKnight, candidate, destinationRow, destinationColumn);
```

Two things travel back from one call, and only one of them is the answer. The compiler cannot tell
you whether `0` was meant as a coordinate or left there as a placeholder. `construction-22` folds
the answer into one value:

```cpp
struct MoveResult
{
    MoveOutcome outcome;
    int destinationRow;
    int destinationColumn;
};
```

Now the call returns the whole answer, and the caller cannot hold a half-filled result:

```cpp
MoveResult moveResult = tryMove(otherKnight, candidate);
```

The sentence to keep:

> An error is a value the caller can inspect. A process exit is a decision the callee made for
> everyone.

Three honest notes before the comparison.

1. The source grows by 41 lines, from 1,030 across 18 files to 1,071 across 20. Two files are new:
   `move_result.h` (16 lines) and `error_probe.cpp` (25). One file changed: `game_engine.h`, 27
   lines added and 27 removed, net zero, still 246 lines. The other seventeen are byte-identical to
   `construction-21`.
2. The object grows by 32 text bytes: `game.o` goes from 4,280 to 4,312. The linked binary follows,
   10,869 to 10,901 text bytes. Data stays at 876, BSS stays at 16, and
   `sizeof(GameEngine<8, 8>)` stays at 40. `sizeof(MoveResult)` is 12 and `sizeof(MoveOutcome)` is
   4. The probe object, `error_probe.o`, is 2,633 text bytes.
3. The transcript does not change. All 20 golden traces match byte for byte. No rule, no board, no
   generator and no adapter moved.

The comparison is not "returning a struct is better". It is narrower: one answer that used to arrive
in three pieces arrives in one, the invalid case becomes a branch the caller writes, and no path in
the core kills the process.

## What you will be able to do

By the end of this lesson you can:

- explain recovery through an error value;
- handle an invalid move without calling exit.

## The concept: three ways to say no

### The three shapes

An error can travel to the caller in three shapes, and the shape decides who is in charge.

| Shape | Who decides what happens next | What the caller must write | What can be forgotten | This baseline |
|---|---|---|---|---|
| Returned value | the caller | a branch on the value | yes, silently | yes |
| Exception | the thrower starts it, the caller may stop it | `try` / `catch` | no, it propagates | no |
| `std::expected<T, E>` | the caller | a branch on `.has_value()` | yes, silently | no |

The returned value is the cheapest shape and the easiest to ignore. An ignored `MoveOutcome` costs
nothing at compile time and produces a wrong transcript at run time. The lesson keeps that weakness
visible instead of hiding it behind a type that forces a branch.

Two honest notes on the other shapes.

- The core does not throw because nothing in a move is exceptional in the C++ sense. An invalid move
  is a normal event: the game rules expect it, the transcript records it, and the loop continues.
  Exceptions are for "this function cannot produce its result", not for "the answer is no".
- `std::expected` is C++23, and this baseline is C++20. If you compile this lesson on C++23,
  `std::expected<MoveResult, MoveError>` is the same idea with a library type: one return value that
  carries either a result or a reason. The shape is what matters here, not the header. Writing
  `MoveResult` by hand is the C++20 way to see the shape before a library names it.

### Why not `exit`

`exit` inside the core has three costs, and they are concrete in this project.

1. **It destroys the caller's choice.** A caller that wanted to report the attempt, retry with
   another candidate, or keep the game going cannot. The callee already decided.
2. **It truncates the observation.** The 20 golden traces are process-level observations. An `exit`
   in the middle of a game ends the transcript early, and the trace comparison fails on a file that
   is shorter, not on a rule that is wrong.
3. **It makes the error path untestable in-process.** `evaluate_move` returns a value, so a test can
   call it a thousand times and count the outcomes. An `exit` can only be observed by starting a
   process, or by forking one.

The precise claim in this lesson: the core never calls `exit`. The console `main` returns `2` for
bad arguments. That is still a decision, but it is the caller's decision, made after the caller saw
the problem. A nonzero return status and a process exit are not the same thing; the first one is a
value the shell reads, the second one is a control-flow jump the callee performs.

### Four outcomes, one order

`MoveOutcome` has exactly four values. There is no `unknown` and no `none`, and every call returns
one of them.

| Outcome | Enum value | Meaning | What `playTurn` does |
|---|---|---|---|
| `outsideBoard` | 0 | destination outside rows 1–8 or columns 1–8 | `illegal_attempt(..., "outside")`, next candidate |
| `visited` | 1 | destination already visited by either knight | `illegal_attempt(..., "visited")`, next candidate |
| `capture` | 2 | destination is the other knight's cell | `capture_attempt`, accept, game over |
| `legal` | 3 | inside, unvisited and empty | `legal_attempt`, accept, switch turn |

The order of the checks inside `tryMove` is a rule, not a style choice.

```cpp
if (outside the board)      -> outsideBoard
if (other knight's cell)    -> capture
if (already visited)        -> visited
otherwise                   -> legal
```

`outsideBoard` comes first because the other two checks assume in-board coordinates. `capture` comes
before `visited` because the other knight's cell is the one legal exception to the visited rule
(game-rules §7). Swap the last two and a capture on a visited cell becomes an illegal attempt, the
transcript gains an `illegal=visited` line, and traces diverge.

That ordering is the whole reason the enum is not just a boolean `wasLegal`. A boolean would
collapse `outsideBoard` and `visited` into one answer, and the transcript distinguishes them.

### The values are enum values

The probe prints:

```text
outside=0 capture=2 legal=3
```

Those are positions in the enum, not error codes chosen by hand. `static_cast<int>` in
`error_probe.cpp` is the only place the numbers appear. The numbers are an implementation detail;
the names are the contract. If someone reorders the enum, the probe output changes and the
transcript does not.

### Where the fourth value hides

The probe uses a fresh engine. A fresh `Board` starts with every cell unvisited, so `evaluate_move`
cannot return `visited` in that program. Three of the four outcomes are demonstrated by the probe.
The fourth needs a board with history: run one game, then ask about destinations.

This is a gap in the probe, not in the contract. Step 3.5 shows the measurement, and `EVIDENCE.md`
§11 is where you name it. A lesson that hides the missing case is teaching the probe, not the
contract.

## Before you start

- [`construction-09`](../construction-09-abstract-data-type/lesson.md) — the catalog's formal
  prerequisite. The contract is a small value type and a total function; you need to read
  `enum class` and an aggregate `struct` without designing a type hierarchy.
- [`construction-21`](../construction-21-concurrency/lesson.md) — the immediate baseline and the
  left side of the diff in Step 4. Its `code/` directory is the artifact this lesson starts from,
  even though the catalog lists `construction-09`.
- [`construction-10`](../construction-10-adt-invariants/lesson.md) and
  [`construction-11`](../construction-11-encapsulation/lesson.md) — optional. The `Board` still owns
  its invariant, and `was_visited` still answers `false` for an out-of-range coordinate instead of
  indexing past the array.
- A C++20 compiler (`g++` or `clang++`). `cmake` is optional; `run.sh` falls back to a direct
  compile when CMake is absent.

You do not need exceptions, `std::expected`, error monads or a logging framework. The baseline is
one enum, one struct, one `if` chain and one `return 2`.

### What you need

- The error-contract baseline:
  [`code/main.cpp`](code/main.cpp), [`code/game.h`](code/game.h),
  [`code/game.cpp`](code/game.cpp), [`code/game_engine.h`](code/game_engine.h),
  [`code/move_result.h`](code/move_result.h), [`code/error_probe.cpp`](code/error_probe.cpp),
  [`code/random_sequence.h`](code/random_sequence.h),
  [`code/random_sequence.cpp`](code/random_sequence.cpp),
  [`code/transcript_port.h`](code/transcript_port.h),
  [`code/console_transcript.h`](code/console_transcript.h),
  [`code/console_transcript.cpp`](code/console_transcript.cpp),
  [`code/recording_transcript.h`](code/recording_transcript.h),
  [`code/adapter_probe.cpp`](code/adapter_probe.cpp),
  [`code/parallel_runner.cpp`](code/parallel_runner.cpp), [`code/board.h`](code/board.h),
  [`code/move_generator.h`](code/move_generator.h), [`code/knight.h`](code/knight.h),
  [`code/knight.cpp`](code/knight.cpp), [`code/position.h`](code/position.h),
  [`code/position.cpp`](code/position.cpp).
- The previous baseline, for the diff:
  [`code/game_engine.h`](../construction-21-concurrency/code/game_engine.h), and its seventeen
  companions in [`../construction-21-concurrency/code/`](../construction-21-concurrency/code/).
- Build scripts and notes: [`run.sh`](run.sh), [`CMakeLists.txt`](CMakeLists.txt) and
  [`README.md`](README.md).
- The rules: [`common/spec/game-rules.md`](../../../common/spec/game-rules.md) — §7 fixes when a
  move is legal, §8 when a cell becomes visited, §9 the capture.
- The transcript format:
  [`common/spec/transcript-format.md`](../../../common/spec/transcript-format.md) — the
  `illegal=outside` and `illegal=visited` lines.
- One golden trace: [`common/traces/seed-0001.txt`](../../../common/traces/seed-0001.txt).
- This learner template: [`EVIDENCE.md`](EVIDENCE.md).

Check that the artifacts are present before you begin, from this lesson directory:

```bash
test -f code/main.cpp &&
test -f code/game.h &&
test -f code/game.cpp &&
test -f code/game_engine.h &&
test -f code/move_result.h &&
test -f code/error_probe.cpp &&
test -f code/random_sequence.h &&
test -f code/random_sequence.cpp &&
test -f code/transcript_port.h &&
test -f code/console_transcript.h &&
test -f code/console_transcript.cpp &&
test -f code/recording_transcript.h &&
test -f code/adapter_probe.cpp &&
test -f code/parallel_runner.cpp &&
test -f code/board.h &&
test -f code/move_generator.h &&
test -f code/knight.h &&
test -f code/knight.cpp &&
test -f code/position.h &&
test -f code/position.cpp &&
test -f ../construction-21-concurrency/code/game_engine.h &&
test -f ../../../common/traces/seed-0001.txt
```

If a file is missing, the comparison has no second side. Record the missing file in `EVIDENCE.md`
§11 and return when it is available. A count against a file you have not read is not evidence.

The 8×8 baseline reproduces all 20 traces in `common/traces/` byte for byte, before you change
anything. That is the behavioral contract, and this lesson does not touch it.

## Step 1 — Inventory the contract (~10 min)

Start with the type. The contract is two declarations in one header:

```bash
grep -n 'enum class\|struct\|MoveOutcome\|destinationRow' code/move_result.h
```

```text
3:enum class MoveOutcome
4:{
5:    outsideBoard,
6:    visited,
7:    capture,
8:    legal
9:};
11:struct MoveResult
12:{
13:    MoveOutcome outcome;
14:    int destinationRow;
15:    int destinationColumn;
16:};
```

Now the function that produces it:

```bash
grep -n 'evaluate_move\|MoveResult tryMove\|tryMove(' code/game_engine.h
```

```text
67:    MoveResult evaluate_move(const Knight &otherKnight, const MoveCandidate &candidate) const
69:        return tryMove(&otherKnight, candidate);
139:    MoveResult tryMove(const Knight *otherKnight, const MoveCandidate &candidate) const
187:            MoveResult moveResult = tryMove(otherKnight, candidate);
```

Four lines. `evaluate_move` is the public entry point, `tryMove` is the private implementation, and
`playTurn` is the one production caller. The probe is the second caller.

Now the process exits. This is the claim the lesson makes, so it gets a command:

```bash
grep -rn '\bexit\b' code/
```

No output. The core, the adapters, the probes and the runner never call `exit`. The only nonzero
returns are argument errors:

```bash
grep -n 'return 2' code/main.cpp code/parallel_runner.cpp
```

```text
code/main.cpp:74:        return 2;
code/parallel_runner.cpp:181:        return 2;
code/parallel_runner.cpp:207:    return 2;
```

Counts for this baseline. The counting rule is stated so you can reproduce it:

- Files: 20.
- Headers: 11 (`board.h`, `console_transcript.h`, `game.h`, `game_engine.h`, `knight.h`,
  `move_generator.h`, `move_result.h`, `position.h`, `random_sequence.h`, `recording_transcript.h`,
  `transcript_port.h`).
- Translation units in the console build: 6 (`main.cpp`, `game.cpp`, `console_transcript.cpp`,
  `knight.cpp`, `position.cpp`, `random_sequence.cpp`).
- Translation units in the probe build: 4 (`error_probe.cpp`, `knight.cpp`, `position.cpp`,
  `random_sequence.cpp`).
- Complete class definitions in the core: 9 (the same nine as `construction-21`).
- Global mutable variables in the core: 0.
- Values in `MoveOutcome`: 4.
- Members in `MoveResult`: 3.
- Source lines: 1,071.
- Files byte-identical to `construction-21`: 17 of 18.
- Files changed: 1 (`game_engine.h`).
- Files new: 2 (`move_result.h`, `error_probe.cpp`).

Then answer in `EVIDENCE.md` §2:

1. Which file declares `MoveOutcome` and `MoveResult`?
2. How many values does `MoveOutcome` have, and what are their names and numbers?
3. What does `tryMove` return, and what did it return in `construction-21`?
4. Which public method exposes the contract without running a game?
5. Which function is the only production caller of `tryMove`?
6. How many branches does `playTurn` take on the outcome, and what does each do?
7. Does any file in `code/` call `exit`? Which command shows it?
8. What does `main` return for bad arguments, and why is that not an `exit`?

Question 8 has a precise answer: it returns `2`, the process ends because `main` returned, and the
caller of `main` — the shell — receives the status. The program did not jump out of the middle of a
function; it finished.

## Step 2 — Predict before you run (~10 min)

Write these predictions before running anything. Do not look at the answers in Step 3 first.

### 2.1 The structure table

| Question | My prediction |
|---|---|
| Files in `construction-21` | |
| Files here | |
| Headers here | |
| Translation units in the console build | |
| Source lines in `construction-21` | |
| Source lines here | |
| Files changed | |
| Files new | |
| Files byte-identical | |
| Values in `MoveOutcome` | |
| Members in `MoveResult` | |

The row that is easy to get wrong is "files changed". The engine looks different, but the port, the
adapters, the board, the knights and the runner are untouched. One header moved a type out of
itself; that is the whole diff outside the two new files.

### 2.2 The probe table

| Question | My prediction |
|---|---|
| `error_probe` output line | |
| Exit status of `error_probe` | |
| Which outcome is missing from the probe, and why | |
| Exit status of `main` with a missing `--name` | |
| Exit status of `main` with a valid game | |
| Seeds that match, of 20 | |

Predict the three numbers as enum positions, not as error codes. Predict the missing outcome as
`visited`, because a fresh board has no visited cell.

### 2.3 The size table

| Question | My prediction |
|---|---|
| `sizeof(MoveOutcome)` | |
| `sizeof(MoveResult)` | |
| `sizeof(GameEngine<8, 8>)`, before and after | |
| `game.o` text bytes, before and after | |
| Binary text bytes, before and after | |
| Binary data bytes, before and after | |
| Binary BSS bytes, before and after | |
| `error_probe.o` text bytes | |

Predict `MoveResult` at 12: a 4-byte enum and two 4-byte integers, with no padding to add. Predict
`game.o` to grow, because every call now builds and returns a 12-byte value instead of writing
through two references. Predict data and BSS flat: no new global, no new static.

### 2.4 The decision table

| Question | My prediction |
|---|---|
| Who decides to continue after an invalid move? | |
| Who decides to report it? | |
| Who decides to stop? | |
| What the core knows about the transcript | |
| What the core knows about the process | |

Predict the caller for all three. If your answer names `tryMove` for any row, the contract has not
moved yet.

## Step 3 — Run the baseline (~15 min)

### 3.1 The traces

First the traces, unmodified:

```bash
mkdir -p /tmp/error-test
for trace in ../../../common/traces/seed-*.txt; do
    traceName="$(basename "${trace}" .txt)"
    seedText="${traceName#seed-}"
    seedNumber="$((10#${seedText}))"
    ./run.sh --seed "${seedNumber}" --name Learner | cmp - "${trace}"
done
```

A silent loop is 20 of 20. If `run.sh` cannot build, compile once by hand:

```bash
c++ -std=c++20 -Wall -Wextra -Werror -O2 -Icode \
    -o /tmp/error-test/error-baseline \
    code/main.cpp code/game.cpp code/console_transcript.cpp \
    code/knight.cpp code/position.cpp code/random_sequence.cpp
```

Then run the same loop with `/tmp/error-test/error-baseline` instead of `./run.sh`.

### 3.2 The error probe

```bash
c++ -std=c++20 -Wall -Wextra -Werror -O2 -Icode -o /tmp/error-test/error-probe \
    code/error_probe.cpp code/knight.cpp code/position.cpp code/random_sequence.cpp
/tmp/error-test/error-probe
echo "exit=$?"
```

The output on this machine:

```text
outside=0 capture=2 legal=3
exit=0
```

Three outcomes, three numbers, one normal exit. The probe is a complete caller: it constructs a
`RecordingTranscript`, an engine and an `otherKnight`, asks three questions, and prints the answers.
It never starts a game.

Read the three candidates in `error_probe.cpp` against the table:

| Candidate | Destination | Outcome | Why |
|---|---|---|---|
| `{1, 0, 0}` | row 0, column 0 | `outsideBoard` (0) | row 0 is outside 1–8 |
| `{1, 1, 1}` | row 1, column 1 | `capture` (2) | the other knight stands there |
| `{1, 2, 3}` | row 2, column 3 | `legal` (3) | inside, empty, unvisited |

The `moveNumber` field is `1` in all three because `evaluate_move` does not read it. It only reads
the destination. That is worth noticing: `MoveCandidate` carries a number for the transcript, and
the contract ignores it. A smaller type would carry only the destination, but the baseline reuses
the candidate the game loop already has.

### 3.3 The argument path

```bash
/tmp/error-test/error-baseline --seed 1
echo "exit=$?"
```

```text
usage: /tmp/error-test/error-baseline --seed INTEGER --name TEXT
exit=2
```

The message goes to standard error, the transcript is empty, and the status is `2`. Compare with a
valid game:

```bash
/tmp/error-test/error-baseline --seed 1 --name Learner > /dev/null
echo "exit=$?"
```

```text
exit=0
```

Two statuses, both produced by `return` from `main`. No `exit` call appears in either path.

### 3.4 Sizes

The probe goes in the scratch directory, never in the tracked files:

```bash
cat > /tmp/error-test/sizes.cpp <<'EOF'
#include "game_engine.h"
#include "move_result.h"
#include "recording_transcript.h"

#include <cstdio>

int main()
{
    std::printf("sizeof(MoveOutcome)=%zu\n", sizeof(MoveOutcome));
    std::printf("sizeof(MoveResult)=%zu\n", sizeof(MoveResult));
    std::printf("sizeof(GameEngine<8,8>)=%zu\n", sizeof(GameEngine<8, 8>));
    return 0;
}
EOF
c++ -std=c++20 -Wall -Wextra -Werror -O2 -Icode \
    -o /tmp/error-test/sizes /tmp/error-test/sizes.cpp \
    code/knight.cpp code/position.cpp code/random_sequence.cpp
/tmp/error-test/sizes
```

The output on this platform:

```text
sizeof(MoveOutcome)=4
sizeof(MoveResult)=12
sizeof(GameEngine<8,8>)=40
```

`MoveResult` is 12 bytes: the enum is 4, each integer is 4, and the total has no padding. The engine
is still 40, exactly as in `construction-21`. The contract is a value passed by return; it does not
become a member.

### 3.5 Optional: the fourth outcome

The probe shows three outcomes. To see `visited`, give the board a history:

```bash
cat > /tmp/error-test/visited_probe.cpp <<'EOF'
#include "game_engine.h"
#include "recording_transcript.h"

#include <cstdio>

int main()
{
    RecordingTranscript transcript;
    GameEngine<8, 8> engine(transcript);
    engine.run(1, "Learner");

    Knight otherKnight("other");
    otherKnight.set_position(1, 1);

    int counts[4] = {0, 0, 0, 0};
    for (int row = 0; row <= 9; row += 1)
    {
        for (int column = 0; column <= 9; column += 1)
        {
            MoveResult result =
                engine.evaluate_move(otherKnight, MoveCandidate{1, row, column});
            counts[static_cast<int>(result.outcome)] += 1;
        }
    }
    std::printf("outside=%d visited=%d capture=%d legal=%d\n", counts[0], counts[1], counts[2],
                counts[3]);
    return 0;
}
EOF
c++ -std=c++20 -Wall -Wextra -Werror -O2 -Icode \
    -o /tmp/error-test/visited-probe /tmp/error-test/visited_probe.cpp \
    code/knight.cpp code/position.cpp code/random_sequence.cpp
/tmp/error-test/visited-probe
```

The output on this machine:

```text
outside=36 visited=21 capture=1 legal=42
```

100 destinations: 36 outside the 8×8 board (the scan runs 0–9), 21 visited by the game, 1 occupied
by the other knight, 42 legal. The exact counts depend on the seed; the point is that all four
outcomes appear once the board has history.

This program calls `evaluate_move` on an engine that already ran a game. That is a legitimate use of
a public method, but it is also a warning: `evaluate_move` reads the engine's current board and the
other knight you pass. It is a query, not a pure function of its two arguments. The lesson names
that coupling in §6.3.

### 3.6 Object and binary

```bash
c++ -std=c++20 -O2 -Icode -c code/game.cpp -o /tmp/error-test/game22.o
c++ -std=c++20 -O2 -I../construction-21-concurrency/code \
    -c ../construction-21-concurrency/code/game.cpp -o /tmp/error-test/game21.o
size /tmp/error-test/game21.o /tmp/error-test/game22.o
c++ -std=c++20 -O2 -Icode -c code/error_probe.cpp -o /tmp/error-test/error_probe.o
size /tmp/error-test/error_probe.o
```

```text
   text    data     bss     dec     hex filename
   4280       0       0    4280    10b8 /tmp/error-test/game21.o
   4312       0       0    4312    10d8 /tmp/error-test/game22.o
   text    data     bss     dec     hex filename
   2633       0       0    2633     a49 /tmp/error-test/error_probe.o
```

Now the linked binaries:

```bash
c++ -std=c++20 -O2 -Icode -o /tmp/error-test/bin22 \
    code/main.cpp code/game.cpp code/console_transcript.cpp \
    code/knight.cpp code/position.cpp code/random_sequence.cpp
c++ -std=c++20 -O2 -I../construction-21-concurrency/code \
    -o /tmp/error-test/bin21 \
    ../construction-21-concurrency/code/main.cpp \
    ../construction-21-concurrency/code/game.cpp \
    ../construction-21-concurrency/code/console_transcript.cpp \
    ../construction-21-concurrency/code/knight.cpp \
    ../construction-21-concurrency/code/position.cpp \
    ../construction-21-concurrency/code/random_sequence.cpp
size /tmp/error-test/bin21 /tmp/error-test/bin22
```

```text
   text    data     bss     dec     hex filename
  10869     876      16   11761    2df1 /tmp/error-test/bin21
  10901     876      16   11793    2e11 /tmp/error-test/bin22
```

Text grows by 32 bytes in the object and 32 in the binary. Data and BSS do not move. The contract is
a return value, not a table and not a global.

If the first trace diff is not empty, stop. That is a different baseline from the one this lesson
describes; record it in `EVIDENCE.md` §11 and do not adjust the prediction to match.

## Step 4 — Compare: what the value carries (~10 min)

Open `move_result.h`, `game_engine.h`, `game.cpp` and `main.cpp`. Five facts, read against the
measurements.

1. **One answer, one value.** `tryMove` returns `MoveResult`. The two out-parameters are gone from
   the signature, and the caller no longer declares two integers before the call.
2. **The destination is always filled.** `tryMove` initializes the struct with the candidate's
   destination on the first line, then overwrites only the outcome. There is no path that returns a
   result with an uninitialized coordinate, including the `outsideBoard` path, where the coordinate
   is the one that was rejected.
3. **The outcome is total.** Every call returns one of four values. `evaluate_move` can be called
   from a probe with no game, and the compiler still requires a branch for every outcome in a
   `switch` without `default`.
4. **The caller decides.** `playTurn` switches on `moveResult.outcome`: two outcomes continue to the
   next candidate and report, one accepts a capture, one accepts a legal move. No branch calls
   `exit`.
5. **The process status is separate.** `main` returns `2` for bad arguments and `run_game`'s value
   for a game. Both are returns. The status the shell sees is a value that crossed the process
   boundary, not a jump out of the middle of the core.

### 4.1 The exact diff

Two files are new and one changed. The diff against `construction-21`:

```bash
diff -rq ../construction-21-concurrency/code code
diff -u ../construction-21-concurrency/code/game_engine.h code/game_engine.h
```

| File | Added | Removed | Net | What changed |
|---|---:|---:|---:|---|
| `game_engine.h` | 27 | 27 | 0 | `MoveOutcome` moves out; `tryMove` returns `MoveResult`; `evaluate_move` appears |
| `move_result.h` | 16 | 0 | +16 | New header: the enum and the struct |
| `error_probe.cpp` | 25 | 0 | +25 | New program: three calls, three numbers |
| Total | 68 | 27 | +41 | One changed, two new, seventeen identical |

The seventeen identical files are `board.h`, `move_generator.h`, `knight.h`, `knight.cpp`,
`position.h`, `position.cpp`, `transcript_port.h`, `console_transcript.h`,
`console_transcript.cpp`, `recording_transcript.h`, `game.h`, `main.cpp`, `game.cpp`,
`adapter_probe.cpp`, `parallel_runner.cpp`, `random_sequence.h` and `random_sequence.cpp`.

Note what did **not** change in `game_engine.h`: `playTurn` still has the same four branches and the
same two stop conditions. Only the way it receives the answer changed. The diff is mostly
mechanical, and that is the point — a contract change that rewrites the logic would be a different,
larger lesson.

### 4.2 What did not change

- The rules. Move order, capture, trap, draw and the 200-move limit are the same.
- The transcript. All 20 traces match, byte for byte, including the `illegal=outside` and
  `illegal=visited` lines.
- The board, the move generator, the knight, the position and the random sequence.
- The port and both adapters.
- The public game interface: `game.h` and `run_game` keep their signatures.
- The engine's size: 40 bytes, the same as `construction-21`.

### 4.3 What it costs

- 41 new lines and two new files, one of them a program.
- 32 more text bytes in `game.o` and 32 more in the linked binary.
- One more header in the include graph. `game_engine.h` includes `move_result.h`; every translation
  unit that includes the engine now sees the contract.
- A new public method, `evaluate_move`, which is a second entry point into `tryMove`. Two entry
  points mean two places to keep honest.
- A naming burden: `MoveResult` is not a `Move`, and `outcome` is not a `status`. The type says what
  it is, and the reader still has to learn the four names.

None of these is a defect. They are the price of turning a hidden protocol into a visible one, and
the record should list them next to the benefit.

### 4.4 The contract table

This table is the heart of the comparison. Read it against the probe.

| Question | `construction-21` | `construction-22` |
|---|---|---|
| What `tryMove` returns | `MoveOutcome`, plus two out-parameters | one `MoveResult` |
| Who initializes the destination | the caller, before the call | `tryMove`, on its first line |
| Can the caller forget the destination | yes, `0` compiles | no, the value carries it |
| Can the caller ignore the outcome | yes | yes |
| Number of return paths | 4, each returning an enum | 4, each returning a struct |
| Who decides to continue | `playTurn` | `playTurn`, from a value |
| Who decides to stop the process | nobody in the core | nobody in the core |
| How a probe sees the contract | not available; `tryMove` is private | `evaluate_move` |
| Size of the answer | 4 bytes plus two caller integers | 12 bytes |

The last row is the honest trade: the answer is bigger, and it is complete.

## Step 5 — Change test: let the caller decide (~15 min)

The acceptance names the change test: **call `evaluate_move` with an invalid move, a capture and a
legal move; record the outcome; make the caller decide whether to continue, report or stop; show
that no path calls `exit` and that the 20 traces still pass.**

You have already seen the three outcomes in Step 3.2. Now write the policy.

### 5.1 Make a scratch copy

Never edit the tracked `code/` directory. Copy it:

```bash
rm -rf /tmp/error-test/change
cp -r code /tmp/error-test/change
```

### 5.2 The caller's policy

Create `/tmp/error-test/change/recovery_probe.cpp`:

```cpp
#include "game_engine.h"
#include "recording_transcript.h"

#include <cstdio>
#include <cstring>

enum class Recovery
{
    continueGame,
    reportAndContinue,
    stop
};

Recovery decide(MoveOutcome outcome, bool stopOnInvalid)
{
    switch (outcome)
    {
    case MoveOutcome::legal:
    case MoveOutcome::capture:
        return Recovery::continueGame;
    case MoveOutcome::outsideBoard:
    case MoveOutcome::visited:
        return stopOnInvalid ? Recovery::stop : Recovery::reportAndContinue;
    }
    return Recovery::stop;
}

int main(int argumentCount, char **argumentValues)
{
    bool stopOnInvalid = argumentCount == 2 &&
                         std::strcmp(argumentValues[1], "--stop-on-invalid") == 0;

    RecordingTranscript transcript;
    GameEngine<8, 8> engine(transcript);
    Knight otherKnight("other");
    otherKnight.set_position(1, 1);

    const MoveCandidate candidates[3] = {
        {1, 0, 0},
        {1, 1, 1},
        {1, 2, 3},
    };

    for (const MoveCandidate &candidate : candidates)
    {
        MoveResult result = engine.evaluate_move(otherKnight, candidate);
        Recovery recovery = decide(result.outcome, stopOnInvalid);
        std::printf("try=%d to=%d,%d outcome=%d recovery=%d\n", candidate.moveNumber,
                    result.destinationRow, result.destinationColumn,
                    static_cast<int>(result.outcome), static_cast<int>(recovery));
        if (recovery == Recovery::stop)
        {
            return 0;
        }
    }
    return 0;
}
```

Three things to read in that file.

1. `decide` is a total function over `MoveOutcome`. There is no `default` label, so adding a fifth
   enum value is a compile error until the policy handles it. That is the compiler enforcing the
   contract.
2. The policy is a parameter, not a constant. `--stop-on-invalid` changes the same three outcomes
   into a different recovery without touching `tryMove`.
3. `return 0` is how the program stops. It is a normal return from `main`, not an `exit`.

### 5.3 Run it

```bash
c++ -std=c++20 -Wall -Wextra -Werror -O2 -I/tmp/error-test/change \
    -o /tmp/error-test/recovery-probe \
    /tmp/error-test/change/recovery_probe.cpp \
    /tmp/error-test/change/knight.cpp \
    /tmp/error-test/change/position.cpp \
    /tmp/error-test/change/random_sequence.cpp
/tmp/error-test/recovery-probe
echo "exit=$?"
/tmp/error-test/recovery-probe --stop-on-invalid
echo "exit=$?"
```

The output on this machine:

```text
try=1 to=0,0 outcome=0 recovery=1
try=1 to=1,1 outcome=2 recovery=0
try=1 to=2,3 outcome=3 recovery=0
exit=0
try=1 to=0,0 outcome=0 recovery=2
exit=0
```

The first run reports the invalid move and continues to the capture and the legal move. The second
run stops after the first invalid move, with status 0. Both are the caller's decision, taken from
the returned value. Neither is a process exit inside the core.

Record the mapping you used: `continueGame=0`, `reportAndContinue=1`, `stop=2`. If you reorder the
`Recovery` enum, the numbers change and the lesson does not.

### 5.4 The traces

The change test touched one scratch file. The console build is unchanged, so the traces must still
pass:

```bash
for trace in ../../../common/traces/seed-*.txt; do
    traceName="$(basename "${trace}" .txt)"
    seedText="${traceName#seed-}"
    seedNumber="$((10#${seedText}))"
    ./run.sh --seed "${seedNumber}" --name Learner | cmp - "${trace}"
done
```

The loop is silent: 20 of 20. A contract change that altered the transcript would have changed the
game, and this one does not.

### 5.5 What the change test proves, and what it does not

It proves five things.

1. The invalid move is observable as a value. Three calls, three outcomes, one of them invalid.
2. The caller owns the recovery. The same three outcomes produce report-and-continue or stop,
   depending on a caller parameter.
3. The core does not need to know the policy. `tryMove` and `playTurn` are untouched by the change.
4. Stopping is a normal return. Both runs end with status 0, and no `exit` appears anywhere.
5. The transcript is independent of the policy. The 20 traces pass because the console build never
   saw the probe.

It does not prove that an invalid move is impossible, that the caller always checks the value, or
that a policy is correct. It does not prove that `visited` can be produced from a fresh engine; it
cannot, and Step 3.5 is the workaround. It does not prove that the enum order is stable across
versions; the names are the contract, the numbers are not.

## Step 6 — Compare with `construction-21` (~5 min)

| Metric | `construction-21` | `construction-22` | Difference |
|---|---:|---:|---:|
| Files | 18 | 20 | +2 |
| Headers | 10 | 11 | +1 |
| Translation units (console build) | 6 | 6 | 0 |
| Source lines | 1,030 | 1,071 | +41 |
| Changed files | — | 1 | +1 |
| New files | — | 2 | +2 |
| Files byte-identical | — | 17 of 18 | — |
| Values in `MoveOutcome` | 4 | 4 | 0 |
| Members in `MoveResult` | — | 3 | +3 |
| `sizeof(MoveResult)` | — | 12 | +12 |
| `sizeof(GameEngine<8, 8>)` | 40 | 40 | 0 |
| `game.o` text | 4,280 | 4,312 | +32 |
| Binary text | 10,869 | 10,901 | +32 |
| Binary data | 876 | 876 | 0 |
| Binary BSS | 16 | 16 | 0 |
| Traces | 20 of 20 | 20 of 20 | 0 |
| `exit` calls in `code/` | 0 | 0 | 0 |

### 6.1 What it buys

- One return value carries the whole answer. The caller cannot hold a destination without an
  outcome, because they arrive together.
- The invalid case is a branch the caller writes, not a jump the callee performs.
- The contract is testable without a game: `evaluate_move` is public, `const`, and returns a value.
- The outcome is exhaustive. A `switch` without `default` makes the compiler ask about every value.
- The transcript format's two illegal reasons map one-to-one onto two enum values, so the adapter
  receives the reason instead of recomputing it.
- The core has no `exit`, so a caller can run a game inside a larger program and keep the process
  alive.
- The 20 traces still pass, so the console behavior is unchanged.

### 6.2 What it costs

- 41 new lines, two new files and one more header in the include graph.
- 32 more text bytes in `game.o` and in the binary. The answer is 12 bytes instead of 4.
- A second public entry point (`evaluate_move`) into a private function. Two entry points mean two
  contracts to keep in sync.
- The enum's numeric values are now visible through a probe. Reordering the enum is a breaking
  change for the probe and not for the transcript; the lesson has to say which one is the contract.
- The caller can still ignore the result. A returned value is not a checked value, and the type
  system does not force the branch.

### 6.3 The gap: a value is not a policy

The baseline is not "the program handles errors". It is "the engine returns an answer, and the
caller decides". Five couplings remain, and they are worth naming.

1. **`evaluate_move` reads the engine's current state.** It is `const`, but it is a query over the
   board and the other knight, not a pure function of its two arguments. On a fresh engine it cannot
   return `visited`; after a game it can. The probe demonstrates three of four outcomes for that
   reason.
2. **The caller can ignore the outcome.** Nothing in `MoveResult` forces a branch. A future caller
   that drops the value compiles and produces a wrong transcript. `[[nodiscard]]` on `tryMove` and
   `evaluate_move` would make the compiler warn; the baseline does not use it, and that is a
   deliberate omission you can measure.
3. **The policy is outside the baseline.** `playTurn` is one policy: report and try the next
   candidate. The change test writes a second one. A real program would need a rule for what happens
   after `stop`, and this lesson does not define one.
4. **The enum numbers are not stable.** `outside=0 capture=2 legal=3` is the current layout. The
   names are the contract; the numbers are a debugging convenience.
5. **`exit` is absent from `code/`, not from the process.** A library can still call `std::abort` on
   an assertion, and the operating system can still kill the process. The claim is narrow: this code
   does not decide to end the process on an invalid move.

The precise acceptance sentence is narrower than "the program handles errors":

> The learner can handle an invalid move through a returned value, let the caller choose the
> recovery, and show that the process continues and the transcript is unchanged.

Value returned, caller decides, process continues, traces unchanged. Nothing about exception safety,
error hierarchies, or logging.

## Step 7 — Explain (~5 min)

Write `EVIDENCE.md` §8. Two parts.

**First, one sentence.** Say what changed between the two baselines and what the change decides,
without using the words *struct*, *enum*, *return value*, *out-parameter*, *exception*, *expected*,
*better*, *worse*, *clean* or *decoupled*. If your sentence needs one of those, you have named the
mechanism rather than the decision.

**Second, one paragraph.** Answer:

- What changed: `tryMove` returns one `MoveResult` instead of an outcome plus two out-parameters;
  `MoveOutcome` moved into its own header; `evaluate_move` exposes the contract publicly.
- What stayed: the four outcomes, their order, the transcript, the board, the loop and the 20
  traces.
- What the probe adds: three calls with no game, and three numbers that are enum positions.
- What the probe misses: `visited`, because a fresh board has no visited cell.
- What the caller decides: continue, report, or stop; `playTurn` reports and continues, the change
  test writes a second policy.
- What the process does: returns from `main`; no branch in `code/` calls `exit`.
- What the numbers prove: `sizeof(MoveResult)` 12, `game.o` 4,280 to 4,312, binary 10,869 to 10,901
  text, data and BSS flat, traces 20 of 20.
- What the next lesson owns: the port to another language, `construction-23`.

A strong answer keeps four things apart: **what the engine returns** (one value), **what the caller
decides** (continue, report, stop), **what the transcript records** (the same bytes), and **what the
process does** (returns a status, never exits from the core). A weak answer says "the code handles
errors now", which is false: the code returns an answer, and a caller may still ignore it.

## Step 8 — Acceptance check and record (~5 min)

The acceptance criterion for this lesson is: **invalid move handled without `exit`; 20 traces
pass.**

Check your record against it:

- [ ] I read `move_result.h`, `game_engine.h`, `error_probe.cpp` and `main.cpp`.
- [ ] I named the four outcomes and their order inside `tryMove`.
- [ ] I explained why `capture` is checked before `visited`.
- [ ] I counted 20 files, 11 headers and 1,071 source lines.
- [ ] I named the one changed file and the two new files.
- [ ] I predicted the structure, probe, size and decision tables before measuring.
- [ ] I reproduced all 20 traces on the unmodified console baseline.
- [ ] I ran `error_probe` and saw `outside=0 capture=2 legal=3` with status 0.
- [ ] I ran `main` with a bad argument and saw status 2.
- [ ] I measured `sizeof(MoveOutcome)` at 4 and `sizeof(MoveResult)` at 12.
- [ ] I measured `game.o` text at 4,312 and the binary at 10,901 text, 876 data and 16 BSS.
- [ ] I measured `error_probe.o` at 2,633 text bytes.
- [ ] I saw the fourth outcome with a board that has history, or I recorded why not.
- [ ] I wrote a caller policy in a scratch copy and ran it in two modes.
- [ ] Both modes ended with status 0 and no `exit`.
- [ ] All 20 traces matched after the change test.
- [ ] The scratch build passed with `-Wall -Wextra -Werror`.
- [ ] I named what the value does not guarantee: the caller can ignore it.
- [ ] I did not edit any tracked code file.
- [ ] I completed `EVIDENCE.md` §1–§10 and the self-check in §12.

The criterion is not "the program handles errors". It is "you can show the invalid case as a value,
choose the recovery at the call site, and keep the process and the transcript unchanged".

## Self-check (binary)

- [ ] I read `move_result.h`, `game_engine.h`, `error_probe.cpp` and `main.cpp`.
- [ ] I explained the change from an enum plus two out-parameters to one `MoveResult`.
- [ ] I named the four outcomes, their enum values and their order.
- [ ] I counted 20 files, 11 headers and 1,071 source lines.
- [ ] I compared the baseline with `construction-21` and named the changed and new files.
- [ ] I predicted the four tables before running.
- [ ] I reproduced all 20 traces on the unmodified baseline.
- [ ] I ran the error probe and recorded the three outcomes and the exit status.
- [ ] I ran the argument path and recorded status 2.
- [ ] I measured `sizeof(MoveOutcome)`, `sizeof(MoveResult)`, `game.o`, the binary and the probe
      object.
- [ ] I wrote a caller policy in a scratch copy and showed continue, report and stop.
- [ ] No path in `code/` calls `exit`.
- [ ] All 20 traces matched after the change test.
- [ ] I completed `EVIDENCE.md`.

These are acceptance criteria. If a box is
unchecked, write the next smallest action in `EVIDENCE.md` §10.

## Misconceptions this lesson refutes

| Misconception | Why it fails | What to do instead |
|---|---|---|
| "Returning a struct is object-oriented." | It is a value type and a total function; there is no hierarchy and no dispatch. | Say what the value carries. |
| "`MoveResult` replaces exceptions." | The baseline never threw; it returned an enum and two out-parameters. | Compare the two return shapes. |
| "The probe tests the whole contract." | A fresh board cannot produce `visited`. | Run a game first, or record the gap. |
| "`evaluate_move` is pure." | It reads the board and the other knight. | Call it a query over current state. |
| "The caller must check the value." | Nothing forces a branch; the value can be dropped. | Add `[[nodiscard]]` if you want the warning. |
| "`return 2` from `main` is an `exit`." | It is a normal return; the shell reads the status. | Separate the return from the jump. |
| "An invalid move is an error in the C++ sense." | The rules expect it; the transcript records it. | Treat it as a normal outcome. |
| "The enum numbers are the API." | The names are the contract; the numbers are the current layout. | Reorder nothing; compare names. |
| "`capture` and `visited` can be checked in any order." | The other knight's cell is the legal exception to the visited rule. | Keep capture before visited. |
| "`outsideBoard` can be checked last." | The other checks assume in-board coordinates. | Check the bounds first. |
| "The destination is optional." | `MoveResult` always carries it, including on the invalid path. | Read the rejected coordinate. |
| "A bigger answer costs nothing." | `game.o` grows by 32 text bytes and the value is 12 bytes. | Measure it. |
| "The policy belongs in the engine." | `playTurn` is one policy; the change test is another. | Keep the policy at the call site. |
| "The 20 traces test the error contract." | They test the transcript; the probe tests the contract. | Run both. |
| "`std::expected` is available in C++20." | It is C++23. | Write the value by hand, or compile with C++23 and say so. |

## Next step

`construction-23` owns the port to another language. The contract from this lesson is the part that
must survive translation: four named outcomes, one value that carries the destination, and a caller
that decides. A port that reproduces the 20 traces but calls `sys.exit` in the middle of the loop
has moved the syntax and lost the decision.

See the [lesson catalog](../../../docs/guide/lesson-catalog.md) for the full path.

If either baseline was missing, do not start `construction-23`. The port needs both sides: the
behavior and the contract.
