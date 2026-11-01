# construction-23 — Evidence

> Learner-owned record. Write in English.

This record follows one game from a C++ type system into a Python module and asks what survived.
It keeps four things apart: the rules, the transcript, the representation and the process. Keep the
predictions. The gap between your prediction and the observation is the result, whether it is zero
or not.

## 1. Run setup

- Date:
- Seed range: `1` to `20`
- Player name: `Learner`
- Repository root:
- Python version (`python3 --version`):
- C++ compiler and version (`c++ --version`):
- Platform and architecture:
- Baseline read on (date):
- Construction-22 record at hand? (yes / no)
- The numbers I carried from `construction-22` (source lines / files / traces):
- Scratch directory I used:
- Did I edit any tracked file? (yes / no)

## 2. Inventory of the port (before the run)

Fill this from the source, with line numbers, not from memory.

### 2.1 Size and shape

| Question | Answer |
|---|---|
| Files in the port | |
| Lines in `code/game.py` | |
| Lines in `code/main.py` | |
| Total source lines | |
| Headers | |
| Translation units | |
| Build steps | |
| Third-party dependencies | |
| Top-level definitions in `game.py` | |
| Top-level definitions in `main.py` | |
| Lines in `common/harness/reference.py` | |

### 2.2 What replaced what

| C++ symbol or file | Python equivalent | What happened to the concept |
|---|---|---|
| `Board` | | |
| `Position` | | |
| `Knight` | | |
| `MoveGenerator` | | |
| `RandomSequence` | | |
| `TranscriptPort` and adapters | | |
| `MoveOutcome` | | |
| `MoveResult` | | |
| `GameEngine::playTurn` | | |
| `parallel_runner.cpp` | | |
| `error_probe.cpp` | | |

### 2.3 The rules that must not move

| Rule | Where in C++ | Where in Python |
|---|---|---|
| LCG multiplier | | |
| LCG increment | | |
| 64-bit mask | | |
| Eight move offsets, in order | | |
| Starting move and rotation | | |
| Bounds check first | | |
| Capture before visited | | |
| Trap winner | | |
| 200-move draw | | |
| Transcript header lines | | |

- The three constants I found in both files:
- The first and last move offset in both files:
- Which file decides the process exit status, and through what call:

## 3. Prediction (written before measuring)

### 3.1 The structure table

| Question | My prediction |
|---|---|
| Files in the C++ baseline | |
| Files in the port | |
| Source lines in the C++ baseline | |
| Source lines in the port | |
| Headers in the port | |
| Build steps in the port | |
| Top-level definitions in `game.py` | |
| Golden traces that match | |

- The row I expect to get wrong, and why:
- Why removing classes does not remove functions:
- What `run.sh` does, in one sentence:

### 3.2 The behavior table

| Question | My prediction |
|---|---|
| Seeds that match, of 20 | |
| Traces that end in a capture | |
| Traces that end in a trap | |
| Traces that end in a draw | |
| Longest game, in moves | |
| Traces with at least one `illegal=visited` line | |

- How a game can end, according to the rules:
- Why a draw needs a long game:
- My guess for the longest game, and the reason:

### 3.3 The timing table

| Question | My prediction |
|---|---|
| 20 games through `run.sh` | |
| 20 games in one Python process | |
| 20 C++ processes | |
| 20 bare `python3 -c 'pass'` startups | |

- Which of these numbers I expect to dominate the `run.sh` figure:
- Why the in-process number is the fair comparison for the game work:
- What a benchmark would need that this lesson does not run:

### 3.4 The change-test table

| Question | My prediction |
|---|---|
| Traces that fail if capture is checked after visited | |
| Traces that fail if the trap winner is flipped | |
| Traces that fail if the draw limit is 199 instead of 200 | |

- Why a rule can only break a trace that reaches it:
- The seeds I expect to fail on the swap, and why:
- The seeds I expect to fail on the trap flip, and why:

## 4. Run — the port

