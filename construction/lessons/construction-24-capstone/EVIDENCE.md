# construction-24 — Evidence

> [English](EVIDENCE.md) | [Español](EVIDENCE-es.md)

> Learner-owned record. Write in English.

This record follows one change through two rungs. It keeps four things apart: the prediction, the
route, the measurement and the verdict. Keep the predictions. The gap between your prediction and
the observation is the result, whether it is zero or not.

## 1. Run setup

- Date:
- Repository root:
- Commit copied, or date if there is no commit (`git rev-parse HEAD`):
- Compiler and version (`g++ --version` or `clang++ --version`):
- Optimization flags:
- Platform and architecture:
- Pair chosen (A and B):
- Scratch directory for A:
- Scratch directory for B:
- Time budget for the change:
- Construction-12 record at hand? (yes / no)
- Construction-15 record at hand? (yes / no)
- Did I edit any tracked file? (yes / no)

## 2. The pair and the baselines

### 2.1 Why this pair

Finish the sentence: "I chose ______ and ______ because they hold ______ constant and vary
______."

- The variable I claim to isolate:
- The variables I am not holding constant, and why that is acceptable:
- If I compared 12 and 15, the three variables that change at once:
- The pair I rejected, and why:

The change, written before any edit:

- Third piece name:
- Third piece operations:
- Third piece movement:
- How I will prove it travels through the same loop:
- How the 10×10 entry point supplies dimensions:
- The default 8×8 entry point:
- What must not move:

### 2.2 Baseline measurements

Measured before the first edit, with `./measure.sh`:

| Rung | Lesson directory | `measure.sh` output | Files | Lines | Traces |
|---|---|---|---:|---:|---|
| A | | | | | |
| B | | | | | |

- Default binary text bytes, A:
- Default binary text bytes, B:
- Compile time, median of 3, A:
- Compile time, median of 3, B:
- Did I remove `build/` from both copies before measuring? (yes / no)

### 2.3 The two designs before the change

| Question | Rung A | Rung B |
|---|---|---|
| How the loop names the piece type | | |
| Where the movement geometry lives | | |
| How the board stores its dimensions | | |
| Board storage bytes | | |
| Where the 8×8 constant is written | | |
| Which build files list the sources | | |
| Does `run.sh` rebuild on a header change? | | |
| What a third piece costs in this design, in one sentence | | |

## 3. Prediction (written before measuring)

### 3.1 The cost prediction

| Metric | Rung A | Rung B | Confidence |
|---|---:|---:|---|
| Files touched | | | |
| Lines added | | | |
| Lines removed | | | |
| New concepts required | | | |
| Traces broken on the first green build | | | |
| Compile time, median of 3 (seconds) | | | |
| Binary text bytes | | | |
| Golden traces passed at the end | | | |

- The row I marked `high` and still expect to miss:
- The row I marked `low` and why:
- The number I expect to decide the verdict:
- The number I expect to come closest to reversing it:

### 3.2 The route prediction

| Question | My prediction |
|---|---|
| Third piece route in A | |
| Third piece route in B | |
| 10×10 route in A | |
| 10×10 route in B | |
| Files I expect to touch in A | |
| Files I expect to touch in B | |
| Which half dominates in A: piece or board? | |
| Which half dominates in B: piece or board? | |
| Build files I expect to edit | |

### 3.3 The verdict prediction

| Question | My prediction |
|---|---|
| Which rung I expect to be cheaper | |
| The number that I expect to decide it | |
| The number that could reverse it | |
| The cost I expect the table not to show | |
| The prediction I am least sure about | |
| What I expect to surprise me | |

- The condition under which my verdict flips:
- Why I am not writing "it depends":

## 4. The change as applied

### 4.1 Route log for rung A

| Checkpoint | Command | Result | Files touched so far | Traces |
|---|---|---|---|---|
| Baseline | `./measure.sh` | | 0 | |
| A1 third piece compiles | | | | |
| A2 traces after the piece | | | | |
| A3 10×10 runs | | | | |
| A4 traces after the board | | | | |

- Time to first green build, A:
- Number of build attempts before the first green, A:
- Route changes in A:
- What the first failing build said:

### 4.2 Route log for rung B

| Checkpoint | Command | Result | Files touched so far | Traces |
|---|---|---|---|---|
| Baseline | `./measure.sh` | | 0 | |
| B1 third piece compiles | | | | |
| B2 traces after the piece | | | | |
| B3 10×10 runs | | | | |
| B4 traces after the board | | | | |

