# construction-19 — Evidence

> [English](EVIDENCE.md) | [Spanish](EVIDENCE-es.md)

> Learner-owned record. Write in English.

This record compares two layouts for the same board. It asks what each layout stores, where the
bytes live, how many heap requests the program makes, and what the run can see. Ownership and method
style stay fixed across the comparison; only the board's representation changes. Keep the
predictions. The gap between your prediction and the observation is the result, whether it is zero
or not.

## 1. Run setup

- Date:
- Seed range: `1` through `10000`
- Player name: `Learner`
- Repository root:
- Compiler and version (`g++ --version` or `clang++ --version`):
- Platform and architecture:
- Baselines read on (date):
- Construction-17 record at hand? (yes / no)
- The numbers I carried from `construction-17` (source lines / board bytes / engine bytes /
  allocations / text bytes / traces):
- Scratch directory I used:
- Did I edit any tracked file? (yes / no)

## 2. The two layouts (before the run)

### 2.1 Inventory

Fill this from the source, with line numbers, not from memory.

| Question | construction-17 | construction-19 |
|---|---|---|
| Board type | | |
| Visited storage | | |
| Bits per visited cell | | |
| Visited bytes | | |
| Occupant storage | | |
| Occupant bytes | | |
| Board bytes | | |
| Engine members | | |
| Engine bytes | | |
| Heap allocations per game | | |
| Board operations | | |
| Engine copy / move | | |

- File and line of the 17 board declaration:
- File and line of the 19 board declaration:
- File and line of the 17 engine members:
- File and line of the 19 engine members:
- The two files that differ between the baselines:
- The ten files that are byte-identical:
- What the ten identical files prove:

### 2.2 The fixed side

- The three engine member declarations in 17:
- The three engine member declarations in 19:
- The `make_unique` calls in 17:
- The `make_unique` calls in 19:
- The board methods in 17:
- The board methods in 19:
- Is `Board` a class in both? (yes / no)
- Is the board still owned by a `unique_ptr` in 19? (yes / no)
- Which lines prove that ownership did not change:

### 2.3 Counts

- Headers:
- Translation units (game):
- Translation units (with benchmark):
- Complete class definitions:
- Named function bodies written by hand:
- Source lines (game):
- Source lines (total):
- `board.h` lines:
- `game_engine.h` lines:
- Benchmark lines:
- Files that differ from `construction-17`:
- Files byte-identical to `construction-17`:

Compared with `construction-17`:

- Headers: ______ → ______
- Translation units (game): ______ → ______
- Complete class definitions: ______ → ______
- Named function bodies: ______ → ______
- Source lines (game): ______ → ______
- Source lines (total): ______ → ______
- `board.h` lines: ______ → ______
- `game_engine.h` lines: ______ → ______
- Files that differ: ______

### 2.4 Design-cost questions

| Question | Answer |
|---|---|
| What does the bitboard save? | |
| What does the occupant array add? | |
| Why is the 19 board larger than the 17 board? | |
| Why is the 19 engine the same size as the 17 engine? | |
| What does the engine still own, and how? | |
| What invariant does the occupant array add? | |
| Which class keeps that invariant private? | |
| What must a reader know to read `was_visited` in 19? | |
| What does the transcript fail to observe? | |

- The invariant the board must maintain on every move that continues the game:
- The methods that maintain it:
- What the bitboard gains compared with the nested array:
- What the bitboard loses compared with the nested array:

## 3. Prediction (written before measuring)

### 3.1 The layout table

| Question | My prediction |
|---|---|
| `sizeof(Board<8, 8>)` in 17 | |
| `sizeof(Board<8, 8>)` in 19 | |
| `sizeof(GameEngine<8, 8>)` in 17 | |
| `sizeof(GameEngine<8, 8>)` in 19 | |
| Heap allocations per game in 17 | |
| Heap allocations per game in 19 | |
| Total allocations for 10,000 games in 17 | |
| Total allocations for 10,000 games in 19 | |
| Files that differ | |
| Traces that match, of 20, in each | |

- The row I expect to get wrong, and why:
- What I expect `sizeof(GameEngine<8, 8>)` to include in each layout:
- Why I expect the board to grow or shrink:
- Why I expect the engine size and the allocation count to move, or not:

