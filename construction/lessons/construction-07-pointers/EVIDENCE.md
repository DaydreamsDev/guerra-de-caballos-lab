# construction-07 — Evidence

> [English](EVIDENCE.md) | [Español](EVIDENCE-es.md)

> Learner-owned record. Write in English.

This record compares two ways of reaching the same knight and the same board cell: a binding name
and a location. Keep both columns. A number for one file alone cannot answer the question, and a tie
is a result, not a missing difference.

## 1. Run setup

- Date:
- Seed: `1`
- Player name: `Learner`
- Repository root:
- Compiler and version (`g++ --version` or `clang++ --version`):
- Pointers baseline read on (date):
- References baseline read on (date):
- Construction-06 record at hand? (yes / no)
- The count I carried from `construction-06` (mutable / read-only / by-value knight parameters):

## 2. Pointer inventory (before the run)

### 2.1 Functions that name a knight

Fill this from the source, with line numbers, not from memory.

| Function | Line | Knight parameters | Form | Reads | Writes |
|---|---|---|---|---|---|
| | | | | | |

Counts:

- Functions with a mutable `Knight *` parameter:
- Functions with a `const Knight *` parameter:
- Functions with a `Knight` parameter by value:
- Functions that return a knight pointer:
- Functions that return a board pointer:
- Mutable knight parameters, total:
- Read-only knight parameters, total:
- Knight parameters by value, total:
- Reference parameters that remain, total:

### 2.2 The five forms

| Form | Example in this file | What it permits | What it forbids |
|---|---|---|---|
| value parameter | | | |
| reference parameter | | | |
| `const` reference parameter | | | |
| pointer parameter | | | |
| pointer return | | | |

- What a pointer is at the language level:
- What a pointer is not:
- What the call site shows about mutation:
- Where the `&` appears, and where it does not:

### 2.3 Board access

| Question | Answer |
|---|---|
| Declaration of the board in `main` | |
| Rows and columns allocated / used | |
| `getBoardCell` signature and line | |
| What `getBoardCell` returns | |
| `getBoardCell` call sites | |
| What the second dimension of the parameter type encodes | |
| What the returned type keeps and what it drops | |

### 2.4 The diff

- Lines added:
- Lines removed:
- Net lines:
- Function definitions before / after:
- Parameters before / after:
- Signatures that change a knight type:
- Call sites that gain an `&`:
- Board expressions that become accessor calls:
- Reference parameters that remain:
- Traces that differ, of 20:

## 3. Prediction (written before measuring)

### 3.1 The structural diff

The change: replace every knight reference with a pointer, add `getBoardCell`, keep the game.

| Question | My prediction | Confidence | Reason |
|---|---|---|---|
| Lines added | | | |
| Lines removed | | | |
| Function definitions after | | | |
| Parameters after | | | |
| Signatures that change a knight type | | | |
| Call sites that gain an `&` | | | |
| Board expressions that become accessor calls | | | |
| Reference parameters that remain | | | |
| Seeds that differ, of 20 | | | |

- The result I expect to miss, and why:

### 3.2 Board addresses

| Question | My prediction |
|---|---|
| Element offset of (2,3) from (0,0), width 9 | |
| Element offset of (2,3) from (0,0), width 11 | |
| What `boardCells[2]` is, and what it decays to | |
| Does `boardCells[2] + 3` equal `&boardCells[2][3]`? | |
| What `+ 3` adds: bytes or elements | |
| What `getBoardCell(...) + 9` points to, width 9 | |
| What `getBoardCell(...) + 9` points to, width 11 | |
| Which of these the compiler checks | |

- Why the unit of pointer arithmetic is the element, not the byte:
- The platform fact that could make elements and bytes look equal:

### 3.3 The limits

- Can a `Knight *` be null?
- What happens when a null pointer is dereferenced?
- Can a pointer be reseated after initialization?
- What does `activeKnight = machineKnight;` do now, and what did it do in `construction-06`?
- What happens if a pointer outlives its pointee?
- Does the call site show that the pointee may change?
- Which of these does the compiler catch?

## 4. Run — both baselines

- Pointers build command:
- References build command:
- Exit statuses:
- Diff target: `../../../common/traces/seed-0001.txt`

Record `empty` or `diff` in each result cell.

| Check | Command | Result |
|---|---|---|
| Pointers vs golden | | |
| References vs golden | | |
| Pointers vs references | | |

```text
<diff output, or "no differences">
```

Seeds that matched, of 20:

If any diff is not empty, stop here and fill §11.

## 5. Rewrite one board access

- Scratch copy:
- Access replaced:
- Replacement:

```cpp
BoardCell *rowStart = boardCells[destinationRow];
BoardCell *destinationCell = rowStart + destinationColumn;
```

- I did not edit the tracked pointers baseline: (yes / no)

### 5.1 Build and run

- Build command:
- Exit status:
- Seeds that matched, of 20:
- Why the addresses are identical:

### 5.2 The stride numbers

- Probe file:
- `rowStart + 3 == &board[2][3]`:
- `flat == &board[2][3]`, width 9:
- Element offset, width 9:
- Byte offset, width 9:
- `sizeof(BoardCell)`:
- Element offset, width 11:
- `flat == &board[2][3]`, width 11:
- What changed when the literal `9` replaced the constant:
- The sentence I will keep about elements and bytes:

### 5.3 Prediction check

- Predicted rewrite result / observed:
- Predicted offset / observed:
- Closest prediction, and why:
- Furthest prediction, and what I missed:

## 6. Change the row width

- Scratch copy:
- Constant changed:
- Build exit status:
- Traces that differ, of 20:
- Why differing traces are expected here and not a regression:

### 6.1 What adapts and what misreads

- The two-dimensional form after the change:
- The flattened form with `boardColumnCount + 1`:
- The flattened form with a literal `9`:
- Which of the three the compiler checks:
- Which of the three fails silently:
- What would make a wrong stride fail loudly instead:

### 6.2 Count where the layout lives

| Baseline | Two-index expressions that rely on the stride | Parameter declarations with `[][boardColumnCount + 1]` |
|---|---|---|
| `construction-06` | | |
| `construction-07` | | |

- The total in each baseline:
- What moved, and what stayed:
- The place where the width is still unavoidable:
- The place where it disappeared:

## 7. Null, reseating and lifetime

- Scratch copies:
- I did not edit the tracked pointers baseline: (yes / no)

### 7.1 Null

- Lines added:
- Build exit status:
- Run result:
- What the compiler could not know:
- What a reference could not represent:
- What every dereference now promises:

### 7.2 Reseating

- Lines used:
- What `activeKnight` points to before / after:
- What `before` points to:
- What the same assignment did in `construction-06`:
- Why I did not run this inside the game:

### 7.3 Lifetime

- Function that returns a local address:
- Build exit status:
- Diagnostic class:
- Message:

```text
<error output>
```

- Why the compiler caught this case:
- Which lifetime errors it cannot catch:
- Why `getBoardCell` is valid for the whole game:
- Which lesson owns ownership and lifetime:

### 7.4 What the compiler proves

| Check | What the compiler proves | What only a run can prove |
|---|---|---|
| A pointer of the right pointee type | | |
| `const` on the pointee | | |
| A returned local address | | |
| A null dereference | | |
| Pointer arithmetic inside a row | | |

- What a type can prevent that a trace cannot:
- What a trace can show that a type cannot:

## 8. Explain

One sentence, without the words *pointer*, *address*, *dereference*, *better*, *worse*, *clean*,
*cleaner*, *nice*, *elegant*, *ugly* or *modular*:

>

One paragraph:

- What a pointer type decides:
- What a pointer return decides:
- What `const` on the pointee decides, and where it is checked:
- What the call site shows, and what it hides after the address is held:
- What the board accessor localizes:
- What it does not localize:
- What the compiler can prove:
- What only a run can prove:
- Which later lesson fixes bounds and representation:
- Which later lessons fix invariants and access:
- Which later lesson fixes ownership and lifetime:

The four things I kept apart:

## 9. Acceptance check

- I ran both unmodified baselines: (yes / no)
- I got three empty diffs and 20 matching seeds: (yes / no)
- I inventoried 17 knight parameters: 9 mutable, 8 read-only, 0 by value: (yes / no)
- I identified the two knight pointer returns and the mutating parameter: (yes / no)
- I identified `getBoardCell` returning `BoardCell *` and its three call sites: (yes / no)
- I predicted the diff and the board offsets before measuring: (yes / no)
- I rewrote one board access as `rowStart + column`: (yes / no)
- The rewrite still matched all 20 traces: (yes / no)
- I measured the offset at 21 for width 9 and 25 for width 11: (yes / no)
- I explained that `+ column` adds elements, not bytes: (yes / no)
- I changed the width and saw the 2D form adapt and the literal `9` misread: (yes / no)
- I counted where the row width lives in both baselines: (yes / no)
- A null pointer compiled and crashed: (yes / no)
- A returned local address failed the build: (yes / no)
- I showed that a pointer can be reseated and a reference cannot: (yes / no)
- I can explain what a pointer makes explicit and what it hides: (yes / no)
- My sentence avoids the three syntax words and the eight verdict words: (yes / no)
- I did not edit either tracked baseline: (yes / no)

If I changed a tracked baseline, what and why:

## 10. Reflection and next step

- What I can now explain without notes:
- What I cannot explain yet:
- One thing I would read again:
- The number I will carry to `construction-08`:
- The check I trust most: the build, the diff, or the address:
- The check that surprised me:
- What the accessor hid that the two-dimensional index showed:
- What the pointer added that the reference did not have:
- Next step:

## 11. If a baseline is missing or unfaithful

Fill this only if a file was absent, a build failed for an environmental reason, or a diff was not
empty.

- Missing file, build error, or diff:
- Exact command output:
- What I did instead of inventing a count:
- When I will retry:

## 12. Self-check

- [ ] I inventoried every knight parameter with its line and form.
- [ ] I inventoried the board accessor and its call sites.
- [ ] I wrote the diff prediction, the address prediction and the limits before measuring.
- [ ] I built both baselines and got three empty diffs, plus 20 matching seeds.
- [ ] I rewrote one board access as `rowStart + column` and kept 20 matches.
- [ ] I measured the element offset at width 9 and width 11.
- [ ] I changed the row width and separated the loud failure from the silent one.
- [ ] I counted where the row width lives in both baselines.
- [ ] I showed a null pointer compiling and crashing.
- [ ] I showed a returned local address failing the build.
- [ ] I showed the difference between reseating a pointer and assigning through a reference.
- [ ] I separated what the type permits, what the call site shows, what the compiler checks and what
      only a run proves.
- [ ] My one-sentence explanation avoids the three syntax words and the eight verdict words.
- [ ] I did not edit the tracked baselines.

No score. If a box is unchecked, write the next smallest action in §10.
