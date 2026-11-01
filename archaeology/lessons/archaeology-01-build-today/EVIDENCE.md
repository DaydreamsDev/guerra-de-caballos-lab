# archaeology-01 — Evidence: build and run

> This is your record. Write it for your future self.
> Record what you observed, not what you expected the lesson to want.

## 1. Setup

- Date:
- Archive path:
- Tag checked out:
- Commit hash (`git rev-parse HEAD`):
- Source file and line count (`wc -l`):
- Compiler and version (`c++ --version`):
- Shell and version:
- `timeout --version` first line:
- Kernel and distribution (`uname -sr`):
- Locale used for compiler messages (`echo $LC_ALL`):
- Working directory:
- Anything unexpected in the setup:

## 2. Prediction — write before the first build

Do not edit this table after Step 5. The gap between prediction and observation is the learning
signal; repairing the prediction destroys it.

| # | Question | My prediction | How I would know it is wrong |
|---|---|---|---|
| 1 | Interface: one screen or a log? | | |
| 2 | Interaction: does it wait for input? | | |
| 3 | Randomness: reproducible twice? | | |
| 4 | End condition and final message | | |
| 5 | First thing to break on Linux | | |

One falsifiable sentence about the whole set:

>

## 3. Naive build — the error ladder

Copy the source out of the archive before compiling. Record the copy command.

- Copy command:
- Compile command 1 (no shims):
- Exit status:

Paste the output verbatim:

```text

```

- Compile command 2 (headers satisfied):
- Exit status:

Paste the output verbatim:

```text

```

- Compile command 3 (any further stage I needed):

Paste the output verbatim:

```text

```

### 3.1 Error table

| # | Error text (first line only) | File:line | Class: missing header / language rule / other |
|---|---|---|---|
| 1 | | | |
| 2 | | | |
| 3 | | | |

- Did the order of errors match the order of includes in the file? yes / no
- If no, why not:

### 3.2 Warnings

- Command used to collect warnings (for example `-Wall -Wextra`):
- Warning count:
- Warning classes and counts:
- Which warning I will hand to `archaeology-04` instead of diagnosing now:

## 4. Compatibility changes

Every change is a build-level adaptation. None of them edits the archive. Fill one row per change.

| Change | Where it lives | What it does | Risk it introduces |
|---|---|---|---|
| `code/compat/iostream.h` | lesson file | | |
| `code/compat/conio.h` | lesson file | | |
| `code/legacy_wrapper.cpp` | lesson file | | |
| `-fpermissive` | compile flag | | |
| `-Dmain=legacy_main` | compile flag | | |
| `-Icode/compat` | compile flag | | |

- Which change alters program behavior most, and how:
- Which change I would remove first if I wanted a real port, and why:

## 5. Probe build and run

- Command:
- Exit status of the script:

Paste all six output lines verbatim:

```text
source=
compile=ok
run_status=
stdout_lines=
stderr_lines=
stderr_first=
```

- What `run_status` means here:
- What `124` would have meant:
- The first `stderr` line in my locale:
- The first `stderr` line with `LC_ALL=C` (if I checked):

### 5.1 Run record

| Run | Wall-clock second | stdout lines | stderr lines | Exit status | Digest (`sha256sum`) |
|---|---|---|---|---|---|
| 1 | | | | | |
| 2 (same second as 1) | | | | | |
| 3 (after waiting 2 s) | | | | | |

## 6. Observations

Answer each with the command you used, not with an impression.

### 6.1 Exit behavior

- Does the program reach its own end without input? yes / no
- Evidence:
- How long it took (`time`):

### 6.2 Variance

- Two causes of varying line counts that I can name:
- Which one dominates, and how I know:

### 6.3 Seed clock

- Two runs in the same second: identical / different
- Two runs in different seconds: identical / different
- The line that explains it, quoted:

### 6.4 Encoding

- Command used to inspect the bytes:
- The bytes I found for the wall, the knight and the separator:
- What a UTF-8 terminal shows instead:
- Are these bytes valid UTF-8? yes / no

### 6.5 Screen vs log

- Number of `cls` failures in `stderr`:
- Number of turns in the run:
- Do they match? yes / no
- What the output would look like if `cls` worked:

## 7. Compare — verdict per prediction

The verdict is exactly one of: confirmed, contradicted, unresolved.

| # | Predicted | Observed | Verdict |
|---|---|---|---|
| 1 | | | |
| 2 | | | |
| 3 | | | |
| 4 | | | |
| 5 | | | |

- Which row was the most useful surprise, and why:
- Which row I still cannot decide, and what I would need to observe:

## 8. Evidence boundary

One paragraph, without syntax words: what the build proved, what it did not prove, where the
evidence stops.

>

### 8.1 The four claims the probe does not support

For each, write one line saying why this run cannot support it.

| Claim | Why this run cannot support it |
|---|---|
| The code is portable | |
| The output is correct | |
| The historical toolchain was Visual C++ 6.0 | |
| This reproduces the modern transcript | |

### 8.2 The honest sentence

Write one sentence you would defend in a code review.

>

## 9. Uncertainties

| Open question | Confidence | What evidence would change my mind |
|---|---|---|
| | | |
| | | |
| | | |

## 10. Self-check

- [ ] I wrote five predictions before running anything.
- [ ] I quoted the naive build's errors verbatim, in order, with line numbers.
- [ ] I listed every compatibility change with its risk.
- [ ] I ran `build_probe.sh` and recorded all six lines.
- [ ] I recorded `run_status` and know what `124` would have meant.
- [ ] I named two causes of varying line counts.
- [ ] I proved that two runs in the same second are identical, and that this is not determinism.
- [ ] I confirmed the board is raw CP437 bytes, not UTF-8.
- [ ] I compared my prediction with the observation, with a verdict per row.
- [ ] I wrote the boundary paragraph and the four unsupported claims.
- [ ] The archive is unmodified (`git status` in the archive is clean).

## 11. Next step

- What I can explain now:
- What I cannot explain yet:
- Next lesson or repeat:
- The defect I noticed and deliberately did not chase:

## 12. Optional note

- Anything else:

---

> Reminder: this file is evidence for you.
