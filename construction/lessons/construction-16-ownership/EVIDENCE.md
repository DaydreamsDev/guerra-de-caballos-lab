# construction-16 — Evidence

> [English](EVIDENCE.md) | [Español](EVIDENCE-es.md)

> Learner-owned record. Write in English.

This record follows the board and both knights from value members into owned resources. It asks who
allocates, who frees, and what happens when the owner is copied. Keep the predictions. The gap
between your prediction and the observation is the result, whether it is zero or not.

## 1. Run setup

- Date:
- Seed: `1`
- Player name: `Learner`
- Repository root:
- Compiler and version (`g++ --version` or `clang++ --version`):
- Baseline read on (date):
- Construction-15 record at hand? (yes / no)
- The numbers I carried from `construction-15` (engine size / allocations / traces):
- Leak detector available? (valgrind / ASan / none)
- If none, the exact reason:
- Scratch directory I used:

## 2. The owners (before the run)

### 2.1 Ownership inventory

Fill this from the source, with line numbers, not from memory.

| Member | Line | Type | What it owns | Allocation site |
|---|---|---|---|---|
| | | | | |
| | | | | |
| | | | | |

Special members:

| Member | Line | `= delete` or `= default` | What it decides |
|---|---|---|---|
| Copy constructor | | | |
| Copy assignment | | | |
| Move constructor | | | |
| Move assignment | | | |

Counts:

- Headers:
- Translation units:
- Complete class definitions:
- Function bodies written by hand:
- Special members declared:
- Private data declarations in headers:
- Public declarations:
- Source lines:

Compared with `construction-15`:

- Headers: ______ → ______
- Translation units: ______ → ______
- Complete class definitions: ______ → ______
- Function bodies: ______ → ______
- Special members declared: ______ → ______
- Public declarations: ______ → ______
- Source lines: ______ → ______
- Files: ______ → ______
- `game_engine.h` lines: ______ → ______
- Files that differ: ______

### 2.2 Line map

`game_engine.h`:

| Lines | Contents |
|---|---|
| | |
| | |
| | |
| | |

Specific lines:

- Line of `#include <memory>`:
- Line of the `GameEngine` template header:
- Line of `class GameEngine`:
- Lines of the three `make_unique` calls:
- Line of the copy constructor, deleted:
- Line of the copy assignment, deleted:
- Line of the move constructor, defaulted:
- Line of the move assignment, defaulted:
- Line of the `board` member:
- Lines of the two `Knight` members:
- Line of `board->reset()` in `run()`:
- Line of `GameEngine<8, 8>` in `game.cpp`:
- Line of `playTurn()`:
- Line of the file-scope generator state in `random_sequence.cpp`:

### 2.3 The ownership contract

| Question | Answer |
|---|---|
| Who allocates the board? | |
| Who allocates each knight? | |
| Who frees them? | |
| Where is the destructor written? | |
| How many owners can a board have? | |
| What does the compiler do with a copy attempt? | |
| What does a move leave in the source object? | |
| Does the engine still call `operator new`? | |
| Does the engine source contain a raw `new`? | |
| Does the engine source contain a raw `delete`? | |
| What do the two `delete` matches actually do? | |
| Does the initialization order match the declaration order? | |
| Why does the order matter? | |
| What does `.get()` transfer? | |

- The three owners, in words:
- The two deleted operations, in words:
- The two defaulted operations, in words:
- What `unique_ptr` guarantees, in one sentence:
- What `unique_ptr` does not guarantee, in one sentence:

## 3. Prediction (written before measuring)

### 3.1 The size table

| Question | My prediction |
|---|---|
| `sizeof(GameEngine<8, 8>)` in `construction-15` | |
| `sizeof(GameEngine<8, 8>)` here | |
| `sizeof(Board<8, 8>)` | |
| `sizeof(std::unique_ptr<Board<8, 8>>)` | |
| `sizeof(Knight)` | |
| Total owned bytes: engine plus board plus two knights | |
| Heap allocations per engine | |
| Heap frees per engine, after destruction | |
| Binary text bytes, before and after | |
| Binary data bytes, before and after | |

- The row I expect to get wrong, and why:
- Why the engine can shrink while the program does not:
- What I expect to happen to `data` and `bss`:

### 3.2 The allocation table

| Question | My prediction |
|---|---|
| `make_unique` calls in the constructor | |
| Allocations after constructing one engine | |
| Frees after that engine dies | |
| Allocations for twenty engines | |
| Frees after those twenty die | |
| Does the count depend on the seed? | |
| Does the count depend on the number of turns? | |
| Does the count depend on the board size? | |

- Where I expect the allocations to happen, and why:
- What I expect if the game traps on turn one:

### 3.3 The change test

