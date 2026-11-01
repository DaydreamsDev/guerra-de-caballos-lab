# archaeology-18 — Evidence: refactor workshop

> This is your record. Write it for your future self.
> Record what you observed, not what you expected the workshop to want.
>
> The report (`WORKSHOP-REPORT.md`) argues the case. This file proves it. Every claim in the report
> should have a command and an output here.

## 1. Setup

- Date:
- Archive path:
- Tag checked out:
- Commit hash (`git rev-parse HEAD`):
- Compiler and version (`c++ --version`):
- Build flags used:
- `git` version:
- Sanitizer available? (`c++ -fsanitize=address` on a one-line file):
- Scratch directory used:
- Shell and version:
- Anything unexpected in the setup:

## 2. Choice

| Field | Value |
|---|---|
| Failure chosen (lesson + failure ID) | |
| Archive location (`variant:line`) | |
| Failure class | |
| Target rung | |
| Rung's change test, copied from `lesson-catalog.md` | |

One sentence: why this failure:

>

One sentence: why this rung:

>

The defect sentence, in the shape "the program does X because Y is not in Z":

>

Check: does the rung touch `Y` or `Z`? If not, the pairing is wrong.

## 3. The bench

| Question | Answer |
|---|---|
| What region of the archive did I transcribe? | |
| What did I change to make it compile? | |
| What input did I control (seed, name, scripted moves, scripted clock)? | |
| What did I deliberately **not** repair? | |
| Where does the transcript go? | |

The transcription, in one line per event:

```cpp

```

If the defect is non-determinism, how I controlled the input without deleting the defect:

>

Two runs, same input:

```bash
./bench 17 > run-1.txt
./bench 17 > run-2.txt
cmp run-1.txt run-2.txt
```

- `cmp` exit code:
- Deterministic? (must be yes):

What would have made the bench a demo instead of a baseline:

>

## 4. Trace A

```bash
./bench 17 > trace-a.txt
sha256sum trace-a.txt
```

- Command:
- SHA-256 digest:
- Byte count (`wc -c < trace-a.txt`):
- Line count (`wc -l < trace-a.txt`):
- Commit that recorded it:
- Did I edit `trace-a.expected.txt` at any point? (must be no):

## 5. Coupling counts (before)

| Metric | Command | Value |
|---|---|---:|
| Files in the artifact | `ls *.cpp *.h \| wc -l` | |
| Lines in the artifact | `cat *.cpp *.h \| wc -l` | |
| Public names a caller must know | declarations in headers | |
| Files that must be read to answer "how is a cell stored?" | `grep -rln` | |

For the fourth row, the same number can mean two opposite things:

- the file that must be read is the **caller**: the representation leaked;
- the file that must be read is the **implementation**: the representation is hidden.

Which one do I have:

>

The same counts from the archive, for comparison:

| Metric | Archive | My bench |
|---|---:|---:|
| Files | | |
| Lines | | |
| Lines that mention the storage name | | |
| Headers | | |

## 6. Plan and prediction

Files to copy:

Files to change:

Files to create:

Untouched:

The prediction, written before the refactor and not edited afterwards:

| Metric | Predicted | Why that number |
|---|---:|---|
| Files touched | | |
| Lines added / removed | | |
| Public names after | | |
| Tests after | | |
| Build time | | |

The rung's change test, restated as a command I can run:

>

## 7. Refactor step log

One row per committed step. Every row must show a green trace.

| Step | Commit | What changed | Files touched | Trace A |
|---:|---|---|---:|---|
| 1 | | | | |
| 2 | | | | |
| 3 | | | | |
| 4 | | | | |
| 5 | | | | |

Steps I reverted, and why:

| Step | Symptom | What I reverted to |
|---:|---|---|
| | | |

Did I revert a step or the whole refactor? (the first is correct):

>

The command I ran after each step:

```bash
c++ -std=c++20 -Wall -Wextra -Werror -O2 *.cpp -o bench-new
./bench-new 17 | cmp - trace-a.expected.txt
```

## 8. Verification

### Build

- Command:
- Diagnostics:
- Did `-Werror` stop anything? What?

### Traces

```bash
./bench-new 17 | cmp - trace-a.expected.txt
diff -u trace-a.expected.txt <(./bench-new 17) || true
```

- `cmp` exit code:
- `diff` output:
- If the artifact is the whole game, golden traces matched: ___ of 20

### Determinism

- Two runs identical? (must be yes):
- Command:

### Sanitizer

