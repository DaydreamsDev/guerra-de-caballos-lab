# archaeology-17 — Code review of a legacy function

> [English](lesson.md) | [Español](lesson-es.md)

> **Status:** drafted · **Concept:** code review · **Prerequisite:** archaeology-04 ·
> **Estimated time:** 60 minutes
> **Reading:** ~21 minutes
> **Lesson role:** core

## Why this lesson exists

`archaeology-04` gave you a catalog: six failures, each with a card, a line and a test. Real review
does not work that way. Nobody hands you the list of defects. You get a function and a caller, and
you have to decide what is worth saying.

That decision has three parts, and most reviews do only the first:

1. **Observation** — what the code does. "Line 300 writes `a`."
2. **Finding** — why that matters, to whom, and how much. "The function that answers 'did someone
   win?' also ends the game, so no caller can ask the question without causing the effect."
3. **Priority** — what to fix first, given consequence and cost.

A review that stops at observation is a summary. A review that skips priority is a wish list. A
review that skips evidence is an opinion with formatting.

This lesson is the protocol for the middle: one function, at least three findings, a severity order,
and one test that would catch the highest-risk finding. The function is the unit because a file
review turns into a rewrite proposal, and a rewrite proposal is not a review.

## What you will be able to do

By the end of this lesson you can:

- choose one function worth reviewing and bound the scope in writing;
- separate an observation, a finding and a preference;
- write a finding with location, symptom, root cause, severity and evidence;
- assign severity by consequence, not by how unpleasant the code looks;
- order findings by a rule you can state, not by gut feeling;
- name one test that fails before the fix and passes after it;
- explain the smallest seam the test needs, even when the current function cannot be called;
- state what you read, what you ran and what you inferred;
- keep the legacy source read-only while you work.

The lesson is done when another reviewer can read your report and act on it without asking you
which finding matters.

## The concept: a review is a ranked set of claims

### Three artifacts that look alike

A review produces sentences, and not every sentence is a finding.

| Artifact | Example | What it lacks |
|---|---|---|
| Observation | "Line 300 assigns `a = 0`." | Consequence. It is true and inert. |
| Preference | "I would return an enum instead of printing." | Evidence of harm. It may still be right. |
| Finding | "The predicate ends the game as a side effect, so a caller cannot ask the question twice." | Nothing, if the evidence supports it. |

The preference is not forbidden. It is a different kind of claim, and it belongs in a separate
section. Mixing preferences into findings is how a review loses authority: the reader cannot tell
which comments are backed by the code and which are taste.

The test is blunt. Ask: **what happens if nobody acts on this?** If the answer is "the code looks
different from my style", it is a preference. If the answer is "a result is wrong", "a caller
breaks" or "the next change costs three files", it is a finding.

### Anatomy of a finding

Every finding in this lesson has six fields. The template has one row per finding, and the row is
the unit of review.

| Field | Question it answers | Weak | Strong |
|---|---|---|---|
| Location | Where is the claim? | "In the win code." | `pruebatriunfo`, lines 296–299. |
| Symptom | What can be observed? | "It is confusing." | The message names the machine on every win. |
| Root cause | Why does it happen? | "Bad design." | The winner is a literal; it is not derived from the side that moved. |
| Severity | What happens if nobody fixes it? | "High, it is ugly." | High: the reported result is wrong at every game end. |
| Evidence | How do I check the claim? | "Everyone knows." | Four `!= -1` checks at 296–297, message at 299. |
| Test | What would catch it? | "Write tests." | `outcome_reports_capturing_side`. |

The weak column is not a straw man. It is what a review looks like when the reviewer is in a hurry,
and it is why the person who owns the code cannot act on it.

### Severity is consequence, not ugliness

Use four levels. They are the ones the archaeology track already uses, and they are about what
happens, not about style.