| Question | My prediction |
|---|---|
| `std::is_copy_constructible_v<GameEngine<8, 8>>` | |
| `std::is_copy_assignable_v<GameEngine<8, 8>>` | |
| `std::is_move_constructible_v<GameEngine<8, 8>>` | |
| `std::is_move_assignable_v<GameEngine<8, 8>>` | |
| A program that copies an engine compiles? | |
| The error names which line? | |
| A moved-to engine reproduces seed 1? | |
| A moved-from engine that calls `run()` exits how? | |
| Seeds that match, of 20, after the change | |

- The prediction I am least sure about, and why:
- What a move does to the source object, in my own words:

## 4. Run — the baseline

- Build command:
- Exit status:
- Diff against `../../../common/traces/seed-0001.txt`:
- Seeds that matched, of 20, on the unmodified baseline:
- `sizeof` probe output:
- Allocation probe output for one engine:
- Allocation probe output for twenty engines:
- `nm` count for `GameEngine<8, 8>` at `-O0`:
- `nm` count for `GameEngine<8, 8>` at `-O2`:
- Constructor symbols by name:
- Is there a symbol named `GameEngine(GameEngine&&)`?
- Is there a destructor symbol?

| Observation | Predicted | Observed | Gap |
|---|---|---|---|
| `sizeof(GameEngine<8, 8>)` | | | |
| `sizeof(Board<8, 8>)` | | | |
| Allocations per engine | | | |
| Frees per engine | | | |
| Allocations for twenty | | | |
| Frees for twenty | | | |
| Traces matched | | | |

- The number that surprised me:
- What I did next instead of adjusting the prediction:

## 5. Compare — what the `unique_ptr` proves

- `grep` command for the owners:
- `grep` command for raw `new`/`delete`:
- Number of matches, and what they are:
- Undefined `operator new` / `operator delete` in `game.o`: (yes / no)
- Text bytes of `game.o` before and after:
- Text bytes of the linked binary before and after:
- Data bytes before and after:
- Indirect calls in `game.o` before and after:
- What the 20 traces prove:
- What the traces do not prove:
- What the allocation probe proves:
- What the allocation probe does not prove:

### 5.1 The relocation

- Command that produced the undefined symbols:
- Output of `nm -C game16.o | grep 'operator new\|operator delete'`:
- Why the source has no `new` and the object file does:
- Which library type calls `operator new`:
- Which library type calls `operator delete`:

### 5.2 What did not change

- The transcript:
- The rules:
- The board's interface:
- The piece contract:
- The dimension constraints:
- Indirect calls:
- Why smart pointers do not imply a vtable:

### 5.3 The copy failures

- Command for the copy-construction probe:
- First error line and message:
- Command for the copy-assignment probe:
- First error line and message:
- What the error names, and why that matters:
- What happens when lines 46–47 are commented out in a scratch copy:
- Why the rule did not change, only its address:

### 5.4 The move

- Command for the move probe:
- Result of `cmp` against seed 1:
- What the move did to the three pointers:
- Command for the moved-from probe:
- Exit status:
- What the moved-from object contains:
- What `run()` does with it:
- What the C++ rule says about a moved-from object:
- The three repairs I can name:
- Which repair this baseline uses, and whether it is deliberate:

### 5.5 The leak evidence

- Detector I tried:
- Exact output of the detector attempt:
- Evidence I used instead:
- Three structural facts:
- Two empirical facts:
- What the evidence does not prove:
- The sentence I will write instead of "no leaks, verified by ASan":
- If a detector did run, its exact output:

## 6. The change test in a scratch copy

- Scratch copy path:
- Files added:
- Files edited:
- Lines in the new file:
- New concepts required:
- Tracked files edited: (yes / no)
- Probe compile status:
- Copy-construction status:
- Copy-assignment status:
- Move status:
- Moved-to engine matches seed 1? (yes / no)
- Moved-from engine result:
- 8×8 traces after the change, of 20:

### 6.1 The probe

- Header included:
- Standard header included:
- The three assertions the acceptance names:
- The two extra assertions I added, and why:
- Why `-c` is enough:
- Why no `.cpp` companion is linked:

### 6.2 Counts

| Metric | construction-15 | construction-16 | Difference |
|---|---|---|---|
| Source lines | | | |
| Files that differ | | | |
| Special members declared | | | |
| `sizeof(GameEngine<8, 8>)` | | | |
| Allocations per engine | | | |
| Owned bytes, total | | | |
| `game.o` text | | | |
| Binary text / data | | | |
| Traces matched | | | |

### 6.3 The limit of the acceptance

