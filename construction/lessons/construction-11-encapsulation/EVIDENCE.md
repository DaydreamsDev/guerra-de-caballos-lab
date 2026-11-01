# construction-11 — Evidence
> [English](EVIDENCE.md) | [Español](EVIDENCE-es.md)

> Learner-owned record. Write in English.

This record follows the board invariant across the class boundary: the rule that only cells inside
the active 8×8 area may be marked visited. Keep the predictions. The gap between your prediction and
the observation is the result, whether it is zero or not.

## 1. Run setup

- Date:
- Seed: `1`
- Player name: `Learner`
- Repository root:
- Compiler and version (`g++ --version` or `clang++ --version`):
- Baseline read on (date):
- Construction-10 record at hand? (yes / no)
- The number I carried from `construction-10` (public declarations / definitions / traces):
- Scratch directory I used:

## 2. The class boundary (before the run)

### 2.1 Header and declarations

Fill this from the source, with line numbers, not from memory.

| Header | Lines | Public declarations | Private data declarations | Complete class | Incomplete type |
|---|---|---|---|---|---|
| | | | | | |

Counts:

- Headers:
- Translation units:
- Public declarations:
- Private data declarations:
- Complete class definitions:
- Incomplete types:
- Function definitions, total:
- Internal helpers (`game.cpp`):
- Internal helpers (`main.cpp`):
- Program entry points:

Compared with `construction-10`:

- Public declarations: ______ → ______
- Definitions: ______ → ______
- Incomplete types: ______ → ______
- Complete class definitions: ______ → ______
- Callers changed:
- Board operations that disappeared:
- Board operations that appeared:
- The one access specifier that carries the boundary:

### 2.2 Line map

`board.h`:

| Lines | Contents |
|---|---|
| | |

`board.cpp`:

| Lines | Contents |
|---|---|
| | |

- Line of `public:`:
- Line of `private:`:
- Line of the array declaration:
- Line of the constructor:
- Line of `is_valid`:
- Line of the guard in `was_visited`:
- Line of the guard in `mark_visited`:

### 2.3 Roles and access

| Role | Operation | Access | What it decides |
|---|---|---|---|
| | | | |

- Public data members:
- Public operations that write:
- Public operations that only read:
- Which operations are `const`:
- What `const` prevents:
- Call sites of `is_valid()` in `code/`:
- Output of `grep -rn 'is_valid' code/`:
- What would stop compiling if `is_valid()` were deleted:
- Why an unused checker is still useful after the class:

## 3. Prediction (written before measuring)

### 3.1 The public operations

The array is `bool visited[16][16]`; the active area is rows 1..8 and columns 1..8.

| Call sequence | `is_valid()` after | `was_visited` after | Confidence |
|---|---|---|---|
| `Board board;` | | — | |
| `board.reset()` | | — | |
| `board.mark_visited(1, 1)` | | | |
| `board.mark_visited(8, 8)` | | | |
| `board.mark_visited(0, 0)` | | | |
| `board.mark_visited(9, 9)` | | | |
| `board.mark_visited(4, 0)` | | | |
| `board.mark_visited(8, 9)` | | | |

- The row I expect to get wrong, and why:

### 3.2 The change test

| Question | My prediction |
|---|---|
| What happens when `private:` is deleted and `board.visited[0][0] = true` compiles? | |
| `is_valid()` after that write | |
| What happens when `private:` is restored and the same probe is compiled? | |
| Seeds that match, of 20, with the class unchanged | |
| Seeds that match, of 20, with `visited` public | |
| Does the game call `is_valid()`? | |
| `board.o` text size: grows, shrinks or stays the same? | |

- The prediction I am least sure about, and why:

### 3.3 The compiler

- The line the compiler points to when `visited` is private:
- The exact error text I expect:
- Why the compiler can reject the write even though `board.h` is included:
- Why the compiler cannot reject a bad index inside `mark_visited`:
- My reason:

## 4. Run — baseline and boundary probe

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
| `reset()` leaves it valid | | | |
| `mark(1,1)` then `was_visited(1,1)` | | | |
| `mark(8,8)` then `was_visited(8,8)` | | | |
| `mark(0,0)` then `was_visited(0,0)` | | | |
| `mark(9,9)` then `was_visited(9,9)` | | | |
| `mark(4,0)` then `was_visited(4,0)` | | | |
| `mark(8,9)` then `was_visited(8,9)` | | | |

- The line that surprised me:
- What I did next instead of adjusting the prediction:

## 5. Compare — what the boundary proves

| Call | Guard decision | Array written? | `was_visited` | `is_valid` |
|---|---|---|---|---|
| none, constructed | — | no | — | |
| `reset()` | — | clears 1..8 | — | |
| `mark(1, 1)` | | | | |
| `mark(8, 8)` | | | | |
| `mark(0, 0)` | | | | |
| `mark(9, 9)` | | | | |
| `mark(4, 0)` | | | | |
| `mark(8, 9)` | | | | |

