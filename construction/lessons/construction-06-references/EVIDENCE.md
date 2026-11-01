# construction-06 — Evidence

> [English](EVIDENCE.md) | [Español](EVIDENCE-es.md)

> Learner-owned record. Write in English.

This record compares two ways of naming the same knight record: a copy and an alias. Keep both
columns. A number for one file alone cannot answer the question, and a tie is a result, not a
missing difference.

## 1. Run setup

- Date:
- Seed: `1`
- Player name: `Learner`
- Repository root:
- Compiler and version (`g++ --version` or `clang++ --version`):
- References baseline read on (date):
- Array-of-records baseline read on (date):
- Construction-05 record at hand? (yes / no)
- The two counts I carried from `construction-05` (signature changes / call sites):

## 2. Alias inventory (before the run)

### 2.1 Functions that name a knight

Fill this from the source, with line numbers, not from memory.

| Function | Line | Knight parameters | Form | Reads | Writes |
|---|---|---|---|---|---|
| | | | | | |

Counts:

- Functions with a mutable `Knight &` parameter:
- Functions with a `const Knight &` parameter:
- Functions with a `Knight` parameter by value:
- Functions that return a knight alias:
- Mutable knight parameters, total:
- Read-only knight parameters, total:
- Knight parameters by value, total:

### 2.2 The five forms

| Form | Example in this file | What it permits | What it forbids |
|---|---|---|---|
| value parameter | | | |
| reference parameter | | | |
| `const` reference parameter | | | |
| reference return | | | |
| local alias | | | |

- What a reference is at the language level:
- What a reference is not:
- What the call site says about mutation:

### 2.3 What was already there

List every knight alias that exists in `construction-05` before this lesson. Use line numbers.

| Function | Line | Form | What it does |
|---|---|---|---|
| | | | |

- The two lines that disappeared from `playTurn`, and what replaced them:
- The line that changed from mutable to `const`, and why:
- What this lesson adds beyond the syntax:
- What it does not add:

### 2.4 The diff

- Lines added:
- Lines removed:
- Net lines:
- Function definitions before / after:
- Parameters before / after:
- Existing signatures that changed type:
- Existing call sites that changed an argument:
- New call sites:
- Traces that differ, of 20:

## 3. Prediction (written before measuring)

### 3.1 The by-value flip

The change: delete the `&` after `Knight` in `moveKnight`'s signature.

| Question | My prediction | Confidence | Reason |
|---|---|---|---|
| Builds with `-Wall -Wextra -Werror`? | | | |
| Diagnostic class, if any | | | |
| First differing line of seed 1 | | | |
| `move=` line still appears? | | | |
| Next `turn=` header of that knight | | | |
| Moves in seed 1 (golden: 21) | | | |
| Seeds that differ, of 20 | | | |
| Signature changes | | | |
| Call sites edited | | | |
| Characters removed | | | |
| Game terminates? | | | |

- Why the move line can survive the failed mutation:
- Why the next header cannot:
- The result I expect to miss, and why:

### 3.2 Selector return types

| Change | Compiles? | What the compiler says | What the program would do |
|---|---|---|---|
| `selectActiveKnight` returns `Knight` | | | |
| `selectActiveKnight` returns `const Knight &` | | | |
| `selectOtherKnight` returns `Knight &` | | | |

- The receiving line I used to reason about binding:
- Which change fails at the return and which fails at the alias:

### 3.3 The limits

- Can a reference be null?
- Can a reference be reseated after initialization?
- What does `activeKnight = machineKnight;` do?
- Does the call site show that the record may change?
- What happens if a reference outlives its record?
- Which of these does the compiler catch?

## 4. Run — both baselines

- References build command:
- Array-of-records build command:
- Exit statuses:
- Diff target: `../../../common/traces/seed-0001.txt`

Record `empty` or `diff` in each result cell.

| Check | Command | Result |
|---|---|---|
| References vs golden | | |
| Array of records vs golden | | |
| References vs array of records | | |

```text
<diff output, or "no differences">
```

Seeds that matched, of 20:

If any diff is not empty, stop here and fill §11.

## 5. Break the boundary

- Scratch copy:
- Signature before:
- Signature after:
- I did not edit the tracked references baseline: (yes / no)

### 5.1 Build with the project flags

- Command:
- Exit status:
- Diagnostic class:
- Full message:

```text
<build output>
```

- What the compiler says the parameter does:
- Why the compiler cannot reject the change on principle:
- What the warning would mean without `-Werror`:

### 5.2 Build and run the observation

- Command used to demote the warning:
- Exit status:
- Seeds that build:
- Command used to run seed 1:

```text
<first 20 lines of the diff, including the move= line and the next turn= header>
```

- The `move=` line that still appears:
- The `turn=` header that exposes the copy:
- Why the move line is not evidence of mutation:
- Why the next header is:
- Moves in seed 1, golden / by value:
- What the stale knight does to the rest of the game:
- The sentence I will keep about intention and proof:

### 5.3 The 20 results

| Seed | Golden moves | By-value moves | First differing line | Differs? |
|---|---|---|---|---|
| 1 | | | | |
| 2 | | | | |
| 3 | | | | |
| 4 | | | | |
| 5 | | | | |
| 6 | | | | |
| 7 | | | | |
| 8 | | | | |
| 9 | | | | |
| 10 | | | | |
| 11 | | | | |
| 12 | | | | |
| 13 | | | | |
| 14 | | | | |
| 15 | | | | |
| 16 | | | | |
| 17 | | | | |
| 18 | | | | |
| 19 | | | | |
| 20 | | | | |

