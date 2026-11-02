# Concurrency (concurrencia)

> [English](lesson.md) | [Español](lesson-es.md)

**Concept:** shared state vs messages · **Time:** about 115 minutes ·
**Prerequisites:** construction-12, construction-20 · **Status:** drafted
**Reading:** ~28 minutes
**Lesson role:** optional · safe to skip

## Why this lesson exists

`construction-20` ended with a core that could run without a console. It could not run two games at
once. The random sequence was a file-static variable:

```cpp
namespace
{
std::uint64_t randomState = 0;
}
```

Every `GameEngine` in the process advanced the same `randomState`. Two threads, two games, one
variable. The console transcript was correct only because the console program ran one game at a
time.

`construction-21` moves that state into the engine:

```cpp
class RandomSequence
{
public:
    void set_seed(std::int64_t seed);
    std::uint64_t below(std::uint64_t bound);
    int integer(int minimum, int maximum);

private:
    std::uint64_t state = 0;
};
```

The sequence is unchanged. The ownership is not. Each `GameEngine` owns one `RandomSequence`, so
two engines cannot touch each other's state. The console transcript is byte-identical.

The sentence to keep:

> A game is deterministic when the state it mutates belongs to it. Running many games at once does
> not create determinism; it shows who owns the state.

Three honest notes before the comparison.

1. The source grows by 215 lines, from 815 across seventeen files to 1,030 across eighteen. Three
   files changed, one is new, and fourteen are byte-identical to `construction-20`. The new file is
   `parallel_runner.cpp`, 208 lines; the other seven lines are the class declaration and the call
   sites that now go through `randomSequence`.
2. The core object grows: `game.o` text goes from 4,138 to 4,280 bytes, and
   `sizeof(GameEngine<8, 8>)` goes from 32 to 40. The eight new bytes are the `RandomSequence`
   member. The binary grows from 10,748 to 10,869 text bytes, data stays at 876, and BSS *shrinks*
   from 24 to 16 because the global RNG disappears.
3. The transcript does not change. All 20 golden traces match byte for byte. The board, the move
   generator, the knight, the position, the port and both adapters are the same files, unchanged.

The comparison is not "concurrency is better". It is narrower: one piece of state changes owner, a
second program runs many games at once, and a deliberately broken result collector shows the
difference between owning state and sharing it.

## What you will be able to do

By the end of this lesson you can:

- explain shared state versus owned state;
- detect and remove a race while keeping deterministic traces.

## The concept: who owns the state

### Two shared states, not one

This lesson contains two different shared-state problems, and mixing them is the main source of
confusion.

1. **The game's random state.** In `construction-20` it lived in one file-static `randomState`.
   Every game in the process used it. `construction-21` moves it into a `RandomSequence` member of
   `GameEngine`. This is the ownership fix, and it is already applied in the baseline you read.
2. **The runner's result state.** `parallel_runner.cpp` starts 8 threads, each running 2,000 games.
   The threads must put 16,000 results somewhere. The `race` mode puts them in one shared
   `std::vector` through one shared `nextIndex`, with no synchronization. That is the failure the
   change test asks you to reproduce and remove.

The first problem is solved in the artifact. The second is the exercise. The runner is a second
program, not part of the console build.

### Ownership removes the first problem

`RandomSequence` has one private `std::uint64_t state`. `GameEngine` has one `RandomSequence`
member. The engine is constructed per game, so the sequence is per game. Two threads cannot write
the same integer because the integers are different objects.

This is why there is no `std::mutex` in the engine. The cheapest synchronization is the one you do
not need: if no two threads can reach the same object, there is nothing to lock.

The LCG is unchanged:

```cpp
state = state * randomMultiplier + randomIncrement;
return state % bound;
```

Same constants, same update, same modulo. The 20 traces are the evidence that the sequence still
produces the same bytes. The ownership change is invisible to the console program, which is exactly
what a correct refactor should be.

### The runner exposes the second problem

`run_race` pre-sizes the result vector and hands every thread a reference to the same `nextIndex`:

```cpp
int resultIndex = nextIndex;
nextIndex += 1;
results[resultIndex] = result;
```

That is three operations on two shared objects. Two threads can read the same `resultIndex`, both
increment, and both write the same slot. One increment is lost, one result overwrites another, and
some slots keep the default `GameResult{0, 0, ""}`. The program still terminates, which is what
makes the bug dangerous.

The vector is pre-sized on purpose. If it grew with `push_back` without a lock, the race would also
hit the vector's size and its internal buffer, and the output would be even harder to read. The
pre-sized vector isolates the race to `nextIndex` and to the slot writes, which is enough for the
lesson.

### Four modes

The runner has four modes, and each answers a different question.

| Mode | Shared result state | Synchronization | Sorts by seed? | What it proves |
|---|---|---|---|---|
| `sequential` | none | none | already in order | The reference checksum |
| `race` | one pre-sized vector and one `nextIndex` | none | no | Shared mutable state without a contract loses writes |
| `mutex` | one vector | `std::mutex` around the append | yes | A lock restores completeness |
| `merge` | one vector per thread | join, then merge | yes | Per-thread ownership restores completeness without a lock |

The two fixed modes are not the same design. `mutex` is shared state with a lock. `merge` is
ownership plus a handoff: each thread fills its own vector, and the parent merges the vectors after
the join. The handoff is the "message" in the lesson title. It is not a queue and it is not an
actor system; it is the smallest message-passing shape that solves this problem.

### Why the race is undefined behavior

`nextIndex += 1` is a read, an add and a write. C++ does not make those three steps atomic. Two
threads that read the same value both write the same successor, so one increment disappears. Worse,
two threads can receive the same `resultIndex` and write the same `std::vector` element at the same
time. Both are data races, and a data race in C++ is undefined behavior.

Undefined behavior means the standard does not promise a wrong number; it promises nothing. The
observed `written=14,719` is one compiled program on one machine. A different compiler, a different
optimization level or a different scheduler can produce a different number, including 16,000. That
is why the lesson asks you to run `race` more than once and why it never treats a single race output
as a stable fact.

