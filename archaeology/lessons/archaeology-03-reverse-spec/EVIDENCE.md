# archaeology-03 — Evidence: reverse-engineer the spec

> This is your record. Write it for your future self.
> Record what you observed, not what you expected the lesson to want.

## 1. Setup

- Date:
- Archive path:
- Tag checked out:
- Commit hash (`git rev-parse HEAD`):
- Source file and line count (`wc -l`):
- Compiler and version (`c++ --version`):
- C library and version (`ldd --version` first line):
- Shell and version:
- `timeout --version` first line:
- Kernel and distribution (`uname -sr`):
- Working directory:
- Anything unexpected in the setup:

## 2. Prediction — write before opening the source

Do not edit this table after Step 4. The gap between prediction and observation is the learning
signal; repairing the prediction destroys it.

| # | Question | My prediction | How I would know it is wrong |
|---|---|---|---|
| 1 | Board and pieces: size, count, display | | |
| 2 | Start: who begins and where the knights start | | |
| 3 | Move priority: how the eight moves are ordered | | |
| 4 | Randomness: generator, seed and reproducibility | | |
| 5 | End: how the game finishes and what it prints | | |

One falsifiable sentence about the whole set:

>

## 3. Source inspection — rule ledger

Read the source in execution order. Add one row per behavior. Use the marks **verified**,
**inferred** or **unknown**.

| # | Rule | Lines | Evidence | Mark |
|---:|---|---:|---|---|
| 1 | Board dimensions | | | |
| 2 | Cell numbering and displayed coordinates | | | |
| 3 | Piece count and symbols | | | |
| 4 | Starting positions | | | |
| 5 | Who starts | | | |
| 6 | Turn alternation | | | |
| 7 | Move order generation | | | |
| 8 | The eight knight offsets | | | |
| 9 | Legal move: inside the board | | | |
| 10 | Legal move: visited cell | | | |
| 11 | Capture | | | |
| 12 | Trap | | | |
| 13 | Draw | | | |
| 14 | Random generator and seed | | | |
| 15 | Output format | | | |

### 3.1 The three layers

| Layer | What I found | Lines |
|---|---|---|
| Game rules | | |
| Randomness | | |
| Presentation | | |

### 3.2 Findings that changed my reading

For each, write the line and the surprise.

- Starting cells marked as visited:
- Move order drawn per pair of turns:
- Roll range:
- Seed reset inside the retry loop:
- No draw rule:
- Other:

### 3.3 Ambiguities

| Ambiguity | What the code does | What the comment claims | Which one I used and why |
|---|---|---|---|
| | | | |
| | | | |

## 4. Capture record

- Command:
- Seed:
- Output file:
- Exit status of the script:

Paste the five output lines verbatim:

```text
seed=
output=
run_status=
stdout_lines=
stderr_lines=
```

- What `run_status` means here:
- What `124` would have meant:
- The first `stderr` line in my locale:
- The first `stderr` line with `LC_ALL=C` (if I checked):
- Number of `color` failures:
- Number of `cls` failures:
- Do they match the turn count? yes / no

### 4.1 Determinism check

| Run | Seed | Output file | stdout lines | `sha256sum` |
|---|---:|---|---:|---|
| 1 | | | | |
| 2 (same seed) | | | | |

- Do the two hashes match? yes / no
- What this proves:
- What this does not prove:

### 4.2 Second seed

| Seed | `run_status` | stdout lines | stderr lines | End: capture / trap / timeout |
|---:|---:|---:|---:|---|
| | | | | |

- What changed between the two seeds:
- What stayed the same:
- What a third seed would test:

### 4.3 Collision seed

- Seed I tried that returned `124` (if any):
- Why it returned `124`:
- Is this a property of the original or of the probe? Explain:

## 5. Observed trace

### 5.1 Start

- Roll for knight 1:
- Roll for knight 2:
- First knight:
- Initial position of knight 1 (index and row,column):
- Initial position of knight 2 (index and row,column):
- Which starting cell holds move number 1:
- Which starting cell holds move number 2:

### 5.2 First moves