| Level | Meaning | Action |
|---|---|---|
| **Critical** | Undefined behavior, data loss, security, legal blocker. | Fix before publishing. |
| **High** | Wrong behavior, non-reproducible results, a hard incident. | Fix in the minimal patch. |
| **Medium** | An expensive change or a fragile structure. | Structural fix, scheduled. |
| **Low** | Readability, naming, comments. | Improve when touching the file. |

The discipline is one sentence per finding:

> If this fires, **\<observable\>** happens to **\<who\>**, in **\<context\>**.

If you cannot fill the blanks, you have not assigned a severity; you have assigned a mood. "High"
is not a synonym for "I dislike it". A one-line label bug can be high; a 200-line style problem can
be low. Cost and difficulty are not severity either. A cheap fix and an expensive fix can carry the
same severity, and the cheap one is usually done first.

Severity is also **contextual**. The same wrong label is high in a game that reports a winner and
low in an internal demo that nobody reads. The report must name the context it prices. A severity
without a context is a number with no unit.

### Priority is not severity

Severity says how bad the consequence is. Priority says what to do first. They usually agree, and
when they disagree the reason must be written down.

Order with three rules, applied in order:

1. **Severity first.** Critical before high, high before medium, medium before low.
2. **Reachability second.** Within a level, the finding on the normal path goes before the finding
   on an edge path.
3. **Cost and certainty third.** Within a level and a path, the cheap and certain fix goes before
   the expensive and uncertain one.

Then split the list into three actions:

- **Fix first** — the top finding; the one whose test you name.
- **Fix next** — real, but not in the same change.
- **Leave as evidence** — recorded, not fixed; the reason is part of the review.

The last bucket is not a dumping ground. It is where a finding goes when the fix would change
behavior the product depends on, or when the evidence is not strong enough to justify the change.

### A test is a claim you can check

A finding says "this is wrong". A test says "here is how you would know". The second sentence is
what separates a review from a complaint.

A named test needs five parts:

| Part | Question |
|---|---|
| Name | What behavior does it protect? |
| Given | What state is set up? |
| When | What single action runs? |
| Then | What observable must hold? |
| Oracle | Why is that the correct answer, independent of the implementation? |

Two more properties matter:

- **It fails before the fix.** If the test passes on the current code, it does not catch the
  finding.
- **It passes after the fix.** If it cannot pass without a rewrite, the "minimal fix" was not
  minimal.

If the current function returns `void` and prints, you may not be able to write the test at all.
That is not a reason to skip it. It is a **testability finding**: the function has no seam where a
caller can observe the result. Name the smallest seam the test needs. In this lesson you name the
test; you do not have to implement it.

### The evidence ladder

Not every claim reaches the same rung. Say which rung each finding reached.

| Rung | What you did | What it proves |
|---|---|---|
| 1. Read | Read the function and its callers. | The code says what you quote. |
| 2. Grep | Found every use of the function, the global or the sentinel. | The claim is not local to one line. |
| 3. Compile | Built the file or a copy. | The code is accepted by a compiler; warnings are visible. |
| 4. Run | Ran the program with a known input. | The behavior happens at least once. |
| 5. Test | Ran a test that fails on the defect. | The claim is reproducible and falsifiable. |

A review that says "this is a bug" from rung 1 is not wrong, but it must say rung 1. A review that
says "this crashes" from rung 1 is overreaching. The difference is not pedantry; it is what lets the
next person reproduce your work.

### Review limits

Every review has an edge, and the edge is part of the deliverable.

- **What I read** — file, function, lines, callers.
- **What I ran** — commands, inputs, exit codes.
- **What I did not run** — and why.
- **What I inferred** — the claims that depend on a model of the program, not on an execution.

A review without limits reads as complete. A review with limits is trustworthy, because the reader
knows where to look next.

## Before you start

Read these in full:

- [`README.md`](README.md) — the lesson card and the acceptance criterion.
- [`CODE-REVIEW-TEMPLATE.md`](CODE-REVIEW-TEMPLATE.md) — the report you fill in.
- [archaeology-04](../archaeology-04-correctness-failures/lesson.md) — the prerequisite: how a
  defect is named, evidenced and tested.
