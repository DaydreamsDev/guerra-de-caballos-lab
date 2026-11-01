# construction-05 — Evidence

> [English](EVIDENCE.md) | [Español](EVIDENCE-es.md)

> Learner-owned record. Write in English.

This record compares two element types for one board. Keep both columns. A number for one file
alone cannot answer the question, and a tie is a result, not a missing difference.

## 1. Run setup

- Date:
- Seed: `1`
- Player name: `Learner`
- Repository root:
- Compiler and version (`g++ --version` or `clang++ --version`):
- Array-of-records baseline read on (date):
- Records baseline read on (date):
- Construction-04 record at hand? (yes / no)
- The two counts I carried from `construction-04` (signature changes / call sites):

## 2. Board inventory (before the run)

### 2.1 Functions that name `BoardCell`

Fill this from the source, with line numbers, not from memory.

| Function | Line | Receives | Fields it reads | Fields it writes |
|---|---|---|---|---|
| | | | | |

Counts:

- Functions that receive the board:
- Of those, functions that read a field:
- Of those, functions that write a field:
- Functions that receive the board and read no field:
- Call sites that pass the board:
- Field accesses (`.visited`) in the whole file:

### 2.2 What the element type says

| Question | construction-04 | construction-05 |
|---|---|---|
| Element type | | |
| What one element holds | | |
| Subscripts to read the visited fact | | |
| Subscripts to read a second per-cell fact | | |
| Where a new per-cell fact is declared | | |
| Signatures a new per-cell fact forces to change | | |
| What the traversal iterates over | | |
| What keeps two per-cell facts on the same cell | | |
| What the board knows about its own size | | |
| What prevents `boardCells[0][0].visited = true` | | |

### 2.3 Parameters

| Metric | construction-04 | construction-05 |
|---|---|---|
| Function definitions (count `main`) | | |
| Parameters across all definitions | | |
| `tryMove` parameters | | |
| `playTurn` parameters | | |
| `playGame` parameters | | |
| Board parameters | | |
| Parameters added or removed by the conversion | | |

- What changed in each board signature:
- What did not change in each board signature:
- Elements in the array / elements that are board cells:

## 3. Prediction (written before measuring)

### 3.1 The field cost

R-LASTVISIT, in my own words:

- What is recorded:
- When it is recorded:
- Who reads it at this point:
- Why the output does not change:

My committed numbers for the field:

| Metric | My number | Confidence | Reason |
|---|---|---|---|
| Field declaration sites | | | |
| Assignment sites | | | |
| Branches of `playTurn` that write | | | |
| Signatures that change | | | |
| Call sites that change | | | |
| New arrays | | | |
| New globals | | | |
| Lines added | | | |
| Golden traces that differ (of 20) | | | |

- What `BoardCell boardCells[9][9] = {}` does to the new field:
- The writer I expect to forget, and why:

### 3.2 The traversal

| Metric | My number | Confidence | Reason |
|---|---|---|---|
| Lines in the traversal function | | | |
| Seeds where the value equals `moves=` (of 20) | | | |
| Seeds where the coordinates equal the last `move=` destination (of 20) | | | |
| Seeds where a forgotten capture write is visible in the value | | | |
| Seeds where a transposed write is visible in the value | | | |
| Seeds where a transposed write is visible in the coordinates | | | |

- The first loop bound I expect to use:
- The last loop bound I expect to use:
- What an untouched board returns:

### 3.3 The alternatives

Counting rules I used (in one line each):

- Edit site:
- Signature change:
- Call site:
- New array:
- New global:

My committed numbers:

| Metric | Field in `BoardCell` | Parallel `int` array | 64 `int` names | Confidence |
|---|---|---|---|---|
| Edit sites | | | | |
| Signature changes | | | | |
| Call sites changed | | | | |
| New arrays | | | | |
| New globals | | | | |
| Lines added | | | | |
| Golden traces broken (of 20) | | | | |
| How a cell is identified | | | | |

### 3.4 Which comparison is fair

- Subscript pairs that identify both facts, record version:
- Subscript pairs that identify both facts, parallel version:
- What makes two parallel subscript pairs the same cell:
- Where a forgotten second-array write appears first:
- What the compiler says about a swapped subscript:
- The comparison I will treat as the real one, and why:

## 4. Run — both baselines

