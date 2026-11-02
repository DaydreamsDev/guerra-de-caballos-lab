# common-05 — Golden traces (trazas de referencia)

> [English](lesson.md) | [Español](lesson-es.md)

> **Status:** drafted · **Concept:** behavioral contract (contrato de comportamiento) ·
> **Prerequisites:** common-02 · **Estimated time:** 45 minutes
> **Reading:** ~14 minutes

## Why this lesson exists

A trace file is just text. On its own it proves nothing: anyone can paste twenty lines that look
right. What turns a trace into evidence is the chain around it — the command that produced the
observation, the observed output, and the explanation that connects them to a rule.

This distinction matters because teams routinely skip it. "It matches the reference" is a claim.
"Here is the command, here is the empty diff, and here is why an empty diff is the expected result"
is evidence. The first cannot be checked by anyone else; the second can.

The laboratory stores its behavioral contract as twenty traces under
[`common/traces/`](../../traces/). In this lesson you verify their integrity, reproduce one of them
from scratch, and then explain what the reproduction does and does not prove.

## What you will be able to do

By the end of this lesson you can:

- explain why a golden trace is a behavioral contract, and what it fixes in place;
- verify the trace set against [`checksums.txt`](../../traces/checksums.txt) and read the result
  correctly, including its localized wording;
- reproduce one trace and show an empty diff, with the command and the output recorded;
- run a negative control that produces a non-empty diff, and explain what it isolates;
- state what twenty passing traces do not prove;
- record the command, the observed output and the explanation as one piece of evidence.

## The concept: behavioral contract (contrato de comportamiento)

A **behavioral contract** is an agreement about observable behavior: given the same inputs, an
implementation must produce the same output. The trace set is where that agreement is written down.

| Idea | Meaning in this lesson |
|---|---|
| **Golden trace** | A recorded expected output, kept as the reference for one input. |
| **Behavioral contract** | Same seed and player name must produce the same transcript. |
| **Authority** | The trace wins for its own seed; the specification wins over both. |
| **Regression** | A change that breaks a trace has changed agreed behavior, on purpose or not. |
| **Control** | A run you can compare against so that a difference means something. |
| **Evidence** | Command + observed output + explanation, together, reproducible by someone else. |

Three sentences carry the lesson:

1. The trace records behavior; it does not describe intent.
2. The checksum proves the bytes have not changed since recording; it proves nothing about whether
   those bytes are right.
3. The reproduction proves the current code still produces those bytes; only the explanation says
   why that matters.

### What a trace fixes, and what it leaves open

A trace pins one path through a program. It says: for this seed, this is the sequence of turns,
attempts and the result. It does not say the program is correct for every seed, and it does not say
the rules were understood. Those are different claims, checked by different means.

[`game-rules.md`](../../spec/game-rules.md) §13 states the agreement plainly: *two implementations
agree when they produce the same transcript for the same seed and player name*. That is the whole
contract. Everything else in this lesson is how you demonstrate it instead of asserting it.

### The sentence this lesson is built on

> A trace is evidence only when the command and the observed output are recorded with it.

Read it twice. The file alone is a specimen. The command is how someone else gets the specimen. The
output is what they should see. The explanation is why the specimen settles the question. Remove any
one of the three and the claim becomes an opinion with a file attached.

## Before you start

Read these documents in full:

- [`common/traces/README.md`](../../traces/README.md) — the trace set, its authority and its
  regeneration procedure.
- [`common/harness/README.md`](../../harness/README.md) — the command line and how to check a trace.
- [`common/spec/game-rules.md`](../../spec/game-rules.md) — especially §13, the transcript as the
  behavioral contract.
- [`common/spec/transcript-format.md`](../../spec/transcript-format.md) — the record shapes you are
  about to compare.
- [`docs/guide/verification.md`](../../../docs/guide/verification.md) — what counts as evidence.

You also need:

- Python 3.8 or newer, a POSIX shell with `diff`, `cmp`, `sha256sum` and `grep`;
- Bash for the process-substitution form used below (the redirect form works in any POSIX shell);
- a scratch directory for your own copies (`$HOME/common-05-scratch` is used here);
- the trace directory: [`common/traces/`](../../traces/).

Do not edit any file under `common/traces/`. They are inputs to this lesson, not workspace.

## The 45-minute path

| Min | Step | What you do | Evidence |
|---|---|---|---|
| 0–8 | Read | Read the trace set, the checksum file and one trace end to end. | Annotated trace |
| 8–15 | Predict | Write what the checksums prove, and what the reproduction will show. | Prediction note |
| 15–22 | Verify | Run `sha256sum -c checksums.txt`; read every line. | Command + output |
| 22–32 | Reproduce | Rebuild one trace from its seed and diff it. | Command + empty diff |
| 32–38 | Control | Force a non-empty diff with one changed input. | The exact differing line |
| 38–42 | Explain | Say why the trace is evidence only with command and output. | One paragraph |
| 42–45 | Record | Fill `EVIDENCE.md`. | Portfolio entry |

One step can take longer; that is information about the
step, not about you.

## Step 1 — Read the trace set (8 min)

Three files define the contract. Read them in this order.

### 1.1 The checksum file

[`common/traces/checksums.txt`](../../traces/checksums.txt) has twenty lines, one per trace. Each
line is a 64-character SHA-256 digest, two spaces, and a bare filename:

```text
bf74281f720ef49df792887caff7dd1512803639b18b2f6754c161663212158b  seed-0001.txt
```

Two details matter and both will matter later:

- the filename is **bare**, not a path, so the check depends on the directory you run it from;
- the digest identifies bytes. It says nothing about meaning.

The file covers `seed-0001.txt` through `seed-0020.txt`, and nothing else. A new trace file outside
that range is not covered by the check, no matter how correct it looks.

### 1.2 One trace, end to end

Open [`seed-0001.txt`](../../traces/seed-0001.txt). It is 89 lines and has four regions:

1. **Header** — seven records: `seed`, `player`, two rolls, `start`, and the two starting cells.
2. **Turns** — one `turn=` line per turn, carrying the active knight, its cell and the eight-move
   `order`.
3. **Attempts and moves** — `try=` lines with two leading spaces, then one `move=` line for the
   successful attempt.
4. **Result** — `winner=machine` and `moves=21`.

Notice what the file does not contain: no timestamps, no paths, no version string, no hostname. A
trace is portable precisely because nothing about the machine leaks into it. That is why a checksum
over it is meaningful at all.

### 1.3 The shape of the set

Five traces are enough to see the pattern. Read the result lines yourself; do not take the table on
faith.

| Trace | Seed | Winner | `moves=` | `turn=` lines | Ending |
|---|---|---|---|---|---|
| `seed-0001.txt` | 1 | machine | 21 | 22 | trap |
| `seed-0007.txt` | 7 | player | 10 | 10 | capture |
| `seed-0010.txt` | 10 | machine | 22 | 22 | capture |
| `seed-0011.txt` | 11 | machine | 8 | 9 | trap |
| `seed-0020.txt` | 20 | machine | 47 | 48 | trap |

The last two columns are the interesting part. On a capture, the winning move is a successful move,
so the counts match. On a trap, the final turn consumes a `start` draw and a `turn=` line but
records no `move=` line, so turns exceed moves by one. That relationship is a small, checkable
consequence of §6, §10 and the transcript format — and it is exactly the kind of thing a prediction
should name.

The set has no draws. That is a property of these twenty seeds, not a rule: a draw needs both
knights alive at move 200. Keep that fact; it becomes a limitation you must state in Step 6.

## Step 2 — Predict (7 min)

Write the prediction **before** you run anything. You are predicting what each command establishes,
not guessing random digits.

Answer at least these:

1. If `sha256sum -c checksums.txt` passes, what exactly have you proven about the traces?
2. What will the passing output say, word for word, on a machine with a non-English locale? What
   will the exit status be?
