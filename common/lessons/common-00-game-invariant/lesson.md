# common-00 — The game as an invariant (El juego como invariante)

> [English](lesson.md) | [Español](lesson-es.md)

> **Status:** drafted · **Concept:** specification (especificación) · **Prerequisites:** none ·
> **Estimated time:** 45 minutes
> **Reading:** ~10 minutes

## Why this lesson exists

The game is the invariant. Every variant in this laboratory must produce the same transcript for the
same seed. That promise only works if the rules are written so two independent readers can agree on
what the program must do.

A specification is not the code. It is the decision about behavior that the code must
satisfy. If the rules are ambiguous, two implementations can both claim to follow them and still
disagree. Then the transcript stops being a contract and becomes an opinion.

This lesson trains the first engineering move: read the rules as decisions, run the reference twice,
and turn one ambiguity into a binary acceptance criterion.

## What you will be able to do

By the end of this lesson you can:

- separate a normative rule from an illustrative example;
- run the reference twice with the same seed and show that the transcripts are identical;
- name one ambiguous rule and explain why it is ambiguous;
- write one binary acceptance criterion that makes the disputed behavior observable.

## The concept: specification (especificación)

The glossary defines **specification** as *the behavior a program must exhibit*. Five ideas matter
here:

| Idea | Meaning in this lesson |
|---|---|
| **Specification** | A decision about observable behavior, written before the code. |
| **Invariant** | The behavior that never changes across variants: same seed, same transcript. |
| **Contract** | The transcript is the behavioral contract between the rules and every implementation. |
| **Ambiguity** | Two competent readers can follow the text and produce different observable lines. |
| **Binary criterion** | A test with two outcomes: pass or not yet. |

The canonical source for the four layers is
[`conceptual-framework.md`](../../../docs/guide/conceptual-framework.md). Read the concept layer
carefully: a concept survives a language change; a specification survives an implementation change.

## Before you start

Read these documents in full:

- [`common/spec/game-rules.md`](../../spec/game-rules.md) — the 14 sections that define the game.
- [`common/spec/transcript-format.md`](../../spec/transcript-format.md) — the record shapes.

You also need:

- Python 3.8 or newer and a terminal;
- a scratch directory for the transcripts (do not commit run output);
- the reference implementation: [`common/harness/reference.py`](../../harness/reference.py).

The reference is the executable form of the rules. The golden traces under
[`common/traces/`](../../traces/) are the recorded contract for seeds 1 through 20. Use seed 7: it
has a golden trace and it does not appear in the illustrative example of the transcript format.

## The 45-minute path

| Min | Step | What you do | Evidence |
|---|---|---|---|
| 0–10 | Read | Read the rules and the transcript format; mark rules and examples. | Three marked rules |
| 10–15 | Predict | Write what the two runs will show, before running. | Prediction note |
| 15–20 | Run | Run the reference twice with the same seed and player. | Two transcripts |
| 20–30 | Compare | Diff run A against run B; diff the prediction against the observation. | Exact differing lines |
| 30–40 | Investigate | Name one ambiguity; write one binary acceptance criterion. | Ambiguity + criterion |
| 40–45 | Explain and record | Explain why the ambiguity matters; fill `EVIDENCE.md`. | Portfolio entry |

One step can take longer; that is information about the
step, not about you.

## Step 1 — Read for decisions, not for code (10 min)

Read `game-rules.md` once for shape, then again with a pencil. Mark:

1. **Normative language:** *is*, *must*, *draws*, *redraw until*, *wins*, *loses*, *ends*.
2. **Numbers that bound behavior:** 8×8, 1..50, 1..8, 200 successful moves.
3. **Examples:** the transcript example is explicitly illustrative; the golden traces are
   authoritative for their seeds.
4. **Places where you ask “which one?” or “when exactly?”** Those are the pressure points.

A useful reading rule: a rule is normative when it changes an observable line. If two readings
change no line, the difference is prose, not behavior.

### Worked example: a contradiction you can resolve

Section 4 says:

- §4.3: if the rolls are equal, redraw `roll_machine` until it differs from `roll_player`.
- §4.5: if the rolls are equal, redraw both rolls.

Under §4.3, equality is removed before §4.4, so §4.5 is unreachable. Under §4.5, the procedure
consumes a different sequence of random values. Two readers can therefore produce different headers
and different games.

Section 12 resolves the call order: `roll_machine` is redrawn until it differs from
`roll_player`. So the specification as a whole has an answer, but §4.5 contradicts it. The ambiguity
is real; the resolution is also real. Your job is to make the resolution testable.

### Pressure points to investigate (pick one)

Do not treat this as a checklist. Use it to start your own reading.

| Where | Question a reader can ask | Why it can change the transcript |
|---|---|---|
| §4.3 vs §4.5 | On equal rolls, redraw one roll or both? | Different random calls produce different rolls and starts. |
| §10 + transcript format | Does a trapped turn emit a `turn=` line and consume a `start` draw, or only `no_legal_move`? | The trap record may be one line or several. |
| §11 + §10 | On move 200, is the game a draw or a trap win? | The final `winner=` line changes. |
| Transcript header | Which characters may `player=<name>` contain? | A newline or space can break the one-record-per-line format. |

