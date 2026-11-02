# Read the rules, run the reference baseline

> [English](lesson.md) | [Español](lesson-es.md)

**Concept:** specification (especificación) · **Time:** about 50 minutes · **Status:** drafted
**Reading:** ~7 minutes

## Why this lesson exists

A specification is the part of the work that survives a change of implementation. This lesson does
not ask you to write code. It asks you to read the contract, commit to a prediction, and let a
transcript show you which part of your mental model was actually a guess.

This is the first lesson of Track A. Every later lesson compares an implementation against the
contract you establish here.

## What you will be able to do

By the end of this lesson you can:

- explain specification and the game invariant;
- predict the next move of a seeded game and compare it with the trace.

## The concept: a specification is a constraint, not a description

The rules do not tell you what happens in seed 1. They tell you what is allowed to happen for any
seed. The seed chooses the values; the specification constrains the shape.

For example, the rules force the move order of a turn to be a rotation of `1..8`. They do not force
the rotation to start at 3. `start` is drawn, so its value belongs to the seed.

That distinction is the whole lesson. When two implementations disagree, you will ask: did the rule
change, or only the value? A specification you can separate from its output is a specification you
can test.

## Before you start

- `common-01` — the game as an invariant.
- `common-02` — the deterministic harness.

You do not need C++ for this lesson. You need Python 3 and a text editor.

### What you need

- The rules: [`common/spec/game-rules.md`](../../../common/spec/game-rules.md).
- The transcript format:
  [`common/spec/transcript-format.md`](../../../common/spec/transcript-format.md).
- The reference baseline: [`common/harness/reference.py`](../../../common/harness/reference.py).
- This learner template: [`EVIDENCE.md`](EVIDENCE.md).

Check that the rules and the baseline are present before you begin:

```bash
test -f common/spec/game-rules.md && test -f common/harness/reference.py
```

If `reference.py` is missing, the baseline is not present in this checkout. Do not invent a
transcript, and do not edit the harness to make it match your prediction. Record the missing
artifact in `EVIDENCE.md` §9 and return when the file is available. A lesson without its artifact
has no evidence yet.

## Step 1 — Read the rules (~10 min)

Read [`game-rules.md`](../../../common/spec/game-rules.md) once, from top to bottom. Then answer
these questions in your own words, without copying sentences from the document:

1. When is a move legal, and what is the one exception to the visited rule?
2. Who starts, and what happens if the rolls are equal?
3. After the first successful move, exactly which cells are visited?
4. A knight may enter its own starting cell. Why is that consistent with the visited rule?
5. What ends the game besides a capture?
6. Which section decides the winner when the active knight has no legal move?

Write the answers in `EVIDENCE.md` §2. If an answer needs a guess, mark it as a guess.

Now read the sections **Header**, **Turn**, **Attempts**, **Successful move**, **Trap** and
**Result** of [`transcript-format.md`](../../../common/spec/transcript-format.md).

The **Example** section uses seed 17, not seed 1. You may read it now; it will not reveal your
target. The example is illustrative; the rules are normative.

## Step 2 — Predict before running (~10 min)

Write the prediction in `EVIDENCE.md` §3 **before you execute anything**. Once you have seen the
transcript, the prediction is no longer evidence.

Predict the first move of seed 1 for the player name `Learner`. A complete prediction has two
layers.

### 2.1 What the rules force

Fill this table from the rules, not from memory:

| Question | Rule that decides it | My answer |
|---|---|---|
| Which knight moves first? | §4 | higher roll starts |
| Where does that knight stand? | §3 | one of the two starting cells |
| What is the shape of the turn order? | §6 | a rotation of `1..8` |
| Which move is tried first? | §6 | the first number in that rotation |
| When is the first attempt legal? | §7, §8 | inside the board and not visited, or a capture |
| What if no attempt is legal? | §10 | the active knight is trapped and loses |

The table is the part you can defend without running anything. "A rotation of `1..8`" is a forced
answer. "Order starts at 3" is not: it is a value chosen by the seed.

### 2.2 What the seed decides

The starting cells and the rolls are not in the rules. Commit to a guess anyway. A prediction with
a guessed value is still a prediction, and the comparison will show you which fields were forced
and which were guessed.

Write one complete expected line:

```text
move=1 <player|machine> <from-row>,<from-column>-><to-row>,<to-column>
```

