# construction-02 — Evidence

> [English](EVIDENCE.md) | [Español](EVIDENCE-es.md)

> Learner-owned record. Write in English.

This record compares two shapes of the same game. Keep both columns. A number for one shape alone
cannot answer the question, and a tie is a result, not a missing difference.

## 1. Run setup

- Date:
- Seed: `1`
- Player name: `Learner`
- Repository root:
- Compiler and version (`g++ --version` or `clang++ --version`):
- Structured baseline read on (date):
- Non-structured baseline read on (date):
- Construction-01 record at hand? (yes / no)
- Rewired jumps I counted in construction-01:

## 2. Control-flow inventory (before the run)

### 2.1 Non-structured baseline

Fill this from the file, not from memory. One row per label.

| Label | Line | Incoming edges (line and kind) | Turn entry? |
|---|---|---|---|
| | | | |

Counts:

- Labels:
- Jumps:
- Turn entry:
- Turn-start region (line range):
- Incoming edges to the turn entry, listed:
- `playerIsActive` assignments (lines):
- `successfulMoveCount` increments (lines):

### 2.2 Structured baseline

One row per loop and per transfer.

| Construct | Line | Enclosing region | Begins a turn? |
|---|---|---|---|
| | | | |

Counts:

- Loops:
- `continue` sites:
- `break` sites:
- Turn-start region (line range):
- Incoming edges to the turn entry, listed:
- `playerIsActive` assignments (lines):
- `successfulMoveCount` increments (lines):

### 2.3 Side by side

| Question | Non-structured | Structured |
|---|---|---|
| The turn boundary is stated as | | |
| Turn-start region | | |
| Incoming edges to the turn entry | | |
| How the next turn begins | | |
| Labels available to the change | | |
| Where the active side is flipped | | |

## 3. Prediction (written before measuring)

R-SKIP, in my own words:

- When the turn is skipped:
- What the turn still records:
- What number a skipped turn prints in `turn=`, and whether that is acceptable:
- What the skipped turn does not count:
- Why this needs no new random draw:
- Where the guard goes relative to the header:

My committed numbers:

| Metric | Non-structured | Structured | Confidence | Reason |
|---|---|---|---|---|
| Edit sites | | | | |
| Incoming edges to the turn entry | | | | |
| New control-flow edges | | | | |
| New labels | | | | |
| Files touched | | | | |
| New language concepts | | | | |
| First golden line expected to differ | | | | |

The localization hypothesis, in one sentence (before measuring):

>

What I expect to tie:

What I expect to differ:

## 4. Run — both baselines

- Structured build command:
- Non-structured build command:
- Exit statuses:
- Diff target: `../../../common/traces/seed-0001.txt`

Record `empty` or `diff` in each result cell.

| Check | Command | Result |
|---|---|---|
| Structured vs golden | | |
| Non-structured vs golden | | |
| Structured vs non-structured | | |

```text
<diff output, or "no differences">
```

If any diff is not empty, stop here and fill §10.

## 5. Apply R-SKIP on scratch copies

- Scratch directory:
- Structured copy:
- Non-structured copy:
- I did not edit the tracked baselines: (yes / no)

Implementation checklist, applied to both:

- [ ] The started-turn counter is declared and starts at 0.
- [ ] It is incremented once per turn, after the terminal check and before the order draw.
- [ ] `turn=` prints the started-turn counter, not `successfulMoveCount + 1`.
- [ ] The guard runs after the `turn=`/`order=` header and before any `try=`.
- [ ] A skip flips the active side.
- [ ] A skip records no move and does not increment `successfulMoveCount`.
- [ ] The 200-move cap and `moves=` still use `successfulMoveCount`.
- [ ] A skipped turn still draws and prints its order.
- [ ] The skip adds no random draw.
- [ ] No new label was added, or the reason for the new label is recorded below.

Build and run commands:

```bash
<commands>
```

| Check | Command | Result |
|---|---|---|
| Modified structured vs modified non-structured | | |
| Modified structured vs golden | | |
| Modified non-structured vs golden | | |