```bash
c++ -std=c++20 -g -O1 -fsanitize=address,undefined *.cpp -o bench-asan
./bench-asan 17 > asan-out.txt 2> asan-err.txt
```

- Did `asan-out.txt` match trace A?
- Sanitizer report before the refactor:
- Sanitizer report after the refactor:
- Did the report change? If yes, which of the two explanations applies:
  - I fixed the defect inside the refactor (the cost table is wrong);
  - I moved the write where the sanitizer cannot see it (worse than the original).

### The correctness fork

Only if the chosen failure is a correctness defect.

| Item | Value |
|---|---|
| Trace B command | |
| Trace B digest | |
| Lines that differ between A and B | |
| Change that caused each line | |
| Commit of the repair | |

```bash
diff -u trace-a.expected.txt trace-b.txt
```

- `diff` output:
- Is every changed line attributed to the repair and not to the refactor?

## 9. Measured cost

| Metric | Before | After | Delta | Command |
|---|---:|---:|---:|---|
| Files | | | | |
| Lines | | | | |
| Public names | | | | |
| Tests | | | | |
| Build time (median of 5) | | | | |

Build time, the five runs:

| Run | Seconds |
|---:|---:|
| 1 | |
| 2 | |
| 3 | |
| 4 | |
| 5 | |
| Median | |

Which metric surprised me, and why:

>

Which metric did I predict worst, and by how much:

>

## 10. The rung's change test

| Question | Answer |
|---|---|
| The change the rung promises to make cheap | |
| The edit I made to test it | |
| Files I had to touch | |
| Did I touch the caller? | |
| Did the traces still pass after the test edit? | |
| Did I revert the test edit afterwards? | |

If the file count is the same as before the rung, the rung was not reached. What I would do next:

>

## 11. Evidence boundary

- Verified by reading the source:
- Verified by compiling:
- Verified by running the trace comparison:
- Verified by running the golden set:
- Verified by running a sanitizer:
- Verified by `git diff --numstat`:
- Inferred:
- Not measured:
- Unknown:

## 12. What the traces prove and do not prove

| Claim | Supported? | Why |
|---|---|---|
| For the recorded input, the output is byte-identical. | | |
| No observable behavior changed for the inputs I ran. | | |
| The two programs are equivalent for every input. | | |
| The refactor introduced no undefined behavior. | | |
| The refactor removed the defect. | | |
| The rung's capability was gained. | | |
| The design is better. | | |
| The archive is fixed. | | |

The strongest claim I can make from this workshop:

>

The weakest claim someone might read into it:

>

## 13. Archive integrity

- `git status` in the archive:
- Did I edit `original/`? (must be no):
- Did I apply any patch inside the archive? (must be no):
- Did I copy archive material into the tracked tree? (must be no):
- Where did I keep my scratch copy?
- Did I edit the lesson's `WORKSHOP-REPORT.md` structure instead of filling it?

## 14. Acceptance checklist

- [ ] I chose one failure and one rung, and justified both in one sentence each.
- [ ] My bench transcribes the defect and does not repair it.
- [ ] My bench controls its input instead of deleting the defect.
- [ ] Two runs of the bench with the same input produced the same bytes.
- [ ] I recorded trace A, its command and its SHA-256 digest before the first edit.
- [ ] I recorded the four coupling counts, and said which one is the representation count.
- [ ] I wrote the change list, including the untouched list.
- [ ] I wrote the predicted cost table before the refactor and did not edit it afterwards.
- [ ] I copied the rung's change test from the catalog into my plan.
- [ ] Every committed step built with `-Wall -Wextra -Werror`.
- [ ] Every committed step kept trace A byte-identical.
- [ ] I reverted failed steps individually, not the whole refactor.
- [ ] I ran the rung's change test and recorded the number of files it touched.
- [ ] I ran the determinism check on the refactored artifact.
- [ ] I ran a sanitizer and explained whether its report changed.
- [ ] If I took the correctness fork, trace B exists and every changed line is attributed.
- [ ] The cost table contains measured values, not adjectives.
- [ ] The verdict names what the rung bought, what it cost and what it did not fix.
- [ ] `WORKSHOP-REPORT.md` is complete.
- [ ] The archive is unmodified.

## 15. Next step

- Continue to `archaeology-19` — the review checklist and the final retrospective.
- If my prediction missed by a factor of two, the reason is:
- If the refactor took longer than planned, the plan I would write now is:
- The rung I would try next, and its price:
