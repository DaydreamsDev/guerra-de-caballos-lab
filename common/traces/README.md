# Golden traces

> [English](README.md) | [Español](README-es.md)

This directory is the behavioral contract of the laboratory. Each file is the complete transcript
of one run, produced by [`../harness/reference.py`](../harness/reference.py) from the rules in
[`../spec/game-rules.md`](../spec/game-rules.md) and the format in
[`../spec/transcript-format.md`](../spec/transcript-format.md).

## The trace set

| Files | Seed | Player name | Format |
|---|---|---|---|
| `seed-0001.txt` … `seed-0020.txt` | `1` … `20` | `Learner` | File number padded; seed unpadded |

Every file contains the full transcript: header, turns, attempted moves, successful moves, and
the result. The set covers both winners and both terminal conditions:

| Outcome | Traces |
|---|---|
| Player wins | 5 |
| Machine wins | 15 |
| Wins by capture | 11 |
| Wins by trap | 9 |
| Draws | 0 |

The absence of draws is a property of these seeds, not a rule. A game reaches the 200-move limit
only when neither knight can capture or trap the other first.

## Authority

These traces are authoritative for their seeds. If another implementation produces different
output for the same seed and player name, the implementation is wrong, unless the trace itself
contradicts the specification — in that case the trace is regenerated from the corrected harness.

Never edit a trace by hand. Change the implementation or the specification, regenerate the whole
set, and review the diff.

## Verify a trace set

From this directory:

```bash
sha256sum -c checksums.txt
```

Every line must report `OK` (or the localized equivalent). `checksums.txt` contains one SHA-256
digest per `seed-*.txt` file.

Reproduce a single trace from the repository root:

```bash
diff -u common/traces/seed-0001.txt \
  <(common/harness/run.sh --seed 1 --name Learner)
```

An empty diff means the run matches the recorded behavior.

## Regenerate the set

From this directory:

```bash
for seed in $(seq 1 20); do
  printf -v paddedSeed '%04d' "$seed"
  ../harness/run.sh --seed "$seed" --name Learner > "seed-${paddedSeed}.txt"
done
sha256sum seed-*.txt > checksums.txt
```

Run the regeneration twice and compare the results byte for byte before accepting a new set. The
harness documentation describes the determinism guarantees in
[`../harness/README.md`](../harness/README.md).

## Using the traces as acceptance evidence

1. Predict the transcript, or at least its result, before running the candidate implementation.
2. Run the candidate with the same seed and player name.
3. Diff the candidate output against the file.
4. Explain every differing line.
5. Record the command, the diff, and the explanation in your evidence file.

The trace is evidence only when the command and the observed output are recorded with it.
