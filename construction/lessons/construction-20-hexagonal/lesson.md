# Hexagonal (hexagonal)

> [English](lesson.md) | [Español](lesson-es.md)

**Concept:** ports and adapters · **Time:** about 92 minutes ·
**Prerequisites:** construction-08, construction-17 · **Status:** drafted
**Reading:** ~26 minutes

## Why this lesson exists

`construction-17` ended with an engine that knew how to print:

```cpp
void printTurnHeader(int turnNumber, const Knight *activeKnight,
                     const int moveOrder[moveCandidateCount]) const
{
    std::printf("turn=%d who=%s at=%d,%d order=", turnNumber,
                activeKnight->role_name(), activeKnight->row(),
                activeKnight->column());
    // ...
}
```

Every event in the game was also a `printf`. The core decided what happened and how it looked,
in the same function. This lesson removes the second half of that sentence. The core keeps
deciding *what happened*; an object outside it decides *how it looks*.

The sentence to keep:

> The core announces events in the language of the game; an adapter turns those events into a
> transcript, a count, or anything else. The core does not know which.

Three honest notes before the comparison.

1. The source grows by 204 lines, from 611 across twelve files to 815 across seventeen. Five files
   are new, two changed, and ten are byte-identical to `construction-17`. `game_engine.h` shrinks
   from 302 lines to 245 largely because six print methods left it; the new port, adapters and
   probe are 260 lines.
2. The core object shrinks: `game.o` text goes from 4,656 to 4,138 bytes, a loss of 518. The console
   adapter object adds 1,341 bytes, and the linked binary grows from 9,668 to 10,748 text bytes and
   from 692 to 876 data bytes. Eight indirect calls appear in `game.o`, where `construction-17` had
   none. That is the price of dispatch, and it is measurable.
3. The transcript does not change. All 20 golden traces match byte for byte. The board, the move
   generator, the knight, the position and the random sequence are the same files, unchanged.

The comparison is not "hexagonal is better". It is narrower: one dependency arrow is reversed, one
new indirection appears, and a second program can run the same core without a console.

## What you will be able to do

By the end of this lesson you can:

- explain ports and adapters and the dependency arrow;
- run the same core with console and test adapters.

## The concept: who decides how it looks

### The dependency arrow

`construction-17` had one arrow: the engine included `<cstdio>` and called `std::printf`. The core
depended on the console.

`construction-20` has a different shape:

```text
        +----------------+          +------------------+
        |   GameEngine   | -------> |  TranscriptPort  | <------- ConsoleTranscript
        +----------------+  calls   +------------------+  implements
              |                            ^
              |                            |
              |                     RecordingTranscript
              |
        owns Board, Knight, Position, random state
```

Read the arrows as source dependencies, not as runtime calls.

- `GameEngine` includes `transcript_port.h`. It never includes `console_transcript.h`.
- `ConsoleTranscript` includes `transcript_port.h`. It may include `<cstdio>`.
- `RecordingTranscript` includes `transcript_port.h`. It includes no output header at all.
- The composition root, `game.cpp`, includes both the engine and the console adapter, and connects
  them.

The arrow from the engine to the console is gone. What remains is an arrow from the engine to an
abstract contract, and an arrow from each adapter to the same contract. The console is now a leaf.

### A port is a contract the core owns

The port has one method per event in the transcript:

| Method | What it announces |
|---|---|
| `game_header` | seed, player name, rolls, who starts, both starting positions |
| `turn_header` | turn number, active role, position, move order |
| `illegal_attempt` | a move number, a destination, and why it was illegal |
| `capture_attempt` | a move number, a destination, and the knight that would be captured |
| `legal_attempt` | a move number and a destination |
| `move` | move number, role, from, to, and whether it was a capture |
| `no_legal_move` | the role that is trapped |
| `winner` | the winner's name and the final move count |

Two properties make this a port and not a print helper.

1. **The names are game events.** `capture_attempt` says what happened in the game. `printf` with
   a format string says what the console should show. The first survives a change of output; the
   second does not.
2. **The parameters are values, not text.** The core passes a row and a column, not `"3,4"`. It
   passes the move-order array, not `"4,5,6,7,8,1,2,3"`. Formatting stays on the adapter side.

The core still decides *which* events happen, *when*, and *with which values*. That is the game's
knowledge, and it belongs in the core. The port does not move that knowledge out; it moves the
rendering out.

### An adapter is a choice

Two adapters implement the same eight methods.

`ConsoleTranscript` turns each event into the exact bytes of
`common/spec/transcript-format.md`. It is the only file in the baseline build that prints the
transcript. It has 20 lines that mention `printf`.

`RecordingTranscript` counts events and stores the winner. It has eight counters, one `std::string`
for the winner and one integer for the final move count. It prints nothing. It is header-only, so
the probe can include it without a separate translation unit.

The same `GameEngine<8, 8>` type runs with both. The template parameter is the board size; the
adapter is not a template parameter. The engine depends on the base class, and the base class is
the only thing it needs to know.

### Where the seam sits

The seam is at the event boundary, not at the cell or the move level.

- If the seam were finer, for example one call per cell write, the adapter would need the rules to
  know which writes matter. The core would leak its decisions.
- If the seam were coarser, for example one call per completed transcript line with a formatted
  string, the core would keep the formatting decision. The adapter would be a `puts`.
- Eight events is the middle. Five of them are attempt or move events inside the turn loop; three
  are header, trap and winner events around it.

That choice is the design. It is also the part a later change can invalidate: an adapter that needs
a fact the port does not carry forces the port to grow.

### What the port is not

- It is not a logging framework. There are no levels, no categories, no timestamps and no global
  registry.
- It is not a dependency-injection container. The dependency is one reference passed to the
  constructor.
- It is not a generic callback bag. The methods are named after the game, and the set is closed by
  the abstract class.
