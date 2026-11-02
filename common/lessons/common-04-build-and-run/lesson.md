# common-04 — Entry point and working directory (punto de entrada y directorio de trabajo)

> [English](lesson.md) | [Español](lesson-es.md)

> **Status:** drafted · **Concept:** entry-point robustness (robustez del punto de entrada) ·
> **Prerequisites:** common-02 · **Estimated time:** 30 minutes
> **Reading:** ~11 minutes

## Why this lesson exists

A command that only works from one directory is not a reproducible command. It may work on your
machine, in your shell, on the day you wrote it, and fail for the next person who runs it from the
repository root or from a scratch directory. Before you compare two runs, you need one entry point
that behaves the same way wherever you start it.

The reference has no compile-and-link phase. That is true, but it is not the lesson. The lesson is
the entry point: the shell resolves the path before the program starts, and a wrapper that resolves
its own location removes the caller's working directory from the experiment.

In this lesson you run the same seed from the repository root and from another directory, compare
the bytes, and explain which line of the wrapper makes the absolute-path run work. The lesson
changes
no code; it changes how you run and observe it.

## What you will be able to do

By the end of this lesson you can:

- run [`common/harness/run.sh`](../../harness/run.sh) from the repository root with one command;
- run the same entry point from another working directory without copying the harness;
- explain what `run.sh` does with its own location and why that makes it location-independent;
- explain why a relative path from another directory fails before the wrapper runs;
- prove that the same seed and player name produce the same transcript from both locations;
- record the prediction, the commands, the interpreter version, the digests and the explanation in
  `EVIDENCE.md`.

## The concept: entry-point robustness

An **entry point** is the command that starts the program. **Entry-point robustness** means that the
command works from any working directory, with the same arguments and the same interpreter.

The shell resolves the path before the wrapper runs. A relative path is resolved against the current
directory; an absolute path is not. That is why the wrapper must locate its own directory instead of
assuming the caller's directory.

| Idea | Meaning in this lesson |
|---|---|
| **Entry point** | The command that starts the program; here, `common/harness/run.sh`. |
| **Working directory** | The directory the shell uses to resolve relative paths. |
| **Relative path** | A path resolved against the current directory. |
| **Absolute path** | A path that does not depend on the current directory. |
| **Location independence** | The wrapper finds the reference from any directory. |
| **Entry-point robustness** | The same command and inputs produce the same behavior from any
directory. |

### What the wrapper does

The wrapper asks the shell where the script itself lives, changes into that directory, stores the
absolute path and executes the reference next to it. The caller's working directory never becomes
the reference directory.

### What it does not mean

Location independence does not mean that a relative path works from anywhere. It does not. The shell
resolves the relative path first; if the file is not there, the wrapper never starts. The absolute
path is the one that reaches the wrapper from another directory.

It also does not mean that the environment is irrelevant. The interpreter version, the arguments and
the seed are still controls. The lesson removes one variable — the caller's directory — not all of
them.

## Before you start

Read these documents in full:

- [`common/harness/README.md`](../../harness/README.md) — requirements, the command line and the
  entry-point description.
- [`common/harness/run.sh`](../../harness/run.sh) — the six-line entry point.
- [`common/harness/reference.py`](../../harness/reference.py) — the reference implementation; read
  the imports and `build_transcript`.
- [`common/traces/README.md`](../../traces/README.md) — the golden traces and how they are verified.
- [`common/spec/game-rules.md`](../../spec/game-rules.md) — §12, the random call order, and §13, the
  transcript as the behavioral contract.

You also need:

- a POSIX shell with `bash`;
- Python 3.8 or newer;
- `cmp`, `sha256sum` and `diff`;
- a scratch directory for the outputs (do not commit run output).

The harness is the executable form of the specification. Its determinism is not an accident of the
implementation; it is a requirement of §12, which fixes the order of every random call.

## The 30-minute path

