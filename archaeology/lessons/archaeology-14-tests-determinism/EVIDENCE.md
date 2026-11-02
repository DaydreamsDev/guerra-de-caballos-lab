# archaeology-14 — Evidence: tests and determinism

> This is your record. Write it for your future self.
> Record what you observed, not what you expected the lesson to want.

## 1. Setup

- Date:
- Archive path:
- Compiler and version (`c++ --version`):
- C runtime (`ldd --version` first line, or `getconf GNU_LIBC_VERSION`):
- `RAND_MAX` on this platform:
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
| 1 | Does the archive accept a seed argument? | | |
| 2 | Two runs in the same second: same board? | | |
| 3 | Two runs with `--seed 1`: identical output? | | |
| 4 | `--seed 2`: does the winner change? | | |
| 5 | Can a glibc `rand()` trace be replayed on MSVC? | | |

One falsifiable sentence about determinism in this bench:

>

## 3. Inspection of the baseline

Files:

```text
code/seed_control.cpp
code/tests/golden-seed-1.txt
run.sh
```

Line counts:

| File | Lines | Verified? |
|---|---:|---|
| `code/seed_control.cpp` | 44 | |
| `code/tests/golden-seed-1.txt` | 6 | |
| `run.sh` | 27 | |

The three seams:

| Seam | Lines | What it does | Verified? |
|---|---:|---|---|
| Argument parsing | 25–31 | | |
| `set_seed` | 13–16 | | |
| `below` | 18–22 | | |

The generator:

- Multiplier:
- Increment:
- State width:
- What `below(3)` returns:
- How the step becomes a position:

The default seed when no valid argument is given:

The behaviour I did not expect from the parser (`--seed abc`, `--seed 1 extra`):

## 4. Inspection of the archive

Anchors I verified in `original/variant-01-baseline-2008/source/guerra-de-caballos-baseline.cpp`:

| Item | Line or count | Verified? |
|---|---:|---|
| `void main()` | 32 | |
| `cabubicacion()` | 69–87 | |
| `while (i==1)` retry loop | 73 | |
| `srand(time(NULL))` | 76 | |
| `rand() % 51` for the starting order | 78–79 | |
| `rand() % 64` for the cells | 81–82 | |
| Distinctness check | 84 | |
| `opcion=(rand() % 8)+1` | 180 | |
| `srand` calls in the file | 1 | |
| `rand()` calls in the file | 5 | |
| `argc` or `argv` in any variant | 0 | |
| Test or harness files in the archive | 0 | |

The other variants:

| Variant | `srand` line | Fifth `rand()` line | Lines |
|---|---:|---:|---:|
| `variant-00-precursor-2007` | | | |
| `variant-01-baseline-2008` | 76 | 180 | 930 |
| `variant-02-records-2008` | 87 | 231 | 1021 |
| `variant-03-pointers-dev-2008` | 99 | 245 | 1058 |
| `variant-04-pointers-final-2008` | 105 | 257 | 1079 |

What the archive's `05-known-issues.md` says about tests and seeds:

>

Why the retry loop repeats the same draws within one second:

>

## 5. Baseline run

Command:

```bash
cd archaeology/lessons/archaeology-14-tests-determinism
./run.sh
```

Observed output:

```text
first=seed=1
move=1 position=2
move=2 position=5
move=3 position=0
winner=player
moves=3
same=yes
golden=yes
```

- Exit code:
- Compiler used:
- Flags observed in `run.sh`:
- Did the build pass with `-Werror`? If not, what diagnostic appeared?

What the four labels mean:

- `first=`:
- `same=`:
- `golden=`:
- exit code:

The arithmetic for seed 1:

| Move | State after the draw | `state % 3` | Step | Position |
|---:|---:|---:|---:|---:|
| 1 | 7806831264735756412 | 1 | 2 | 2 |
| 2 | 9396908728118811419 | 2 | 3 | 5 |
| 3 | 11960119808228829710 | 2 | 3 | 0 |

- Did my hand calculation match the printed positions?
- If not, where did it diverge?

## 6. Two runs with the same seed

Commands:

```bash
./build/seed-control --seed 1 > build/run-a.txt
./build/seed-control --seed 1 > build/run-b.txt
diff build/run-a.txt build/run-b.txt
```

- `diff` output:
- `diff` exit code:
- Are the transcripts identical?
- What `same=yes` asserts in `run.sh`:
- What `same=yes` does not assert:

Ambient inputs I checked:

| Input | Read by the program? | Evidence |
|---|---|---|
| Clock | | |
| Environment variables | | |
| Files | | |
| Locale | | |
| Shared generator state | | |

## 7. Golden comparison

Commands:

```bash
cat code/tests/golden-seed-1.txt
./build/seed-control --seed 1 | diff - code/tests/golden-seed-1.txt
./build/seed-control --seed 1 | cmp - code/tests/golden-seed-1.txt
```

- Golden file contents (copy exactly, including the final newline):

```text

```