- Array-of-records build command:
- Records build command:
- Exit statuses:
- Diff target: `../../../common/traces/seed-0001.txt`

Record `empty` or `diff` in each result cell.

| Check | Command | Result |
|---|---|---|
| Array of records vs golden | | |
| Records vs golden | | |
| Array of records vs records | | |

```text
<diff output, or "no differences">
```

Seeds that matched, of 20:

If any diff is not empty, stop here and fill §11.

## 5. Add `moveNumber` and record it

- Scratch copy:
- The field declaration line:
- The capture-branch assignment:
- The legal-branch assignment:
- I did not edit the tracked array-of-records baseline: (yes / no)

```text
<diff of the scratch copy against the tracked baseline>
```

Output of `grep -n 'moveNumber' <scratch copy>`:

```text
<output>
```

Seeds that matched, of 20:

- Signatures that changed:
- Call sites that changed:
- Functions whose body changed:
- Lines added / removed:

### 5.1 The four observations

| Observation | Command or edit | Result |
|---|---|---|
| No signature moved | | |
| The trace cannot see the field | | |
| The field has two writers | | |
| No new namespace-scope data | | |

- What `= {}` does to the board and to a field added later:
- What the trace proves, and what it does not:
- `sizeof(BoardCell)` before / after:
- Bytes in the 9×9 board before / after:
- Mutable namespace-scope names, before / after (`nm`):
- What `moveNumber` already means elsewhere in this file:

### 5.2 Prediction check

- Predicted assignment sites / observed:
- Predicted lines / observed:
- Closest prediction, and why:
- Furthest prediction, and what I missed:

## 6. The traversal

- Function name:
- Scratch file:
- The two loop bounds:
- The starting value of the maximum:
- I printed the result to stderr, so stdout stayed unchanged: (yes / no)

```text
<function source>
```

```bash
<commands>
```

Seed 1 result:

```text
<last_recorded=... moves=...>
```

### 6.1 The 20 results

| Seed | `last_recorded` | `moves=` | Coordinates | Last `move=` destination | Match? |
|---|---|---|---|---|---|
| 1 | | | | | |
| 2 | | | | | |
| 3 | | | | | |
| 4 | | | | | |
| 5 | | | | | |
| 6 | | | | | |
| 7 | | | | | |
| 8 | | | | | |
| 9 | | | | | |
| 10 | | | | | |
| 11 | | | | | |
| 12 | | | | | |
| 13 | | | | | |
| 14 | | | | | |
| 15 | | | | | |
| 16 | | | | | |
| 17 | | | | | |
| 18 | | | | | |
| 19 | | | | | |
| 20 | | | | | |

- Seeds where value and `moves=` agree:
- Seeds where coordinates and the last move agree:
- Why the maximum equals `moves=`:
- Why the maximum is unique:
- What an untouched board returns, and what that value means:
- Why the scan cannot stop early:
- What the traversal adds that the transcript did not print:

### 6.2 The forgotten writer

- Assignment I removed:
- Seeds where all traces still passed, of 20:
- Seeds where the value disagreed with `moves=`:
- Seeds where the value still agreed:
- Games that end in a capture, of 20:
- Games that end in a trap, of 20:
- Why the split is exactly that:

### 6.3 The value-only limit

- Subscripts I swapped:
- Seeds where all traces still passed, of 20:
- Seeds where the value-traversal disagreed, of 20:
- Seeds where the coordinate-traversal disagreed, of 20:
- Seeds that survived the transpose, and why:
- The sentence I will keep about the maximum and permutation:

### 6.4 What each check proves

| Check | What it proves | What it cannot see |
|---|---|---|
| Golden trace | | |
| Value traversal | | |
| Coordinate traversal | | |
| An invariant (not built here) | | |

- The claim I can make today without overreaching:
- The claim I cannot make yet, and which lesson supplies it:

## 7. The same fact without the record

- Scratch copy:
- Placement I applied first:
- I did not edit the tracked records baseline: (yes / no)

```text
<diff of the parallel-array copy against the records baseline>
```

Seeds that matched, of 20:

### 7.1 Parallel array

| Metric | My prediction | Observed |
|---|---|---|
| Edit sites | | |
| Signature changes | | |
| Call sites changed | | |
| New arrays | | |
| New globals | | |
| Lines added / removed | | |
| Golden traces broken (of 20) | | |

- Why `tryMove` did not change:
- Why the new array still had to reach `playTurn` and `playGame`:
- What the compiler checks about the two subscript pairs:
- What the compiler does not check:
- What a third per-cell fact would cost:

### 7.2 The 64-name version

- Declarations needed for the second fact, keeping the boolean board:
- Way to select one name from a row and a column:
- Lines in that selection at each write site:
- Comparisons in the traversal:
- Way to pass the set to a function:
- Declarations needed if the boolean board is also replaced by names:
- What the exercise shows about the array:
- What it shows about the array of records:

### 7.3 Global placement

- Edit sites with the array at namespace scope:
- Signature changes:
- Call sites changed:
- New mutable globals:
- What moved out of the signatures:
- Where the cost reappears:

### 7.4 Cost table

| Metric | Field in `BoardCell` | Parallel `int` array | 64 `int` names | What it measures |
|---|---|---|---|---|
| Edit sites | | | | typing |
| Signature changes | | | | coupling made visible |
| Call sites changed | | | | call-graph reach |
| New arrays | | | | second collection |
| New globals | | | | hidden state |
| Lines added / removed | | | | blast radius |
| Golden traces broken (of 20) | | | | contract change |
| What identifies a cell | | | | attachment |
| Cost of a third fact | | | | marginal cost |

- The row where the record wins:
- The row where the parallel array wins, if any:
- Why that row is not the whole comparison:
- What the conversion from `construction-04` cost, and whether this change repays it:

## 8. Explain

One sentence, without the word *struct* and without *better*, *worse*, *clean*, *cleaner*, *nice*,
*elegant*, *ugly* or *modular*:

>

One paragraph:

- What the element type decides:
- What the traversal decides:
- What the array of records localizes:
- What it does not localize:
- What a reader must hold in working memory to know which fields a function uses, in each file:
- What a golden trace, a value traversal and a coordinate traversal can each prove:
- Which later lesson fixes naming the cell once:
- Which later lesson fixes representation and bounds:
- Which later lessons fix invariants and access:
- Which later lesson fixes the loop itself:

## 9. Acceptance check

- I traversed the board with two loops: (yes / no)
- The traversal found the last move number: (yes / no)
- The result matched `moves=` on all 20 seeds: (yes / no)
- `moveNumber` was added to `BoardCell`: (yes / no)
- The successful move number was recorded at both write sites: (yes / no)
- All 20 golden traces still passed: (yes / no)
- I can explain why no parallel array is needed: (yes / no)
- I can explain why 64 variables cannot be selected by a coordinate: (yes / no)
- I can name what the record localizes and what it does not: (yes / no)
- The explanation omits *struct* and the eight verdict words: (yes / no)
- I did not edit either tracked baseline: (yes / no)

If I changed a tracked baseline, what and why:

## 10. Reflection and next step

- What I can now explain without notes:
- What I cannot explain yet:
- One thing I would read again:
- The number I will carry to `construction-06`:
- The row where the record wins, and the row where it loses:
- The change that would make the parallel array expensive next:
- The change that would make the record expensive next:
- Next step:

## 11. If a baseline is missing or unfaithful

Fill this only if a file was absent, a build failed, or a diff was not empty.

- Missing file, build error, or diff:
- Exact command output:
- What I did instead of inventing a count:
- When I will retry:

## 12. Self-check

- [ ] I inventoried which function receives the board and which field it touches, with line numbers.
- [ ] I wrote the field prediction, the traversal prediction and both alternatives before measuring.
- [ ] I built both baselines and got three empty diffs, plus 20 matching seeds.
- [ ] I added `moveNumber` to a scratch copy and recorded it at both write sites.
- [ ] I kept all 20 traces passing after the field was added.
- [ ] I wrote one two-loop traversal and checked its value against `moves=`.
- [ ] I checked its coordinates against the last `move=` destination.
- [ ] I showed that a forgotten capture write is invisible in the trace and visible in 11 of 20.
- [ ] I showed that a transposed write is invisible in the value and visible in 14 of 20
      coordinates.
- [ ] I built the parallel-array variant and counted its signatures and call sites.
- [ ] I explained why 64 names cannot be selected by a computed coordinate.
- [ ] I separated the marginal cost from the conversion cost.
- [ ] My one-sentence explanation avoids *struct* and the eight verdict words.
- [ ] I did not edit the tracked baselines.

No score. If a box is unchecked, write the next smallest action in §10.
