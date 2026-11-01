# construction-01 — Evidence

> [English](EVIDENCE.md) | [Español](EVIDENCE-es.md)

> Learner-owned record. Write in English.

This is a template. Replace the prompts with what actually happened. If you counted
before you predicted, say so; the record is more useful when it is honest.

## 1. Run setup

- Date:
- Seed: `1`
- Player name: `Learner`
- Repository root:
- Compiler and version (`g++ --version` or `clang++ --version`):
- Baseline file read on (date):
- Rules read on (date):

## 2. Jump inventory (before the run)

Fill this from the file, not from memory. One row per label.

| Label | Line | Reached from (lines) | Turn entry? |
|---|---|---|---|
| | | | |

Counts:

- Labels:
- Jumps:
- Labels reached from more than one jump:
- Labels reached from both a player path and a machine path:
- Turn entries:

Places where the move order is drawn:

- Line:
- Labels that can arrive there:

## 3. Prediction (written before measuring)

R-SKIP, in my own words:

- When the turn is skipped:
- What the turn still records:
- What number a skipped turn prints in `turn=`, and whether that is acceptable:
- What the skipped turn does not count:
- Why this needs no new random draw:

My committed numbers:

| Question | My number | Confidence | Reason |
|---|---|---|---|
| Labels that begin a turn | | | |
| Jumps to rewire | | | |
| New labels | | | |
| New jumps | | | |
| Places that decide who plays next | | | |

The cheapest single insertion point, and why it is not sufficient:

## 4. Run

- Build command (or `./run.sh`):
- Run command:
- Exit status:
- Diff command and target (`../../../common/traces/seed-0001.txt`):
- Empty diff? (yes / no):

```text
<diff output, or "no differences">
```

If the diff was not empty, stop here and fill §9.

## 5. Compare — the cost table

| Item | Predicted | Counted | Difference |
|---|---|---|---|
| Labels that begin a turn | | | |
| Jumps rewired (arrivals at a turn entry) | | | |
| New labels required | | | |
| New jumps required | | | |
| Places that decide who plays next | | | |
| Files touched | 1 | 1 | 0 |

Every rewired jump, listed:

| Jump line | Target | Entry carries the check? | What changes for this arrival |
|---|---|---|---|
| | | | |

- Closest prediction, and why:
- Furthest prediction, and what I missed:
- Was the cheapest single insertion point sufficient? If not, which behavior breaks:
- Would the transcript show that break? How:

## 6. Explain the cost

One sentence, without the words *bad*, *goto*, *spaghetti* or *unreadable*:

>

One paragraph:

- Why R-SKIP has no single home:
- What the reader must hold in working memory:
- What makes duplicated checks drift apart:
- What the reader must do to know a change is complete:

## 7. Acceptance check — skip turn

- Requirement restated without adding a random draw:
- Counting rule used:
- Number of rewired jumps:
- Each counted jump listed in §5:
- Cost explained without naming the keyword:

Did I implement R-SKIP? (yes / no / not required)

If yes, what changed in the transcript, and why it is a new game rather than a new design:

## 8. Reflection and next step

- What I can now explain without notes:
- What I cannot explain yet:
- One thing I would read again:
- The number I will carry to `construction-02`:
- Next step:

## 9. If the baseline is missing or unfaithful

Fill this only if the file was absent, the build failed, or the diff was not empty.

- Missing file, build error, or diff:
- Exact command output:
- What I did instead of inventing a jump count:
- When I will retry:

## 10. Self-check

- [ ] I inventoried labels and jumps from the file, with line numbers.
- [ ] I wrote the predicted count before counting.
- [ ] I built the baseline and got an empty diff against `seed-0001.txt`.
- [ ] I applied one stated counting rule to every jump.
- [ ] I listed each rewired jump by target and line.
- [ ] My one-sentence explanation avoids the four forbidden words.
- [ ] My paragraph names the structural cause, not the symptom.
- [ ] I recorded the number to compare in `construction-02`.

No score. If a box is unchecked, write the next smallest action in §8.
