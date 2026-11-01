# construction-14 — Evidence

> [English](EVIDENCE.md) | [Español](EVIDENCE-es.md)

> Learner-owned record. Write in English.

This record follows the eight move destinations from an inline formula into a function template:
what the generator accepts, what the loop still names, how many functions the compiler produced, and
what the design costs. Keep the predictions. The gap between your prediction and the observation is
the result, whether it is zero or not.

## 1. Run setup

- Date:
- Seed: `1`
- Player name: `Learner`
- Repository root:
- Compiler and version (`g++ --version` or `clang++ --version`):
- Baseline read on (date):
- Construction-13 record at hand? (yes / no)
- The number I carried from `construction-13` (source lines / files / traces):
- Scratch directory I used:

## 2. The generator (before the run)

### 2.1 Header and declarations

Fill this from the source, with line numbers, not from memory.

| Header | Lines | Public declarations | Private data declarations | Complete class | Incomplete type |
|---|---|---|---|---|---|
| | | | | | |

Counts:

- Headers:
- Translation units:
- Public declarations:
- Private data declarations:
- Complete class definitions:
- Incomplete types:
- Function definitions, total:
- Internal helpers (`game.cpp`):
- Internal helpers (`main.cpp`):
- Program entry points:

Compared with `construction-13`:

- Headers: ______ → ______
- Translation units: ______ → ______
- Public declarations: ______ → ______
- Private data declarations: ______ → ______
- Complete class definitions: ______ → ______
- Function definitions: ______ → ______
- Lines in the new header:
- Lines added to `game.cpp`:
- Lines removed from `game.cpp`:
- Hunks in the `game.cpp` diff:
- Files that are byte-identical to `construction-13`:

### 2.2 Line map

`move_generator.h`:

| Lines | Contents |
|---|---|
| | |

`game.cpp`, around `tryMove` and `playTurn`:

| Lines | Contents |
|---|---|
| | |

- Line of the template declaration:
- Line of the template body's opening brace:
- Line of `piece.row()`:
- Line of `piece.column()`:
- Line of the `MoveCandidate` record:
- Line of `moveCandidateCount`:
- Line of the first offset array:
- Line of the second offset array:
- Line that includes `move_generator.h` in `game.cpp`:
- Line that calls `generate_move_candidates`:
- Line that indexes `moveCandidates`:
- Line that passes the candidate to `tryMove`:
- Lines of `tryMove`:
- Lines of `playTurn`:
- Lines of `run_game`:
- Lines that construct a `Knight` in `run_game`:

### 2.3 The contract and the loop

| Question | Answer |
|---|---|
| The template's type parameter | |
| Operations the body requires | |
| Are they required on a `const` object? | |
| Does the header include `knight.h`? | |
| Does the header mention `Knight`? | |
| How many times does `game.cpp` call the generator? | |
| How many accessor call sites remain in `game.cpp`? | |
| How many helper signatures in `game.cpp` name `Knight`? | |
| What does `tryMove` receive now? | |
| What did `tryMove` receive in `construction-13`? | |
| Where are the offsets now? | |
| Where were they in `construction-13`? | |

- Number of `row` reads in `game.cpp`:
- Number of `column` reads in `game.cpp`:
- Number of `set_position` calls in `game.cpp`:
- Number of `role_name` reads in `game.cpp`:
- The sum of those four counts:
- The functions that contain them:

## 3. Prediction (written before measuring)

### 3.1 The candidate table

The knight is at `(4, 5)`. The test type is at `(1, 1)` on purpose: some candidates leave the board.

| Move | `Knight` at (4, 5) | `ScoutingPawn` at (1, 1) | Confidence |
|---|---|---|---|
| 1 | | | |
| 2 | | | |
| 3 | | | |
| 4 | | | |
| 5 | | | |
| 6 | | | |
| 7 | | | |
| 8 | | | |

- The rows I expect to get wrong, and why:
- What I expect the generator to do with off-board destinations:
- Where I expect the rejection to happen:

### 3.2 The change test

| Question | My prediction |
|---|---|
| Files I will add | |
| Files I will edit | |
| Lines in the new header | |
| Lines I will add in `game.cpp` | |
| Lines I will remove in `game.cpp` | |
| Hunks in the diff | |
| Functions whose signature changes | |
| Logic lines I will change (not names) | |
| Seeds that match, of 20, after the change | |
| New concepts the change requires | |

- The prediction I am least sure about, and why:

### 3.3 Instantiation and code generation

