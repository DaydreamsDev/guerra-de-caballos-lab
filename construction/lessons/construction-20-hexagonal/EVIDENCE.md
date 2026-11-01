# construction-20 — Evidence

> [English](EVIDENCE.md) | [Spanish](EVIDENCE-es.md)

> Learner-owned record. Write in English.

This record compares a core that prints with a core that announces events. It asks who decides what
happened, who decides how it looks, what the seam costs, and what a run can see. Keep the
predictions. The gap between your prediction and the observation is the result, whether it is zero
or not.

## 1. Run setup

- Date:
- Seed: `1`
- Player name: `Learner`
- Repository root:
- Compiler and version (`g++ --version` or `clang++ --version`):
- Platform and architecture:
- Baseline read on (date):
- Construction-17 record at hand? (yes / no)
- The numbers I carried from `construction-17` (source lines / files / `game.o` text / binary
  text / traces):
- Scratch directory I used:
- Did I edit any tracked file? (yes / no)

## 2. The boundary (before the run)

### 2.1 Port inventory

Fill this from the source, with line numbers, not from memory.

| Port method | Declaration line | Core call site | Console rendering | Recording field |
|---|---|---|---|---|
| `game_header` | | | | |
| `turn_header` | | | | |
| `illegal_attempt` | | | | |
| `capture_attempt` | | | | |
| `legal_attempt` | | | | |
| `move` | | | | |
| `no_legal_move` | | | | |
| `winner` | | | | |

### 2.2 Adapter table

| Question | Console adapter | Recording adapter |
|---|---|---|
| Header file | | |
| Translation unit | | |
| Methods implemented | | |
| `printf`/`fprintf` lines | | |
| State it stores | | |
| Output it produces | | |
| Linked into the console binary? | | |
| Linked into the probe? | | |

### 2.3 Counts

- Files:
- Headers:
- Translation units in the baseline build:
- Complete class definitions:
- Pure virtual methods:
- Overrides:
- `printf`/`fprintf` lines:
- `<cstdio>` includes:
- `port.*` call sites:
- Source lines:
- Files byte-identical to `construction-17`:
- Files changed:
- Files new:

Compared with `construction-17`:

- Files: ______ → ______
- Headers: ______ → ______
- Translation units: ______ → ______
- Complete classes: ______ → ______
- Pure virtual methods: ______ → ______
- Overrides: ______ → ______
- Print methods in the core: ______ → ______
- `printf`/`fprintf` lines: ______ → ______
- `<cstdio>` includes: ______ → ______
- Source lines: ______ → ______
- `game_engine.h` lines: ______ → ______
- `game.cpp` lines: ______ → ______

### 2.4 Contract questions

| Question | Answer |
|---|---|
| Which files include `<cstdio>`? | |
| Which of them is the core? | |
| Which two programs include `game_engine.h`? | |
| Which adapter does each program use? | |
| Why are there more call sites than methods? | |
| Which six print methods left `GameEngine`? | |
| Where did their bodies go? | |
| Which file is the composition root? | |
| What does the composition root connect? | |
| Which ten files are byte-identical? | |
| Which event does the console render and the recording adapter only count? | |
| Which parameters are values rather than text? | |
| Which two objects must be declared in a specific order? | |
| Why does that order matter? | |

- The core's dependency, in one sentence:
- The console adapter's dependency, in one sentence:
- The recording adapter's dependency, in one sentence:
- What the port owns:
- What the adapter owns:
- What the core still decides:

## 3. Prediction (written before measuring)

### 3.1 The structure table

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

- The row I expect to get wrong, and why:
- Why the net line count is not the size of the new files:
- Which file I expect to shrink, and by how much:

### 3.2 The adapter table

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

- Why I expect `ConsoleTranscript` to be one pointer:
- Why I expect `RecordingTranscript` to be larger:
- Which adapter I expect to be ignored by the other binary:

### 3.3 The size and text table

| Question | My prediction |
|---|---|
| `sizeof(GameEngine<8, 8>)` | |
| `sizeof(Board<8, 8>)` | |
| `game.o` text bytes, before and after | |
| Console adapter object text bytes | |
| Binary text bytes, before and after | |
| Binary data bytes, before and after | |
| Binary BSS bytes, before and after | |
| Indirect calls in `game.o`, before and after | |
| Seeds that match, of 20 | |

- Why I expect the engine to grow by 8 bytes:
- Why I expect the board to stay at 256:
- Why I expect the binary to grow even though `game.o` shrinks:
- The number I expect to be surprised by:

### 3.4 The change test

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

- Why `game_started` is a fair target:
- What would tell me I picked an event that lives inside the loop:
- What the console adapter must do to keep the transcript unchanged:

