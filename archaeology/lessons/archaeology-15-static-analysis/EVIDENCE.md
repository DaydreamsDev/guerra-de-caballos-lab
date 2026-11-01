# archaeology-15 — Evidence: static analysis and compiler warnings

> This is your record. Write it for your future self.
> Record what you observed, not what you expected the lesson to want.

## 1. Setup

- Compiler and version (`c++ --version | head -1`):
- C++ standard used:
- Standard library:
- Shell and version:
- Kernel (`uname -sr`):
- Working directory:
- Scratch directory:
- Exact flags used:
- Anything unexpected in the setup:

## 2. Prediction — write before compiling

Do not edit this table after Step 2. The gap between prediction and observation is the learning
signal; repairing the prediction destroys it.

| # | Question | My prediction | How I would know I am wrong |
|---:|---|---|---|
| 1 | How many diagnostics does `-Wall -Wextra` produce? | | |
| 2 | Which line and column does the caret point to? | | |
| 3 | Does the warning survive `-O0` and `-O2`? | | |
| 4 | Does `-Wall` alone produce it? Does `-Wextra` alone? | | |
| 5 | Does deleting the declaration change stdout or the exit code? | | |

One falsifiable sentence about the warning in this bench:

>

## 3. Compile

Harness output and exit code:

```text

```

The diagnostic, copied verbatim:

```text

```

Command used to reproduce it:

```bash

```

| Measurement | Result |
|---|---|
| Exit code without `-Werror` | |
| Exit code with `-Werror` | |
| Message severity without `-Werror` | |
| Message severity with `-Werror` | |
| Flag tag without `-Werror` | |
| Flag tag with `-Werror` | |

Warning counts:

| Flags | Warning count |
|---|---:|
| `-Wall` | |
| `-Wextra` | |
| `-Wall -Wextra` | |
| `-Wunused-variable` | |
| `-Wall -Wextra -Wno-unused-variable` | |

What the `-Wextra`-only run shows about the flag sets:

What `-Wno-unused-variable` proves, and why it is not the fix:

What `-fanalyzer` reported on this bench:

## 4. Read

Line counts:

| File | Lines | Verified? |
|---|---:|---|
| `code/warnings.cpp` | 9 | |
| `code/fixed.cpp` | 8 | |
| `run.sh` | 27 | |

What each line of `warnings.cpp` does:

| Line | Content | Read, written or neither? |
|---:|---|---|
| 5 | `int unusedValue = 0;` | |
| 6 | `int value = 10;` | |
| 7 | `std::printf("value=%d\n", value);` | |
| 8 | `return 0;` | |

Every occurrence of the name:

```text

```

Scope argument — why this search is complete for this name:

>

The source diff between the baseline and the reference fix:

```text

```

What `run.sh` does, by line range:

| Lines | What they do |
|---:|---|
| 2 | |
| 4–7 | |
| 9–11 | |
| 13 | |
| 15–16 | |
| 18–19 | |
| 22–25 | |
| 26–27 | |

What the warning count actually counts:

What `fixed=ok` proves about the strict build:

What `match=yes` actually compares:

## 5. Triage

| # | Diagnostic | Flag | Location | Claim true? | Class | Decision | Behavior risk |
|---:|---|---|---|---|---|---|---|
| 1 | | | | | | | |

Alternatives considered:

| Response | Why I accepted or rejected it |
|---|---|
| Delete the declaration | |
| `(void)unusedValue;` | |
| `[[maybe_unused]]` | |
| `-Wno-unused-variable` | |
| Print the value | |

Is this a real defect, dead code, a false positive or an intentional exception? Why:

>

What would change my classification:

>

## 6. Fix

Scratch path:

The diff of my fix against the baseline:

```text

```

Build command and result:

```text

```

Does my scratch file match `code/fixed.cpp`? (`diff` output):

```text

```

What I deliberately did not change:

## 7. Verify

| Check | Command | Result |
|---|---|---|
| stdout bytes | `cmp build/before.txt build/after.txt` | |
| exit status | `cmp build/before.exit build/after.exit` | |
| stdout size | `wc -c < build/before.txt` | |
| strict build | `-Wall -Wextra -Werror` | |
| pedantic build | `-Wall -Wextra -Werror -Wpedantic` | |
| `-O0` versus `-O2` | `cmp` of both outputs | |
| negative control | `cmp` of original and changed output | |

The 9-byte output, decoded:

```text

```

The negative control:

- What I changed:
- What the comparison reported:
- Exit code of `cmp`:

The harness after the fix:

```text

```

Why `warnings=1` is still correct:

>

## 8. What the run proves

| Claim | Supported? | Evidence |
|---|---|---|
| The baseline emits exactly one warning. | | |
| The warning's claim is true. | | |
| The finding is dead code, not a live defect. | | |
| The fix preserves behavior. | | |
| The fixed bench is clean under `-Werror`. | | |
| The byte comparison detects a behavior change. | | |
| The program is correct. | | |
| There are no other bugs. | | |
| `match=yes` proves byte equality. | | |
| Compiler warnings replace tests. | | |

My one-sentence conclusion, with the compiler, the flags and the scope:

>

## 9. Evidence boundary

What the clean build proves:

>

What the clean build does not prove:

>

What the harness does not compare:

>

What I did not test:

>

## 10. Checklist

- [ ] I wrote the five predictions before compiling.
- [ ] I ran `./run.sh` and recorded the three lines and the exit code.
- [ ] I reproduced the diagnostic by hand and copied it verbatim.
- [ ] I recorded the compiler and version.
- [ ] I recorded the exit codes with and without `-Werror`.
- [ ] I recorded the warning counts for each flag set.
- [ ] I read all three files and recorded the 9/8/27 line counts.
- [ ] I verified the claim with `grep` and stated the scope argument.
- [ ] I classified the finding as dead code.
- [ ] I filled the triage row and the alternatives table.
- [ ] I copied the bench to `build/scratch/` before editing.
- [ ] My diff removes one declaration and adds nothing.
- [ ] The strict and pedantic builds exit 0 with no output.
- [ ] My scratch file matches `code/fixed.cpp`.
- [ ] `cmp` reports identical stdout and identical exit status.
- [ ] I recorded the 9-byte output.
- [ ] The negative control makes the comparison fail.
- [ ] I stated the harness boundary: text equality, not byte equality.
- [ ] I stated what the clean build proves and what it does not.
- [ ] `code/` is unmodified.
