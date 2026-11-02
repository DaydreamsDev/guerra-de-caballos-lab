# archaeology-11 — Input and output and testability

> [English](lesson.md) | [Español](lesson-es.md)

> **Status:** drafted · **Concept:** input/output separation and testability ·
> **Prerequisite:** archaeology-09 · **Estimated time:** 60 minutes
> **Reading:** ~22 minutes
> **Lesson role:** optional · safe to skip

## Why this lesson exists

`archaeology-08` drew a module boundary and `archaeology-09` made the board an opaque type. Both
changes were about the inside of the program. This lesson is about its edge: the place where the
program meets a person, a file, or another program.

In the archive, that edge does not exist. The movement function prints:

```text
234:  int movcab(int &cf,int &cc)
262:      cout<<" 1.";//muestra si ejecuto este caso
704:      cout<<"No hay movimientos posibles ...";
```

`movcab` occupies lines 234–724, about 491 lines, and contains nineteen lines with `cout`. It moves
a knight, decides whether the game ended, and narrates both. The turn loop at 753 clears the screen
with `system("cls")` and waits for a key with `getch()`. To observe one move you must run the
program, sit through the screen clears, and press a key.

That is the defect the catalog calls `failure-012`: **input and output inside the movement logic**.
The program works. The cost is paid by anyone who wants to test it, script it, or give it a second
interface.

This lesson has a baseline that already separates the console from the core. Your job is not to
believe the claim; it is to verify it, find what it still couples, strengthen it, and prove that the
console and test paths agree. The baseline is a small bench, not the full horse game.

The product is a boundary you can demonstrate: a core that links no console, a console adapter
that renders it, a test adapter that asserts it, and a harness that compares the two instead of
announcing agreement.

## What you will be able to do

By the end of this lesson you can:

- explain what an input port, an output port and an adapter are, without using the word "layer";
- separate three levels of the boundary: the device, the format and the policy;
- prove that a translation unit contains no console dependency, with the linker and `nm`, not with a
  comment;
- read a `GameResult` and say which parts are domain data and which parts are already presentation;
- run a core from a test adapter that never links the console;
- explain why `lines.size() == 8` is a brittle assertion and what to assert instead;
- move transcript formatting out of the core without changing one byte of console output;
- repair a harness that printed `match=yes` without comparing anything;
- state the 2008 cost with line numbers: nineteen prints inside `movcab`, five `getch`, five
  `system("cls")`, and a display character threaded through domain signatures;
- name what the separation does not fix: validation, magic numbers, the missing input adapter and
  the archive itself;
- keep the archive read-only while you work.

The boundary is done when someone
else can reproduce both runs.

## The concept: the boundary to the outside world

### Ports and adapters, in one picture

A **port** is the shape of a conversation. An **adapter** is one implementation of that
conversation.

```text
 input ──► [ core: run_game ] ──► GameResult ──┬──► console adapter ──► terminal
                                                └──► test adapter    ──► assertions
```

The core does not know who called it. The console does not know how the game is played. The
test does not know how to draw a board. Each side can change without the other.

The baseline has one output port (`GameResult`), two output adapters (`console.cpp`, `test.cpp`)
and one input port (`run_game(int seed, const std::string &playerName)`). It has **no input
adapter**: the values are hard-coded in `main.cpp`. That is a real gap, and this lesson names it
instead of hiding it.

### Three levels of separation

"Separated from the console" is not one property. It is three, and they are often confused.

| Level | Question | Baseline |
|---|---|---|
| **Device** | Does the core write to a terminal? | Separated: `core.cpp` includes no `<cstdio>`, no `cout`, no `getch`. |
| **Format** | Does the core decide the text? | Not separated: `lines` holds preformatted strings such as `"move=1 position=2"`. |
| **Policy** | Does the core decide what is shown and when? | Partly: winner and move count are data; line order and wording are format. |

The first level is the one you can prove mechanically. The second is the one that bites later: a
core that returns strings is testable, but it is still the owner of the transcript. Change the
wording and you touch the core. The third is a design decision: some policy belongs in the core (the
winner is a game fact), some in the adapter (whether to clear the screen).

This table is the lesson. A learner who only removes `printf` has done level one and called it
architecture.

### What the baseline actually separates

Read the includes before you read the prose.

| File | Lines | Includes | Prints? |
|---|---:|---|---|
| `core.h` | 13 | `<string>`, `<vector>` | no |
| `core.cpp` | 26 | `core.h`, `<cstdint>`, `<string>` | no |
| `console.h` | 5 | `core.h` | no |
| `console.cpp` | 11 | `core.h`, `<cstdio>` | yes, the transcript |
| `test.cpp` | 20 | `core.h`, `<cstdio>` | yes, its own report |
| `main.cpp` | 9 | `console.h`, `core.h` | no |

The README says "`console.cpp` is the only file that prints". That is shorthand, not a fact:
`test.cpp` prints its one-line report. The precise claim is that **`console.cpp` is the only adapter
that renders the game transcript**. A test adapter is allowed to report; the core is not allowed to
print at all.

`test.cpp` includes `core.h` and links `core.cpp`. It never sees `console.cpp`. That is the
mechanical proof of testability, and Step 2 repeats it with the linker.

### Why testability is the payoff

A test is a caller without a terminal. If the core needs a terminal, the test cannot call it. If the
core returns data, the test can. That is the whole argument, and it is structural, not cultural.