3. Will reproducing seed 1 produce an empty diff? How many header records will the reproduced
   transcript have?
4. What happens if you run the checksum command from the repository root instead of from
   `common/traces/`? Why?
5. If you reproduce seed 1 with the player name `Alex`, will the diff be empty? Which line or lines
   will differ?
6. What could be wrong with the implementation and still pass all twenty traces?

Write one falsifiable sentence, for example:

> The checksum check will pass with exit status 0, and the reproduced seed 1 transcript will diff
> empty against `seed-0001.txt`, because both commands only read files that are already in the
> repository.

A prediction is not a guess you are allowed to fix later. The gap between prediction and observation
is the learning signal; editing the prediction after the run destroys it.

## Step 3 — Verify the checksum set (7 min)

Run the check from the directory the checksum file expects:

```bash
cd common/traces
sha256sum -c checksums.txt
echo "exit=$?"
```

Expected: twenty lines, each reporting a match, and `exit=0`. On an English locale the line ends in
`OK`; on a Spanish locale it ends in `La suma coincide`. Both are a pass. The wording is translated;
the exit status is not.

This is the first place where a prediction can be wrong in an instructive way. If you wrote "it will
print OK", you predicted a translation, not a behavior. The behavior is: every line reports success
and the process exits `0`.

Count the lines to confirm the check covered what you think it covered:

```bash
sha256sum -c checksums.txt | wc -l
```

Twenty is the only acceptable answer. Nineteen means a trace is unreferenced; twenty-one means the
file refers to something that should not be there.

### 3.1 The directory trap

Now run the same check from the repository root, and keep the failure:

```bash
cd ../..
sha256sum -c common/traces/checksums.txt
echo "exit=$?"
```

It fails, and the message names a missing file such as `seed-0001.txt`, even though you can see that
file under `common/traces/`. The reason is in the file you read in Step 1: entries are bare
filenames. `sha256sum` resolves them against the current directory, not against the directory of the
checksum file.

Record this. It is not a defect in the repository; it is how the tool reads the file. It is also the
kind of mismatch that produces a false alarm in a CI job, and the explanation is a single sentence
about relative paths.

If you prefer a version that works from the root, be explicit instead of guessing:

```bash
cd common/traces && sha256sum -c checksums.txt
```

### 3.2 If a line fails

A `FAILED` line means the bytes on disk no longer match the recorded digest. Do not fix it by
regenerating the checksum. That would erase the evidence that something changed. Record:

- the filename and the reported digest versus the expected digest;
- whether the file was modified in your working tree (`git status`);
- what you changed, if anything.

A trace is regenerated only when the implementation or the specification changed, and then the whole
set is regenerated and its diff reviewed — the procedure in
[`common/traces/README.md`](../../traces/README.md).

## Step 4 — Reproduce one trace (10 min)

Now produce the trace yourself and compare. From the repository root:

```bash
diff -u common/traces/seed-0001.txt \
  <(common/harness/run.sh --seed 1 --name Learner)
echo "exit=$?"
```

Expected: no output, `exit=0`. An empty diff means the current code reproduces the recorded behavior
for seed 1 and player `Learner`.

If your shell does not support `<(...)`, use a file. It is the same observation, one step longer:

```bash
mkdir -p "$HOME/common-05-scratch"
common/harness/run.sh --seed 1 --name Learner > "$HOME/common-05-scratch/seed-0001.txt"
diff -u common/traces/seed-0001.txt "$HOME/common-05-scratch/seed-0001.txt"
cmp -s common/traces/seed-0001.txt "$HOME/common-05-scratch/seed-0001.txt" \
  && echo "IDENTICAL" || echo "DIFFERENT"
```

`IDENTICAL` plus an empty diff is the same fact stated twice. That redundancy is deliberate: `diff`
shows you where, `cmp` gives you a binary answer you can script.

Record the digest of both sides, because a path is not a proof that the bytes matched:

```bash
sha256sum common/traces/seed-0001.txt "$HOME/common-05-scratch/seed-0001.txt"
```