- [`../../../docs/guide/archaeology-track.md`](../../../docs/guide/archaeology-track.md) — §5, the
  severity scale; §2, the autopsy protocol.
- [`../../../docs/guide/lesson-catalog.md`](../../../docs/guide/lesson-catalog.md) — the Track B
  sequence.

You also need:

- a local clone of the legacy source, read-only;
- `nl`, `grep`, `sed` and `wc`;
- optionally a C++ compiler, for the rung-3 checks;
- a scratch directory outside `original/`, for any copy you make.

> **Rule:** the legacy source is read-only. Copy any file you want to change into a scratch
> directory; never edit `original/`. This lesson does not patch the code. The test is named, not
> applied.

## The 60-minute path

| Min | Step | What you do | Evidence |
|---:|---|---|---|
| 0–10 | Inspect | Choose one function, bound the scope, read it three times | Scope card |
| 10–25 | Review | Write at least three distinct findings with severity | Findings table |
| 25–35 | Prioritize | Order by severity, reachability, cost | Fix first / next / leave |
| 35–45 | Test | Name one test for the highest-risk finding | Test contract |
| 45–55 | Explain | Write the review comment and the limits | Comment and limits |
| 55–60 | Record | Fill the review template and `EVIDENCE.md`; confirm the source is clean | Portfolio entry |

If the test design takes fifteen minutes because the function has no seam, that investigation is the
lesson. Do not shorten it to keep the schedule.

### The engineering check in this lesson

Track B uses the same habit as Track A, with a different verb: the behavior already exists, so you
**inspect** what it does before you judge it.

| Step | Where it happens | What it produces |
|---|---|---|
| **Inspect** | Step 1 | One function, one line range, one scope card. |
| **Review** | Step 2 | At least three findings, each with evidence and severity. |
| **Prioritize** | Step 3 | An order with a stated rule. |
| **Test** | Step 4 | One named test for the highest-risk finding. |
| **Explain** | Step 5 | The review comment, the non-findings and the limits. |
| **Record** | Step 6 | `EVIDENCE.md` with commands and boundaries. |

## Step 1 — Inspect (10 min)

### Choose the function

A function is worth reviewing when it carries a contract, touches state, and can be reached. Use
these five filters:

| Filter | Why it matters |
|---|---|
| One named function | A file review becomes a rewrite proposal. |
| Roughly 5–60 lines | Small enough to hold in your head; large enough to have a contract. |
| At least one global read or write | State is where hidden coupling lives. |
| At least one branch or loop | A straight line has fewer ways to be wrong. |
| Reachable from `main`, directly or through a caller | An unreachable function is a museum piece. |

Length is not risk. The worked example is five lines long and carries a wrong result, a hidden state
write and four repeated positional globals. A 60-line function with one clear contract can be
cleaner than a five-line function with three jobs.

If you already have a function in mind, use it. If you want a guided path, use the worked example:
`pruebatriunfo()` in
`original/variant-00-precursor-2007/source/ajedrez-caballo-2007.txt`, lines 296–300.

### Bound the scope

Write the scope card before you write a finding. It keeps the review from growing into the file.

| Field | Worked example |
|---|---|
| File | `original/variant-00-precursor-2007/source/ajedrez-caballo-2007.txt` |
| Function | `pruebatriunfo()` |
| Lines reviewed | 296–300 |
| Callers | The turn loop; find them with `grep -n 'pruebatriunfo'`. |
| Globals read | `tablero`, `peon1f`, `peon1c`, `peon2f`, `peon2c`, `peon3f`, `peon3c`, `peon4f`, `peon4c`. |
| Globals written | `a` (line 300). |
| Outputs | One message on `cout` (line 299). |
| Out of scope | The turn loop, the board representation, the rest of the file. |

The scope card is not paperwork. It is the answer to "why did you not mention the other 900 lines?"
The answer is "because they are out of scope, and here is the line where the scope ends."

### Read it three times

Read the same five lines three times, with a different question each time.

