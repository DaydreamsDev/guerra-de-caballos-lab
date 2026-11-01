# archaeology-06 — Evidence: duplication and the eight-case switch

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
| 1 | Lines per `case` body, including comment and braces | | |
| 2 | Lines inside a body that actually change between cases | | |
| 3 | Lines that must move together when one delta is corrected | | |
| 4 | What the table buys and what it costs | | |

One falsifiable sentence about the switch:

>

## 3. Inspection of the original switch

Source file:

```text
original/variant-01-baseline-2008/source/guerra-de-caballos-baseline.cpp
```

Anchors I verified:

| Item | Line or count | Verified? |
|---|---:|---|
| `switch(vecjugadas[i])` | 239 | |
| `case 1` | 241 | |
| Comment: other cases have the same structure as case 1 | 301 | |
| `case 2` | 302 | |
| `case 3` | 359 | |
| `case 4` | 415 | |
| `case 5` | 471 | |
| `case 6` | 527 | |
| `case 7` | 584 | |
| `case 8` | 640 | |
| `default` | 696 | |
| Total lines in the eight case bodies | 455 | |
| Case bodies | 8 | |
| Integers that actually vary | 16 | |

Measured case sizes (total lines / non-blank lines):

| Case | Total | Non-blank |
|---:|---:|---:|
| 1 | | |
| 2 | | |
| 3 | | |
| 4 | | |
| 5 | | |
| 6 | | |
| 7 | | |
| 8 | | |

The eight deltas I read from the source:

| Case | Row delta | Column delta |
|---:|---:|---:|
| 1 | | |
| 2 | | |
| 3 | | |
| 4 | | |
| 5 | | |
| 6 | | |
| 7 | | |
| 8 | | |

The repeated skeleton steps I identified (line numbers from case 1):

- Bounds check:
- First conversion:
- Turn branch:
- Visited check:
- Capture check:
- Apply the delta:
- Record the move:
- Print and exit the scan:

Normalization check (optional): did I strip comments, whitespace and digits and compare the bodies?

- Result:
- What differed:

## 4. Baseline run

Command:

```bash
cd archaeology/lessons/archaeology-06-duplication-switch
./run.sh
```

Observed output:

```text
comparisons=
mismatches=
```

- Exit code:
- Compiler used:
- Flags observed in `run.sh`:
- Did the build pass with `-Werror`? If not, what diagnostic appeared?

Baseline structure I verified:

| Part | Lines | Present? |
|---|---:|---|
| `Position` | 4–10 | |
| `Delta` | 12–16 | |
| `moveDeltas` (8 entries) | 18–27 | |
| `apply_switch` (8 cases) | 29–52 | |
| `apply_table` (1 lookup) | 54–58 | |
| `main` (200 comparisons) | 60–83 | |

What the baseline does not contain:

- Bounds checks:
- Visited checks:
- Capture logic:
- Turn selection:
- Printing:
- Termination:

## 5. Oracle capture

The oracle must be frozen before the switch is deleted. Record the exact commands.

```bash
mkdir -p build/scratch
cd build/scratch
cp ../../code/movement.cpp movement.cpp
# replace main with the temporary emission loop
c++ -std=c++20 -Wall -Wextra -Werror -O2 movement.cpp -o movement-golden
./movement-golden > golden.txt
wc -l golden.txt
```

- `wc -l golden.txt` output:
- First line of `golden.txt`:
- Last line of `golden.txt`:
- Format of each line (fields, in order):
- Did I confirm the oracle came from `apply_switch`, not from `apply_table`?

## 6. Extraction

What I removed:

- Function:
- Lines removed:
- Cases removed:

What I kept:

- Table:
- Lookup:
- Entries:

What I added or rewrote:

- `main` reader:
- Includes:
- Output line:
- Exit code:

Diff summary (or a short description if the diff is long):

```text

```

Did the scratch copy compile with the same flags?