`movcab` cannot be called by a test. It prints, it reads globals (`vectab`, `cab`, `jugada`), and
the turn loop waits for a key. `run_game` can be called by a test because its inputs are parameters
and
its output is a value.

Testability is not a framework you install. It is a property of the call graph: **can this function
finish without the outside world?** If the answer is no, no assertion framework will help.

### The 2008 shape, measured

| Anchor | `variant-01` | What it shows |
|---|---:|---|
| Lines containing `cout` | 62 | Printing is spread through the program. |
| `movcab` | 234–724 | One function, 491 lines. |
| Lines containing `cout` inside `movcab` | 19 | Movement and narration in the same body. |
| `getch()` | 42, 781, 806, 835, 861 | Progress is gated by a keypress. |
| `system("cls")` | 43, 784, 809, 838, 864 | The turn loop owns the screen. |
| `system("color 1f")` | 38 | The entry point owns the terminal theme. |
| `srand(time(NULL))` | 76 | The run is not reproducible. |
| `impresionorden` | 753–868 | The turn loop prints, waits and clears. |

`variant-02` adds the input side:

| Anchor | `variant-02` | What it shows |
|---|---:|---|
| `char nombre[250];` | 32 | A fixed buffer for the player name. |
| `cin>>nom.nombre;` | 46 | Input is read inside `main`, unbounded. |
| `movcab(..., char &pisadas)` | 285 | A display character is threaded into movement. |
| `ordenjuego(..., char &pisadas)` | 775 | The same display detail reaches turn order. |
| `impresionorden(char pisadas)` | 802 | The printer receives it back. |

That last group is the sharpest evidence. `pisadas` is the first character of the player's name. It
exists so the board can draw the knight with that character. It travels through `cabubicacion`,
`ordenjuego` and `movcab`, which are domain functions. A presentation detail has contaminated three
signatures. That is what "input and output mixed with logic" means in practice: not only `cout`
inside a loop, but display data inside the call graph.

### The change test

A design lesson is judged by a change. Here the change is: **add one line to the transcript**, say
`version=1`, between `start=player` and the first move.

In the baseline, the line is built in `core.cpp`. You edit the core, the console prints nine lines,
and `test.cpp` fails because it asserts `lines.size() == 8`. A presentation change broke a test.

In the separated version of Step 3, the core returns `seed`, `playerName`, `startingPlayer`, `moves`
and `winner`. The console renders the text. Add `version=1` in `console.cpp` and the test still
passes: no domain fact changed. That is the measured difference, and it is where the edit lands, not
how many lines it takes.

### The legacy constraint

`getch()` and `system("cls")` were reasonable for a console demo. The console was the product. The
program showed a board and waited so the user could read it. Printing from `movcab` avoided passing
a board and a transcript around in a small program.

The lesson is not "the code was wrong". It is "the structure made the next requirement expensive,
and the next requirement was a test". The cost appears when someone wants to call the movement rule
without a person in front of the screen.

## Before you start

Read these in full:

- [`README.md`](README.md) — the lesson card, the build command and the acceptance criterion.
- [archaeology-08](../archaeology-08-module-boundary/README.md) — the module boundary this lesson
  assumes.
- [archaeology-09](../archaeology-09-abstract-data-type/README.md) — the prerequisite: an opaque
  type and a representation swap.
- [`../../../docs/guide/archaeology-track.md`](../../../docs/guide/archaeology-track.md) — §3, the
  failure taxonomy; §5, the severity scale; §9, the Track A mapping.
- [`../../../docs/archive.md`](../../../docs/archive.md) — the pinned archive, tag and commit.

You also need:

- a local clone of the archive at tag `v1.0.0` (for the anchors; the bench runs without it);
- a C++20 compiler (`c++`, `g++` or `clang++`);
- `bash`, `nm` and `diff`;
- a scratch directory outside `original/`.

> **Rule:** the archive is read-only. Copy the file you want to change into a scratch directory;
> never edit `original/`. The baseline in this lesson is also evidence: copy it before you change
> it.

## The 60-minute path

| Min | Step | What you do | Evidence |
|---|---|---|---|
| 0–8 | Predict | Estimate the dependency graph, the symbol check and what the test asserts | Prediction table |
| 8–20 | Inspect | Read the six files and the archive anchors; run the baseline | Anchor notes and the four output lines |
| 20–35 | Separate | Prove the core links no console; move formatting into the adapter | `nm` output and a byte-identical transcript |
| 35–47 | Test | Break the old assertion, check determinism, repair the harness | Failing run, two seeds, `match=yes` that compares |
| 47–55 | Explain | Price the 2008 shape and state what the separation does not fix | Cost table and change test |
| 55–60 | Record | Fill `EVIDENCE.md`; confirm the archive is clean | Portfolio entry |

If the `nm` proof takes five minutes to explain, that explanation is the
lesson.

### The engineering check in this lesson

| Step | Where it happens | What it produces |
|---|---|---|
| **Predict** | Step 1 | A falsifiable estimate of the link graph and the assertions. |
| **Inspect** | Step 2 | The six files, the archive anchors and the baseline run. |
| **Separate** | Step 3 | A scratch copy where the core returns data and the adapter renders text. |
| **Test** | Step 4 | A test that asserts domain facts, a broken assertion reproduced, a harness that compares. |
| **Explain** | Step 5 | The 2008 bill, the two fixes and the remaining coupling. |
| **Record** | Step 6 | `EVIDENCE.md` with commands, counts and results. |

