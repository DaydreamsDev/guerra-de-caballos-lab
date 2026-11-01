# construction-03 — Evidence

> [English](EVIDENCE.md) | [Español](EVIDENCE-es.md)

> Learner-owned record. Write in English.

This record compares two scopes for the same game. Keep both columns. A number for one file alone
cannot answer the question, and a tie is a result, not a missing difference.

## 1. Run setup

- Date:
- Seed: `1`
- Player name: `Learner`
- Repository root:
- Compiler and version (`g++ --version` or `clang++ --version`):
- Functions baseline read on (date):
- Structured baseline read on (date):
- Construction-02 record at hand? (yes / no)
- Edit sites and incoming edges I counted in construction-02:

## 2. Scope inventory (before the run)

### 2.1 Mutable namespace-scope names

Fill this from the object file and the source, not from memory. One row per name.

| Name | Line | construction-02 scope | construction-03 home | How it arrives there |
|---|---|---|---|---|
| | | | | |

Counts:

- Mutable namespace-scope names, construction-02:
- Mutable namespace-scope names, construction-03:
- Same names before and after:
- Names that became locals of `main`:
- Names that became locals of another function (name the function):
- Names that became value parameters:
- Names that became reference parameters:
- Names that disappeared:

### 2.2 Signatures

One row per function that reads or writes game state.

| Function | Line | Parameters | Returns | Reads a global? |
|---|---|---|---|---|
| | | | | |

Counts:

- Function definitions, construction-02:
- Function definitions, construction-03:
- `tryMove` parameters:
- `tryMove` parameters that are outputs:
- Functions that depend on `randomState` without naming it in their parameter list:
- Function whose definition line answers "what does this depend on?" completely:

### 2.3 Side by side

| Question | construction-02 | construction-03 |
|---|---|---|
| Where the move counter lives | | |
| How a function learns the active knight | | |
| How a function learns the visited board | | |
| What the call site states about dependencies | | |
| Where a new piece of turn state would be declared | | |
| What can be tested before the game starts | | |

## 3. Prediction (written before measuring)

### 3.1 One call

The call to predict: `tryMove(2, 1, 8, 8, 8, visitedCells, destinationRow, destinationColumn)`
with only cell (3,3) marked and the other knight at (8,8).

- `MoveOutcome` I expect:
- Destination row and column I expect:
- Why (the check order I assumed):
- Confidence, 0–100 %:

### 3.2 R-SKIP cost

R-SKIP, in my own words:

- When the turn is skipped:
- What the turn still records:
- Where the guard goes relative to the header:
- Where the started-turn counter must live, and why not one level up or down:

My committed numbers:

| Metric | construction-02 | construction-03 | Confidence | Reason |
|---|---|---|---|---|
| Edit sites | | | | |
| Signature changes | | | | |
| Call sites changed | | | | |
| Callers of `playTurn` changed | | | | |
| Callers of `playGame` changed | | | | |
| New globals | | | | |
| New control-flow edges | | | | |
| Files touched | | | | |
| Golden traces broken (of 20) | | | | |

The scope hypothesis, in one sentence (before measuring):

>

What I expect to tie:

What I expect to differ:

## 4. Run — both baselines

- Functions build command:
- Structured build command:
- Exit statuses:
- Diff target: `../../../common/traces/seed-0001.txt`

Record `empty` or `diff` in each result cell.

| Check | Command | Result |
|---|---|---|
| Functions vs golden | | |
| Structured vs golden | | |
| Functions vs structured | | |

```text
<diff output, or "no differences">
```

If any diff is not empty, stop here and fill §10.

## 5. The move-decision probe

- Scratch directory:
- Probe source (copy of the functions baseline):
- The one edit to the copy (rename `main`):
- I did not edit the tracked baseline: (yes / no)

Command:

```bash
<compile and run>
```

Output:

```text
<output>
```

- Predicted outcome (from §3.1):
- Observed outcome:
- Prediction correct? (yes / no). If not, which check did I miss:

The four cases, with the board holding only (3,3):

| Active | Move | Other | Expected | Observed |
|---|---|---|---|---|
| 2,1 | 8 | 8,8 | visited | |
| 2,1 | 1 | 8,8 | legal | |
| 2,1 | 8 | 3,3 | capture | |
| 1,1 | 2 | 8,8 | outside | |

What the probe proves:

- Game state initialized by the probe (list every value it sets):
- Functions the probe never calls:
- Why the call is possible here and not in `construction-02`:

What the probe does not prove:

- What the signature still does not state about `visitedCells`:
- What the `outside` row shows about the out-parameters:
- If I called `playTurn` instead, what I would have to set first:

## 6. Apply R-SKIP on a scratch copy

- Scratch copy:
- Counter owner (function):
- Why that scope:
- I did not edit the tracked baseline: (yes / no)

### 6.1 Placement

| Where the counter could live | Survives a call? | Visible without a parameter? | Chosen? |
|---|---|---|---|
| local in `playTurn` | | | |
| `static` in `playTurn` | | | |
| local in `playGame`, by reference | | | |
| local in `main`, by reference | | | |

### 6.2 Edit sites, listed

| Site | Line | What changes | Signature or call site? |
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

- Why the counter's owner decides whether `main` changes:
- The compiler check the extra signature buys:

### 6.4 Cost table

| Metric | construction-02 | construction-03 | Difference | What it measures |
|---|---|---|---|---|
| Edit sites | | | | typing |
| Signature changes | | | | coupling made visible |
| Call sites changed | | | | call-graph reach |
| New globals | | | | hidden state |
| New control-flow edges | | | | added flow |
| Files touched | | | | coupling |
| New language concepts | | | | learning cost |
| Golden traces broken by R-SKIP (of 20) | | | | contract change |
| Golden traces reproduced unmodified (seeds checked) | | | | contract |
| Modified transcripts that agree with each other | | | | fidelity |

### 6.5 Run and compare

```bash
<commands>
```

| Check | Result |
|---|---|
| Modified functions vs golden | |
| Modified functions vs modified structured (if kept) | |
| Seeds that differ from golden (of 20) | |

- First divergence from golden (line and text):
- Why the two modified versions must agree:
- First divergence I expected, and whether it matched:

### 6.6 Answers

- How many callers does `playTurn` have? How many change?
- How many callers does `playGame` have? How many change?
- Which count ties with `construction-02`, and which does not?
- What did the extra signature buy, in compiler terms?
- Where could new turn state leak in each file, and how would a reader notice?
- Closest prediction, and why:
- Furthest prediction, and what I missed:

## 7. Explain

One sentence, without *better*, *worse*, *clean*, *cleaner*, *nice*, *elegant*, *ugly* or
*modular*:

>

One paragraph:

- Which names `tryMove` can read that its caller did not pass:
- Which names `playTurn` can read that its caller did not pass:
- What that difference does to the probe:
- What a reader must hold in working memory to know a function's inputs in each file:
- Where new turn state must be declared, and what forces the threading:
- What the signature still fails to say, and which later lesson fixes it:

## 8. Acceptance check

- Requirement restated without adding a random draw:
- Counting rule used:
- The move decision I exercised:
- The local values the probe used:
- Global game state the probe needed: (none / list it)
- Edit sites counted (construction-02 / construction-03):
- Callers of `playTurn` counted:
- Which count ties:
- What the parameter list made visible:
- What it charged for it:
- The explanation avoids the eight forbidden words: (yes / no)
- I did not edit the tracked baselines: (yes / no)

If I changed a tracked baseline, what and why:

## 9. Reflection and next step

- What I can now explain without notes:
- What I cannot explain yet:
- One thing I would read again:
- The number I will carry to `construction-04`:
- The result that makes `tryMove` easy to test, and the one that does not:
- Next step:

## 10. If a baseline is missing or unfaithful

Fill this only if a file was absent, a build failed, or a diff was not empty.

- Missing file, build error, or diff:
- Exact command output:
- What I did instead of inventing a count:
- When I will retry:

## 11. Self-check

- [ ] I inventoried both scopes from the object files and the source.
- [ ] I wrote the probe prediction and the cost prediction before measuring.
- [ ] I built both baselines and got three empty diffs.
- [ ] The probe calls the tracked `tryMove` and initializes no game state.
- [ ] I recorded the probe command and its exact output.
- [ ] I applied R-SKIP with the counter at the narrowest owning scope.
- [ ] I counted callers with `grep`, not by assumption.
- [ ] I separated the edit cost from what the signature makes visible.
- [ ] My one-sentence explanation avoids the eight forbidden words.
- [ ] I did not edit the tracked baselines.

No score. If a box is unchecked, write the next smallest action in §9.
