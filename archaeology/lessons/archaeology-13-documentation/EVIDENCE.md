# archaeology-13 — Evidence: technical documentation for maintainers

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
- Scratch documentation directory (`$DOCBENCH/docs`):
- Scratch build directory (`$DOCBENCH`):
- Tools used (`timeout`, `wc`, `grep`, `cp` versions):
- Anything unexpected in the setup:

## 2. Scope and audience

- Source documented:
- Version or tag:
- Compiler and flags documented:
- Platform documented:
- Audience:
- In scope:
- Out of scope:
- One sentence a maintainer should be able to answer after reading:

>

## 3. Documentation inventory

| Document | Path | Question it answers | Question it leaves open |
|---|---|---|---|
| | | | |
| | | | |
| | | | |
| | | | |
| | | | |

- The gap I am filling:
- The gap I am not filling:
- The document I almost copied, and why I did not:

## 4. Source inspection

Commands and anchors:

```bash
wc -l "$ARCHIVE/original/variant-01-baseline-2008/source/guerra-de-caballos-baseline.cpp"
file "$ARCHIVE/original/variant-01-baseline-2008/source/guerra-de-caballos-baseline.cpp"
grep -c '//' "$ARCHIVE/original/variant-01-baseline-2008/source/guerra-de-caballos-baseline.cpp"
```

- Total lines:
- Encoding (`file` output, verbatim):
- Lines containing `//`:

Anchors I verified:

| Anchor | Line(s) | What it proves | Verified? |
|---|---:|---|---|
| Includes | 10–14 | The source assumes pre-standard headers. | |
| Glyphs | 16 | The board uses the integer values 21, 156 and 219. | |
| Board and move order | 17 | The board has 64 cells; the order has 8 entries. | |
| Mutable state | 18 | The state is global. | |
| Entry point | 32 | `void main()` is not standard C++. | |
| Console calls | 38, 43 | The program assumes a Windows console. | |
| Input | 42 | `getch()` is the only input. | |
| Game loop | 44 | `jugada` is the loop condition and the sentinel. | |
| Off-by-one | 61 | The initialization writes one past the board. | |
| Seeding | 76 | `srand(time(NULL))` is inside the retry loop. | |
| Start placement | 78–83 | Cells and rolls are random and redrawn on collision. | |
| Move order | 176–205 | The eight cases are evaluated in a random rotation. | |
| Index formula | 226–229 | A cell index is `row * 8 + column`. | |
| Capture | 254–257 | Capture sets `ganador=0`. | |
| Trap | 702–717 | Eight failed cases end the game. | |
| Turn order | 726–751 | The higher roll starts. | |
| Board output | 870–928 | The board is printed as 8 by 8 cells. | |

## 5. README record

| Template section | What I wrote | Source of the claim |
|---|---|---|
| What it is | | |
| Build | | |
| Run | | |
| Test | | |
| Behavior spec | | |
| Evidence | | |

- The build command I documented:
- The run command I documented:
- The test command I documented:
- The toolchain and flags I recorded:
- The expected files I recorded:
- The seed control I recorded:
- The coverage boundary I recorded:
- The weak sentence I replaced, and with what:

>

## 6. Behavior spec record

| Clause | Spec text | Source lines | Mark (verified / inferred / unknown) |
|---|---|---|---|
| Board | | | |
| Pieces | | | |
| Start cells | | | |
| Start order | | | |
| Turn order | | | |
| Move order | | | |
| Legal move | | | |
| Capture | | | |
| Trap | | | |
| Draw | | | |
| Randomness | | | |
| Output | | | |

- Rules, randomness and presentation separated? (yes / no)
- The clause I could not verify:
- The clause I inferred, and why:
- The unknown I stated in the spec:
- The clause I deleted because it described the implementation:

## 7. Command run log

| # | Command (short) | Directory | Exit | Observed | Expected | Match? |
|---:|---|---|---:|---|---|---|
| 1 | Prepare source and headers | | | | | |
| 2 | Compile object | | | | | |
| 3 | Link probe | | | | | |
| 4 | Run with timeout | | | | | |
| 5 | Count stdout and stderr | | | | | |
| 6 | Smoke test | | | | | |
| 7 | Naive build (blocker) | | | | | |

- The clean shell I used:
- The command that failed the first time:
- The README sentence I added or fixed:
- Output volume across runs (three runs):
- The exact first stderr line, verbatim:
- Did any build or run command depend on a file outside `$DOCBENCH` after preparation?
  (It must be no.)

## 8. Claim audit

| Claim in the documentation | Source | Type (line / command / document) | Verdict |
|---|---|---|---|
| | | | |
| | | | |
| | | | |
| | | | |
| | | | |
| | | | |

- Claims I deleted because they had no source:
- Claims I downgraded to unknown:
- Claims that were false even though a command passed:

## 9. Unknowns

| Unknown | Why it matters | What would resolve it |
|---|---|---|
| | | |
| | | |
| | | |
| | | |

- The unknown that blocks a maintainer today:
- The unknown that does not block, but should be recorded:
- The unknown I am tempted to hide, and why I will not:

## 10. Scope check

- [ ] Every sentence helps a maintainer build, run, test or change the artifact.
- [ ] I removed sentences that describe the artifact's context rather than its use.
- [ ] The README has no history section.
- [ ] The spec has no function-by-function tour.
- [ ] The evidence section names the source, the commands and the unknowns.

The sentence I removed, and the section it was in:

>

## 11. Archive integrity

- `git status` in the archive:
- Did I edit `original/`? (It must be no.)
- Did I apply a patch inside the archive? (It must be no.)
- Where did the source copy live?
- Did I edit the lesson templates? (It must be no; I copied them.)

## 12. Acceptance checklist

- [ ] I inventoried at least four existing documents and named their gaps.
- [ ] I wrote the scope sentence before drafting the README.
- [ ] The README fills every section of `README-TEMPLATE.md`.
- [ ] The build section names the toolchain, flags, dependencies and expected files.
- [ ] The run section names the input, output, seed control and expected status.
- [ ] The test section names the command and what it does not cover.
- [ ] The spec fills every section of `SPEC-TEMPLATE.md`.
- [ ] Every spec clause cites source lines and carries a confidence mark.
- [ ] Rules, randomness and presentation are separate.
- [ ] I ran every README command from a clean directory.
- [ ] I recorded exit status and observed output for every command block.
- [ ] I audited every factual claim against a line, a command or a document.
- [ ] I stated the unknowns with a resolution path.
- [ ] The scope check passes.
- [ ] The archive is unmodified.

## 13. Next step

- Continue to `archaeology-14` — tests and determinism.
- If a command needed a fix, keep the failing text next to the working text. The pair is the
  evidence that the README was executed.
- If the spec has an inferred clause, name the observation that would promote it to verified.
- One sentence I want my future self to read before writing another README:

>
