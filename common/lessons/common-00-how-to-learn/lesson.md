# common-00 — How to learn (cómo aprender)

> [English](lesson.md) | [Español](lesson-es.md)

> **Status:** drafted · **Concept:** metacognition (metacognición) · **Prerequisites:** none ·
> **Estimated time:** 15 minutes
> **Reading:** ~11 minutes

## Why this lesson exists

Every other lesson in this laboratory asks you to build something and check it. This one asks a
different question: **do you know what you know, and how would you find out?**

That question is not philosophy. It is the skill that decides whether the next hour is useful. A
learner who cannot tell "I understood the explanation" from "I can predict the behavior" will keep
re-reading the same page and keep being surprised by the same failures. A learner who can tell the
difference knows exactly where to spend the next effort.

The tool is small and unglamorous: **predict → run → compare → explain → record**. You will run it
once, on a task small enough to finish in minutes, and then write a self-diagnosis with four
questions. The gap between what you expected and what happened is the whole point.

No grade, no exam, no deadline, no group. Nothing here is scored, and no one approves it.

## What you will be able to do

By the end of this lesson you can:

- run one complete predict → run → compare → explain → record cycle on a small task;
- name the exact gap between your prediction and the observation, instead of "it mostly matched";
- explain the gap by naming the wrong assumption behind it;
- answer the four self-diagnosis questions and turn them into one next step;
- distinguish a self-diagnosis from a self-grade.

## The concept: metacognition (metacognición)

**Metacognition** is knowledge about your own knowledge: what you can do without help, what you only
recognize when you see it, and what you are guessing at. Its practical form is **calibration** — how
close your predictions are to reality.

| Idea | Meaning in this lesson |
|---|---|
| **Metacognition** | Knowing what you know, and how you would find out. |
| **Calibration** | The size of the gap between what you predicted and what happened. |
| **Prediction** | A falsifiable statement about observable behavior, written before the run. |
| **Gap** | The exact lines or values where the prediction and the observation disagree. |
| **Explanation** | The wrong assumption that produced the gap, named in plain language. |
| **Self-diagnosis** | Four questions that end in a decision, not in a score. |

Two things follow from this and they are easy to mix up:

- A **small** gap means your mental model works for this task. It does not mean you are "good".
- A **large** gap means you found the exact place to study next. It does not mean you failed.

The gap is information. It is the only feedback this method needs.

### The cycle

The five steps are the same ones used in every artifact lesson. Here they are cheap enough to run in
minutes.

| Step | What you do | What you keep |
|---|---|---|
| **Predict** | Write what will happen, before running anything. | A falsifiable sentence. |
| **Run** | Execute the task exactly as written, with the output visible. | The command and its output. |
| **Compare** | Put the prediction next to the observation. | The exact differing line or value. |
| **Explain** | Say why the gap exists; name the wrong assumption. | One paragraph, without syntax words. |
| **Record** | Save prediction, command, output and explanation. | The entry in `EVIDENCE.md`. |

It is not an exam. It is open book, untimed, ungraded, and the observed behavior is the answer.

### The four questions

The self-diagnosis comes from
[`docs/guide/how-learning-works.md`](../../../docs/guide/how-learning-works.md) §4.4. Four
questions, no levels, no score:

1. **Can I explain it?** Without notes, without syntax words.
2. **Can I change it?** On demand, with a measured cost.
3. **Can I extend it?** To a requirement I have not seen.
4. **Did my prediction match?** If not, which part of my mental model was wrong?

If any answer is "not yet", the next step is explicit: repeat the task, reread the worked example,
or go back to the prerequisite. In a fifteen-minute lesson, "not yet" on questions 2 and 3 is normal
and informative; they describe work that takes longer than one cycle.

## Before you start

Read these sections in full:

- [`docs/guide/how-learning-works.md`](../../../docs/guide/how-learning-works.md) §4 — verification
  without grades, especially §4.4 (self-diagnosis) and §4.5 (the engineering check).
- [`docs/guide/verification.md`](../../../docs/guide/verification.md) §4 and §5 — the cycle and the
  four questions as the project uses them.

You also need:

- a terminal and about fifteen minutes;
- one small task whose output you can observe;
- [`EVIDENCE.md`](EVIDENCE.md) open in an editor;
- optionally, [`common/harness/reference.py`](../../harness/reference.py) for the worked example.

"Small" is a requirement, not a suggestion. If the task takes more than five minutes to run, it is
too big for one cycle. Split it until the observation fits on one screen.

## The 15-minute path

| Min | Step | What you do | Evidence |
|---|---|---|---|
| 0–2 | Choose | Pick one small task; write down what it currently does. | Task note |
| 2–5 | Predict | Write a falsifiable prediction with a reason. | Prediction sentence |
| 5–7 | Run | Execute once; capture the command and output. | Command + output |
| 7–10 | Compare | Name the exact differing line or value. | The gap |
| 10–12 | Explain | Name the wrong assumption behind the gap. | One paragraph |
| 12–15 | Record | Fill `EVIDENCE.md`; answer the four questions; decide next step. | Portfolio entry |

