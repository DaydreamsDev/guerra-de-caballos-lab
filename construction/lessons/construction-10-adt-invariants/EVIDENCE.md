# construction-10 — Evidence
> [English](EVIDENCE.md) | [Español](EVIDENCE-es.md)

> Learner-owned record. Write in English.

This record follows one invariant from the declaration to a probe: the rule that only cells inside
the active 8×8 area may be marked visited. Keep the predictions. The gap between your prediction and
the observation is the result, whether it is zero or not.

## 1. Run setup

- Date:
- Seed: `1`
- Player name: `Learner`
- Repository root:
- Compiler and version (`g++ --version` or `clang++ --version`):
- Baseline read on (date):
- Construction-09 record at hand? (yes / no)
- The number I carried from `construction-09` (public declarations / definitions / traces):
- Scratch directory I used:

## 2. The three checks (before the run)

### 2.1 Header and declarations

Fill this from the source, with line numbers, not from memory.

| Header | Lines | Declarations | Incomplete type | Definition in |
|---|---|---|---|---|
| | | | | |

Counts:

- Headers:
- Translation units:
- Public declarations:
- Incomplete types:
- Function definitions, total:
- Internal helpers (`game.cpp`):
- Internal helpers (`main.cpp`):
- Program entry points:

Compared with `construction-09`:

- Declarations added:
- Definitions added:
- Callers changed:
- The one new declaration, in full:

### 2.2 Line map of `board.cpp`

| Lines | Contents |
|---|---|
| | |

- Line of the `board_is_valid` definition:
- Line of the guard in `board_was_visited`:
- Line of the guard in `board_mark_visited`:
- Line of the `outsideActiveArea` predicate:
- Line of the null check inside the checker:

### 2.3 Roles

| Role | Function | What it decides |
|---|---|---|
| Enforcement | | |
| Enforcement | | |
| Observation | | |

- Call sites of `board_is_valid` in `code/`:
- Output of `grep -rn 'board_is_valid' code/`:
- What would stop compiling if the checker were deleted:
- Why an unused check is a seam and not a guarantee:

## 3. Prediction (written before measuring)

### 3.1 The guards

The array is `bool visited[16][16]`; the active area is rows 1..8 and columns 1..8.

| Call sequence | `board_was_visited` after | `board_is_valid` after | Confidence |
|---|---|---|---|
| `board_mark_visited(board, 8, 8)` | | | |
| `board_mark_visited(board, 0, 0)` | | | |
| `board_mark_visited(board, 9, 9)` | | | |
| `board_mark_visited(board, 4, 0)` | | | |
| `board_mark_visited(board, 8, 9)` | | | |

- The row I expect to get wrong, and why:

### 3.2 The checker

| Question | My prediction |
|---|---|
| `board_is_valid` on a fresh board | |
| `board_is_valid` after one legal mark | |
| `board_is_valid` after one guarded out-of-range mark | |
| Seeds that match, of 20 | |
| Does the game call the checker? | |
| `board_is_valid` after removing the guard and marking `(0, 0)` | |
| `board.o` text size: grows, shrinks or stays the same? | |

- The prediction I am least sure about, and why:

### 3.3 The demonstration

- The guard I need to remove for the invalid state to appear:
- Which predicate line classifies `(0, 0)` as outside:
- Why the scan reaches `(0, 0)` at all:
- My reason:

## 4. Run — baseline and probe

- Baseline build command:
- Baseline exit status:
- Diff against `../../../common/traces/seed-0001.txt`:
- Seeds that matched, of 20, on the unmodified baseline:
- Probe source path (scratch only):
- Probe build command:
- Probe exit status:

Paste the probe output:

```text
<probe output>
```

| Observation | Predicted | Observed | Gap |
|---|---|---|---|
| Fresh board valid | | | |
| `mark(8,8)` then `was_visited(8,8)` | | | |
| `mark(0,0)` then `was_visited(0,0)` | | | |
| `mark(9,9)` then `was_visited(9,9)` | | | |

- The line that surprised me:
- What I did next instead of adjusting the prediction:

## 5. Compare — what the probe proves

| Call | Guard decision | Array written? | `board_was_visited` | `board_is_valid` |
|---|---|---|---|---|
| none, fresh board | — | no | — | |
| `mark(8, 8)` | | | | |
| `mark(0, 0)` | | | | |
| `mark(9, 9)` | | | | |

- The array dimension and the loop condition, side by side:
- What index the last iteration reads:
- Which cells the scan covers, and how many:
- What `outsideActiveArea` computes for `(0, 0)`:
- What the guards compute for `(0, 0)`:
- Why the predicate and the guards cannot disagree:
- Where the null check runs, and what it returns:
- Why `(16, 16)` is never read by the scan:
- Why 20 of 20 traces cannot detect a missing checker call:
- The one number that separates "implemented" from "used":

## 6. The change test in a scratch copy

- Scratch copy path:
- Files edited:
- Lines edited:
- Tracked files edited: (yes / no)

### 6.1 Guard intact

