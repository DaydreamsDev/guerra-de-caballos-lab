# Verification and evidence (verificación y evidencia)
> [English](verification.md) | [Español](verification-es.md)

> Nothing is graded. Nothing is ranked. The only question is: **can you demonstrate the concept with
> a working artifact that meets its acceptance criteria?**

The learning method and its rationale are in [How learning works](how-learning-works.md). This guide
covers verification only.

There are no exams, no quizzes, no points, no levels, no partial credit, and no teacher required.

---

## 1. The stance

| We do not do | We do instead |
|---|---|
| Exams | Build the artifact and run it |
| Quizzes | Predict, run, compare with the trace |
| Grades | Binary acceptance criteria |
| Ranking | Comparison with your own previous work (ipsative) |
| Rubrics with levels | A checklist of observable criteria |
| Forced group work | Independent path; collaboration optional and outside the method |
| Deadlines | Suggested time; the learner decides |

**The only purpose of verification is the learner's own knowledge:** *did I learn this, and what
exactly can I now do?*

---

## 2. What counts as evidence (qué cuenta como evidencia)

Evidence is something that runs or can be inspected, not an opinion.

| Evidence | Form | When it is required |
|---|---|---|
| **Working artifact** | Code/project that builds and runs offline | Every artifact lesson |
| **Prediction note** | What I expected, written *before* running | Every artifact lesson |
| **Prediction/reality diff** | The exact lines or behavior that differ | Every artifact lesson |
| **Acceptance report** | Criterion → pass/fail → observed output | Every artifact lesson |
| **Golden trace comparison** | Transcript diff against the reference | Every variant |
| **Change-cost table** | Files / lines / concepts / tests touched | Design lessons |
| **Reflection note** | What I learned, what I cannot explain yet, next step | Every lesson |
| **Recorded explanation** (optional) | 2–5 min, no audience required | When the learner wants to test explanation |

A screenshot, a paragraph of prose, or “I understood it” is not evidence.

---

## 3. Acceptance criteria (criterios de aceptación)

### 3.1 Rules

1. **Derived from the specification**, not from the implementation.
2. **Observable:** a command, a diff, a behavior.
3. **Binary:** pass / not yet. No percentages.
4. **Public:** written before the work starts.
5. **One concept per criterion.**
6. **No hidden criteria.** If it is not written, it is not required.

### 3.2 Template

```markdown
## Acceptance criteria — construction-09 (abstract data type)

- [ ] acceptance-criterion-1: With seed 42, the transcript matches `traces/seed-42.txt`.
- [ ] acceptance-criterion-2: Replacing the array with a linked list changes only `board.cpp`.
- [ ] acceptance-criterion-3: Every existing caller compiles without edits.
- [ ] acceptance-criterion-4: No caller indexes the board directly (grep returns zero hits).
- [ ] acceptance-criterion-5: I can explain the difference between specification and
  representation without using the word "struct".
```

### 3.3 Examples per track

| ID | Acceptance criteria (summary) |
|---|---|
| construction-02 | Trace matches; no `goto`; one entry/exit per function |
| construction-05 | Add a field; trace unchanged; list touched signatures |
| construction-07 | Same traversal over two layouts; pointer arithmetic explained |
| construction-09 | Representation swap touches one file; callers compile |
| construction-12 | Add a piece type; zero edits in the game loop |
| construction-15 | Same engine on 8×8 and 10×10; no duplicated algorithm |
| archaeology-04 | Failing test reproduces the bug; fix passes; traces unchanged |
| archaeology-09 | Opaque type; representation swap; callers unchanged |
| archaeology-18 | Refactor preserves every golden trace |

---

## 4. The engineering check (comprobación de ingeniería)

This is the real-life loop for every lesson that produces or changes an artifact, not a school
ritual. It is open book, untimed, ungraded, and the artifact is the answer.

> **Predict → Run → Compare → Explain → Record.**

| Step | What the learner does | Evidence |
|---|---|---|
| **Predict** | Writes the expected output or behavior **before** running | Prediction note |
| **Run** | Executes with a fixed seed and captures the output | Command + transcript |
| **Compare** | Diffs prediction against observation | Exact differing lines |
| **Explain** | Says *why* the gap exists and names the wrong assumption | One paragraph, no syntax words |
| **Record** | Saves prediction, diff and explanation in `EVIDENCE.md` | Portfolio entry |

**Calibration** (calibración): progress is the shrinking gap between expectation and observation. A
correct prediction means a working mental model; a wrong one points to the exact next thing to
study.

**Misuse:** predicting after seeing the result, or copying the explanation. The check only works if
the prediction is written first.

### 4.1 Verification protocol

1. **Read the criteria** before writing code.
2. **Predict** the result and write it down.
3. **Build.**
4. **Run** the acceptance script.
5. **Compare** the result with the prediction.
6. **Diagnose** if it fails: which criterion, what output, what cause?
7. **Fix and rerun.**
8. **Record** the evidence, including the prediction diff.
9. **Reflect**: what did I learn? what can I not explain yet?
10. **Decide** the next step.

No step requires another person.

---

## 5. Self-diagnosis (autodiagnóstico)

Four questions, no levels, no score:

1. **Explain:** can I describe the concept without notes and without syntax words?
2. **Change:** can I apply the change test on demand, and predict its cost?
3. **Extend:** can I use the concept in a requirement I have not seen?
4. **Calibrate:** did my prediction match? If not, which assumption was wrong?

If any answer is “not yet”, the next step is explicit:

- repeat the contrast pair,
- read the worked example again,
- go back to the prerequisite,
- or ask the material a question through an issue (optional).

There is no penalty for repetition. Repetition is the method.

---

## 6. Evidence portfolio (portafolio de evidencia)

One `EVIDENCE.md` per lesson, kept by the learner:

```markdown
# construction-09 — abstract data type

## Goal
Swap the board representation without touching callers.

## Artifact
`construction/lessons/construction-09-abstract-data-type/code/`

## Prediction
Expected: only `board.cpp` changes; the trace stays identical.
Observed: `board.cpp` and `game.h` changed; trace identical.

## Acceptance
- acceptance-criterion-1 pass — trace matches
- acceptance-criterion-2 pass — only `board.cpp` changed
- acceptance-criterion-3 pass — callers compile
- acceptance-criterion-4 pass — grep returns 0
- acceptance-criterion-5 not yet — I still confuse specification with representation

## Change cost
| Files | Lines | New concepts | Tests broken |
|---|---|---|---|
| 1 | 74 | 2 | 0 |

## Reflection
I understood the swap, but I still explain it with the word "struct".
Next: rewrite the explanation without syntax words, then construction-10.

## Next step
construction-10 — invariants.
```

The portfolio is not submitted. It is the learner's record and, later, a professional sample.

---

## 7. Feedback without a grader (retroalimentación sin calificador)

Feedback is information about the artifact, not a judgment about the person.

### 7.1 Self-feedback

Three questions:

- **Feed up:** does the artifact meet the stated goal?
- **Feed back:** which criterion fails, with what observed output?
- **Feed forward:** what is the next smallest change?

### 7.2 Optional external feedback

A mentor or peer may comment, but:

- it is not required,
- it is not graded,
- it must cite evidence (`file:line`, output, trace),
- the learner decides whether to act on it.

### 7.3 Feedback literacy

The learner practices:

- judging the work against the criteria, not against another person,
- separating the symptom from the cause,
- deciding what to ignore,
- asking for evidence when feedback is vague.

This is the professional skill; the material only provides the scaffold.

---

## 8. Progression (progresión)

- **Criterion-based:** move on when the acceptance criteria pass.
- **No fixed calendar:** suggested time only.
- **No penalty for repetition.**
- **Prerequisites are explicit**, but the learner may attempt any rung and discover the prerequisite
  by failing.
- **Done is local:** a lesson is done for *you* when you can demonstrate it, not when a syllabus
  says so.

---

## 9. What was removed and why (qué se eliminó y por qué)

| Removed | Reason |
|---|---|
| Exams | They measure recall under artificial constraints and do not resemble engineering acceptance |
| Quizzes | Same; prediction against a trace already provides retrieval with a purpose |
| Grades | They shift attention to scoring, encourage working to the grade, and invite comparison |
| Points, curves, badges | Ranking mechanisms with no learning value |
| Rubrics with levels | They become grades in disguise; binary criteria are clearer and less arbitrary |
| Checkpoints with pass/fail gates | Replaced by acceptance criteria that the learner runs |
| Forced group work | The learner may study independently; collaboration is optional and outside the method |
| Deadlines | They punish life and create artificial pressure |

**What we kept:** prediction, self-explanation, contrast, reflection, and repeated practice. These
are learning mechanisms, not assessment ceremonies.

---

## 10. Independent learner checklist — artifact lessons (lista de una persona)

- [ ] I can run the artifact offline with one command.
- [ ] I wrote a prediction before running, and I compared it with the observation.
- [ ] Every acceptance criterion passes, and I have the output.
- [ ] I can explain the concept without notes and without syntax words.
- [ ] I can apply the change test and predict its cost.
- [ ] I wrote the reflection and the next step.
- [ ] I know which lesson comes next (or why I am repeating this one).

If all boxes are checked, the lesson is complete **for you**. No one else needs to approve it.

---

## 11. For teachers, if there is one (opcional)

The material can be used in a course, but it is not designed around one.

- Use the acceptance criteria as the only assessment.
- Do not grade; give evidence-based feedback.
- Ask for the prediction before the run; the gap is the teaching moment.
- Do not require group work; if you use it, it is outside the method.
- Let learners choose the path; teach the prerequisites on demand.
- The teacher is a facilitator and a model of expert reasoning, not a grader.

---

## 12. Validity check (validez)

Acceptance criteria can be gamed. Four safeguards:

1. **Prediction:** the learner writes the expected result before running; the gap between prediction
   and observation is the calibration signal.
2. **Change test:** passing the criteria is not enough; the learner must apply a change and measure
   the cost.
3. **Explanation without syntax:** if the learner can only describe the code, the concept is not
   learned.
4. **Transfer:** apply the concept to a new requirement (the “extend” stage).

A criterion that can be passed by copying is a bad criterion. Rewrite it until it requires
understanding.

---

## 13. The one-sentence model

> Predict it, build it, run the criteria, compare with the prediction, explain the gap without
> syntax, change it, and decide what is next — at your own pace, with no grade.