| Question | My prediction |
|---|---|
| Instantiations of the generator in the game | |
| `nm -C game14.o` symbols with `generate_move_candidates`, at `-O2` | |
| The same count at `-O0` | |
| `game.o` text: grows, shrinks or stays the same? | |
| Indirect calls in `game14.o` | |
| `sizeof(MoveCandidate)` | |
| `sizeof(std::array<MoveCandidate, 8>)` | |
| Candidates computed on seed 1 | |
| Destination pairs computed by `construction-13` on seed 1 | |

- What I expect the template to change in the object code, in one sentence:
- What I expect it not to change:

## 4. Run — baseline and instantiation probe

- Baseline build command:
- Baseline exit status:
- Diff against `../../../common/traces/seed-0001.txt`:
- Seeds that matched, of 20, on the unmodified baseline:
- Probe source path (scratch only):
- Probe build command:
- Probe exit status:

Paste the probe output:

```text
<probe output>
```

| Observation | Predicted | Observed | Gap |
|---|---|---|---|
| `Knight` candidate 1 | | | |
| `Knight` candidate 4 | | | |
| `Knight` candidate 8 | | | |
| `ScoutingPawn` candidate 1 | | | |
| `ScoutingPawn` candidate 4 | | | |
| `ScoutingPawn` candidate 8 | | | |
| `sizeof(MoveCandidate)` | | | |
| `sizeof(array)` | | | |

- The line that surprised me:
- What I did next instead of adjusting the prediction:

## 5. Compare — what the template proves

- Output of `grep -n 'Knight' code/move_generator.h`:
- Output of `grep -n 'generate_move_candidates' code/game.cpp`:
- Number of generator calls in `game.cpp`:
- Number of helper signatures in `game.cpp` that name `Knight`:
- Which of them are inside the anonymous namespace:

| Lines | Function | Calls to `Knight` operations |
|---|---|---|
| | | |

- Total accessor call sites:
- Total lines that contain an accessor call site:
- The operation that the generator requires but does not declare:
- What the generator decides:
- What `tryMove` decides:
- What the probe proves about the generator:
- What the probe does not prove:

### 5.1 The two instantiations

- Command that produced the symbols:
- Number of `generate_move_candidates` symbols at `-O0`:
- The two mangled or demangled names:
- Number of symbols at `-O2`:
- Why the count changes:
- What `W` means in `nm` output:
- Whether the two instantiations share a runtime object:

### 5.2 Code generation

- `game13.o` text:
- `game14.o` text:
- Difference:
- Linked `construction-13` text / data:
- Linked `construction-14` text / data:
- Indirect calls in `game13.o`:
- Indirect calls in `game14.o`:
- `generate_move_candidates` symbols in `game14.o` at `-O2`:
- `generate_move_candidates` symbols in `game14.o` at `-O0`:
- Why the absence of a symbol is not a property of templates:
- What the absence of a table does not prove about speed:

### 5.3 Failure modes

- Type without `row()`: first error line and message:
- Type with non-const `row()`: first error line and message:
- Template body in a `.cpp`: linker message:
- Why the body must be visible:
- What a concept would remove from the first error:

## 6. The change test in a scratch copy

- Scratch copy path:
- Files added:
- Files edited:
- Lines added in `game.cpp`:
- Lines removed in `game.cpp`:
- Hunks in the diff:
- Logic lines changed:
- Tracked files edited: (yes / no)

### 6.1 Add the generator

- Header path:
- Lines in the header:
- The type parameter:
- The two operations it requires:
- The record it returns:
- The size of one record:
- The size of the array:
- Why the body is in the header:

### 6.2 Remove the inline geometry

- Lines removed from `game.cpp`:
- What replaced them in `tryMove`:
- What `tryMove` no longer receives:
- What `playTurn` builds:
- Where the candidate is indexed:
- The include added:

### 6.3 Build and traces

- Build command:
- Seeds that matched, of 20:
- Diff against the unmodified baseline transcript:

### 6.4 Counts

| Metric | construction-13 | construction-14 | Difference |
|---|---|---|---|
| Files | | | |
| Source lines | | | |
| `game.cpp` lines | | | |
| Public declarations | | | |
| Complete classes | | | |
| Function definitions | | | |
| Translation units | | | |
| `game.o` text | | | |
| Binary text / data | | | |
| Indirect calls | | | |
| Traces | | | |

### 6.5 The gap: only the generator is generic

- The second piece type the generator accepts:
- The functions that still name `Knight`:
- Option 1 (duplicate the loop): functions that would be duplicated:
- Option 2 (restore an interface): which lesson that is:
- Option 3 (parameterize the loop): which lesson that is:
- The precise acceptance sentence of this lesson:
- What the acceptance does not claim:

## 7. The generator and its limits

| Question | Answer |
|---|---|
| What does the generator accept? | |
| What does it require? | |
| What does it return? | |
| What does it not decide? | |
| Where do the offsets live? | |
| How many instantiations does the game create? | |
| What does the loop still name? | |
| How many signatures name it? | |
| What does a second piece type cost in the generator? | |
| What does a second piece type cost in the loop? | |
| What does the array cost per turn? | |
| What does the change cost in the source? | |
| What does it cost in the object code? | |
| Can the loop move a second type without edits? | |

- What the template removes from `tryMove`:
- What it moves into `move_generator.h`:
- What it does not remove:
- Which later lesson parameterizes the loop:
- Which later lesson owns lifetime:

## 8. Explain

One sentence, without the words *template*, *generic*, *type parameter*, *instantiation*,
*compiler*, *better*, *worse*, *clean*, *flexible*, *reusable* or *elegant*:

>

One paragraph:

- What the generator fixes:
- What enforces it:
- What makes it different from `construction-13`:
- What the 17 accessor call sites do:
- What the 20 traces prove:
- What the traces do not prove:
- What the change test costs:
- What the template does not cover:
- What it saves:
- What it costs:
- Which later lesson builds on it:

The four things I kept apart:

## 9. Acceptance check

- I located the template and named its type parameter: (yes / no)
- I confirmed that the header does not include `knight.h` and does not mention `Knight`: (yes / no)
- I counted the 17 accessor call sites and the one generator call: (yes / no)
- I named the two operations the generator requires: (yes / no)
- I predicted the candidate table and observed both columns: (yes / no)
- I reproduced all 20 traces on the unmodified baseline: (yes / no)
- I built the probe and saw the same eight destinations for both types: (yes / no)
- I saw the two weak symbols at `-O0`: (yes / no)
- I measured `sizeof(MoveCandidate)` and the array: (yes / no)
- I reproduced the missing-operation and non-const errors: (yes / no)
- I reproduced the undefined reference from a split definition: (yes / no)
- I added the header and edited `game.cpp` in a scratch copy: (yes / no)
- I got 10 added and 8 removed lines and zero rule changes: (yes / no)
- I got 20 of 20 traces after the refactor: (yes / no)
- I measured the candidate counts against the destination pairs: (yes / no)
- I named the limit of this design: the loop still names `Knight`: (yes / no)
- I did not edit any tracked code file: (yes / no)

If I edited a tracked code file, what and why:

## 10. Reflection and next step

- What I can now explain without notes:
- What I cannot explain yet:
- One thing I would read again:
- The number I will carry to `construction-15`:
- The candidate row that surprised me most:
- What the generator decides that I expected the loop to decide:
- What the loop still decides that I expected the generator to decide:
- What the template buys that the inline formula could not offer:
- What it gives up:
- What the change test proved that the traces could not:
- The change I would make next to test this design again:
- Next step:

## 11. If a baseline is missing or unfaithful

Fill this only if a file was absent, a build failed for an environmental reason, a diff was not
empty, or the probe contradicted the documented behavior in a way you could not explain.

- Missing file, build error, diff, or contradiction:
- Exact command output:
- What I did instead of inventing a count:
- When I will retry:

## 12. Self-check

- [ ] I inventoried every header and every definition with its file.
- [ ] I located the template, the record and the offsets by line.
- [ ] I compared the inline formula with the generator and named what each one owns.
- [ ] I predicted the candidate table, the change test and the instantiation probe before measuring.
- [ ] I reproduced all 20 traces on the unmodified baseline.
- [ ] I built the probe and saw the same eight destinations for both types.
- [ ] I confirmed that the generator produces off-board candidates.
- [ ] I saw the two instantiations by name at `-O0`.
- [ ] I measured the record, the array and the object-code growth.
- [ ] I reproduced the missing-operation, non-const and split-definition failures.
- [ ] I moved the geometry into the header in a scratch copy.
- [ ] I got 20 of 20 traces after the refactor and changed zero rules.
- [ ] I counted the candidates against the destination pairs.
- [ ] I named the limit of this design: only the generator is generic.
- [ ] I separated what the generator accepts, what the loop names, what the compiler emits and
      what a run can see.
- [ ] My one-sentence explanation avoids the mechanism words and the verdict words.
- [ ] I did not edit any tracked code file.

No score. If a box is unchecked, write the next smallest action in §10.
