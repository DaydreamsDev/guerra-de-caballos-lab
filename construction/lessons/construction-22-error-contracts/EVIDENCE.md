# construction-22 — Evidence

> [English](EVIDENCE.md) | [Español](EVIDENCE-es.md)

> Learner-owned record. Write in English.

This record follows one answer that used to arrive in three pieces and now arrives in one. It asks
what the engine returns, what the caller decides, what the transcript records, and what the process
does. Keep the predictions. The gap between your prediction and the observation is the result,
whether it is zero or not.

## 1. Run setup

- Date:
- Seed range: `1` to `20`
- Player name: `Learner`
- Repository root:
- Compiler and version (`g++ --version` or `clang++ --version`):
- Platform and architecture:
- Baseline read on (date):
- Construction-21 record at hand? (yes / no)
- The numbers I carried from `construction-21` (source lines / files / `game.o` text / binary
  text / data / BSS / traces):
- Scratch directory I used:
- Did I edit any tracked file? (yes / no)

## 2. Contract inventory (before the run)

### 2.1 The declarations

Fill this from the source, with line numbers, not from memory.

| Question | Answer |
|---|---|
| Which file declares `MoveOutcome`? | |
| Which file declares `MoveResult`? | |
| How many values does `MoveOutcome` have? | |
| Their names, in order | |
| Their numeric values | |
| How many members does `MoveResult` have? | |
| Their names and types | |
| What `tryMove` returns in `construction-21` | |
| What `tryMove` returns here | |
| Which method exposes the contract publicly | |
| Which function is the only production caller | |

### 2.2 The branches

| Outcome | What `playTurn` calls | Continues or accepts? | Game ends? |
|---|---|---|---|
| `outsideBoard` | | | |
| `visited` | | | |
| `capture` | | | |
| `legal` | | | |

- Why `outsideBoard` is checked first:
- Why `capture` is checked before `visited`:
- What happens to the trace if those two are swapped:

### 2.3 Counts

- Files:
- Headers:
- Translation units in the console build:
- Translation units in the probe build:
- Complete class definitions in the core:
- Global mutable variables in the core:
- Values in `MoveOutcome`:
- Members in `MoveResult`:
- Source lines:
- Files byte-identical to `construction-21`:
- Files changed:
- Files new:
- Calls to `exit` in `code/`:

Compared with `construction-21`:

- Files: ______ → ______
- Headers: ______ → ______
- Translation units (console build): ______ → ______
- Source lines: ______ → ______
- Changed files: ______
- New files: ______
- `sizeof(MoveOutcome)`: ______
- `sizeof(MoveResult)`: ______
- `sizeof(GameEngine<8, 8>)`: ______ → ______
- `game.o` text: ______ → ______
- Binary text: ______ → ______
- Binary data: ______ → ______
- Binary BSS: ______ → ______
- `error_probe.o` text: ______

## 3. Prediction (written before measuring)

### 3.1 The structure table

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

- The row I expect to get wrong, and why:
- Why the engine looks different but only its signature and one call site changed:
- Which file carries the enum and the struct:

### 3.2 The probe table

| Question | My prediction |
|---|---|
| `error_probe` output line | |
| Exit status of `error_probe` | |
| Which outcome is missing from the probe | |
| Why it is missing | |
| Exit status of `main` with a missing `--name` | |
| Exit status of `main` with a valid game | |
| Seeds that match, of 20 | |

- The three enum numbers I expect:
- Why the numbers are not the contract:
- What the probe would print if the engine returned an enum only:

### 3.3 The size table

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

- Why I expect `MoveResult` to be 12 bytes:
- Why I expect the engine to stay at 40:
- Why I expect data and BSS to stay flat:
- The number I expect to be surprised by:

### 3.4 The decision table

| Question | My prediction |
|---|---|
| Who decides to continue after an invalid move? | |
| Who decides to report it? | |
| Who decides to stop? | |
| What the core knows about the transcript | |
| What the core knows about the process | |
| What `return 2` from `main` does | |

- Why `return` from `main` is not `exit`:
- What a caller can do that `exit` prevents:
- The recovery I expect to write in the change test:

## 4. Run — the baseline

### 4.1 Traces

- Build command:
- Exit status:
- Diff against `../../../common/traces/seed-0001.txt`:
- Seeds that matched, of 20, on the unmodified baseline:

### 4.2 The error probe

- Build command:
- Exit status:
- Output line:
- `outside` outcome, name and number:
- `capture` outcome, name and number:
- `legal` outcome, name and number:
- Which outcome did not appear, and why:

### 4.3 The argument path

