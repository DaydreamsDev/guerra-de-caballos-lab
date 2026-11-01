# archaeology-17 — Evidence: code review of a legacy function

> This is your record. Write it for your future self.
> Record what you observed, not what you expected the lesson to want.

## 1. Setup

- Source path:
- Function under review:
- Compiler and version (if used):
- Shell and version:
- Terminal and `TERM`:
- Working directory:
- Scratch directory used:
- Anything unexpected in the setup:

## 2. Scope

| Field | Value |
|---|---|
| File | |
| Function | |
| Lines reviewed | |
| Callers | |
| Globals read | |
| Globals written | |
| Outputs | |
| Out of scope | |

Why I chose this function:

>

How I found the callers:

```bash
grep -n '<function>' <file>
```

Observed callers:

## 3. Inspection notes

Contract, in one sentence:

>

State read:

State written:

Control flow, in one sentence:

Branches and loops:

Early exits and error paths:

What the name promises that the body does not do:

What the body does that the name does not promise:

## 4. Findings

At least three rows. Every row has all six fields. Use the severity scale: critical, high, medium
or low.

| ID | Location | Symptom | Root cause | Severity | Evidence | Test that would catch it |
|---|---|---|---|---|---|---|
| F-01 | | | | | | |
| F-02 | | | | | | |
| F-03 | | | | | | |
| F-04 (optional) | | | | | | |

Severity sentence for each row. Fill the blanks: if this fires, **\<observable\>** happens to
**\<who\>**, in **\<context\>**.

- F-01:
- F-02:
- F-03:

The context I priced:

>

Preferences I separated from the findings:

| Preference | Why it is not a finding |
|---|---|
| | |

A row I merged into another, and why:

>

## 5. Evidence rungs

| Finding | Rung reached (read, grep, compile, run, test) | What that rung proves |
|---|---|---|
| F-01 | | |
| F-02 | | |
| F-03 | | |

Commands I ran, with their purpose:

```bash
nl -ba <file> | sed -n '<start>,<end>p'
grep -n '<function>' <file>
```

- What I compiled, if anything:
- What I ran, if anything:
- What the output showed:

## 6. Prioritization

The three rules, applied in order: severity, then reachability, then cost and certainty.

| Order | Finding | Rule that decides it | Why |
|---:|---|---|---|
| 1 | | | |
| 2 | | | |
| 3 | | | |

- Fix first:
- Fix next:
- Leave as evidence:
- Why the leave-as-evidence row is not fixed now:

The counter-order, and the context that would produce it:

>

## 7. Test contract

The highest-risk finding is:

The test name:

| Part | Content |
|---|---|
| Name | |
| Given | |
| When | |
| Then | |
| Oracle | |
| Fails before the fix | |
| Passes after the fix | |
| Runtime | |

The seam the test needs:

```cpp

```

- Does the current signature allow the test? If not, what is missing?
- Is the seam a minimal change or a redesign? How do I know?
- Does the test have a negative case, so it cannot pass for the wrong reason?

The test sketch, if I wrote one:

```cpp

```

- Did I run it? If not, why not?
- What would make it fail for the right reason?

## 8. What I would not change

| Behavior or structure | Why I keep it |
|---|---|
| | |
| | |

- What I need more evidence for:
- What looks like a defect but is required by a caller:

## 9. Review limits

- What I read:
- What I ran:
- What I did not run:
- What I inferred:
- What would raise the top finding to the next rung:

## 10. Source integrity

- Did I edit `original/`? (It must be no.)
- Did I apply any patch inside the source tree? (It must be no.)
- Where did I keep my scratch copy?
- Did I modify the lesson code? (It must be no.)

## 11. Acceptance checklist

- [ ] I chose one function and recorded its file, line range and callers.
- [ ] I wrote the scope card before the findings.
- [ ] I wrote at least three distinct findings.
- [ ] Each finding has location, symptom, root cause, severity, evidence and a test field.
- [ ] Each severity has a consequence sentence with a context.
- [ ] I separated at least one preference from the findings.
- [ ] I ordered the findings with the three rules and a reason per row.
- [ ] I named fix-first, fix-next and leave-as-evidence.
- [ ] I named one test for the highest-risk finding.
- [ ] The test has given, when, then and an oracle.
- [ ] I stated whether the test fails before the fix and passes after it.
- [ ] I named the seam the test needs.
- [ ] I recorded the evidence rung for every finding.
- [ ] I wrote the review limits: read, ran, did not run, inferred.
- [ ] I recorded one behavior I would not change, with a reason.
- [ ] The legacy source is unmodified.

## 12. Next step

- Continue to `archaeology-18` — refactor workshop.
- If the test seam interested me, go to `archaeology-11` — input and output.
- If I want to practice severity, review the same function in a second context.
- If I disagree with the order, write the context that changes it.