## Two sittings

This lesson is long by design. Split it into two sittings:

- **Sitting 1 — Core (Steps 1–3, about 35 min).** Stop after Step 3, when the core is separated
  from the console.
- **Sitting 2 — Extension (Steps 4–6, about 25 min).** Start only after Sitting 1. You will
  test the seam and record the evidence.

## Step 1 — Predict (8 min)

Before you open the source, answer these five questions in writing. Commit to answers you can be
wrong about.

1. How many source files does the console binary link? How many does the test binary link?
2. Does `core.o` reference any stdio or console symbol? Name the symbols you expect to find.
3. How many lines does the console print, and where does that number come from?
4. What does `test.cpp` assert, and what does it not assert?
5. If you add one line to the transcript inside the core, which test breaks and why?

A useful prediction is falsifiable. "The core is decoupled" is not. "`core.o` has no undefined
`printf`, `cout` or `getch`, and the test binary links only `core.cpp` and `test.cpp`" is.

### A worked prediction

Before reading, I would write:

- console binary: three sources — `core.cpp`, `console.cpp`, `main.cpp`;
- test binary: two sources — `core.cpp`, `test.cpp`; no console;
- `core.o`: undefined symbols for `memcpy`, `operator new`, `operator delete` and string internals,
  none for `printf`, `cout` or `getch`;
- console lines: eight, produced by `result.lines`, not by a counter in the adapter;
- test assertions: line count, winner, move count; not the text of each line, not determinism, not
  agreement with the console;
- adding a line: the core edit makes `lines.size()` nine, so the size assertion fails even though
  the game did not change.

Every claim can be checked in the next step. If your prediction says "separation is good", you have
not predicted anything yet; that is a conclusion.

## Step 2 — Inspect (12 min)

### Read the six files

```bash
cd archaeology/lessons/archaeology-11-input-output
wc -l code/*.h code/*.cpp
```

The baseline is 84 lines of C++ across six files:

| Part | Lines | What it is |
|---|---:|---|
| `core.h` | 13 | `GameResult` and the `run_game` declaration. |
| `core.cpp` | 26 | A deterministic three-move loop that builds transcript lines. |
| `console.h` | 5 | One declaration: `print_result`. |
| `console.cpp` | 11 | The only transcript renderer. |
| `test.cpp` | 20 | The test adapter: three booleans and a report. |
| `main.cpp` | 9 | The console entry point; input is hard-coded. |

Read `core.h` first. The result is:

```cpp
struct GameResult
{
    std::vector<std::string> lines;
    std::string winner;
    int moveCount;
};
```

Two fields are domain facts: `winner` and `moveCount`. One field is already presentation: `lines`.
Hold that thought; Step 3 is about it.

Read `core.cpp`. It seeds a 64-bit state, advances a linear congruential generator three times,
wraps a position modulo 8, and pushes one string per move. It includes `<cstdint>` and `<string>`.
There is no `<cstdio>`, no `printf`, no `cout`, no `getch`.

Read `console.cpp`. It loops over `result.lines` and calls `std::printf("%s\n", line.c_str())`. That
is the entire adapter. It does not know what a knight is.

Read `test.cpp`. It calls `run_game(11, "Learner")` and computes three booleans:

```cpp
bool linesMatch = result.lines.size() == 8;
bool winnerMatches = result.winner == "player";
bool movesMatch = result.moveCount == 3;
```

Read `main.cpp`. It calls `run_game(11, "Learner")` and `print_result(result)`. The input port
exists, but there is no input adapter: the seed and the name are literals.

### Prove the core has no console

Do not trust the include list. Ask the compiler and the linker.

```bash
mkdir -p build
c++ -std=c++20 -Wall -Wextra -Werror -O2 -c code/core.cpp -o build/core.o
nm -C -u build/core.o
```

Expected undefined symbols on this platform (string names abbreviated):

```text
memcpy
memmove
operator new(unsigned long)
operator delete(void*, unsigned long)
std::__cxx11::basic_string<...>::_M_dispose()
std::__cxx11::basic_string<...>::_M_replace_cold(...)
std::__throw_length_error(char const*)
__gxx_personality_v0
_Unwind_Resume
```

Now filter for the console:

```bash
nm -C -u build/core.o | grep -E 'printf|cout|getch|fwrite|puts'
```

The command prints nothing and exits 1. That is the result you want: the core object has no
undefined console symbol. `memcpy` is the standard library moving bytes; it is not a terminal.

Link the test without the console:

```bash
c++ -std=c++20 -Wall -Wextra -Werror -O2 code/core.cpp code/test.cpp -o build/core-test
./build/core-test
```

Expected:

```text
lines=8 winner=player moves=3
test=ok
```

If the console were required, this link would fail. It does not. That is a stronger proof than any
comment in a header.

### Read the harness

`run.sh` builds two binaries and prints four lines. Read lines 16–24 carefully:

```bash
consoleLines="$("${buildDirectory}/console-app" | wc -l)"
testOutput="$("${buildDirectory}/core-test")"

printf 'console_lines=%s\n' "${consoleLines}"
printf 'test_output=%s\n' "${testOutput}"
if [[ "${testOutput}" != *"test=ok"* ]]; then
    exit 1
fi
printf 'match=yes\n'
```

