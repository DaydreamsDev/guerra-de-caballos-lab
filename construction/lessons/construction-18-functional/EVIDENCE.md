# construction-18 — Evidence

> [English](EVIDENCE.md) | [Español](EVIDENCE-es.md)

> Learner-owned record. Write in English.

This record compares a mutable board with immutable snapshots. It asks what is a value, what is
still mutated, what the history buys, and what it costs. Keep the predictions. The gap between your
prediction and the observation is the result, whether it is zero or not.

## 1. Run setup

- Date:
- Seed: `1`
- Player name: `Learner`
- Repository root:
- Compiler and version (`g++ --version` or `clang++ --version`):
- Platform and architecture:
- Baseline read on (date):
- Construction-17 record at hand? (yes / no)
- The numbers I carried from `construction-17` (source lines / `for` headers / engine size /
  text bytes / traces):
- Scratch directory I used:
- Did I edit any tracked file? (yes / no)

## 2. The state inventory (before the run)

### 2.1 Functions and state

Fill this from the source, with line numbers, not from memory.

| Function or member | File:lines | New here? | Reads | Writes |
|---|---|---|---|---|
| `generate_move_candidates` | | | | |
| `Board` storage | | | | |
| `history` | | | | |
| `historyIndex` | | | | |
| `history_size` | | | | |
| `undo_last_move` | | | | |
| `redo_last_move` | | | | |
| `record_history` | | | | |
| `tryMove` | | | | |
| `playTurn` | | | | |

### 2.2 The history boundary

| Question | Answer |
|---|---|
| Type stored by `history` | |
| `sizeof` of one element | |
| What `undo_last_move` does to the index | |
| What it assigns | |
| What `record_history` erases | |
| Where `record_history` is called | |
| What happens to the redo branch after a new move | |
| What the history does not store | |

### 2.3 Counts

- Headers:
- Translation units:
- Complete class definitions:
- Named function bodies written by hand:
- Public declarations:
- Lambdas:
- `for` headers:
- Algorithm call sites:
- Source lines:
- Files that differ from `construction-17`:

Compared with `construction-17`:

- Headers: ______ → ______
- Translation units: ______ → ______
- Complete class definitions: ______ → ______
- Named function bodies: ______ → ______
- Public declarations: ______ → ______
- Lambdas: ______ → ______
- `for` headers: ______ → ______
- Algorithm call sites: ______ → ______
- Source lines: ______ → ______
- `game_engine.h` lines: ______ → ______
- Files that differ: ______
- Added / removed lines in the diff: ______ / ______

### 2.4 Contract questions

| Question | Answer |
|---|---|
| Which function in this baseline is pure? | |
| Which four properties prove it? | |
| Is `tryMove` pure? Why or why not? | |
| What makes `Board` a value type? | |
| Why can a snapshot not be a pointer? | |
| What does undo decide? | |
| What does redo decide? | |
| What does `record_history` decide? | |
| Which state is still mutated in place? | |
| Which three parts of the state are outside the history? | |
| Which two files did not change, and why? | |

- Purity, in my own words:
- Immutability, in my own words:
- The difference between the two, in one sentence:
- The one thing the history cannot restore:

## 3. Prediction (written before measuring)

### 3.1 The structure table

| Question | My prediction |
|---|---|
| Files that differ from `construction-17` | |
| Source lines in `construction-17` | |
| Source lines here | |
| `game_engine.h` lines, before and after | |
| Added / removed lines in the diff | |
| Named function bodies, before and after | |
| Public declarations, before and after | |
| `for` headers, before and after | |
| Algorithm call sites, before and after | |
| Lambdas, before and after | |

- The row I expect to get wrong, and why:
- Why I expect the `for` count to stay at 5:
- Why I expect the algorithm count to stay at 3:

### 3.2 The size and memory table

| Question | My prediction |
|---|---|
| `sizeof(Board<8, 8>)` | |
| `sizeof(GameEngine<8, 8>)`, before and after | |
| `sizeof(std::vector<Board<8, 8>>)` | |
| Heap allocations per game, before and after | |
| Bytes allocated, seed 1, before and after | |
| `history_size()` after seed 1 | |
| `history_size()` for the largest of the 20 seeds | |
| `game.o` text bytes, before and after | |
| Binary text bytes, before and after | |
| Binary data bytes, before and after | |
| Seeds that match, of 20 | |

- Why I expect `history_size()` to be `moves + 1`:
- Why I expect the board size to stay at 256:
- Why I expect the allocation count to grow but stay small:
- The number I expect to be surprised by:

### 3.3 The change test

| Question | My prediction |
|---|---|
| What I will add to the scratch copy | |
| Which file I will edit | |
| `sizeof(GameEngine<8, 8>)` after the change | |
| Does the board change on undo? | |
| Is the board identical after undo then redo? | |
| Seeds that match, of 20, after the change | |
| Undo successes from the end of seed 1 | |
| Redo successes from the start | |

