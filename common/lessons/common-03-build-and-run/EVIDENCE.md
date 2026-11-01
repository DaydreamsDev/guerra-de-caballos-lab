# common-03 — Evidence: build and run

> [English](EVIDENCE.md) | [Español](EVIDENCE-es.md)

> This is your record. Write it for your future self.
> Record what you observed, not what you expected the lesson to want.

## 1. Setup

- Date:
- Repository path:
- Scratch directory:
- Shell and version:
- Output of `python3 --version`:
- Reference path:
- Expected digest for seed 7 and player Learner:
  `b8ff5f7999322ac770a5b37056e86f7eb4fe8c71ea3f4fd5c457cc720af27dba`

## 2. Prediction — write before running

- From the repository root, will the relative command exit `0`? Why?
- From `/tmp`, will the same relative command exit `0`? What will the shell say?
- From `/tmp`, will the absolute path exit `0`? Why?
- Will the two successful runs produce byte-identical transcripts? What could make them differ?
- Which line of `run.sh` makes the absolute run work from another directory?
- What would happen if `run.sh` called `python3 reference.py` without `scriptDirectory`?
- What does the wrapper’s location logic predict about the outputs, and what does it not predict?
- One falsifiable sentence:

>

## 3. Run from the repository root

- Working directory:
- Command:
- Exit status:
- Transcript path:
- `wc -l` result:
- Last two lines:

```text

```

## 4. Run from another directory

- Working directory:
- Command:
- Exit status:
- Transcript path:
- Anything unexpected:

### Relative-path control

- Working directory:
- Command:
- Exit status:
- Exact shell message:

```text

```

## 5. Compare

### 5.1 Byte comparison

- `cmp` result (IDENTICAL / DIFFERENT):
- `cmp` exit status:

### 5.2 Digest comparison

- SHA-256 from the repository root:
- SHA-256 from the other directory:
- Do the digests match? [ ] yes [ ] no
- Do both equal the expected digest? [ ] yes [ ] no

### 5.3 Diff comparison

- Was the diff between the two transcripts empty? [ ] yes [ ] no
- First differing line, if any:
- Field that differs:

### 5.4 Cross-check against the golden trace

- Trace path:
- Was the diff against the trace empty? [ ] yes [ ] no
- First differing line, if any:

### 5.5 What the comparison proves

- Which field of the transcript could have contained the working directory?
- Why does the absence of that field matter?

>

## 6. Prediction vs observation

- What matched:
- What differed, with the exact lines:
- What the gap tells me about my mental model:
- Did the relative-path control fail exactly as predicted? [ ] yes [ ] no

## 7. Explanation — why the entry point is location-independent

Write one paragraph, without syntax words. Cover:

- what a build step normally produces, and why that artifact is absent here;
- what the interpreter does instead, and when it does it;
- what the wrapper resolves before the program starts and what it leaves uncontrolled;
- why the interpreter version is part of the controls even though nothing is compiled;
- why a port to a compiled language adds a build step without changing the concept.

>

## 8. Self-check

- [ ] I read `run.sh` and can explain what `scriptDirectory` does.
- [ ] I wrote the prediction before running anything.
- [ ] I ran seed 7 and player Learner from the repository root; the exit status was `0`.
- [ ] I ran the absolute path from another directory; the exit status was `0`.
- [ ] I ran the relative path from another directory and observed the expected failure.
- [ ] `cmp` reports the two successful transcripts as identical.
- [ ] The two SHA-256 digests match, and both equal the expected digest.
- [ ] The diff between the two transcripts is empty.
- [ ] The diff against `common/traces/seed-0007.txt` is empty.
- [ ] I recorded the Python version and the exact commands.
- [ ] I can explain why the entry point is location-independent, without syntax words.
- [ ] I know my next step.

## 9. Next step

- What I can explain now:
- What I cannot explain yet:
- Next lesson or repeat:

## 10. Optional note

- Anything else:

---

> Reminder: this file is evidence for you.
