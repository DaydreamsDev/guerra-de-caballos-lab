# archaeology-00 — Evidence: orientation

> This is your record. Write it for your future self.
> Record what you observed, not what you expected the lesson to want.

## 1. Setup

- Working directory:
- Source path:
- Source line count (`wc -l`):
- Compiler and version (`c++ --version`):
- Shell and version:
- Scratch directory used for the build:
- Anything unexpected in the setup:

## 2. Prediction — write before reading any function body

Do not edit this table after Step 5. The gap between prediction and observation is the learning
signal; repairing the prediction destroys it.

| # | Question | My prediction | How I would know it is wrong |
|---|---|---|---|
| 1 | Purpose in one line | | |
| 2 | Interface: screen, log or no output | | |
| 3 | Which declarations are mutable state | | |
| 4 | What starts and ends the program | | |
| 5 | First thing to break on my machine | | |

One falsifiable sentence about the whole set:

>

## 3. Entry point

- File:
- Function:
- First call:
- Last call:
- What starts the program:
- What ends the program:
- How control leaves the program:
- Evidence (line numbers):

## 4. Functions

One row per function. Use names in the relationship columns and line numbers in the last column.

| Function | Purpose | Called by | Calls | Reads | Writes | Lines |
|---|---|---|---|---|---|---|
| | | | | | | |
| | | | | | | |
| | | | | | | |
| | | | | | | |
| | | | | | | |
| | | | | | | |
| | | | | | | |
| | | | | | | |

Add rows until every function has one. A function you cannot explain gets `?` in the purpose column
and a confidence label in the last column.

## 5. State

One row per file-scope name. Separate mutable state from constants.

| Global or data structure | Type | Written by | Read by | Why it exists | Confidence |
|---|---|---|---|---|---|
| | | | | | |
| | | | | | |
| | | | | | |
| | | | | | |
| | | | | | |
| | | | | | |

Constants excluded from the state table, and why:

- Names:
- Type:
- Why they are not state:

## 6. Data flow

- Input:
- Transformation:
- Output:
- Where state is mutated:
- Where output is produced:

One value traced from input to output. Write one line per arrow, with a line number.

```text

```

## 7. Build and run

- Build command:
- Build result (builds / builds but does not run / does not build):
- First observable result, verbatim:

```text

```

- Failure point (file:line):
- Run command, if a binary was produced:
- Exit status:
- First output line:
- First error line:
- What `124` would have meant:

## 8. Mental model

- One sentence: what is this program?
- The part I understand best:
- The part I cannot explain yet:
- The next question I would answer:
- Why answering it would change at least one row of the map:

### 8.1 Prediction verdicts

| # | Predicted | Map or build evidence | Verdict: confirmed / contradicted / unresolved |
|---|---|---|---|
| 1 | | | |
| 2 | | | |
| 3 | | | |
| 4 | | | |
| 5 | | | |

- Which row was the most useful surprise, and why:

### 8.2 The sentence test

| Test | My answer |
|---|---|
| Prediction: if I change the move order, what changes? | |
| Compression: does the sentence cover entry, state and output? | |
| Falsifiability: which line would prove it wrong? | |

## 9. Evidence boundary

| Claim | Status: read / ran / inferred / unknown | Evidence or reason |
|---|---|---|
| The entry point is at the cited line. | | |
| The cited function writes the cited global. | | |
| The program's purpose is the sentence I wrote. | | |
| The build result holds on another machine. | | |

## 10. Self-check

- [ ] I wrote five predictions before reading any function body.
- [ ] I read `main` completely and named what starts and ends the program.
- [ ] I grouped the prototypes by job and corrected the grouping after reading.
- [ ] I read one case of `movcab` and the `default` branch, and recorded the repetition.
- [ ] Every function has a row with purpose, callers, callees, reads and writes.
- [ ] Every mutable global has a row with type, writers, readers and a reason to exist.
- [ ] I separated the `const char` print symbols from mutable state.
- [ ] I traced one value from input to output with a line number per arrow.
- [ ] I attempted the build and recorded the command and the first observable result.
- [ ] I wrote the one-sentence purpose and tested it three ways.
- [ ] I wrote a next question that names a symbol, a file or a line.
- [ ] I labelled every uncertain claim as inferred or unknown.
- [ ] The source is unmodified and no build artifact was written into its tree.

## 11. Next step

- What I can explain now:
- What I cannot explain yet:
- Next lesson or repeat:
- The observation I parked instead of chasing:

## 12. Optional note

- Anything else:

---

> Reminder: this file is evidence for you.