- Why a read-only accessor is a fair way to observe the snapshot:
- Why a `const` reference is enough to prove the board changed:
- What would tell me I broke the baseline by mistake:

## 4. Run — the baseline

### 4.1 Traces

- Build command:
- Exit status:
- Diff against `../../../common/traces/seed-0001.txt`:
- Seeds that matched, of 20, on the unmodified baseline:
- Last two lines of the transcript:

### 4.2 Sizes

- `sizeof` probe output:
- `sizeof(Board<8, 8>)`:
- `sizeof(GameEngine<8, 8>)`:
- `sizeof(Knight)`:
- `sizeof(Position)`:
- `sizeof(MoveCandidate)`:
- `sizeof(std::vector<Board<8, 8>>)`:

### 4.3 The undo and redo probe

- `history_size()` after seed 1:
- Result of one `undo_last_move()`:
- Result of one `redo_last_move()`:
- Undo successes until `false`:
- Redo successes until `false`:
- What the two boundary numbers mean:
- Did the probe print the board? (yes / no / with an accessor):

### 4.4 Allocations

| Seed | `history_size()` | Allocations | Bytes allocated |
|---|---|---|---|
| 1 | | | |
| 5 | | | |
| 7 | | | |
| 11 | | | |
| 20 | | | |

- The rows in the table are a sample; the full 20-seed output is in the probe run:
- Same probe against `construction-17`:
- Live snapshots at the end of seed 1, and bytes held:
- The rule that explains the bytes:
- Worst case at `maximumMoveCount`:

### 4.5 Object code and compile cost

- `game.o` text before and after:
- Linked binary text before and after:
- Linked binary data before and after:
- Preprocessed lines for `game.cpp` before and after:
- Compile time for `game.cpp` before and after:
- Number of timing runs:
- Compiler flags used:

| Observation | Predicted | Observed | Gap |
|---|---|---|---|
| Files that differ | | | |
| Source lines | | | |
| `sizeof(GameEngine<8, 8>)` | | | |
| `history_size()`, seed 1 | | | |
| Bytes allocated, seed 1 | | | |
| `game.o` text | | | |
| Binary text | | | |
| Preprocessed lines | | | |
| Compile time | | | |
| Traces matched | | | |

- The number that surprised me:
- What I did next instead of adjusting the prediction:

## 5. Compare — what the history proves

- The diff command:
- Files changed:
- Added / removed lines:
- What the `for` and algorithm counts prove:
- What the sizes prove:
- What the allocation numbers prove:
- What the 20 traces prove:
- What the traces do not prove:
- What the probe proves:
- What the probe does not prove:

### 5.1 What did not change

- The transcript:
- The rules:
- The turn loop:
- `generate_move_candidates`:
- `board.h`:
- The traversal counts:
- The three `unique_ptr` members:

### 5.2 What it costs

- Bytes per accepted move:
- Live snapshots and bytes, seed 1:
- Bytes allocated, smallest and largest seed:
- Engine size added:
- `game.o` text added:
- Binary text and data added:
- Preprocessed lines added:
- Compile time added:
- The second copy of state that can drift:

### 5.3 The boundary

| Part of the state | In the history? | Where it lives |
|---|---|---|
| Visited cells | | |
| Player knight position | | |
| Machine knight position | | |
| Active player flag | | |
| Random sequence state | | |

- What `undo_last_move()` therefore restores:
- What a `GameState` record would need to hold:
- The size of one such record, before padding:
- Why this is a limit and not a defect:

### 5.4 Undo by lookup against undo by replay

| Question | Replay | Snapshot |
|---|---|---|
| Work to undo the last move of seed 1 | | |
| Work to undo the first move of seed 1 | | |
| Code that must be correct | | |
| Memory held, seed 1 | | |
| Memory held, worst case | | |
| Visible in the transcript | | |

- The row that decided the design for me:
- The row I would have to measure before claiming a speed win:

## 6. The change test in a scratch copy

- Scratch copy path:
- Files copied:
- File edited:
- Function added:
- Tracked files edited: (yes / no)
- Build command:
- Build status with `-Wall -Wextra -Werror`:
- `history=`, `undo=`, `redo=`:
- `undo_changed_board=`:
- `state_restored=`:
- `sizeof(GameEngine<8, 8>)` after the change:
- Seeds that matched, of 20, after the change:

### 6.1 The accessor

- Its signature:
- Why it returns a `const` reference:
- Why it adds no data member:
- What the fingerprint covers:
- What the fingerprint does not cover:

### 6.2 What the change test proves, and what it does not

- It proves:
- It does not prove about the knights:
- It does not prove about the turn flag:
- It does not prove about the redo branch:
- Why the transcript cannot see any of this:

### 6.3 The redo-branch model (optional)

- Did I model `record_history` on a sequence? (yes / no)
- The sequence before the undo:
- The index after two undos:
- The sequence after `erase` and `push_back`:
- What was discarded:
- Why the model is not a run of the engine:

### 6.4 The full-state experiment (optional)

