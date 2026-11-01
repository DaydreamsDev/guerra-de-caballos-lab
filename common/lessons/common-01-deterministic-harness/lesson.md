# common-01 — Deterministic harness (arnés determinista)

> [English](lesson.md) | [Español](lesson-es.md)

> **Status:** drafted · **Concept:** harness construction (construcción del arnés) ·
> **Prerequisites:** common-00 · **Estimated time:** 60 minutes
> **Reading:** ~8 minutes

## Why this lesson exists

A trace is evidence only if the program that produced it has no hidden inputs. The harness is small
enough to read: three inputs, one pseudo-random sequence and one transcript writer. If you can name
those parts, you can tell whether a difference came from the change under test or from something the
harness forgot to control.

`common-00` already proved the same-seed invariant. This lesson does not repeat that proof. It opens
the harness and answers a different question: **what makes the output depend on the seed and on
nothing else?**

## What you will be able to do

By the end of this lesson you can:

- name the three inputs of the reference harness;
- locate the 64-bit LCG constants and the mask that keeps the state in range;
- trace the order of the random calls that reach the transcript;
- show that seed 7 and seed 8 produce different games;
- name one hidden input that would break the harness if it reached the output;
- explain why a fixed seed is a control, not a correctness proof.

## The concept: harness construction (construcción del arnés)

A harness is a small program with three inputs and one deterministic sequence. Its job is to make
the output depend on those inputs and on nothing else. The concept is not “randomness is good”; it
is
“the path from seed to bytes is explicit and controlled.”

| Idea | Meaning in this lesson |
|---|---|
| **Input** | Everything the program reads: seed, player name, maximum move count. |
| **Control variable** | An input you hold fixed so it cannot explain a difference. |
| **LCG** | The 64-bit linear congruential generator that turns the seed into a sequence. |
| **Seed path** | The line where the seed initializes the generator state. |
| **Call order** | The order in which random values are drawn and written to the transcript. |
| **Attribution** | Knowing which input caused which observed difference. |

The seed does not remove randomness. It makes the randomness repeatable. A harness with a hidden
input — a clock, an environment variable, an unordered collection — breaks that property even if the
seed is fixed.

## Before you start

Read these documents in full:

- [`common/harness/README.md`](../../harness/README.md) — the command line and the determinism
  guarantees.
- [`common/spec/game-rules.md`](../../spec/game-rules.md) — especially §12, the exact call order of
  the random generator.
- [`common/spec/transcript-format.md`](../../spec/transcript-format.md) — what one transcript
  contains.
- [`common/traces/README.md`](../../traces/README.md) — the golden traces and how they are verified.

You also need:

- Python 3.8 or newer and a terminal;
- a scratch directory for the outputs (do not commit run output);
- the reference implementation: [`common/harness/reference.py`](../../harness/reference.py);
- the trace directory: [`common/traces/`](../../traces/).

The harness is the executable form of the rules. Its determinism is not an accident of the
implementation; it is a requirement of §12, which fixes the order of every random call.

## The 60-minute path

| Min | Step | What you do | Evidence |
|---|---|---|---|
| 0–15 | Read | Read `reference.py` ; list the three inputs, the LCG constants and the call order. |
Input and LCG notes |
| 15–25 | Predict | Predict seed 7 vs seed 8 and one hidden input that would break the harness. |
Prediction note |
| 25–35 | Run | Run seed 7 and seed 8 once each. | Two transcripts |
| 35–45 | Compare | Diff the two seeds; record the first difference and the turn counts. | Diff and
counts |
| 45–55 | Trace | Trace the seed, the LCG and the first three random calls. | Trace notes |
| 55–60 | Explain and record | Explain the control; fill `EVIDENCE.md`. | Portfolio entry |

One step can take longer; that is information about the
step, not about you.

## Step 1 — Read the harness (10 min)

Read [`common/harness/reference.py`](../../harness/reference.py) with one question in mind: **what
can change the output?**

Start at `build_transcript`. The function receives three values: `seed`, `playerName` and
`maximumMoveCount`. Everything it writes depends on those three values and on nothing else.

Trace the random calls in order:

1. `create_initial_positions` draws two cells and redraws the machine cell while it equals the
   player cell.
2. `create_initial_rolls` draws two rolls and redraws the machine roll while it equals the player
   roll.