### 4.1 Traces

- Command:
- Seeds that matched, of 20, on the unmodified port:
- First byte offset of any diff:
- The trace I read first, and what it contains:

### 4.2 The C++ side

- Command for the capture seed:
- Command for the trap seed:
- Diff output:
- What the two programs share, according to the diff:
- What the diff cannot show:

### 4.3 The oracle

- Command:
- Diff output:
- Why `reference.py` is an oracle:
- Why it is not an independent design:
- The renames I found in `diff -u`:
- What this match proves, and what it does not:

### 4.4 Timing

- 20 games through `run.sh`:
- 20 C++ processes:
- 20 bare Python startups:
- 20 games in one Python process:
- My prediction and the gap:
- The statement I can defend: startup dominates or work dominates?
- What I will not claim from these four numbers:

### 4.5 The argument path

- Command with a missing `--name`:
- Message:
- Exit status:
- Command with a valid game:
- Exit status:
- Which call produces the status:
- How this differs from `main` returning 2 in C++:

### 4.6 Prediction and observation

| Observation | Predicted | Observed | Gap |
|---|---|---|---|
| Files | | | |
| Source lines | | | |
| Top-level definitions | | | |
| Traces matched | | | |
| Capture-ending traces | | | |
| Trap-ending traces | | | |
| Draw-ending traces | | | |
| Longest game | | | |
| 20 games through `run.sh` | | | |
| 20 games in one process | | | |
| 20 C++ processes | | | |
| 20 Python startups | | | |

- The number that surprised me:
- What I did next instead of adjusting the prediction:

## 5. Compare — what survived

- What the rules look like in both files:
- What the representation looks like in the port:
- What happened to the four outcomes:
- What happened to the caller's decision from `construction-22`:
- What the compiler used to check, and what checks it now:
- What the traces prove:
- What the traces do not prove:

### 5.1 Three things that changed

1.
2.
3.

- Evidence for each:
- Which change is the biggest, and why:
- Which change is only a consequence of another:

### 5.2 Three things that did not change

1.
2.
3.

- Evidence for each:
- Why the LCG is the strongest evidence of a port:
- Why the check order is the second strongest:
- What else did not change:

### 5.3 What the port dropped

| Dropped | What it provided | What replaces it |
|---|---|---|
| `parallel_runner.cpp` | | |
| `error_probe.cpp` | | |
| `adapter_probe.cpp` | | |
| `RecordingTranscript` | | |
| `--max-moves` | | |

- Which dropped piece weakens the port's evidence the most:
- What the port cannot ask that the C++ baseline could:
- What the oracle keeps that the port does not:

### 5.4 The behavior diff

- Command:
- Output:
- Seeds compared:
- Why a structural diff is not the right tool here:

## 6. The change test — one rule, in the port only

- Scratch copy path:
- Files copied:
- Files created:
- Tracked files edited: (yes / no)
- Command that runs the copy:
- Command that compares with the traces:

### 6.1 Change A: capture after visited

- Lines moved:
- Seeds that failed:
- Count, of 20:
- Seeds that end in a capture:
- Are the two lists the same? (yes / no)
- Why a capture game can see the change:
- Why a trap game cannot:
- What the transcript shows at the first divergence:

### 6.2 Change B: trap winner flipped

- Line changed:
- Seeds that failed:
- Count, of 20:
- Seeds with `no_legal_move`:
- Are the two lists the same? (yes / no)
- Why the flip is invisible to every other trace:

### 6.3 Change C: draw limit 199

- Seeds that failed:
- Longest game in the traces:
- Why the limit is code and not evidence:

### 6.4 Restore

- Command:
- Seeds that matched after restoring, of 20:
- Did I have to revert a tracked file? (yes / no)

### 6.5 What the change test proves, and what it does not

- It proves about the traces as a detector:
- It proves about the port's guard:
- It proves about unreachable rules:
- It proves about the two implementations:
- It does not prove about unseen seeds:
- It does not prove about shared bugs:
- It does not prove about performance:

## 7. The port model and its limits

| Question | Answer |
|---|---|
| What must match exactly | |
| What is free to change | |
| What the transcript is | |
| What the rules are | |
| What the representation chose | |
| What the process promises | |
| Why 245 lines is not a quality measure | |
| Why the oracle is a cousin | |
| Why the draw is unreachable | |
| What the port cannot probe | |
| What startup does to the timing | |
| Which later lesson owns the capstone | |

- The four limits I can name:
- The limit that matters most for correctness:
- The limit that matters most for measurement:
- What I would add to the port to close one gap, without changing its behavior:

## 8. Explain

One sentence, without the words *syntax*, *language*, *translate*, *better*, *worse*, *cleaner* or
*faster*:

>

One paragraph:

- What changed:
- What stayed:
- What the change test showed:
- What the port dropped:
- What the numbers mean:
- What the traces prove:
- What they do not prove:
- Which later lesson owns the capstone:

The four things I kept apart:

## 9. Acceptance check

- I read `game.py`, `main.py` and `run.sh`: (yes / no)
- I counted 245 lines across 2 files and 1,071 across 20: (yes / no)
- I found the three LCG constants and the eight move offsets in both: (yes / no)
- I predicted the four tables before measuring: (yes / no)
- I reproduced all 20 traces, byte for byte: (yes / no)
- I diffed the port against C++ on a capture seed and a trap seed: (yes / no)
- I ran `reference.py` and explained the cousin problem: (yes / no)
- I measured the four timing numbers: (yes / no)
- I stated that startup dominates the Python figure: (yes / no)
- I ran the bad-argument path and recorded status 2: (yes / no)
- I named three things that changed and three that did not: (yes / no)
- I swapped capture and visited and saw 11 seeds fail: (yes / no)
- I flipped the trap winner and saw 9 seeds fail: (yes / no)
- I explained why the other seeds cannot see each change: (yes / no)
- I recorded that no golden trace reaches the 200-move draw: (yes / no)
- I restored the copy and confirmed 20 of 20: (yes / no)
- I did not edit any tracked code file: (yes / no)

If I edited a tracked code file, what and why:

## 10. Reflection and next step

- What I can now explain without notes:
- What I cannot explain yet:
- One thing I would read again:
- The number I will carry to `construction-24`:
- The prediction that was furthest from the observation:
- What I expected Python to make easier that it did not:
- What I expected Python to make harder that it did not:
- What the C++ baseline still tells me that the port cannot:
- Which dropped probe I would rebuild first, and why:
- What a third language would test that a second one did not:
- Next step:

## 11. If something is missing or unfaithful

Fill this only if a file was absent, a run failed for an environmental reason, a diff was not empty,
or a measurement contradicted the documented behavior.

- Missing file, run error, diff or contradiction:
- Exact command output:
- What I did instead of inventing a count:
- When I will retry:

## 12. Self-check

- [ ] I inventoried the port: files, lines, definitions and the rules that must not move.
- [ ] I named the four layers and which one must match exactly.
- [ ] I predicted the structure, behavior, timing and change-test tables.
- [ ] I reproduced all 20 traces on the unmodified port.
- [ ] I compared the port with the C++ baseline on two seeds.
- [ ] I ran the oracle and stated the cousin problem.
- [ ] I measured the four timing numbers and separated startup from work.
- [ ] I ran the bad-argument path and recorded the owner of the status.
- [ ] I named three things that changed and three that did not.
- [ ] I broke one rule in a scratch copy and located the failing seeds.
- [ ] I explained why the other seeds cannot see the change.
- [ ] I named the unreachable draw and the dropped probes.
- [ ] I restored the copy and confirmed 20 of 20.
- [ ] My one-sentence explanation avoids the banned words.
- [ ] I did not edit any tracked code file.

No score. If a box is unchecked, write the next smallest action in §10.
