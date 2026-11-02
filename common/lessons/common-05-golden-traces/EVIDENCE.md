# common-05 — Evidence: golden traces
> [English](EVIDENCE.md) | [Español](EVIDENCE-es.md)

> This is your record. Write it for your future self.
> Record what you observed, not what you expected the lesson to want.

## 1. Setup

- Date:
- Trace directory:
- Working directory for the checksum command:
- Reproduced seed:
- Player name:
- Scratch path:
- Shell used (name and version):

## 2. Prediction — write before running

- If the checksum check passes, what exactly have I proven about the traces?
- What will the passing output say, word for word, on this machine, and what will the exit status
  be?
- Will the reproduced seed produce an empty diff? How many header records will it have?
- What happens when the checksum command runs from the repository root, and why?
- Will reproducing with a different player name produce an empty diff? Which lines will differ?
- What could be wrong with the implementation and still pass all twenty traces?
- One falsifiable sentence:

>

## 3. Verify the checksum set

- Command:
- Working directory:
- Full output (paste it, including the localized wording):

```text

```

- Line count of the output (`sha256sum -c checksums.txt | wc -l`):
- Exit status:
- Any line that did not report a match:

### 3.1 The directory trap

- Command run from the repository root:
- Observed output:
- Exit status:
- My explanation of the failure, in one sentence:

## 4. Reproduce one trace

- Golden trace path:
- Reproduction command:
- Diff output (empty, or the exact lines):

```text

```

- Diff exit status:
- `cmp` result (IDENTICAL / DIFFERENT):
- SHA-256 of the golden trace:
- SHA-256 of my reproduced transcript:
- First line of `checksums.txt`, for comparison:
- Do the three digests agree? [ ] yes [ ] no

### 4.1 What each check established

- What the checksum check proves:
- What the reproduction proves:
- What the format check proves:
- Which of the three would fail first if the implementation changed one move:

## 5. Negative control

- Input changed (seed, name, or other):
- Command:
- Diff exit status:
- The exact line or lines that differ:

```text

```

- What the diff isolates:
- Did the observed control match my prediction? [ ] yes [ ] no

## 6. Compare prediction and observation

- What matched:
- What differed, with the exact lines:
- What the gap tells me about my mental model:

## 7. Explanation — why a trace is evidence only with the command and the output

Write one paragraph, without syntax words. Cover:

- what the checksum proves and what it does not prove;
- what the reproduction proves and what it does not prove;
- why the command is part of the evidence and not just a way to produce it;
- what would have to be added to the set to cover a draw.

>

## 8. Limits of this evidence

- Seeds covered:
- Endings covered:
- Endings not covered:
- One defect that would survive this trace set:
- What the specification decides when a trace and an implementation disagree:

## 9. Self-check

- [ ] I read `checksums.txt`, one full trace and the trace README.
- [ ] I wrote the prediction before running any command.
- [ ] The checksum check reported twenty successful lines from `common/traces`.
- [ ] I recorded the exact wording and the exit status, not just "it passed".
- [ ] I explained why the same command fails from the repository root.
- [ ] The reproduced trace gave an empty diff against its golden file.
- [ ] I recorded the command, the empty output and the exit status.
- [ ] The reproduced digest equals the recorded digest.
- [ ] I produced a non-empty diff with one changed input and named the differing line.
- [ ] I can state what the trace set does not prove, without syntax words.
- [ ] I know my next step.

## 10. Next step

- What I can explain now:
- What I cannot explain yet:
- Next lesson or repeat:

## 11. Optional note

- Anything else:

---

> Reminder: this file is evidence for you. A trace without its command
> and observed output is a specimen, not proof.