**Pass 1 — contract.** What does the name promise? `pruebatriunfo` promises a test: it answers
whether someone won. A test should not print, and it should not end the game. Hold that thought.

**Pass 2 — state.** What does it read? Nine globals: the board and four pairs of coordinates. What
does it write? `a`. The write is not in the name and not in the return type, because there is no
return type.

**Pass 3 — control flow.** There is one `if`, four negated sentinel checks, one message and one
assignment. No loop, no early return, no error path. The condition is true when all four pawn cells
are no longer `-1`.

Now read the callers. The question is not "is this function good?" but "who depends on the side
effect?":

```bash
grep -n 'pruebatriunfo' original/variant-00-precursor-2007/source/ajedrez-caballo-2007.txt
```

The answer tells you whether `a = 0` is the only way the game stops. If it is, the side effect is
load-bearing, and a finding that says "remove it" is wrong. A finding that says "name it" may still
be right.

### The code under review

```cpp
if( tablero[peon1f][peon1c]!=-1&&tablero[peon2f][peon2c]!=-1
&&tablero[peon3f][peon3c]!=-1&&tablero[peon4f][peon4c]!=-1 )
{
    cout<<"\n\nTRIUNFO PARA LA MIQUINA :D\n\n";   // line 299
    a=0;
}
```

Lines 296–297 hold the condition, 299 the message, 300 the assignment. Quote the lines in the
report; do not paraphrase them. A reviewer who paraphrases cannot be checked.

### What inspection does not do

Inspection does not assign severity, does not propose a fix and does not decide priority. Those are
the next three steps. Keeping them apart is what stops the first impression from becoming the whole
review.

## Step 2 — Review (15 min)

### Six probes

Run the same six questions against the function. They are ordered from contract to testability, and
each one produces at most one finding.

| Probe | Question | What it catches |
|---|---|---|
| Contract | Does the function do what its name promises? | A query that also commands. |
| Boundary | Is every index, length and sentinel checked? | Off-by-one, overflow, missing validation. |
| State | Which globals does it read and write? | Hidden coupling, order dependence. |
| Failure path | What happens on the bad input or the empty case? | Silent success, wrong default. |
| Coupling | What must change when a rule changes? | Duplicated literals, positional globals. |
| Testability | Can a caller observe the result without the console? | No seam, no oracle, no test. |

One probe can produce more than one finding, but only if the consequences are different. The next
section is the rule for that.

### Write findings, not a tour

The minimum is three findings. The maximum is six, and the maximum is not a target. A review with
three distinct findings is stronger than a review with ten overlapping ones.

**Distinctness rule:** two rows are one finding when they share a root cause and a consequence.
Split them only when acting on one would not act on the other.

The worked example has three findings:

| ID | Location | Symptom | Root cause | Severity | Evidence | Test that would catch it |
|---|---|---|---|---|---|---|
| F-01 | 296–299 | The message names the machine on every win, whoever captured the last pawn. | The winner is a literal string; it is not derived from the side that moved. | High | Four `!= -1` checks at 296–297; hard-coded message at 299. | `outcome_reports_capturing_side` |
| F-02 | 300 | Asking "did someone win?" also sets `a = 0`, ending the game. | Query and command are fused; the function has no return value. | Medium | Assignment at 300; `a` is the turn-loop control. | Not named; same seam as F-01. |
| F-03 | 296–297 | Four pawn positions are compared by hand against the literal `-1`. | The board's sentinel and the pawn set are positional globals, not a named predicate. | Medium | Four coordinate pairs and four `!= -1` comparisons. | Not named; structural. |

**F-01** is the highest-risk finding. It fires at every game end, and it produces a wrong result
that a player reads. It is high, not critical: no memory is corrupted and no data is lost. The fix
is a label decision, and the test is cheap.

**F-02** is real but contextual. Today the caller in scope wants the game to stop, so the side
effect is load-bearing. Tomorrow a second caller that only wants to ask the question will end the
game by accident. That is a fragile structure, which is medium. If the function were part of a
library, the same row would be high; the report must say which context it prices.