Choose a different pressure point from the §4.3 / §4.5 example for your own criterion.

## Step 2 — Predict (5 min)

Write the prediction **before** you run anything. The prediction is about the invariant, not about
guessing random digits.

Answer at least these:

1. Will run A and run B be byte-identical? Why?
2. Which parts of the transcript could differ if the program used unseeded randomness, the clock, or
   an unordered collection?
3. If the first `start` were 5, what would the `order=` field be? Derive it from §6 without running.
4. What will the last two lines of the transcript look like in shape (`winner=...`, `moves=...`)?

Write one falsifiable sentence, for example:

> Run A and run B will be byte-identical because the only randomness is seeded and the program reads
> no other input.

A prediction is not a guess you are allowed to fix later. The gap between prediction and observation
is the learning signal; editing the prediction after the run destroys it.

## Step 3 — Run (5 min)

Use the same seed and the same player name for both runs. From the repository root:

```bash
mkdir -p "$HOME/common-00-scratch"
common/harness/run.sh --seed 7 --name Learner > "$HOME/common-00-scratch/run-a.txt"
common/harness/run.sh --seed 7 --name Learner > "$HOME/common-00-scratch/run-b.txt"
```

Then compare the two files:

```bash
diff -u "$HOME/common-00-scratch/run-a.txt" "$HOME/common-00-scratch/run-b.txt"
```

If `diff` prints nothing, the two transcripts are identical. Also check the exit status:

```bash
cmp -s "$HOME/common-00-scratch/run-a.txt" "$HOME/common-00-scratch/run-b.txt" \
  && echo "IDENTICAL" || echo "DIFFERENT"
```

Record the exact command you used. A command you cannot reproduce is not evidence.

**Optional extension:** run with a different seed and confirm that the transcript changes. That
shows the seed is doing work; it is not required for this lesson.

## Step 4 — Compare (10 min)

Make these checks, in this order.

### 4.1 Run A against Run B

The invariant says: same seed, same transcript. The only acceptable result is **no differences**.

If the files differ, record:

- the first differing line;
- the field that differs;
- the smallest cause you can defend (unseeded randomness, time, iteration order, environment).

Do not “fix” the comparison by normalizing the output. Normalizing hides the defect.

### 4.2 Run A against the golden trace

The trace set is the recorded contract. Compare your run with the trace for the same seed:

```bash
diff -u common/traces/seed-0007.txt "$HOME/common-00-scratch/run-a.txt"
```

An empty diff means your run matches the committed behavior. A non-empty diff means either your
command used a different seed or player name, or the implementation and the trace disagree. Record
which one.

### 4.3 Prediction against observation

Compare your prediction note with the actual transcript:

| Question | Observation |
|---|---|
| Were the two runs identical? | yes / no |
| Did the `order=` field match your derivation? | yes / no; the exact line |
| Did the header have the seven expected records? | yes / no; the exact line |
| Did the result have the expected shape? | yes / no; the exact line |

Record the exact lines that differ. “It mostly matched” is not evidence.

### 4.4 Format conformance

Check the transcript against `transcript-format.md`:

- the header records appear once, in the documented order;
- every `turn=` line has eight move numbers in `order=`;
- attempt lines start with the two documented spaces;
- a capture appends `capture` to the successful move line;
- the result has both `winner=` and `moves=`.

A transcript can be deterministic and still violate the format. Both checks matter.

## Step 5 — Investigate: one ambiguity, one binary criterion (10 min)

This is the deliverable of the lesson. Do it in three moves: name, test, write.

### 5.1 Name the ambiguity

An ambiguity is not “I do not understand.” It is a precise claim:

> Under reading A, the transcript contains line X. Under reading B, it contains line Y. The text
> supports both readings.

A good ambiguity has three properties:

1. It lives in normative text, not in an illustrative example.
2. It changes at least one observable line.
3. It is not already resolved by a section you have not read. If another section resolves it, say
   so; the issue is then a contradiction or redundancy, which is still worth recording.

### 5.2 Test the ambiguity

For your chosen rule, write:

- the section reference;
- the two readings, in one sentence each;
- the observable line that would differ;
- the input that exposes the difference (seed, player name, or board position).

If you cannot name an observable line, you have a feeling, not an ambiguity. Keep reading.

### 5.3 Write a binary acceptance criterion

Use this shape:

```text
Given <input>, the transcript shows <observable condition>.
Pass: the condition holds.
Not yet: the condition does not hold.
```

A good criterion:

- names the input (for example, `seed=7` and `player=Learner`);
- names the observable (a field, a line, a count, a diff);
- has exactly two outcomes;
- does not use “correct”, “reasonable”, “as expected” without a reference;

**Worked criterion example (do not reuse it):**

- Ambiguity: §10 and the transcript format do not say whether a trapped turn emits a `turn=` line
  and attempt lines, or only the `no_legal_move` line. The reference and `seed-0007.txt` emit the
  turn line and the eight attempts.