- Result:
- Diagnostic, if any:

## 7. Extraction cost

| Measure | Value |
|---|---:|
| Lines removed with `apply_switch` | |
| `case` bodies removed | |
| Table entries kept | |
| Lines added for the golden reader | |
| Comparisons after extraction | |
| Mismatches after extraction | |
| Original case-body lines replaced by the table | |

Optional change test: append a ninth delta and change the loop bound.

| Path | Lines touched | Notes |
|---|---:|---|
| Table | | |
| Switch | | |

## 8. Comparison result

Observed output after extraction:

```text
comparisons=
mismatches=
```

- Exit code:
- Did the count stay at 200?
- Did the mismatch count stay at 0?
- If a mismatch appeared, which tuple failed and why?

Oracle direction:

- Which implementation produced `golden.txt`?
- Which implementation is the candidate?
- Why would generating both from the same table be circular?

## 9. Explanation

The change test, in one paragraph. Name the change, the touched places in the switch and the
touched places in the table. Do not claim a performance improvement.

>

The class, not the instance. Name at least two other places in the archive where one rule is stored
as multiple copies.

- Instance in this lesson:
- Other instance 1:
- Other instance 2:

Two fixes, two sizes:

- Minimal fix:
- Structural fix:
- Track A links:
- Why `construction-12` is not this fix:

What the extraction does not fix:

- Out-of-range guard:
- Turn-branch duplication:
- Globals and printing:
- End-to-end testability:

## 10. Evidence boundary

- Verified by reading the source:
- Verified by running the baseline:
- Verified by running the extracted version:
- Inferred from the change test:
- Not measured:
- Unknown:

## 11. What the extraction proves and does not prove

| Claim | Supported? | Why |
|---|---|---|
| The table reproduces the switch for 200 inputs. | | |
| The table is equivalent for all inputs. | | |
| The original `movcab` behavior is preserved. | | |
| The extraction removes 24 lines and 8 cases from the baseline. | | |
| The extraction removes dispatch. | | |
| The extraction removes the skeleton duplication. | | |
| The table preserves the `default` behavior. | | |
| The change test is cheaper with the table. | | |
| The table is faster at runtime. | | |

## 12. Severity and cost

One paragraph. Name the cost in real work, not the ugliness of the code. Say whether the cost is
medium or high in your context and why. Do not propose a rewrite in this paragraph.

>

## 13. Archive integrity

- `git status` in the archive:
- Did I edit `original/`? (It must be no.)
- Did I apply any patch inside the archive? (It must be no.)
- Where did I keep my scratch copy?
- Did I edit `code/movement.cpp`? (It must be no.)

## 14. Acceptance checklist

- [ ] I wrote the predictions before inspecting the source.
- [ ] I verified the eight case anchors and the comment at line 301.
- [ ] I counted 8 bodies, 455 lines and 16 varying integers.
- [ ] The baseline printed `comparisons=200 mismatches=0`.
- [ ] I captured a 200-line golden trace from `apply_switch` before deleting it.
- [ ] I removed `apply_switch` and kept `moveDeltas` and `apply_table`.
- [ ] The extracted version printed `comparisons=200 mismatches=0`.
- [ ] I recorded the cost: 24 lines, 8 cases, 8 entries, 200/0.
- [ ] I explained the oracle direction and the circularity risk.
- [ ] I stated the isolation boundary: delta calculation only.
- [ ] I noted the lost `default` guard.
- [ ] I linked the structural fix to `construction-03`, `construction-05` and `construction-19`.
- [ ] The archive is unmodified.

## 15. Next step

- Continue to `archaeology-07` — representation leakage and parallel arrays.
- If the switch was easy to remove but the skeleton was not, write one sentence about the boundary
  between the minimal fix and the structural fix.
- If you did the optional change test, keep the two line counts. They are the design argument.
- If you want the dispatch lesson, go to `archaeology-10`. Keep the two repairs separate.