- What the acceptance claims:
- What it does not claim:
- Why "movable" is not "usable after a move":
- What the shared generator state has to do with ownership:
- Which later lesson owns the generator state:
- Which later lesson owns the error contract:
- The precise acceptance sentence of this lesson:

## 7. The ownership model and its limits

| Question | Answer |
|---|---|
| Who owns the board? | |
| Who owns each knight? | |
| Who owns the generator state? | |
| What does the compiler refuse? | |
| What does the compiler allow? | |
| What does a copy of the engine mean? | |
| What does a move of the engine mean? | |
| What does a moved-from engine contain? | |
| Can two engines share a board? | |
| Can two engines share the generator? | |
| What does `unique_ptr` buy? | |
| What does it cost in the source? | |
| What does it cost in the object code? | |
| What does it cost at run time? | |
| What does the value-member version already have? | |

- What the ownership change removes from the engine:
- What it moves into a library type:
- What it does not remove:
- Which later lesson owns the shared state:
- Which later lesson owns the moved-from error:

## 8. Explain

One sentence, without the words *smart pointer*, *ownership*, *lifetime*, *RAII*, *destructor*,
*better*, *worse*, *clean*, *safe* or *modern*:

>

One paragraph:

- What the three members decide:
- What lines 46–47 decide:
- What lines 48–49 decide:
- What the `new`/`delete` grep proves:
- What the `nm` output proves:
- What the counting probe proves:
- What the traces prove:
- What the moved-from crash proves:
- What the cost is:
- What the value-member version already had:
- Which later lesson owns algorithmic abstraction:
- Which later lesson owns the shared generator:

The four things I kept apart:

## 9. Acceptance check

- I located the three owning members and their lines: (yes / no)
- I located the two deleted copy operations and the two defaulted move operations: (yes / no)
- I confirmed the raw `new`/`delete` grep returns only the two `= delete` lines: (yes / no)
- I confirmed `game.o` still references `operator new` and `operator delete`: (yes / no)
- I compiled the probe with the three `static_assert`s: (yes / no)
- I reproduced the copy-construction error and read its line: (yes / no)
- I reproduced the copy-assignment error and read its line: (yes / no)
- I moved an engine and the moved-to engine reproduced seed 1: (yes / no)
- I ran the moved-from engine and recorded the result: (yes / no)
- I counted three allocations and three frees for one engine: (yes / no)
- I counted sixty and sixty for twenty engines: (yes / no)
- I measured the engine at 24 bytes and the board at 256: (yes / no)
- I reproduced all 20 traces on the unmodified baseline: (yes / no)
- I recorded that no leak detector was available and named my evidence: (yes / no)
- I named the shared generator state and the moved-from state as gaps: (yes / no)
- I did not edit any tracked code file: (yes / no)

If I edited a tracked code file, what and why:

## 10. Reflection and next step

- What I can now explain without notes:
- What I cannot explain yet:
- One thing I would read again:
- The number I will carry to `construction-17`:
- The prediction that was furthest from the observation:
- What I expected the compiler to refuse that it allowed:
- What I expected the compiler to allow that it refused:
- What the ownership change decides that I expected the engine to decide:
- What the engine still decides that I expected the type to decide:
- What the change buys that the value members could not offer:
- What it gives up:
- What the counting probe proved that the traces could not:
- The change I would make next to test this design again:
- Next step:

## 11. If a baseline is missing or unfaithful

Fill this only if a file was absent, a build failed for an environmental reason, a diff was not
empty, a probe contradicted the documented behavior, or a leak detector was requested and could not
run.

- Missing file, build error, diff, contradiction or unavailable detector:
- Exact command output:
- What I did instead of inventing a count:
- When I will retry:

## 12. Self-check

- [ ] I inventoried the three owners and the four special members with their lines.
- [ ] I compared the ownership baseline with `construction-15` and named the eight added lines.
- [ ] I predicted the size table, the allocation table and the change test before measuring.
- [ ] I reproduced all 20 traces on the unmodified baseline.
- [ ] I compiled the probe with the three `static_assert`s.
- [ ] I reproduced both copy errors and read the line each one names.
- [ ] I moved an engine and matched seed 1 with the moved-to object.
- [ ] I ran the moved-from engine and recorded the result.
- [ ] I counted allocations and frees for one engine and for twenty.
- [ ] I measured the engine at 24 bytes and the board at 256.
- [ ] I separated structural evidence from empirical evidence.
- [ ] I recorded that no leak detector was available, without claiming one ran.
- [ ] I named the shared generator state and the moved-from state as gaps.
- [ ] I separated who owns, what the compiler refuses, what it allows and what a run can see.
- [ ] My one-sentence explanation avoids the mechanism words and the verdict words.
- [ ] I did not edit any tracked code file.

No score. If a box is unchecked, write the next smallest action in §10.