### What determinism requires

The fixed modes match the sequential checksum because three conditions hold together.

1. **Per-game state is owned.** Each game has its own `RandomSequence`, so the game result is a
   function of its seed and nothing else.
2. **The result collection is complete.** Every one of the 16,000 results is written exactly once.
   The mutex guarantees it by serializing the append; the merge guarantees it by giving each thread
   a private vector.
3. **The order is explicit.** The checksum is order-sensitive FNV-1a. `sequential` produces seed
   order by construction. `mutex` and `merge` call `sort_by_seed` before the checksum. Without that
   sort, a complete result set would still produce a different checksum.

The third condition is the one people forget. Determinism is not only "the same values"; it is "the
same values in the same order". The engine fix gives you the values. The sort gives you the order.

One detail worth noticing: `merge` concatenates thread blocks in thread order, and this runner
assigns seeds in blocks (`threadIndex * gamesPerThread + gameIndex + 1`). So the concatenation is
already seed-ordered for this partition, and the `sort_by_seed` call is defensive. `mutex` is not
so lucky: threads finish in an arbitrary order, so its sort is required. The code sorts in both
places because the order is a contract, not an accident of the current seed assignment.

### What this lesson does not claim

- It does not claim the engine is thread-safe. One `GameEngine` shared by two threads is still a
  data race. The fix is one engine per thread, not a lock inside the engine.
- It does not claim the transcript adapter is thread-safe. It is a reference; the runner gives each
  game its own `RecordingTranscript`, so the question does not arise.
- It does not claim parallelism makes the game faster. The runner is a correctness experiment;
  timing is not measured.
- It does not claim the race is a pattern. It is intentional undefined behavior for teaching.
- It does not claim the 20 traces test the parallel code. They test the console transcript; the
  checksum tests the parallel result.

## Before you start

- [`construction-12`](../construction-12-polymorphism/lesson.md) — the catalog's formal
  prerequisite. The engine still uses the port from `construction-20`; you need to read
  `virtual void f() = 0;` and `override` without designing a hierarchy from scratch.
- [`construction-20`](../construction-20-hexagonal/lesson.md) — the immediate baseline and the left
  side of the diff in Step 4. Its `code/` directory is the artifact this lesson starts from, even
  though the catalog lists `construction-12`.
- [`construction-16`](../construction-16-ownership/lesson.md) — optional context. The engine owns
  its board and knights through `std::unique_ptr`; this lesson adds one more owned value.
- A C++20 compiler (`g++` or `clang++`) with `std::thread` and `std::mutex`. The runner needs
  `-pthread`. `cmake` is optional; `run.sh` falls back to a direct compile when CMake is absent.
- Optional: ThreadSanitizer (`-fsanitize=thread`). It makes the race visible as a report, not only
  as a wrong number.

You do not need to know lock-free programming, memory models or actor frameworks. The baseline is
one private integer, one `std::mutex`, and one vector per thread.

### What you need

- The concurrency baseline:
  [`code/main.cpp`](code/main.cpp), [`code/game.h`](code/game.h),
  [`code/game.cpp`](code/game.cpp), [`code/game_engine.h`](code/game_engine.h),
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
  [`code/random_sequence.h`](../construction-20-hexagonal/code/random_sequence.h),
  [`code/random_sequence.cpp`](../construction-20-hexagonal/code/random_sequence.cpp),
  [`code/game_engine.h`](../construction-20-hexagonal/code/game_engine.h), and their fourteen
  companions in [`../construction-20-hexagonal/code/`](../construction-20-hexagonal/code/).
- Build scripts and notes: [`run.sh`](run.sh), [`CMakeLists.txt`](CMakeLists.txt) and
  [`README.md`](README.md).
- The rules: [`common/spec/game-rules.md`](../../../common/spec/game-rules.md) — §12 fixes the LCG
  constants, §6 the move priority, §7 the knight moves, §8 when a cell becomes visited.
- The transcript format:
  [`common/spec/transcript-format.md`](../../../common/spec/transcript-format.md).
- One golden trace: [`common/traces/seed-0001.txt`](../../../common/traces/seed-0001.txt).
- This learner template: [`EVIDENCE.md`](EVIDENCE.md).

Check that the artifacts are present before you begin, from this lesson directory:

```bash
test -f code/main.cpp &&
test -f code/game.h &&
test -f code/game.cpp &&
test -f code/game_engine.h &&
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
test -f ../construction-20-hexagonal/code/random_sequence.cpp &&
test -f ../../../common/traces/seed-0001.txt
```

If a file is missing, the comparison has no second side. Record the missing file in `EVIDENCE.md`
§11 and return when it is available. A count against a file you have not read is not evidence.

The 8×8 baseline reproduces all 20 traces in `common/traces/` byte for byte, before you change
anything. That is the behavioral contract, and this lesson does not touch it.

## Two sittings

This lesson is long by design. Split it into two sittings:

- **Sitting 1 — Core (Steps 1–4, about 70 min).** Stop after Step 4, when ownership and the race
  are understood.
- **Sitting 2 — Extension (Steps 5–8, about 45 min).** Start only after Sitting 1. You will
  remove the race, compare and record the evidence.

## Step 1 — Inventory the state (~15 min)

Start with the state that moved. In `construction-20`:

```bash
grep -n 'randomState' ../construction-20-hexagonal/code/random_sequence.cpp
```

```text
7:std::uint64_t randomState = 0;
12:    randomState = static_cast<std::uint64_t>(seed);
17:    randomState = randomState * randomMultiplier + randomIncrement;
18:    return randomState % bound;
```

In `construction-21`:

```bash
grep -n 'state' code/random_sequence.h code/random_sequence.cpp
```

```text
code/random_sequence.h:13:    std::uint64_t state = 0;
code/random_sequence.cpp:11:    state = static_cast<std::uint64_t>(seed);
code/random_sequence.cpp:16:    state = state * randomMultiplier + randomIncrement;
code/random_sequence.cpp:17:    return state % bound;
```

The variable is now a private member. The only way to touch it is through the three methods.

