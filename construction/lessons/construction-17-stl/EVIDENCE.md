# construction-17 — Evidence

> [English](EVIDENCE.md) | [Español](EVIDENCE-es.md)

> Learner-owned record. Write in English.

This record compares hand-written traversals with named algorithms. It asks what each traversal
promises, what the call site states after the change, what the compiler pays, and what a run can
see. Keep the predictions. The gap between your prediction and the observation is the result,
whether it is zero or not.

## 1. Run setup

- Date:
- Seed: `1`
- Player name: `Learner`
- Repository root:
- Compiler and version (`g++ --version` or `clang++ --version`):
- Platform and architecture:
- Baseline read on (date):
- Construction-16 record at hand? (yes / no)
- The numbers I carried from `construction-16` (source lines / `for` headers / text bytes / traces):
- Scratch directory I used:
- Did I edit any tracked file? (yes / no)

## 2. The traversals (before the run)

### 2.1 Inventory

Fill this from the source, with line numbers, not from memory.

| Traversal | File:lines | Before | After | Contract |
|---|---|---|---|---|
| Board reset | | nested `for` | | |
| Move generation | | `for` with index | | |
| Move order | | `for` with index | | |
| Attempt loop | | `for` with `continue` | | |
| Turn header | | `for` with separator | | |
| Board validity | | nested `for` with early return | | |
| Argument parsing | | `for` over `argv` | | |

### 2.2 Algorithm calls

| Call | File:line | Algorithm | Source range | Destination |
|---|---|---|---|---|
| Board reset | | | | |
| Move generation | | | | |
| Move order | | | | |

- Header that provides `fill`:
- Header that provides `transform`:
- Header that provides `views::iota`:
- Does `views::iota` store the indices? (yes / no)
- Does it allocate? (yes / no)

### 2.3 Counts

- Headers:
- Translation units:
- Complete class definitions:
- Named function bodies written by hand:
- Lambdas:
- `for` headers:
- Algorithm call sites:
- Source lines:
- Files that differ from `construction-16`:

Compared with `construction-16`:

- Headers: ______ → ______
- Translation units: ______ → ______
- Complete class definitions: ______ → ______
- Named function bodies: ______ → ______
- Lambdas: ______ → ______
- `for` headers: ______ → ______
- Algorithm call sites: ______ → ______
- Source lines: ______ → ______
- Files: ______ → ______
- `board.h` lines: ______ → ______
- `move_generator.h` lines: ______ → ______
- `game_engine.h` lines: ______ → ______
- Files that differ: ______

### 2.4 Contract questions

| Question | Answer |
|---|---|
| What does `fill` promise? | |
| What does `transform` promise? | |
| What does `views::iota` promise? | |
| Which loop headers disappeared? | |
| Which loop headers remain? | |
| Which remaining loop could become `any_of`? | |
| Which remaining loop cannot become an algorithm without hiding effects? | |
| What does the attempt loop print before deciding? | |
| How many outcomes does the attempt loop handle? | |
| How many ways can the attempt loop stop? | |
| What does the board reset clear now that the old loop did not? | |
| Which file grew the most, and by how many lines? | |

- The three traversals that changed, in words:
- The four `for` headers that disappeared, in words:
- The one traversal I would not change, and why:
- What `std::array` changed about the board, and what it did not:

## 3. Prediction (written before measuring)

### 3.1 The structure table

| Question | My prediction |
|---|---|
| `for` headers in `construction-16` | |
| `for` headers here | |
| Algorithm call sites here | |
| Source lines in `construction-16` | |
| Source lines here | |
| Files that differ | |
| Which file shrinks | |
| Which file grows the most | |
| Lambdas added | |

- The row I expect to get wrong, and why:
- Why four loop headers become three algorithm calls:
- What I expect the `grep 'for ('` count to be after the change test:

### 3.2 The size and text table

| Question | My prediction |
|---|---|
| `sizeof(Board<8, 8>)` | |
| `sizeof(GameEngine<8, 8>)` | |
| `sizeof(std::array<MoveCandidate, 8>)` | |
| `game.o` text bytes, before and after | |
| Binary text bytes, before and after | |
| Binary data bytes, before and after | |
| Heap allocations per engine, before and after | |
| Preprocessed lines for `game.cpp`, before and after | |
| Compile time for `game.cpp`, before and after | |
| Seeds that match, of 20, after the change | |

- Why I expect the allocation count to stay the same:
- Why I expect the board size to stay the same:
- The number I expect to be surprised by:
- What I expect the compile-time difference to be, and why:

### 3.3 The change test

| Question | My prediction |
|---|---|
| The traversal I will replace | |
| Its line range | |
| The algorithm I will use | |
| `for` headers after the change | |
| The transcript changes? | |
| Seeds that match, of 20, after the change | |
| The attempt loop after the change | |

- Why `printTurnHeader` is a fair target:
- Why `Board::is_valid` would be a weaker target:
- What would tell me I picked the counterexample by mistake:

## 4. Run — the baseline

### 4.1 Traces

- Build command:
- Exit status:
- Diff against `../../../common/traces/seed-0001.txt`:
- Seeds that matched, of 20, on the unmodified baseline:

### 4.2 Sizes

- `sizeof` probe output:
- `sizeof(Board<8, 8>)`:
- `sizeof(GameEngine<8, 8>)`:
- `sizeof(Knight)`:
- `sizeof(Position)`:
- `sizeof(MoveCandidate)`:
- `sizeof(std::array<MoveCandidate, 8>)`:

### 4.3 Object and binary

- `game.o` text before and after:
- Linked binary text before and after:
- Linked binary data before and after:
- Linked binary BSS before and after:
- Indirect calls in `game.o` before and after:

### 4.4 Compile cost

- Preprocessed lines for `game.cpp` before and after:
- Compile time for `game.cpp` before and after:
- Number of timing runs:
- Compiler flags used:
- What the numbers include, and what they do not:

| Observation | Predicted | Observed | Gap |
|---|---|---|---|
| `for` headers | | | |
| Algorithm call sites | | | |
| Source lines | | | |
| `sizeof(Board<8, 8>)` | | | |
| `sizeof(GameEngine<8, 8>)` | | | |
| `game.o` text | | | |
| Binary text | | | |
| Preprocessed lines | | | |
| Compile time | | | |
| Traces matched | | | |

- The number that surprised me:
- What I did next instead of adjusting the prediction:

## 5. Compare — what the algorithms prove

- `grep` command for the traversals:
- `for` headers before and after:
- `grep` command for the algorithm calls:
- Algorithm call sites, and which algorithms:
- Text bytes of `game.o` before and after:
- Text bytes of the linked binary before and after:
- What the sizes prove:
- What the 20 traces prove:
- What the traces do not prove:
- What the compile numbers prove:
- What the compile numbers do not prove:

### 5.1 The exact diff

- Command:
- Files changed:
- Added / removed / net lines:
- `board.h`: added / removed / net:
- `move_generator.h`: added / removed / net:
- `game_engine.h`: added / removed / net:
- Files byte-identical:

### 5.2 What did not change

- The transcript:
- The rules:
- The interfaces:
- The ownership model:
- The sizes:
- The allocation count:
- Indirect calls:
- What `std::array` bought without changing the size:

### 5.3 What it costs

- Include lines added:
- Preprocessed lines added:
- Compile time added:
- `game.o` text added:
- Binary text added:
- Writes per reset, before and after:
- One more thing a reader must know:

### 5.4 The reset semantics

- Storage type in `construction-16`:
- Storage type here:
- Cells cleared by the old loop:
- Cells cleared by `fill`:
- Why the transcript cannot see the difference:
- Why the difference is still worth recording:
- What an algorithm can widen without changing its name:

## 6. The change test in a scratch copy

- Scratch copy path:
- Files copied:
- File edited:
- Lines replaced:
- New algorithm:
- Tracked files edited: (yes / no)
- Build command:
- Build status with `-Wall -Wextra -Werror`:
- Seeds that matched, of 20, after the change:
- `for` headers before and after:
- Attempt loop changed? (yes / no)

### 6.1 The replacement

- The loop header that disappeared:
- The source range:
- The destination:
- The operation:
- What the lambda still computes:
- Why the separator rule needs the index:

### 6.2 What the change test proves, and what it does not

- It proves:
- It does not prove about speed:
- It does not prove about readability:
- Why it does not license changing the attempt loop:

### 6.3 The attempt-loop experiment (optional)

- Did I try an algorithm on the attempt loop? (yes / no)
- Which algorithm:
- Where the predicate had to print:
- Where the predicate had to mutate:
- Which outcome information was lost:
- What the compiler accepted:
- Why the result is worse than the loop:

### 6.4 Counts