- Time to first green build, B:
- Number of build attempts before the first green, B:
- Route changes in B:
- What the first failing build said:
- If B did not reach a green build: the last error and the route I tried:

### 4.3 The third piece

- Type name in A:
- Type name in B:
- The four operations, and where each is declared:
- How it is constructed:
- How I proved it travels through the same loop:
- Did the 8×8 transcript stay byte-identical? (yes / no)
- Did I keep the substitution or restore the original?
- What the build file needed:
- What the compiler emitted for the new type (`nm -C`, one line):

### 4.4 The 10×10 entry point

- File in A:
- File in B:
- How dimensions reach the board:
- How the same turn loop is reused:
- Seed 1, A, head and tail:

```text
<A: first six lines and last two lines>
```

- Seed 1, B, head and tail:

```text
<B: first six lines and last two lines>
```

- Are the initial coordinates in 1..10? (yes / no)
- Does the format match `transcript-format.md`? (yes / no)
- Is there a golden trace for 10×10? (yes / no)

### 4.5 What did not change

- The 20 traces on the 8×8 path, A:
- The 20 traces on the 8×8 path, B:
- The transcript format:
- The rules I did not touch:
- The tracked files I did not touch:

## 5. Measurement

### 5.1 Files and lines

Command used:

```bash
<the numstat command>
```

Raw output for A:

```text
<A raw output>
```

Raw output for B:

```text
<B raw output>
```

| Metric | A | B |
|---|---:|---:|
| Code files edited | | |
| Code files added | | |
| Build files edited | | |
| Files touched, total | | |
| Lines added | | |
| Lines removed | | |

- Do the `--numstat` totals include comments and blank lines? (yes / no)
- Does `measure.sh` count the 10×10 entry point? (yes / no)
- Why the size of the copy is not the same as the delta I caused:

### 5.2 New concepts

List each concept, the rung that needed it, and the line that uses it. A count without a list is not
accepted.

| Concept | A or B | File and line | Why it is new for this rung |
|---|---|---|---|
| | | | |
| | | | |
| | | | |

- Count for A:
- Count for B:
- The concept that took the longest to understand:
- The concept I expected but did not need:

### 5.3 Tests broken and traces passed

- First green build, A: seeds failed, of 20:
- First green build, B: seeds failed, of 20:
- Final count, A: passed, of 20:
- Final count, B: passed, of 20:
- The first failing seed in A, and the first differing line:
- The first failing seed in B, and the first differing line:
- What the failure told me about which path moved:
- The command that produced the final count:

### 5.4 Compile time

- Compiler and version:
- Flags:
- Source list used (default program only):
- A, run 1 / run 2 / run 3:
- A, median:
- B, run 1 / run 2 / run 3:
- B, median:
- Did both sides use the same build route? (yes / no)
- If not, what the difference includes:
- Why compile time is a weak signal on a program this size:

### 5.5 Binary text

Command used:

```bash
<the size command>
```

- A, text / data:
- B, text / data:
- Direction of the change, and why:
- Which binary I measured (default, not 10×10):
- What the number does not include:

### 5.6 Prediction versus observation

| Metric | Predicted A | Observed A | Gap | Predicted B | Observed B | Gap |
|---|---:|---:|---:|---:|---:|---:|
| Files touched | | | | | | |
| Lines added | | | | | | |
| Lines removed | | | | | | |
| New concepts | | | | | | |
| Tests broken | | | | | | |
| Compile time | | | | | | |
| Binary text | | | | | | |
| Golden traces | | | | | | |

- The largest gap, and its mechanical cause:
- The gap I did not expect:
- What I did next instead of adjusting the prediction:

## 6. The comparison

| Metric | Rung A | Rung B |
|---|---:|---:|
| Files touched | | |
| Lines added | | |
| Lines removed | | |
| New concepts required | | |
| Tests broken | | |
| Compile time (seconds) | | |
| Binary text bytes | | |
| Golden traces passed | | |

### 6.1 Which half dominated

- In A, the piece cost:
- In A, the board cost:
- In B, the piece cost:
- In B, the board cost:
- The half that decided the comparison:
- The half that did not, and why it still belongs in the table:

### 6.2 Which metric decided