### 3.2 The profile table

| Question | My prediction |
|---|---|
| 10,000-game wall time in 17 | |
| 10,000-game wall time in 19 | |
| Which one is faster | |
| Difference in milliseconds | |
| `game.o` text in 17 and 19 | |
| Binary text in 17 and 19 | |
| Binary data in 17 and 19 | |
| Source lines in 17 and 19 | |

- Why I expect the wall time to move, or not:
- The number I expect to be surprised by:
- What I expect the object text to do, and why:
- What I expect the allocation count to do, and why:

### 3.3 The explanation test

| Question | My prediction |
|---|---|
| Will the board size change? | |
| Will the engine size change? | |
| Will the allocation count change? | |
| Will the wall time change? | |
| Why? | |
| What does the transcript see? | |
| What does `sizeof(GameEngine<8, 8>)` include? | |
| What does it not include? | |

- What I expect the board size to be, and why:
- What I expect the wall time to be, and why:
- What would tell me I confused "different representation" with "faster":
- What would tell me I confused "larger board" with "more total memory":

## 4. Run — traces and sizes

### 4.1 Traces

- Build command for 17:
- Build command for 19:
- Exit status for 17:
- Exit status for 19:
- Diff against `../../../common/traces/seed-0001.txt` for 17:
- Diff against `../../../common/traces/seed-0001.txt` for 19:
- Seeds that matched, of 20, in 17:
- Seeds that matched, of 20, in 19:

### 4.2 Sizes

- `sizeof` probe output for 17:
- `sizeof` probe output for 19:
- `sizeof(Board<8, 8>)` in 17:
- `sizeof(Board<8, 8>)` in 19:
- `sizeof(GameEngine<8, 8>)` in 17:
- `sizeof(GameEngine<8, 8>)` in 19:
- `sizeof(Knight)`:
- `sizeof(Position)`:
- `sizeof(MoveCandidate)`:
- `sizeof(std::array<MoveCandidate, 8>)`:

### 4.3 Object and binary

- `game.o` text in 17 and 19:
- Linked binary text in 17 and 19:
- Linked binary data in 17 and 19:
- Linked binary BSS in 17 and 19:
- What the text difference proves:
- What the data difference proves:
- What the text difference does not prove:

## 5. Profile — 10,000 games

### 5.1 Wall time

- Benchmark command for 17:
- Benchmark command for 19:
- Compiler flags:
- Number of runs per side:
- 10,000-game times for 17:
- 10,000-game times for 19:
- Range for 17:
- Range for 19:
- Which side is faster:
- Is the difference outside the run-to-run noise? (yes / no)

### 5.2 Allocations

- Allocation probe command:
- Allocations for 10,000 games in 17:
- Allocations for 10,000 games in 19:
- Allocations per game in 17:
- Allocations per game in 19:
- What the 17 count corresponds to:
- What the 19 count corresponds to:
- Why the count did not change:
- Did `std::string` allocate for the role names? (yes / no)

### 5.3 Observations

| Observation | Predicted | Observed | Gap |
|---|---|---|---|
| `sizeof(Board<8, 8>)` in 17 | | | |
| `sizeof(Board<8, 8>)` in 19 | | | |
| `sizeof(GameEngine<8, 8>)` in 17 | | | |
| `sizeof(GameEngine<8, 8>)` in 19 | | | |
| Allocations in 17 | | | |
| Allocations in 19 | | | |
| 10,000-game time in 17 | | | |
| 10,000-game time in 19 | | | |
| `game.o` text in 17 | | | |
| `game.o` text in 19 | | | |
| Traces matched in 17 | | | |
| Traces matched in 19 | | | |

- The number that surprised me:
- What I did next instead of adjusting the prediction:

## 6. Compare

### 6.1 The table

| Metric | construction-17 | construction-19 | Difference |
|---|---|---|---|
| Source lines (game) | | | |
| Source lines (total) | | | |
| `board.h` lines | | | |
| `game_engine.h` lines | | | |
| `sizeof(Board<8, 8>)` | | | |
| `sizeof(GameEngine<8, 8>)` | | | |
| `sizeof(Knight)` | | | |
| Heap allocations per game | | | |
| Heap allocations per 10,000 games | | | |
| 10,000-game wall time | | | |
| `game.o` text | | | |
| Binary text | | | |
| Binary data | | | |
| Traces | | | |