- `diff` output:
- `diff` exit code:
- `cmp` output:
- `cmp` exit code:
- Did `run.sh` report `golden=yes`?
- Why `cmp` is stricter than `run.sh`:
- What the oracle is:
- What the test is:
- What happens to the test if the `exit 1` lines are deleted:

A deliberate mismatch I produced (optional):

- Change made:
- `golden=` result:
- Exit code:
- Did I edit the golden file? (It must be no.)

## 8. A different seed

Commands:

```bash
./build/seed-control --seed 2
./build/seed-control --seed 7
```

Observed:

| Seed | Move 1 | Move 2 | Move 3 | Winner | Moves |
|---:|---:|---:|---:|---|---:|
| 1 | 2 | 5 | 0 | player | 3 |
| 2 | 2 | 3 | 4 | player | 3 |
| 7 | 3 | 4 | 7 | player | 3 |

- Which fields changed?
- Which fields did not change?
- Why the winner and the move count cannot change in this bench:
- What a real game would do differently:
- Why seeds 1 and 2 share the first position:

## 9. Portability note

What `rand()` guarantees and what it does not:

- Guaranteed by the standard:
- Not guaranteed:
- `RAND_MAX` on this platform:
- `RAND_MAX` on MSVC:
- Why a `% 51` draw uses different bits on each platform:
- Why a new `rand()` call anywhere shifts later draws:

A platform note for a captured legacy trace:

| Field | Value for my capture |
|---|---|
| Compiler and version | |
| C runtime / libc | |
| Architecture and word size | |
| Seed source | |
| Exact seed value | |
| Command line | |
| Output bytes (line endings, encoding) | |
| Scope statement | |

- What the archive cannot provide for this note:
- Why capturing a trace requires patching a scratch copy:
- Whose trace is it then:

## 10. Explanation

The cost, in one paragraph. Name the retry loop, the clock granularity and the absent seed
argument. Say what each one prevents.

>

Two fixes, two sizes:

- Minimal fix:
- Structural fix:
- Track A links:
- What `common-02` adds to the question:
- What `common-05` adds to the question:

What seed control does not fix:

- Winner:
- Modulo bias:
- `void main()`:
- Global state:
- Input validation:
- Platform note:
- The rest of the game:

The change test:

| Change | Baseline | Seeded bench |
|---|---|---|
| Run twice, same seed | | |
| Run with another seed | | |
| Add a draw before the loop | | |
| Change a format string | | |
| Run on another libc | | |
| Report a bug with a board | | |

## 11. Evidence boundary

- Verified by reading the source:
- Verified by compiling:
- Verified by running the harness:
- Verified by `cmp`:
- Inferred:
- Not measured:
- Unknown:

## 12. What the harness proves and does not prove

| Claim | Supported? | Why |
|---|---|---|
| Seed 1 produces the golden transcript. | | |
| Two runs with seed 1 are identical. | | |
| A different seed changes the positions. | | |
| The program is portable. | | |
| The generator is unbiased. | | |
| The game logic is correct. | | |
| The archive is testable as published. | | |
| The harness catches every regression. | | |
| The golden file is a test by itself. | | |
| The trace is valid on another platform. | | |

## 13. Severity and cost

One paragraph. Name the cost in real work, not the ugliness of the code. Say whether the cost is
high or critical in your context and why. Do not propose a rewrite in this paragraph.

>

## 14. Archive integrity

- `git status` in the archive:
- Did I edit `original/`? (It must be no.)
- Did I apply any patch inside the archive? (It must be no.)
- Where did I keep my scratch copy?
- Did I edit `code/`? (It must be no.)

## 15. Acceptance checklist

- [ ] I wrote the five predictions before inspecting the source.
- [ ] I named the three seams and the LCG constants.
- [ ] I compiled with the shipped flags and recorded the compiler.
- [ ] I ran `--seed 1` and saw the six-line transcript.
- [ ] I recomputed at least one position from the state.
- [ ] I ran `./run.sh` and saw `same=yes` and `golden=yes`.
- [ ] I ran the program twice by hand and `diff` was empty.
- [ ] I compared with `cmp` and recorded its exit code.
- [ ] I ran seeds 2 and 7 and recorded the positions.
- [ ] I explained why the winner and the move count did not change.
- [ ] I recorded the archive anchors: one `srand`, five `rand()`, no `argv`, no test file.
- [ ] I explained why `srand(time(NULL))` inside the retry loop repeats draws within one second.
- [ ] I stated why `rand()` is implementation-defined and why `RAND_MAX` matters.
- [ ] I wrote a platform note for a captured legacy trace.
- [ ] I distinguished the oracle from the test.
- [ ] I named what seed control does not fix.
- [ ] I stated the evidence boundary: one seed, one bench, one environment.
- [ ] I linked the structural fix to `common-02` and `common-05`.
- [ ] The archive is unmodified.

## 16. Next step

- Continue to `archaeology-15` — static analysis and compiler warnings.
- If the retry loop surprised you, log the four draws per attempt with a fixed seed.
- If the portability question is still open, capture the archive's output on two compilers.
- If you disagree with the severity, write the context that changes it.