**F-03** is the change-cost finding. Adding a fifth pawn means editing the condition, and the
meaning of `-1` is nowhere stated. That is medium: not wrong today, expensive next time.

F-02 and F-03 do not get a named test in this review, and the report says why. F-02's consequence
is a side effect on a caller; the test would be `outcome_has_no_side_effects`, and it becomes
writable with the same seam as F-01. F-03 is structural: its check is a count of touched files, not
a runtime test. The lesson requires one test, for the highest-risk finding; it does not require one
per row. A row with no test is weaker, and saying so is part of the review.

### The finding that is not a finding

The message is in capitals, ends with a smiley and says `MIQUINA`. A reviewer may dislike all three.
None of them is a finding in this lesson, because none of them changes a result, breaks a caller or
raises the cost of a change. They are preferences, and they go in "What I would not change" or in a
separate style note.

The `cout` on line 299 is a different case. It is the mechanism by which F-01 becomes visible, so
it belongs in F-01's evidence. If you list "printing inside a predicate" as a separate finding, you
must name a consequence that F-01 does not already cover, such as "the result cannot be observed
without capturing stdout". Otherwise you have split one root cause into two rows, and the reader
will pay for the duplication.

### Check each finding against the ladder

Write the rung next to the finding.

| Finding | Rung reached | What that means |
|---|---|---|
| F-01 | 1 (read) and 2 (grep) | The condition and the label are visible; the message is the only writer of the result. |
| F-02 | 1 and 2 | The assignment is visible; the caller search shows who depends on it. |
| F-03 | 1 | The four pairs and the sentinel are visible in one line range. |

None of the three reached rung 4. That is honest, and it is enough for a review of this size. If you
compile a copy and run a game to the end, you can raise F-01 to rung 4. Say so if you do.

### A finding you can send

The report row is for the record. The comment you would send is shorter, and it has a fixed shape:

> At `pruebatriunfo`, lines 296–299, when all four pawns are captured, the message credits the
> machine. I expected the winner to be the side that made the last capture, because the function is
> the only place that decides the result. Severity: high. Test: `outcome_reports_capturing_side`.

That is one finding, one claim, one consequence, one test. It does not say "bad code", and it does
not mention the person who wrote it. It says what happens and what would prove it.

## Step 3 — Prioritize (10 min)

### Apply the three rules

| Order | Finding | Rule that decides it | Why |
|---:|---|---|---|
| 1 | F-01 | Severity | High beats medium; it is also on the normal path. |
| 2 | F-02 | Severity, then reachability | Medium; fires on the same path, but only matters when a second caller exists. |
| 3 | F-03 | Severity, then cost | Medium; structural, scheduled, not in the minimal patch. |

The order is not the same as the severity column, and that is the point. Severity sets the bands;
reachability and cost order the rows inside a band.

### Write the three actions

- **Fix first:** F-01. It changes what the player reads at the end of every game. The minimal fix
  is to derive the label from the side that moved, not to rewrite the function.
- **Fix next:** F-02. Return the outcome and let the caller stop the game. That is a seam change,
  and it makes the test in Step 4 possible.
- **Leave as evidence:** F-03. Record the four positional globals and the unnamed sentinel. Do not
  replace the board in this review; that is a structural change with its own price.

### The counter-order

If this function were called by a second component that only asks "is the game over?", F-02 would
become high and go first. The wrong label would still be wrong, but the accidental game stop would
break a caller that has nothing to do with winning.

Write the counter-order down. It shows that your priority is a decision with reasons, not a
transcription of the severity column.

### What priority is not

Priority is not "what is easiest". The easiest fix can be the least important. Priority is not "what
annoys me most", either. If the review's order cannot be defended in one sentence per row, the order
is a preference wearing a number.

## Step 4 — Test (10 min)

### Name the test before you design the fix

The highest-risk finding is F-01. Its test is:

```text
outcome_reports_capturing_side
```

