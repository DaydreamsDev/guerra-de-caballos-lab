# archaeology-04 — Evidence: correctness failures

> This is your record. Write it for your future self.
> Record what you observed, not what you expected the lesson to want.

## 1. Setup

- Date:
- Archive path:
- Tag checked out:
- Commit hash (`git rev-parse HEAD`):
- Compiler and version (`c++ --version`):
- Shell and version:
- Kernel and distribution (`uname -sr`):
- Working directory:
- Anything unexpected in the setup:

## 2. Prediction — write before reading the cards

Do not edit this table after Step 3. The gap between prediction and observation is the learning
signal; repairing the prediction destroys it.

| # | Failure | My prediction: what the code does | How I would know I am wrong |
|---:|---|---|---|
| 1 | Loop bound | | |
| 2 | Entry point | | |
| 3 | Seed in the retry loop | | |
| 4 | Pointer initialization | | |
| 5 | Name input | | |
| 6 | Win condition | | |

One falsifiable sentence about the set:

>

## 3. Test run

Command:

```bash
bash tests/run_failure_tests.sh /path/to/archive
```

Observed output:

```text
failure-001=detected
failure-003=detected
failure-004=detected
failure-005=detected
failure-002=detected
failure-006=detected
```

- Exit code:
- Compiler used by the harness:
- Archive path used:
- Did the output order match the reference? If not, why?

What `detected` means here: the test reproduced the pattern. It does not mean the archived program
was compiled, run or proven broken at runtime.

## 4. Card ledger — diagnosis

| Card | Variant:line | Symptom | Root cause | Severity | Confidence |
|---|---|---|---|---|---|
| failure-001 | | | | | |
| failure-002 | | | | | |
| failure-003 | | | | | |
| failure-004 | | | | | |
| failure-006 | | | | | |
| failure-005 | | | | | |

## 5. Card ledger — fixes and tests

| Card | Test that catches it | What the test checks | Minimal fix | Structural fix and rung |
|---|---|---|---|---|
| failure-001 | | | | |
| failure-002 | | | | |
| failure-003 | | | | |
| failure-004 | | | | |
| failure-006 | | | | |
| failure-005 | | | | |

## 6. Severity ranking

Rank the six by cost in your context. The order is not universal; the reason is the evidence.

| Rank | Card | Cost in real work | Why this rank |
|---:|---|---|---|
| 1 | | | |
| 2 | | | |
| 3 | | | |
| 4 | | | |
| 5 | | | |
| 6 | | | |

## 7. What the tests prove and do not prove

| Claim | Supported? | Why |
|---|---|---|
| The pattern exists at the cited source line. | | |
| The original program crashes or corrupts memory. | | |
| The original program is exploitable. | | |
| The proposed minimal fix removes the harm. | | |
| The proposed structural fix prevents the class. | | |
| `void main` fails on every compiler. | | |
| `cin >>` overflows for every long input. | | |

## 8. Evidence limits

Write four bullets, one per limit:

- The tests are isolated reproductions; they do not compile or run the archived source.
- The two shell checks read text; they do not execute the program.
- The source is the only witness; there is no 2008 binary or toolchain to compare.
- The intent behind a choice is inferred, not verified. Say which claims are inferences.

Add any limit specific to your environment:

-

## 9. What I would fix first

One paragraph. Name the card, the minimal fix, the test and the reason the cost is higher than the
others. Do not propose a rewrite in this paragraph.

>

## 10. Archive integrity

- `git status` in the archive:
- Did I edit `original/`? (It must be no.)
- Did I apply any patch inside the archive? (It must be no.)
- Where did I keep my scratch copy, if I made one?

## 11. Next step

- Continue to `archaeology-05` — global state and hidden coupling.
- Re-read the card for the failure you ranked last. Was the rank about cost or about how easy the
  defect was to see? Write one sentence.
- If a test printed `not-detected`, record the cause before changing the test. A failing check is
  evidence about the check first.
