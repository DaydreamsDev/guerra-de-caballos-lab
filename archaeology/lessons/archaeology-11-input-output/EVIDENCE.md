# archaeology-11 — Evidence: input, output and testability

> This is your record. Write it for your future self.
> Record what you observed, not what you expected the lesson to want.

## 1. Setup

- Date:
- Archive path:
- Tag checked out:
- Commit hash (`git rev-parse HEAD`):
- Compiler and version (`c++ --version`):
- Shell and version:
- Kernel and distribution (`uname -sr`):
- Working directory:
- Scratch directory used:
- Anything unexpected in the setup:

## 2. Prediction — write before inspecting the source

Do not edit this table after Step 2. The gap between prediction and observation is the learning
signal; repairing the prediction destroys it.

| # | Question | My prediction | How I would know I am wrong |
|---:|---|---|---|
| 1 | Sources linked by the console binary and by the test binary | | |
| 2 | Undefined stdio or console symbols in `core.o` | | |
| 3 | Console lines printed, and where the count comes from | | |
| 4 | What `test.cpp` asserts and what it omits | | |
| 5 | What breaks when one transcript line is added inside the core | | |

One falsifiable sentence about the boundary in this baseline:

>

## 3. Inspection of the baseline

Files:

```text
code/core.h
code/core.cpp
code/console.h
code/console.cpp
code/test.cpp
code/main.cpp
```

Parts I verified:

| Part | Lines | Verified? |
|---|---:|---|
| `core.h` | 13 | |
| `core.cpp` | 26 | |
| `console.h` | 5 | |
| `console.cpp` | 11 | |
| `test.cpp` | 20 | |
| `main.cpp` | 9 | |

Includes per file:

| File | Includes | Prints? |
|---|---|---|
| `core.h` | | |
| `core.cpp` | | |
| `console.h` | | |
| `console.cpp` | | |
| `test.cpp` | | |
| `main.cpp` | | |

The `GameResult` fields, classified:

| Field | Domain fact or presentation? | Why |
|---|---|---|
| `lines` | | |
| `winner` | | |
| `moveCount` | | |

What `test.cpp` asserts:

- line count:
- winner:
- move count:

What it does not assert:

- line text:
- line order:
- determinism:
- console agreement:
- input validity:

## 4. Inspection of the archive

Anchors I verified in `original/variant-01-baseline-2008/source/guerra-de-caballos-baseline.cpp`:

| Item | Line or count | Verified? |
|---|---:|---|
| `system("color 1f")` | 38 | |
| `getch()` in `main` | 42 | |
| `system("cls")` in `main` | 43 | |
| `while (jugada!=0)` | 45 | |
| `srand(time(NULL))` | 76 | |
| `movcab` | 234–724 | |
| Lines with `cout` inside `movcab` | 19 | |
| Lines with `cout` in the whole file | 62 | |
| Trapped-knight messages | 704, 712 | |
| `impresionorden` | 753–868 | |
| `getch()` in the turn loop | 781, 806, 835, 861 | |
| `system("cls")` in the turn loop | 784, 809, 838, 864 | |

The other variants:

| Variant | Lines | Lines with `cout` | `getch` | `system("cls")` |
|---|---:|---:|---:|---:|
| `variant-01-baseline-2008` | | | | |
| `variant-02-records-2008` | | | | |
| `variant-03-pointers-dev-2008` | | | | |
| `variant-04-pointers-final-2008` | | | | |

The input side in `variant-02`:

| Item | Line | Verified? |
|---|---:|---|
| `char nombre[250];` | 32 | |
| `cin>>nom.nombre;` | 46 | |
| `movcab(..., char &pisadas)` | 285 | |
| `ordenjuego(..., char &pisadas)` | 775 | |
| `impresionorden(char pisadas)` | 802 | |

What I expected the archive to contain, and what it actually contains:

>

## 5. Baseline run

Command:

```bash
cd archaeology/lessons/archaeology-11-input-output
./run.sh
```

Observed output:

```text
console_lines=
test_output=
test=ok
match=yes
```

- Exit code:
- Compiler used:
- Flags observed in `run.sh`:
- Did the build pass with `-Werror`? If not, what diagnostic appeared?

What the four lines mean:

- `console_lines`:
- `test_output`:
- `test=ok`:
- `match=yes`:

## 6. Boundary proof

Commands:

```bash
mkdir -p build
c++ -std=c++20 -Wall -Wextra -Werror -O2 -c code/core.cpp -o build/core.o
nm -C -u build/core.o
nm -C -u build/core.o | grep -E 'printf|cout|getch|fwrite|puts'
```

Undefined symbols observed:

```text

```

The filtered command produced:

- Output:
- Exit code:
- What that result proves:

Link the test without the console:

```bash
c++ -std=c++20 -Wall -Wextra -Werror -O2 code/core.cpp code/test.cpp -o build/core-test
./build/core-test
```

- Result:
- Did the link succeed without `console.cpp`? (It must.)
- What the run printed:

Dependency summary:

| Binary | Sources | Links `console.cpp`? |
|---|---|---|
| `console-app` | | |
| `core-test` | | |

## 7. Separation in the scratch copy

Commands:

```bash
mkdir -p build/scratch
cd build/scratch
cp ../../code/core.h ../../code/core.cpp ../../code/console.h \
   ../../code/console.cpp ../../code/test.cpp ../../code/main.cpp .
```

What moved from the core to the adapter:

| Before | After |
|---|---|
| `lines` | |
| `moveCount` | |
| transcript text | |
| transcript order | |