## 4. Run — the baseline

### 4.1 Traces

- Build command:
- Exit status:
- Diff against `../../../common/traces/seed-0001.txt`:
- Seeds that matched, of 20, on the unmodified baseline:

### 4.2 The adapter probe

- Build command:
- Exit status:
- `headers`:
- `turns`:
- `illegal`:
- `capture`:
- `legal`:
- `moves`:
- `no_legal`:
- `winners`:
- `winner`:
- `final_moves`:
- Does the arithmetic close? (`illegal + legal = attempts`; `moves + no_legal = turns`):

### 4.3 Sizes

- `sizeof` probe output:
- `sizeof(Board<8, 8>)`:
- `sizeof(GameEngine<8, 8>)`:
- `sizeof(Knight)`:
- `sizeof(Position)`:
- `sizeof(MoveCandidate)`:
- `sizeof(TranscriptPort)`:
- `sizeof(ConsoleTranscript)`:
- `sizeof(RecordingTranscript)`:

### 4.4 Object and binary

- `game.o` text before and after:
- Console adapter object text:
- Linked binary text before and after:
- Linked binary data before and after:
- Linked binary BSS before and after:
- Indirect calls in `game.o` before and after:

### 4.5 Prediction and observation

| Observation | Predicted | Observed | Gap |
|---|---|---|---|
| Files | | | |
| Source lines | | | |
| `game_engine.h` lines | | | |
| `printf`/`fprintf` lines | | | |
| `sizeof(GameEngine<8, 8>)` | | | |
| `sizeof(Board<8, 8>)` | | | |
| `game.o` text | | | |
| Console adapter object text | | | |
| Binary text | | | |
| Binary data | | | |
| Indirect calls | | | |
| Traces matched | | | |

- The number that surprised me:
- What I did next instead of adjusting the prediction:

## 5. Compare — what the boundary proves

- Command for the output-header check:
- Files that include `<cstdio>`:
- Files that contain `printf`/`fprintf`:
- Command for the port check:
- Port methods:
- Overrides per adapter:
- `port.*` call sites:
- Text bytes of `game.o` before and after:
- Text bytes of the console adapter object:
- Text bytes of the linked binary before and after:
- Data bytes of the linked binary before and after:
- Indirect calls before and after:
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
- `game.cpp`: added / removed / net:
- `transcript_port.h` lines:
- `console_transcript.h` lines:
- `console_transcript.cpp` lines:
- `recording_transcript.h` lines:
- `adapter_probe.cpp` lines:
- Files byte-identical:

### 5.2 What did not change

- The transcript:
- The rules:
- The board layout:
- The move generator:
- The attempt loop:
- The public game interface:
- The trace count:
- What the port did not move out of the core:

### 5.3 What it costs

- New files and lines:
- Indirect calls added:
- Bytes added per engine:
- Binary text added:
- Binary data added:
- The lifetime rule:
- The change rule:
- One more thing a reader must know:

### 5.4 The event table

| Event | Core call site | Console rendering | Recording field |
|---|---|---|---|
| `game_header` | | | |
| `turn_header` | | | |
| `illegal_attempt` | | | |
| `capture_attempt` | | | |
| `legal_attempt` | | | |
| `move` | | | |
| `no_legal_move` | | | |
| `winner` | | | |

- Which event is rendered by the console and only counted by the recording adapter:
- Which event is counted by both:
- Which event carries the winner name:
- Which event carries the move-order array:
- Which event is emitted outside the turn loop:

## 6. The change test in a scratch copy

- Scratch copy path:
- Files copied:
- Files edited:
- Tracked files edited: (yes / no)
- Event added:
- Line added to the core:
- Line added to the loop:
- Console adapter implementation:
- Recording adapter implementation:
- Build command:
- Build status with `-Wall -Wextra -Werror`:
- Seeds that matched, of 20, after the change:
- New probe count:
- `playTurn` changed? (yes / no)

### 6.1 The patch

- Port declaration:
- Core emission site:
- Console declaration:
- Console body:
- Recording counter:
- Recording body:
- Probe format change:
- Why `(void)seed;` is required:

### 6.2 What the change test proves, and what it does not

- It proves:
- It proves about the console adapter:
- It proves about the recording adapter:
- It proves about the loop:
- It does not prove about events inside the loop:
- It does not prove about the value of the event:
- What would change the transcript, and why that is a different test:

### 6.3 Counts

| Metric | construction-17 | construction-20 | Difference |
|---|---|---|---|
| Source lines | | | |
| Files | | | |
| Port methods | | | |
| Overrides | | | |
| `port.*` call sites | | | |
| Indirect calls in `game.o` | | | |
| `sizeof(GameEngine<8, 8>)` | | | |
| `game.o` text | | | |
| Console adapter object text | | | |
| Binary text / data | | | |
| Traces matched | | | |

