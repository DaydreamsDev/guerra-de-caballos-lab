# common-02 — Evidence: deterministic harness
> [English](EVIDENCE.md) | [Español](EVIDENCE-es.md)

> This is your record. Write it for your future self.
> Record what you observed, not what you expected the lesson to want.

## 1. Setup

- Date:
- Seed 7:
- Seed 8:
- Player name:
- Maximum move count:
- Reference path:
- Working directory:
- Command for seed 7:
- Command for seed 8:

## 2. Prediction — write before running

- Which three inputs can change the transcript?
- Which parts of the transcript depend on the seed?
- Will seed 7 and seed 8 produce the same game? Which line must differ first?
- What would break if the harness read the clock, an environment variable or an unordered
  collection?
- One falsifiable sentence:

>

## 3. Run

- Exit status seed 7:
- Exit status seed 8:
- Transcript seed 7 path:
- Transcript seed 8 path:
- Anything unexpected:

## 4. Compare the two seeds

### 4.1 Byte comparison

- `cmp` result (IDENTICAL / DIFFERENT):
- `cmp` exit status:

### 4.2 Diff comparison

- First differing line:
- Fields that changed:
- Did the rolls, starts, positions, moves and result also change, or only the seed line?

### 4.3 Turn counts

- `turn=` count for seed 7:
- `turn=` count for seed 8:

### 4.4 What this proves

- What the difference proves:
- What it does not prove:

## 5. Trace the seed through the harness

- LCG constants:
- Mask:
- Line where the seed initializes the state:
- First three random calls, in order:
- One place where the state is updated:
- One place where the generated value reaches the transcript:
- What would change first if the mask were removed:
- What would move if the call order were swapped:

## 6. Prediction vs observation

- What matched:
- What differed, with the exact lines:
- What the gap tells me about my mental model:

## 7. Explanation — why a fixed seed is required

Write one paragraph, without syntax words. Cover:

- what a comparison needs before it can attribute a difference to a cause;
- what you could no longer claim if the seed changed between the two sides;
- why “it looked random” does not excuse an unreproducible run;
- the difference between removing randomness and making it repeatable.

>

## 8. Self-check

- [ ] I read the harness and named its three inputs.
- [ ] I located the LCG constants and the mask.
- [ ] I traced the first three random calls.
- [ ] I wrote the prediction before running.
- [ ] I ran seed 7 and seed 8 once each.
- [ ] `cmp` reports the two transcripts as different.
- [ ] I recorded the first differing line and the fields that changed.
- [ ] I recorded the `turn=` count for both seeds.
- [ ] I named one hidden input that would break the harness if it reached the output.
- [ ] I can explain why a fixed seed is a control, not a correctness proof.
- [ ] I know my next step.

## 9. Next step

- What I can explain now:
- What I cannot explain yet:
- Next lesson or repeat:

## 10. Optional note

- Anything else:

---

> Reminder: this file is evidence for you.
