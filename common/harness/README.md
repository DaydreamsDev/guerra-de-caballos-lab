# Deterministic harness

> [English](README.md) | [Español](README-es.md)

The harness turns one seed into one transcript. It is the executable form of
[`../spec/game-rules.md`](../spec/game-rules.md) and the producer of the golden traces under
[`../traces/`](../traces/).

## Requirements

- Python 3.8 or newer.
- No third-party packages. The implementation uses only the standard library.

There is no build step.

## Command line

```text
reference.py --seed INTEGER --name TEXT [--max-moves INTEGER]
```

| Option | Required | Default | Meaning |
|---|---|---|---|
| `--seed` | yes | — | Integer used as the initial state of the deterministic generator. |
| `--name` | yes | — | Player name written to the `player=` header line. |
| `--max-moves` | no | `200` | Successful moves allowed before the game is declared a draw. |

Examples:

```bash
python3 common/harness/reference.py --seed 1 --name Learner
common/harness/run.sh --seed 1 --name Learner
```

`run.sh` is a thin wrapper that locates `reference.py` relative to itself, so it works from any
working directory. It forwards every argument unchanged.

Exit status is `0` after a transcript is written. Invalid arguments exit with the standard
`argparse` status `2`.

## Determinism

The same seed and the same player name always produce the same bytes. The generator is the 64-bit
linear congruential generator defined in the specification, and the call order is fixed:

1. two starting cells, redrawing the machine cell while it equals the player cell;
2. two roll values, redrawing the machine roll while it equals the player roll;
3. one move-order value per turn, immediately before the attempts of that turn.

No other random call is made. Set iteration is never used to build output, so the transcript
does not depend on the interpreter's hash seed.

## Transcript

The output matches [`../spec/transcript-format.md`](../spec/transcript-format.md) exactly:

- one UTF-8 record per line, terminated by a newline;
- header, one `turn=` line per turn, attempt lines with two leading spaces, successful `move=`
  lines, an optional `no_legal_move` line, and the two result lines;
- `capture=<knight>` on an attempt line names the knight that would be captured;
- a successful capture appends ` capture` to its `move=` line.

The output goes to standard output. Redirect it to create a trace:

```bash
common/harness/run.sh --seed 7 --name Learner > common/traces/seed-0007.txt
```

## Checking a trace set

Compare one run against its golden trace:

```bash
diff -u common/traces/seed-0007.txt \
  <(common/harness/run.sh --seed 7 --name Learner)
```

Verify that every recorded trace is reproducible:

```bash
cd common/traces
sha256sum -c checksums.txt
```

The checksum file covers `seed-0001.txt` through `seed-0020.txt`. See
[`../traces/README.md`](../traces/README.md) for the regeneration procedure.

## Behavior at the limits

- A capture ends the game immediately, before the turn passes.
- A trapped knight loses immediately; no successful move is recorded for that turn.
- When the successful move count reaches `--max-moves` with no winner, the result is
  `winner=draw`. The default value reproduces the 200-move rule in the specification.
- `--max-moves 0` is valid and produces a draw with zero moves.

## Relationship to the specification

The specification is authoritative. If this implementation and the specification disagree, the
implementation is corrected. The traces are authoritative for their seeds and act as the
behavioral contract for every other implementation.
