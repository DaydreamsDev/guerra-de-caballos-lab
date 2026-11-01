# construction-12 — Evidence

> [English](EVIDENCE.md) | [Español](EVIDENCE-es.md)

> Learner-owned record. Write in English.

This record follows the piece through the interface: the four questions the game loop may ask, the
class that answers them, and the concrete type that is still named once at the root. Keep the
predictions. The gap between your prediction and the observation is the result, whether it is zero
or not.

## 1. Run setup

- Date:
- Seed: `1`
- Player name: `Learner`
- Repository root:
- Compiler and version (`g++ --version` or `clang++ --version`):
- Baseline read on (date):
- Construction-11 record at hand? (yes / no)
- The number I carried from `construction-11` (public declarations / definitions / traces):
- Scratch directory I used:

## 2. The interface (before the run)

### 2.1 Headers and declarations

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

Compared with `construction-11`:

- Public declarations: ______ → ______
- Definitions: ______ → ______
- Complete class definitions: ______ → ______
- Headers: ______ → ______
- Translation units: ______ → ______
- Private data declarations: ______ → ______
- Board declarations that changed:
- New interface operations:
- New implementations of those operations:
- The concrete type that is still named at the root:

### 2.2 Line map

`piece.h`:

| Lines | Contents |
|---|---|
| | |

`knight_piece.h`:

| Lines | Contents |
|---|---|
| | |

`knight_piece.cpp`:

| Lines | Contents |
|---|---|
| | |

- Line of the virtual destructor:
- Line of `row`:
- Line of `column`:
- Line of `set_position`:
- Line of `role_name`:
- Line of the `public:` label:
- Line of the `private:` label:
- Lines that construct a `KnightPiece` in `run_game`:
- Line that includes `knight_piece.h` in `game.cpp`:

### 2.3 Roles and dispatch

| Role | Operation | Implemented by | What the loop asks |
|---|---|---|---|
| | | | |

- Number of pure virtual operations in `Piece`:
- Operations that are `const`:
- What a `const Piece *` may call:
- The only operation that writes:
- Public data members in `KnightPiece`:
- Number of virtual call sites in `game.cpp`:
- The four operations those call sites ask for:
- Type tests in the loop (`switch`, tag, `dynamic_cast`):
- What `= 0` prevents:
- What would change if `row()` lost `= 0`:

## 3. Prediction (written before measuring)

### 3.1 The dispatch table

Every expression has static type `Piece *` or `const Piece *` and points to a `KnightPiece`.

| Expression | Which function runs | Predicted output | Confidence |
|---|---|---|---|
| `playerPiece.set_position(3, 4)` through `KnightPiece` | | | |
| `piece->row()` through `const Piece &` | | | |
| `piece->column()` through `const Piece &` | | | |
| `piece->role_name()` through `const Piece &` | | | |
| `otherPiece->row()` through `const Piece *` | | | |
| `fromPiece->role_name()` in `printMove` | | | |

- The row I expect to get wrong, and why:

### 3.2 The change test

| Question | My prediction |
|---|---|
| Files I will add for a second `Piece` type | |
| Lines I will change in `game.cpp` | |
| Edits inside `playTurn`, `tryMove`, `printTurnHeader`, `printMove`, `playGame` | |
| Seeds that match, of 20, with the substitute at the root | |
| Seeds that match, of 20, after restoring `KnightPiece` | |
| New concepts the change requires | |

- The prediction I am least sure about, and why:

### 3.3 The binding probe

| Question | My prediction |
|---|---|
| `piece.row()` through `KnightPiece` after `set_position(3, 4)` | |
| `piece.row()` through `const Piece &` with `row` virtual | |
| The same call with `row` non-virtual and a base body returning 0 | |
| `sizeof(Piece)` | |
| `sizeof(KnightPiece)` | |
| `game.o` text size: grows, shrinks or stays the same? | |

- What I expect the `virtual` keyword to change, in one sentence:

## 4. Run — baseline and dispatch probe

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
| `role` through `KnightPiece` | | | |
| `role` through `const Piece &` | | | |
| `row` through `const Piece &` | | | |
| `sizeof(Piece)` | | | |
| `sizeof(KnightPiece)` | | | |
| `game.o` text | | | |

- The line that surprised me:
- What I did next instead of adjusting the prediction:

## 5. Compare — what the dispatch proves

- Output of `grep -n 'KnightPiece' code/game.cpp`:
- Number of lines that mention the concrete type:
- Which of them are inside the loop:

| Lines | Function | Virtual calls |
|---|---|---|
| | | |

- Total virtual call sites:
- `nm -C knight_piece.o` lines that mention `vtable` or `typeinfo`:
- `sizeof(Piece)`:
- `sizeof(KnightPiece)`:
- What the object stores:
- What the class stores:
- What the probe proves about the call:
- What the probe does not prove:

### 5.1 The rename probe

- Files changed in the scratch copy:
- Old role names:
- New role names:
- Lines that differ, of 89:
- Which kinds of line differ (`who=`, `move=`, `trapped=`, `winner=`):
- Which lines are identical:
- What this proves about `role_name()`:

### 5.2 Devirtualization

- Output of `objdump` or `nm` that shows a direct call:
- The function where it appears:
- Why the compiler could prove the type there:
- Why the loop still compiles against the interface:
- What this does not change about the design:

## 6. The change test in a scratch copy