`match=yes` is printed unconditionally after the test passes. The script never compares
`consoleLines` with the number of lines the test saw. It compares nothing. The word "match" is a
claim, not a check.

That is not a fatal defect in a demo, but it is exactly the kind of harness that makes a suite look
green while two adapters disagree. Step 4 repairs it.

### Run the baseline

```bash
./run.sh
```

Expected output:

```text
console_lines=8
test_output=lines=8 winner=player moves=3
test=ok
match=yes
```

The script compiles with:

```text
-std=c++20 -Wall -Wextra -Werror -O2
```

`-Werror` matters: the baseline is warning-clean under the flags it ships with. If your compiler
rejects it, record the compiler and the diagnostic; do not silence the warning to make the run pass.

### Read the archive anchors

In the pinned archive at tag `v1.0.0`:

| Anchor | `variant-01` | What it shows |
|---|---:|---|
| `system("color 1f")` | 38 | The entry point owns the terminal theme. |
| `getch()` in `main` | 42 | The first screen waits for a key. |
| `system("cls")` in `main` | 43 | The first screen is cleared. |
| `while (jugada!=0)` | 45 | The game loop is the program loop. |
| `srand(time(NULL))` | 76 | Placement is not reproducible. |
| `movcab` | 234–724 | Movement and narration share one body. |
| `cout` inside `movcab` | 19 lines | Every branch narrates itself. |
| Trapped-knight messages | 704, 712 | `movcab` prints the ending. |
| `impresionorden` | 753–868 | The turn loop prints and pauses. |
| `getch()` in the turn loop | 781, 806, 835, 861 | Each turn waits for a key. |
| `system("cls")` in the turn loop | 784, 809, 838, 864 | Each turn clears the screen. |

Count the prints yourself:

```bash
grep -c 'cout' original/variant-01-baseline-2008/source/guerra-de-caballos-baseline.cpp
awk 'NR>=234 && NR<=724 && /cout/ {n++} END {print n}' \
    original/variant-01-baseline-2008/source/guerra-de-caballos-baseline.cpp
```

The answers are 62 and 19. The first number is the whole program; the second is the movement
function alone.

The other variants carry the same shape:

| Variant | Lines | Lines with `cout` | `getch` | `system("cls")` |
|---|---:|---:|---:|---:|
| `variant-01-baseline-2008` | 930 | 62 | 5 | 5 |
| `variant-02-records-2008` | 1021 | 76 | 5 | 5 |
| `variant-03-pointers-dev-2008` | 1058 | 75 | 5 | 5 |
| `variant-04-pointers-final-2008` | 1079 | 54 | 5 | 5 |

The pointer variants changed the container. They did not separate the console. That is worth
noticing: a syntax change is not an architectural change.

### Check your prediction

- The console binary links three sources; the test binary links two and never sees `console.cpp`.
- `core.o` has no undefined `printf`, `cout` or `getch`. It does have `memcpy`, allocation and
  string symbols.
- The console prints eight lines because `result.lines` has eight elements.
- The test asserts size, winner and move count. It does not assert line text, order, determinism or
  agreement with the console.
- Adding a line inside the core makes the size assertion fail.

If your prediction said the harness verifies agreement, you found the gap between the README's
"match" and the script's actual work. Write it down; it is part of the diagnosis.

## Step 3 — Separate (15 min)

### Make the scratch copy

Create a scratch directory outside `original/`. `build/` is ignored by this repository:

```bash
mkdir -p build/scratch
cd build/scratch
cp ../../code/core.h ../../code/core.cpp ../../code/console.h \
   ../../code/console.cpp ../../code/test.cpp ../../code/main.cpp .
```

From here on, edit the files in `build/scratch/`. Do not edit `code/`; it is the baseline the next
learner will run.

### Move the format out of the core

The baseline separates the device. This step separates the format. The core will return domain data;
the console will decide the text.

Replace `core.h` with:

```cpp
#pragma once

#include <string>
#include <vector>

struct Move
{
    int number;
    int position;
};

struct GameResult
{
    int seed;
    std::string playerName;
    std::string startingPlayer;
    std::vector<Move> moves;
    std::string winner;
};

GameResult run_game(int seed, const std::string &playerName);
```

Replace `core.cpp` with:

```cpp
#include "core.h"

#include <cstdint>
#include <string>

GameResult run_game(int seed, const std::string &playerName)
{
    std::uint64_t state = static_cast<std::uint64_t>(seed);
    int position = 0;
    std::vector<Move> moves;

    for (int moveNumber = 1; moveNumber <= 3; moveNumber += 1)
    {
        state = state * 6364136223846793005ULL + 1442695040888963407ULL;
        position = (position + static_cast<int>(state % 3) + 1) % 8;
        moves.push_back(Move{moveNumber, position});
    }

    return GameResult{seed, playerName, "player", moves, "player"};
}
```

Replace `console.cpp` with:

```cpp
#include "console.h"

#include <cstdio>

void print_result(const GameResult &result)
{
    std::printf("seed=%d\n", result.seed);
    std::printf("player=%s\n", result.playerName.c_str());
    std::printf("start=%s\n", result.startingPlayer.c_str());
    for (const Move &move : result.moves)
    {
        std::printf("move=%d position=%d\n", move.number, move.position);
    }
    std::printf("winner=%s\n", result.winner.c_str());
    std::printf("moves=%zu\n", result.moves.size());
}
```

`console.h` and `main.cpp` do not change.