The two digests must be equal, and they must equal the first line of `checksums.txt`.

### 4.1 What you just established

Three separate facts, and they are not the same fact:

1. the recorded trace matches its digest (Step 3);
2. the current implementation reproduces those bytes (Step 4);
3. the reproduced bytes have the shape the format requires (Step 1).

A claim of "it matches" collapses all three. Your evidence file should keep them separate, because
each one can fail for a different reason.

## Step 5 — Run a negative control (6 min)

A check you have never seen fail is not a check; it might be comparing a file with itself. Prove the
diff can see a difference by changing exactly one input.

```bash
diff -u common/traces/seed-0001.txt \
  <(common/harness/run.sh --seed 1 --name Alex)
```

Expected: exactly one hunk, and inside it exactly one changed line pair:

```text
-player=Learner
+player=Alex
```

`diff` exits `1` when it finds differences. That exit status is a result, not an error. Two
conclusions follow, and both are useful:

- the comparison is sensitive to the player name, which is part of the transcript even though it
  never reaches the random generator;
- the rest of the game is unchanged, because the name is written to the header and nothing else
  reads it.

That second conclusion is the interesting one. The diff isolates one input's effect. Without the
control, "empty diff" and "diff that cannot detect anything" look identical.

### Optional second control

Change the seed instead of the name:

```bash
diff -u common/traces/seed-0001.txt \
  <(common/harness/run.sh --seed 2 --name Learner)
```

Predict the size of the diff first. It will be large and will start at the `seed=` line. If it were
small, the seeds would not be selecting different games, which would contradict §12. Do not record
this one unless it taught you something the first control did not.

## Step 6 — Explain (4 min)

Write one paragraph, without syntax words. Answer:

- What does the checksum prove, and what does it not prove?
- What does the reproduction prove, and what does it not prove?
- Why is the command part of the evidence, and not just a convenience for producing it?
- What would you have to add to the trace set to cover a draw, and why can you not claim draw
  correctness from these twenty files?

A useful sentence to finish: *"The trace is the claim; the command and the output are how someone
else checks the claim; the explanation is why the claim settles the question."*

Do not name a programming language, a data type, or a syntax element. If you need one to explain the
idea, the idea is not yet separated from the implementation.

### The limits you must state

Twenty traces are a regression net, not a proof. They pin twenty paths. A bug that only appears on
seed 21 survives the whole set. A draw path is never exercised, because none of these seeds draws.
And a trace can, in principle, contradict the specification — in that case the trace is regenerated
from the corrected harness, and the specification wins.

Saying this out loud is part of the lesson. Evidence with a stated scope is stronger than evidence
presented as universal.

### The trap to avoid

The tempting wrong explanation is: *"The checksums pass, so the traces are correct."* The checksums
pass because the files have not changed. Whether the files describe the game the rules require is a
different question, answered by reading §1–§13 and comparing behavior, not by a digest.

## Step 7 — Record (3 min)

Fill [`EVIDENCE.md`](EVIDENCE.md). It is your portfolio entry. Include:

- the prediction, written before the runs;
- the checksum command, its working directory, and its full output, including the localized wording;
- the line count of the checksum output;
- the reproduce command, the diff result and the exit status;
- the two digests, and the first line of `checksums.txt` for comparison;
- the negative control: the input you changed, the differing line and the diff exit status;
- the explanation paragraph, including the limits of the set;
- the next step.

## Self-check (binary)

- [ ] I read `checksums.txt`, one full trace and the trace README.
- [ ] I wrote the prediction before running any command.
- [ ] `sha256sum -c checksums.txt` reported twenty successful lines from `common/traces`.
- [ ] I recorded the exact wording and the exit status, not just "it passed".
- [ ] I explained why the same command fails from the repository root.
- [ ] I reproduced seed 1 and the diff against the golden trace was empty.
- [ ] I recorded the command, the empty output and the exit status.
- [ ] The reproduced digest equals the recorded digest.
- [ ] I produced a non-empty diff with one changed input and named the differing line.
- [ ] I can state what the trace set does not prove, without syntax words.
- [ ] I filled `EVIDENCE.md` and know my next step.