| Min | Step | What you do | Evidence |
|---|---|---|---|
| 0–5 | Read | Read `run.sh` , the harness README and the reference entry points. | Notes on the six
lines |
| 5–9 | Predict | Write what each command will do from each directory. | Prediction note |
| 9–14 | Run | Run seed 7 from the repository root. | `root.txt` and exit status |
| 14–19 | Run | Run seed 7 from another directory; try the relative path as a control. |
`elsewhere.txt` and exit status |
| 19–24 | Compare | Compare bytes and digests. | `cmp`, SHA-256, empty diff |
| 24–27 | Explain | Say why the entry point is location-independent and what still has to be
controlled. | One paragraph |
| 27–30 | Record | Fill `EVIDENCE.md` with commands, versions and observations. | Portfolio entry |

If one step takes longer, that is information about the
step, not about you.

### The engineering check in this lesson

| Step | Where it happens | What it produces |
|---|---|---|
| **Predict** | Step 2 | Exit statuses and byte equality for each directory. |
| **Run** | Steps 3 and 4 | Two transcripts, one from each working directory. |
| **Compare** | Step 5 | `cmp`, two digests and two diffs. |
| **Explain** | Step 6 | One paragraph on entry-point robustness. |
| **Record** | Step 7 | A completed `EVIDENCE.md`. |

The artifact is the pair of transcripts.

## Step 1 — Read the entry point (5 min)

Read [`common/harness/run.sh`](../../harness/run.sh). It is six lines long, including the blank
line. Each line has a job:

```bash
#!/usr/bin/env bash
set -euo pipefail

scriptDirectory="$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")" && pwd)"

exec python3 "${scriptDirectory}/reference.py" "$@"
```

Here is what each part does, in order:

1. The first line selects the shell that runs the script.
2. `set -euo pipefail` makes the script stop on an error, an unset variable or a failed pipeline.
   The wrapper is not allowed to hide a failure and exit successfully.
3. `scriptDirectory` asks the shell where the script itself lives, changes into that directory,
   prints the absolute path, and stores it. The value does not depend on where you called the script
   from.
4. `exec` replaces the shell process with `python3` and passes every argument unchanged. There is no
   intermediate copy and no argument rewriting.

The important line is the third one. `BASH_SOURCE[0]` is the path of the script as the shell sees
it. If the path is relative, the subshell changes into its directory and `pwd` returns an absolute
path. If the path is absolute, the result is still absolute. In both cases, `reference.py` is found
next to the wrapper, not next to the caller.

### A question before moving on

If you run `common/harness/run.sh` from the repository root, the shell finds the script because the
relative path exists. If you run the same relative path from `/tmp`, the shell cannot find the
script at all: it looks for `/tmp/common/harness/run.sh`, which does not exist. The wrapper never
starts, so its location logic never runs.

If you run the absolute path from `/tmp`, the shell finds the script, the wrapper computes its own
directory, and `reference.py` is found. The game should be identical.

Keep that distinction: *the shell resolves the entry point before the wrapper runs*. Location
independence belongs to the wrapper, not to the caller's path.

## Step 2 — Predict (4 min)

Write the prediction **before** you run anything. You are predicting exit statuses and byte
equality, not guessing random digits.

Answer at least these:

1. From the repository root, will `common/harness/run.sh --seed 7 --name Learner` exit `0`? Why?
2. From `/tmp`, will the same relative command exit `0`? What will the shell say?
3. From `/tmp`, will the absolute path to `run.sh` exit `0`? Why?
4. Will both successful runs produce byte-identical transcripts? What could make them differ?
5. Which line of `run.sh` makes the absolute run work from another directory?
6. If `run.sh` called `python3 reference.py` without `scriptDirectory`, what would happen from
   `/tmp`?
7. What does the wrapper’s location logic predict about the outputs? What does it not predict?

Write one falsifiable sentence, for example:

> The root run and the absolute-path run will produce byte-identical transcripts because the working
> directory is not written to the output; the relative run from `/tmp` will fail before Python
> starts.

A prediction is not a guess you are allowed to fix later. The gap between prediction and observation
is the learning signal; editing the prediction after the run destroys it.

### A worked prediction

Take question 3. Before running, write:

- expected exit status: `0`;
- expected reason: the shell finds the absolute path, `scriptDirectory` becomes the harness
  directory, and `exec python3` receives the same arguments;
- expected digest: the same as the root run.

That prediction is specific enough to fail. If the absolute run failed, the reason would be in the
wrapper or in the environment, not in the game.

## Step 3 — Run from the repository root (5 min)

Set the repository path once, then run from the root. Replace the path with your checkout.

```bash
REPO="/path/to/38-guerra-de-caballos-lab"
cd "$REPO"
mkdir -p "$HOME/common-04-scratch"

python3 --version
common/harness/run.sh --seed 7 --name Learner > "$HOME/common-04-scratch/root.txt"
echo "root exit=$?"
```

The `echo` prints the exit status of the command before it. Record the exact status and the exact
interpreter version. “Python 3” is not enough: the version is part of the toolchain.

Check the output shape:

```bash
wc -l "$HOME/common-04-scratch/root.txt"
tail -n 2 "$HOME/common-04-scratch/root.txt"
```

For seed 7 and player `Learner`, the transcript has 41 lines and ends with:

```text
winner=player
moves=10
```

That result is a fact about seed 7, not a rule. Do not generalize it to other seeds.

## Step 4 — Run from another directory (5 min)

Change to a directory outside the repository. `/tmp` is fine; use a scratch directory you can write
to. Run the same entry point by absolute path, with the same seed and name.

```bash
cd /tmp
"$REPO/common/harness/run.sh" --seed 7 --name Learner \
  > "$HOME/common-04-scratch/elsewhere.txt"
echo "elsewhere exit=$?"
```

The quoted path is the only difference from the root command. The arguments are identical. If the
transcript changes, the change came from the location or the environment, not from the game.

Now run the control: the relative path from the same directory.

```bash
cd /tmp
common/harness/run.sh --seed 7 --name Learner > /dev/null
echo "relative exit=$?"
```

The expected status is `127`, and the shell reports that the file does not exist. The exact wording
depends on your locale; record it as you see it. This is not a bug in the harness. The shell
resolves the relative path before the wrapper runs, and `/tmp/common/harness/run.sh` is not there.

The control matters because it separates two claims:

| Claim | What it means | How you test it |
|---|---|---|
| The entry point is location-independent. | The wrapper finds the reference from any directory. |
Absolute path from `/tmp` exits `0` . |
| A relative path is location-independent. | A path relative to the caller works everywhere. |
Relative path from `/tmp` exits `127` . |

The first claim is true. The second is false, and it is supposed to be false. A path is resolved
against the working directory; a wrapper that resolves its own directory is what removes the
dependency.

## Step 5 — Compare (5 min)

This is the proof. Use three independent tools; agreement between them is stronger than any one of
them alone.

### 5.1 Byte comparison

```bash
cmp -s "$HOME/common-04-scratch/root.txt" "$HOME/common-04-scratch/elsewhere.txt" \
  && echo "IDENTICAL" || echo "DIFFERENT"
```

`cmp` exits `0` when the files are byte-identical. The only acceptable result here is `IDENTICAL`.

### 5.2 Digest comparison

```bash
sha256sum "$HOME/common-04-scratch/root.txt" "$HOME/common-04-scratch/elsewhere.txt"
```

The two digests must match exactly. For seed 7 and player `Learner`, the expected digest is:

```text
b8ff5f7999322ac770a5b37056e86f7eb4fe8c71ea3f4fd5c457cc720af27dba
```

If your digest differs, stop and investigate before changing anything. Common causes: a different
seed, a different player name, a different `--max-moves`, a redirect that added extra output, or an
output file that already existed and was appended to.

### 5.3 Diff comparison

```bash
diff -u "$HOME/common-04-scratch/root.txt" "$HOME/common-04-scratch/elsewhere.txt"
```

`diff` prints nothing and exits `0` when the files match. An empty diff is the visual form of the
same fact.

### 5.5 What the comparison proves

The transcript does not contain the working directory. It contains the seed, the player name, the
rolls, the positions, the moves and the result. Two runs from different directories therefore have
no field in which to differ, as long as the same entry point and the same interpreter are used. The
comparison is evidence of location independence, not a coincidence.

## Step 6 — Explain the entry point (3 min)

Write one paragraph, without syntax words. Answer:

- what the shell resolves before the wrapper runs;
- why the relative path from another directory fails;
- which line of the wrapper makes the absolute-path run work;
- why the caller's working directory is not part of the transcript;
- what the lesson controls, and what it leaves uncontrolled.