`test.cpp` no longer compiles: it reads `lines` and `moveCount`, which the new `GameResult` does not
have. Replace it with the domain-field version. Step 4 explains each assertion.

```cpp
#include "core.h"

#include <cstdio>

int main()
{
    GameResult result = run_game(11, "Learner");
    bool seedMatches = result.seed == 11;
    bool playerMatches = result.playerName == "Learner";
    bool startMatches = result.startingPlayer == "player";
    bool movesMatch = result.moves.size() == 3;
    bool firstMoveMatches = movesMatch && result.moves[0].number == 1 &&
                            result.moves[0].position == 2;
    bool winnerMatches = result.winner == "player";
    std::printf("moves=%zu winner=%s\n", result.moves.size(), result.winner.c_str());
    if (!seedMatches || !playerMatches || !startMatches || !movesMatch ||
        !firstMoveMatches || !winnerMatches)
    {
        std::printf("test=failed\n");
        return 1;
    }
    std::printf("test=ok\n");
    return 0;
}
```

Compile and run the console binary:

```bash
c++ -std=c++20 -Wall -Wextra -Werror -O2 core.cpp console.cpp main.cpp -o console-app
./console-app
```

Expected, unchanged:

```text
seed=11
player=Learner
start=player
move=1 position=2
move=2 position=5
move=3 position=0
winner=player
moves=3
```

Compare it with the baseline:

```bash
cd ../..
./build/console-app > /tmp/console-baseline.txt
cd build/scratch
./console-app > /tmp/console-separated.txt
diff /tmp/console-baseline.txt /tmp/console-separated.txt
```

`diff` prints nothing. The transcript is byte-identical. That is the point: moving the format did
not change the output.

### What you just changed

| Before | After |
|---|---|
| `GameResult` holds `lines` | `GameResult` holds `seed`, `playerName`, `startingPlayer`, `moves`, `winner` |
| `moveCount` is a field, hard-coded to 3 | `moves.size()` derives it from the data |
| `core.cpp` builds `"move=1 position=2"` | `console.cpp` formats the string |
| The test must count lines | The test can assert fields |
| A wording change edits the core | A wording change edits the adapter |

The last two rows are the change test. The first three are the mechanism.

Note what did **not** change: the core still runs three moves, the LCG is the same, the positions
are the same, and the console output is the same. A separation that changes behavior is a rewrite,
not a separation. Keep the diff behavioral-neutral and prove it with `diff`.

### Do not touch the baseline

`code/core.h`, `code/core.cpp`, `code/console.cpp` and `code/test.cpp` stay as they are. The
baseline is the artifact that lets the next person reproduce your comparison. Your scratch copy is
the candidate.

## Step 4 — Test (12 min)

### What the shipped test asserts

`test.cpp` computes three booleans and prints a summary line plus a verdict. That is enough to catch
a missing move, a wrong winner and a wrong count. It is not enough to catch a wrong transcript.

| Assertion | Catches | Does not catch |
|---|---|---|
| `lines.size() == 8` | A missing or extra line | Wrong text, wrong order, embedded newlines |
| `winner == "player"` | A wrong winner string | A winner that is correct by accident |
| `moveCount == 3` | A wrong move count | A count that disagrees with `lines` |

### What it does not assert

- the text of any line;
- the order of the lines;
- that two runs with the same seed are identical;
- that two seeds produce different games;
- that the console output matches the test's view;
- that the input is valid.

The last two are the ones that matter for this lesson.

### Break it on purpose

In a throwaway copy of the shipped baseline, add `lines.push_back("version=1");` before the return.
Rebuild the test and run it. You will see:

```text
lines=9 winner=player moves=3
test=failed
```

In the separated scratch copy, the equivalent mistake is `moves.push_back(Move{0, 0});`. The test
then prints `moves=4 winner=player` and fails on the move count. Either way, the game did not
change. The transcript gained a line, or the move list gained a fake move, and the test failed.

That is a brittle assertion: it measures the shape of the presentation, not the behavior of the
game.

This is not an argument against assertions. It is an argument for asserting the right thing. A test
that fails when you add a harmless line teaches the team to stop adding lines, or to stop trusting
the test.

### Check determinism

Build and run the scratch test twice with the same seed:

```bash
c++ -std=c++20 -Wall -Wextra -Werror -O2 core.cpp test.cpp -o core-test
./core-test
./core-test
```

Both runs print `moves=3 winner=player` and `test=ok`. The LCG is seeded from the parameter, not
from the clock, so the result is reproducible.

In a temporary copy of `test.cpp`, change the seed from `11` to `12` and relax the seed assertion
(`bool seedMatches = true;`). Rebuild and run it. The positions change:

| Seed | Move 1 | Move 2 | Move 3 |
|---:|---:|---:|---:|
| 11 | 2 | 5 | 0 |
| 12 | 1 | 2 | 3 |

The winner and the count do not change, because this bench always plays three moves and always
declares the player the winner. That is a bench simplification, not a game rule.

### Repair the harness

The shipped `run.sh` prints `match=yes` without comparing. Fix it in the scratch copy.

The test adapter from Step 3 already prints `moves=3 winner=player`. Replace the last block of
`run.sh` with a comparison of the fields that both adapters expose:

```bash
consoleOutput="$("${buildDirectory}/console-app")"
testOutput="$("${buildDirectory}/core-test")"
consoleLines="$(printf '%s\n' "${consoleOutput}" | wc -l)"

printf 'console_lines=%s\n' "${consoleLines}"
printf 'test_output=%s\n' "${testOutput}"
if [[ "${testOutput}" != *"test=ok"* ]]; then
    exit 1
fi

consoleMoves="$(printf '%s\n' "${consoleOutput}" | sed -n 's/^moves=//p')"
consoleWinner="$(printf '%s\n' "${consoleOutput}" | sed -n 's/^winner=//p')"
testMoves="$(printf '%s\n' "${testOutput}" | sed -n 's/.*moves=\([0-9]*\).*/\1/p')"
testWinner="$(printf '%s\n' "${testOutput}" | sed -n 's/.*winner=\([a-z]*\).*/\1/p')"

if [[ "${consoleMoves}" != "${testMoves}" || "${consoleWinner}" != "${testWinner}" ]]; then
    printf 'match=no\n'
    exit 1
fi
printf 'match=yes\n'
```

Run it:

```bash
./run.sh
```

Expected:

```text
console_lines=8
test_output=moves=3 winner=player
test=ok
match=yes
```

Now `match=yes` means something: the console's `winner` and `moves` fields equal the test's fields.
If you change the console to print `moves=4`, the script prints `match=no` and exits 1. That is the
difference between a report and a check.

### The sharp check (optional)

If you want to see the shipped harness lie, use a throwaway copy of the baseline (not the separated
scratch copy). Make `main.cpp` read the name from `argv` and pass a name containing a newline:

```bash
./console-app $'Learner\ninjected' | wc -l
```

The console prints nine lines. `test.cpp` still sees `lines.size() == 8`, because its name is
`"Learner"`. The shipped `run.sh` would still print `match=yes`. The two adapters disagree and the
script says they match.

The cause is input validation: `run_game` accepts any string and copies it into the transcript. The
newline is not escaped. This is the same class as the unbounded `cin>>nom.nombre` at
`variant-02:46`, one level up: the program trusts input to have the shape the printer expects.

You do not have to fix validation in this lesson. You do have to notice that a line count is not a
contract.

## Step 5 — Explain (8 min)

### The bill in 2008

| Cost | Where | Consequence |
|---|---|---|
| 19 prints inside `movcab` | 234–724 | Movement cannot run without a console. |
| 5 `getch()` calls | 42, 781, 806, 835, 861 | Progress needs a human keypress. |
| 5 `system("cls")` calls | 43, 784, 809, 838, 864 | The turn loop owns the screen. |
| `srand(time(NULL))` | 76 | Two runs are not comparable. |
| `pisadas` threaded through domain signatures | `variant-02:285`, 775 | A display character contaminates the call graph. |
| `cin>>nom.nombre` into `char[250]` | `variant-02:32, 46` | Unbounded input, no validation. |

The first three rows are why a test cannot call the movement rule. The fourth is why a golden trace
cannot be compared. The fifth is why the domain signature lies about what it needs. The sixth is why
the input side is also a boundary problem, not only the output side.

### Two fixes, two sizes

**Minimal fix — return a result.** Extract the printing from `movcab` into a printer that takes the
board and the move, and return the resulting position and the game state. The movement function no
longer prints; the caller decides what to show. This is the shape of the baseline: `run_game`
returns a `GameResult`, and `console.cpp` prints it. It removes the console from the core and makes
the movement callable from a test.

**Structural fix — ports and adapters.** Define the result as domain data, not as text. Give the
console a renderer. Give the test an assertion adapter. Give the entry point an input adapter. Then
the format lives in one place and the policy lives where it belongs:

```text
argv / stdin ──► input adapter ──► run_game ──► GameResult ──► console adapter
                                                     └───────► test adapter
```

Track A links: `construction-20` (hexagonal: ports and adapters) is the direct rung.
`construction-08` (modular programming) is the prerequisite: without a module boundary there is no
"core" to put behind a port. `common-02` (deterministic harness) is the testing counterpart: a
seeded, repeatable run is what makes the adapter comparison meaningful.

### What the separation does not fix

- **Validation.** `run_game` accepts any name, including one with a newline. The transcript contract
  is not enforced.
- **Magic numbers.** `3` appears as the loop bound, the `moveCount`, and the `moves=3` line. The
  separated version derives the count from `moves.size()`, which removes one copy, not the rule.
- **The winner.** `"player"` is hard-coded. There is no game rule that decides it.
- **The input adapter.** The port exists; the adapter does not. `main` hard-codes `11` and
  `"Learner"`.
- **The archive.** None of this touches `movcab`. The 2008 program still prints from its movement
  function.
- **The missing tests.** One seed and three moves are a smoke test, not a suite.
- **The transcript format itself.** Even after Step 3, the console owns a format that no test pins
  byte-for-byte. If you care about the exact text, freeze it as a golden file (`common-05`).

### The tradeoff, stated fairly

An adapter costs a file, an indirection and a result type to design. A one-shot console demo does
not need it: printing from the movement function is shorter and there is no second caller. The cost
appears when a second caller arrives — a test, a script, a graphical interface, a replay tool. At
that moment the console-bound core charges the full price: you cannot call it, so you cannot test
it, so you cannot change it safely.

The lesson asks you to choose with the change test, not with taste. If your context says the program
will only ever be a console demo with a human in front of it, the 2008 shape is defensible. Write
that context down; it is part of the diagnosis.

### The change test, measured