3. On each turn, one `random_integer` call draws the starting move number.

That is the complete list. There is no clock read, no environment variable, no network call, and no
iteration over an unordered collection that could reach the output. The `visitedPositions` set is
used only for membership tests, never to build a line.

### The three inputs

| Input | Flag | Effect on the transcript |
|---|---|---|
| Seed | `--seed` | Selects the entire sequence: starts, rolls, move orders, game. |
| Player name | `--name` | Copied into the `player=` header line. |
| Maximum moves | `--max-moves` | Changes when the game becomes a draw; default `200`. |

Two of these are easy to forget. The player name is part of the bytes even though it does not touch
the random generator. The maximum move count is part of the control even though it is optional.

### A question to answer before moving on

If you change only the player name, does the game change? Predict the answer, then reason from the
code: the name is written after the random calls, and no random call reads it. The game should be
identical except for the `player=` line. Keep that prediction; you can test it later as an optional
extension.

## Step 2 — Predict (10 min)

Write the prediction **before** you run anything. You are predicting causes, not guessing random
digits.

Answer at least these:

1. Which three inputs can change the transcript?
2. Which parts of the transcript depend on the seed?
3. Will seed 7 and seed 8 produce the same game? Which is the first line that must differ?
4. If the harness read the clock, an environment variable or an unordered collection, where would
   the instability appear first?
5. Why does the player name change the bytes without changing the game?

Write one falsifiable sentence, for example:

> Seed 7 and seed 8 will differ because the seed selects the entire sequence of rolls, starts and
> move orders; the player name will change only the `player=` line.

A prediction is not a guess you are allowed to fix later. The gap between prediction and observation
is the learning signal; editing the prediction after the run destroys it.

## Step 3 — Run (10 min)

From the repository root, run one game with each seed. Do not run the same seed twice; that proof
belongs to `common-00`.

```bash
mkdir -p "$HOME/common-01-scratch"
common/harness/run.sh --seed 7 --name Learner > "$HOME/common-01-scratch/seed-7.txt"
common/harness/run.sh --seed 8 --name Learner > "$HOME/common-01-scratch/seed-8.txt"
```

Check both exit statuses:

```bash
common/harness/run.sh --seed 7 --name Learner > /dev/null && echo "seed 7 exit 0"
common/harness/run.sh --seed 8 --name Learner > /dev/null && echo "seed 8 exit 0"
```

Record the exact commands. A command you cannot reproduce is not evidence.

Seed 7 plays 10 successful moves and ends with `winner=player`. Seed 8 ends with `winner=machine`
after 26 successful moves. Those are facts about the seeds, not rules.

## Step 4 — Compare the two seeds (10 min)

Use three independent tools; agreement between them is stronger than one tool alone.

### 4.1 Byte comparison

```bash
cmp -s "$HOME/common-01-scratch/seed-7.txt" "$HOME/common-01-scratch/seed-8.txt" \
  && echo "IDENTICAL" || echo "DIFFERENT"
```

The expected result is `DIFFERENT`. That is not a failure; it is the proof that the seed selects the
game.

### 4.2 Diff comparison

```bash
diff -u "$HOME/common-01-scratch/seed-7.txt" "$HOME/common-01-scratch/seed-8.txt"
```

Record the first differing line and the fields that change. Check whether the rolls, starts,
positions, moves and result also change, or only the seed line.

### 4.3 Trace counts

Count the `turn=` lines in each transcript:

```bash
grep -c '^turn=' "$HOME/common-01-scratch/seed-7.txt"
grep -c '^turn=' "$HOME/common-01-scratch/seed-8.txt"
```

The counts are different because the games are different. Record both numbers.

### 4.4 What this proves

A difference between seed 7 and seed 8 proves that the seed is doing real work. It does **not**
prove
that either game is correct. Correctness is checked against the rules and the golden traces.

## Step 5 — Trace the seed through the harness (10 min)

Open [`common/harness/reference.py`](../../harness/reference.py) and locate the random generator.

Record:

- the four LCG constants and the mask;
- the line where the seed initializes the state;
- the order of the first three random calls;
- one place where the state is updated;
- one place where the generated value reaches the transcript.

Then answer: if the mask were removed, what would change first? If the call order were swapped,
which
line of the transcript would move? You do not need to edit the harness; reason from the source.