- It is not a way to remove all dependencies. The core still depends on the port. The point is
  that the dependency points at a contract the core defines, not at a device the core cannot
  control.

### The cost of the seam

The seam is not free, and the numbers in Step 3 show it.

- **Dispatch.** Each `port.method(...)` call goes through a vtable. `game.o` has eight indirect
  calls where `construction-17` had none.
- **Size.** The engine grows from 24 to 32 bytes because it stores a reference to the port. The
  console adapter object is 8 bytes: one vtable pointer and no state.
- **Files.** Five new files and 260 new lines, most of them the port declaration and the two
  adapters.
- **A lifetime contract.** The engine stores `TranscriptPort &`, so the adapter must outlive the
  engine. `game.cpp` declares the adapter first and the engine second, which gives the right
  destruction order. A pointer would allow a null adapter and a later rebind; a reference makes
  the contract explicit and removes those two failure modes.
- **A change contract.** A new event changes the abstract class, and the compiler then refuses to
  build every adapter that has not answered it. That is the point of Step 5.

The baseline also deletes the move constructor and move assignment that `construction-17`
defaulted. A moved engine would still refer to the same adapter, so this is a choice rather than a
requirement of the pattern: the wiring point stays fixed for the whole life of the engine.

## Before you start

- [`construction-08`](../construction-08-modular/lesson.md) — modular programming. The seam in this
  lesson is a module boundary, and the lesson is about the direction of the dependency across it.
- [`construction-17`](../construction-17-stl/lesson.md) — the immediate baseline and the left side
  of the diff in Step 6. Its `code/` directory is the artifact this lesson starts from, even though
  the catalog lists `construction-08` as the formal prerequisite.
- [`construction-12`](../construction-12-polymorphism/lesson.md) — optional context. The port uses
  `virtual` and `override`. You do not need to design a class hierarchy from scratch, but you need
  to read `virtual void f() = 0;` and know that the call is resolved at run time.
- A C++20 compiler (`g++` or `clang++`). `cmake` is optional; `run.sh` falls back to a direct
  compile when CMake is absent.

You do not need to know the hexagonal architecture literature, and you do not need a framework. The
baseline is one abstract class, two subclasses and one reference member.

### What you need

- The hexagonal baseline:
  [`code/main.cpp`](code/main.cpp), [`code/game.h`](code/game.h),
  [`code/game.cpp`](code/game.cpp), [`code/game_engine.h`](code/game_engine.h),
  [`code/transcript_port.h`](code/transcript_port.h),
  [`code/console_transcript.h`](code/console_transcript.h),
  [`code/console_transcript.cpp`](code/console_transcript.cpp),
  [`code/recording_transcript.h`](code/recording_transcript.h),
  [`code/adapter_probe.cpp`](code/adapter_probe.cpp), [`code/board.h`](code/board.h),
  [`code/move_generator.h`](code/move_generator.h), [`code/knight.h`](code/knight.h),
  [`code/knight.cpp`](code/knight.cpp), [`code/position.h`](code/position.h),
  [`code/position.cpp`](code/position.cpp), [`code/random_sequence.h`](code/random_sequence.h),
  [`code/random_sequence.cpp`](code/random_sequence.cpp).
- The previous baseline, for the diff:
  [`code/game_engine.h`](../construction-17-stl/code/game_engine.h),
  [`code/game.cpp`](../construction-17-stl/code/game.cpp), and their ten companions in
  [`../construction-17-stl/code/`](../construction-17-stl/code/).
- Build scripts and notes: [`run.sh`](run.sh), [`CMakeLists.txt`](CMakeLists.txt) and
  [`README.md`](README.md).