If a box is not ticked, the lesson is not yet done **for you**. The unticked box names the next
step. There is no penalty for repeating a step.

## Misconceptions this lesson refutes

| Misconception | Why it is wrong | What to do instead |
|---|---|---|
| "The trace is the evidence." | A file can be copied, edited or stale; evidence needs a reproducible command and an observed output. | Record command, output and explanation together. |
| "Passing checksums prove correctness." | A digest proves the bytes have not changed, not that they obey the rules. | Use digests for integrity, reproduction for behavior, rules for correctness. |
| "An empty diff proves the program is right." | It proves the program reproduced one recorded path. | State the scope: twenty seeds, no draws. |
| "A failing checksum means the trace is wrong." | It means the bytes changed; the cause may be an edit, a bad merge or a regeneration. | Investigate the change before touching the digest. |
| "The checksum command is broken if it fails from the root." | Entries are bare filenames resolved against the working directory. | Run it from `common/traces`, or change directory explicitly. |
| "Localized output means the check failed." | The tool translates its message; the exit status carries the result. | Read the status, and record the wording you saw. |
| "A diff with exit status 1 is an error." | `diff` uses `1` to mean "differences found". | Treat `1` as an observation, `2` as an error. |
| "Hand-editing a trace is a quick fix." | It breaks the contract and hides the real change. | Change code or spec, regenerate the whole set, review the diff. |
| "Twenty traces cover the game." | They cover twenty seeds and no draws. | Say "regression for these seeds", not "proof for all games". |

## Terms used here

| Term | One-line meaning |
|---|---|
| Golden trace | A recorded expected output used as the reference for one input. |
| Behavioral contract | The agreement that the same inputs produce the same observable output. |
| Digest | A fixed-length fingerprint of a byte sequence. |
| Reproduce | Produce the recorded output again from the current code. |
| Diff | A line-by-line comparison of two texts. |
| Negative control | A run expected to differ, used to prove the comparison can detect a difference. |
| Regression | A change that breaks previously agreed behavior. |
| Scope | The set of inputs a piece of evidence actually covers. |

The canonical definitions are in [`glossary.md`](../../../docs/guide/glossary.md).

## What “done” means here

The **Self-check (binary)** above is the authoritative checklist. If every box is ticked, the
lesson is done for you. `meta.yml` mirrors the same criteria for tooling.

## Next step

- If the checksums pass and seed 1 reproduces with an empty diff, continue to `construction-00` —
  read the rules and run the reference baseline — and use the trace set as your regression net.
- If a checksum fails, stop and investigate before any later lesson. A trace set you cannot verify
  cannot support any claim built on it.
- If the reproduction diff is non-empty but the checksums pass, the implementation and the trace
  disagree. Record the first differing line; the specification decides which side is wrong.
- If you cannot yet explain why the command belongs in the evidence, repeat Step 6 with a stricter
  test: delete the command from your note and ask whether a stranger could reproduce your result.

## References

- [`common/traces/`](../../traces/) — the twenty golden traces.
- [`common/traces/checksums.txt`](../../traces/checksums.txt) — the digests that pin them.
- [`common/traces/README.md`](../../traces/README.md) — authority, verification and regeneration.
- [`common/harness/README.md`](../../harness/README.md) — the command line and the check recipes.
- [`common/harness/reference.py`](../../harness/reference.py) — the reference implementation.
- [`common/spec/game-rules.md`](../../spec/game-rules.md) — §13 defines the contract.
- [`common/spec/transcript-format.md`](../../spec/transcript-format.md) — the record shapes.
- [`docs/guide/verification.md`](../../../docs/guide/verification.md) — what counts as evidence.
- [`docs/guide/how-learning-works.md`](../../../docs/guide/how-learning-works.md) — the engineering
  check and the evidence portfolio.
- [`docs/guide/glossary.md`](../../../docs/guide/glossary.md) — canonical terms.
