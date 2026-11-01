# construction-00 — Evidence (evidencia)

> [English](EVIDENCE.md) | [Español](EVIDENCE-es.md)

> Learner-owned record. Write in English.

This is a template. Replace the prompts with what actually happened. If you did not
write a prediction before running, say so; the record is more useful when it is honest.

## 1. Run setup

- Date:
- Seed: `1`
- Player name: `Learner`
- Repository root:
- Python version (`python3 --version`):
- Rules read on (date):
- Transcript format read on (date):

## 2. Rules reading check (before the run)

Answer in your own words. Mark anything you had to guess.

- A move is legal when:
- The one exception to the visited rule is:
- The knight that starts is:
- A starting cell becomes visited when:
- A knight with no legal move:
- The turn order for a turn is built by:

## 3. Prediction (written before running)

### 3.1 What the rules force

| Field | Rule section | Predicted value or shape |
|---|---|---|
| Knight that starts | §4 | |
| Turn 1 order shape | §6 | |
| First attempted move | §6 | |
| Legality condition | §7, §8 | |
| Result if no attempt is legal | §10 | |

### 3.2 What the seed decides — my committed guess

- Who starts:
- Turn 1 `order`:
- First `try=` line:
- Expected result of that attempt:
- First `move=` line:
- Confidence (0–100 %):
- Why I chose this guess:

## 4. Run

- Command:
- Exit status:
- Transcript saved at:
- Full output:

```text

```

## 5. Compare

- Observed first `move=` line:
- Fields that matched:
- Fields that differed:
- Exact diff:

```diff
- predicted:
+ observed:
```

- Header fields that differed:
- Disagreement between the format example and this run:
- Which source wins, and why:

## 6. Explain the gap

- The field I got wrong:
- Was it a rule I misread or a value the seed chose?
- The assumption the specification never made:
- One paragraph, without programming-language words:

## 7. Acceptance check — predict the next move

- Turn I chose:
- State I used (positions and visited cells):
- My predicted `try=` lines:
- My predicted `move=` line:
- Observed lines:
- Match? What the difference taught me:

## 8. Reflection and next step

- What I can now explain without notes:
- What I cannot explain yet:
- One thing I would read again:
- Next step:

## 9. If the baseline is missing

Fill this only if the command could not run.

- Missing file or error:
- Command output:
- What I did instead of inventing a transcript:
- When I will retry:

## 10. Self-check

- [ ] My prediction was written before I ran anything.
- [ ] I captured the full transcript, not a summary.
- [ ] I named exact differing lines or explained why there were none.
- [ ] I predicted one later move from state and rules and checked it.
- [ ] I can explain the difference between a rule and a seed-dependent value.
- [ ] I recorded the next step.

No score. If a box is unchecked, write the next smallest action in §8.