The times are a suggestion, not a deadline. If the cycle takes twenty minutes, the lesson still
worked.

## Step 1 — Choose one small task (2 min)

Pick a task with three properties:

- it runs in under five minutes;
- its output is observable as text, a number, or an exit status;
- you are not completely sure what it will do.

That last property is the one people skip. If you already know the answer, the cycle teaches
nothing; you will write a prediction, watch it come true, and learn that you already knew something.
That is not a waste, but it is not the exercise.

### Worked example: the trap turn

Use this to see the shape of a cycle. Do not copy it as your own; run your own.

**Task.** Count the turns and the successful moves in one golden trace, and predict the relation
between the two numbers before counting.

**Prediction.** The trace has `moves=21`, and every successful move happens on a turn, so the file
should contain twenty-one `turn=` lines. Reason: one turn per successful move, no exceptions.

**Run.**

```bash
grep -c '^turn=' common/traces/seed-0001.txt
grep -c '^move=' common/traces/seed-0001.txt
tail -3 common/traces/seed-0001.txt
```

**Observation.** The counts are `22` and `21`. The last three lines are a `turn=` line, a
`no_legal_move trapped=player` line, and `winner=machine`.

**The gap.** One more turn than moves. The prediction assumed every turn ends in a successful move.

**Explanation.** The game ends on a trap: the active knight has no legal move, so the turn is
recorded — including its `order=` field — but no `move=` line follows. The turn still consumed a
`start` draw, because §6 draws the order before the moves are tried, and the trap is discovered
after that draw. My model said "turn implies move"; the rules say "turn implies attempt, and
attempts can all fail".

**Record.** Prediction, two commands, the two counts, the last three lines, the paragraph.

That is a complete cycle. Notice what made it work: the prediction was specific enough to be wrong
(`21`), and the observation was small enough to explain completely (`22`).

### A menu, if you want one

Pick one, or bring your own:

| Task | What to predict | How you observe |
|---|---|---|
| Header shape | How many header records a trace has, and their order | `head -7` on a trace |
| Move order | The `order=` field when the draw is 5 | `game-rules.md` §6, then a trace |
| Name effect | Whether changing only the player name changes the game | Two runs, one `diff` |
| Checksum message | The exact wording of a passing checksum line | `sha256sum -c checksums.txt` |
| Your own tool | The output, exit status, or message of a command you use often | Run it |

The last row is the most useful in the long run, and the least dependent on this repository.

## Step 2 — Predict (3 min)

Write the prediction **before** you run anything. Use this shape:

```text
I expect <observable value or line>.
Because <the rule or mechanism I believe applies>.
```

The second line is what makes the gap explainable later. A prediction without a reason can only tell
you that you were wrong; with a reason, it tells you *what* was wrong.

A prediction is falsifiable when someone else could disagree with it. "It will work" is not
falsifiable. "The file will contain forty `turn=` lines because each move occupies one turn" is.

Do not run the task while writing the prediction. Do not look at the answer "just to check". The
value of the cycle is in the gap; peeking deletes the gap.

## Step 3 — Run (2 min)

Run the task once, exactly as you described it, and capture the output. Not a summary of the output
— the output.

```bash
<your command> 2>&1 | tee "$HOME/common-00-run.txt"
echo "exit=$?"
```

`tee` keeps what you saw. The exit status matters as much as the text: a command can print a helpful
message and still fail.

If the run produced more than a screen of output, you picked too large a task. Do not analyze it;
shrink the task and run again.

## Step 4 — Compare (3 min)

Put the prediction and the observation side by side. Name the difference precisely:

- a value, with both numbers: predicted `21`, observed `22`;
- a line, quoted exactly, with the diff marker that shows which side it came from;
- an exit status, with both values;
- an absence: "I expected a line and there was none".

"Mostly matched" is not a comparison. Neither is "close enough". The gap is either zero or it has a
location, and finding the location is the skill this lesson trains.

If the prediction matched exactly, do not stop — **sharpen it**. Add a second claim: the exact
count, the exact final line, the exit status, the message wording. Then run again. A prediction that
always comes true because it says little is not calibration; it is a tautology.

### Why a gap is the deliverable

The acceptance criterion of this lesson asks for one recorded gap. That is deliberate. A cycle with
no gap has not tested your model; it has confirmed that you chose something you already knew. The
useful output of the cycle is the sentence that starts with *"I assumed …"*.

## Step 5 — Explain (2 min)

Write one paragraph, without syntax words, that does three things:

1. states the assumption the prediction relied on;
2. states what the observation shows instead;
3. states the rule or mechanism that makes the observation correct.

For the worked example: *"I assumed a turn always ends in a successful move. The observation shows
one more turn than move. The rules draw a move order for every turn and then try the moves; when
none is legal, the turn is recorded and the game ends, so the last turn exists without a move."*

Do not name a programming language, a data type, or a syntax element. If you need one, the idea is
still attached to the surface.

### The difference between explaining and excusing

- **Explanation:** "I assumed X; the rules say Y; that is why the number differs."
- **Excuse:** "It is a weird edge case." / "The output is confusing." / "I would have seen it with
  more time."

