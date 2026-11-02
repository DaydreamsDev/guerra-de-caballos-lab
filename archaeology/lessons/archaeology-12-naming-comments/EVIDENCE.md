# archaeology-12 — Evidence: naming and comments

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
| 1 | What does `calc` compute? | | |
| 2 | What do `f` and `c` mean, and in which order? | | |
| 3 | Does a rename change the output? | | |
| 4 | How many archive comments explain a reason? | | |

One falsifiable sentence about the rename:

>

## 3. Inspection of the bench

Files:

```text
code/before.cpp
code/after.cpp
```

| Item | Before | After |
|---|---:|---:|
| Lines in the file | | |
| Function name | | |
| Parameter names | | |
| Comments | | |
| Expression | | |
| Output | | |

The diff, pasted or summarized:

```text

```

What changed:

- Function name:
- Parameter names:
- Comment:
- Output:

What did not change:

- Constant:
- Operator:
- Call site:
- Format string:
- Tests:

## 4. Inspection of the archive

Source file:

```text
original/variant-01-baseline-2008/source/guerra-de-caballos-baseline.cpp
```

Naming anchors I verified:

| Item | Line or count | Verified? |
|---|---:|---|
| `simcab1=21,simcab2=156,sep=219` | 16 | |
| `vectab[64],vecjugadas[8]` | 17 | |
| `cab1,cab2,jugada=1,orden,ganador=1` | 18 | |
| `conversionmv(int cf,int cc)` | 226 | |
| Lines containing `vectab` | 80 | |
| Lines containing `jugada` | 76 | |
| Comment lines containing `//` | 119 | |

Comments I audited:

| Line | Text (short quote) | Kind | Verb | Why |
|---:|---|---|---|---|
| 1–8 | Program description | Description | Keep | The code does not say what the program is for. |
| 9 | Libraries needed | Narration | Delete | The `#include` lines say it. |
| 16 | Symbols for the board | Unnamed constants | Rewrite | Name the three symbols and what they draw. |
| 18 | Positions and current move | Incomplete | Rewrite | Omits `orden` and `ganador`. |
| 19 | Initializes the vector | Narration and typo | Delete | A better name replaces it. |
| 76 | Changes the seed | Narration | Delete | `srand(time(NULL))` says it. |
| 78 | Roll for knight 2 | Lie | Repair or delete | The variable is `tirocab1`. |
| 310 | Knight has not been there | Lie | Delete | The `if` tests whose turn. |
| 702 | Trapped-knight rule | Reason | Keep or shorten | The rule is real. |

The two comments I would repair first, and why:

>

## 5. Baseline run

Command:

```bash
cd archaeology/lessons/archaeology-12-naming-comments
./run.sh
```

Observed output:

```text
before=
after=
match=
```

- Exit code:
- Compiler used:
- Flags observed in `run.sh`:
- Did the build pass with `-Werror`? If not, what diagnostic appeared?

What the run compares:

- `before.cpp`:
- `after.cpp`:
- The single input:
- The expected index:

## 6. Rename record

The scratch copy:

```bash
mkdir -p build/scratch
cp code/before.cpp build/scratch/renamed.cpp
```

| Old name | New name | Reason | Scope |
|---|---|---|---|
| `calc` | | | |
| `f` | | | |
| `c` | | | |

Did I rename anything else? (It must be no for the minimal fix.)

- Extra rename:
- Why I stopped there:

The renamed file, pasted:

```cpp

```

Compile and run:

```bash
c++ -std=c++20 -Wall -Wextra -Werror -O2 build/scratch/renamed.cpp \
    -o build/scratch/renamed
./build/scratch/renamed
```

- Compile result:
- Output:
- Exit code:
- Warning or error, if any:

## 7. Comment record

| Location | Before | After | Verb | Reason |
|---|---|---|---|---|
| `before.cpp:3` | | | | |
| `v01:16` | | | | |
| `v01:18` | | | | |
| `v01:78` | | | | |
| `v01:310` | | | | |
| `v01:702` | | | | |

The comment I kept, and what the code cannot say without it:

>

The comment I deleted, and the name that replaced it:

>

The comment that was wrong, and how I proved it:

>

## 8. Comparison result

Baseline output:

```text
before=
after=
match=
```

Renamed output:

```text
index=
```

- Did all three values agree?
- Exit code:
- If a mismatch appeared, which expression was wrong and why?

Oracle direction:

- Which file produced the reference output?
- Which file is the candidate?
- Why would generating both from the new names be circular?

## 9. Explanation

What the old names hid, in one paragraph. Name the decision and the reader who paid for it.

>

What the comments did, in one paragraph. Separate reasons from narration and lies.

>

The severity and cost in my context:

- Severity per incident:
- Cost per year:
- When it becomes high:
- What I would fix first:

Two fixes, two sizes:

- Minimal fix:
- Structural fix:
- Track A links:
- What the structural fix adds that the rename does not:

What the rename does not fix:

- Magic number:
- Bounds check:
- `jugada`:
- `ganador`:
- Globals:
- Tests:
- Archive comments:

## 10. Evidence boundary

- Verified by reading the source:
- Verified by running the baseline:
- Verified by running the renamed copy:
- Inferred:
- Not measured:
- Unknown:

## 11. What the rename proves and does not prove

| Claim | Supported? | Why |
|---|---|---|
| The renamed copy prints the same output. | | |
| The rename is behavior-preserving for all inputs. | | |
| The new names remove the local ambiguity. | | |
| The new names are the best possible names. | | |
| The comment deletion is safe. | | |
| The archive's comments are all repaired. | | |
| The archive's globals were renamed. | | |
| The function is correct for a real board. | | |
| The code is now tested. | | |

## 12. Severity and cost

One paragraph. Name the cost in real work, not the ugliness of the code. Say whether the cost is
low, medium or high in your context and why. Do not propose a rewrite in this paragraph.

>

The class, not the instance. Name two other places in the archive where a name hides a decision.

- Instance in this lesson:
- Other instance 1:
- Other instance 2:

## 13. Archive integrity

- `git status` in the archive:
- Did I edit `original/`? (It must be no.)
- Did I apply any patch inside the archive? (It must be no.)
- Where did I keep my scratch copy?
- Did I edit `code/before.cpp` or `code/after.cpp`? (It must be no.)

## 14. Acceptance checklist

- [ ] I wrote the four predictions before inspecting the source.
- [ ] I diffed `before.cpp` and `after.cpp` and recorded the only changes.
- [ ] The baseline printed `before=index=19`, `after=index=19`, `match=yes`.
- [ ] I recorded the compiler and flags.
- [ ] The rename table has three rows with a reason and a scope each.
- [ ] The renamed copy compiles with `-Werror` and prints `index=19`.
- [ ] The comment table has a verb and a reason for every row I touched.
- [ ] I cited the wrong-knight comment at line 78 and the wrong-statement comment at 310.
- [ ] I stated what the rename does not fix.
- [ ] I priced the defect by reader time and incident cost.
- [ ] I linked the structural fix to `common-03` and `construction-03`.
- [ ] The archive is unmodified.

## 15. Next step

- Continue to `archaeology-13` — technical documentation for maintainers.
- If I want the harder rename, I take `vectab` across the archive in a scratch copy and count the
  references.
- One sentence I want my future self to read before renaming anything:

>