| Change | Baseline | Separated |
|---|---|---|
| Add one transcript line | Edit `core.cpp`; `test.cpp` fails on `lines.size()` | Edit `console.cpp`; test passes |
| Change the winner wording | Edit `core.cpp` | Edit `console.cpp` |
| Add a silent adapter | New file; core still builds the strings it ignores | New file; core returns fields |
| Assert the first move's position | Parse `lines[3]` | Read `moves[0].position` |
| Run without a terminal | Yes | Yes |

The first row is the one to record. A presentation change should not fail a domain test.

## Step 6 — Record (5 min)

Fill [`EVIDENCE.md`](EVIDENCE.md):

- setup and archive pin;
- the five predictions, written before inspection;
- the link graph: three sources for the console, two for the test;
- the `nm` output: no stdio or console symbol in `core.o`;
- the baseline run and its exact four lines;
- the archive anchors: 62 `cout` lines, 19 inside `movcab`, 5 `getch`, 5 `cls`;
- the separation diff: what moved, what stayed, and the `diff` result on the transcript;
- the test: what it asserts, the broken assertion, the two seeds;
- the harness repair: the comparison that replaced the unconditional `match=yes`;
- the change test: adding a line, and where the edit lands in each version;
- the evidence boundary: what the run proves and what it does not;
- the archive integrity check.

A sentence such as "the core is decoupled" is an opinion. "`nm -C -u core.o` has no `printf`,
`cout` or `getch`; the test binary links two sources; the separated console transcript is
byte-identical under `diff`" is a diagnosis.

## The original in detail

Facts you can cite, with line numbers from `variant-01` at tag `v1.0.0`:

| Location | What is there |
|---:|---|
| 38 | `system("color 1f")` — the entry point sets the terminal theme. |
| 42 | `getch()` — the first screen waits for a key. |
| 43 | `system("cls")` — the first screen is cleared. |
| 45 | `while (jugada!=0)` — the game loop is the program loop. |
| 76 | `srand(time(NULL))` — placement is seeded from the clock. |
| 234 | `int movcab(int &cf, int &cc)` — movement takes coordinates, not a board. |
| 262, 286, 323, 346, 379, 402, 435, 458 | `cout` inside the first four move branches. |
| 491, 514, 548, 571, 604, 627, 660, 683 | `cout` inside the last four move branches. |
| 699 | `cout` for a move that could not be applied. |
| 704, 712 | Trapped-knight messages, printed from `movcab`. |
| 753 | `void impresionorden()` — the turn loop. |
| 781, 806, 835, 861 | `getch()` — each turn waits for a key. |
| 784, 809, 838, 864 | `system("cls")` — each turn clears the screen. |
| 870 | `void impresion()` — the board printer. |

`variant-02` adds input:

| Location | What is there |
|---:|---|
| 32 | `char nombre[250];` inside `struct movimiento`. |
| 45–46 | `cout<<"digite su nombre"; cin>>nom.nombre;` in `main`. |
| 48 | `pisadas2=nom.nombre[0];` — the display character is derived from the name. |
| 285 | `int movcab(int &cf,int &cc,char &pisadas)` — display data in movement. |
| 775 | `int ordenjuego(int &tirocab1,int &tirocab2,char &pisadas)`. |
| 802 | `void impresionorden(char pisadas)`. |

The catalog calls the defect "input and output inside `moverJugador`". The source function is
`movcab`; there is no `moverJugador` in `variant-01`. Cite the source name, not the shorthand.

## What the separation proves and does not prove

| Claim | Supported? | Why |
|---|---|---|
| `core.o` has no console dependency. | Yes | `nm -C -u` finds no `printf`, `cout` or `getch`. |
| The test binary runs without `console.cpp`. | Yes | It links two sources and prints `test=ok`. |
| The core is deterministic for a fixed seed. | Yes | Two runs with seed 11 are identical. |
| The core owns no presentation. | No | `lines` in the baseline are preformatted strings. |
| The console and test agree in the shipped harness. | No | `match=yes` is unconditional. |
| The repaired harness checks agreement. | Yes | It compares `winner` and `moves` and exits 1 on a mismatch. |
| The separated console output is unchanged. | Yes | `diff` prints nothing. |
| The core validates its input. | No | A name with a newline changes the printed line count. |
| The archive's `movcab` is testable now. | No | The bench does not touch the archive. |
| The bench is the full horse game. | No | It plays three moves and hard-codes the winner. |
| The separation is free. | No | It costs a result type, an adapter and a renderer. |

This table is the honesty check. A record that says "the code is now testable" without naming the
sample is overreaching. Say "the core is callable without a console; one seed and three moves are
asserted" when that is what you ran.

## Self-check (binary)

- [ ] I wrote the five predictions before opening the source.
- [ ] I named the three sources of the console binary and the two of the test binary.
- [ ] I compiled `core.cpp` alone and inspected `nm -C -u`.
- [ ] I confirmed there is no `printf`, `cout` or `getch` in `core.o`.
- [ ] I ran `./run.sh` and saw the four expected lines.
- [ ] I found the unconditional `match=yes` in `run.sh`.
- [ ] I counted 62 lines with `cout` in `variant-01` and 19 inside `movcab`.
- [ ] I counted 5 `getch` and 5 `system("cls")` calls.
- [ ] I found the `variant-02` input anchors: `char[250]`, `cin`, `pisadas`.
- [ ] I copied the baseline into `build/scratch/` before editing.
- [ ] I moved the transcript format from the core to `console.cpp`.
- [ ] The separated console transcript is byte-identical under `diff`.
- [ ] I reproduced the brittle assertion: one extra line makes the old test fail.
- [ ] I checked determinism with two runs of the same seed and two different seeds.
- [ ] I repaired the harness so `match=yes` follows an actual comparison.
- [ ] I stated that the baseline still owns the transcript format.
- [ ] I stated that there is no input adapter.
- [ ] I stated that the bench is not the full horse game.
- [ ] I linked the structural fix to `construction-20`, `construction-08` and `common-02`.
- [ ] The archive is unmodified (`git status` in the archive is clean).