- Build command:
- Exit status:

Paste the change probe output:

```text
<guarded probe output>
```

- Validity before the illegal call:
- Validity after `mark(0, 0)`:
- Was the array written? (yes / no)
- Why the state stays legal:

### 6.2 Guard removed

- What I deleted from `board_mark_visited`:
- Build command:
- Exit status:

Paste the change probe output:

```text
<unguarded probe output>
```

- Validity before the illegal call:
- Validity after `mark(0, 0)`:
- Which cell the write reached:
- Which predicate line now reports it:
- The state that is now constructible:
- The function that now observes it:

### 6.3 Guard restored and traces

- Guard restored: (yes / no)
- Full build command:
- Exit status:
- Seeds that matched, of 20, with the guard:
- Seeds that matched, of 20, without the guard:
- Did removing the guard change any transcript? (yes / no)
- Why the trace count is the same either way:
- What the traces confirm:
- What the traces do not confirm:

## 7. Lifecycle

| Question | Answer |
|---|---|
| Who can break the invariant? | |
| Which function can detect it? | |
| Which function repairs it? | |
| Does `board_reset` repair a broken board? | |
| What happens to the invariant at `board_destroy`? | |
| Who is responsible for calling the checker? | |

- Cells initialized by `board_create`:
- Cells cleared by `board_reset`:
- Cells a correct board never writes:
- What an unguarded `mark(9,9)` leaves behind after `board_reset`:
- Why `board_was_visited(9, 9)` cannot report the damage:
- Which function can:
- Which later lesson removes the possibility instead of checking it:
- Which later lesson owns the other end of the lifecycle:

## 8. Explain

One sentence, without the words *header*, *file*, *module*, *compile*, *pointer*, *struct*,
*class*, *opaque*, *encapsulation*, *better*, *worse*, *clean*, *cleaner*, *nice*, *elegant*,
*ugly* or *modular*:

>

One paragraph:

- What the invariant fixes:
- What enforces it:
- What makes the scan correct:
- What the scan cannot do:
- What the 20 traces prove:
- What the traces do not prove:
- What the change test proves:
- What the change test costs:
- What `board_reset` does not do:
- Which later lesson removes the possibility:
- Which later lesson owns lifetime:

The four things I kept apart:

## 9. Acceptance check

- I identified the checker, the two guards and the outside-active-area rule: (yes / no)
- I found that the checker has zero call sites: (yes / no)
- I predicted `true` for a fresh board and observed `true`: (yes / no)
- I explained the scan domain as `0..15` over a 16×16 array: (yes / no)
- I explained why `(0, 0)` is a valid index and an invalid cell: (yes / no)
- I showed that the predicate and both guards use the same four comparisons: (yes / no)
- With the guard intact, `(0, 0)` and `(9, 9)` are rejected: (yes / no)
- With the guard removed, `(0, 0)` makes the board invalid: (yes / no)
- I restored the guard and the invalid state is rejected again: (yes / no)
- All 20 traces pass with the guard and without it: (yes / no)
- I recorded that the traces cannot see the checker: (yes / no)
- I showed that `board_reset` does not repair a broken board: (yes / no)
- I did not edit any tracked code file: (yes / no)

If I edited a tracked code file, what and why:

## 10. Reflection and next step

- What I can now explain without notes:
- What I cannot explain yet:
- One thing I would read again:
- The number I will carry to `construction-11`:
- The check I trust most: the trace, the guards or the checker:
- The check that surprised me:
- What this baseline adds that `construction-09` could not offer:
- What it still leaves to its caller:
- What the probe proved that the traces could not:
- The change I would make next to test the boundary again:
- Next step:

## 11. If a baseline is missing or unfaithful

Fill this only if a file was absent, a build failed for an environmental reason, a diff was not
empty, or the probe contradicted the documented behavior in a way you could not explain.

- Missing file, build error, diff, or contradiction:
- Exact command output:
- What I did instead of inventing a count:
- When I will retry:

## 12. Self-check

- [ ] I inventoried every header and every definition with its file.
- [ ] I located the checker, both guards and the predicate by line.
- [ ] I compared the header with `construction-09` and found the one new declaration.
- [ ] I predicted the guard behavior and the checker result before measuring.
- [ ] I reproduced all 20 traces on the unmodified baseline.
- [ ] I built a probe and saw a fresh board report valid.
- [ ] I explained the scan domain and why `(0, 0)` is outside the active area.
- [ ] I showed that the predicate matches the guards.
- [ ] I removed the guard and saw `(0, 0)` make the board invalid.
- [ ] I restored the guard and saw the invalid state rejected.
- [ ] I reproduced 20 of 20 traces with the guard and without it.
- [ ] I showed that `board_reset` does not repair a broken board.
- [ ] I separated what the rule says, what the code enforces, what the checker observes and what a
      run can see.
- [ ] My one-sentence explanation avoids the six syntax words and the eight verdict words.
- [ ] I did not edit any tracked code file.

No score. If a box is unchecked, write the next smallest action in §10.