A useful sentence to finish: *“The wrapper is location-independent because it resolves its own
directory; the caller's path is not.”*

Do not name a programming language, a data type, or a syntax element. If you need one to explain the
idea, the idea is not yet separated from the implementation.

## Step 7 — Record (3 min)

Fill [`EVIDENCE.md`](EVIDENCE.md). It is your portfolio entry. Include:

- the prediction, written before the run;
- the repository path, the scratch path and the interpreter version;
- the command and exit status for the root run;
- the command and exit status for the absolute run from another directory;
- the result of the relative-path control;
- the two SHA-256 digests and the result of `cmp`;
- the first differing line, if any;
- the output of the golden-trace diff;
- the explanation paragraph;
- the next step.

## Self-check (binary)

- [ ] I read `run.sh` and can explain what `scriptDirectory` does.
- [ ] I wrote the prediction before running anything.
- [ ] I ran seed 7 and player `Learner` from the repository root; the exit status was `0`.
- [ ] I ran the absolute path from another directory; the exit status was `0`.
- [ ] I ran the relative path from another directory and observed the expected failure.
- [ ] `cmp` reports the two successful transcripts as identical.
- [ ] I explained why the absolute-path run works and the relative-path run does not.
- [ ] I filled `EVIDENCE.md` and know my next step.

## Misconceptions this lesson refutes

| Misconception | Why it is wrong | What to do instead |
|---|---|---|
| “No build step means no environment.” | The interpreter, its version and the standard library are
part of the run. | Record the interpreter version with the command. |
| “If it runs from the repository root, it runs anywhere.” | A relative path depends on the working
directory. | Run the absolute path from another directory. |
| “The wrapper changes the game.” | `run.sh` only locates and executes `reference.py` ; it forwards
arguments unchanged. | Compare digests, not assumptions. |
| “A build is required for reproducibility.” | Interpreted source can be reproducible when the
toolchain and inputs are fixed. | Fix the interpreter, the arguments and the seed. |
| “Build reproducibility means identical binaries.” | Here the observable is a transcript, not a
binary. | Compare transcript bytes. |
| “The same seed is enough; the directory does not matter.” | The directory affects path resolution
before the program starts. | Run from two directories and compare. |
| “The Python version cannot matter.” | Language semantics and the standard library can change
between versions. | Record the version; use Python 3.8 or newer. |
| “An empty diff proves correctness.” | It proves identity or location independence. | Compare the
transcript with the specification and the trace. |

## Terms used here

| Term | One-line meaning |
|---|---|
| Interpreter | A program that reads source and executes it. |
| Entry point | The command that starts the program. |
| Working directory | The directory used to resolve relative paths. |
| Location independence | The entry point works from any working directory. |
| Entry-point robustness | The same command and inputs produce the same behavior from any directory.
|
| Transcript | The observable behavior of one run, one record per line. |
| Digest | A fixed-length fingerprint of a byte sequence. |

## What “done” means here

The **Self-check (binary)** above is the authoritative checklist. If every box is ticked, the
lesson is done for you. `meta.yml` mirrors the same criteria for tooling.

## Next step

- Continue to `common-05` — golden traces — and use the entry point you just verified to reproduce a
  trace.
- If you want the harness internals first, go to `common-02` — deterministic harness.
- If the absolute-path run failed, do not move on. Record the exit status and the first error before
  changing anything.

## References

- [`common/harness/run.sh`](../../harness/run.sh) — the entry point.
- [`common/harness/reference.py`](../../harness/reference.py) — the reference implementation.
- [`common/harness/README.md`](../../harness/README.md) — requirements, command line and
  determinism.
- [`common/traces/README.md`](../../traces/README.md) — verification and regeneration procedure.
- [`common/traces/seed-0007.txt`](../../traces/seed-0007.txt) — the golden trace for seed 7.
- [`common/spec/game-rules.md`](../../spec/game-rules.md) — §12 fixes the random call order.
- [`common/spec/transcript-format.md`](../../spec/transcript-format.md) — the record shapes.
- [`docs/guide/how-learning-works.md`](../../../docs/guide/how-learning-works.md) — §4.5, the
  engineering check.
- [`docs/guide/glossary.md`](../../../docs/guide/glossary.md) — the project glossary; the build
  terms above are not there yet.