- Scratch copy path:
- Files added:
- Lines added:
- Files edited:
- Lines edited:
- Tracked files edited: (yes / no)

### 6.1 Add the subclass

- Class name:
- Header path:
- Implementation path:
- Lines in the header:
- Lines in the implementation:
- The four operations implemented:
- What the subclass deliberately keeps the same:

### 6.2 Exercise it through the interface

- Probe source path:
- Probe build command:
- Probe exit status:

Paste the probe output:

```text
<probe output>
```

- The parameter type of the function that used it:
- What the function knew about the concrete class:
- What this proves:

### 6.3 Substitute at the root

Paste the diff against the tracked `code/game.cpp`:

```text
<diff>
```

- Lines added:
- Lines removed:
- Lines inside the loop:
- Build command:
- Seeds that matched, of 20:

### 6.4 Restore the original

- What I restored:
- Build command:
- Seeds that matched, of 20, with the original `KnightPiece`:
- Did adding a file disturb the baseline? (yes / no)

### 6.5 The gap: movement

- Where the movement tables are declared:
- Where they are indexed:
- The virtual operation the loop does not call:
- What a different movement rule requires:
- Lines I would add to the interface:
- Lines I would change in `tryMove`:
- Does a golden trace cover the new movement rule? (yes / no)
- What the knight game proves after the extension:
- What the extension does not prove:
- The precise acceptance sentence of this lesson:

## 7. The interface and its limits

| Question | Answer |
|---|---|
| What does the loop hold? | |
| What does it ask for a position? | |
| What does it ask for a name? | |
| What does it ask to move a piece? | |
| Where is the concrete type still named? | |
| What does the interface not cover? | |
| What does a new same-behavior type cost? | |
| What does a new movement rule cost? | |
| What does `virtual` cost per object? | |
| What does `virtual` cost per class? | |
| What does a virtual call cost at run time? | |
| Can the compiler remove the indirection? | |

- What the interface removes from the loop:
- What it moves to the root:
- What it does not remove:
- Which later lesson builds on the interface:
- Which later lesson owns lifetime:

## 8. Explain

One sentence, without the words *virtual*, *polymorphism*, *interface*, *class*, *pointer*,
*compiler*, *dispatch*, *late binding*, *better*, *worse*, *clean*, *flexible*, *extensible*,
*elegant* or *ugly*:

>

One paragraph:

- What the interface fixes:
- What enforces it:
- What makes it different from `construction-11`:
- What the 19 call sites do:
- What the 20 traces prove:
- What the traces do not prove:
- What the change test proves:
- What the change test costs:
- What the interface does not cover:
- What the concrete type still costs:
- What late binding costs at run time:
- Which later lesson builds on the interface:

The four things I kept apart:

## 9. Acceptance check

- I identified the five virtual declarations and the four overrides: (yes / no)
- I counted the 19 virtual call sites and mapped them to functions: (yes / no)
- I found that `KnightPiece` is named only in the two constructions, and its header is included
  once: (yes / no)
- I predicted the dispatch table and observed the derived functions run: (yes / no)
- I reproduced all 20 traces on the unmodified baseline: (yes / no)
- I built the binding probe and saw the derived function through `const Piece &`: (yes / no)
- I measured `sizeof(Piece)` and `sizeof(KnightPiece)`: (yes / no)
- I added a second `Piece` implementation in a scratch file: (yes / no)
- I exercised it through `Piece &`: (yes / no)
- I substituted it at the root and got 20 of 20 traces: (yes / no)
- I restored `KnightPiece` and got 20 of 20 traces: (yes / no)
- I confirmed zero edits in the loop: (yes / no)
- I removed `virtual` in a scratch copy and explained the result: (yes / no)
- I explained the `override` error and the abstract-type error: (yes / no)
- I named the movement geometry as the limit of the interface: (yes / no)
- I did not edit any tracked code file: (yes / no)

If I edited a tracked code file, what and why:

## 10. Reflection and next step

- What I can now explain without notes:
- What I cannot explain yet:
- One thing I would read again:
- The number I will carry to `construction-13`:
- The call site that surprised me most:
- The concrete type I expected to disappear, and where it still appears:
- What the interface buys that `construction-11` could not offer:
- What it still leaves in the loop:
- What the change test proved that the traces could not:
- The change I would make next to test the interface again:
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
- [ ] I located the interface, the overrides and the private state by line.
- [ ] I compared `struct Knight` with `Piece` and `KnightPiece` and named what each one exposes.
- [ ] I predicted the dispatch table, the change test and the binding probe before measuring.
- [ ] I reproduced all 20 traces on the unmodified baseline.
- [ ] I built the dispatch probe and saw the derived functions run through `const Piece &`.
- [ ] I measured the object and binary cost of late binding.
- [ ] I added a second implementation in a scratch file and exercised it through `Piece &`.
- [ ] I substituted it at the root with zero loop edits and got 20 of 20 traces.
- [ ] I restored the original and got 20 of 20 traces.
- [ ] I removed `virtual` and saw the same call return the base value.
- [ ] I explained `override` and the virtual destructor.
- [ ] I named what the interface does not cover: the movement geometry.
- [ ] I separated what the call site knows, what the object decides, what the compiler emits and
      what a run can see.
- [ ] My one-sentence explanation avoids the mechanism words and the verdict words.
- [ ] I did not edit any tracked code file.

No score. If a box is unchecked, write the next smallest action in §10.