## 7. The port model and its limits

| Question | Answer |
|---|---|
| What does the core decide? | |
| What does the adapter decide? | |
| What is a port? | |
| What is an adapter? | |
| Why are the method names game events rather than output commands? | |
| Why are the parameters values rather than text? | |
| Where is the seam? | |
| What would a finer seam require from the adapter? | |
| What would a coarser seam leave in the core? | |
| What does the core still depend on? | |
| What does the port still depend on? | |
| Which parameters are borrowed, and for how long? | |
| Why must the adapter outlive the engine? | |
| What does the compiler enforce when the port grows? | |
| What does the transcript fail to observe? | |
| What does the probe observe that the transcript does not? | |
| What does the binary pay for the seam? | |

- The three couplings that remain:
- Which coupling is a header dependency:
- Which coupling is a lifetime rule:
- Which coupling is a design choice:
- Which later lesson owns concurrency:

## 8. Explain

One sentence, without the words *port*, *adapter*, *interface*, *virtual*, *hexagonal*,
*dependency*, *better*, *worse*, *clean* or *decoupled*:

>

One paragraph:

- What the core decides:
- What the adapter decides:
- What the port is:
- What the console adapter does:
- What the recording adapter does:
- What `game.cpp` does:
- What `adapter_probe.cpp` does:
- What the diff proves:
- What the sizes prove:
- What the indirect calls prove:
- What the change test proves:
- What the loop proves:
- Which later lesson owns concurrency:

The four things I kept apart:

## 9. Acceptance check

- I located the eight port methods and the sixteen overrides: (yes / no)
- I located the ten `port.*` call sites and explained why there are more than eight: (yes / no)
- I showed that `game_engine.h` includes no `<cstdio>` and contains no `printf`: (yes / no)
- I named `game.cpp` as the composition root and `adapter_probe.cpp` as the second program:
  (yes / no)
- I predicted the structure, adapter and size tables before measuring: (yes / no)
- I reproduced all 20 traces on the unmodified baseline: (yes / no)
- I ran the adapter probe and read the counts: (yes / no)
- I measured `sizeof(GameEngine<8, 8>)` at 32 and `sizeof(Board<8, 8>)` at 256: (yes / no)
- I measured `game.o` text at 4,138 and the console adapter object at 1,341: (yes / no)
- I measured the binary at 10,748 text and 876 data bytes: (yes / no)
- I counted 8 indirect calls in `game.o`, against 0 in `construction-17`: (yes / no)
- I added `game_started` in a scratch copy and implemented it in both adapters: (yes / no)
- The scratch build passed with `-Wall -Wextra -Werror`: (yes / no)
- All 20 traces matched after the change: (yes / no)
- The probe printed `started=1` and every other count unchanged: (yes / no)
- `playTurn` was byte-identical in the scratch copy: (yes / no)
- I explained why an adapter may ignore an event: (yes / no)
- I named the three couplings that remain: (yes / no)
- I did not edit any tracked code file: (yes / no)

If I edited a tracked code file, what and why:

## 10. Reflection and next step

- What I can now explain without notes:
- What I cannot explain yet:
- One thing I would read again:
- The number I will carry to `construction-21`:
- The prediction that was furthest from the observation:
- What I expected the compiler to optimize away that it did not:
- What I expected the seam to cost that it did not:
- What the transcript could not tell me:
- What the probe told me that the transcript could not:
- Which event I would add next, and why:
- Which event I would not add, and why:
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

- [ ] I inventoried the eight port methods with their lines.
- [ ] I inventoried the two adapters and their state.
- [ ] I counted the `port.*` call sites: ten.
- [ ] I showed that the core has no `<cstdio>` and no `printf`.
- [ ] I predicted the structure, adapter and size tables before measuring.
- [ ] I reproduced all 20 traces on the unmodified baseline.
- [ ] I ran the probe and checked its arithmetic.
- [ ] I measured the engine, the board, `game.o`, the console adapter object and the binary.
- [ ] I counted the indirect calls: 8 against 0.
- [ ] I added one port event in a scratch copy and implemented it in both adapters.
- [ ] All 20 traces matched after the change.
- [ ] `playTurn` was untouched.
- [ ] I separated what the core decides from what the adapter decides.
- [ ] I named the lifetime rule and the change rule.
- [ ] I named the three couplings that remain.
- [ ] My one-sentence explanation avoids the mechanism words and the verdict words.
- [ ] I did not edit any tracked code file.

No score. If a box is unchecked, write the next smallest action in §10.