- The rules: [`common/spec/game-rules.md`](../../../common/spec/game-rules.md) — §1 fixes the board
  and its 1-based numbering, §4 fixes the knight move order, §8 fixes when a cell becomes visited.
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
test -f code/transcript_port.h &&
test -f code/console_transcript.h &&
test -f code/console_transcript.cpp &&
test -f code/recording_transcript.h &&
test -f code/adapter_probe.cpp &&
test -f code/board.h &&
test -f code/move_generator.h &&
test -f code/knight.h &&
test -f code/knight.cpp &&
test -f code/position.h &&
test -f code/position.cpp &&
test -f code/random_sequence.h &&
test -f code/random_sequence.cpp &&
test -f ../construction-17-stl/code/game_engine.h &&
test -f ../construction-17-stl/code/game.cpp &&
test -f ../../../common/traces/seed-0001.txt
```

If a file is missing, the comparison has no second side. Record the missing file in `EVIDENCE.md`
§11 and return when it is available. A count against a file you have not read is not evidence.

The 8×8 baseline reproduces all 20 traces in `common/traces/` byte for byte, before you change
anything. That is the behavioral contract, and this lesson does not touch it.

## Two sittings

This lesson is long by design. Split it into two sittings:

- **Sitting 1 — Core (Steps 1–3, about 39 min).** Stop after Step 3, when the dependency arrow and
  the port are visible.
- **Sitting 2 — Extension (Steps 4–8, about 53 min).** Start only after Sitting 1. You will
  prove the boundary with the change test and the probe.

## Step 1 — Inventory the boundary (~12 min)

Start with the count that the acceptance is about:

```bash
grep -rn 'printf' code/ | wc -l
```

```text
26
```

Twenty of those lines are in `console_transcript.cpp`, five in `main.cpp` (usage errors on
`stderr`) and one in `adapter_probe.cpp`. None is in `game_engine.h`, `game.h` or `game.cpp`.

Now the output header:

```bash
grep -rn 'cstdio' code/
```

```text
code/console_transcript.cpp:3:#include <cstdio>
code/main.cpp:5:#include <cstdio>
code/adapter_probe.cpp:4:#include <cstdio>
```

Three files include it. The core does not. `game.cpp` does not print either; it only wires the
adapter.

Now the port and its implementations:

```bash
grep -c 'virtual void' code/transcript_port.h
grep -rn 'override' code/ | wc -l
grep -rn 'console_transcript.h' code/
grep -rn 'game_engine.h' code/
```

```text
8
16
code/console_transcript.cpp:1:#include "console_transcript.h"
code/game.cpp:3:#include "console_transcript.h"
code/game.cpp:4:#include "game_engine.h"
code/adapter_probe.cpp:1:#include "game_engine.h"
```

Eight pure virtual methods, sixteen overrides (eight per adapter), and exactly two programs that
include the core: the console program and the probe. The core does not include the console.

The port call sites inside the engine:

```bash
grep -nE 'port\.[a-z_]+\(' code/game_engine.h
```

```text
64:        port.game_header(seed, playerName, playerRoll, machineRoll, playerIsActive,
174:        port.turn_header(successfulMoveCount + 1, activeKnightName, activeKnight->row(),
192:                port.illegal_attempt(attemptedMoveNumber, destinationRow, destinationColumn,
198:                port.illegal_attempt(attemptedMoveNumber, destinationRow, destinationColumn,
204:                port.capture_attempt(attemptedMoveNumber, destinationRow, destinationColumn,
207:                port.move(successfulMoveCount, activeKnightName, activeKnight->row(),
215:            port.legal_attempt(attemptedMoveNumber, destinationRow, destinationColumn);
217:            port.move(successfulMoveCount, activeKnightName, activeKnight->row(),
227:            port.no_legal_move(activeKnightName);
243:        port.winner(winnerName.c_str(), successfulMoveCount);
```

Ten call sites for eight methods: `illegal_attempt` and `move` are each called from two branches.
The line numbers are the ones to use in `EVIDENCE.md` §2.

Counts for this baseline. The counting rule is stated so you can reproduce it:

- Files: 17.
- Headers: 10 (`board.h`, `console_transcript.h`, `game.h`, `game_engine.h`, `knight.h`,
  `move_generator.h`, `position.h`, `random_sequence.h`, `recording_transcript.h`,
  `transcript_port.h`).
- Translation units in the baseline build: 6 (`main.cpp`, `game.cpp`, `console_transcript.cpp`,
  `knight.cpp`, `position.cpp`, `random_sequence.cpp`). `adapter_probe.cpp` is a second program.
- Complete class definitions: 8 (`Board`, `GameEngine`, `Knight`, `Position`, `MoveCandidate`,
  `TranscriptPort`, `ConsoleTranscript`, `RecordingTranscript`).
- Pure virtual methods: 8.
- Overrides: 16.
- `printf`/`fprintf` lines: 26.
- `<cstdio>` includes: 3.
- Source lines: 815.
- Files byte-identical to `construction-17`: 10.

Then answer in `EVIDENCE.md` §2:

1. Which files include `<cstdio>`, and which of them is the core?
2. How many methods does the port have, and how many overrides does each adapter write?
3. Which two programs include `game_engine.h`, and which adapter does each use?
4. How many `port.` call sites are there, and why is the number larger than eight?
5. Which six print methods left `GameEngine`, and where did they go?
6. Which file is the composition root, and what does it connect?
7. Which ten files are byte-identical to `construction-17`?
8. Which event does the console adapter render but the recording adapter only counts?
9. Which parameters does the port pass as values rather than as text?
10. Which two objects must be declared in a specific order, and why?

Question 8 has a precise answer once you read both adapters: every event is rendered by the console
and counted by the recording adapter; the recording adapter also stores the winner name and the
final move count. Question 10: the adapter must be declared before the engine, because the engine
holds a reference and must be destroyed first.

## Step 2 — Predict before you run (~12 min)

Write these predictions before running anything. Do not look at the answers in Step 3 first.

### 2.1 The structure table

| Question | My prediction |
|---|---|
| Files in `construction-17` | |
| Files here | |
| Headers here | |
| Translation units in the baseline build | |
| Source lines in `construction-17` | |
| Source lines here | |
| `game_engine.h` lines, before and after | |
| Print methods in the core, before and after | |
| `<cstdio>` includes, before and after | |
| Files byte-identical | |
| New files | |
| Files changed | |

The row that is easy to get wrong is the source-line total. Five new files add 260 lines, but the
engine loses 57 and `game.cpp` gains 1. The net is not the size of the new files.

### 2.2 The adapter table

| Question | My prediction |
|---|---|
| Methods in the port | |
| Console adapter methods | |
| Recording adapter methods | |
| Console adapter `printf` lines | |
| Recording adapter output lines | |
| Console adapter `sizeof` | |
| Recording adapter `sizeof` | |
| What the recording adapter stores about the winner | |
| Which adapter is linked into the console binary | |
| Which adapter is linked into the probe | |

Predict `sizeof(ConsoleTranscript)` as one pointer: it has no data members. Predict
`sizeof(RecordingTranscript)` as larger: eight counters, a `std::string` and an integer, plus the
vtable pointer.

### 2.3 The size and text table

| Question | My prediction |
|---|---|
| `sizeof(GameEngine<8, 8>)` here | |
| `sizeof(Board<8, 8>)` here | |
| `game.o` text bytes, before and after | |
| Console adapter object text bytes | |
| Binary text bytes, before and after | |
| Binary data bytes, before and after | |
| Binary BSS bytes, before and after | |
| Indirect calls in `game.o`, before and after | |
| Seeds that match, of 20 | |

Predict the engine at 32 bytes: three `unique_ptr` members at 8 bytes each, plus one reference at
8 bytes. Predict the board at 256: the port does not touch the board layout.

### 2.4 The change test predictions

The change test is: add one event to the port, implement it in both adapters, and show that the
console transcript does not change.

| Question | My prediction |
|---|---|
| The event I will add | |
| Files I will edit | |
| Lines I will add to the core | |
| Lines I will add to the loop | |
| Console transcript changes? | |
| Seeds that match, of 20, after the change | |
| New count in the probe | |
| Build result with `-Wall -Wextra -Werror` | |

Predict the loop as zero lines. If your change touches `playTurn`, you have picked an event that
originates inside the loop; that is a different test, and it is described in Step 5.4.

## Step 3 — Run the baseline (~15 min)

First the traces, unmodified:

```bash
mkdir -p /tmp/hex-test
./run.sh --seed 1 --name Learner > /tmp/hex-test/baseline-0001.txt
diff -u ../../../common/traces/seed-0001.txt /tmp/hex-test/baseline-0001.txt
for trace in ../../../common/traces/seed-*.txt; do
  traceName="$(basename "${trace}" .txt)"
  seedText="${traceName#seed-}"
  seedNumber="$((10#${seedText}))"
  ./run.sh --seed "${seedNumber}" --name Learner | cmp - "${trace}"
done
```

One empty diff and a silent loop: 20 of 20. If `run.sh` cannot build, compile once by hand:

```bash
mkdir -p build
g++ -std=c++20 -Wall -Wextra -Werror -O2 -Icode \
  -o build/hexagonal-baseline code/main.cpp code/game.cpp \
  code/console_transcript.cpp code/knight.cpp code/position.cpp \
  code/random_sequence.cpp
```

Now the test adapter. The probe links the core with `RecordingTranscript` and does not link the
console adapter at all:

```bash
g++ -std=c++20 -O2 -Icode -o /tmp/hex-test/adapter-probe \
  code/adapter_probe.cpp code/knight.cpp code/position.cpp \
  code/random_sequence.cpp
/tmp/hex-test/adapter-probe
```

The probe prints one line. Split into fields, it reads:

| Field | Value |
|---|---|
| `headers` | 1 |
| `turns` | 22 |
| `illegal` | 15 |
| `capture` | 0 |
| `legal` | 21 |
| `moves` | 21 |
| `no_legal` | 1 |
| `winners` | 1 |
| `winner` | `machine` |
| `final_moves` | 21 |

Read the fields as a summary of seed 1:

- One game header.
- 22 turns: 21 turns with an accepted move, plus one turn with no legal move.
- 15 illegal attempts and 21 legal attempts: 36 attempts in total.
- 0 capture attempts. The game ended by trapping a knight, not by capturing one.
- 21 moves and one `no_legal_move` event.
- One winner event, and the winner is `machine`, after 21 moves.

The arithmetic closes: `15 + 21 = 36` attempts, `21 + 1 = 22` turns, and `moves = legal = 21`
because no capture occurred. If your numbers do not close, the adapter is counting a different
game.

Now the sizes. The probe goes in the scratch directory, never in the tracked files:

```bash
cat > /tmp/hex-test/sizes.cpp <<'EOF'
#include "console_transcript.h"
#include "game_engine.h"
#include "recording_transcript.h"

#include <cstdio>

int main()
{
    std::printf("sizeof(Board<8,8>)=%zu\n", sizeof(Board<8, 8>));
    std::printf("sizeof(GameEngine<8,8>)=%zu\n", sizeof(GameEngine<8, 8>));
    std::printf("sizeof(Knight)=%zu\n", sizeof(Knight));
    std::printf("sizeof(Position)=%zu\n", sizeof(Position));
    std::printf("sizeof(MoveCandidate)=%zu\n", sizeof(MoveCandidate));
    std::printf("sizeof(TranscriptPort)=%zu\n", sizeof(TranscriptPort));
    std::printf("sizeof(ConsoleTranscript)=%zu\n", sizeof(ConsoleTranscript));
    std::printf("sizeof(RecordingTranscript)=%zu\n", sizeof(RecordingTranscript));
    return 0;
}
EOF
g++ -std=c++20 -Wall -Wextra -Werror -O2 -Icode \
  -o /tmp/hex-test/sizes /tmp/hex-test/sizes.cpp \
  code/knight.cpp code/position.cpp code/console_transcript.cpp
/tmp/hex-test/sizes
```

The output on this platform:

```text
sizeof(Board<8,8>)=256
sizeof(GameEngine<8,8>)=32
sizeof(Knight)=40
sizeof(Position)=8
sizeof(MoveCandidate)=12
sizeof(TranscriptPort)=8
sizeof(ConsoleTranscript)=8
sizeof(RecordingTranscript)=80
```

The board, the knight, the position and the candidate match `construction-17`. The engine grew by
8 bytes: the reference to the port. `TranscriptPort` and `ConsoleTranscript` are one vtable pointer
each. `RecordingTranscript` is 80 bytes because it carries the counters and the winner string.

Now the object code:

```bash
g++ -std=c++20 -O2 -Icode -c code/game.cpp -o /tmp/hex-test/game20.o
g++ -std=c++20 -O2 -Icode -c code/console_transcript.cpp \
  -o /tmp/hex-test/console20.o
g++ -std=c++20 -O2 -I../construction-17-stl/code \
  -c ../construction-17-stl/code/game.cpp -o /tmp/hex-test/game17.o
size /tmp/hex-test/game17.o /tmp/hex-test/game20.o /tmp/hex-test/console20.o
```

```text
   text	   data	    bss	    dec	    hex	filename
   4656	      0	      0	   4656	   1230	/tmp/hex-test/game17.o
   4138	      0	      0	   4138	   102a	/tmp/hex-test/game20.o
   1341	      0	      0	   1341	    53d	/tmp/hex-test/console20.o
```

The core object shrinks by 518 bytes because the print code leaves it. The console adapter object
adds 1,341. Link both programs and compare the binaries:

```bash
g++ -std=c++20 -O2 -Icode -o /tmp/hex-test/bin20 \
  code/main.cpp code/game.cpp code/console_transcript.cpp \
  code/knight.cpp code/position.cpp code/random_sequence.cpp
g++ -std=c++20 -O2 -I../construction-17-stl/code -o /tmp/hex-test/bin17 \
  ../construction-17-stl/code/main.cpp ../construction-17-stl/code/game.cpp \
  ../construction-17-stl/code/knight.cpp ../construction-17-stl/code/position.cpp \
  ../construction-17-stl/code/random_sequence.cpp
size /tmp/hex-test/bin17 /tmp/hex-test/bin20
```

```text
   text	   data	    bss	    dec	    hex	filename
   9668	    692	     24	  10384	   2890	/tmp/hex-test/bin17
  10748	    876	     24	  11648	   2d80	/tmp/hex-test/bin20
```

Text grows by 1,080 bytes and data by 184. BSS does not move. The program got bigger even though
the core got smaller: the adapter code and its dispatch tables have to live somewhere.

The indirect calls are the visible cost of the seam:

```bash
objdump -d -C /tmp/hex-test/game17.o | grep -c 'call.*\*' || true
objdump -d -C /tmp/hex-test/game20.o | grep -c 'call.*\*' || true
```

```text
0
8
```

Eight indirect call instructions, where `construction-17` had none. There are ten source call
sites; the compiler merged or specialized some of them, but every distinct virtual method is still
dispatched through the vtable. That is the trade: the core no longer knows the adapter, and the
adapter call is no longer direct.

If the first diff is not empty, stop. That is a different baseline from the one this lesson
describes; record it in `EVIDENCE.md` §11 and do not adjust the prediction to match.

## Step 4 — Compare: what the boundary proves (~15 min)

Open `game_engine.h`, `transcript_port.h`, `console_transcript.cpp` and `game.cpp`. Five facts,
read against the measurements.

1. **The core has no output header.** `game_engine.h` includes `board.h`, `knight.h`,
   `move_generator.h`, `random_sequence.h` and the port, plus `<array>`, `<concepts>`,
   `<cstdint>`, `<memory>`, `<ranges>` and `<string>`. It does not include `<cstdio>`, and it
   contains no `printf`. The only output in the baseline build is in the console adapter and in
   `main.cpp`'s error path.
2. **The six print methods became eight events.** `printGameHeader`, `printTurnHeader`,
   `printIllegalAttempt`, `printCaptureAttempt`, `printLegalAttempt` and `printMove` are gone. Two
   direct `printf` sites, `no_legal_move` and `winner`, became methods too. The engine calls
   `port.*` at ten sites; the console implements eight methods.
3. **`game.cpp` is the composition root.** It constructs `ConsoleTranscript`, then
   `GameEngine<8, 8>` with a reference to it, then calls `run`. The engine never names the console.
   The probe does the same with `RecordingTranscript`.
4. **The board did not move.** `board.h` is byte-identical to `construction-17`, and
   `sizeof(Board<8, 8>)` is still 256. The port is about events, not about data layout.
5. **The seam has a dispatch cost.** Eight indirect calls, 1,080 more text bytes and 184 more data
   bytes in the linked binary. The core object is smaller; the program is larger.

### 4.1 The exact diff

Two files changed and five are new. The diff against `construction-17`:

```bash
diff -u ../construction-17-stl/code/game_engine.h code/game_engine.h
diff -u ../construction-17-stl/code/game.cpp code/game.cpp
```

| File | Added | Removed | Net | What changed |
|---|---|---|---|---|
| `game_engine.h` | 27 | 84 | -57 | Six print methods removed, one reference added, ten `port.*` calls |
| `game.cpp` | 3 | 2 | +1 | Constructs the console adapter and passes it to the engine |
| `transcript_port.h` | 35 | 0 | +35 | New abstract class with eight pure virtual methods |
| `console_transcript.h` | 29 | 0 | +29 | New adapter declaration |
| `console_transcript.cpp` | 76 | 0 | +76 | New adapter implementation, 20 `printf` lines |
| `recording_transcript.h` | 101 | 0 | +101 | New test adapter, header-only |
| `adapter_probe.cpp` | 19 | 0 | +19 | New program that runs the core with the test adapter |
| Total | 290 | 86 | +204 | Five new files, two changed |

The ten other files are byte-identical. The rules, the transcript format, the board, the move
generator, the knights, the position and the random sequence are untouched.

### 4.2 What did not change

- The transcript. All 20 traces match, byte for byte.
- The rules. Move order, capture, trap, draw and the 200-move limit are the same.
- The board layout. `board.h` is byte-identical; `sizeof(Board<8, 8>)` is 256.
- The move generator. `move_generator.h` is byte-identical, including the `transform` over `iota`.
- The attempt loop. `playTurn` has the same four outcomes and the same two ways to stop. Only the
  calls inside it changed from `printX` to `port.x`.
- The public game interface. `game.h` and `run_game` keep their signatures.
- The trace count. 20 of 20 before and after.

### 4.3 What it costs

- Five new files and 260 new lines, mostly port declaration and adapter bodies.
- Eight indirect calls in `game.o`, where there were none.
- 8 more bytes per engine for the reference.
- 1,080 more text bytes and 184 more data bytes in the linked binary.
- A lifetime rule: the adapter must outlive the engine.
- A change rule: a new event touches the port and every adapter, and the compiler enforces it.
- One more thing to read: the port declaration. A reader who wants to know what the program prints
  now reads two files instead of one.

None of these is a defect. They are the price of moving the rendering decision out of the core,
and the record should list them next to the benefit.

### 4.4 The event table

This table is the heart of the comparison. Read it against both adapters.

| Event | Core call site | `ConsoleTranscript` | `RecordingTranscript` |
|---|---|---|---|
| `game_header` | `run`, line 64 | 7 `printf` lines | `gameHeaderCount += 1` |
| `turn_header` | `playTurn`, line 174 | prefix, loop, newline | `turnHeaderCount += 1` |
| `illegal_attempt` | `playTurn`, lines 192, 198 | 1 line | `illegalAttemptCount += 1` |
| `capture_attempt` | `playTurn`, line 204 | 1 line | `captureAttemptCount += 1` |
| `legal_attempt` | `playTurn`, line 215 | 1 line | `legalAttemptCount += 1` |
| `move` | `playTurn`, lines 207, 217 | 2 or 3 lines | `moveCount += 1` |
| `no_legal_move` | `playTurn`, line 227 | 1 line | `noLegalMoveCount += 1` |
| `winner` | `playGame`, line 243 | 2 lines | count, name, final move count |

Every event is rendered by the console and counted by the recording adapter. No event is ignored
in the baseline. Step 5 adds one that the console ignores on purpose, to show that an adapter may
choose not to render an event without changing the core.

## Step 5 — Change test: add one port event (~20 min)

The acceptance names the change test: **add one port event without editing the game loop.**

The event is `game_started`. It reports the seed before the header. The console adapter implements
it as a no-op, so the transcript stays byte-identical. The recording adapter counts it. The core
emits it once, in `run`, before the loop starts.

One note on wording. The baseline README's change-test paragraph says "without touching the core";
the acceptance sentence says "without editing the game loop". This lesson tests the second, narrower
claim, because it is the one a diff can check: `playTurn` is byte-identical. The one-line call in
`run` is a core edit, and pretending otherwise would make the diff lie. If you want a
zero-core-edit variant, emit the event from `game.cpp` after `run` returns; the event then comes
from the composition root, not from the core, and proves less about the seam.

### 5.1 Make a scratch copy

Never edit the tracked `code/` directory. Copy it:

```bash
rm -rf /tmp/hex-test/change
cp -r code /tmp/hex-test/change
```

### 5.2 Add the event

In `/tmp/hex-test/change/transcript_port.h`, add one declaration after the destructor:

```cpp
virtual void game_started(std::int64_t seed) = 0;
```

In `/tmp/hex-test/change/game_engine.h`, add one line in `run`, after the rolls and before the
header:

```cpp
port.game_started(seed);
```

In `/tmp/hex-test/change/console_transcript.h`, declare the override:

```cpp
void game_started(std::int64_t seed) override;
```

In `/tmp/hex-test/change/console_transcript.cpp`, implement it as a no-op:

```cpp
void ConsoleTranscript::game_started(std::int64_t seed)
{
    (void)seed;
}
```

The `(void)seed;` is not decoration. The build uses `-Wall -Wextra -Werror`, and an unused
parameter is an error. The cast is the smallest honest way to say "this adapter receives the value
and does not use it".

In `/tmp/hex-test/change/recording_transcript.h`, add the counter and the method:

```cpp
int gameStartedCount = 0;

void game_started(std::int64_t seed) override
{
    (void)seed;
    gameStartedCount += 1;
}
```

Finally, print the new count in `/tmp/hex-test/change/adapter_probe.cpp`:

```cpp
std::printf("started=%d headers=%d turns=%d illegal=%d capture=%d legal=%d moves=%d "
            "no_legal=%d winners=%d winner=%s final_moves=%d\n",
            transcript.gameStartedCount, transcript.gameHeaderCount,
            // ... the remaining arguments are unchanged
```

The full call has one more argument than before; keep the order aligned with the format string.

### 5.3 Build and compare

```bash
g++ -std=c++20 -Wall -Wextra -Werror -O2 -I/tmp/hex-test/change \
  -o /tmp/hex-test/change-bin \
  /tmp/hex-test/change/main.cpp /tmp/hex-test/change/game.cpp \
  /tmp/hex-test/change/console_transcript.cpp /tmp/hex-test/change/knight.cpp \
  /tmp/hex-test/change/position.cpp /tmp/hex-test/change/random_sequence.cpp
for trace in ../../../common/traces/seed-*.txt; do
  traceName="$(basename "${trace}" .txt)"
  seedText="${traceName#seed-}"
  seedNumber="$((10#${seedText}))"
  /tmp/hex-test/change-bin --seed "${seedNumber}" --name Learner | cmp - "${trace}"
done
```

The loop is silent: 20 of 20. The console transcript did not change, even though the core now
emits one more event.

Build the probe against the same scratch copy:

```bash
g++ -std=c++20 -O2 -I/tmp/hex-test/change -o /tmp/hex-test/change-probe \
  /tmp/hex-test/change/adapter_probe.cpp /tmp/hex-test/change/knight.cpp \
  /tmp/hex-test/change/position.cpp /tmp/hex-test/change/random_sequence.cpp
/tmp/hex-test/change-probe
```

The new count is 1, and every other field matches the table in Step 3. The same core produced both
results; only the adapter changed.

Now verify the claim about the loop. The only core edit is one line in `run`:

```bash
diff -u code/game_engine.h /tmp/hex-test/change/game_engine.h
```

```text
@@ -61,6 +61,7 @@
         initializeRolls(playerRoll, machineRoll);

         bool playerIsActive = playerRoll > machineRoll;
+        port.game_started(seed);
         port.game_header(seed, playerName, playerRoll, machineRoll, playerIsActive,
                          playerKnight->row(), playerKnight->column(),
                          machineKnight->row(), machineKnight->column());
```

`playTurn` does not appear in the diff. The event originates before the loop, so the loop does not
need to know about it. That is the precise sense in which this event is added "without editing the
game loop".

### 5.4 What the change test proves, and what it does not

It proves four things.

1. The seam accepts a new event with one line in the core and one method in each adapter.
2. An adapter may ignore an event. The console implements `game_started` as a no-op and the
   transcript stays byte-identical.
3. A second adapter may consume the same event differently. The recording adapter counts it.
4. The game loop is untouched. The event is emitted outside `playTurn`, so the loop's four
   outcomes and two stop conditions do not change.

It does not prove that every event can be added without editing the loop. An event that originates
inside the attempt loop, for example "this attempt was the third illegal attempt in a row", needs a
call inside `playTurn`. That is the boundary this test probes: the loop is the part of the core
that knows the most, and an event that needs that knowledge must be emitted there.

It also does not prove the event is useful. A count of one `game_started` per game is not
interesting by itself. The test is about the shape of the change, not about the value of the
event.

Try the other direction if you want to see the cost: add an event that the console must render,
for example a `turn_finished` line. The transcript changes, so all 20 golden traces must change
too, and the lesson's behavioral contract no longer holds. That is why the console event in this
test is a no-op: the test is about the seam, not about changing the output format.

## Step 6 — Compare with `construction-17` (~10 min)

| Metric | construction-17 | construction-20 | Difference |
|---|---|---|---|
| Files | 12 | 17 | +5 |
| Headers | 7 | 10 | +3 |
| Translation units (baseline build) | 5 | 6 | +1 |
| Source lines | 611 | 815 | +204 |
| `game_engine.h` | 302 | 245 | -57 |
| `game.cpp` | 10 | 11 | +1 |
| Complete classes | 5 | 8 | +3 |
| Pure virtual methods | 0 | 8 | +8 |
| Overrides | 0 | 16 | +16 |
| Print methods in the core | 6 | 0 | -6 |
| `<cstdio>` includes | 2 | 3 | +1 |
| `printf`/`fprintf` lines | 25 | 26 | +1 |
| Indirect calls in `game.o` | 0 | 8 | +8 |
| `sizeof(GameEngine<8, 8>)` | 24 | 32 | +8 |
| `sizeof(Board<8, 8>)` | 256 | 256 | 0 |
| `game.o` text | 4,656 | 4,138 | -518 |
| Console adapter object text | — | 1,341 | +1,341 |
| Binary text / data | 9,668 / 692 | 10,748 / 876 | +1,080 / +184 |
| Binary BSS | 24 | 24 | 0 |
| Files byte-identical | — | 10 of 12 | — |
| Traces | 20 of 20 | 20 of 20 | 0 |

### 6.1 What it buys

- The core has no output dependency. It includes no `<cstdio>` and contains no `printf`.
- The console format lives in one file. A change to the transcript format touches
  `console_transcript.cpp`, not the engine.
- The core runs without a console. The probe runs the same `GameEngine<8, 8>` with an adapter that
  prints nothing.
- A test can observe the game through counts instead of captured stdout. The probe checks 22 turns,
  36 attempts, 21 moves, one trap and one winner without parsing text.
- The composition root chooses the adapter. `game.cpp` picks the console; `adapter_probe.cpp` picks
  the recording adapter; the engine does not change.
- The port names the events in the language of the game. That vocabulary survives a change from
  text to JSON, a GUI, or a network stream.

### 6.2 What it costs

- Five new files and 260 new lines.
- Eight indirect calls in `game.o`.
- 8 more bytes per engine for the reference.
- 1,080 more text bytes and 184 more data bytes in the linked binary.
- A lifetime rule: the adapter must outlive the engine.
- A change rule: a new event changes the abstract class and every adapter.
- One more file to read before you know what the program prints.

### 6.3 The gap: eight events, one seam

The baseline is not "the core has no output". It is "the core has no output *device*". Three
couplings remain, and they are worth naming.

1. **The port includes `move_generator.h`.** `transcript_port.h` needs `moveCandidateCount` for
   the `turn_header` parameter. The port is therefore not independent of the game's move count. A
   more neutral signature would take `std::array<int, moveCandidateCount>` or a span, but the
   constant still has to come from somewhere.
2. **The `const char *` parameters are borrowed.** `role_name()` returns a pointer into a
   `std::string` owned by a `Knight`. The pointer is valid for the duration of the call. An
   adapter that stored it would dangle. `RecordingTranscript` copies the winner name into a
   `std::string`; it does not store role names.
3. **The core still decides which events happen.** The adapter decides how they look, not whether
   they occur. If a future adapter needs a fact the port does not carry, the port grows and every
   adapter changes. That is the coupling the seam does not remove.

The precise acceptance sentence is narrower than "the program is decoupled":

> The learner can run the same core with two adapters, show the console transcript is unchanged,
> and add one port event without editing the game loop.

Same core, two adapters, one unchanged transcript, one new event outside the loop. Nothing about
zero coupling, zero cost, or every event being addable without a core edit.

## Step 7 — Explain (~5 min)

Write `EVIDENCE.md` §8. Two parts.

**First, one sentence.** Say what changed between the two baselines and what the change decides,
without using the words *port*, *adapter*, *interface*, *virtual*, *hexagonal*, *dependency*,
*better*, *worse*, *clean* or *decoupled*. If your sentence needs one of those, you have named the
mechanism rather than the decision.

**Second, one paragraph.** Answer:

- What the core decides: which events happen, when, and with which values.
- What the adapter decides: how those events become bytes, counts or anything else.
- What the port is: eight named events, values rather than text, closed by an abstract class.
- What the console adapter does: renders the exact transcript in one file.
- What the recording adapter does: counts events and stores the winner, with no output.
- What `game.cpp` does: chooses the console adapter and connects it to the engine.
- What `adapter_probe.cpp` does: chooses the recording adapter for the same engine.
- What the diff proves: five new files, two changed, ten byte-identical.
- What the sizes prove: the core shrinks by 518 text bytes; the program grows by 1,080.
- What the indirect calls prove: the seam costs dispatch.
- What the change test proves: a new event outside the loop touches the port and both adapters,
  and the transcript does not move.
- What the loop proves: an event that needs the loop's knowledge must be emitted inside it.
- What the next lesson owns: concurrency, `construction-21`.

A strong answer keeps four things apart: **who decides the event** (the core), **who decides the
rendering** (the adapter), **what the seam costs** (dispatch, size, files), and **what a run can
see** (the transcript, unchanged). A weak answer says "the code is decoupled", which is false in
both directions: the core still depends on the port, and the port still depends on a game constant.

## Step 8 — Acceptance check and record (~3 min)

The acceptance criterion for this lesson is: **same core, console and test adapters; 20 traces
pass; add one port event without editing the game loop.**

Check your record against it:

- [ ] I located the eight port methods and the sixteen overrides.
- [ ] I located the ten `port.*` call sites and explained why there are more than eight.
- [ ] I showed that `game_engine.h` includes no `<cstdio>` and contains no `printf`.
- [ ] I named `game.cpp` as the composition root and `adapter_probe.cpp` as the second program.
- [ ] I predicted the structure, adapter and size tables before measuring.
- [ ] I reproduced all 20 traces on the unmodified baseline.
- [ ] I ran the adapter probe and read the counts: 22 turns, 36 attempts, 21 moves, 1 trap.
- [ ] I measured `sizeof(GameEngine<8, 8>)` at 32 and `sizeof(Board<8, 8>)` at 256.
- [ ] I measured `game.o` text at 4,138 and the console adapter object at 1,341.
- [ ] I measured the binary at 10,748 text and 876 data bytes.
- [ ] I counted 8 indirect calls in `game.o`, against 0 in `construction-17`.
- [ ] I added `game_started` in a scratch copy and implemented it in both adapters.
- [ ] The scratch build passed with `-Wall -Wextra -Werror`.
- [ ] All 20 traces matched after the change.
- [ ] The probe printed `started=1` and every other count unchanged.
- [ ] `playTurn` was byte-identical in the scratch copy.
- [ ] I explained why an adapter may ignore an event.
- [ ] I named the three couplings that remain: the move-count include, the borrowed `const char *`
      parameters, and the core's choice of events.
- [ ] I did not edit any tracked code file.
- [ ] I completed `EVIDENCE.md` §1–§10 and the self-check in §12.

The criterion is not "the architecture is hexagonal". It is "you can run one core with two
adapters, show the transcript did not move, and add one event outside the loop".

## Self-check (binary)

- [ ] I read `game_engine.h`, `transcript_port.h`, `console_transcript.cpp` and `game.cpp`.
- [ ] I counted 8 port methods, 16 overrides and 9 `port.*` call sites.
- [ ] I showed the core includes no `<cstdio>` and contains no `printf`.
- [ ] I compared the baseline with `construction-17` and named the two changed files and five new
      ones.
- [ ] I predicted the structure, adapter and size tables before running.
- [ ] I reproduced all 20 traces on the unmodified baseline.
- [ ] I ran the probe and explained the counts arithmetically.
- [ ] I measured the engine at 32 bytes and the board at 256.
- [ ] I measured `game.o` at 4,138 text bytes, the console adapter at 1,341, and the binary at
      10,748 text and 876 data bytes.
- [ ] I counted 8 indirect calls in `game.o`.
- [ ] I added one port event in a scratch copy, implemented it in both adapters, and kept the
      transcript unchanged.
- [ ] I showed that `playTurn` did not change.
- [ ] I named the lifetime rule: the adapter outlives the engine.
- [ ] I completed `EVIDENCE.md`.

These are acceptance criteria. If a box is
unchecked, write the next smallest action in `EVIDENCE.md` §10.

## Misconceptions this lesson refutes

| Misconception | Why it fails | What to do instead |
|---|---|---|
| "The core has no dependencies." | It depends on `TranscriptPort`. | Say which dependency was reversed, not that all are gone. |
| "Hexagonal means six layers." | The baseline is one port and two adapters. | Read the source: eight methods, two subclasses. |
| "The port is a logging library." | It has no levels, categories or formatting. | Check the method names: they are game events. |
| "The adapter decides what happened." | The core decides which events occur and with which values. | Separate the event from its rendering. |
| "Virtual calls are free." | `game.o` has 8 indirect calls and the binary grows by 1,080 text bytes. | Measure the object and the binary. |
| "The core is testable because it has no `printf`." | It is testable because an adapter can observe it. | Run the probe; a test still needs an adapter. |
| "Adding an event never touches the core." | The event must be emitted somewhere; here one line in `run`. | Name the emission site. |
| "Adding an event never touches the adapters." | Every adapter must answer the new pure virtual method. | Count the files the compiler rejects. |
| "The reference member is optional." | Without it, the engine needs the adapter on every call or a pointer. | Read the constructor and the member. |
| "The recording adapter proves the transcript." | It proves counts and the winner, not bytes. | Pair it with the console diff against the traces. |
| "The console transcript is produced by the core." | The console adapter produces it from events. | Follow one event from `port.move` to `printf`. |
| "The port should pass formatted strings." | Then the core keeps the formatting decision. | Pass values; format in the adapter. |
| "The adapter can be destroyed before the engine." | The engine holds a reference; the call would dangle. | Declare the adapter first. |
| "Hexagonal needs a framework or a DI container." | Two classes and one reference are enough. | Read `game.cpp`. |
| "The board changed." | `board.h` is byte-identical and the board is still 256 bytes. | Diff the file and measure `sizeof`. |
| "The program is smaller because the core is smaller." | The binary grows by 1,080 text bytes. | Measure the linked binary, not only `game.o`. |
| "The 20 traces prove both adapters agree." | They prove the console adapter; the probe checks the recording one. | Use both checks for their own claims. |
| "An adapter may silently drop any event." | It may choose not to render one; the event still occurs. | Say "renders nothing", not "does not happen". |

## Next step

`construction-21` owns concurrency. The port is the seam where a second thread would deliver
events: a queue between the core and the adapter, with the adapter consuming on another thread.
That change would expose the lifetime rule from this lesson, because the adapter would outlive not
only the engine but also the thread that produces the events. The seam is in the right place for
that question; whether it is enough is the next lesson's problem.

See the [lesson catalog](../../../docs/guide/lesson-catalog.md) for the full path.

If either baseline was missing, do not start `construction-21`. The next contrast needs both
sides.
