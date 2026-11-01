# construction-15 — Evidence

> [English](EVIDENCE.md) | [Español](EVIDENCE-es.md)

> Learner-owned record. Write in English.

This record follows the board and the engine from a concrete 8×8 class into two class templates, and
adds a named piece contract. Keep the predictions. The gap between your prediction and the
observation is the result, whether it is zero or not.

## 1. Run setup

- Date:
- Seed: `1`
- Player name: `Learner`
- Repository root:
- Compiler and version (`g++ --version` or `clang++ --version`):
- Baseline read on (date):
- Construction-14 record at hand? (yes / no)
- The numbers I carried from `construction-14` (source lines / files / traces):
- Scratch directory I used:

## 2. The templates (before the run)

### 2.1 Header and declaration inventory

Fill this from the source, with line numbers, not from memory.

| Header | Lines | Public declarations | Private data declarations | Complete class | Incomplete type |
|---|---|---|---|---|---|
| | | | | | |
| | | | | | |
| | | | | | |

Counts:

- Headers:
- Translation units:
- Public declarations:
- Private data declarations:
- Complete class definitions:
- Incomplete types:
- Function definitions, total:
- Internal helpers (`game_engine.h`):
- Internal helper (`main.cpp`):
- Program entry points:

Compared with `construction-14`:

- Headers: ______ → ______
- Translation units: ______ → ______
- Public declarations: ______ → ______
- Private data declarations: ______ → ______
- Complete class definitions: ______ → ______
- Function definitions: ______ → ______
- Source lines: ______ → ______
- Files: ______ → ______
- `board.cpp` lines: ______ → ______
- `board.h` lines: ______ → ______
- `game.cpp` lines: ______ → ______
- `game_engine.h` lines: ______ → ______

### 2.2 Line map

`board.h`:

| Lines | Contents |
|---|---|
| | |
| | |
| | |

`game_engine.h`:

| Lines | Contents |
|---|---|
| | |
| | |
| | |

Specific lines:

- Line of the `Board` template header:
- Line of `class Board`:
- Line of `row_count()`:
- Line of `column_count()`:
- Line of the first dimension `static_assert`:
- Line of the second dimension `static_assert`:
- Line of the `visited` array:
- Line of the `GameEngine` template header:
- Line of `class GameEngine`:
- Line of the `PositionedPiece` concept:
- Line of `static_assert(PositionedPiece<Knight>)`:
- Line of the `Board<RowCount, ColumnCount>` member:
- Lines of the two `Knight` members:
- Line of `GameEngine<8, 8>`:
- Line of the `GameEngine` constructor:
- Line of `run()`:
- Line of `playTurn()`:

### 2.3 The contract and the loop

| Question | Answer |
|---|---|
| Template parameters of `Board` | |
| Template parameters of `GameEngine` | |
| Kind of parameters (type or non-type) | |
| Operations `PositionedPiece` requires | |
| Does the concept constrain a template parameter? | |
| Where is the concept checked? | |
| How many member signatures name `Knight`? | |
| Which ones? | |
| What does the engine own? | |
| What is the board storage? | |
| What is the active area? | |
| What happened to `board.cpp`? | |
| What happened to `game.cpp`? | |

- The two dimension constraints, in words:
- The concept's four requirements, in words:
- The piece type the engine uses:
- The type the concept is checked against:
- The line where the check happens:
- What the check would reject:

## 3. Prediction (written before measuring)

### 3.1 The dimension table

Seed 1, name `Learner`. Write the 8×8 and 10×10 columns before running.

| Question | My prediction for 8×8 | My prediction for 10×10 | Confidence |
|---|---|---|---|
| Initial player position | | | |
| Initial machine position | | | |
| Rolls | | | |
| Turns | | | |
| Attempts | | | |
| Winner | | | |
| Moves | | | |
| Transcript lines | | | |
| `sizeof(Board)` | | | |
| `sizeof(GameEngine)` | | | |
| Compiles? | | | |
| Same output as 8×8? | | | |

- The rows I expect to get wrong, and why:
- What I expect to change when the board grows:
- What I expect to stay the same when the board grows:

### 3.2 The change test

| Question | My prediction |
|---|---|
| Files I will add | |
| Files I will edit | |
| Lines in the new file | |
| New concepts the change requires | |
| Seeds that match, of 20, after the change | |
| 10×10 compiles? | |
| 10×10 output identical to 8×8? | |
| Tracked files I will touch | |

- The prediction I am least sure about, and why:

### 3.3 Instantiation and sizes