- Seeds that differ:
- Seeds that match:
- What the count says about the blast radius of one character:

### 5.4 Restore

- How I restored the reference:
- Build exit status:
- Seeds that matched after restoring, of 20:

### 5.5 Prediction check

- Predicted build result / observed:
- Predicted first line / observed:
- Predicted seed count / observed:
- Closest prediction, and why:
- Furthest prediction, and what I missed:

## 6. `const` is a one-way street

- Scratch copy:
- I did not edit the tracked references baseline: (yes / no)

### 6.1 A direct write

- Line added:
- Build exit status:
- Compiler message:

```text
<error output>
```

- What the message says about the alias:
- Where the check happens:

### 6.2 The wrong call

- Line added:
- Build exit status:
- Compiler message:

```text
<error output>
```

- Why the call fails:
- Which knight the compiler protected:
- What would have happened at run time without `const`:

### 6.3 Remove `const`

- Return type before / after:
- Parameter types before / after:
- Build exit status:
- Both lines compile? (yes / no)
- Why I did not run this variant:

### 6.4 What the compiler proves

| Check | What the compiler proves | What only a run can prove |
|---|---|---|
| Direct write through a `const` alias | | |
| Passing a `const` alias to `Knight &` | | |
| Writing through a mutable reference | | |
| A copy that receives a write | | |

- The count of lines that failed with `const`, and the count that passed without it:
- What a type can prevent that a trace cannot:
- What a trace can show that a type cannot:

## 7. Selector return types

One change per copy. Record the exact result.

### 7.1 `selectActiveKnight` returns `Knight`

- Build exit status:
- Compiler message:

```text
<error output>
```

- Why the receiving alias rejects it:
- What would be lost even if it compiled:

### 7.2 `selectActiveKnight` returns `const Knight &`

- Build exit status:
- Compiler message:

```text
<error output>
```

- Why the alias rejects it:
- Which qualifier is discarded:

### 7.3 `selectOtherKnight` returns `Knight &`

- Build exit status:
- Compiler message:

```text
<error output, if any>
```

- What the parameters already prevent:
- What would change if the parameters also lost `const`:
- What the program would allow then:

### 7.4 What each form permits

| Return type | Caller may read | Caller may write | Binds to a temporary | Can be null |
|---|---|---|---|---|
| `Knight` | | | | |
| `Knight &` | | | | |
| `const Knight &` | | | | |

- The form `selectActiveKnight` must have, and why:
- The form `selectOtherKnight` must have, and why:
- The form a print helper can have, and why:

## 8. Explain

One sentence, without the words *pointer*, *address*, *better*, *worse*, *clean*, *cleaner*, *nice*,
*elegant*, *ugly* or *modular*:

>

One paragraph:

- What a parameter type decides:
- What a return type decides:
- What `const` decides, and where it is checked:
- What the call site says about mutation, and what it hides:
- What the references localize:
- What they do not localize:
- What the compiler can prove:
- What only a run can prove:
- Which later lesson fixes addresses, null and reseating:
- Which later lesson fixes bounds and representation:
- Which later lessons fix invariants and access:
- Which later lesson fixes ownership and lifetime:

The distinction I kept apart:

## 9. Acceptance check

- I ran both unmodified baselines: (yes / no)
- I got three empty diffs and 20 matching seeds: (yes / no)
- I inventoried 17 knight parameters: 9 mutable, 8 read-only, 0 by value: (yes / no)
- I identified the two reference returns and the mutating parameter: (yes / no)
- I predicted the by-value build result before compiling: (yes / no)
- I changed `moveKnight` to take `Knight` by value: (yes / no)
- I saw the caller's record stop changing: (yes / no)
- I explained the surviving `move=` line and the stale `turn=` header: (yes / no)
- All 20 by-value runs differed: (yes / no)
- I restored the reference and got 20 matches: (yes / no)
- `const` rejected the direct write and the wrong call: (yes / no)
- Removing `const` made both compile: (yes / no)
- I can explain what a reference makes explicit and what it hides: (yes / no)
- My sentence avoids the two syntax words and the eight verdict words: (yes / no)
- I did not edit either tracked baseline: (yes / no)

If I changed a tracked baseline, what and why:

## 10. Reflection and next step

- What I can now explain without notes:
- What I cannot explain yet:
- One thing I would read again:
- The number I will carry to `construction-07`:
- The check I trust most: the build, the diff, or the header:
- The check that surprised me:
- What a pointer would make visible that a reference hides:
- What a pointer would add that a reference does not have:
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
- [ ] I listed the aliases that already existed in `construction-05`.
- [ ] I wrote the by-value prediction, the selector prediction and the limits before measuring.
- [ ] I built both baselines and got three empty diffs, plus 20 matching seeds.
- [ ] I flipped `moveKnight` to by value and recorded the build diagnostic.
- [ ] I explained why the `move=` line survives and the next `turn=` header does not.
- [ ] I ran all 20 seeds by value and counted the differences.
- [ ] I restored the reference and got 20 matches.
- [ ] I used `const` to reject a direct write and a wrong call.
- [ ] I removed `const` and saw both compile without running the result.
- [ ] I tested the three selector return types.
- [ ] I separated what the signature permits, what the call site shows and what the compiler checks.
- [ ] My one-sentence explanation avoids the two syntax words and the eight verdict words.
- [ ] I did not edit the tracked baselines.

No score. If a box is unchecked, write the next smallest action in §10.