The name states the behavior, not the implementation. It does not mention `pruebatriunfo`, `cout` or
the message string, so the test survives a rename and a refactor.

### Fill the test contract

| Part | Content |
|---|---|
| Name | `outcome_reports_capturing_side` |
| Given | A board where all four pawn cells differ from `-1`, and a recorded last mover. |
| When | The outcome is evaluated once for the player as last mover, once for the machine. |
| Then | The first result is a player win; the second is a machine win. |
| Oracle | The winner is the side that made the last capture, not a fixed label. |
| Fails before | The first assertion fails: both win cases print the machine label. |
| Passes after | The label is derived from the last mover. |
| Runtime | No console, no sleep, no random seed; two win cases plus one control. |

The oracle is the part reviewers skip. "The result is correct" is not an oracle; it is a hope.
Here the oracle is a rule from the game, stated independently of the code: the side that captures
the last pawn wins. The test encodes the rule, not the current output.

### The seam the test needs

The current signature cannot express the test. The function returns `void`, prints to `cout` and
writes a global. A caller cannot ask for the result without causing the effect.

The smallest seam is a value-returning predicate:

```cpp
enum class Outcome { Playing, PlayerWins, MachineWins };
Outcome evaluate_outcome(const Board& board, Side last_mover);
```

That is a sketch, not a patch. The review does not apply it. The point is to show that the test is
writable, and to price the seam: one enum, one function signature, one caller update.

If the seam feels like a rewrite, the finding is bigger than it looked. Say so. A test that requires
a redesign is evidence that F-02 is not medium in your context.

### The test sketch

```cpp
// test: outcome_reports_capturing_side
CHECK(evaluate_outcome(board_with_all_pawns_captured(), Side::Player)
      == Outcome::PlayerWins);
CHECK(evaluate_outcome(board_with_all_pawns_captured(), Side::Machine)
      == Outcome::MachineWins);
CHECK(evaluate_outcome(board_with_pawns_present(), Side::Player)
      == Outcome::Playing);
```

Three assertions, one behavior. The third one is the control: it proves the test does not report a
win for every board. A test without a negative case can pass for the wrong reason.

### Why this test catches F-01

On the current logic, both win cases produce the machine label. The first assertion fails. After the
label is derived from `last_mover`, the first assertion passes and the second still passes. The test
is red before and green after, which is the only sequence that proves it tests the finding.

If you cannot run the test in this lesson, say that. Naming it is the acceptance criterion; running
it is the next lesson's work. Do not claim a green run you did not have.

## Step 5 — Explain (10 min)

### The review is a conversation, not a verdict

A review comment has four jobs: locate, describe, justify, and propose. The formula from Step 2 does
all four in five sentences. Keep it short enough that the reader can reply to one claim.

Blameless does not mean consequence-free. The code can be wrong, and the review must say so. What
it does not do is explain the defect through the person who wrote it. "This label is hard-coded" is
a claim about the code. "They did not think" is a claim about a person, and it is not reviewable.

### State the context you priced

Severity is contextual, so the report names the context:

- **Product:** a game whose result is read by a player. A wrong winner is high.
- **Internal demo:** nobody reads the message. The same defect is low.
- **Library:** a second caller can trigger the side effect. F-02 becomes high.

If the context is not stated, the reader will silently substitute their own, and the priority will
look arbitrary.

### What I would not change

The template has a section for behavior that looks odd but is required. Fill it. In the worked
example:

- The `a = 0` side effect is how the game stops. Removing it without moving the stop to the caller
  would leave the loop running.
- The four pawn coordinates are the current representation. Replacing them is a structural change,
  not part of this review.
- The message text is evidence of F-01, not a style finding.

This section is what keeps a review from becoming a rewrite. It also protects the reviewer: a
finding that says "this is intentional" is as valuable as one that says "this is wrong".

### Review limits

Write the four lines:

- **Read:** the function, its line range and its callers.
- **Ran:** the grep commands; optionally a compile of a copy.
- **Did not run:** the game, the test, the program build.
- **Inferred:** the last mover's identity and the intended winner, from the surrounding turn logic.