Now the call sites in the engine:

```bash
grep -nE 'randomSequence\.' code/game_engine.h
```

```text
55:        randomSequence.set_seed(seed);
94:        int playerCellNumber = static_cast<int>(randomSequence.below(boardCellCount));
95:        int machineCellNumber = static_cast<int>(randomSequence.below(boardCellCount));
98:            machineCellNumber = static_cast<int>(randomSequence.below(boardCellCount));
108:        playerRoll = randomSequence.integer(1, 50);
109:        machineRoll = randomSequence.integer(1, 50);
112:            machineRoll = randomSequence.integer(1, 50);
166:        int startingMoveNumber = randomSequence.integer(1, moveCandidateCount);
```

Eight call sites. The free functions `random_set_seed`, `random_below` and `random_integer` are
gone; the engine now sends the same messages to an object it owns.

Now the runner's shared state:

```bash
grep -n 'nextIndex' code/parallel_runner.cpp
grep -n 'threadCount\|gamesPerThread\|totalGameCount' code/parallel_runner.cpp
grep -n 'lock_guard\|std::mutex' code/parallel_runner.cpp
grep -n 'perThreadResults' code/parallel_runner.cpp
```

```text
76:    int nextIndex = 0;
88:                    int resultIndex = nextIndex;
89:                    nextIndex += 1;
108:                totalGameCount, nextIndex, writtenResults.size(),
15:constexpr int threadCount = 8;
16:constexpr int gamesPerThread = 2000;
17:constexpr int totalGameCount = threadCount * gamesPerThread;
115:    std::mutex resultsMutex;
127:                    std::lock_guard<std::mutex> lock(resultsMutex);
143:    std::vector<std::vector<GameResult>> perThreadResults(threadCount);
154:                    perThreadResults[threadIndex].push_back(run_one_game(seed));
```

Read the four mode functions in order: `run_sequential` at line 61, `run_race` at 73, `run_mutex`
at 112 and `run_merge` at 141. The difference between `race` and `mutex` is four lines; the
difference between `mutex` and `merge` is the ownership of the result vector.

Counts for this baseline. The counting rule is stated so you can reproduce it:

- Files: 18.
- Headers: 10 (`board.h`, `console_transcript.h`, `game.h`, `game_engine.h`, `knight.h`,
  `move_generator.h`, `position.h`, `random_sequence.h`, `recording_transcript.h`,
  `transcript_port.h`).
- Translation units in the console build: 6 (`main.cpp`, `game.cpp`, `console_transcript.cpp`,
  `knight.cpp`, `position.cpp`, `random_sequence.cpp`).
- Translation units in the runner build: 4 (`parallel_runner.cpp`, `knight.cpp`, `position.cpp`,
  `random_sequence.cpp`).
- Translation units in the probe build: 4 (`adapter_probe.cpp`, `knight.cpp`, `position.cpp`,
  `random_sequence.cpp`).
- Complete class definitions in the core: 9 (the eight from `construction-20` plus
  `RandomSequence`). The runner adds the local `GameResult` record; it is not part of the core.
- Global mutable variables in the core: 0. In `construction-20`: 1 (`randomState`).
- Shared mutable result objects in the runner: 2 in `race` (`nextIndex` and `results`), 1 in
  `mutex` (`results`), 0 in `merge`.
- Source lines: 1,030.
- Files byte-identical to `construction-20`: 14 of 17.
- Files changed: 3 (`game_engine.h`, `random_sequence.h`, `random_sequence.cpp`).
- Files new: 1 (`parallel_runner.cpp`).

Then answer in `EVIDENCE.md` §2:

1. Where was the RNG state in `construction-20`, and where is it in `construction-21`?
2. How many bytes does `RandomSequence` occupy, and which member explains it?
3. How many `randomSequence.*` call sites are in `game_engine.h`, and which free functions did they
   replace?
4. Which three files changed, which file is new, and which fourteen are byte-identical?
5. How many threads, games per thread and total games does the runner use?
6. Which modes share a result vector, and which mode gives each thread its own?
7. Which mode has no synchronization, and which two objects are shared in it?
8. Which modes sort by seed before the checksum, and why does the checksum need that order?
9. What sentinel does the race scan use, and why is it safe for these seeds?
10. Which state left BSS, and how many bytes did BSS lose?

Question 9 has a precise answer: the scan counts results whose `seed` is not 0, and the runner
assigns seeds from 1 to 16,000, so 0 is a safe "not written" marker. If seeds could be 0, the scan
would undercount. Question 10: `randomState` left `.bss`, and BSS went from 24 to 16 bytes.

## Step 2 — Predict before you run (~15 min)

Write these predictions before running anything. Do not look at the answers in Step 3 first.

### 2.1 The structure table

| Question | My prediction |
|---|---|
| Files in `construction-20` | |
| Files here | |
| Headers here | |
| Translation units in the console build | |
| Translation units in the runner build | |
| Source lines in `construction-20` | |
| Source lines here | |
| Files changed | |
| Files new | |
| Files byte-identical | |
| Global mutable variables in the core, before and after | |
| Shared mutable result objects in the runner | |

The row that is easy to get wrong is "files changed". The engine looks different, but only its call
sites changed; the port, the adapters, the board and the knights are untouched.

### 2.2 The race table

| Question | My prediction |
|---|---|
| `race` written, of 16,000 | |
| `race` `next_index` | |
| Does `written` equal `next_index`? | |
| Is the `race` checksum stable across runs? | |
| `sequential` checksum | |
| `mutex` checksum | |
| `merge` checksum | |
| Which mode does not sort by seed? | |
| Which modes match the sequential checksum? | |

Predict `written` below 16,000. If you predict 16,000, you are assuming the increment is atomic,
which it is not. Predict the checksum as unstable: the race changes the set of results and their
order.

### 2.3 The size and text table

| Question | My prediction |
|---|---|
| `sizeof(GameEngine<8, 8>)`, before and after | |
| `sizeof(RandomSequence)` | |
| `game.o` text bytes, before and after | |
| Binary text bytes, before and after | |
| Binary data bytes, before and after | |
| Binary BSS bytes, before and after | |
| Seeds that match, of 20 | |

