# construction-04 — Evidence

> [English](EVIDENCE.md) | [Español](EVIDENCE-es.md)

> Learner-owned record. Write in English.

This record compares two representations of one fact. Keep both columns. A number for one file
alone cannot answer the question, and a tie is a result, not a missing difference.

## 1. Run setup

- Date:
- Seed: `1`
- Player name: `Learner`
- Repository root:
- Compiler and version (`g++ --version` or `clang++ --version`):
- Records baseline read on (date):
- Functions baseline read on (date):
- Construction-03 record at hand? (yes / no)
- Callers changed and edit sites I counted in construction-03:

## 2. Signature inventory (before the run)

### 2.1 Functions that receive a `Knight`

Fill this from the source, with line numbers, not from memory.

| Function | Line | Receives | Fields it reads | Fields it writes |
|---|---|---|---|---|
| | | | | |

Counts:

- Functions that receive a `Knight`:
- Of those, `const Knight &`:
- Of those, `Knight &`:
- Functions that receive a knight and read no field:
- Field accesses (`.row` / `.column`) in the whole file:

### 2.2 Parameters

| Metric | construction-03 | construction-04 |
|---|---|---|
| Function definitions (count `main`) | | |
| Parameters across all definitions | | |
| `tryMove` parameters | | |
| `playTurn` parameters | | |
| `playGame` parameters | | |
| `tryMove` parameters that are outputs | | |

- Parameters that disappeared:
- Functions that changed parameter count:
- Functions whose parameter count did not change, and why:

### 2.3 Side by side

| Question | construction-03 | construction-04 |
|---|---|---|
| How a function learns the active knight | | |
| How many names per knight cross a call boundary | | |
| Where a knight's row is first set | | |
| What a signature says about the fields a body reads | | |
| Where a new per-knight fact is declared | | |
| What a new per-knight fact costs in signatures | | |
| What prevents `row = 0` | | |

## 3. Prediction (written before measuring)

### 3.1 The record cost

R-CAPTURED, in my own words:

- What is marked:
- When the mark is set:
- Who reads it:
- Why the output does not change:

My committed numbers for the record:

| Metric | My number | Confidence | Reason |
|---|---|---|---|
| Signatures that change | | | |
| Call sites that change | | | |
| Functions whose body changes | | | |
| Lines added | | | |
| Golden traces that differ (of 20) | | | |

- The reference that makes the assignment possible:
- What I expect `Knight playerKnight = {};` to do to the new field:

### 3.2 The separate-variable cost

Counting rules I used (in one line each):

- Edit site:
- Signature change:
- Call site:
- Caller:
- New global:

My committed numbers:

| Metric | Flags in `playGame` | Flags in `main` | Confidence |
|---|---|---|---|
| Edit sites | | | |
| Signature changes | | | |
| Call sites changed | | | |
| Callers of `playTurn` changed | | | |
| Callers of `playGame` changed | | | |
| New globals | | | |
| Lines added | | | |
| Golden traces broken (of 20) | | | |

### 3.3 Which placement is the fair comparison

- Scope that owns the fact in the record version, and the exact variable:
- Scope that owns the fact in the flag version, and why:
- If the end of `playGame` printed which knight was captured, which placement still compiles:
- The compare that makes `main` the honest owner:

## 4. Run — both baselines

- Records build command:
- Functions build command:
- Exit statuses:
- Diff target: `../../../common/traces/seed-0001.txt`

Record `empty` or `diff` in each result cell.

| Check | Command | Result |
|---|---|---|
| Records vs golden | | |
| Functions vs golden | | |
| Records vs functions | | |

```text
<diff output, or "no differences">
```

Seeds that matched, of 20:

If any diff is not empty, stop here and fill §10.

## 5. Add `captured` to the record

- Scratch copy:
- The field declaration line:
- The initialization lines:
- The assignment line:
- I did not edit the tracked record baseline: (yes / no)

```text
<diff of the scratch copy against the tracked baseline>
```

Output of `grep -n 'captured' <scratch copy>`:

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
| The default was already correct | | |
| No new namespace-scope data | | |

- What `= {}` does to a field added after the line was written:
- What the trace proves, and what it does not:
- `sizeof(Knight)` before / after:
- Mutable namespace-scope names, before / after (`nm`):

### 5.2 Prediction check

- Predicted signatures / observed:
- Predicted lines / observed:
- Closest prediction, and why:
- Furthest prediction, and what I missed:

## 6. The same fact with separate variables

- Scratch copy:
- Placement I applied first:
- Why that placement is the honest owner:
- I did not edit the tracked functions baseline: (yes / no)

### 6.1 Placement

