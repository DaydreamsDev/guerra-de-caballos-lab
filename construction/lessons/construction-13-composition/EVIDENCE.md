# construction-13 — Evidence

> [English](EVIDENCE.md) | [Español](EVIDENCE-es.md)

> Learner-owned record. Write in English.

This record follows the square from the base class into a part: what `Knight` owns, what it
forwards, which type the loop can name, and what the object saves and gives up. Keep the
predictions. The gap between your prediction and the observation is the result, whether it is zero
or not.

## 1. Run setup

- Date:
- Seed: `1`
- Player name: `Learner`
- Repository root:
- Compiler and version (`g++ --version` or `clang++ --version`):
- Baseline read on (date):
- Construction-12 record at hand? (yes / no)
- The number I carried from `construction-12` (public declarations / definitions / traces):
- Scratch directory I used:

## 2. The composition (before the run)

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

Compared with `construction-12`:

- Public declarations: ______ → ______
- Definitions: ______ → ______
- Complete class definitions: ______ → ______
- Headers: ______ → ______
- Translation units: ______ → ______
- Private data declarations: ______ → ______
- Board declarations that changed:
- Virtual declarations removed:
- Overrides removed:
- Forwarding methods added:
- The type the loop can name:

### 2.2 Line map

`position.h`:

| Lines | Contents |
|---|---|
| | |

`knight.h`:

| Lines | Contents |
|---|---|
| | |

`knight.cpp`:

| Lines | Contents |
|---|---|
| | |

- Line of the `Position` member:
- Line of the `roleName` member:
- Line of `row`:
- Line of `column`:
- Line of `set_position`:
- Line of `role_name`:
- Line of the `public:` label in `knight.h`:
- Line of the `private:` label in `knight.h`:
- Lines that construct a `Knight` in `run_game`:
- Line that includes `knight.h` in `game.cpp`:

### 2.3 Roles and delegation

| Role | Operation | Answered by | What the loop asks |
|---|---|---|---|
| | | | |

- Number of public operations in `Knight`:
- Operations that forward to `Position`:
- The operation that does not forward, and why:
- Operations that are `const`:
- What a `const Knight *` may call:
- The only operation that writes:
- Public data members in `Knight` or `Position`:
- Number of call sites in `game.cpp`:
- The four operations those call sites ask for:
- Number of helper signatures in `game.cpp` that name `Knight`:
- Headers with `virtual`, `override` or `: public`:
- What would break if `Position::rowValue` were public:

## 3. Prediction (written before measuring)

### 3.1 The delegation table

Every expression names a `Knight`; there is no base type to hide behind.

| Expression | Which function runs | Predicted output | Confidence |
|---|---|---|---|
| `playerKnight.set_position(3, 4)` | | | |
| `playerKnight.row()` | | | |
| `playerKnight.column()` | | | |
| `playerKnight.role_name()` | | | |
| `asKnight.row()` through `const Knight &` | | | |
| `copy.set_position(5, 6)` after `Knight copy = playerKnight` | | | |

- The row I expect to get wrong, and why:
- What I expect the copy to share with the original:

### 3.2 The change test

| Question | My prediction |
|---|---|
| Files I will delete | |
| Files I will add | |
| Lines I will change in `game.cpp` | |
| Functions whose signature changes | |
| Logic lines I will change (not names) | |
| Seeds that match, of 20, after the change | |
| New concepts the change requires | |

- The prediction I am least sure about, and why:

### 3.3 The size and indirection probe

| Question | My prediction |
|---|---|
| `sizeof(Position)` | |
| `sizeof(Knight)` | |
| `sizeof(KnightPiece)` | |
| Bytes saved per object | |
| `nm -C knight.o` lines with `vtable` or `typeinfo` | |
| Indirect calls in `game12.o` | |
| Indirect calls in `game13.o` | |
| `game.o` text: grows, shrinks or stays the same? | |

- What I expect the member to change in the object code, in one sentence:

## 4. Run — baseline and delegation probe

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
| `role` through `Knight` | | | |
| `role` through `const Knight &` | | | |
| Original after moving the copy | | | |
| `sizeof(Position)` | | | |
| `sizeof(Knight)` | | | |
| `game.o` text | | | |

- The line that surprised me:
- What I did next instead of adjusting the prediction:

## 5. Compare — what delegation proves

- Output of `grep -rn 'virtual\|override\|: public' code/`:
- Number of lines that mention inheritance syntax:
- Output of `nm -C knight.o | grep -E 'vtable|typeinfo'`:
- Number of helper signatures in `game.cpp` that name `Knight`:
- Which of them are inside the anonymous namespace:

| Lines | Function | Calls |
|---|---|---|
| | | |

- Total call sites:
- Total lines that contain a call site:
- `sizeof(Position)`:
- `sizeof(Knight)`:
- `sizeof(KnightPiece)`:
- What the object stores:
- What the class stores:
- What the probe proves about the forwarded call:
- What the probe does not prove:

### 5.1 The rename probe

- Files changed in the scratch copy:
- Old role names:
- New role names:
- Lines that differ, of 89:
- Which kinds of line differ (`who=`, `move=`, `trapped=`, `winner=`):
- Which lines are identical:
- What this proves about `role_name()`:
- What this proves about the refactor:

### 5.2 Code generation

- Indirect calls in `game12.o`:
- Indirect calls in `game13.o`:
- Direct call relocations to `Knight::` in `game13.o`:
- Why the direct count is larger than the call-site count:
- What the absence of a table does not prove about speed:

## 6. The change test in a scratch copy

- Scratch copy path:
- Files deleted:
- Files added:
- Files edited:
- Lines added in `game.cpp`:
- Lines removed in `game.cpp`:
- Hunks in the diff:
- Logic lines changed:
- Tracked files edited: (yes / no)

### 6.1 Delete the interface

- Files deleted:
- Lines deleted:
- Virtual declarations removed:
- Overrides removed:
- What still included `piece.h`:

### 6.2 Add the part

- Header path:
- Implementation path:
- Lines in the header:
- Lines in the implementation:
- The operations it exposes:
- Its private state:
- Its default state:

### 6.3 Add the whole

- Header path:
- Implementation path:
- Lines in the header:
- Lines in the implementation:
- Operations that forward:
- Operation that does not forward:
- What the constructor initializes:

### 6.4 Rename the loop

- Command or script I used:
- Output of `grep -n 'Piece' game.cpp`:
- Lines added:
- Lines removed:
- Hunks:
- Kinds of line that changed:
- Rules that changed:

### 6.5 Build and traces

- Build command:
- Seeds that matched, of 20:
- Diff against the unmodified baseline transcript:

### 6.6 The gap: substitutability

- The `construction-12` substitution that no longer compiles:
- Why it has nowhere to attach:
- Option 1 (duplicate the loop): functions that would be duplicated:
- Option 2 (restore an interface): which lesson that is:
- Option 3 (parameterize the loop): which lesson that is:
- The precise acceptance sentence of this lesson:

## 7. The composition and its limits

| Question | Answer |
|---|---|
| What does `Knight` own? | |
| What does it forward? | |
| What does it answer alone? | |
| Where does position logic live? | |
| What type does the loop hold? | |
| How many signatures name that type? | |
| What does a new same-behavior type cost? | |
| What does a new movement rule cost? | |
| What does the member cost per object? | |
| What does it save per object? | |
| What does it save in the binary? | |
| What does it cost in the source? | |
| Can the loop accept a second type without edits? | |

- What composition removes from the loop:
- What it moves into the loop:
- What it does not remove:
- Which later lesson restores one loop for two types:
- Which later lesson owns lifetime:

## 8. Explain

One sentence, without the words *composition*, *inheritance*, *delegation*, *class*, *object*,
*virtual*, *pointer*, *compiler*, *better*, *worse*, *clean*, *flexible*, *reusable* or *elegant*:

>

One paragraph:

- What composition fixes:
- What enforces it:
- What makes it different from `construction-12`:
- What the 19 call sites do:
- What the 20 traces prove:
- What the traces do not prove:
- What the change test costs:
- What composition does not cover:
- What it saves:
- What it costs:
- Which later lesson builds on it:

The four things I kept apart:

## 9. Acceptance check

- I identified the `Position` member and the three forwarding methods: (yes / no)
- I counted the 19 call sites and mapped them to functions: (yes / no)
- I found that no header contains `virtual`, `override` or `: public`: (yes / no)
- I named the ten helper signatures that mention `Knight`: (yes / no)
- I predicted the delegation table and observed the forwarded values: (yes / no)
- I reproduced all 20 traces on the unmodified baseline: (yes / no)
- I built the delegation probe and saw the forwarded values: (yes / no)
- I measured `sizeof(Position)` and `sizeof(Knight)`: (yes / no)
- I confirmed that copying a knight copies its position: (yes / no)
- I deleted the interface files and added `Position` and `Knight` in a scratch copy: (yes / no)
- I renamed the loop with zero rule changes: (yes / no)
- I got 20 of 20 traces after the refactor: (yes / no)
- I counted 13 indirect calls in `game12.o` and 0 in `game13.o`: (yes / no)
- I confirmed that `nm` shows no vtable or typeinfo for `Knight`: (yes / no)
- I named substitutability as the limit of this design: (yes / no)
- I explained why no virtual destructor is needed: (yes / no)
- I did not edit any tracked code file: (yes / no)

If I edited a tracked code file, what and why:

## 10. Reflection and next step

- What I can now explain without notes:
- What I cannot explain yet:
- One thing I would read again:
- The number I will carry to `construction-14`:
- The call site that surprised me most:
- The type I expected to stay hidden, and where it appears instead:
- What the member buys that the base class could not offer:
- What it gives up:
- What the change test proved that the traces could not:
- The change I would make next to test this design again:
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
- [ ] I located the member, the forwards and the private state by line.
- [ ] I compared `KnightPiece` with `Knight` and `Position` and named what each one exposes.
- [ ] I predicted the delegation table, the change test and the size probe before measuring.
- [ ] I reproduced all 20 traces on the unmodified baseline.
- [ ] I built the delegation probe and saw the forwarded values through `const Knight &`.
- [ ] I measured the object and binary cost of the member.
- [ ] I confirmed that copying a knight copies its position.
- [ ] I replaced the inheritance with a member and forwarding in a scratch copy.
- [ ] I got 20 of 20 traces after the refactor and changed zero rules.
- [ ] I counted 13 indirect calls in `game12.o` and 0 in `game13.o`.
- [ ] I confirmed that `nm` shows no vtable or typeinfo for `Knight`.
- [ ] I named substitutability as the limit of this design.
- [ ] I separated what the loop can name, what the part decides, what the compiler emits and what a
      run can see.
- [ ] My one-sentence explanation avoids the mechanism words and the verdict words.
- [ ] I did not edit any tracked code file.

No score. If a box is unchecked, write the next smallest action in §10.