| Metric | construction-16 | construction-17 | Difference |
|---|---|---|---|
| Source lines | | | |
| `for` headers | | | |
| Algorithm call sites | | | |
| Lambdas | | | |
| Files that differ | | | |
| `sizeof(Board<8, 8>)` | | | |
| `sizeof(GameEngine<8, 8>)` | | | |
| `game.o` text | | | |
| Binary text / data | | | |
| Preprocessed lines | | | |
| Compile time | | | |
| Traces matched | | | |

## 7. The traversal model and its limits

| Question | Answer |
|---|---|
| What does a `for` header state? | |
| What does an algorithm call state? | |
| What does `fill` hide? | |
| What does `transform` hide? | |
| What does `views::iota` avoid storing? | |
| What does the STL not remove? | |
| When is an algorithm the honest form? | |
| When is a loop the honest form? | |
| Which traversal has a pure early exit? | |
| Which traversal has effects around its early exit? | |
| Which loop is parsing rather than traversal? | |
| What does the transcript fail to observe? | |
| What does the compiler pay for the abstraction? | |
| What does the source gain for that price? | |

- What the change removes from the source:
- What it moves into a header:
- What it moves into a lambda:
- What it does not remove:
- Which later lesson owns purity and immutability:
- Which later lesson owns the data layout:

## 8. Explain

One sentence, without the words *STL*, *ranges*, *algorithm*, *loop*, *iterator*, *better*, *worse*,
*clean* or *modern*:

>

One paragraph:

- What `fill` decides:
- What `transform` decides:
- What `views::iota` decides:
- What the `for` count proves:
- What the remaining loops decide:
- What the diff proves:
- What the sizes prove:
- What the object code proves:
- What the compile numbers prove:
- What the transcript proves:
- What the attempt loop proves:
- Which later lesson owns purity and immutability:

The four things I kept apart:

## 9. Acceptance check

- I located the three algorithm call sites and their lines: (yes / no)
- I located the four `for` headers that disappeared and the five that remain: (yes / no)
- I explained why `Board::reset` now clears 256 cells instead of 64: (yes / no)
- I explained why `views::iota` does not allocate: (yes / no)
- I predicted the structure, size and text tables before measuring: (yes / no)
- I reproduced all 20 traces on the unmodified baseline: (yes / no)
- I replaced `printTurnHeader`'s loop in a scratch copy with `for_each` over `iota`: (yes / no)
- The scratch build passed with `-Wall -Wextra -Werror`: (yes / no)
- All 20 traces matched after the change: (yes / no)
- The `for` count in the scratch copy dropped from 5 to 4: (yes / no)
- I measured the board at 256 bytes and the engine at 24: (yes / no)
- I measured `game.o` text at 4,656 and the binary text at 9,668: (yes / no)
- I measured the preprocessed input and the compile time: (yes / no)
- I explained why the attempt loop stays: (yes / no)
- I named `Board::is_valid` as a traversal that could be `any_of`: (yes / no)
- I did not edit any tracked code file: (yes / no)

If I edited a tracked code file, what and why:

## 10. Reflection and next step

- What I can now explain without notes:
- What I cannot explain yet:
- One thing I would read again:
- The number I will carry to `construction-18`:
- The prediction that was furthest from the observation:
- What I expected the compiler to optimize away that it did not:
- What I expected the algorithm to cost that it did not:
- What the transcript could not tell me:
- What the compile numbers told me that the transcript could not:
- The traversal I would change next, and why:
- The traversal I would leave alone, and why:
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

- [ ] I inventoried the seven traversals with their lines.
- [ ] I counted the `for` headers before and after: 9 and 5.
- [ ] I counted the algorithm call sites and named the algorithms.
- [ ] I explained the reset widening from 64 cells to 256.
- [ ] I predicted the structure, size and text tables before measuring.
- [ ] I reproduced all 20 traces on the unmodified baseline.
- [ ] I replaced a transcript-visible traversal in a scratch copy.
- [ ] All 20 traces matched after the change.
- [ ] I measured the board, the engine, `game.o` and the binary.
- [ ] I measured the preprocessed input and the compile time.
- [ ] I separated what the traversal promises from what the compiler pays.
- [ ] I named the attempt loop as the counterexample and gave four reasons.
- [ ] I named `Board::is_valid` as a leftover that could be `any_of`.
- [ ] I separated "the transcript is unchanged" from "the change is free".
- [ ] My one-sentence explanation avoids the mechanism words and the verdict words.
- [ ] I did not edit any tracked code file.

No score. If a box is unchecked, write the next smallest action in §10.