| Where the flags could live | Owner of the fact | Signatures forced to change | Chosen? |
|---|---|---|---|
| local in `playTurn` | | | |
| local in `playGame`, passed to `playTurn` | | | |
| local in `main`, passed through `playGame` | | | |
| global at namespace scope | | | |

### 6.2 Edit sites, listed

| Site | Line | What changes | Signature, call site or body? |
|---|---|---|---|
| | | | |

- Total edit sites:
- Signature changes:
- Call sites changed:
- Lines added / removed:

### 6.3 Callers

| Function | Definition line | Call sites | Callers | Callers that change |
|---|---|---|---|---|
| `playTurn` | | | | |
| `playGame` | | | | |

- Why the flag's owner decides whether `main` changes:
- What the extra signature buys, in compiler terms:
- What the extra signature costs, in reader terms:

### 6.4 The conditional

- What `playTurn` holds about the other knight, and in what form:
- Why the flag version needs an `if` / `else`:
- Why the record version needs one assignment:
- The fact `playTurn` already knew and had to repeat:

### 6.5 Cost table

| Metric | Record | Flags in `main` | Flags in `playGame` | What it measures |
|---|---|---|---|---|
| Edit sites | | | | typing |
| Signature changes | | | | coupling made visible |
| Call sites changed | | | | call-graph reach |
| Callers changed | | | | call-graph reach |
| New globals | | | | hidden state |
| Lines added / removed | | | | blast radius |
| New language concepts | | | | learning cost |
| Golden traces broken (of 20) | | | | contract change |
| Golden traces reproduced unmodified (of 20) | | | | contract |
| Functions that can read the fact | | | | precision of the parameter list |

### 6.6 Run and answers

```bash
<commands>
```

| Check | Result |
|---|---|
| Modified flags version vs golden (20 seeds) | |
| Modified narrow version vs golden (20 seeds) | |
| First divergence, if any | |

- Why the narrow placement compiles: the exact condition it relies on:
- What happens to the narrow placement if `playGame` prints the captured knight:
- What the record version does in that case:
- In `construction-03`, the smallest edit that makes the flags belong to the knights:
- Which row the narrow placement beats the record on, and why it is not the same change:

## 7. Explain

One sentence, without the word *struct* and without *better*, *worse*, *clean*, *cleaner*, *nice*,
*elegant*, *ugly* or *modular*:

>

One paragraph:

- Which names the new field had to reach in each version:
- What the shorter parameter list stopped saying:
- One function where I lost information:
- One function where I lost nothing:
- What the record localizes:
- What the record does not localize:
- What a reader must hold in working memory to know which fields a function uses, in each file:
- What a golden trace can and cannot prove about this change:
- Which later lesson fixes representation, invariants and access:

## 8. Acceptance check

- `captured` added to the record: (yes / no)
- Initialized for both knights: (yes / no)
- Signatures that changed:
- Call sites that changed:
- The two call sites that did **not** change, and why:
- What the record localizes, in one line:
- What the record does not localize, in one line:
- Separate-variable placement that is the honest comparison:
- Edit sites, record / flags in `main`:
- Signature changes, record / flags in `main`:
- Golden traces reproduced after the record change, of 20:
- The explanation omits *struct* and the eight verdict words: (yes / no)
- I did not edit the tracked baselines: (yes / no)

If I changed a tracked baseline, what and why:

## 9. Reflection and next step

- What I can now explain without notes:
- What I cannot explain yet:
- One thing I would read again:
- The number I will carry to `construction-05`:
- The row where the record wins, and the row where it loses:
- The change that would make the record version expensive, and the change that would make it cheap:
- Next step:

## 10. If a baseline is missing or unfaithful

Fill this only if a file was absent, a build failed, or a diff was not empty.

- Missing file, build error, or diff:
- Exact command output:
- What I did instead of inventing a count:
- When I will retry:

## 11. Self-check

- [ ] I inventoried which function reads which field, with line numbers.
- [ ] I wrote the record prediction and both flag predictions before measuring.
- [ ] I built both baselines and got three empty diffs, plus 20 matching seeds.
- [ ] I added `captured` to a scratch copy and kept all 20 traces passing.
- [ ] I showed that `= {}` covers the new field without explicit initializers.
- [ ] I showed that a wrong default passes the traces, because nothing reads it.
- [ ] I applied the same fact to the functions baseline with separate flags.
- [ ] I counted both flag placements and named the condition the narrow one relies on.
- [ ] I separated the edit cost from the precision the parameter list lost.
- [ ] My one-sentence explanation avoids *struct* and the eight verdict words.
- [ ] I did not edit the tracked baselines.

No score. If a box is unchecked, write the next smallest action in §9.