An excuse closes the gap. An explanation converts it into a rule you can use next time.

## Step 6 — Record and self-diagnose (3 min)

Fill [`EVIDENCE.md`](EVIDENCE.md). It is your record, not a submission. Then answer the four
questions in writing, in this order:

1. **Can I explain it?** Say the explanation out loud, without notes. If you reach for a note, write
   "not yet" and name the part you lost.
2. **Can I change it?** Name one change you could make and what it would cost: how many files, how
   many lines, how many concepts.
3. **Can I extend it?** Name one requirement you have not seen, and say whether you could attempt it
   now.
4. **Did my prediction match?** If not, quote the wrong assumption from Step 5.

Then write the next step. It must be one of these four shapes:

- repeat this cycle on a sharper prediction;
- reread the specific rule that surprised you;
- go to the lesson that teaches the concept behind the gap;
- move on, because the gap is closed and the explanation survives without notes.

### Self-diagnosis is not self-grading

There is no score, no level, no percentage, and no comparison with anyone. The four questions
produce a decision: what to do next. "Not yet" is a location, not a verdict. Answering "not yet"
honestly is the correct use of the tool; answering "yes" to look good defeats it.

## Self-check (binary)

- [ ] I chose one task that runs in under five minutes and whose output is observable.
- [ ] I wrote a prediction with both a value and a reason, before running.
- [ ] I ran the task once and captured the command, the output and the exit status.
- [ ] I named the gap by its exact value or line, not as "mostly matched".
- [ ] I recorded at least one gap between prediction and observation.
- [ ] I explained the gap by naming the wrong assumption.
- [ ] I answered all four self-diagnosis questions in writing.
- [ ] I recorded one next step, in one of the four allowed shapes.
- [ ] I can say why this was a diagnosis and not a grade.

If a box is not ticked, the lesson is not yet done **for you**. The unticked box names the next
step.

## Misconceptions this lesson refutes

| Misconception | Why it is wrong | What to do instead |
|---|---|---|
| "Predicting is guessing." | A prediction states an expected observable and the rule behind it; a guess states neither. | Write the value and the reason. |
| "A wrong prediction means I failed." | The gap is the feedback; it locates the wrong assumption. | Record the gap and explain it. |
| "A correct prediction means I learned it." | It may mean the task was too easy to test anything. | Sharpen the prediction until it can fail. |
| "Self-diagnosis is self-grading." | It has no score, no level and no comparison; it produces a next step. | Answer honestly and decide the next action. |
| "I can fix the prediction after seeing the result." | That deletes the only signal the cycle produces. | Write the prediction first and keep it unchanged. |
| "I understood it because the explanation made sense." | Recognition is not recall, and neither is prediction. | Explain it without notes, then predict behavior. |
| "A bigger task gives better evidence." | A large task hides the gap inside noise. | Shrink the task until the output fits one screen. |
| "Not yet means go back to the beginning." | It names one specific missing piece. | Repeat only the step that the gap points to. |

## Terms used here

| Term | One-line meaning |
|---|---|
| Metacognition | Knowledge about your own knowledge and how to test it. |
| Calibration | How close your predictions are to what actually happens. |
| Prediction note | The expected behavior, written before running the artifact. |
| Gap | The exact place where prediction and observation disagree. |
| Self-diagnosis | Four questions that end in a decision, not a score. |
| Engineering check | Predict, run, compare, explain, record. |
| Feedback literacy | Judging work against criteria and acting on evidence. |

The canonical definitions are in [`glossary.md`](../../../docs/guide/glossary.md).

## What “done” means here

The **Self-check (binary)** above is the authoritative checklist. If every box is ticked, the
lesson is done for you. `meta.yml` mirrors the same criteria for tooling.

## Next step

- If the gap is closed and you can explain it without notes, move to `common-01` — the game as an
  invariant — and use the cycle on a task with a real specification behind it.
- If the gap pointed at a rule you misread, reread that section and repeat the cycle on a sharper
  prediction before moving on.
- If you could not explain the task without notes, repeat the cycle with a smaller task. Shrinking
  is a technique, not a retreat.
- If you answered "not yet" to questions 2 or 3, that is expected in fifteen minutes. Record it and
  treat it as the next lesson's job, not as unfinished business here.

## References

- [`docs/guide/how-learning-works.md`](../../../docs/guide/how-learning-works.md) — §4.4
  self-diagnosis and §4.5 the engineering check.
- [`docs/guide/verification.md`](../../../docs/guide/verification.md) — evidence and the four
  questions.
- [`docs/guide/conceptual-framework.md`](../../../docs/guide/conceptual-framework.md) — the
  four-layer framework behind the method.
- [`common/traces/seed-0001.txt`](../../traces/seed-0001.txt) — the trace used in the worked
  example.
- [`common/spec/game-rules.md`](../../spec/game-rules.md) — §6 and §10 explain the trap turn.
- [`docs/guide/glossary.md`](../../../docs/guide/glossary.md) — canonical terms.