Predict the engine at 40 bytes: the `construction-20` engine is 32, and the `RandomSequence` member
is 8. Predict BSS at 16: the global `randomState` was 8 bytes, and it is gone.

### 2.4 The removal table

| Question | My prediction |
|---|---|
| The fix I will apply | |
| Files I will edit | |
| Lines I will add | |
| Written after the fix | |
| Checksum after the fix | |
| Seeds that match, of 20, after the fix | |
| Build result with `-Wall -Wextra -Werror` | |

Predict one file: `parallel_runner.cpp`. The engine, the transcript and the traces should not move.

## Step 3 — Run the baseline (~20 min)

### 3.1 The traces

First the traces, unmodified:

```bash
mkdir -p /tmp/concurrency-test
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
    -o /tmp/concurrency-test/concurrency-baseline \
    code/main.cpp code/game.cpp code/console_transcript.cpp \
    code/knight.cpp code/position.cpp code/random_sequence.cpp
```

Then run the same loop with `/tmp/concurrency-test/concurrency-baseline` instead of `./run.sh`.

### 3.2 The four modes

Build the runner:

```bash
c++ -std=c++20 -O2 -pthread -Icode -o /tmp/concurrency-test/parallel-runner \
    code/parallel_runner.cpp code/knight.cpp code/position.cpp code/random_sequence.cpp
```

Run each mode:

```bash
for mode in sequential race mutex merge; do
    /tmp/concurrency-test/parallel-runner --mode "$mode"
done
```

The output on this machine:

```text
mode=sequential games=16000 written=16000 checksum=2826026307342797653
mode=race games=16000 next_index=14719 written=14719 checksum=17876456234189950703
mode=mutex games=16000 written=16000 checksum=2826026307342797653
mode=merge games=16000 written=16000 checksum=2826026307342797653
```

The race line is one sample. Your `next_index`, `written` and checksum will differ.

| Mode | Shared result state | Synchronization | Written | Checksum |
|---|---|---|---|---|
| `sequential` | none | none | 16,000 | 2826026307342797653 |
| `race` | one pre-sized vector and `nextIndex` | none | 14,719–14,821 | varies |
| `mutex` | one vector | `std::mutex` | 16,000 | 2826026307342797653 |
| `merge` | one vector per thread | join, then merge | 16,000 | 2826026307342797653 |

The fixed modes match the sequential checksum exactly. The race does not, and the written count is
short.

### 3.3 Repeat the race

One race output is an anecdote. Run it four times:

```bash
for run in 1 2 3 4; do
    /tmp/concurrency-test/parallel-runner --mode race
done
```

Four separate runs on this machine:

| Run | `next_index` | `written` | Checksum |
|---|---:|---:|---:|
| 1 | 14,719 | 14,719 | 17,876,456,234,189,950,703 |
| 2 | 14,821 | 14,821 | 12,745,221,390,738,065,136 |
| 3 | 14,778 | 14,778 | 13,488,474,134,281,646,459 |
| 4 | 14,766 | 14,766 | 8,624,756,145,285,332,907 |

The table keeps the order in which the runs were recorded; the point is that both columns move. The
pairing is illustrative, not a property of the program.

Two observations to explain.

1. **`written` equals `next_index` in these runs.** The loop writes every index the surviving
   increments produce, and the scan counts non-zero seeds. A thread that loses an increment still
   writes at the index it read, so the slot is not empty; it is overwritten. This equality is an
   observation of this compiled program under undefined behavior, not a guarantee.
2. **The checksum moves for two reasons.** Some results are missing, and the race does not sort by
   seed. Even if `written` were 16,000, the vector order would not be seed order, so the checksum
   would still differ from sequential. The written count is the direct evidence of lost writes; the
   checksum is evidence of a different result set *and* a different order.

That second point matters for the change test. A fix that only makes the writes complete, without
sorting, would still fail the checksum. The acceptance is "parallel checksum matches sequential",
and that requires both completeness and order.

### 3.4 Sizes

The probe goes in the scratch directory, never in the tracked files:

```bash
cat > /tmp/concurrency-test/sizes.cpp <<'EOF'
#include "game_engine.h"
#include "random_sequence.h"

#include <cstdio>

int main()
{
    std::printf("sizeof(GameEngine<8,8>)=%zu\n", sizeof(GameEngine<8, 8>));
    std::printf("sizeof(RandomSequence)=%zu\n", sizeof(RandomSequence));
    return 0;
}
EOF
c++ -std=c++20 -Wall -Wextra -Werror -O2 -Icode \
    -o /tmp/concurrency-test/sizes /tmp/concurrency-test/sizes.cpp \
    code/knight.cpp code/position.cpp code/random_sequence.cpp
/tmp/concurrency-test/sizes
```

The output on this platform:

```text
sizeof(GameEngine<8,8>)=40
sizeof(RandomSequence)=8
```

The engine grew by 8 bytes, exactly the `RandomSequence` member. The sequence itself is one 64-bit
integer.

### 3.5 Object and binary

```bash
c++ -std=c++20 -O2 -Icode -c code/game.cpp -o /tmp/concurrency-test/game21.o
c++ -std=c++20 -O2 -I../construction-20-hexagonal/code \
    -c ../construction-20-hexagonal/code/game.cpp -o /tmp/concurrency-test/game20.o
size /tmp/concurrency-test/game20.o /tmp/concurrency-test/game21.o
```

```text
   text    data     bss     dec     hex filename
   4138       0       0    4138    102a /tmp/concurrency-test/game20.o
   4280       0       0    4280    10b8 /tmp/concurrency-test/game21.o
```

The core object grows by 142 text bytes: the class methods and the call sites that now pass
`this`.

Now the linked binaries:

```bash
c++ -std=c++20 -O2 -Icode -o /tmp/concurrency-test/bin21 \
    code/main.cpp code/game.cpp code/console_transcript.cpp \
    code/knight.cpp code/position.cpp code/random_sequence.cpp
c++ -std=c++20 -O2 -I../construction-20-hexagonal/code \
    -o /tmp/concurrency-test/bin20 \
    ../construction-20-hexagonal/code/main.cpp \
    ../construction-20-hexagonal/code/game.cpp \
    ../construction-20-hexagonal/code/console_transcript.cpp \
    ../construction-20-hexagonal/code/knight.cpp \
    ../construction-20-hexagonal/code/position.cpp \
    ../construction-20-hexagonal/code/random_sequence.cpp
size /tmp/concurrency-test/bin20 /tmp/concurrency-test/bin21
```

```text
   text    data     bss     dec     hex filename
  10748     876      24   11648    2d80 /tmp/concurrency-test/bin20
  10869     876      16   11761    2df1 /tmp/concurrency-test/bin21
```

Text grows by 121 bytes, data does not move, and BSS shrinks by 8. The last number is the global RNG
leaving the binary: `randomState` used to live in `.bss`, and now the state lives inside a
heap-allocated `GameEngine`.

If the first trace diff is not empty, stop. That is a different baseline from the one this lesson
describes; record it in `EVIDENCE.md` §11 and do not adjust the prediction to match.

### 3.6 Optional: see the race as a report

ThreadSanitizer turns the wrong number into a diagnosis:

```bash
c++ -std=c++20 -O1 -g -fsanitize=thread -pthread -Icode \
    -o /tmp/concurrency-test/race-tsan \
    code/parallel_runner.cpp code/knight.cpp code/position.cpp code/random_sequence.cpp
/tmp/concurrency-test/race-tsan --mode race
```

Look for `WARNING: ThreadSanitizer: data race`. One stack points at the read or write of
`nextIndex`; the other points at another thread doing the same. TSan slows the run and changes the
interleaving, so the `written` count is not comparable to the optimized build. The report is the
evidence, not the number.

If TSan is not installed, skip this step and record that in `EVIDENCE.md` §11. Do not install a
toolchain just for this lesson.

## Step 4 — Compare: what ownership proves (~20 min)

Open `random_sequence.h`, `random_sequence.cpp`, `game_engine.h` and `parallel_runner.cpp`. Five
facts, read against the measurements.

1. **The core has no global mutable RNG.** `randomState` is gone. The only `state` is a private
   member of `RandomSequence`, and the only way to reach it is through `set_seed`, `below` and
   `integer`.
2. **The sequence is unchanged.** The constants, the multiply-add and the modulo are the same. The
   20 traces are the check: if the sequence had changed, the first trace would diverge.
3. **Each engine owns its sequence.** `sizeof(GameEngine<8, 8>)` is 40, up from 32, and
   `sizeof(RandomSequence)` is 8. The engine carries the state, so two engines carry two states.
4. **BSS shrinks.** The binary's BSS goes from 24 to 16 bytes. The global had 8 bytes; the member
   lives in the engine, which the runner allocates per game. Data is unchanged at 876.
5. **The runner's race is a second, separate shared state.** Removing the global RNG did not remove
   the need to think about the result collection. The engine is deterministic per game; the runner
   still has to collect the games correctly.

### 4.1 The exact diff

Three files changed and one is new. The diff against `construction-20`:

```bash
diff -u ../construction-20-hexagonal/code/random_sequence.h code/random_sequence.h
diff -u ../construction-20-hexagonal/code/random_sequence.cpp code/random_sequence.cpp
diff -u ../construction-20-hexagonal/code/game_engine.h code/game_engine.h
```

| File | Added | Removed | Net | What changed |
|---|---:|---:|---:|---|
| `game_engine.h` | 9 | 8 | +1 | Eight call sites go through `randomSequence`; one member added |
| `random_sequence.h` | 10 | 3 | +7 | Free functions become a class with private state |
| `random_sequence.cpp` | 7 | 8 | -1 | `randomState` leaves the anonymous namespace; methods use `state` |
| `parallel_runner.cpp` | 208 | 0 | +208 | New second program with four modes |
| Total | 234 | 19 | +215 | Three changed, one new, fourteen identical |

The fourteen identical files are `board.h`, `move_generator.h`, `knight.h`, `knight.cpp`,
`position.h`, `position.cpp`, `transcript_port.h`, `console_transcript.h`,
`console_transcript.cpp`, `recording_transcript.h`, `game.h`, `main.cpp`, `game.cpp` and
`adapter_probe.cpp`. The rules, the transcript format and the game loop are untouched.

### 4.2 What did not change

- The LCG. Same multiplier, same increment, same modulo, same seed handling.
- The transcript. All 20 traces match, byte for byte.
- The rules. Move order, capture, trap, draw and the 200-move limit are the same.
- The board layout. `board.h` is byte-identical; `sizeof(Board<8, 8>)` is still 256.
- The move generator. `move_generator.h` is byte-identical.
- The attempt loop. `playTurn` has the same four outcomes and the same two stop conditions.
- The public game interface. `game.h` and `run_game` keep their signatures.
- The trace count. 20 of 20 before and after.

### 4.3 What it costs

- 215 new lines and one new file, most of them the runner.
- 8 more bytes per engine for the `RandomSequence` member.
- 142 more text bytes in `game.o` and 121 more text bytes in the linked binary.
- A new lifetime rule inside the engine: the sequence is constructed with the engine and destroyed
  with it. There is no shared registry and no reset between games.
- A new reading burden: to know how a game gets its randomness, a reader now follows
  `GameEngine::randomSequence` instead of a free function.
- A second program to build and run. The console build does not link the runner.

None of these is a defect. They are the price of moving the state into the object that uses it, and
the record should list them next to the benefit.

### 4.4 The state table

This table is the heart of the comparison. Read it against the four modes.

| State | `construction-20` owner | `construction-21` owner | Shared between games? | Synchronization |
|---|---|---|---|---|
| RNG state | one file-static `randomState` | one `RandomSequence` per engine | No | None needed |
| Board | one `Board` per engine | one `Board` per engine | No | None needed |
| Transcript | one adapter per game | one adapter per game | No | None needed |
| Result vector | — | one per process in `race` and `mutex`; one per thread in `merge` | Yes in `race` and `mutex` | None in `race`; mutex in `mutex`; join in `merge` |
| `nextIndex` | — | one per process in `race` | Yes | None, intentionally |

