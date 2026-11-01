# archaeology-10 — Evidence: missing polymorphism

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
| 1 | Virtual functions declared by `Piece` | | |
| 2 | Lines in `delta_with_switch` and branches it contains | | |
| 3 | Cost of one virtual call and calls per run | | |
| 4 | Existing declarations a third piece must change, per path | | |
| 5 | Which switch the archive contains: move number or piece kind | | |

One falsifiable sentence about dispatch in this baseline:

>

## 3. Inspection of the interface and the baseline

Source file:

```text
code/polymorphism.cpp
```

Parts I verified:

| Part | Lines | Verified? |
|---|---:|---|
| `Position`, `Delta` | 5–17 | |
| `PieceKind` | 19–23 | |
| `knightDeltas`, `mirrorDeltas` | 25–33 | |
| `Piece` (three virtual declarations) | 35–41 | |
| `Knight`, `MirrorKnight` (two overrides each) | 43–71 | |
| `delta_with_switch` | 73–123 | |
| `move_with_switch` | 125–129 | |
| `main` (400 comparisons) | 131–163 | |

The virtual declarations I found:

- Destructor:
- `move`:
- `role_name`:

The overrides per implementation:

| Implementation | `move` | `role_name` |
|---|---|---|
| `Knight` | | |
| `MirrorKnight` | | |

Inner switches in `delta_with_switch` and their cases:

- Outer switch on:
- Inner switches:
- Cases per inner switch:
- `default` branches:

## 4. Inspection of the archive

Anchors I verified in `original/variant-01-baseline-2008/source/guerra-de-caballos-baseline.cpp`:

| Item | Line or count | Verified? |
|---|---:|---|
| `switch(vecjugadas[i])` | 239 | |
| `int cf1,cc1,cf2,cc2,cab;` | 29 | |
| `int cab1,cab2,jugada=1,...` | 18 | |
| `if (cab==1)` sites | 9 | |
| `impresionorden` | 753 | |
| `cab=1;` / `cab=2;` turn blocks | 765, 790, 820, 844 | |
| `movcab(int &cf, int &cc)` | 234 | |

What the switch dispatches on:

What the `cab` integer selects:

The other variants:

| Variant | `switch` line | `cab==1` sites |
|---|---:|---:|
| `variant-01-baseline-2008` | | |
| `variant-02-records-2008` | | |
| `variant-03-pointers-dev-2008` | | |
| `variant-04-pointers-final-2008` | | |

What I expected the archive to contain, and what it actually contains:

>

## 5. Baseline run

Command:

```bash
cd archaeology/lessons/archaeology-10-polymorphism
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

What the 400 cases are:

- Pieces:
- Moves:
- Positions:
- Product:

## 6. Oracle capture

The oracle must be frozen before the switch is deleted. Record the exact commands.

```bash
mkdir -p build/scratch
cd build/scratch
cp ../../code/polymorphism.cpp polymorphism.cpp
# replace main with the temporary emission loop
c++ -std=c++20 -Wall -Wextra -Werror -O2 polymorphism.cpp -o polymorphism-golden
./polymorphism-golden > golden.txt
wc -l golden.txt
```

- `wc -l golden.txt` output:
- First line of `golden.txt`:
- Last line of `golden.txt`:
- Format of each line (fields, in order):
- Did I confirm the oracle came from `move_with_switch`, not from the classes?

## 7. Replacement

What I removed:

- `delta_with_switch`:
- `move_with_switch`:
- `PieceKind`:
- Old `main` body:

What I kept:

- `Piece`:
- `Knight`:
- `MirrorKnight`:
- Delta tables:

What I added or rewrote:

- `main` reader:
- Includes or headers:
- Output line:
- Exit code:

Diff summary (or a short description if the diff is long):

```text

