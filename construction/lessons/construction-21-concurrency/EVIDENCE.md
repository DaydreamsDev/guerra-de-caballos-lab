# construction-21 — Evidence

> [English](EVIDENCE.md) | [Spanish](EVIDENCE-es.md)

> Learner-owned record. Write in English.

This record compares a core with a global random sequence to a core with one sequence per engine,
and it follows a runner that loses writes when it shares a result index. It asks who owns the game
state, who owns the result state, what the order contract is, and what a run can see. Keep the
predictions. The gap between your prediction and the observation is the result, whether it is zero
or not.

## 1. Run setup

- Date:
- Seed range: `1` to `16000`
- Player name: `Learner`
- Repository root:
- Compiler and version (`g++ --version` or `clang++ --version`):
- Platform and architecture:
- Baseline read on (date):
- Construction-20 record at hand? (yes / no)
- The numbers I carried from `construction-20` (source lines / files / `game.o` text / binary
  text / data / BSS / traces):
- Scratch directory I used:
- Did I edit any tracked file? (yes / no)

## 2. State inventory (before the run)

### 2.1 Where the state lives

Fill this from the source, with line numbers, not from memory.

| State | `construction-20` owner | `construction-21` owner | Shared between games? | Synchronization |
|---|---|---|---|---|
| RNG state | | | | |
| Board | | | | |
| Transcript | | | | |
| Result vector | | | | |
| `nextIndex` | | | | |

### 2.2 Counts

- Files:
- Headers:
- Translation units in the console build:
- Translation units in the runner build:
- Translation units in the probe build:
- Complete class definitions in the core:
- Global mutable variables in the core:
- Shared mutable result objects in the runner (race / mutex / merge):
- Source lines:
- Files byte-identical to `construction-20`:
- Files changed:
- Files new:

Compared with `construction-20`:

- Files: ______ → ______
- Headers: ______ → ______
- Translation units (console build): ______ → ______
- Source lines: ______ → ______
- Changed files: ______
- New files: ______
- Global mutable RNG: ______ → ______
- `sizeof(GameEngine<8, 8>)`: ______ → ______
- `sizeof(RandomSequence)`: ______
- `game.o` text: ______ → ______
- Binary text: ______ → ______
- Binary data: ______ → ______
- Binary BSS: ______ → ______

### 2.3 Contract questions

| Question | Answer |
|---|---|
| Where was the RNG state in `construction-20`? | |
| Where is it in `construction-21`? | |
| How many bytes does `RandomSequence` occupy? | |
| Which member explains that size? | |
| How many `randomSequence.*` call sites are in `game_engine.h`? | |
| Which free functions did they replace? | |
| Which three files changed? | |
| Which file is new? | |
| Which fourteen files are byte-identical? | |
| How many threads does the runner start? | |
| How many games per thread? | |
| How many games in total? | |
| Which modes share a result vector? | |
| Which mode gives each thread its own vector? | |
| Which mode has no synchronization? | |
| Which two objects are shared in `race`? | |
| Which modes sort by seed before the checksum? | |
| Why does the checksum need that order? | |
| What sentinel does the race scan use? | |
| Why is that sentinel safe for these seeds? | |
| Which state left BSS? | |
| How many bytes did BSS lose? | |

### 2.4 The four modes

| Mode | Shared result state | Synchronization | Sorts by seed? | What it proves |
|---|---|---|---|---|
| `sequential` | | | | |
| `race` | | | | |
| `mutex` | | | | |
| `merge` | | | | |

- The mode that is the reference:
- The mode that is the failure:
- The mode that is shared state with a lock:
- The mode that is ownership plus a handoff:
- The two conditions a fixed mode must satisfy:

## 3. Prediction (written before measuring)

### 3.1 The structure table

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

- The row I expect to get wrong, and why:
- Why the engine looks different but only its call sites changed:
- Which file carries most of the new lines:

### 3.2 The race table

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