- Binary criterion: *Given `seed=7`, after the last `move=` line the transcript contains one
  `turn=` line, eight `try=` lines, and one `no_legal_move trapped=player` line. Pass when that
  sequence appears exactly; not yet when any line is missing or reordered.*

Why this works: it names an input, an observable, and two outcomes. It does not say which reading
“feels right”; it says what the transcript must show.

**Completion (optional, 5 min):** take the §4.3 / §4.5 contradiction. Section 4.3 redraws only
`roll_machine`; §4.5 redraws both rolls. Write the two readings, the observable line, and a binary
criterion that distinguishes the §12 call order from the §4.5 order. This is practice; it is not
your deliverable.

**Your turn (required):** choose one ambiguity different from the trap example and write your own
criterion in `EVIDENCE.md`.

## Step 6 — Explain (5 min)

Write one paragraph, without syntax words. Answer:

- Why does an ambiguous rule invalidate every comparison?
- What assumption did you bring that the text did not support?
- What would you need to change in the rules to remove the ambiguity: add a rule, remove a rule, or
  name a priority between two rules?

A useful sentence to finish: *“An ambiguous rule is not a small documentation bug; it means two
implementations can both claim to follow the rules and still disagree.”*

Do not name a programming language, a data type, or a syntax element. If you need one to explain the
idea, the idea is not yet separated from the implementation.

## Step 7 — Record (5 min)

Fill [`EVIDENCE.md`](EVIDENCE.md). It is your portfolio entry. Include:

- the prediction, written before the run;
- the exact commands and their output;
- the diff, including “no differences” when that is the result;
- the ambiguity, with its section reference;
- your binary acceptance criterion;
- the explanation paragraph;
- the next step.

## Self-check (binary)

- [ ] I read all 14 sections of `common/spec/game-rules.md` and the transcript format.
- [ ] I wrote the prediction before running the reference.
- [ ] I ran the reference twice with the same seed and player.
- [ ] The two transcripts are byte-identical, or I recorded the first differing line.
- [ ] I compared the prediction with the observation and recorded the exact lines.
- [ ] I compared the run with the golden trace for the same seed.
- [ ] I named one ambiguous rule with a section reference.
- [ ] I wrote one binary acceptance criterion with an input and an observable condition.
- [ ] I explained why the ambiguity matters without using syntax words.
- [ ] I filled `EVIDENCE.md` and know my next step.

If a box is not ticked, the lesson is not yet done **for you**. The unticked box names the next
step. There is no penalty for repeating a step.

## Misconceptions this lesson refutes

| Misconception | Why it is wrong | What to do instead |
|---|---|---|
| “The code is the specification.” | Code is evidence of one implementation; the specification is the decision it must satisfy. | Read the rules, then test the code against them. |
| “The example is the specification.” | The transcript example is explicitly illustrative; the traces are authoritative for their seeds. | Use the example to learn the shape, not to settle a dispute. |
| “If it runs, it is correct.” | A program can run and still violate the contract. | Compare transcripts and check format conformance. |
| “An ambiguity is a small documentation issue.” | It lets two implementations disagree while both claim compliance. | Write a binary criterion that makes the difference observable. |
| “A criterion needs a score to be useful.” | A score hides the decision; a binary condition exposes it. | Use pass / not yet, with the observed output. |

## Terms used here

| Term | One-line meaning |
|---|---|
| Specification | The behavior a program must exhibit. |
| Invariant | The property that must always hold across variants. |
| Determinism | Same input and seed produce the same output. |
| Seed | The value that initializes the pseudo-random sequence. |
| Transcript | The observable behavior of one run, one record per line. |
| Acceptance criterion | An observable, binary condition that defines whether the artifact meets its goal. |
| Ambiguity | Two readings of the same text produce different observable behavior. |

The canonical definitions are in [`glossary.md`](../../../docs/guide/glossary.md).

## What “done” means here

The **Self-check (binary)** above is the authoritative checklist. If every box is ticked, the
lesson is done for you. `meta.yml` mirrors the same criteria for tooling.

## Next step

- If both runs are identical and match the golden trace, continue to `common-01` — deterministic
  harness — and compare its implementation with the rules you just read.
- If your two runs differ, do not move on. Record the first differing line and investigate the
  cause; a broken invariant makes every later comparison invalid.
- If you cannot name an ambiguity yet, reread §4, §10, §11 and the transcript format. The pressure
  points are there, but the wording is yours to produce.

## References

- [`common/spec/game-rules.md`](../../spec/game-rules.md) — the rules, sections 1–14.
- [`common/spec/transcript-format.md`](../../spec/transcript-format.md) — the record shapes.
- [`common/harness/reference.py`](../../harness/reference.py) — the reference implementation.
- [`common/traces/README.md`](../../traces/README.md) — the golden trace set.
- [`docs/guide/conceptual-framework.md`](../../../docs/guide/conceptual-framework.md) — the four
  layers.
- [`docs/guide/verification.md`](../../../docs/guide/verification.md) — criteria and evidence.
- [`docs/guide/glossary.md`](../../../docs/guide/glossary.md) — canonical terms.