The last line matters. The code has no player model, so "the player should win" is an inference from
the game's rules, not a fact in the function. A review that hides the inference is overclaiming.

## Step 6 — Record (5 min)

Fill [`CODE-REVIEW-TEMPLATE.md`](CODE-REVIEW-TEMPLATE.md) with the scope, the findings, the
priority, what you would not change and the review limits. Then fill [`EVIDENCE.md`](EVIDENCE.md).
At minimum:

- setup: source path, compiler, shell, terminal, scratch directory;
- the scope card: file, function, lines, callers, globals, out of scope;
- the findings table with at least three rows and a rung per row;
- the severity sentence for each finding: who is hurt, in which context;
- the priority order and the rule that produced it;
- the test contract for the highest-risk finding, including the oracle and the seam;
- the non-findings and the behaviors you would not change;
- the review limits: read, ran, did not run, inferred;
- the source integrity check: `original/` unmodified.

A sentence such as "the function is badly designed" is an opinion. "Lines 296–299 print the machine
label for both last movers; the winner is a literal; the test `outcome_reports_capturing_side` fails
on the player case and passes after the label is derived from `last_mover`" is a record.

The acceptance criterion is three facts:

```text
findings=3
severity_order=yes
test=outcome_reports_capturing_side
```

If you cannot produce all three, the missing line names the next step. There is no penalty for
repeating one.

## The original in detail

Facts you can cite, from
`original/variant-00-precursor-2007/source/ajedrez-caballo-2007.txt`:

| Location | What is there |
|---:|---|
| 296–297 | The condition: four `tablero[peonNf][peonNc] != -1` checks, joined by `&&`. |
| 299 | `cout << "\n\nTRIUNFO PARA LA MIQUINA :D\n\n";` — the hard-coded winner. |
| 300 | `a = 0;` — the state write that ends the game. |

The condition is true when all four pawn cells are no longer `-1`. The message is a literal, so it
does not depend on which side moved. The assignment is the only write in the function, and it is not
visible in the signature.

Related anchors you may need:

| Topic | Where to look | Why |
|---|---|---|
| The turn loop | The caller of `pruebatriunfo` | Shows what `a` controls. |
| The pawn sentinel | The board declaration and the capture code | Shows what `-1` means. |
| The capture path | The move logic | Shows who can capture the last pawn. |

Do not copy a line number from a summary. Re-run `nl -ba` and quote what is there. A review that
cites a stale line is worse than a review with no line, because it looks checkable and is not.

## What the review proves and does not prove

| Claim | Supported? | Why |
|---|---|---|
| The condition and the label are at 296–299. | Yes | The quoted lines. |
| The label does not depend on the last mover. | Yes | It is a string literal. |
| The function writes `a` at line 300. | Yes | The assignment is visible. |
| The game reports the wrong winner. | Yes, if the rules say the capturer wins | The inference is stated. |
| The test passes after the fix. | No | The test is named, not implemented. |
| The minimal fix is safe for every caller. | No | Only the callers you read are covered. |
| The function is the only defect in the file. | No | The scope is one function. |
| The review is complete. | No | It is one pass at one rung. |

This table is the honesty check. A review that says "the winner is wrong" without naming the rule is
overreaching. A review that says "the label is hard-coded and the rules say the capturer wins" is
defensible.

## Self-check (binary)

- [ ] I chose one function and wrote its file, line range and callers.
- [ ] I read the function three times: contract, state, control flow.
- [ ] I wrote the scope card before the findings.
- [ ] I wrote at least three distinct findings.
- [ ] Each finding has location, symptom, root cause, severity and evidence.
- [ ] Each severity has a sentence: who is hurt, in which context.
- [ ] I separated at least one preference from the findings.
- [ ] I ordered the findings with the three rules and wrote the reason per row.
- [ ] I named the fix-first, fix-next and leave-as-evidence rows.
- [ ] I named one test for the highest-risk finding.
- [ ] The test has given, when, then and an oracle independent of the implementation.
- [ ] I stated whether the test fails before the fix and passes after it.
- [ ] I named the smallest seam the test needs.
- [ ] I recorded the evidence rung for every finding.
- [ ] I wrote what I read, ran, did not run and inferred.
- [ ] I named at least one behavior I would not change, with a reason.
- [ ] The source is unmodified and my scratch copy is outside `original/`.