| Question | My prediction |
|---|---|
| Instantiations of `GameEngine` in the 8×8 game | |
| Instantiations of `GameEngine` in a unit with both sizes | |
| `nm -C both.o` symbols for `GameEngine<8, 8>`, at `-O0` | |
| The same count for `GameEngine<10, 10>` | |
| `GameEngine<8, 8>` symbols at `-O2` | |
| `sizeof(Board<8, 8>)` | |
| `sizeof(Board<10, 10>)` | |
| `sizeof(GameEngine<8, 8>)` | |
| `sizeof(GameEngine<10, 10>)` | |
| Indirect calls in `both.o` | |

- What I expect the class template to change in the object code, in one sentence:
- What I expect it not to change:

## 4. Run — baseline and second instantiation

- Baseline build command:
- Baseline exit status:
- Diff against `../../../common/traces/seed-0001.txt`:
- Seeds that matched, of 20, on the unmodified baseline:
- 10×10 source path (scratch only):
- 10×10 build command:
- 10×10 exit status:
- 8×8 transcript lines:
- 10×10 transcript lines:

Paste the 10×10 head and tail:

```text
<10×10 head and tail>
```

| Observation | Predicted | Observed | Gap |
|---|---|---|---|
| 8×8 initial player | | | |
| 8×8 initial machine | | | |
| 10×10 initial player | | | |
| 10×10 initial machine | | | |
| 8×8 turns | | | |
| 10×10 turns | | | |
| 8×8 moves | | | |
| 10×10 moves | | | |
| 8×8 transcript lines | | | |
| 10×10 transcript lines | | | |
| `sizeof(Board<8, 8>)` | | | |
| `sizeof(Board<10, 10>)` | | | |

- The line that surprised me:
- What I did next instead of adjusting the prediction:

## 5. Compare — what the class templates prove

- Output of the template `grep`:
- Output of `grep -n 'Knight' code/game_engine.h`:
- Number of lines that mention `Knight` in `game_engine.h`:
- Number of member signatures that name `Knight`:
- Which signatures:
- Number of `Knight` mentions in `game.cpp`:
- What the engine owns:
- What the board owns:
- What the concept checks:
- What the concept does not check:
- What the 20 traces prove:
- What the traces do not prove:
- What the 10×10 run proves:
- What the 10×10 run does not prove:

### 5.1 The two instantiations

- Command that produced the symbols:
- Number of `GameEngine<8, 8>` symbols at `-O0`:
- Number of `GameEngine<10, 10>` symbols at `-O0`:
- Number of `GameEngine<8, 8>` symbols at `-O2`:
- Number of `GameEngine<10, 10>` symbols at `-O2`:
- Number of `Board<8, 8>` symbols at `-O2`:
- Why the counts change:
- What `W` means in `nm` output:
- Whether the two instantiations share a runtime object:
- Whether the two instantiations share source code:

### 5.2 Code generation

- `game14.o` text:
- `game15.o` text:
- Difference:
- Linked `construction-14` text / data:
- Linked `construction-15` text / data:
- Indirect calls in `game14.o`:
- Indirect calls in `game15.o`:
- `GameEngine<8, 8>` symbols in `game15.o` at `-O2`:
- `GameEngine<8, 8>` symbols in `game15.o` at `-O0`:
- Why the absence of symbols is not a property of class templates:
- Which unit I should use to describe the cost, and why:

### 5.3 The concept probe and failure modes

- `GoodPawn` result:
- `MissingSetter` result:
- `Knight` result:
- Type without the operations: first error line and message:
- What the concept removes from the old cascade:
- What the concept does not check:
- `GameEngine<17, 8>`: first error line and message:
- `GameEngine<0, 8>`: first error line and message:
- Why the dimension check is not a concept:

## 6. The change test in a scratch copy

- Scratch copy path:
- Files added:
- Files edited:
- Lines in the new file:
- New concepts required:
- Tracked files edited: (yes / no)
- 10×10 compile status:
- 10×10 exit status:
- 10×10 output differs from 8×8? (yes / no)
- 8×8 traces after the change, of 20:

### 6.1 Add the second instantiation

- Header included:
- Engine type:
- Seed and name:
- Why `game.cpp` is not linked:
- Why `knight.cpp`, `position.cpp` and `random_sequence.cpp` are linked:

### 6.2 Counts

| Metric | 8×8 | 10×10 | Difference |
|---|---|---|---|
| Scratch source lines | | | |
| Tracked files edited | | | |
| Instantiations of `GameEngine` | | | |
| Board active cells | | | |
| Board storage bytes | | | |
| Turns (seed 1) | | | |
| Attempts (seed 1) | | | |
| Moves (seed 1) | | | |
| Transcript lines (seed 1) | | | |
| Traces matched | | | |