The first three rows are owned. The last two are the runner's problem. The lesson's title is about
the difference: owned state needs no protocol; shared state needs one.

### 4.5 Why the fixed modes match

Walk the three conditions from the concept section against the code.

- `run_one_game` constructs a fresh `RecordingTranscript` and a fresh `GameEngine<8, 8>` per call.
  The engine constructs a fresh `RandomSequence`. No state survives a game.
- `run_mutex` appends under `std::lock_guard<std::mutex>`, so the vector has exactly 16,000
  elements after the join. It then calls `sort_by_seed`.
- `run_merge` gives each thread its own vector, merges after the join, and calls `sort_by_seed`.
- `result_checksum` folds `seed`, `moveCount` and `winner` in vector order with FNV-1a. It does not
  sort. The caller owns the order.

The sequential mode pushes seeds 1 to 16,000 in order, so its vector is already in the order the
checksum expects. The fixed parallel modes reproduce that order explicitly. That is why the three
checksums are equal.

## Step 5 — Change test: remove the race (~25 min)

The acceptance names the change test: **reproduce a lost-write race, remove it with a mutex or
per-thread results, and show the parallel checksum matches the sequential checksum while the 20
traces still pass.**

You have already reproduced the race in Step 3. Now remove it, twice if you can: once with a lock
and once with per-thread ownership. The two fixes teach different things.

### 5.1 Make a scratch copy

Never edit the tracked `code/` directory. Copy it:

```bash
rm -rf /tmp/concurrency-test/change
cp -r code /tmp/concurrency-test/change
```

### 5.2 Fix A: a mutex around the append

In `/tmp/concurrency-test/change/parallel_runner.cpp`, replace the `run_race` body. The original
starts with a pre-sized vector and a shared index:

```cpp
    std::vector<GameResult> results(totalGameCount);
    int nextIndex = 0;
```

Replace those two lines with a growable vector and a mutex:

```cpp
    std::vector<GameResult> results;
    results.reserve(totalGameCount);
    std::mutex resultsMutex;
```

Change the lambda capture from `[&results, &nextIndex, threadIndex]` to
`[&results, &resultsMutex, threadIndex]`, and replace the three shared-index lines:

```cpp
                    int resultIndex = nextIndex;
                    nextIndex += 1;
                    results[resultIndex] = result;
```

with one protected append:

```cpp
                    std::lock_guard<std::mutex> lock(resultsMutex);
                    results.push_back(result);
```

After the join, add the sort and replace the race `printf`:

```cpp
    sort_by_seed(results);
    std::printf("mode=race games=%d written=%zu checksum=%llu\n", totalGameCount,
                results.size(), static_cast<unsigned long long>(result_checksum(results)));
```

Delete the `writtenResults` scan; it existed only to count the holes. `run_race` is now almost
identical to `run_mutex`, which is the point: the lock turns the broken collector into the working
one.

### 5.3 Fix B: one vector per thread

If you want the message-passing shape, replace the `run_race` body with the `run_merge` body:

```cpp
    std::vector<std::vector<GameResult>> perThreadResults(threadCount);
    std::vector<std::thread> threads;
    threads.reserve(threadCount);
    for (int threadIndex = 0; threadIndex < threadCount; threadIndex += 1)
    {
        threads.emplace_back(
            [&perThreadResults, threadIndex]
            {
                for (int gameIndex = 0; gameIndex < gamesPerThread; gameIndex += 1)
                {
                    int seed = threadIndex * gamesPerThread + gameIndex + 1;
                    perThreadResults[threadIndex].push_back(run_one_game(seed));
                }
            });
    }
    for (std::thread &thread : threads)
    {
        thread.join();
    }

    std::vector<GameResult> results;
    results.reserve(totalGameCount);
    for (const std::vector<GameResult> &threadResults : perThreadResults)
    {
        results.insert(results.end(), threadResults.begin(), threadResults.end());
    }
    sort_by_seed(results);
```

Then print the same three-field line as Fix A. Each thread owns its vector; the join is the handoff;
the merge is the reduction. No lock appears.

### 5.4 Build and compare

```bash
c++ -std=c++20 -Wall -Wextra -Werror -O2 -pthread \
    -I/tmp/concurrency-test/change \
    -o /tmp/concurrency-test/change-runner \
    /tmp/concurrency-test/change/parallel_runner.cpp \
    /tmp/concurrency-test/change/knight.cpp \
    /tmp/concurrency-test/change/position.cpp \
    /tmp/concurrency-test/change/random_sequence.cpp
/tmp/concurrency-test/change-runner --mode race
```

Expected output for either fix:

```text
mode=race games=16000 written=16000 checksum=2826026307342797653
```

Run it more than once. The checksum must not move. Then run the 20 traces on the console program;
the fix touched only the runner:

```bash
for trace in ../../../common/traces/seed-*.txt; do
    traceName="$(basename "${trace}" .txt)"
    seedText="${traceName#seed-}"
    seedNumber="$((10#${seedText}))"
    ./run.sh --seed "${seedNumber}" --name Learner | cmp - "${trace}"
done
```

The loop is silent: 20 of 20. The fix touched one file and no game code.

### 5.5 What the change test proves, and what it does not

It proves five things.

1. The lost writes are in the result collection, not in the game logic. The engine and the
   transcript did not change, and the fixed checksum matches sequential.
2. The per-engine RNG is what makes the per-game result independent of scheduling. Without it, a
   lock around the vector would still produce different game results.
3. The checksum match needs complete results *and* seed order. The sort is part of the fix, not
   decoration.
4. The fix is local: one function in one file. The console build, the port and the adapters are
   untouched.
5. Both shapes work. A lock and a per-thread vector produce the same checksum; they differ in cost,
   not in result.