## Step 6 — Explain why a fixed seed is required (5 min)

Write one paragraph, without syntax words. Answer:

- What does a comparison need before it can attribute a difference to a cause?
- If the seed changed between the two sides of a comparison, what could you no longer claim?
- Why does “it looked random” not excuse an unreproducible run?
- What is the difference between removing randomness and making it repeatable?

A useful sentence to finish: *“A fixed seed is not a convenience; it is the control that lets a
difference mean something.”*

Do not name a programming language, a data type, or a syntax element. If you need one to explain the
idea, the idea is not yet separated from the implementation.

### The trap to avoid

The tempting wrong explanation is: *“The seed makes the output correct.”* It does not. A fixed seed
makes the output **repeatable**. Correctness is a separate question, answered by comparing the
transcript with the rules and the golden traces. A reproducible wrong answer is still wrong.

## Step 7 — Record (5 min)

Fill [`EVIDENCE.md`](EVIDENCE.md). It is your portfolio entry. Include:

- the prediction, written before the run;
- the commands and exit statuses for seed 7 and seed 8;
- the first differing line and the fields that changed;
- the `turn=` counts;
- the LCG constants, the seed line and the first three random calls;
- the explanation paragraph;
- the next step.

## Self-check (binary)

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
- [ ] I filled `EVIDENCE.md` and know my next step.

## Misconceptions this lesson refutes

| Misconception | Why it is wrong | What to do instead |
|---|---|---|
| “Deterministic means there is no randomness.” | The game is full of random draws; determinism
means the draws repeat. | Say “seeded” or “repeatable”, not “random-free”. |
| “The seed is enough; the name does not matter.” | The name is copied into the transcript, so it
changes the bytes. | Hold every input fixed, not only the seed. |
| “Two identical runs prove the program is correct.” | They prove reproducibility. A reproducible
program can still break the rules. | Compare with the specification and the golden traces. |
| “Any seed will do for a comparison.” | Any *fixed* seed works, but both sides must use the same
one. | Fix one seed and record it with the result. |
| “Equal hashes prove correctness.” | A hash only proves that two byte sequences are equal. | Use
the hash for identity, the rules for correctness. |
| “The seed is a secret or a password.” | It is an ordinary input that selects a sequence. | Treat
it as part of the command line. |
| “A different output means the harness is broken.” | A different seed is supposed to produce a
different game. | Compare same-seed runs to test stability; vary the seed to test sensitivity. |

## Terms used here

| Term | One-line meaning |
|---|---|
| Determinism | The same inputs and seed produce the same output. |
| Reproducibility | An independent run under the same controls yields the same bytes. |
| Seed | The value that initializes the pseudo-random sequence. |
| Pseudo-random | A repeatable sequence that behaves statistically like random values. |
| Control variable | An input held fixed so it cannot explain a difference. |
| Attribution | Assigning an observed difference to a specific cause. |
| Transcript | The observable behavior of one run, one record per line. |
| Digest | A fixed-length fingerprint of a byte sequence. |

The canonical definitions are in [`glossary.md`](../../../docs/guide/glossary.md).

## What “done” means here

The **Self-check (binary)** above is the authoritative checklist. If every box is ticked, the
lesson is done for you. `meta.yml` mirrors the same criteria for tooling.

## Next step

- Continue to `common-04` — golden traces — and use the harness you just read to verify the trace
  set.
- If you want the entry-point side first, go to `common-03` — entry point and working-directory
  independence.
- If seed 7 and seed 8 produced the same transcript, stop and investigate before moving on.

## References

- [`common/harness/reference.py`](../../harness/reference.py) — the reference implementation.
- [`common/harness/README.md`](../../harness/README.md) — command line and determinism guarantees.
- [`common/traces/`](../../traces/) — the golden traces for seeds 1–20.
- [`common/traces/README.md`](../../traces/README.md) — verification and regeneration procedure.
- [`common/spec/game-rules.md`](../../spec/game-rules.md) — §12 fixes the random call order.
- [`common/spec/transcript-format.md`](../../spec/transcript-format.md) — the record shapes.
- [`docs/guide/how-learning-works.md`](../../../docs/guide/how-learning-works.md) — the engineering
  check and the evidence portfolio.
- [`docs/guide/glossary.md`](../../../docs/guide/glossary.md) — canonical terms.