### 6.3 The limit of the acceptance

- What the acceptance claims:
- What it does not claim:
- The six signatures that still name `Knight`:
- What a full piece parameterization would touch:
- Which later work owns it:
- The precise acceptance sentence of this lesson:

## 7. The templates and their limits

| Question | Answer |
|---|---|
| What is generic? | |
| What is constrained? | |
| What changes with dimensions? | |
| What does not change with dimensions? | |
| What is still concrete? | |
| What does the class template buy? | |
| What does it cost in the source? | |
| What does it cost in the object code? | |
| What does the concept buy? | |
| What does the concept cost? | |
| What does the concept not check? | |
| What does the engine not cover? | |
| Can the engine move a second piece type without edits? | |

- What the class template removes from `game.cpp`:
- What it moves into `game_engine.h`:
- What it does not remove:
- Which later lesson owns lifetime:
- Which later work owns the full piece parameterization:

## 8. Explain

One sentence, without the words *template*, *generic*, *type parameter*, *instantiation*,
*compiler*, *better*, *worse*, *clean*, *flexible*, *reusable* or *elegant*:

>

One paragraph:

- What the class template fixes:
- What enforces the dimensions:
- What enforces the piece contract:
- What changes with dimensions:
- What does not change with dimensions:
- What the 20 traces prove:
- What the traces do not prove:
- What the 10×10 run proves:
- What the concept does not cover:
- What the engine does not cover:
- What the change test costs:
- Which later lesson builds on it:

The four things I kept apart:

## 9. Acceptance check

- I located both class templates and named their non-type parameters: (yes / no)
- I named the concept's four operations and found the `static_assert`: (yes / no)
- I confirmed that `GameEngine` does not take a piece type parameter: (yes / no)
- I reproduced all 20 traces on the unmodified 8×8 baseline: (yes / no)
- I built `GameEngine<10, 10>` from the same header and ran it: (yes / no)
- I saw that the 10×10 output differs from the 8×8 output: (yes / no)
- I measured the same `sizeof(Board)` and `sizeof(GameEngine)` for both sizes: (yes / no)
- I saw the two engine instantiations by name at `-O0`: (yes / no)
- I reproduced the dimension `static_assert` failure for 17×8: (yes / no)
- I reproduced the concept failure for a type without the operations: (yes / no)
- I named what the concept does not check: (yes / no)
- I named the limit: the piece type is still `Knight`: (yes / no)
- I did not edit any tracked code file: (yes / no)

If I edited a tracked code file, what and why:

## 10. Reflection and next step

- What I can now explain without notes:
- What I cannot explain yet:
- One thing I would read again:
- The number I will carry to `construction-16`:
- The row of the dimension table that surprised me most:
- What the class template decides that I expected the engine to decide:
- What the engine still decides that I expected the class template to decide:
- What the class template buys that the concrete 8×8 class could not offer:
- What it gives up:
- What the change test proved that the 20 traces could not:
- The change I would make next to test this design again:
- Next step:

## 11. If a baseline is missing or unfaithful

Fill this only if a file was absent, a build failed for an environmental reason, a diff was not
empty, or the second instantiation contradicted the documented behavior in a way you could not
explain.

- Missing file, build error, diff, or contradiction:
- Exact command output:
- What I did instead of inventing a count:
- When I will retry:

## 12. Self-check

- [ ] I inventoried every header and every definition with its file.
- [ ] I located both class templates, the concept and the dimension checks by line.
- [ ] I compared the generic function with the class templates and named what each one owns.
- [ ] I predicted the dimension table, the change test and the instantiation counts before
      measuring.
- [ ] I reproduced all 20 traces on the unmodified 8×8 baseline.
- [ ] I built and ran `GameEngine<10, 10>` from the same header.
- [ ] I confirmed that the 10×10 output differs from the 8×8 output.
- [ ] I measured that both board types are 256 bytes and both engines are 336 bytes.
- [ ] I saw the two instantiations by name at `-O0`.
- [ ] I reproduced the dimension and concept failures.
- [ ] I named what the concept does not check.
- [ ] I named the limit: the engine is generic over dimensions, not pieces.
- [ ] I separated what is generic, what is constrained, what the compiler emits and what a run
      can see.
- [ ] My one-sentence explanation avoids the mechanism words and the verdict words.
- [ ] I did not edit any tracked code file.

No score. If a box is unchecked, write the next smallest action in §10.