It does not prove that the engine is thread-safe when shared. One `GameEngine` used by two threads
is still a data race. It does not prove that the mutex is free: the append is serialized, and the
game computation still runs in parallel. It does not prove that the merge is faster: it avoids the
lock but allocates per-thread vectors and copies them at the end. It does not prove that the race is
reproducible; it is undefined behavior, and a different build can hide it.

One variant worth naming: making `nextIndex` a `std::atomic<int>` with `fetch_add` gives each thread
a unique index, so the writes become disjoint and the lost writes disappear. But the order is still
arbitrary, so the checksum still differs until you sort. The atomic fixes the counter, not the
order contract, and it does not generalize to a vector that grows. Do not mistake it for a complete
fix.

## Step 6 — Compare with `construction-20` (~10 min)

| Metric | `construction-20` | `construction-21` | Difference |
|---|---:|---:|---:|
| Files | 17 | 18 | +1 |
| Headers | 10 | 10 | 0 |
| Translation units (console build) | 6 | 6 | 0 |
| Source lines | 815 | 1,030 | +215 |
| Changed files | — | 3 | +3 |
| New files | — | 1 | +1 |
| Files byte-identical | — | 14 of 17 | — |
| Global mutable RNG | 1 | 0 | -1 |
| `sizeof(GameEngine<8, 8>)` | 32 | 40 | +8 |
| `sizeof(RandomSequence)` | — | 8 | +8 |
| `game.o` text | 4,138 | 4,280 | +142 |
| Binary text | 10,748 | 10,869 | +121 |
| Binary data | 876 | 876 | 0 |
| Binary BSS | 24 | 16 | -8 |
| Traces | 20 of 20 | 20 of 20 | 0 |
| Parallel modes | 0 | 4 | +4 |

### 6.1 What it buys

- The core has no global mutable state. The RNG state belongs to the engine that uses it.
- Two engines can run in two threads without coordinating their randomness. The sequence is per
  game, not per process.
- The fixed parallel modes reproduce the sequential checksum exactly. The game result is
  independent of the scheduler.
- BSS shrinks by 8 bytes because the global leaves the binary.
- The runner separates the two questions: game state (fixed by ownership) and result collection
  (fixed by a lock or a handoff).
- The 20 traces still pass, so the console behavior is unchanged.
- The lesson leaves a reusable diagnostic: `written` versus `totalGameCount`, plus a checksum that
  is sensitive to both values and order.

### 6.2 What it costs

- 215 new lines and one new file, most of them the runner.
- 8 more bytes per engine for the sequence.
- 142 more text bytes in `game.o` and 121 more in the binary.
- The engine is not reentrant. One engine per thread is the contract, and the type does not enforce
  it.
- The result order must be made explicit. A complete but unsorted vector still fails the checksum.
- The transcript adapter is per game. A shared adapter would need its own synchronization, which
  this baseline avoids.
- The race mode is undefined behavior and must not be copied into production code.

### 6.3 The gap: ownership is not the whole answer

The baseline is not "the program is concurrent". It is "the game state is owned, and the runner
shows what happens when the result state is not". Five couplings remain, and they are worth naming.

1. **The engine is not thread-safe when shared.** The `RandomSequence` member makes per-game state
   private, not the whole engine reentrant. Two threads calling `run` on the same engine would race
   on the board, the knights and the sequence.
2. **The transcript port is a reference.** A shared adapter would need a lock or a queue. The runner
   sidesteps the question by constructing one `RecordingTranscript` per game.
3. **The result collection is a separate design.** The runner's four modes are a teaching device,
   not a general-purpose scheduler. A real system would need a queue, backpressure and a shutdown
   protocol.
4. **The checksum is order-sensitive.** "Deterministic" includes the merge order. A future change
   that removes `sort_by_seed` would break the comparison without breaking any single game.
5. **The traces do not test threads.** They test the console transcript. The parallel claim rests
   on the checksum, and the checksum rests on the same per-game computation.

The precise acceptance sentence is narrower than "the program is concurrent":

> The learner can reproduce a lost-write race, remove it with a mutex or per-thread results, and
> show that the parallel checksum matches the sequential checksum while the 20 traces still pass.

Race reproduced, race removed, checksum equal, traces unchanged. Nothing about lock-free code,
throughput, or a general actor framework.

## Step 7 — Explain (~5 min)

Write `EVIDENCE.md` §8. Two parts.

**First, one sentence.** Say what changed between the two baselines and what the change decides,
without using the words *thread*, *mutex*, *lock*, *race*, *concurrency*, *concurrent*, *parallel*,
*atomic*, *shared*, *better*, *worse*, *clean* or *decoupled*. If your sentence needs one of those,
you have named the mechanism rather than the decision.

**Second, one paragraph.** Answer:

- What changed: the RNG state moved from a file-static variable to a private member of
  `RandomSequence`, owned by each `GameEngine`.
- What stayed: the LCG constants, the update, the modulo, the transcript, the board, the loop and
  the 20 traces.
- What the runner adds: four modes; `race` is the failure; `mutex` and `merge` are the fixes.
- What the race is: two shared objects (`nextIndex` and the pre-sized vector) without
  synchronization; lost writes; undefined behavior.
- What the fixes are: one lock around the append, or one vector per thread merged after the join;
  both sort by seed before the checksum.
- What determinism requires: per-game state ownership plus a deterministic result order.
- What the numbers prove: BSS 24 to 16 (the global is gone), engine 32 to 40 (the member is added),
  `game.o` 4,138 to 4,280, binary 10,748 to 10,869 text, data unchanged, traces 20 of 20.
- What the checksum proves: `mutex` and `merge` equal `sequential`; `race` does not.
- What the traces prove: the console transcript did not change; they do not test threads.
- What the next lesson owns: error contracts, `construction-22`.

A strong answer keeps four things apart: **who owns the game state** (the engine), **who owns the
result state** (the runner, one vector or one per thread), **what the order contract is** (sort by
seed before the checksum), and **what a run can see** (the transcript and the checksum). A weak
answer says "the code is thread-safe", which is false: the engine is safe only when each thread has
its own.

## Step 8 — Acceptance check and record (~5 min)