What stayed in the core:

- seed:
- player name:
- starting player:
- moves:
- winner:

Diff summary (or a short description if the diff is long):

```text

```

Byte comparison:

```bash
./build/console-app > /tmp/console-baseline.txt
cd build/scratch
./console-app > /tmp/console-separated.txt
diff /tmp/console-baseline.txt /tmp/console-separated.txt
```

- `diff` output:
- Did the transcript change? (It must not.)
- Did I edit `code/`? (It must be no.)

## 8. Test

What the shipped test asserts:

| Assertion | Catches | Does not catch |
|---|---|---|
| `lines.size() == 8` | | |
| `winner == "player"` | | |
| `moveCount == 3` | | |

The brittle assertion:

- Change I made:
- Observed output:
- Exit code:
- Why the game behavior did not change:
- Why the test failed anyway:

Determinism:

| Run | Seed | Move 1 | Move 2 | Move 3 | Winner | Moves |
|---|---:|---:|---:|---:|---|---:|
| 1 | 11 | | | | | |
| 2 | 11 | | | | | |
| 3 | 12 | | | | | |

- Are runs 1 and 2 identical?
- Did run 3 differ in positions?
- Did run 3 differ in winner or count? Why?

The harness repair:

- Old behavior of `match=yes`:
- New comparison:
- What happens when the console prints `moves=4`:
- Exit code in that case:

Observed output after the repair:

```text
console_lines=
test_output=
test=ok
match=yes
```

## 9. Agreement result

- Console `winner`:
- Test `winner`:
- Console `moves`:
- Test `moves`:
- Do the fields agree?
- Did the harness exit 0?
- If they disagreed, which field differed and why?

The sharp check (optional):

- Command:
- Console lines observed:
- Test lines observed:
- What the shipped harness would print:
- Why the two paths disagree:

## 10. Explanation

The 2008 cost, in one paragraph. Name the prints inside `movcab`, the keypresses, the screen clears
and the display character in domain signatures. Say what each one prevents.

>

Two fixes, two sizes:

- Minimal fix:
- Structural fix:
- Track A links:
- What `construction-20` adds to the question:
- What `common-01` adds to the question:

What the separation does not fix:

- Validation:
- Magic numbers:
- Winner:
- Input adapter:
- Archive:
- Tests:
- Exact transcript:

The change test:

| Change | Baseline | Separated |
|---|---|---|
| Add one transcript line | | |
| Change the winner wording | | |
| Add a silent adapter | | |
| Assert the first move's position | | |
| Run without a terminal | | |

Where each edit lands:

- Baseline:
- Separated:
- Which version fails a domain test on a presentation change:
- Which version keeps the test green:

## 11. Evidence boundary

- Verified by reading the source:
- Verified by compiling and linking:
- Verified by running the baseline:
- Verified by running the separated version:
- Verified by the harness comparison:
- Inferred:
- Not measured:
- Unknown:

## 12. What the separation proves and does not prove

| Claim | Supported? | Why |
|---|---|---|
| `core.o` has no console dependency. | | |
| The test binary runs without `console.cpp`. | | |
| The core is deterministic for a fixed seed. | | |
| The core owns no presentation. | | |
| The console and test agree in the shipped harness. | | |
| The repaired harness checks agreement. | | |
| The separated console output is unchanged. | | |
| The core validates its input. | | |
| The archive's `movcab` is testable now. | | |
| The bench is the full horse game. | | |
| The separation is free. | | |

## 13. Severity and cost

One paragraph. Name the cost in real work, not the ugliness of the code. Say whether the cost is
medium or high in your context and why. Do not propose a rewrite in this paragraph.

>

## 14. Archive integrity

- `git status` in the archive:
- Did I edit `original/`? (It must be no.)
- Did I apply any patch inside the archive? (It must be no.)
- Where did I keep my scratch copy?
- Did I edit `code/`? (It must be no.)

## 15. Acceptance checklist

- [ ] I wrote the five predictions before inspecting the source.
- [ ] I named the three sources of the console binary and the two of the test binary.
- [ ] I compiled `core.cpp` alone and inspected `nm -C -u`.
- [ ] I confirmed there is no `printf`, `cout` or `getch` in `core.o`.
- [ ] I ran `./run.sh` and saw the four expected lines.
- [ ] I found the unconditional `match=yes` in `run.sh`.
- [ ] I counted 62 lines with `cout` in `variant-01` and 19 inside `movcab`.
- [ ] I counted 5 `getch` and 5 `system("cls")` calls.
- [ ] I found the `variant-02` input anchors: `char[250]`, `cin`, `pisadas`.
- [ ] I copied the baseline into `build/scratch/` before editing.
- [ ] I moved the transcript format from the core to `console.cpp`.
- [ ] The separated console transcript is byte-identical under `diff`.
- [ ] I reproduced the brittle assertion: one extra line makes the old test fail.
- [ ] I checked determinism with two runs of the same seed and two different seeds.
- [ ] I repaired the harness so `match=yes` follows an actual comparison.
- [ ] I stated that the baseline still owns the transcript format.
- [ ] I stated that there is no input adapter.
- [ ] I stated that the bench is not the full horse game.
- [ ] I linked the structural fix to `construction-20`, `construction-08` and `common-01`.
- [ ] The archive is unmodified.

## 16. Next step

- Continue to `archaeology-12` — naming and comments.
- If the input side felt unfinished, add an `argv` adapter and count what changes.
- If the transcript matters byte-for-byte, freeze it as a golden file (`common-04`).
- If you disagree with the severity, write the context that changes it.