- Did I try a `GameState` history? (yes / no)
- What I had to add to the record:
- The new `sizeof` per snapshot:
- Seeds that matched:
- What it cost that the board-only history did not:

### 6.5 Counts

| Metric | construction-17 | construction-18 | Difference |
|---|---|---|---|
| Source lines | | | |
| Named function bodies | | | |
| Public declarations | | | |
| `for` headers | | | |
| Algorithm call sites | | | |
| `sizeof(Board<8, 8>)` | | | |
| `sizeof(GameEngine<8, 8>)` | | | |
| Allocations per game | | | |
| Bytes allocated per game | | | |
| `game.o` text | | | |
| Binary text / data | | | |
| Preprocessed lines | | | |
| Compile time | | | |
| Traces matched | | | |

## 7. The purity and immutability model and its limits

| Question | Answer |
|---|---|
| What makes a function pure? | |
| Which function here is pure? | |
| What does `const` on a member function promise? | |
| What does `const` not promise about the parameters? | |
| What makes `Board` a value type? | |
| What does a copy of a `Board` share with its source? | |
| Why would a `Board *` history record nothing? | |
| What does undo read? | |
| What does undo not read? | |
| What does `record_history` discard? | |
| What is still mutated in place? | |
| What does the history not store? | |
| What does the transcript fail to observe? | |
| What does the memory measurement show? | |

- What the change removes from the source:
- What it moves into a container:
- What it does not remove:
- Which later lesson owns the data layout:
- Which later lesson owns concurrency and shared state:

## 8. Explain

One sentence, without the words *functional*, *pure*, *immutable*, *snapshot*, *history*, *better*,
*worse*, *clean* or *modern*:

>

One paragraph:

- What `generate_move_candidates` decides:
- What `Board` decides:
- What `history` decides:
- What `undo_last_move` decides:
- What `record_history` decides:
- What the diff proves:
- What the sizes prove:
- What the allocation numbers prove:
- What the traces prove:
- What the probe proves:
- What the boundary does not cover:
- Which later lesson owns the data layout:

The four things I kept apart:

## 9. Acceptance check

- I located the four new functions and the three new public declarations by line: (yes / no)
- I explained why `board.h` and `move_generator.h` did not change: (yes / no)
- I explained why a snapshot must be a value and not a pointer: (yes / no)
- I explained what `record_history` erases and why: (yes / no)
- I predicted the structure, size and memory tables before measuring: (yes / no)
- I reproduced all 20 traces on the unmodified baseline: (yes / no)
- I ran the probe and read `history=22`, `undo=1`, `redo=1`: (yes / no)
- I found the boundaries: 21 undos and 21 redos for seed 1: (yes / no)
- I measured the allocation cost across the 20 seeds: (yes / no)
- I added a read-only accessor in a scratch copy and fingerprinted the board: (yes / no)
- `undo_changed_board=1` and `state_restored=1` in the scratch build: (yes / no)
- All 20 traces matched with the accessor present: (yes / no)
- `sizeof(GameEngine<8, 8>)` was 56 before and after the accessor: (yes / no)
- I named the three parts of the state that the history does not store: (yes / no)
- I explained that undo is a value copy, not a replay: (yes / no)
- I did not edit any tracked code file: (yes / no)

If I edited a tracked code file, what and why:

## 10. Reflection and next step

- What I can now explain without notes:
- What I cannot explain yet:
- One thing I would read again:
- The number I will carry to `construction-19`:
- The prediction that was furthest from the observation:
- What I expected the compiler to optimize away that it did not:
- What I expected the history to cost that it did not:
- What the transcript could not tell me:
- What the probe told me that the transcript could not:
- The state I would put in a `GameState` record first, and why:
- The part of the state I would leave out, and why:
- The change I would make next to test this design again:
- Next step:

## 11. If a baseline is missing or unfaithful

Fill this only if a file was absent, a build failed for an environmental reason, a diff was not
empty, or a measurement contradicted the documented behavior.

- Missing file, build error, diff or contradiction:
- Exact command output:
- What I did instead of inventing a count:
- When I will retry:

## 12. Self-check

- [ ] I inventoried the ten functions and members with their lines.
- [ ] I counted the function bodies and public declarations before and after.
- [ ] I explained why `board.h` and `move_generator.h` did not change.
- [ ] I explained why the history stores values and not pointers.
- [ ] I predicted the structure, size and memory tables before measuring.
- [ ] I reproduced all 20 traces on the unmodified baseline.
- [ ] I ran the undo and redo probe and read both boundaries.
- [ ] I measured the allocations and bytes across the 20 seeds.
- [ ] I computed the worst case from `maximumMoveCount`.
- [ ] I observed a restored board from outside the engine in a scratch copy.
- [ ] All 20 traces matched after the change.
- [ ] I named the three parts of the state outside the history.
- [ ] I separated "the transcript is unchanged" from "the change is free".
- [ ] My one-sentence explanation avoids the mechanism words and the verdict words.
- [ ] I did not edit any tracked code file.

No score. If a box is unchecked, write the next smallest action in §10.