Read the board and the `evaluo los casos` lines. Reconstruct the first three moves.

| Turn | Active piece | From (row,col) | Move number | Destination (row,col) | Outcome |
|---:|---|---:|---:|---|---|
| 1 | | | | | |
| 2 | | | | | |
| 3 | | | | | |

- The order vector for turn 1:
- The order vector for turn 2:
- Are they the same? yes / no
- What that shows about the draw frequency:

### 5.3 End

- Winner or trapped knight:
- Last move number on the board:
- Number of actual knight moves (last number minus 2):
- End reason:
- Raw evidence (quote the line):

### 5.4 Output facts

- Encoding of the board symbols:
- Bytes for knight 1, knight 2 and the wall:
- Is the output valid UTF-8? yes / no
- Does the output contain turn numbers? yes / no
- Does the output contain coordinates in `row,column` form? yes / no
- Does the output contain a seed? yes / no
- What a transcript would need that this output lacks:

## 6. Comparison with the modern spec

Open `common/spec/game-rules.md` only after section 5 is complete.

### 6.1 What matches

| Rule | Legacy evidence | Modern clause |
|---|---|---|
| | | |
| | | |
| | | |

### 6.2 What differs

Classify each row as **encoding**, **rule** or **unobservable**.

| # | Modern spec | Legacy source | Lines | Class | Why it matters |
|---:|---|---|---:|---|---|
| 1 | | | | | |
| 2 | | | | | |
| 3 | | | | | |
| 4 | | | | | |
| 5 | | | | | |

### 6.3 The randomness boundary

One paragraph: why the same seed does not produce the same game in both implementations.

>

- Legacy generator:
- Modern generator:
- Draw order in the legacy program:
- Draw order in the modern spec:
- Consequence for trace comparison:

### 6.4 Is the legacy trace a golden trace?

- Yes / no:
- Why:
- What would be needed to make a fair comparison:

## 7. Evidence limits

### 7.1 The four categories

| Category | Claims I put here | Why |
|---|---|---|
| Verified from source | | |
| Observed in a trace | | |
| Inferred from structure | | |
| Unknown | | |

### 7.2 The four claims the trace does not support

| Claim | Why this trace cannot support it |
|---|---|
| This is what the 2008 program printed | |
| The modern engine can reproduce this trace | |
| The game is deterministic | |
| The rules are correct | |

### 7.3 The honest sentence

Write one sentence you would defend in a code review.

>

## 8. Confidence summary

| Claim | Confidence: high / medium / low | What would change my mind |
|---|---|---|
| | | |
| | | |
| | | |

- The claim I am most confident about, and why:
- The claim I am least confident about, and why:
- The question I cannot answer from this archive:

## 9. Self-check

- [ ] I wrote five predictions before opening the source.
- [ ] I read the source in execution order and built a rule ledger with line citations.
- [ ] I separated game rules, randomness and presentation.
- [ ] I recorded that the starting cells are marked as visited.
- [ ] I recorded that the move order is drawn once per pair of turns.
- [ ] I recorded that the rolls are 0..50, not 1..50.
- [ ] I explained why `srand(time(NULL))` is inside the retry loop.
- [ ] I captured at least one trace with `capture_trace.sh` and recorded all five lines.
- [ ] I ran the same seed twice and confirmed identical bytes.
- [ ] I ran a second seed and recorded how the line counts changed.
- [ ] I explained the collision seed as a probe artifact.
- [ ] I read the CP437 board and reconstructed the first moves.
- [ ] I compared the reconstructed spec with `common/spec/game-rules.md`.
- [ ] I classified each difference as encoding, rule or unobservable.
- [ ] I stated the randomness boundary.
- [ ] I filled `SPEC-TEMPLATE.md` and `TRACE-TEMPLATE.md`.
- [ ] The archive is unmodified (`git status` in the archive is clean).

## 10. Next step

- What I can explain now:
- What I cannot explain yet:
- Next lesson or repeat:
- The defect I noticed and deliberately did not chase:
- The seed I want to try next, and why:

## 11. Optional note

- Anything else:

---

> Reminder: this file is evidence for you.
