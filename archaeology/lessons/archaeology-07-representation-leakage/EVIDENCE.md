# archaeology-07 — Evidence: representation leakage and parallel arrays

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
| 1 | Hand-computed board indices in `parallel_arrays.cpp` | | |
| 2 | Variables that must change together to move one knight | | |
| 3 | Lines changed by a 10×10 board, parallel version | | |
| 4 | Lines changed by a 10×10 board, record version | | |
| 5 | What `Knight` hides and what it does not hide | | |

One falsifiable sentence about the representation:

>

## 3. Inspection of the archive

Source file:

```text
original/variant-01-baseline-2008/source/guerra-de-caballos-baseline.cpp
```

Anchors I verified:

| Item | Line or count | Verified? |
|---|---:|---|
| `int vectab[64],vecjugadas[8];` | 17 | |
| `int cab1,cab2,jugada=1,orden,ganador=1;` | 18 | |
| `int cf1,cc1,cf2,cc2,cab;` | 29 | |
| `for (i=0;i<=64;i++) vectab[i]=0;` | 61–63 | |
| `cab1=rand()%64; cab2=rand()%64;` | 81–82 | |
| `conversionvm();` call | 90 | |
| `conversionvm` body | 104–112 | |
| `conversionmv` body | 226–232 | |
| `int movcab(int &cf,int &cc)` | 234 | |
| `vectab[ccab]=jugada;` | 269 | |
| `vectab[cab1]=jugada;` | 732 | |
| `cab1=movcab(cf1,cc1);` | 766 | |
| `cab2=movcab(cf2,cc2);` | 791 | |
| `cab2=movcab(cf2,cc2);` | 821 | |
| `cab1=movcab(cf1,cc1);` | 846 | |
| Lines containing `vectab[` | 80 | |
| Lines containing `conversionmv(` | 26 | |
| `conversionmv` calls excluding prototype and definition | 24 | |
| `movcab` call sites | 4 | |

The two representations I found:

| Representation | Names | Range | Produced by | Consumed by |
|---|---|---|---|---|
| Flat board index | | | | |
| Row and column | | | | |

The line that updates both representations at once:

```cpp

```

What I think can go wrong with that line:

>

Other places in the archive where the board storage is public:

- `ordenjuego`:
- `impresion`:
- `movcab`:
- Other:

## 4. Baseline run

Command:

```bash
cd archaeology/lessons/archaeology-07-representation-leakage
./run.sh
```

Observed output:

```text
parallel=
record=
match=
```

- Exit code:
- Compiler used:
- Flags observed in `run.sh`:
- Did the build pass with `-Werror`? If not, what diagnostic appeared?

Baseline structure I verified:

| Part | `parallel_arrays.cpp` lines | `record.cpp` lines | Present? |
|---|---:|---:|---|
| `Summary` | 4–11 | 35–42 | |
| `Knight` | absent | 4–8 | |
| `Board` | absent | 10–33 | |
| `mark` | absent | 13–16 | |
| `visited_count` | absent | 18–29 | |
| Private array | absent | 31–32 | |
| `run_scenario` | 13–38 | 44–58 | |
| `main` | 40–47 | 60–67 | |

What the baseline does not reproduce:

- The archive's flat index and row/column cache stored together:
- The archive's board payload:
- Random placement:
- Turn selection:
- Capture:
- Printing:
- Game loop:
- Other omissions:

## 5. Refactor

I froze the output before refactoring:

```bash
mkdir -p build/scratch
./build/parallel-arrays > build/scratch/golden.txt
cat build/scratch/golden.txt
```

- `golden.txt` content:
- Did I capture it before editing anything? (It must be yes.)

Scratch copy:

```bash
cp code/parallel_arrays.cpp build/scratch/record_attempt.cpp
```

The `Knight` record I introduced:

```cpp

```

The `Board` interface I introduced:

```cpp

```

What moved inside `Board::mark`:

- Index formula:
- Array:
- Count:

What stayed outside `Board`:

- Coordinate convention:
- Output shape:
- Other:

Compile command:

```bash
c++ -std=c++20 -Wall -Wextra -Werror -O2 \
    build/scratch/record_attempt.cpp -o build/scratch/record_attempt
```

- Build result:
- Diagnostic, if any:

## 6. Callers updated

The caller in this baseline is `run_scenario`. Fill one row per site.

| Site | `parallel_arrays.cpp` | `record.cpp` | Lines touched | Notes |
|---|---|---|---:|---|
| Knight declarations | | | | |
| Mark player start | | | | |
| Move player | | | | |
| Mark player after move | | | | |
| Move machine | | | | |
| Mark machine | | | | |
| Count visited | | | | |
| Build `Summary` | | | | |

Counts:

- Hand-computed indices removed:
- `Knight` values introduced:
- `Board` calls introduced:
- Net lines added or removed:
- Callers updated:

In the archive, the analogous places are `ordenjuego`, `impresion`, `movcab` and the four
`movcab` call sites in `impresionorden`. I verified:

- [ ] `ordenjuego` writes `vectab[cab1]` and `vectab[cab2]`.
- [ ] `impresion` reads `vectab[k]`, `vectab[cab1]` and `vectab[cab2]`.
- [ ] `movcab` mutates `cf`/`cc` and returns a flat index.
- [ ] `impresionorden` calls `movcab` at 766, 791, 821 and 846.

## 7. Comparison result

Observed output after the refactor:

```text
player=2,3 machine=3,4 visited=3
```

Diff against the frozen line:

```bash
diff <(./build/scratch/record_attempt) build/scratch/golden.txt
```

- Diff output:
- Exit code:

Diff against the shipped record version:

```bash
diff <(./build/scratch/record_attempt) <(./build/record)
```

- Diff output:
- Exit code:

- Did the summary stay the same?
- Did `match=yes` appear?
- If a difference appeared, which field differed and why?

Oracle direction:

- Which version produced `golden.txt`?
- Which version is the candidate?
- Why would generating the golden line from the record version be circular?

## 8. Explanation

The change test, in one paragraph. Name the change, the touched places in the parallel version and
the touched places in the record version. Do not claim a performance improvement.

>

The class, not the instance. Name at least two other places in the archive where the representation
is public.

- Instance in this lesson:
- Other instance 1:
- Other instance 2:

Two fixes, two sizes:

- Minimal fix (record only):
- Structural fix (record plus owner):
- Stronger fix (invariant and bounds check):
- Track A links:
- Why `construction-09` is not fully reached by the baseline:

What the refactor does not fix:

- Private array still flat:
- No bounds check:
- One-based convention:
- `Summary` still parallel:
- Archive dual storage:
- Globals and printing:
- End-to-end testability:

## 9. Evidence boundary

- Verified by reading the source:
- Verified by running the baseline:
- Verified by running the refactor:
- Inferred from the change test:
- Not measured:
- Unknown:

## 10. What the refactor proves and does not prove

| Claim | Supported? | Why |
|---|---|---|
| The record version produces the same summary for this scenario. | | |
| The record version is equivalent for every board and position. | | |
| The caller no longer computes the board index. | | |
| The archive's behavior is preserved. | | |
| `Board` hides its storage from callers. | | |
| `Board` eliminates the flat representation. | | |
| `Board::mark` validates coordinates. | | |
| The record version is shorter. | | |
| The record version is faster. | | |
| The change test is cheaper with the record. | | |

## 11. Severity and cost

One paragraph. Name the cost in real work, not the ugliness of the code. Say whether the cost is
medium or high in your context and why. Do not propose a rewrite in this paragraph.

>

## 12. Archive integrity

- `git status` in the archive:
- Did I edit `original/`? (It must be no.)
- Did I apply any patch inside the archive? (It must be no.)
- Where did I keep my scratch copy?
- Did I edit `code/parallel_arrays.cpp`? (It must be no.)
- Did I edit `code/record.cpp`? (It must be no.)

## 13. Acceptance checklist

- [ ] I wrote the predictions before inspecting the source.
- [ ] I verified the declarations at lines 17, 18 and 29.
- [ ] I explained `conversionvm` and `conversionmv`.
- [ ] I counted 80 `vectab[` mentions and 26 `conversionmv(` mentions, 24 of them calls.
- [ ] I named the four `movcab` call sites: 766, 791, 821, 846.
- [ ] The baseline printed `match=yes`.
- [ ] I froze `golden.txt` before refactoring.
- [ ] I introduced `Knight` in a scratch copy.
- [ ] I introduced `Board` with a private array and `mark`.
- [ ] I updated the caller and counted the sites.
- [ ] The refactor printed the same summary.
- [ ] I diffed the refactor against `code/record.cpp`.
- [ ] I stated the isolation boundary: record/board change only.
- [ ] I noted that `Board` still stores `std::array<bool, 64>`.
- [ ] I noted that `Board::mark` has no bounds check.
- [ ] I linked the fix to `construction-04`, `construction-05`, `construction-09` and
      `construction-11`.
- [ ] I did not claim a performance improvement.
- [ ] The archive is unmodified.

## 14. Next step

- Continue to `archaeology-08` — missing module boundary.
- If grouping the data was easier than hiding it, write one sentence about the difference between a
  record and an owner.
- If you did the 10×10 change test, keep the two line counts. They are the design argument.
- If you want the invariant lesson, go to `archaeology-09`. Keep the private array and the validated
  type separate in your record.