Mark every field `[forced]` or `[guess]`, and add a confidence from 0 to 100 %. Also predict:

- which knight starts;
- the eight-number turn order;
- the first `try=` line;
- the result of that first attempt (`legal`, `illegal=outside`, `illegal=visited` or `capture`).

A wrong guess is data. A prediction written after the run is not.

## Step 3 — Run (~5 min)

From the repository root, run exactly:

```bash
python3 common/harness/reference.py --seed 1 --name Learner
```

Capture the complete output. Do not truncate the attempt lines; they are where the rules become
observable.

If the command fails, record the exact error in `EVIDENCE.md` §9. Do not patch the harness, and do
not replace its output with the example from the format document.

## Step 4 — Compare (~5 min)

Find `move=1` in the transcript. Put your prediction and the observation side by side and compare
field by field:

- Did the predicted knight start?
- Was the turn order the rotation you expected?
- Which attempts appeared before the successful move?
- Did each attempt have the result you predicted?
- Did the `move=` line match in move number, knight, origin and destination?

Write the exact differing lines, not "it was different". If nothing differs, say which fields you
predicted for the right reason and which you guessed by luck.

Only now read the **Example** in
[`transcript-format.md`](../../../common/spec/transcript-format.md). It is illustrative, not
normative, and it is not guaranteed to agree with the move table. If it disagrees with the rules,
the rules win (game-rules §14). Record the disagreement; noticing it is part of reading a
specification critically.

## Step 5 — Explain the gap (~10 min)

Write one paragraph in `EVIDENCE.md` §6. Answer:

- Which field did I get wrong?
- Was it a rule I misread, or a value the seed chose?
- What did I assume that the specification never said?

Do not use programming-language words in the explanation. "The rotation began at a different number"
is an explanation. "`random_integer` returned another value" names a mechanism
without explaining the model.

If your prediction matched, explain why each field had to match. A correct guess without a reason
is luck, and luck does not transfer to the next seed.

## Step 6 — Acceptance check: predict the next move (~5 min)

The acceptance criterion for this lesson is: **predict the next move of a seeded game.**

1. Choose a turn in your transcript that is followed by another turn. Turn 2 is a good default.
2. Copy its `turn=` line. Cover every line after it.
3. Using only the header, the previous moves, the visited cells and rules §6–§8, write every
   `try=` line and the `move=` line you expect.
4. Reveal the covered lines and compare.

You are not guessing the random draw here. The turn order is already printed, the board state is
known, and the rules decide the rest. If your line differs, explain the difference before you move
on; that explanation is the acceptance evidence.

## Step 7 — Record (~5 min)

Complete `EVIDENCE.md` §1–§8. Keep the command, the prediction, the exact diff and the explanation.
The file is your record.

If the baseline was missing, fill §9 instead and stop. There is nothing to compare yet.

## Self-check (binary)

- [ ] I read the rules and the transcript format sections, not only the example.
- [ ] I wrote my prediction before running the baseline.
- [ ] I ran `python3 common/harness/reference.py --seed 1 --name Learner` and captured the full
      output.
- [ ] I named the exact fields that differed, or explained why none did.
- [ ] I predicted one later move from the printed state and the rules, then checked it.
- [ ] I completed `EVIDENCE.md`, including the next step.

These are acceptance criteria. If a box is unchecked, write the
next smallest action in `EVIDENCE.md` §8.

## Misconceptions this lesson refutes

| Misconception | Why it fails | What to do instead |
|---|---|---|
| "The example is the specification." | An example shows one run; the rules define every run. | Test the example against the rules and say which wins. |
| "A wrong prediction means I failed." | The gap locates the assumption that needs repair. | Name the wrong assumption; keep the evidence. |
| "The order is always `1..8`." | It is a rotation that starts at a drawn value. | Separate the forced shape from the seed-dependent start. |
| "Starting cells are visited." | They are occupied but unvisited until entered. | Re-read §3 and §8 before predicting legality. |
| "If the output disagrees, the output is wrong." | The specification is the contract; the implementation is corrected. | Record the discrepancy and let the rules decide. |

## Next step

`construction-01` — the non-structured baseline. You now have the contract that every later
variant must satisfy. See the [lesson catalog](../../../docs/guide/lesson-catalog.md) for the full
path.

If `reference.py` was missing, do not start `construction-01`. Restore the baseline first; without
it, "same behavior" cannot be checked.