- The public interface has a data member: (yes / no)
- The only public writers:
- What `reset()` clears:
- Which cells no public operation can mark as visited:
- Why those cells stay `false`:
- What `is_valid()` scans:
- Why the public operations cannot construct an invalid state:
- What the probe does not prove:
- The one number that separates "the class is used" from "the checker is called":

## 6. The change test in a scratch copy

- Scratch copy path:
- Files edited:
- Lines edited:
- Tracked files edited: (yes / no)

### 6.1 State public

- What I deleted from `board.h`:
- Build command:
- Exit status:

Paste the change probe output:

```text
<public probe output>
```

- Validity before the write:
- Validity after `visited[0][0] = true`:
- Which cell the write reached:
- Which predicate line reports it:
- The state that is now constructible:
- The operation that observes it:

### 6.2 State private again

- What I restored:
- Build command:
- Exit status:

Paste the compiler output:

```text
<compiler output>
```

- The line the compiler rejects:
- The error text:
- Why the same line compiled in 6.1:
- What this proves about the access specifier:

### 6.3 Traces

- Seeds that matched, of 20, with `visited` public:
- Seeds that matched, of 20, with `visited` private:
- Did the visibility change alter any transcript? (yes / no)
- Why the trace count is the same either way:
- What the traces confirm:
- What the traces do not confirm:

## 7. The invariant and its writers

| Question | Answer |
|---|---|
| Who can write `visited`? | |
| Who can read `visited`? | |
| Which operation observes the invariant? | |
| Which operations enforce it? | |
| Does the class make the member functions correct? | |
| What can still break the invariant? | |
| Is `private` a security boundary? | |
| Who is responsible for calling `is_valid()`? | |

- What the class removes from the caller:
- What the class moves inside the type:
- What the class does not remove:
- Which later lesson builds on the class:
- Which later lesson owns lifetime:

## 8. Explain

One sentence, without the words *class*, *private*, *public*, *access*, *compiler*, *keyword*,
*header*, *file*, *module*, *encapsulation*, *better*, *worse*, *clean*, *cleaner*, *nice*,
*elegant*, *ugly* or *modular*:

>

One paragraph:

- What the class fixes:
- What enforces it:
- What makes the boundary different from `construction-10`:
- What the public operations do:
- What the 20 traces prove:
- What the traces do not prove:
- What the change test proves:
- What the change test costs:
- What the class does not fix:
- Which later lesson builds on the class:

The four things I kept apart:

## 9. Acceptance check

- I identified the private state, the public operations and the access specifier: (yes / no)
- I found that there is no public data member: (yes / no)
- I predicted `valid=1` after construction and observed `valid=1`: (yes / no)
- I showed that every public operation preserves the invariant: (yes / no)
- I found that `is_valid()` still has zero call sites: (yes / no)
- I deleted `private:` in a scratch copy and the write compiled: (yes / no)
- I observed `is_valid()` become false after `visited[0][0] = true`: (yes / no)
- I restored `private:` and the same write no longer compiled: (yes / no)
- All 20 traces pass with `visited` public and with `visited` private: (yes / no)
- I recorded that the traces cannot see the boundary: (yes / no)
- I explained what access control prevents and what it does not: (yes / no)
- I did not edit any tracked code file: (yes / no)

If I edited a tracked code file, what and why:

## 10. Reflection and next step

- What I can now explain without notes:
- What I cannot explain yet:
- One thing I would read again:
- The number I will carry to `construction-12`:
- The boundary I trust most: the access specifier, the guards or the checker:
- The boundary that surprised me:
- What this baseline adds that `construction-10` could not offer:
- What it still leaves to the class implementation:
- What the change test proved that the traces could not:
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
- [ ] I located the private state, the public interface and the access specifier by line.
- [ ] I compared the class with `construction-10` and named what each one hides.
- [ ] I predicted the public-operation behavior and the change test before measuring.
- [ ] I reproduced all 20 traces on the unmodified baseline.
- [ ] I built a boundary probe and saw a fresh board report valid.
- [ ] I showed that every public operation preserves the invariant.
- [ ] I deleted `private:` and saw the invalid state become constructible.
- [ ] I restored `private:` and saw the same write rejected at compile time.
- [ ] I reproduced 20 of 20 traces with `visited` public and with it private.
- [ ] I explained the difference between access control and array bounds.
- [ ] I separated what the rule says, who may write, what the compiler checks and what a run can
      see.
- [ ] My one-sentence explanation avoids the syntax words and the verdict words.
- [ ] I did not edit any tracked code file.

No score. If a box is unchecked, write the next smallest action in §10.