- The deciding metric:
- Its value in A and in B:
- Why this metric and not lines of code:
- The metric that came closest to reversing the result:
- How close it came:

### 6.3 What the table does not show

- Time to first green, A and B:
- Build attempts, A and B:
- Route changes:
- Conceptual load:
- Risk a reviewer would have to re-check by hand:
- What I did not measure:
- Which of these changed my confidence:

## 7. Verdict and defense

- Verdict, one sentence, with the deciding number and the scope:
- The number that decided it:
- The number that nearly reversed it:
- What would reverse the verdict, as a concrete next change:
- Which rung I would choose for that next change, and why:

### 7.1 The three defense questions

1. Why is the comparison fair?
2. What would reverse the verdict?
3. What did you not measure?

### 7.2 The next change, predicted

| Metric | Current change, winner | Next change, predicted |
|---|---:|---:|
| Files touched | | |
| Lines added | | |
| Lines removed | | |
| New concepts required | | |
| Tests broken | | |
| Golden traces passed | | |

- The next change:
- Why it is the right test of this verdict:
- What result would falsify my verdict:

## 8. Explain

One sentence, without the words *better*, *worse*, *cleaner*, *more flexible*, *more reusable*,
*more elegant*, *modern* or *proper*:

>

One paragraph:

- What the change was:
- What the pair held constant:
- What it varied:
- What the deciding number was:
- What the 20 traces prove:
- What the traces do not prove:
- What the 10×10 run proves:
- What the third piece proves:
- What the table does not show:
- What I would choose next, and why:

The four things I kept apart:

## 9. Acceptance check

- I chose a pair and finished the "holds constant / varies" sentence: (yes / no)
- I copied two rungs to scratch directories and removed `build/`: (yes / no)
- I recorded the commit and both baseline `measure.sh` outputs: (yes / no)
- I wrote the prediction, with confidence, before the first edit: (yes / no)
- I applied the same third piece and the same 10×10 entry point to both: (yes / no)
- The third piece travels through the same turn loop: (yes / no)
- The 10×10 entry point runs and prints the transcript format: (yes / no)
- Both copies pass 20/20 on the 8×8 path: (yes / no)
- I measured files and lines with `--numstat`, including build files: (yes / no)
- I measured compile time as the median of three clean builds: (yes / no)
- I recorded binary text bytes for the default program: (yes / no)
- I recorded tests broken separately from the final 20/20: (yes / no)
- The report has all six sections and fits on one page: (yes / no)
- The verdict names the deciding number and the near-reversal: (yes / no)
- The next change is named and predicted in the same table: (yes / no)
- I did not edit any tracked file: (yes / no)

If I edited a tracked file, what and why:

## 10. Reflection and next step

- What I can now explain without notes:
- What I cannot explain yet:
- One thing I would read again:
- The number I will carry to the next lens:
- The prediction that was furthest from the observation:
- What the pair taught me that a single rung could not:
- What I would measure differently next time:
- The change I would make next, and its predicted cost:
- Next step:

## 11. If something is missing or unfaithful

Fill this only if a rung was absent, a build failed for an environmental reason, a diff was not
empty, a trace failed in a way you could not explain, or a measurement contradicted the documented
behavior.

- Missing file, build error, diff or contradiction:
- Exact command output:
- What I did instead of inventing a count:
- When I will retry:

## 12. Self-check

- [ ] I chose a pair whose difference I can attribute to one variable.
- [ ] I copied two complete lesson directories and removed `build/`.
- [ ] I measured both baselines before editing.
- [ ] I wrote the cost, route and verdict predictions before the first edit.
- [ ] I applied the same change to both copies.
- [ ] I recorded the route, not only the result.
- [ ] I proved the third piece travels through the same loop.
- [ ] I ran the 10×10 entry point and checked the format.
- [ ] I confirmed 20/20 on the 8×8 path in both copies.
- [ ] I measured files, lines, concepts, broken tests, compile time and text bytes.
- [ ] I kept "tests broken" and "traces passed" as two numbers.
- [ ] I wrote the one-page report with all six sections.
- [ ] The verdict names the deciding number and the near-reversal.
- [ ] The next change is predicted in the same table.
- [ ] I separated the prediction, the route, the measurement and the verdict.
- [ ] My one-sentence explanation avoids the verdict words.
- [ ] I did not edit any tracked code file.

No score. If a box is unchecked, write the next smallest action in §10.