```

Did the scratch copy compile with the same flags?

- Result:
- Diagnostic, if any:

## 8. Change test

The change: add a third piece with its own movement rule.

| Measure | Switch path | Dispatch path |
|---|---:|---:|
| Lines added to the rule | | |
| Existing rule declarations edited | | |
| New types | | |
| Harness lines to register it | | |
| Forgot one move: compiles clean? | | |
| Can it answer "what are you called?" | | |
| Merge conflict with another piece's rule | | |

Where each edit landed:

- Switch path:
- Dispatch path:
- Which path forces a reviewer to re-read shared code:
- Which path can be tested alone:

## 9. Comparison result

Observed output after the replacement:

```text
comparisons=
mismatches=
```

- Exit code:
- Did the count stay at 400?
- Did the mismatch count stay at 0?
- If a mismatch appeared, which tuple failed and why?

Oracle direction:

- Which implementation produced `golden.txt`?
- Which implementation is the candidate?
- Why would generating both from the same table be circular?

## 10. Explanation

The cost of a virtual call, in one paragraph. Name the vptr, the vtable, the indirect call and what
happens to inlining. Say whether you measured it.

>

My measurement, or the sentence "not measured":

- Repetitions:
- Compiler and flags:
- CPU:
- Result:
- Is the difference above the noise?

The two switches, in one paragraph. Say which one the archive has, which one the baseline
reconstructs, and why the repairs differ.

>

The class, not the instance. Name at least two other places in the archive where the program decides
something by piece kind or by an integer kind.

- Instance in this lesson:
- Other instance 1:
- Other instance 2:

Two fixes, two sizes:

- Minimal fix:
- Structural fix:
- Track A links:
- What `construction-13` adds to the question:

What the replacement does not fix:

- Unchecked index:
- `default` behavior:
- Globals:
- Printing and turn loop:
- Tests:
- `cab==1` sites in the archive:

## 11. Evidence boundary

- Verified by reading the source:
- Verified by running the baseline:
- Verified by running the replacement:
- Verified by the change test:
- Inferred:
- Not measured:
- Unknown:

## 12. What the replacement proves and does not prove

| Claim | Supported? | Why |
|---|---|---|
| Dispatch reproduces the switch for 400 inputs. | | |
| Dispatch is equivalent for all inputs. | | |
| The archive's `movcab` behavior is preserved. | | |
| The replacement removes the kind switch. | | |
| The replacement removes the move-number switch. | | |
| A third piece needs zero loop edits in the bench. | | |
| A third piece needs zero edits in the archive. | | |
| Dispatch is faster. | | |
| Dispatch removes the unchecked index. | | |
| Dispatch removes the globals. | | |

## 13. Severity and cost

One paragraph. Name the cost in real work, not the ugliness of the code. Say whether the cost is
medium or high in your context and why. Do not propose a rewrite in this paragraph.

>

## 14. Archive integrity

- `git status` in the archive:
- Did I edit `original/`? (It must be no.)
- Did I apply any patch inside the archive? (It must be no.)
- Where did I keep my scratch copy?
- Did I edit `code/polymorphism.cpp`? (It must be no.)

## 15. Acceptance checklist

- [ ] I wrote the five predictions before inspecting the source.
- [ ] I named the three virtual declarations and the two overrides per class.
- [ ] I found the archive's switch at line 239 and confirmed it is on the move number.
- [ ] I counted the `cab==1` sites: 9 in `variant-01`, 10 in the others.
- [ ] The baseline printed `comparisons=400 mismatches=0`.
- [ ] I captured a 400-line golden trace from the switch before deleting it.
- [ ] I removed `delta_with_switch`, `move_with_switch` and `PieceKind`.
- [ ] The dispatch version printed `comparisons=400 mismatches=0`.
- [ ] I recorded the change test: 23 lines in the shared function, 20 in a new type, and the
      touched places.
- [ ] I stated the call cost with a measurement or with "not measured".
- [ ] I separated the switch on piece kind from the switch on move number.
- [ ] I stated that the baseline reconstructs the kind switch and is not verbatim 2008 code.
- [ ] I noted that the archive's two knights move identically.
- [ ] I noted that the inner `default` hides a missing move.
- [ ] I linked the structural fix to `construction-11`, `construction-12` and `construction-13`.
- [ ] The archive is unmodified.

## 16. Next step

- Continue to `archaeology-11` — input, output and testability.
- If the interface felt too small for a real piece, write one sentence about the limit: movement in
  the interface is enough for a delta, not for a game.
- If you did the measurement, keep the numbers. They are the only honest answer to "is it slow?".
- If you want the harder change test, add a piece whose movement depends on the board. Note where
  the table fails.