If a box is not ticked, the unticked box names the next step. There is no penalty for repeating one.

## Misconceptions this lesson refutes

| Misconception | Why it is wrong | What to do instead |
|---|---|---|
| "No `printf` in the core means the core is separated." | It separates the device, not the format. | Check whether the core builds strings or returns data. |
| "A `GameResult` with strings is domain data." | The strings are already rendered. | Return fields; let the adapter render. |
| "The test proves the console and core agree." | It calls the core only; `run.sh` compares nothing. | Compare the fields both adapters expose. |
| "A line count is a behavioral assertion." | A harmless extra line fails it. | Assert domain fields and freeze text separately. |
| "Testability comes from a framework." | It comes from a call graph that finishes without the outside world. | Call the core from a test with no console linked. |
| "`getch` is just a pause." | It gates progress on a human keypress. | Inject input; do not read the keyboard in the core. |
| "`system("cls")` is a display detail." | The turn loop uses it to control flow. | Move screen control to the adapter. |
| "The pointer variants fixed the I/O coupling." | They changed the container; they still print from `movcab`. | Count the prints, not the syntax. |
| "The archive has a function called `moverJugador`." | `variant-01` names it `movcab`. | Cite the source. |
| "The bench is the game." | It plays three moves and hard-codes the winner. | State the isolation boundary. |
| "If the test passes, the output is correct." | The test checks three facts, not the transcript. | Add a golden file if the text matters. |

## Terms used here

| Term | One-line meaning |
|---|---|
| Core | The code that decides the game, with no terminal. |
| Adapter | A translator between the core and one outside world. |
| Port | The shape of a conversation the core accepts or offers. |
| Input port | The parameters a caller must supply. |
| Output port | The value the core returns. |
| Device separation | The core writes to no terminal. |
| Format separation | The core decides no text. |
| Policy separation | The core decides no presentation timing. |
| Test adapter | A caller that asserts instead of printing a transcript. |
| Brittle assertion | A test that fails on a harmless change. |
| Golden file | A frozen expected output compared byte-for-byte. |
| Determinism | The same input produces the same output. |
| Link graph | Which translation units end up in which binary. |
| Undefined symbol | A name the object file needs another unit to provide. |
| Change test | A concrete change whose touched places measure the design. |
| Evidence boundary | The line between what the run proves and what you infer. |

## What “done” means here

The **Self-check (binary)** above is the authoritative checklist. If every box is ticked, the
lesson is done for you. `meta.yml` mirrors the same criteria for tooling.

## Next step

- Continue to `archaeology-12` — naming and comments. The legacy comments narrate the obvious and
  sometimes lie; the `cout` lines are part of that narration.
- If the input side felt unfinished, add an `argv` adapter in the scratch copy and pass the seed and
  the name as arguments. Count what changes: `main.cpp` only, if the port is honest.
- If the transcript matters byte-for-byte, freeze it as a golden file (`common-05`) and compare the
  console output, not the line count.
- If you disagree with the severity, write the context that changes it. A console demo and a
  maintained product do not price the same coupling.

## References

- [`README.md`](README.md) — the lesson card, the build command and the acceptance criterion.
- [`code/core.h`](code/core.h) — the result type and the entry point.
- [`code/core.cpp`](code/core.cpp) — the core; no console.
- [`code/console.cpp`](code/console.cpp) — the only transcript renderer.
- [`code/test.cpp`](code/test.cpp) — the test adapter.
- [`code/main.cpp`](code/main.cpp) — the console entry point.
- [`run.sh`](run.sh) — compiles both adapters with `-std=c++20 -Wall -Wextra -Werror -O2`.
- [`EVIDENCE.md`](EVIDENCE.md) — the portfolio record you fill in.
- [`../../../docs/guide/archaeology-track.md`](../../../docs/guide/archaeology-track.md) — §3, the
  failure taxonomy; §5, the severity scale; §9, the Track A mapping.
- [`../../../docs/guide/lesson-catalog.md`](../../../docs/guide/lesson-catalog.md) — the lesson
  sequence and the `archaeology-11` acceptance criterion.
- [`../../../docs/archive.md`](../../../docs/archive.md) — the pinned archive, tag and commit.
- [archaeology-08](../archaeology-08-module-boundary/README.md) — the module boundary.
- [archaeology-09](../archaeology-09-abstract-data-type/README.md) — the opaque type.
- [construction-20](../../../construction/lessons/construction-20-hexagonal/lesson.md) — the modern
  ports-and-adapters lesson.
- Archive: <https://github.com/DaydreamsDev/DaydreamsDev-guerra-de-caballos-archive>, tag `v1.0.0`,
  commit `284e144e41403afbf0cc44422876425cacfb2bd5`.