- Command with a missing `--name`:
- Message:
- Exit status:
- Command with a valid game:
- Exit status:
- Which line of `main.cpp` produces each status:

### 4.4 Sizes

- `sizeof` probe output:
- `sizeof(MoveOutcome)`:
- `sizeof(MoveResult)`:
- `sizeof(GameEngine<8, 8>)`:

### 4.5 Object and binary

- `game.o` text before and after:
- Linked binary text before and after:
- Linked binary data before and after:
- Linked binary BSS before and after:
- `error_probe.o` text:
- Which of these numbers moved, and which did not:

### 4.6 Optional: the fourth outcome

- Probe written? (yes / no)
- Output line:
- Which seed I used:
- Counts for each outcome:
- Why a fresh engine cannot produce `visited`:
- What this probe proves about `evaluate_move` being a query:

### 4.7 Prediction and observation

| Observation | Predicted | Observed | Gap |
|---|---|---|---|
| Files | | | |
| Headers | | | |
| Source lines | | | |
| `sizeof(MoveOutcome)` | | | |
| `sizeof(MoveResult)` | | | |
| `game.o` text | | | |
| Binary text | | | |
| Binary data | | | |
| Binary BSS | | | |
| `error_probe.o` text | | | |
| Probe output | | | |
| Traces matched | | | |

- The number that surprised me:
- What I did next instead of adjusting the prediction:

## 5. Compare — what the value carries

- Command for the declaration check:
- What `tryMove` returns here:
- What it returned in `construction-21`:
- Who initializes the destination now:
- Can the caller forget the destination? (yes / no)
- Can the caller ignore the outcome? (yes / no)
- How many return paths `tryMove` has:
- What `evaluate_move` adds:
- What the sizes prove:
- What the 20 traces prove:
- What the traces do not prove:
- What the probe proves:
- What the probe does not prove:

### 5.1 The exact diff

- Command:
- Files changed:
- Files new:
- Added / removed / net lines:
- `game_engine.h`: added / removed / net:
- `move_result.h` lines:
- `error_probe.cpp` lines:
- Files byte-identical:
- What moved out of `game_engine.h`:
- What stayed the same inside `playTurn`:

### 5.2 What did not change

- The rules:
- The transcript:
- The board, the generator, the knight, the position:
- The port and the adapters:
- The public game interface:
- The engine's size:
- The trace count:

### 5.3 What it costs

- New files and lines:
- Binary text added:
- Binary data added:
- Binary BSS added:
- Headers added to the include graph:
- The second public entry point:
- The naming burden:
- The risk of ignoring the value:

### 5.4 The contract table

| Question | `construction-21` | `construction-22` |
|---|---|---|
| What `tryMove` returns | | |
| Who initializes the destination | | |
| Can the caller forget the destination | | |
| Can the caller ignore the outcome | | |
| Number of return paths | | |
| Who decides to continue | | |
| Who decides to stop the process | | |
| How a probe sees the contract | | |
| Size of the answer | | |

- The row that is a real trade, and why:
- The row that is unchanged, and why:

## 6. The change test — let the caller decide

- Scratch copy path:
- Files copied:
- Files created:
- Tracked files edited: (yes / no)
- Policy modes I ran:
- Build command:
- Build status with `-Wall -Wextra -Werror`:
- Output in report mode:
- Output in stop mode:
- Exit status in report mode:
- Exit status in stop mode:
- `Recovery` values I used:
- Seeds that matched, of 20, after the change:

### 6.1 The policy

- The function that maps outcome to recovery:
- Why it has no `default` label:
- What the compiler does if a fifth outcome is added:
- How the caller parameter changes the recovery:
- Which line stops the program:
- Why that line is not `exit`:

### 6.2 What the change test proves, and what it does not

- It proves about the invalid move:
- It proves about the caller:
- It proves about the core:
- It proves about stopping:
- It proves about the transcript:
- It does not prove about `visited` from a fresh engine:
- It does not prove about ignored values:
- It does not prove about enum stability:

### 6.3 Counts

| Metric | `construction-21` | `construction-22` | Difference |
|---|---:|---:|---:|
| Source lines | | | |
| Files | | | |
| Headers | | | |
| Values in `MoveOutcome` | | | |
| Members in `MoveResult` | | | |
| `sizeof(MoveResult)` | | | |
| `sizeof(GameEngine<8, 8>)` | | | |
| `game.o` text | | | |
| Binary text | | | |
| Binary data | | | |
| Binary BSS | | | |
| Traces matched | | | |
| `exit` calls in `code/` | | | |

## 7. The contract model and its limits