- First divergence from golden (line and text):
- Why the two modified versions must agree:
- If I added a label, why:

## 6. Compare — where the change lands

### 6.1 Cost table

| Metric | Non-structured | Structured | Difference | What it measures |
|---|---|---|---|---|
| Edit sites | | | | typing |
| Files touched | | | | coupling |
| New control-flow edges | | | | added flow |
| New labels | | | | named positions |
| Incoming edges to the turn entry (before) | | | | audit surface |
| Active-side assignment sites (before / after) | | | | state coupling |
| New language concepts | | | | learning cost |
| Golden traces broken by R-SKIP (of 20) | | | | contract change |
| Golden traces reproduced unmodified (seeds checked) | | | | contract |
| Modified transcripts that agree with each other | | | | fidelity |

### 6.2 Edit sites, listed

Non-structured:

| Site | Line | What changes | Where it sits (before / inside / after the region) |
|---|---|---|---|
| | | | |

Structured:

| Site | Line | What changes | Where it sits (before / inside / after the region) |
|---|---|---|---|
| | | | |

- Total edit sites, non-structured:
- Total edit sites, structured:
- Lines added / removed, non-structured:
- Lines added / removed, structured:

### 6.3 Incoming edges before the change, listed

Non-structured:

| Edge | Line | Kind (jump / fall-through) | Passes the guard? |
|---|---|---|---|
| | | | |

Structured:

| Edge | Line | Kind (loop edge / jump / fall-through) | Passes the guard? |
|---|---|---|---|
| | | | |

- Total incoming edges, non-structured:
- Total incoming edges, structured:

### 6.4 Answers

- Did the structured version need fewer edit sites? If not, what did it localize?
- Which count moved between the two shapes, and which stayed equal?
- Which incoming edge in the non-structured file was easiest to miss?
- If a fifth `goto gameLoopStart` were added, would the guard cover it? What would I read to know?
- If a second loop that starts a turn were added to the structured file, would the guard cover it?
  What would I read to know?
- What does the structured file make visible at the point of change that the non-structured file
  leaves implicit?
- Closest prediction, and why:
- Furthest prediction, and what I missed:

## 7. Explain

One sentence, without *bad*, *clean*, *readable*, *elegant*, *ugly*, *spaghetti*, *unreadable* or
*goto*:

>

One paragraph:

- Where a turn starts in each file, as a control-flow fact:
- What the skip edge targets in each file:
- Why the structured turn entry has the incoming-edge count it has:
- Why equal edit counts can still differ in verification cost, or what the extra site bought:
- What a reader must hold in working memory in each case:
- What would make a future change incomplete in each file, and how I would notice:

## 8. Acceptance check

- Requirement restated without adding a random draw:
- Counting rule used:
- Edit sites counted (non-structured / structured):
- Incoming edges counted (non-structured / structured):
- Which count ties:
- What the structured version localizes:
- What it does not localize:
- The explanation avoids the eight forbidden words: (yes / no)
- I did not edit the tracked baselines: (yes / no)

If I changed a tracked baseline, what and why:

## 9. Reflection and next step

- What I can now explain without notes:
- What I cannot explain yet:
- One thing I would read again:
- The two numbers I will carry to `construction-03`:
- Next step:

## 10. If a baseline is missing or unfaithful

Fill this only if a file was absent, a build failed, or a diff was not empty.

- Missing file, build error, or diff:
- Exact command output:
- What I did instead of inventing a count:
- When I will retry:

## 11. Self-check

- [ ] I inventoried both shapes from the files, with line numbers.
- [ ] I wrote the predicted counts before counting.
- [ ] I built both baselines and got three empty diffs.
- [ ] I applied the same R-SKIP specification to both scratch copies.
- [ ] The two modified versions agreed with each other.
- [ ] I listed every edit site and every incoming edge.
- [ ] I separated edit cost from verification cost.
- [ ] My one-sentence explanation avoids the eight forbidden words.
- [ ] My paragraph names the control-flow cause, not the style.
- [ ] I did not edit the tracked baselines.

No score. If a box is unchecked, write the next smallest action in §9.