If a box is not ticked, the unticked box names the next step. There is no penalty for repeating one.

## Misconceptions this lesson refutes

| Misconception | Why it is wrong | What to do instead |
|---|---|---|
| "A review lists everything wrong." | A list without priority cannot be acted on. | Rank the findings and name the first fix. |
| "Severity is how bad the code looks." | Severity is consequence in a context. | Write the who/what/where sentence. |
| "More findings means a better review." | Overlapping rows hide the real risk. | Three distinct findings beat ten duplicates. |
| "A test is optional in a review." | The test is how the finding becomes checkable. | Name one test for the top finding. |
| "If I cannot test it, I skip the test." | The missing seam is itself a finding. | Name the seam the test needs. |
| "Priority equals severity." | Reachability and cost reorder rows inside a band. | State the three rules and apply them. |
| "Style comments are findings." | They have no consequence. | Put them in a separate section. |
| "Blameless means no severity." | Blameless is about the person, not the code. | Judge the decision, price the consequence. |
| "The reviewer should fix it." | A review and a patch are different acts. | Name the smallest change; let the patch be reviewed. |
| "One pass is a complete review." | It is one pass at one rung. | State the limits. |

## Terms used here

| Term | One-line meaning |
|---|---|
| Observation | A true statement about the code with no consequence attached. |
| Finding | A claim that something harms a caller, a result or a change. |
| Preference | A style or design opinion without evidence of harm. |
| Severity | The consequence if nobody fixes the finding. |
| Priority | The order in which findings should be fixed. |
| Reachability | How often the normal path reaches the code. |
| Root cause | The decision that produces the symptom. |
| Evidence rung | How far the claim was checked: read, grep, compile, run, test. |
| Oracle | The rule that says what the correct result is, independent of the code. |
| Seam | The smallest boundary where a caller can observe or replace behavior. |
| Testability | Whether a caller can check the result without the console or a global. |
| Review limit | The line between what was checked and what was inferred. |
| Blameless | Judge the decision, not the person. |

## What “done” means here

The **Self-check (binary)** above is the authoritative checklist. If every box is ticked, the
lesson is done for you. `meta.yml` mirrors the same criteria for tooling.

## Next step

- Continue to `archaeology-18` — refactor workshop. The review names the smallest change; the
  workshop applies it and proves the traces still match.
- If the test seam interested you, go to `archaeology-11` — input and output. The missing return
  value is the same problem as a core that cannot run without a console.
- If you want to practice severity, review the same function in a second context: a library, a demo
  and a product. Write the three severity tables and compare them.
- If you disagree with the order, write the context that changes it. A review without a context is a
  review without a price.

## References

- [`README.md`](README.md) — the lesson card and the acceptance criterion.
- [`CODE-REVIEW-TEMPLATE.md`](CODE-REVIEW-TEMPLATE.md) — the report you fill in.
- [`EVIDENCE.md`](EVIDENCE.md) — the portfolio record you fill in.
- [`../../../docs/guide/archaeology-track.md`](../../../docs/guide/archaeology-track.md) — §2, the
  autopsy protocol; §5, the severity scale; §6, tone and ethics.
- [archaeology-04](../archaeology-04-correctness-failures/lesson.md) — the prerequisite: naming,
  evidencing and testing a defect.
- [archaeology-11](../archaeology-11-input-output/lesson.md) — the seam between logic and console.
- [archaeology-14](../archaeology-14-tests-determinism/lesson.md) — tests, seeds and golden traces.
- [archaeology-18](../archaeology-18-refactor-workshop/lesson.md) — applying the minimal change and
  proving the traces still match.