| Question | Answer |
|---|---|
| What the engine returns | |
| What the caller decides | |
| What the transcript records | |
| What the process does | |
| Why an invalid move is not exceptional | |
| Why the enum has four values and not a boolean | |
| Why the check order matters | |
| What `evaluate_move` reads | |
| Why a fresh engine cannot return `visited` | |
| What the caller can still ignore | |
| What `[[nodiscard]]` would add | |
| Why `std::expected` is not in this baseline | |
| What `return 2` from `main` means | |
| Which later lesson owns the port | |

- The five couplings that remain:
- Which coupling is about the query:
- Which coupling is about the ignored value:
- Which coupling is about the policy:
- Which coupling is about the numbers:
- Which coupling is about the process:

## 8. Explain

One sentence, without the words *struct*, *enum*, *return value*, *out-parameter*, *exception*,
*expected*, *better*, *worse*, *clean* or *decoupled*:

>

One paragraph:

- What changed:
- What stayed:
- What the probe adds:
- What the probe misses:
- What the caller decides:
- What the process does:
- What the numbers prove:
- What the traces prove:
- Which later lesson owns the port:

The four things I kept apart:

## 9. Acceptance check

- I read `move_result.h`, `game_engine.h`, `error_probe.cpp` and `main.cpp`: (yes / no)
- I named the four outcomes and their order inside `tryMove`: (yes / no)
- I explained why `capture` is checked before `visited`: (yes / no)
- I counted 20 files, 11 headers and 1,071 source lines: (yes / no)
- I named the one changed file and the two new files: (yes / no)
- I predicted the structure, probe, size and decision tables before measuring: (yes / no)
- I reproduced all 20 traces on the unmodified console baseline: (yes / no)
- I ran `error_probe` and saw `outside=0 capture=2 legal=3` with status 0: (yes / no)
- I ran `main` with a bad argument and saw status 2: (yes / no)
- I measured `sizeof(MoveOutcome)` at 4 and `sizeof(MoveResult)` at 12: (yes / no)
- I measured `game.o` text at 4,312 and the binary at 10,901 text, 876 data and 16 BSS:
  (yes / no)
- I measured `error_probe.o` at 2,633 text bytes: (yes / no)
- I saw the fourth outcome with a board that has history, or I recorded why not: (yes / no)
- I wrote a caller policy in a scratch copy and ran it in two modes: (yes / no)
- Both modes ended with status 0 and no `exit`: (yes / no)
- All 20 traces matched after the change test: (yes / no)
- The scratch build passed with `-Wall -Wextra -Werror`: (yes / no)
- I named what the value does not guarantee: (yes / no)
- I did not edit any tracked code file: (yes / no)

If I edited a tracked code file, what and why:

## 10. Reflection and next step

- What I can now explain without notes:
- What I cannot explain yet:
- One thing I would read again:
- The number I will carry to `construction-23`:
- The prediction that was furthest from the observation:
- What I expected the compiler to optimize away that it did not:
- What I expected the probe to show that it did not:
- What the traces told me that the probe could not:
- What the probe told me that the traces could not:
- Which recovery I would choose for a larger program, and why:
- Which recovery I would not choose, and why:
- The change I would make next to test this design again:
- Next step:

## 11. If a baseline is missing or unfaithful

Fill this only if a file was absent, a build failed for an environmental reason, a diff was not
empty, a probe could not produce an outcome, or a measurement contradicted the documented behavior.

- Missing file, build error, diff or contradiction:
- Exact command output:
- What I did instead of inventing a count:
- When I will retry:

## 12. Self-check

- [ ] I inventoried the contract: the enum, the struct, the public entry point and the one
      production caller.
- [ ] I named the four outcomes, their values and their order.
- [ ] I explained why `capture` precedes `visited`.
- [ ] I predicted the four tables before measuring.
- [ ] I reproduced all 20 traces on the unmodified baseline.
- [ ] I ran the probe and the argument path and recorded both statuses.
- [ ] I measured `sizeof(MoveOutcome)`, `sizeof(MoveResult)` and the engine.
- [ ] I measured `game.o`, the binary text, the data, the BSS and the probe object.
- [ ] I showed the fourth outcome, or named why I could not.
- [ ] I wrote a caller policy and ran it in two modes.
- [ ] No path in `code/` calls `exit`.
- [ ] All 20 traces matched after the change test.
- [ ] I separated what the engine returns from what the caller decides.
- [ ] My one-sentence explanation avoids the mechanism words and the verdict words.
- [ ] I did not edit any tracked code file.

No score. If a box is unchecked, write the next smallest action in §10.