### 6.2 What moved

- The board representation:
- The board size:
- The object text:
- The source:
- Why the board grew by 32 bytes:
- What the bitboard saved:
- What the occupant array added:

### 6.3 What did not move

- The engine size:
- The allocation count:
- The ownership contract:
- The method style:
- The transcript:
- The rules:
- The random generator:
- The ten byte-identical files:
- The wall time:
- What the flat result refuses to claim:

### 6.4 What it costs

- Larger board:
- More instructions:
- Less readable cell access:
- Maintained occupant invariant:
- More source:
- No allocation win and no speed win:
- One more thing a reader must know:

## 7. Explain

One sentence, without the words *cache*, *struct-of-arrays*, *array-of-structs*, *data-oriented*,
*better*, *faster*, *slower* or *optimization*:

>

One paragraph:

- What the layout decides:
- What the bitboard decides:
- What the occupant array decides:
- What stayed fixed:
- What the sizes prove:
- What the allocation count proves:
- What the wall time proves:
- What the object code proves:
- What the transcript proves:
- What the lesson does not prove:
- Which later lesson owns ports and adapters:

The four things I kept apart:

## 8. Acceptance check

- I compared the two layouts field by field: (yes / no)
- I named the two files that differ and the ten that do not: (yes / no)
- I confirmed that ownership and method style are unchanged: (yes / no)
- I predicted the layout and profile tables before measuring: (yes / no)
- I measured `sizeof(Board<8, 8>)` at 256 and 288: (yes / no)
- I measured `sizeof(GameEngine<8, 8>)` at 24 in both: (yes / no)
- I measured 30,000 allocations in both: (yes / no)
- I ran 10,000 games on both sides and recorded the elapsed milliseconds: (yes / no)
- I ran each side at least three times: (yes / no)
- I reproduced all 20 traces on both baselines: (yes / no)
- I measured `game.o` text at 4,656 and 4,961: (yes / no)
- I measured the binary text at 9,668 and 9,972: (yes / no)
- I explained why the board grew by 32 bytes: (yes / no)
- I explained why the engine size and the allocation count did not move: (yes / no)
- I explained why the wall time did not move: (yes / no)
- I named the occupant invariant and the bitboard's readability cost: (yes / no)
- I did not edit any tracked code file: (yes / no)

If I edited a tracked code file, what and why:

## 9. Reflection and next step

- What I can now explain without notes:
- What I cannot explain yet:
- One thing I would read again:
- The number I will carry to `construction-20`:
- The prediction that was furthest from the observation:
- What I expected the compiler to optimize away that it did not:
- What I expected the layout change to buy that it did not:
- What the transcript could not tell me:
- What the board size told me that the wall time could not:
- What the wall time told me that the board size could not:
- The layout change I would try next, and why:
- The layout change I would refuse, and why:
- The change I would make next to test this design again:
- Next step:

## 10. If a baseline is missing or unfaithful

Fill this only if a file was absent, a build failed for an environmental reason, a diff was not
empty, or a measurement contradicted the documented behavior.

- Missing file, build error, diff or contradiction:
- Exact command output:
- What I did instead of inventing a count:
- When I will retry:

## 11. Self-check

- [ ] I inventoried the two layouts with file and line references.
- [ ] I confirmed that the `unique_ptr` members and the `make_unique` calls are unchanged.
- [ ] I confirmed that `Board` is a class with methods in both baselines.
- [ ] I predicted the layout and profile tables before measuring.
- [ ] I reproduced all 20 traces on both baselines.
- [ ] I measured the board, the engine, the knight and the position.
- [ ] I counted the allocations: 30,000 in both.
- [ ] I ran 10,000 games on both sides and recorded the range.
- [ ] I measured `game.o` and the linked binary.
- [ ] I separated the board size from the engine size.
- [ ] I separated the representation change from the allocation count.
- [ ] I explained why the board grew and why the engine did not.
- [ ] I explained why the wall time stayed flat.
- [ ] My one-sentence explanation avoids the mechanism words and the verdict words.
- [ ] I did not edit any tracked code file.

No score. If a box is unchecked, write the next smallest action in §9.