The acceptance criterion for this lesson is: **race detected and removed; determinism kept; 20
traces pass.**

Check your record against it:

- [ ] I located the global `randomState` in `construction-20` and the private `state` member here.
- [ ] I counted the eight `randomSequence.*` call sites in `game_engine.h`.
- [ ] I named the three changed files, the new runner and the fourteen identical files.
- [ ] I predicted the structure, race, size and removal tables before measuring.
- [ ] I reproduced all 20 traces on the unmodified console baseline.
- [ ] I ran `sequential`, `race`, `mutex` and `merge` and recorded written and checksum.
- [ ] I ran `race` at least four times and saw the result move.
- [ ] I explained why `written` equals `next_index` in the observed runs, with the UB caveat.
- [ ] I explained why the race checksum differs for two reasons: missing results and unsorted order.
- [ ] I measured `sizeof(GameEngine<8, 8>)` at 40 and `sizeof(RandomSequence)` at 8.
- [ ] I measured `game.o` text at 4,280 and the binary at 10,869 text, 876 data and 16 BSS.
- [ ] I removed the race in a scratch copy with a mutex or per-thread results.
- [ ] The fixed run printed `written=16000` and `checksum=2826026307342797653`.
- [ ] The fixed run was stable across at least two executions.
- [ ] All 20 traces matched after the fix.
- [ ] The scratch build passed with `-Wall -Wextra -Werror`.
- [ ] I named what the engine does not guarantee: one engine shared by two threads is still a race.
- [ ] I named the order contract: `sort_by_seed` before the checksum.
- [ ] I did not edit any tracked code file.
- [ ] I completed `EVIDENCE.md` §1–§10 and the self-check in §12.

The criterion is not "the program is concurrent". It is "you can show a lost write, remove it, and
keep the result deterministic without changing the transcript".

## Self-check (binary)

- [ ] I read `random_sequence.h`, `random_sequence.cpp`, `game_engine.h` and `parallel_runner.cpp`.
- [ ] I explained the ownership change from a file-static `randomState` to a per-engine
      `RandomSequence`.
- [ ] I counted 8 `randomSequence.*` call sites and 1 intentional `nextIndex`.
- [ ] I compared the baseline with `construction-20` and named the three changed files and the new
      runner.
- [ ] I predicted the structure, race, size and removal tables before running.
- [ ] I reproduced all 20 traces on the unmodified baseline.
- [ ] I ran the four modes and recorded written and checksum for each.
- [ ] I ran `race` more than once and explained the variation.
- [ ] I measured the engine at 40 bytes and `RandomSequence` at 8.
- [ ] I measured `game.o` at 4,280 text bytes, the binary at 10,869 text and 876 data bytes, and
      BSS at 16.
- [ ] I removed the race in a scratch copy, with a mutex and, if possible, with per-thread results.
- [ ] The fixed checksum matched the sequential checksum and stayed stable across runs.
- [ ] All 20 traces matched after the fix.
- [ ] I named the order contract and the engine's non-reentrancy.
- [ ] I completed `EVIDENCE.md`.

These are acceptance criteria. If a box is
unchecked, write the next smallest action in `EVIDENCE.md` §10.

## Misconceptions this lesson refutes

| Misconception | Why it fails | What to do instead |
|---|---|---|
| "The global RNG was fine because each game sets the seed." | Two threads set and advance the same variable; the interleaving corrupts both. | Give each engine its own `RandomSequence`. |
| "Concurrency means adding threads to the console program." | The console program is sequential; the runner is a second program. | Keep the experiment separate. |
| "The race is in the game engine." | The engine's RNG is per engine; the race is in the runner's `nextIndex` and vector. | Read `parallel_runner.cpp`. |
| "`written` should be 16,000 even with the race." | Lost updates leave holes; the scan counts non-zero seeds. | Measure `written` and `next_index`. |
| "The checksum mismatch proves lost writes." | The race also does not sort, so the order differs even with full writes. | Use the written count as the direct evidence. |
| "The race is reproducible." | It is undefined behavior; the interleaving varies. | Run it more than once. |
| "Making `nextIndex` atomic is enough." | It removes lost indices, but the order is still arbitrary; you still need to sort. | Fix the collection and the order. |
| "A mutex makes the whole program sequential." | The game computation still runs in parallel; only the append is serialized. | Measure if you care about time. |
| "Merge is always faster than mutex." | It avoids the lock but allocates per-thread vectors and copies at the end. | Measure both. |
| "The 20 traces prove the parallel code." | They exercise the console program, one game at a time. | Use the checksum for the parallel claim. |
| "The engine is thread-safe now." | One engine shared by two threads is still a race. | One engine per thread. |
| "`sizeof(GameEngine)` grew because of the thread." | It grew by 8 bytes for the `RandomSequence` member. | Measure `sizeof(RandomSequence)`. |
| "BSS should grow with threads." | The global RNG left BSS; the member lives in the heap-allocated engine. | Measure BSS. |
| "The transcript adapter is thread-safe." | It is a reference; a shared adapter would need synchronization. | One adapter per game. |
| "The race is a pattern to reuse." | It is intentional undefined behavior for the lesson. | Use a mutex or per-thread results. |
| "The merge sort is required because concatenation is random." | For this block assignment, concatenation is already seed-ordered; the sort is a contract. | Explain why it is still there. |
| "Determinism is only about the RNG." | The result order also matters. | Sort before the checksum. |
| "The parallel checksum replaces the traces." | They test different things: result equality versus transcript bytes. | Run both. |

## Next step

`construction-22` owns error contracts. The seam from `construction-20` and the per-engine state
from this lesson make the next question sharp: when a move is invalid, who decides what happens?
`tryMove` already returns a `MoveOutcome` value, but the console program still returns a nonzero
status on bad command-line input, and the engine has no way to report a recoverable error to a
caller. The next lesson turns that value into a contract the caller can handle without killing the
process.

See the [lesson catalog](../../../docs/guide/lesson-catalog.md) for the full path.

If either baseline was missing, do not start `construction-22`. The next contrast needs both sides.