- Why I expect `written` below 16,000:
- Why the checksum can differ even if `written` were 16,000:
- Which number is the direct evidence of a lost write:

### 3.3 The size and text table

| Question | My prediction |
|---|---|
| `sizeof(GameEngine<8, 8>)`, before and after | |
| `sizeof(RandomSequence)` | |
| `game.o` text bytes, before and after | |
| Binary text bytes, before and after | |
| Binary data bytes, before and after | |
| Binary BSS bytes, before and after | |
| Seeds that match, of 20 | |

- Why I expect the engine to grow by 8 bytes:
- Why I expect BSS to shrink:
- Why I expect data to stay flat:
- The number I expect to be surprised by:

### 3.4 The removal table

| Question | My prediction |
|---|---|
| The fix I will apply | |
| Files I will edit | |
| Lines I will add | |
| Written after the fix | |
| Checksum after the fix | |
| Seeds that match, of 20, after the fix | |
| Build result with `-Wall -Wextra -Werror` | |

- Why the fix should touch only `parallel_runner.cpp`:
- Why the sort is part of the fix:
- What would tell me the race is still present:

## 4. Run — the baseline

### 4.1 Traces

- Build command:
- Exit status:
- Diff against `../../../common/traces/seed-0001.txt`:
- Seeds that matched, of 20, on the unmodified baseline:

### 4.2 The four modes

- Build command:
- Exit status:
- `sequential` line:
- `race` line:
- `mutex` line:
- `merge` line:

| Mode | Written | `next_index` | Checksum |
|---|---:|---:|---:|
| `sequential` | | | |
| `race` | | | |
| `mutex` | | | |
| `merge` | | | |

- Which modes match the sequential checksum:
- Which mode loses writes:
- Which mode has an unstable checksum:

### 4.3 Race repeated

| Run | `next_index` | `written` | Checksum |
|---|---:|---:|---:|
| 1 | | | |
| 2 | | | |
| 3 | | | |
| 4 | | | |

- Did `written` equal `next_index` in every run? (yes / no)
- Why that equality appears, and why it is not a guarantee:
- The two reasons the checksum differs:
- What the written count proves that the checksum does not:

### 4.4 Sizes

- `sizeof` probe output:
- `sizeof(GameEngine<8, 8>)`:
- `sizeof(RandomSequence)`:

### 4.5 Object and binary

- `game.o` text before and after:
- Linked binary text before and after:
- Linked binary data before and after:
- Linked binary BSS before and after:
- Which state explains the BSS change:

### 4.6 Optional: ThreadSanitizer

- TSan available? (yes / no)
- Command:
- Report found? (yes / no)
- The two locations in the report:
- What the report proves that the number does not:
- What the report does not prove:

### 4.7 Prediction and observation

| Observation | Predicted | Observed | Gap |
|---|---|---|---|
| Files | | | |
| Source lines | | | |
| `sizeof(GameEngine<8, 8>)` | | | |
| `sizeof(RandomSequence)` | | | |
| `game.o` text | | | |
| Binary text | | | |
| Binary data | | | |
| Binary BSS | | | |
| Race written | | | |
| Race checksum | | | |
| Traces matched | | | |

- The number that surprised me:
- What I did next instead of adjusting the prediction:

## 5. Compare — what ownership proves

- Command for the RNG-state check:
- Where the state was before:
- Where the state is now:
- Command for the call-site check:
- `randomSequence.*` call sites:
- Which free functions disappeared:
- Text bytes of `game.o` before and after:
- Text bytes of the linked binary before and after:
- Data bytes of the linked binary before and after:
- BSS bytes of the linked binary before and after:
- What the sizes prove:
- What the 20 traces prove:
- What the traces do not prove:
- What the checksum proves:
- What the checksum does not prove:

### 5.1 The exact diff

- Command:
- Files changed:
- Files new:
- Added / removed / net lines:
- `game_engine.h`: added / removed / net:
- `random_sequence.h`: added / removed / net:
- `random_sequence.cpp`: added / removed / net:
- `parallel_runner.cpp` lines:
- Files byte-identical:

### 5.2 What did not change

- The LCG:
- The transcript:
- The rules:
- The board layout:
- The move generator:
- The attempt loop:
- The public game interface:
- The trace count:
- What the ownership change did not move out of the engine:

### 5.3 What it costs

- New files and lines:
- Bytes added per engine:
- Binary text added:
- Binary data added:
- Binary BSS removed:
- The lifetime rule inside the engine:
- The new reading burden:
- The second program:
- The order contract:

### 5.4 The state table

| State | `construction-20` owner | `construction-21` owner | Shared between games? | Synchronization |
|---|---|---|---|---|
| RNG state | | | | |
| Board | | | | |
| Transcript | | | | |
| Result vector | | | | |
| `nextIndex` | | | | |

- Which rows are owned:
- Which rows are the runner's problem:
- Why owned state needs no protocol:
- Why shared state needs one:

### 5.5 Why the fixed modes match

- What `run_one_game` constructs per call:
- What `run_mutex` does around the append:
- What `run_merge` does after the join:
- What `result_checksum` folds, and in what order:
- Why `sequential` is already in seed order:
- Why `mutex` needs the sort:
- Why `merge` sorts even though its concatenation is already seed-ordered:
- The three conditions that make the checksums equal:

## 6. The change test — remove the race

- Scratch copy path:
- Files copied:
- Files edited:
- Tracked files edited: (yes / no)
- Fix A (mutex) applied? (yes / no)
- Fix B (per-thread results) applied? (yes / no)
- Build command:
- Build status with `-Wall -Wextra -Werror`:
- Output after Fix A:
- Output after Fix B:
- Stable across at least two runs? (yes / no)
- Seeds that matched, of 20, after the fix:

### 6.1 Fix A: the patch

- Lines removed from `run_race`:
- Lines added for the mutex:
- Lambda capture change:
- The protected append:
- The sort call:
- The `printf` change:
- Why the `writtenResults` scan is deleted:

### 6.2 Fix B: the patch

- The per-thread vector:
- The lambda capture:
- The append inside the thread:
- The merge after the join:
- The sort call:
- Why no lock appears:
- How Fix B differs from Fix A in cost:

### 6.3 What the change test proves, and what it does not

- It proves about the lost writes:
- It proves about the engine's RNG:
- It proves about the order:
- It proves about the blast radius:
- It proves about the two fixes:
- It does not prove about a shared engine:
- It does not prove about performance:
- It does not prove about the race's reproducibility:
- What the atomic variant fixes, and what it leaves broken:

### 6.4 Counts

| Metric | `construction-20` | `construction-21` | Difference |
|---|---:|---:|---:|
| Source lines | | | |
| Files | | | |
| Global mutable RNG | | | |
| `sizeof(GameEngine<8, 8>)` | | | |
| `sizeof(RandomSequence)` | | | |
| `game.o` text | | | |
| Binary text | | | |
| Binary data | | | |
| Binary BSS | | | |
| Traces matched | | | |
| Parallel modes | | | |

## 7. The concurrency model and its limits

| Question | Answer |
|---|---|
| What is owned by the engine? | |
| What is owned by the runner? | |
| What is shared in `race`? | |
| What is shared in `mutex`? | |
| What is shared in `merge`? | |
| What is the "message" in `merge`? | |
| Why does `race` lose writes? | |
| Why does `written` equal `next_index` in the observed runs? | |
| Why is that equality not a guarantee? | |
| Why does the checksum need a sort? | |
| What does the mutex serialize? | |
| What does the merge avoid? | |
| What does the engine not guarantee? | |
| What does the transcript adapter not guarantee? | |
| What do the 20 traces not test? | |
| What does the checksum not test? | |
| What would a real scheduler need that the runner lacks? | |
| Which later lesson owns error contracts? | |

- The five couplings that remain:
- Which coupling is about the engine:
- Which coupling is about the adapter:
- Which coupling is about the result collection:
- Which coupling is about the order:
- Which coupling is about the evidence:

## 8. Explain

One sentence, without the words *thread*, *mutex*, *lock*, *race*, *concurrency*, *concurrent*,
*parallel*, *atomic*, *shared*, *better*, *worse*, *clean* or *decoupled*:

>

One paragraph:

- What changed:
- What stayed:
- What the runner adds:
- What the race is:
- What the fixes are:
- What determinism requires:
- What the numbers prove:
- What the checksum proves:
- What the traces prove:
- Which later lesson owns error contracts:

The four things I kept apart:

## 9. Acceptance check

- I located the global `randomState` and the private `state` member: (yes / no)
- I counted the eight `randomSequence.*` call sites: (yes / no)
- I named the three changed files, the new runner and the fourteen identical files: (yes / no)
- I predicted the structure, race, size and removal tables before measuring: (yes / no)
- I reproduced all 20 traces on the unmodified console baseline: (yes / no)
- I ran the four modes and recorded written and checksum: (yes / no)
- I ran `race` at least four times and saw the result move: (yes / no)
- I explained why `written` equals `next_index`, with the UB caveat: (yes / no)
- I explained the two reasons the race checksum differs: (yes / no)
- I measured `sizeof(GameEngine<8, 8>)` at 40 and `sizeof(RandomSequence)` at 8: (yes / no)
- I measured `game.o` text at 4,280 and the binary at 10,869 text, 876 data and 16 BSS:
  (yes / no)
- I removed the race in a scratch copy with a mutex or per-thread results: (yes / no)
- The fixed run printed `written=16000` and `checksum=2826026307342797653`: (yes / no)
- The fixed run was stable across at least two executions: (yes / no)
- All 20 traces matched after the fix: (yes / no)
- The scratch build passed with `-Wall -Wextra -Werror`: (yes / no)
- I named what the engine does not guarantee: (yes / no)
- I named the order contract: (yes / no)
- I did not edit any tracked code file: (yes / no)

If I edited a tracked code file, what and why:

## 10. Reflection and next step

- What I can now explain without notes:
- What I cannot explain yet:
- One thing I would read again:
- The number I will carry to `construction-22`:
- The prediction that was furthest from the observation:
- What I expected the compiler to optimize away that it did not:
- What I expected the race to do that it did not:
- What the written count told me that the checksum could not:
- What the checksum told me that the written count could not:
- Which fix I would choose for a larger result set, and why:
- Which fix I would not choose, and why:
- The change I would make next to test this design again:
- Next step:

## 11. If a baseline is missing or unfaithful

Fill this only if a file was absent, a build failed for an environmental reason, a diff was not
empty, TSan was unavailable, or a measurement contradicted the documented behavior.

- Missing file, build error, diff or contradiction:
- Exact command output:
- What I did instead of inventing a count:
- When I will retry:

## 12. Self-check

- [ ] I inventoried the two states: the game's RNG and the runner's result collection.
- [ ] I counted the eight `randomSequence.*` call sites.
- [ ] I named the three changed files and the new runner.
- [ ] I predicted the structure, race, size and removal tables before measuring.
- [ ] I reproduced all 20 traces on the unmodified baseline.
- [ ] I ran `sequential`, `race`, `mutex` and `merge`.
- [ ] I ran `race` more than once and explained the variation.
- [ ] I measured the engine at 40 bytes and `RandomSequence` at 8.
- [ ] I measured `game.o`, the binary text, the binary data and the binary BSS.
- [ ] I removed the race with a mutex and, if possible, with per-thread results.
- [ ] The fixed checksum matched the sequential checksum and stayed stable.
- [ ] All 20 traces matched after the fix.
- [ ] I separated owned state from shared state.
- [ ] I named the order contract and the engine's non-reentrancy.
- [ ] My one-sentence explanation avoids the mechanism words and the verdict words.
- [ ] I did not edit any tracked code file.

No score. If a box is unchecked, write the next smallest action in §10.
